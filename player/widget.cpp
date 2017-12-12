#include "widget.h"
#include <QtWidgets>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent),currentFileName(""),
      videoSpeed(1),
      resultFd(-1),My_cmdPipeFd(-1)
{
    mplayerProcess = new QProcess;

    createButton();

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(openFileButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(volDown);
    buttonLayout->addWidget(backwardButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(stepButton);
    buttonLayout->addWidget(volUp);
    buttonLayout->addWidget(muteButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->setSpacing(10);
//    buttonLayout->setMargin(0);


//    label = new QLabel;
//    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    label->setAutoFillBackground(false);
//    label->setAttribute(Qt::WA_TranslucentBackground,true);
//    label->setStyleSheet("QWidget{background-color:rgba(0,0,0,255);}");


//    frame = new QFrame;
//    QVBoxLayout *vLayout = new QVBoxLayout;
//    vLayout->addWidget(videoSlider);
//    vLayout->addLayout(buttonLayout);
//    vLayout->addSpacing(0);
//    frame->setLayout(vLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(videoSlider);
//    mainLayout->addWidget(frame);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


/***********************定时1秒获取MPlayer的时间信息1次*******************************/
    videoTime = new QTimer(this);
    playVideoDelay = new QTimer(this);
    connect(videoTime,SIGNAL(timeout()),this,SLOT(slotGetTimeInfo()));
    connect(playVideoDelay,SIGNAL(timeout()),this,SLOT(slotPlayVideo()));
    clearFbTimer = new QTimer;
    connect(clearFbTimer,SIGNAL(timeout()),this,SLOT(slotClearFb()));

//    setAutoFillBackground(true);
//    setAttribute(Qt::WA_TranslucentBackground,true);
//    setStyleSheet("QWidget{background-color:rgba(100,100,100,255);}");//rgb(255,255,255) white rgb(0,0,0) black
//    setAttribute(Qt::WA_OpaquePaintEvent,true);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框
}

Widget::~Widget()
{ }

void Widget::slotOpenFile()
{
    currentFileName.clear();
    currentFileName = QFileDialog::getOpenFileName(this, tr("打开媒体文件"), tr("."),
                tr("Video files(*.rmvb *.rm *.avi *.wmv *.mkv *.asf *.3gp *.mov *.mp4 *.ogv *.wav);; All files ( *.* );;"));

    if( !currentFileName.isEmpty() )
    {
        playVideoDelay->start(100);
        return ;
    }
    else
    {
        clearFbTimer->start(1);
    }
    return ;
}

void Widget::slotStepChange(int value)
{
    videoTime->stop();
    qDebug()<<QString("slot Step Change %1").arg(value)<<endl;

    QString tmp;
    if( value==3 )
        tmp = "a "+QString::number(videoSlider->value()+10)+".00 \n";
    else if(value == 4)
        tmp = "a "+QString::number(videoSlider->value()-10)+".00 \n";

    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());

    videoTime->start();

}

void Widget::slotGetTimeInfo()
{
    if(playButton->toolTip() == "pause")
    {
        status = write(My_cmdPipeFd,"c \n",4);
        if(status == -1)
            qDebug()<<"write My_cmdPipeFd c for get current time ERROR !!!!"<<endl;

        char buffer[128];
        status = read(resultFd,(void *)&buffer,sizeof(buffer));
        if(status == -1)
            qDebug()<<"read resultFd for get current time ERROR !!!!"<<endl;

        int time =(int)( ((QString)(QLatin1String((char *)&buffer))).toFloat() );
//        qDebug()<<"current time is :"<<(QString)(QLatin1String((char *)&buffer))<<"time is:"<<time<<endl;

        videoSlider->setValue(time);
    }
}

void Widget::slotVideoStarted()
{
    qDebug()<<"video started!!!"<<endl;

    int i =0;

    if(access("/tmp/cmd_pipe", F_OK) == -1)
        mkfifo("/tmp/cmd_pipe",S_IFIFO | 0777);
    i = 0;

    do{
        My_cmdPipeFd = open("/tmp/cmd_pipe",O_WRONLY);
        i++;
    }while(My_cmdPipeFd<0 && i<50);
    if(My_cmdPipeFd == -1)
    {
        QMessageBox::warning(this,"Warning",\
                                     "open pipe /tmp/My_cmdPipeFd failed,quit now!!!",QMessageBox::Ok);
        this->close();
    }

    qDebug()<<QString("open /tmp/cmd_pipe successful")<<endl;


    if(access("/tmp/cmd_result", F_OK) == -1)
        mkfifo("/tmp/cmd_result",S_IFIFO | 0777);
    i = 0;
    do{
        resultFd = open("/tmp/cmd_result",O_RDONLY);
        i++;
    }while(resultFd<0 &&  i<50);
    if(resultFd == -1)
    {
        QMessageBox::warning(this,"Warning",\
                                     "open pipe /tmp/cmd_result failed,quit now!!!",QMessageBox::Ok);
        this->close();
    }
    qDebug()<<QString("open /tmp/cmd_result successful")<<endl;

    status = write(My_cmdPipeFd,"t \n",4);
    char buffer[128];
    status = read(resultFd,(void *)&buffer,sizeof(buffer));
    int totalTime =(int)( ((QString)(QLatin1String((char *)&buffer))).toFloat() + 1);

    qDebug()<<"total time is :"<<(QString)(QLatin1String((char *)&buffer))\
           <<"totalTime is:"<<totalTime<<endl;

    videoSlider->setRange(0,totalTime);
    videoSlider->setValue(0);
    videoSlider->setSingleStep(5);
    videoSlider->setPageStep(10);

    videoSlider->setEnabled(true);
    stopButton->setEnabled(true);
    stepButton->setEnabled(true);
    backwardButton->setEnabled(true);
    muteButton->setEnabled(true);
    volUp->setEnabled(true);
    volDown->setEnabled(true);
    playButton->setEnabled(true);

    playButton->setIcon(QIcon(":/icon/images/pause.png"));
    playButton->setToolTip("pause");

    videoTime->start(1000);
}

void Widget::slotSliderReleased()
{
    qDebug()<<"videoSlider->value():"<<videoSlider->value()<<endl;
    videoTime->start();
    QString tmp;
    tmp = "a "+QString::number(videoSlider->value())+".00 \n";
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
}

void Widget::slotSliderMoved(int)
{
    videoTime->stop();//暂时终止计时器,在用户拖动过程中不修改slider的值
}

void Widget::slotPlayVideo()
{
    playVideoDelay->stop();

    if(mplayerProcess->state()==QProcess::Running)
    {
        mplayerProcess->kill();
        mplayerProcess->waitForFinished(-1);
        qDebug()<<"old process is finished"<<endl;
        delete mplayerProcess;
        mplayerProcess = new QProcess(this);
    }
    mplayerProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(mplayerProcess,SIGNAL(started()),this,SLOT(slotVideoStarted()));
    connect(mplayerProcess,SIGNAL(finished(int,QProcess::ExitStatus)),\
            this,SLOT(slotVideoFinished(int,QProcess::ExitStatus)));
    connect(mplayerProcess,SIGNAL(readyReadStandardOutput()),\
            this,SLOT(slotVideoDataReceive()));

    QStringList args;
    QString programs;
    programs = "/usr/local/bin/video";
    args <<"2";
    args << currentFileName;
    mplayerProcess->start(programs, args);
}

void Widget::slotStop()
{
    mplayerProcess->kill();

    currentFileName.clear();
    playButton->setIcon(QIcon(":/icon/images/play.png"));
    playButton->setToolTip("play");

    videoSlider->setEnabled(false);
    videoSlider->setValue(0);
    stopButton->setEnabled(false);
    stepButton->setEnabled(false);
    backwardButton->setEnabled(false);
    muteButton->setEnabled(false);
    volUp->setEnabled(false);
    volDown->setEnabled(false);
}

void Widget::slotCloseAPP()
{
    mplayerProcess->write(QString("quit \n").toLatin1());
    this->close();
}

void Widget::slotPlay()
{
    if(playButton->isEnabled()){
        if(playButton->toolTip() == "pause")
        {
            playButton->setIcon(QIcon(":/icon/images/play.png"));
            playButton->setToolTip("play");
        }
        else if(playButton->toolTip() == "play")
        {
            playButton->setIcon(QIcon(":/icon/images/pause.png"));
            playButton->setToolTip("pause");
        }
        status = write(My_cmdPipeFd,"p \n",4);
        if(status != -1)
            qDebug()<<"Write p for play/pause,status is:"<<status<<endl;
        else
            qDebug()<<"write p error"<<endl;
    }
}

void Widget::slotVideoFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() <<"video play finished the exitCode is :"<<exitCode\
           <<"the exit status is "<<exitStatus<<endl;

    videoSlider->setEnabled(false);
    videoSlider->setValue(0);
    playButton->setIcon(QIcon(":/icon/images/play.png"));
    playButton->setToolTip("play");

    playButton->setEnabled(false);
    stopButton->setEnabled(false);
    stepButton->setEnabled(false);
    backwardButton->setEnabled(false);
    muteButton->setEnabled(false);
    volUp->setEnabled(false);
    volDown->setEnabled(false);
}

void Widget::slotVideoDataReceive()
{
    while( mplayerProcess->canReadLine() )
    {
        QString message = mplayerProcess->readLine();
//        qDebug()<<"message is:"<<message<<endl;
    }
}

void Widget::slotVolumeUp()
{
    qDebug()<<"slotVolumeUp"<<endl;
    status = write(My_cmdPipeFd,"+ \n",4);
}

void Widget::slotVolumeDown()
{
    status = write(My_cmdPipeFd,"- \n",4);
}

void Widget::slotSliderChanged(int value)
{
//    qDebug()<<"current slider value is :"<<value<<endl;
}

void Widget::slotStep()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed < 4)
        videoSpeed+=1;
    QString tmp ="s "+QString::number(videoSpeed)+".00 \n";
    qDebug()<<"slotStep write:"<<tmp<<endl;
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
}

