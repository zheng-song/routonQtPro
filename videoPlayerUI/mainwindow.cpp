#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent,Qt::WindowFlags f)
    : QMainWindow(parent,f)
{
    playVideo = new QPushButton("&Play",this);
    connect(playVideo,SIGNAL(clicked()),this,\
            SLOT(on_playVideo_click()));
    playVideo->move(0,450);
//    playVideo->setAutoFillBackground(true);
//    playVideo->setFlat(true);
    playVideo->setStyleSheet("QPushButton{background-color:transparent;}");

    quitAPP = new QPushButton("&Exit",this);
    connect(quitAPP,SIGNAL(clicked()),this,\
            SLOT(on_quitAPP_click()));
    quitAPP->move(720,450);

//    setStyleSheet("QMainWindow{background-color:translucent;}");
    setAttribute(Qt::WA_TranslucentBackground,true);
}

MainWindow::~MainWindow()
{
    delete gLayout;
    delete hLayout;
    delete videoProgress;
}

void MainWindow::on_playVideo_click()
{
    videoProgress = new QProcess(this);
    QStringList arguments;
    arguments<<"2"<<"test.mp4";

    videoProgress->start("/mnt/udisk/video/video",\
                         arguments);
    if(videoProgress->waitForStarted(-1))
    {
        qDebug()<<"video is started"<<endl;
        this->show();
        this->playVideo->show();
        this->quitAPP->show();
    }
}
void MainWindow::on_quitAPP_click()
{

    QApplication::exit();
}
