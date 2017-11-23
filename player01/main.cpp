#include "widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

#ifdef PC
    w.setMinimumSize(600,400);
    w.show();
#endif

#ifdef RAM
    w.showMaximized();
#endif
    return a.exec();
}
