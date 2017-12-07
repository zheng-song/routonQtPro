#include "widget.h"
#include <QtWidgets>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent),currentFileName(""),
      videoSpeed(1),
      resultFd(-1),My_cmdPipeFd(-1)
{
//    setAutoFillBackground(true);
//    setStyleSheet("QWidget{background-color:rgb(0,0,0);}");//rgb(255,255,255) white rgb(0,0,0) black
//    setAttribute(Qt::WA_TranslucentBackground,true);
//    this->setAttribute(Qt::WA_OpaquePaintEvent,true);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框

    openFileButton = new QPushButton(QIcon(tr(":/icon/images/openfile.png")), tr(""));
    openFileButton->setFlat(true);//设置按钮无边框
    openFileButton->setToolTip("open file");
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(slotOpenFile()));

    playButton = new QPushButton(QIcon(tr(":/icon/images/play.png")), tr(""));
    playButton->setEnabled(false);
    playButton->setToolTip("play");
    playButton->setFlat(true); //设置按钮无边框
    connect(playButton, SIGNAL(clicked()), this, SLOT(slotPlay()));

    stopButton = new QPushButton(QIcon(tr(":/icon/images/stop.png")), tr(""));
    stopButton->setToolTip("close");
    stopButton->setFlat(true);
    stopButton->setEnabled(false);
    connect(stopButton, SIGNAL(clicked()), this, SLOT(slotStop()));

    closeButton = new QPushButton(QIcon(":/icon/images/closeHovered.png"),"");
    closeButton->setToolTip("quit");
    closeButton->setFlat(true);
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(slotCloseAPP()));

    backwardButton = new QPushButton(QIcon(tr(":/icon/images/backward.png")), tr(""));
    backwardButton->setEnabled(false);
    backwardButton->setFlat(true);
    connect(backwardButton,SIGNAL(clicked(bool)),this,SLOT(slotBackward()));

    stepButton = new QPushButton(QIcon(tr(":/icon/images/step.png")), tr(""));
    stepButton->setEnabled(false);
    stepButton->setFlat(true);
    connect(stepButton,SIGNAL(clicked(bool)),this,SLOT(slotStep()));

    volUp = new QPushButton(QIcon( tr(":/icon/images/volumeHovered.png")), tr("") );
    volUp->setEnabled(false);
    volUp->setFlat(true);
    volUp->setToolTip("volUp");
    connect(volUp,SIGNAL(clicked(bool)),this,SLOT(slotVolumeUp()));

    volDown = new QPushButton(QIcon( tr(":/icon/images/volume.png")),tr(""));
    volDown->setEnabled(false);
    volDown->setFlat(true);
    volDown->setToolTip("volDown");
    connect(volDown,SIGNAL(clicked(bool)),this,SLOT(slotVolumeDown()));


    muteButton = new QPushButton(QIcon(tr(":/icon/images/mute.png")), tr(""));
    muteButton->setToolTip("mute-off");
    muteButton->setFlat(true);
    muteButton->setEnabled(false);
    connect(muteButton,SIGNAL(clicked(bool)),this,SLOT(slotMute()));


    videoSlider = new QSlider(Qt::Horizontal);
    videoSlider->setEnabled(false);
    connect(videoSlider, SIGNAL(valueChanged(int)),this,SLOT(slotSliderChanged(int)));
    connect(videoSlider,SIGNAL(sliderReleased()),this,SLOT(slotSliderReleased()));
    connect(videoSlider,SIGNAL(actionTriggered(int)),this,SLOT( slotStepChange(int)));
    connect(videoSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSliderMoved(int)));

    buttonLayout = new QHBoxLayout();
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

//    player->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    player->setAutoFillBackground(true);
//    player->setAttribute(Qt::WA_TranslucentBackground,true);

    label = new QLabel;


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(videoSlider);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    mplayerProcess = new QProcess;

