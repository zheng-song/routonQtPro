#include "thread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termio.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <QDebug>

#define BAUDRATE B9600
#define RS_DEVICE "/dev/ttyS0"
//#define RS_DEVICE "/dev/ttySAC1"

M_Thread::M_Thread()
{}

void M_Thread::run()
{
    int fd, c=0, res;
    struct termios oldtio,newtio; //termios结构是用来保存波特率、字符大小等
    qDebug("start...\n");
    fd = open(RS_DEVICE,O_RDWR|O_NOCTTY);//以读写方式打开串口。不控制TTY
    if(fd<0)
    {
        perror("error");
        exit(1);                        //失败退出
    }
    qDebug()<<"Open...\n"<<endl;
    tcgetattr(fd,&oldtio);              //保存当前设置到oldtio
    bzero(&newtio,sizeof(newtio));     //清除newtio结构，并重新对它的成员设置如下
    newtio.c_cflag=BAUDRATE|CS8|CLOCAL|CREAD;  //9600、8位、忽略DCD信号、启用接收装置
    newtio.c_iflag|=IGNPAR;                    //忽略奇偶
    newtio.c_oflag=0;
    newtio.c_lflag=0;
    newtio.c_cc[VMIN]=0;
    newtio.c_cc[VTIME]=100;                   //在规定时间(VTIME)内读取(VMIN)个字符;
    tcflush(fd,TCIFLUSH);                    //清除所有队列在串口的输入与输出；
    tcsetattr(fd,TCSANOW,&newtio);           //把我们的设置写入termios

    while(stopped)                          //stopped为0时将退出线程
    {
        if(write_rs)                           //write_rs为1时把字符串从串口中输出
        {
            write_rs=0;
            write(fd,"QtEmbedded-4.5.1",16);  //向串口中写入字符，通过串口调试助手可看到QtEmbedded-4.5.1这个字符
        }
        if(read_rs)                           //read_rs为1时读取，并存在buf
        {
            read_rs=0;
            res=read(fd,buf,10);     //读取串口的数据到buf
            buf[res]=0;
            emit finished();                      //读完后发一个信号
        }
    }
    qDebug("Close...\n");
    tcsetattr(fd,TCSANOW,&oldtio);      //重新设置回原来的
    close(fd);
    quit();
}
