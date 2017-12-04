#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 改变缺省消息处理程序的输出
//    qSetMessagePattern("Message:%{message} File:%{file} Line:%{line} Function:%{function} DateTime:%{time [yyyy-MM-dd hh:mm:ss ddd]}");
    qSetMessagePattern("%{message} :%{file} Line:%{line}");
    MainWindow w;
    w.show();
    return a.exec();
}
