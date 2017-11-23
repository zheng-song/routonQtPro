#include "widget.h"
#include "unistd.h"

Widget::Widget(QWidget *parent)
    : QMainWindow(parent),currentFileName(""),
      videoSpeed(1),videoTotalTime(0),
      videoCurrTime(0),isDoubleClicked(0)
{
    setAutoFillBackground(true);
    setStyleSheet("QWidget{background-color:rgb(255,255,255);}");
    setWindowModality(Qt::WindowModal);
//    setWindowOpacity(1);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框

    openFileButton = new QPushButton(QIcon(tr(":/icon/images/openfile.png")), tr(""));
    openFileButton->setFlat(true);//设置按钮无边框
    openFileButton->setToolTip("open file");
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(slotOpenFile()));

    playButton = new QPushButton(QIcon(tr(":/icon/images/play.png")), tr(""));
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
}

Widget::~Widget()
{ }

void Widget::slotSliderReleased()
{
    qDebug()<<"videoSlider->value():"<<videoSlider->value()<<endl;
    float time =(float)(videoSlider->value()) / 100.0;
    qDebug()<<"time is: "<<time<<endl;
    player->controlCmd(QString("seek "+QString::number(time) +" 2\n" ));
}

void Widget::slotCloseAPP()
{
    player->controlCmd("quit\n");
    this->close();
}

void Widget::slotGetTimeInfo()
{
    if(playButton->toolTip() == "pause")
    {
        player->controlCmd("get_time_pos \n");
    }
}


void Widget::slotVideoStarted()
{
    qDebug()<<"video started!!!"<<endl;
    player->controlCmd("get_time_length \n");

    videoSlider->setValue(0);
    videoSlider->setSingleStep(200);
    videoSlider->setPageStep(500);

    stopButton->setEnabled(true);
    stepButton->setEnabled(true);
    backwardButton->setEnabled(true);
    muteButton->setEnabled(true);
    volUp->setEnabled(true);
    volDown->setEnabled(true);

    playButton->setIcon(QIcon(":/icon/images/pause.png"));
    playButton->setToolTip("pause");
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

    stopButton->setEnabled(false);
    stepButton->setEnabled(false);
    backwardButton->setEnabled(false);
    muteButton->setEnabled(false);
    volUp->setEnabled(false);
    volDown->setEnabled(false);
}

void Widget::slotVideoDataReceive()
{
    while( player->mplayerProcess->canReadLine() )
    {
        QString message = player->mplayerProcess->readLine();
        QStringList messageList = message.split("=");
        if(messageList[0] == "ANS_TIME_POSITION")
        {
            videoCurrTime = messageList[1].toFloat()*100;
            videoSlider->setValue(videoCurrTime);
            videoTime->start(1000);
        }
        else if(messageList[0] == "ANS_LENGTH")
        {
            videoTotalTime = messageList[1].toFloat()*100;
            videoSlider->setRange(0, videoTotalTime);
            videoSlider->setEnabled(true);
            videoTime->start(1000);
        }
    }
}


void Widget::slotVolumeUp()
{
    player->controlCmd("volume +1\n");
    if(playButton->toolTip() == "play")
    {
        playButton->setIcon(QIcon(":/icon/images/pause.png"));
        playButton->setToolTip("pause");
    }
}

void Widget::slotVolumeDown()
{
     player->controlCmd("volume -1\n");
     if(playButton->toolTip() == "play")
     {
         playButton->setIcon(QIcon(":/icon/images/pause.png"));
         playButton->setToolTip("pause");
     }
}

void Widget::slotSliderChanged(int value)
{
    qDebug()<<"void Widget::slotSliderChanged value is :"<<value<<endl;

    //TODO  将视频的播放进度跳转到此处
//    float time =(float)value / 100.0;
//    qDebug()<<"time is: "<<time<<endl;
//    player->controlCmd(QString("seek "+QString::number(time) +" 2\n" ));
}

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

            isDoubleClicked =0;
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

void Widget::slotOpenFile()
{
    currentFileName.clear();
    currentFileName = QFileDialog::getOpenFileName(this, tr("打开媒体文件"), tr("/home/zs/qtBuild/video/"),
                    tr("Video files(*.rmvb *.rm *.avi *.wmv *.mkv *.asf *.3gp *.mov *.mp4 *.ogv *.wav);; All files ( *.* );;"));
    if( !currentFileName.isEmpty() )
    {
        player->play(currentFileName);

    }
}

void Widget::slotPlay()
{
    if( currentFileName.isEmpty() )
    {
        slotOpenFile();
    }
    else
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
        player->controlCmd("pause\n");
    }
}

void Widget::slotStop()
{
    player->controlCmd("quit \n");

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
    if(videoSpeed < 5)
        videoSpeed+=1;
    player->controlCmd( QString("speed_set "+QString::number(videoSpeed)+"\n") );
}

void Widget::slotBackward()
{
    qDebug()<<"now videoSpeed is:"<<videoSpeed<<endl;
    if(videoSpeed > 0)
        videoSpeed-=1;

    player->controlCmd( QString("speed_set "+QString::number(videoSpeed)+"\n") );
}

void Widget::slotMute()
{
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
}




bool Widget::eventFilter(QObject *watched, QEvent *event)
{

}
