#include "widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setMinimumSize(600,400);
    w.show();

    return a.exec();
}
