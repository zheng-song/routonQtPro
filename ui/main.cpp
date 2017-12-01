#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setMinimumSize(720,600);
//    w.setMaximumSize(1024,800);
//    w.showMinimized();
    w.show();
    return a.exec();
}
