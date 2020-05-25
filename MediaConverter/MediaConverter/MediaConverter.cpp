// MediaConverter.cpp : Defines the exported functions for the DLL.
//
#include "pch.h"
#include "framework.h"
#include "MediaConverter.h"

//ffmpeg includes
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

ErrorCode CMediaConverter::loadFrame(const char* filename, int& width, int& height, unsigned char** data)
{
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx)
        return ErrorCode::NO_FMT_CTX;

    if (avformat_open_input(&av_format_ctx, filename, NULL, NULL)) //returns 0 on success
        return ErrorCode::FMT_UNOPENED;

    if (av_format_ctx->nb_streams < 1)
        return ErrorCode::NO_STREAMS;

    int vid_str_idx = -1;
    AVCodec* av_codec = nullptr;
    AVCodecParameters* av_codec_params = nullptr;

    for (unsigned int i = 0; i < av_format_ctx->nb_streams; ++i)
    {
        auto stream = av_format_ctx->streams[i];
        av_codec_params = av_format_ctx->streams[i]->codecpar;

        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if (!av_codec)
            return ErrorCode::NO_CODEC;

        if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            vid_str_idx = i;
            break;
        }
    }

    if (vid_str_idx == -1)
        return ErrorCode::NO_VID_STREAM;

    AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx)
        return ErrorCode::NO_CODEC_CTX;

    if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
        return ErrorCode::CODEC_CTX_UNINIT;

    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0)
        return ErrorCode::CODEC_UNOPENED;

    AVFrame* av_frame = av_frame_alloc();
    if (!av_frame)
        return ErrorCode::NO_FRAME;

    AVPacket* av_packet = av_packet_alloc();
    if (!av_packet)
        return ErrorCode::NO_PACKET;

    int response;
    while (av_read_frame(av_format_ctx, av_packet) >= 0)
    {
        if (av_packet->stream_index != vid_str_idx)
            continue;

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0)
            return ErrorCode::PKT_NOT_DECODED;

        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            continue;
        else if (response < 0)
            return ErrorCode::PKT_NOT_RECEIVED;

        av_packet_unref(av_packet);
        break;
    }

    unsigned char* output = new unsigned char[av_frame->width * av_frame->height * 4];

    SwsContext* sws_scaler_ctx = sws_getContext(av_frame->width, av_frame->height, av_codec_ctx->pix_fmt, //input
                                                av_frame->width, av_frame->height, AV_PIX_FMT_RGB0, //output
                                                SWS_BILINEAR, NULL, NULL, NULL); //options

    if (!sws_scaler_ctx)
        return ErrorCode::NO_SCALER;

    unsigned char* dest[4] = { output, NULL, NULL, NULL };
    int dest_linesize[4] = { av_frame->width * 4, 0, 0, 0 };

    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);
    sws_freeContext(sws_scaler_ctx);

    width = av_frame->width;
    height = av_frame->height;
    *data = output;

    return ErrorCode::SUCCESS;
}

// This is the constructor of a class that has been exported.
CMediaConverter::CMediaConverter()
{
}
