#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

#ifdef PC
    w.setMinimumSize(600,400);
    w.show();
    qDebug()<<"in PC"<<endl;

#endif

#ifdef ARM
    w.showMaximized();
    qDebug()<<"in ARM"<<endl;
#endif
    return a.exec();
}
