#include "widget.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

extern int close(int);

Widget::Widget(QWidget *parent)
    : QMainWindow(parent),currentFileName(""),
      videoSpeed(1),
      fifoWriteFile("/tmp/cmd_pipe"),
      fifoReadFile("/tmp/cmd_result")
{
    setAutoFillBackground(true);
    setStyleSheet("QWidget{background-color:rgb(255,255,255);}");
//    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框

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

    player = new Player();
    player->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    player->setAutoFillBackground(true);
    player->setAttribute(Qt::WA_TranslucentBackground,true);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(player);
    mainLayout->addWidget(videoSlider);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    connect(player,SIGNAL(started()),this,SLOT(slotVideoStarted()));
    connect(player,SIGNAL(finished(int,QProcess::ExitStatus)),\
            this,SLOT(slotVideoFinished(int,QProcess::ExitStatus)));
    connect(player,SIGNAL(readyReadStandardOutput()),\
            this,SLOT(slotVideoDataReceive()));

/***********************定时1秒获取MPlayer的时间信息1次*******************************/
    videoTime = new QTimer(this);
    connect(videoTime,SIGNAL(timeout()),this,SLOT(slotGetTimeInfo()));
    videoTime->start(1000);

    if(!fifoWriteFile.exists())
        mkfifo("/tmp/cmd_pipe",S_IFIFO | 0777);
//    if(!fifoWriteFile.open(QIODevice::ReadWrite | QIODevice::Text))
//    {
//        qDebug()<<"open /tmp/cmd_pipe fifo failed!quit now."<<endl;
//    }
}

Widget::~Widget()
{

    fifoWriteFile.close();
//    fifoReadFile.close();

}

void Widget::slotVideoStarted()
{
    qDebug()<<"video started!!!"<<endl;

#ifdef PC
    player->controlCmd("get_time_length \n");

    videoSlider->setValue(0);
    videoSlider->setSingleStep(200);
    videoSlider->setPageStep(500);
#endif

#ifdef ARM

    if(!fifoWriteFile.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug()<<"open /tmp/cmd_pipe fifo failed!"<<endl;
    else
        qDebug()<<"open /tmp/cmd_pipe fifo successful!"<<endl;

    QString tmp = "t \n";
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write get video total time error")<<endl;


//    if(!fifoReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
//        qDebug()<<"open /tmp/cmd_result failed";
//    else
//        qDebug()<<"open /tmp/cmd_result successful";
//    char buffer[20];
//    memset(buffer,0,sizeof(buffer));
//    length = fifoReadFile.readLine((char *)&buffer,sizeof(buffer));
//    if (length == -1)
//    {
//        qDebug()<<"read /tmp/cmd_result error"<<endl;
//    }
//    else
//    {
//        qDebug()<<QString("read number is:%1").arg(length);
//    }
//    float totalTime = ((QString)(QLatin1String((char *)&buffer))).toFloat();
//    qDebug()<<"total time is :"<<(QString)(QLatin1String((char *)&buffer))<<"totalTime is:"<<totalTime<<endl;

//    videoSlider->setRange(0,totalTime);
    videoSlider->setValue(0);
    videoSlider->setSingleStep(5);
    videoSlider->setPageStep(15);
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

void Widget::slotGetTimeInfo()
{
    if(playButton->toolTip() == "pause")
    {
    #ifdef PC
        if(playButton->toolTip() == "pause")
        {
            player->controlCmd("get_time_pos \n");
        }
    #endif

    #ifdef ARM

        QString tmp = "c \n";
        length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
        if(length == -1)
            qDebug()<<QString("write get video current time error")<<endl;

//        if(!fifoReadFile.open(QIODevice::ReadOnly))
//        {
//            qDebug()<<"open /tmp/cmd_result failed";
//        }

//        qDebug()<<"open /tmp/cmd_result failed";
//        char buffer[20];
//        length = fifoReadFile.read((char *)&buffer,sizeof(buffer));
//        if (length == -1)
//        {
//            qDebug()<<"read /tmp/cmd_result error"<<endl;
//        }

//        int time = (int)(((QString)(QLatin1String((char *)&buffer))).toFloat());
//        qDebug()<<"current time is :"<<(QString)(QLatin1String((char *)&buffer))<<"time is:"<<time<<endl;

//        fifoWriteFile.close();
//        fifoReadFile.close();

//        videoSlider->setValue(time);
    #endif

    }
}


void Widget::slotSliderReleased()
{
    qDebug()<<"videoSlider->value():"<<videoSlider->value()<<endl;
#ifdef PC
    float time =(float)(videoSlider->value()) / 100.0;
    qDebug()<<"time is: "<<time<<endl;
    player->controlCmd(QString("seek "+QString::number(time) +" 2\n" ));
#endif

#ifdef ARM
    QString tmp = "a "+QString::number(videoSlider->value()) + ".00 \n";
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write slotSliderReleased error")<<endl;
//    else
//        qDebug()<<QString("write slotSliderReleased OK")<<endl;
#endif

}

void Widget::slotCloseAPP()
{

    player->controlCmd("quit\n");
    this->close();
}


void Widget::slotVideoFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() <<"视频播放完毕 the exitCode is :"<<exitCode\
           <<"the exit status is "<<exitStatus<<endl;
    videoSlider->setEnabled(false);
    videoSlider->setValue(0);
    playButton->setIcon(QIcon(":/icon/images/play.png"));
    playButton->setToolTip("play");
    currentFileName.clear();

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
    qDebug()<<"slotVideoDataReceive";

    while( player->mplayerProcess->canReadLine() )
    {
        QString message = player->mplayerProcess->readLine();
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
        QStringList messageList = message.split(" ");
        if(messageList[0] == "chenrui," && messageList[1] == "current" && messageList[2] == "time" && messageList[3] == "is")
        {
            int videoCurrTime = (int)(messageList[4].toFloat());
            qDebug()<<"videoCurrentTime is:"<<videoCurrTime<<endl;
//            videoSlider->setValue(videoCurrTime);
        }
#ifdef ARM

#endif
    }
}


void Widget::slotVolumeUp()
{
    qDebug()<<"slotVolumeUp"<<endl;

#ifdef PC
    player->controlCmd("volume +1\n");
#endif

#ifdef ARM
    QString tmp = "+ \n";
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write slotVolumeUp error")<<endl;
    else
        qDebug()<<QString("write slotVolumeUp OK")<<endl;

//    status = write(My_cmdPipeFd,"+ \n",4);
#endif
}

void Widget::slotVolumeDown()
{
#ifdef PC
     player->controlCmd("volume -1\n");
#endif

#ifdef ARM
     QString tmp = "- \n";
     length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
     if(length == -1)
         qDebug()<<QString("write slotVolumeDown error")<<endl;
     else
         qDebug()<<QString("write slotVolumeDown OK")<<endl;

//    status = write(My_cmdPipeFd,"- \n",4);
#endif

}

void Widget::slotSliderChanged(int value)
{
    qDebug()<<"void Widget::slotSliderChanged value is :"<<value<<endl;
}
/*
void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"in Widget:: mouseDoubleClickEvent!!!!"<<endl;


    if(event->buttons() == Qt::LeftButton)
    {
        qDebug()<<"1"<<endl;
        isDoubleClicked++;
        if(isDoubleClicked == 1)
        {
            qDebug()<<"2"<<endl;
//            timer->start(300);
        }

        if(isDoubleClicked == 2)
        {
            qDebug()<<"3"<<endl;

            isDoubleClicked =0;status = write(My_cmdPipeFd,"p",1);
            if(status != -1)
                qDebug()<<"Write p for play/pause,status is:"<<status<<endl;
            else
                qDebug()<<"write p error"<<endl;
//            timer->stop();
            if(this->isFullScreen())
            {
                qDebug()<<"4"<<endl;
                this->showMinimized();
            }
            else
            {
                qDebug()<<"5"<<endl;
                this->showFullScreen();
            }
        }
    }
}
*/

void Widget::slotOpenFile()
{
    currentFileName = QFileDialog::getOpenFileName(this, tr("打开媒体文件"), tr("/home/zs/qtBuild/video/"),
                    tr("Video files(*.rmvb *.rm *.avi *.wmv *.mkv *.asf *.3gp *.mov *.mp4 *.ogv *.wav);; All files ( *.* );;"));
    if( !currentFileName.isEmpty() )
    {
        player->play(currentFileName);
    }
}

void Widget::slotPlay()
{
    if( !currentFileName.isEmpty() )
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
        player->controlCmd("pause\n");
#endif

#ifdef ARM
        QString tmp = "p \n";
        length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
        if(length == -1)
            qDebug()<<QString("write slotPlay error")<<endl;
        else
            qDebug()<<QString("write slotPlay OK")<<endl;

//        status = write(My_cmdPipeFd,"p \n",4);
#endif
    }
}

