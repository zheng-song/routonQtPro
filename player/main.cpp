#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

int My_cmdPipeFd;

int main(int argc, char *argv[])
{
    int status;
    QApplication a(argc, argv);

    QFile fifoFile("/tmp/cmd_pipe");
    if(!fifoFile.exists())
        mkfifo("/tmp/cmd_pipe",S_IFIFO | 0777);
    My_cmdPipeFd = open("/tmp/cmd_pipe",O_WRONLY);
    if(My_cmdPipeFd == -1)
        qDebug()<<QString("open /tmp/cmd_pipe failed")<<endl;
    else
        qDebug()<<QString("open /tmp/cmd_pipe successful")<<endl;


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
    close(My_cmdPipeFd);
    return status;

}
