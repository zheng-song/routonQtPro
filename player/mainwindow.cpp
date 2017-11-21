#include "mainwindow.h"

QPoint MainWindow::videoWidgetPosition(QPoint(0,0));

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      screenWidth(QApplication::desktop()->availableGeometry().width()),
      screenHeight(QApplication::desktop()->availableGeometry().height()),
      windowIniWidth(screenWidth*29/48),
      windowIniHeight(screenHeight*20/27)
{
    this->setAutoFillBackground(true);
    this->setStyleSheet("QWidget{background-color:rgb(60,65,68);}");
    setWindowOpacity(1);         //设置透明度
    setWindowModality(Qt::NonModal);
    setMinimumHeight(500);
    setMinimumWidth(720);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint/*WindowSystemMenuHint*/); // 设置成无边框对话框
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    normalRect.setWidth(windowIniWidth);
    normalRect.setHeight(windowIniHeight);
    setThreeButtton();


    //左上角的菜单
    menuButton = new QPushButton("File",this);
    menuButton->setGeometry(10,10,70,23);
    menuButton->setStyleSheet("QPushButton{ border-radius:8px; border: none; background-color:rgb(120,125,128);}"
                              "QPushButton:hover{background-color:rgb(200,200,201);} "
                              "QPushButton:checked{background-color:rgb(0,0,1);}");
    menu = new QMenu;
    openAction = menu->addAction(tr("Open"));
    menu->setStyleSheet(" QMenu{border:none } "
                        "QMenu::item{border:none; font-size:13px;  padding-left:20px; background-color:white; height:25px; width:50px; }"
                        "QMenu::item:selected{ background-color:rgb(140,140,240)}");
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(slotOpenFile()));
    menuButton->setMenu(menu);


    //用于放置底部的所有按钮
    bottomWidget = new BottomWidget(this);
    bottomWidget->setFixedHeight(120);
    bottomWidget->setGeometry(0, normalRect.height()-80, normalRect.width()-201, 80);
    //bottomWidget->setStyleSheet("QWidget{background-color:rgba(30,35,38,0.6))}");
    bottomWidget->setStyleSheet("QWidget{background-color:rgb(40,40,40)}");
    bottomWidget->setCursor(QCursor(Qt::ArrowCursor));
    //connect(bottomWidget, &BottomWidget::signalMovePoint, this, &Widget::slotSignalMovePoint );
    connect(bottomWidget, SIGNAL(signalMovePoint(QPoint)), this, \
            SLOT(slotSignalMovePoint(QPoint)));
    connect(bottomWidget, SIGNAL(signalLeftPressDown(bool)), this, \
            SLOT(slotVideoWidgetMouseLeftDown()));

    pushButton_play = new QPushButton(bottomWidget);
    pushButton_play->setGeometry(120,20,50,50);
    pushButton_play->setToolTip("play");
    pushButton_play->setStyleSheet("QPushButton{image:url(:icon/img/play.png);  border-radius:25px; background-color: rgb(40,40,40)}"
                                   "QPushButton::hover{background-color: rgb(90,90,90); image:url(:/icon/img/playHovered.png)}");
    connect(pushButton_play, SIGNAL(clicked(bool)), this, SLOT(slotPlayVideoBtClicked()));


    pushButton_stop = new QPushButton(bottomWidget);
    pushButton_stop->setGeometry(20,25,40,40);
    pushButton_stop->setToolTip("pause");
    pushButton_stop->setStyleSheet("QPushButton{image:url(:icon/img/stop.png); border-radius:20px; background-color: rgb(40,40,40)}"
                                   "QPushButton::hover{background-color: rgb(90,90,90);image:url(:/icon/img/stopHovered.png);}");
    connect(pushButton_stop, SIGNAL(clicked(bool)), this, SLOT(slotStopVideoBtClicked()));

    //滑动条
    slider = new MainSlider(Qt::Horizontal, bottomWidget);
    slider->setRange(0,0);
    slider->setGeometry(10, 7, normalRect.width()-225, 13);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(slotSetValue(int)));


    //音量按钮
    pushButton_volume = new QPushButton(bottomWidget);
    pushButton_volume->setGeometry(bottomWidget->width()-80,35, 30,30);
    pushButton_volume->setStyleSheet("QPushButton{image:url(:/icon/img/volume.png);border-radius:15px; background-color: rgb(40,40,40)}"
                                    "QPushButton::hover{image:url(:/icon/img/volumeHovered.png);border-color:1px black; background-color:rgb(90,90,90);}");
    pushButton_volume->installEventFilter(this);
    volumeWidget = new QWidget(this);
    volumeWidget->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint); // 设置成无边框对话框
    volumeWidget->setStyleSheet("background-color:rgb(25, 38, 58)");
    volumeWidget->setFixedWidth(30);
    volumeWidget->installEventFilter(this);   //安装事件过滤器
    volumeSlider = new VolumeSlider(Qt::Vertical, this);
    volumeSlider->setRange(0,100);
    volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));

    QVBoxLayout *volumeLayout =  new QVBoxLayout;
    volumeLayout->addWidget(volumeSlider);
    volumeWidget->setLayout(volumeLayout);
    volumeWidget->hide();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotStopVideoBtClicked()
{
    if(videoPlayProcess->state() == QProcess::Running)
    {
        videoPlayProcess->write("pause\n");
    }
}

void MainWindow::slotPlayVideoBtClicked()
{
    if(videoPlayProcess->state() == QProcess::Running) //音量按钮
        videoPlayProcess->write("pause\n");
    if(videoPlayProcess->state() == QProcess::NotRunning)
        videoPlayProcess->write("pause\n");
}

