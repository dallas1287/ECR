// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MEDIACONVERTER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MEDIACONVERTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MEDIACONVERTER_EXPORTS
#define MEDIACONVERTER_API __declspec(dllexport)
#else
#define MEDIACONVERTER_API __declspec(dllimport)
#endif
#include <iostream>

enum class ErrorCode
{
	SUCCESS,
	NO_FMT_CTX,
	FMT_UNOPENED,
	NO_CODEC,
	CODEC_UNOPENED,
	NO_STREAMS,
	NO_VID_STREAM,
	NO_CODEC_CTX,
	CODEC_CTX_UNINIT,
	NO_FRAME,
	NO_PACKET,
	PKT_NOT_DECODED,
	PKT_NOT_RECEIVED,
	NO_SCALER
};

// This class is exported from the dll
class MEDIACONVERTER_API CMediaConverter {
public:
	CMediaConverter(void);
	// TODO: add your methods here.
	void dllTest() { std::cout << "This is a Dll test." << std::endl; }
	ErrorCode loadFrame(const char* filename, int& width, int& height, unsigned char** data);
};
