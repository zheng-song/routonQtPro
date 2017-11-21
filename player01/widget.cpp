#include "widget.h"

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
{
    openFileButton = new QPushButton(QIcon(tr(":/icon/images/openfile.png")), tr(""));
    openFileButton->setToolTip("open file");
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(slotOpenFile()));

    playButton = new QPushButton(QIcon(tr(":/icon/images/play.png")), tr(""));
    playButton->setToolTip("play");
    connect(playButton, SIGNAL(clicked()), this, SLOT(slotPlay()));

    stopButton = new QPushButton(QIcon(tr(":/icon/images/stop.png")), tr(""));
    stopButton->setToolTip("quit");
    connect(stopButton, SIGNAL(clicked()), this, SLOT(slotStop()));

    backwardButton = new QPushButton(QIcon(tr(":/icon/images/backward.png")), tr(""));
    connect(backwardButton,SIGNAL(clicked(bool)),this,SLOT(slotBackward()));

    stepButton = new QPushButton(QIcon(tr(":/icon/images/step.png")), tr(""));
    connect(stepButton,SIGNAL(clicked(bool)),this,SLOT(slotStep()));


    muteButton = new QPushButton(QIcon(tr(":/icon/images/vocal.png")), tr(""));
    muteButton->setToolTip("mute-off");
    connect(muteButton,SIGNAL(clicked(bool)),this,SLOT(slotMute()));


    videoSlider = new QSlider(Qt::Horizontal);
    videoSlider->setRange(0,100);
    videoSlider->setValue(50);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openFileButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(backwardButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(stepButton);
    buttonLayout->addWidget(muteButton);

    player = new Player();
    player->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(player, SIGNAL(started()), this, SLOT(slotStarted()));
    connect(player, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotError(QProcess::ProcessError)));
    connect(player, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));
    connect(player, SIGNAL(readyReadStandardOutput()), this, SLOT(slotBackMessage()));


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(player);
    mainLayout->addWidget(videoSlider);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);

    resize(600, 400);
    setCentralWidget(centralWidget);

    isDoubleClicked =0;
    timer = new QTimer;
}

Widget::~Widget()
{ }

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
            timer->start(300);
        }

        if(isDoubleClicked == 2)
        {
            qDebug()<<"3"<<endl;

            isDoubleClicked =0;
            timer->stop();
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
    currentFileName = "";
    currentFileName = QFileDialog::getOpenFileName(this, tr("打开媒体文件"), tr("/home/zs/qtBuild/video/"),
                    tr("Video files(*.rmvb *.rm *.avi *.wmv *.mkv *.asf *.3gp *.mov *.mp4 *.ogv *.wav);; All files ( *.* );;"));
    if( !currentFileName.isEmpty() )
    {
        player->play(currentFileName);
        playButton->setIcon(QIcon(":/icon/images/pause.png"));
        playButton->setToolTip("pause");
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
    player->controlCmd("quit\n");
    currentFileName = "";
    playButton->setIcon(QIcon(":/icon/images/play.png"));
    playButton->setToolTip("play");
}

void Widget::slotStep()
{
    player->controlCmd("volume +1\n");
}

void Widget::slotBackward()
{
    player->controlCmd("volume -1\n");
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

void Widget::slotStarted()
{
    qDebug() << "lplayer started...";
}

void Widget::slotError(QProcess::ProcessError /*error*/)
{

}

void Widget::slotFinished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    qDebug() <<"视频播放完毕";
//    player->play(currentFileName);
}

void Widget::slotBackMessage()
{
}


bool Widget::eventFilter(QObject *watched, QEvent *event)
{

}
