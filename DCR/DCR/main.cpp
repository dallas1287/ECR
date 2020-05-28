#include "DigitalComicReader.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
    QApplication a(argc, argv);
    DigitalComicReader w;
    w.centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    w.setMouseTracking(true);
    w.showMaximized();
    return a.exec();
}