void MainWindow::slotSetValue(int value)
{
    qDebug()<<"设置视频播放的进度 value is :"<<value<<endl;
    //mediaPlayer0->setPosition(value);
}

//使用videoWidget0移动窗体
void MainWindow::slotSignalMovePoint(QPoint point)
{
    qDebug()<<"connect(bottomWidget, SIGNAL(signalMovePoint(QPoint)), this, \
              SLOT(slotSignalMovePoint(QPoint))) :: triggered "<<endl;
}

//再videoWidget0上按下鼠标左键
void MainWindow::slotVideoWidgetMouseLeftDown()
{
    qDebug()<<" connect(bottomWidget, SIGNAL(signalLeftPressDown(bool)), this, \
              SLOT(slotVideoWidgetMouseLeftDown(bool))) :: triggered "<<endl;
     videoWidgetPosition=this->pos();
}

void MainWindow::slotCloseWidget()
{
    videoPlayProcess->close();
    // 杀死创建的进程，结束本程序
    this->close();
}

void MainWindow::slotOpenFile()
{
    QString s = QFileDialog::getOpenFileName(
                       this, "选择要播放的文件",/*QDir::currentPath()*/"/home/zs/qtBuild/video/",
                        "视频文件 (*.flv *.rmvb *.avi *.mp4 *.wav);; 所有文件 (*.*);; ");
    if (!s.isEmpty())
    {
        play_video(s);
    }

}



void MainWindow::setThreeButtton()
{
    closeButtonOf3 = new QPushButton(this);
    minimumButtonOf3 = new QPushButton(this);
    maximumButtonOf3 = new QPushButton(this);

    closeButtonOf3->setFixedSize(30,20);
    minimumButtonOf3->setFixedSize(30,20);
    maximumButtonOf3->setFixedSize(30,20);

    int a = normalRect.width();
    closeButtonOf3->setGeometry(a-34,10,28,20);         //将关闭按钮放在最右上角
    maximumButtonOf3->setGeometry(a-68,10, 28, 20);
    minimumButtonOf3->setGeometry(a-102, 10, 28, 20);   //按钮之间间隔6px

    closeButtonOf3->setStyleSheet("QPushButton{image:url(:/icon/img/close.png); \
border-radius:8px; background-color:rgb(60,65,68);}"\
    "QPushButton:hover{image:url(:/icon/img/close.png);background-color:rgb(120,120,120);}");

    minimumButtonOf3->setStyleSheet("QPushButton{image:url(:/icon/img/minimize.png);\
 border-radius:8px; background-color:rgb(60,65,68);}"\
    "QPushButton:hover{image:url(:/icon/img/minimizeHovered.png); background-color:rgb(120,120,120);}");

    maximumButtonOf3->setStyleSheet("QPushButton{image:url(:/icon/img/maximize.png);\
 border-radius:8px; background-color:rgb(60,65,68);}"\
    "QPushButton:hover{image:url(:/icon/img/maximizeHovered.png); background-color:rgb(120,120,120);}");

    closeButtonOf3->setToolTip("close");
    minimumButtonOf3->setToolTip("minimize");
    maximumButtonOf3->setToolTip("maxmize");
    connect(closeButtonOf3,SIGNAL(clicked()),this,SLOT(slotCloseWidget()/*close()*/));
    connect(minimumButtonOf3,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(maximumButtonOf3,SIGNAL(clicked()),this,SLOT(showMaximized()));
}



void MainWindow::play_video(const QString &videoName)
{
    videoPlayProcess = new QProcess(this);
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
    programs = "/mnt/udisk/video/video";
    args <<"2";
#endif
    args<<videoName;

    videoPlayProcess->start(programs,args);

    if(videoPlayProcess->waitForStarted(-1))
    {
        qDebug()<<"video player is started"<<endl;
        this->pushButton_play->setText("pause");
        this->show();
    }
}

/*
void MainWindow::on_pushButton_clicked()
{
    if(QObject::sender() == this->pushButton_play)
    {

        qDebug()<<"play is clicked"<<endl;
        videoPlayProcess->write("pause\n");
        if(pushButton_play->text() == "play")
        {
            pushButton_play->setText("pause");
        }
        else
        {
            pushButton_play->setText("play");
        }
    }
    else if(QObject::sender() == this->pushButton_silence)
    {
        qDebug()<<"pushButton_silence is clicked"<<endl;
        if(pushButton_silence->text() == "Silence-off")
        {
            videoPlayProcess->write("mute 1\n");
            pushButton_silence->setText("Silence-on");
        }
        else
        {
            videoPlayProcess->write("mute 0\n");
            pushButton_silence->setText("Silence-off");
        }
    }
    else if(QObject::sender() == this->pushButton_volup)
    {
        qDebug()<<"pushButton_volup is clicked"<<endl;
        videoPlayProcess->write("volume 1\n");
    }
    else if(QObject::sender() == this->pushButton_voldown)
    {
        qDebug()<<"pushButton_voldown is clicked"<<endl;
        videoPlayProcess->write("volume -1\n");
    }
    else if(QObject::sender() == this->pushButton_volleft)
    {
        qDebug()<<"pushButton_volleft is clicked"<<endl;
        videoPlayProcess->write("balance -1 1\n");
    }
    else if(QObject::sender() == this->pushButton_volright)
    {
        qDebug()<<"pushButton_volright is clicked"<<endl;
        videoPlayProcess->write("balance 1 1\n");
    }
}
*/

void MainWindow::closeEvent(QCloseEvent *)//事件过滤器，当窗口关闭的时候，杀死进程videoPlayProcess
{
    videoPlayProcess->kill();
}