void Widget::slotBackward()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed > 1)
        videoSpeed-=1;
    QString tmp ="s "+QString::number(videoSpeed)+".00 \n";
    qDebug()<<"slotStep write:"<<tmp<<endl;
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
}

void Widget::slotMute()
{
    write(My_cmdPipeFd,"m \n",4);
}

void Widget::createButton()
{
    openFileButton = new QPushButton(QIcon(tr(":/icon/images/openfile.png")), tr(""));
    playButton = new QPushButton(QIcon(tr(":/icon/images/play.png")), tr(""));
    stopButton = new QPushButton(QIcon(tr(":/icon/images/stop.png")), tr(""));
    closeButton = new QPushButton(QIcon(":/icon/images/closeHovered.png"),"");
    backwardButton = new QPushButton(QIcon(tr(":/icon/images/backward.png")), tr(""));
    stepButton = new QPushButton(QIcon(tr(":/icon/images/step.png")), tr(""));
    volUp = new QPushButton(QIcon( tr(":/icon/images/volumeHovered.png")), tr(""));
    volDown = new QPushButton(QIcon( tr(":/icon/images/volume.png")),tr(""));
    muteButton = new QPushButton(QIcon(tr(":/icon/images/mute.png")), tr(""));

    openFileButton->setFlat(true);//设置按钮无边框
    playButton->setFlat(true); //设置按钮无边框
    stopButton->setFlat(true);
    closeButton->setFlat(true);
    backwardButton->setFlat(true);
    stepButton->setFlat(true);
    volUp->setFlat(true);
    volDown->setFlat(true);
    muteButton->setFlat(true);

    openFileButton->setToolTip("open file");
    playButton->setToolTip("play");
    stopButton->setToolTip("close");
    closeButton->setToolTip("quit");
    volUp->setToolTip("volUp");
    volDown->setToolTip("volDown");
    muteButton->setToolTip("mute-off");

    playButton->setEnabled(false);
    stopButton->setEnabled(false);
    backwardButton->setEnabled(false);
    stepButton->setEnabled(false);
    volUp->setEnabled(false);
    volDown->setEnabled(false);
    muteButton->setEnabled(false);

    connect(openFileButton, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(slotPlay()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(slotStop()));
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(slotCloseAPP()));
    connect(backwardButton,SIGNAL(clicked(bool)),this,SLOT(slotBackward()));
    connect(stepButton,SIGNAL(clicked(bool)),this,SLOT(slotStep()));
    connect(volUp,SIGNAL(clicked(bool)),this,SLOT(slotVolumeUp()));
    connect(volDown,SIGNAL(clicked(bool)),this,SLOT(slotVolumeDown()));
    connect(muteButton,SIGNAL(clicked(bool)),this,SLOT(slotMute()));

    videoSlider = new QSlider(Qt::Horizontal);
    videoSlider->setEnabled(false);
    connect(videoSlider, SIGNAL(valueChanged(int)),this,SLOT(slotSliderChanged(int)));
    connect(videoSlider,SIGNAL(sliderReleased()),this,SLOT(slotSliderReleased()));
    connect(videoSlider,SIGNAL(actionTriggered(int)),this,SLOT( slotStepChange(int)));
    connect(videoSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSliderMoved(int)));
}

void Widget::slotClearFb()
{
    clearFbTimer->stop();

    qDebug()<<"clear ui"<<endl;
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    long int screensize = 0;
    char *fbp = 0;
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd)
    {
        qDebug("Error: cannot open framebuffer device.\n");
        return ;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        qDebug("Error reading variable information.\n");
        return ;
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if(fbp == NULL)
    {
        qDebug("Error: failed to map framebuffer device to memory.\n");
        return;
    }

    for(int i = 0;i<screensize;i++)
    {
        if( i%2 == 1)
        {
            *(fbp+i)=0;
        }
        else
        {
            *(fbp+i)=1;
        }
    }

    munmap(fbp, screensize);
//    close(fbfd);

    return ;
}
