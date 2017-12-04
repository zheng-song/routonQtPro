#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char *argv[])
{
    int status;
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

    status = a.exec();
    return status;

}
