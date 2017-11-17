#include "mainwindow.h"

#define PC
//#define ARM

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    pushButton_open = new QPushButton("open video file");
    connect(pushButton_silence,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));

    pushButton_silence = new QPushButton("Silence-off");
    connect(pushButton_silence,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_silence->setFlat(true);
//    pushButton_silence->setStyleSheet("QPushButton{background-color:transparent;}");

    pushButton_play = new QPushButton("paly");
    connect(pushButton_play,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_play->setFlat(true);
//    pushButton_play->setStyleSheet("QPushButton{background-color:transparent;}");

    pushButton_volup = new QPushButton("volUp");
    connect(pushButton_volup,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_volup->setFlat(true);
//    pushButton_volup->setStyleSheet("QPushButton{background-color:transparent;}");

    pushButton_voldown = new QPushButton("volDown");
    connect(pushButton_voldown,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_voldown->setFlat(true);
//    pushButton_voldown->setStyleSheet("QPushButton{background-color:transparent;}");

    pushButton_volleft = new QPushButton("volLeft");
    connect(pushButton_volleft,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_volleft->setFlat(true);
//    pushButton_volleft->setStyleSheet("QPushButton{background-color:transparent;}");

    pushButton_volright = new QPushButton("volRight");
    connect(pushButton_volright,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
//    pushButton_volright->setFlat(true);
//    pushButton_volright->setStyleSheet("QPushButton{background-color:transparent;}");

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(pushButton_play);
    hLayout->addWidget(pushButton_volup);
    hLayout->addWidget(pushButton_voldown);
    hLayout->addWidget(pushButton_volleft);
    hLayout->addWidget(pushButton_volright);


    QGridLayout *gLayout = new QGridLayout;
    gLayout->addLayout(hLayout,0,0,Qt::AlignBottom);
    setLayout(gLayout);

//    setStyleSheet("QMainWindow{background-color:translucent;}");
    setAttribute(Qt::WA_TranslucentBackground,true);

#ifdef PC
    play_video("/home/zs/qtBuild/video/test.mp4");
#endif

#ifdef ARM
    play_video("/mnt/udisk/video/test.mp4");
#endif


}

MainWindow::~MainWindow()
{

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
        this->show();
    }
//    update();
}

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
    else if(QObject::sender() == this->pushButton_open)
    {
        QString s = QFileDialog::getOpenFileName(
                           this, "选择要播放的文件","\\",
                            "视频文件 (*.flv *.rmvb *.avi *.MP4);; 所有文件 (*.*);; ");
        if (!s.isEmpty())
        {
//            s.replace("/","");

//            videoPlayProcess->write("pause\n"); //如果在播放则先停止

            mPlayer->setFileName(s);

            videoPlayProcess->start();

        }

    }
}

void MainWindow::closeEvent(QCloseEvent *)//事件过滤器，当窗口关闭的时候，杀死进程videoPlayProcess
{
    videoPlayProcess->kill();
}