void Widget::slotStop()
{
#ifdef PC
    player->controlCmd("quit\n");
#endif

#ifdef ARM
    player->mplayerProcess->kill();
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

void Widget::slotStep()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed < 4)
        videoSpeed+=1;
#ifdef PC
    player->controlCmd( QString("speed_set "+QString::number(videoSpeed)+"\n") );
#endif

#ifdef ARM
    QString tmp = "s "+QString::number(videoSpeed)+"\n";
    qDebug()<<"slotStep write:"<<tmp<<endl;
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write speed %1 error").arg(videoSpeed)<<endl;
    else
        qDebug()<<QString("write speed s %1").arg(videoSpeed)<<endl;
#endif
}

void Widget::slotBackward()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed > 1)
        videoSpeed-=1;
#ifdef PC
    player->controlCmd( QString("speed_set "+QString::number(videoSpeed)+"\n") );
#endif

#ifdef ARM
    QString tmp = "s "+QString::number(videoSpeed)+"\n";
    qDebug()<<"slotBackward write:"<<tmp<<endl;
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write speed %1 error").arg(videoSpeed)<<endl;
    else
        qDebug()<<QString("write speed s %1").arg(videoSpeed)<<endl;
//    char *tmp = (((QString)("s "+QString::number(videoSpeed) + "\n")).toLatin1()).data();
//    status = write(My_cmdPipeFd,tmp,sizeof(tmp));
#endif
}

void Widget::slotMute()
{
#ifdef PC
    if(muteButton->toolTip()=="mute-on")
    {
        player->controlCmd("mute 0\n");
        muteButton->setToolTip("mute-off");
    }
    else
    {
        player->controlCmd("mute 1\n");
        muteButton->setToolTip("mute-on");
    }
#endif

#ifdef ARM
    QString tmp = "m \n";
    length = fifoWriteFile.write(tmp.toLatin1() , tmp.length());
    if(length == -1)
        qDebug()<<QString("write mute error")<<endl;
    else
        qDebug()<<QString("write mute ok")<<endl;
//    write(My_cmdPipeFd,"m \n",4);
#endif

}