/***********************定时1秒获取MPlayer的时间信息1次*******************************/
    videoTime = new QTimer(this);
    playVideoDelay = new QTimer(this);
    connect(videoTime,SIGNAL(timeout()),this,SLOT(slotGetTimeInfo()));
    connect(playVideoDelay,SIGNAL(timeout()),this,SLOT(slotPlayVideo()));


    //    setAutoFillBackground(true);
    //    setStyleSheet("QWidget{background-color:rgb(0,0,0);}");//rgb(255,255,255) white rgb(0,0,0) black
    //    setAttribute(Qt::WA_TranslucentBackground,true);
    //    this->setAttribute(Qt::WA_OpaquePaintEvent,true);
    //    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框
}

Widget::~Widget()
{ }

void Widget::slotOpenFile()
{
    slotPlay();
//    this->hide();
    currentFileName.clear();
    currentFileName = QFileDialog::getOpenFileName(this, tr("打开媒体文件"), tr("/home/zs/qtBuild/video/"),
                tr("Video files(*.rmvb *.rm *.avi *.wmv *.mkv *.asf *.3gp *.mov *.mp4 *.ogv *.wav);; All files ( *.* );;"));

    qDebug()<<currentFileName<<endl;
    if( !currentFileName.isEmpty() )
    {
        playVideoDelay->start(100);
        return ;
    }
//    this->repaint(0,0,this->width(),this->height());

//    this->hide();
//    this->show();
    slotPlay();
    return ;
}

void Widget::paintEvent(QPaintEvent *event)
{
    qDebug()<<"paint event"<<endl;
}

void Widget::slotStepChange(int value)
{
    videoTime->stop();
    qDebug()<<QString("slot Step Change %1").arg(value)<<endl;

#ifdef PC
    if(value == 3)
    {
        float time =(float)(videoSlider->value() + 1000) / 100.0;
        mplayerProcess->write(QString("seek "+QString::number(time) +" 2\n").toLatin1());
    }
    else if(value == 4)
    {
        float time =(float)(videoSlider->value() - 1000) / 100.0;
        mplayerProcess->write(QString("seek "+QString::number(time) +" 2\n").toLatin1());
    }

#endif

#ifdef ARM
    QString tmp;
    if( value==3 )
        tmp = "a "+QString::number(videoSlider->value()+10)+".00 \n";
    else if(value == 4)
        tmp = "a "+QString::number(videoSlider->value()-10)+".00 \n";

    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
#endif

    videoTime->start();

}

void Widget::slotGetTimeInfo()
{
    if(playButton->toolTip() == "pause")
    {
    #ifdef PC
        if(playButton->toolTip() == "pause")
        {
            mplayerProcess->write(QString("get_time_pos \n").toLatin1());
        }
    #endif

    #ifdef ARM
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
    #endif
    }
}

void Widget::slotVideoStarted()
{
    qDebug()<<"video started!!!"<<endl;

#ifdef PC
    mplayerProcess->write(QString("get_time_length \n").toLatin1());
    videoSlider->setValue(0);
    videoSlider->setSingleStep(300);
    videoSlider->setPageStep(1000);
#endif

#ifdef ARM
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
#endif

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
#ifdef PC
    float time =(float)(videoSlider->value()) / 100.0;
    qDebug()<<"time is: "<<time<<endl;
    mplayerProcess->write(QString("seek "+QString::number(time) +" 2\n").toLatin1());
#endif

#ifdef ARM
    QString tmp;
    tmp = "a "+QString::number(videoSlider->value())+".00 \n";
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
#endif

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
    connect(mplayerProcess,SIGNAL(errorOccurred(QProcess::ProcessError)),this,SLOT(slotErrorOccurred(QProcess::ProcessError)));

    QStringList args;
    QString programs;
#ifdef  PC
    programs = "mplayer";
    args << "-slave";           //使用slave模式
    args << "-quiet";           //不要输出冗余信息
    args << "-wid"<<QString::number(this->winId());
    args << "-zoom";
    args << "-vo";
    args << "x11";
#endif

#ifdef ARM
    programs = "/usr/local/bin/video";
    args <<"2";
#endif
    args << currentFileName;

    mplayerProcess->start(programs, args);
}

void Widget::slotStop()
{
#ifdef PC
    mplayerProcess->write(QString("quit \n").toLatin1());
#endif

#ifdef ARM
    mplayerProcess->kill();
#endif

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

void Widget::slotErrorOccurred(QProcess::ProcessError errorNum)
{
    qDebug() <<"video process start error, the error state is :"<<errorNum<<endl;
}

void Widget::slotPlay()
{
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
#ifdef PC
    mplayerProcess->write(QString("pause\n").toLatin1());
#endif

#ifdef ARM
    status = write(My_cmdPipeFd,"p \n",4);
    if(status != -1)
        qDebug()<<"Write p for play/pause,status is:"<<status<<endl;
    else
        qDebug()<<"write p error"<<endl;
#endif

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
        qDebug()<<"message is:"<<message<<endl;
#ifdef PC
        QStringList messageList = message.split("=");
        if(messageList[0] == "ANS_TIME_POSITION")
        {
            int videoCurrTime = messageList[1].toFloat()*100;
            videoSlider->setValue(videoCurrTime);
        }
        else if(messageList[0] == "ANS_LENGTH")
        {
            int videoTotalTime = messageList[1].toFloat()*100;
            videoSlider->setRange(0, videoTotalTime);
            videoSlider->setEnabled(true);
        }
#endif

    }
}

void Widget::slotVolumeUp()
{
    qDebug()<<"slotVolumeUp"<<endl;

#ifdef PC
    mplayerProcess->write(QString("volume +1\n").toLatin1());
#endif

#ifdef ARM
    status = write(My_cmdPipeFd,"+ \n",4);
#endif
}

void Widget::slotVolumeDown()
{
#ifdef PC
    mplayerProcess->write(QString("volume -1\n").toLatin1());
#endif

#ifdef ARM
    status = write(My_cmdPipeFd,"- \n",4);
#endif

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
#ifdef PC
    mplayerProcess->write(QString("speed_set "+QString::number(videoSpeed)+"\n").toLatin1());
#endif

#ifdef ARM
    QString tmp ="s "+QString::number(videoSpeed)+".00 \n";
    qDebug()<<"slotStep write:"<<tmp<<endl;
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
#endif
}

void Widget::slotBackward()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed > 1)
        videoSpeed-=1;
#ifdef PC
    mplayerProcess->write(QString("speed_set "+QString::number(videoSpeed)+"\n").toLatin1());
#endif

#ifdef ARM
    QString tmp ="s "+QString::number(videoSpeed)+".00 \n";
    qDebug()<<"slotStep write:"<<tmp<<endl;
    status = write(My_cmdPipeFd,(tmp.toLatin1()).data(),tmp.length());
#endif
}

void Widget::slotMute()
{
#ifdef PC
    if(muteButton->toolTip()=="mute-on")
    {
        mplayerProcess->write(QString("mute 0\n").toLatin1());
        muteButton->setToolTip("mute-off");
    }
    else
    {
        mplayerProcess->write(QString("mute 1\n").toLatin1());
        muteButton->setToolTip("mute-on");
    }
#endif

#ifdef ARM
    write(My_cmdPipeFd,"m \n",4);
#endif

}

void Widget::playVideo(const QString &fileName)
{
    if(mplayerProcess->state()==QProcess::Running)
    {
        mplayerProcess->kill();
        mplayerProcess->waitForFinished(-1);
        qDebug()<<"old process is finished"<<endl;
        delete mplayerProcess;
        mplayerProcess = new QProcess(this);
    }
    mplayerProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(mplayerProcess,SIGNAL( readyReadStandardOutput() ),this,SIGNAL(readyReadStandardOutput() ));
    connect(mplayerProcess,SIGNAL( started() ),this,SIGNAL( started() ));
    connect(mplayerProcess,SIGNAL( errorOccurred(QProcess::ProcessError)),this,SIGNAL(error(QProcess::ProcessError)));
    connect(mplayerProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SIGNAL(finished(int,QProcess::ExitStatus)));
    QStringList args;
    QString programs;
#ifdef  PC
    programs = "mplayer";
    args << "-slave";           //使用slave模式
    args << "-quiet";           //不要输出冗余信息
    args << "-wid"<<QString::number(this->winId());
    args << "-zoom";
    args << "-vo";
    args << "x11";
#endif

#ifdef ARM
    programs = "/usr/local/bin/video";
    args <<"2";
#endif
    args << fileName;

    mplayerProcess->start(programs, args);
}
