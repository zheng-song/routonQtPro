#include "player.h"
#include <QtWidgets>

Player::Player(QWidget * parent)
    : QWidget(parent),isDoubleClick(0)
{

    renderTarget = new QWidget(this);
    renderTarget->setAttribute(Qt::WA_OpaquePaintEvent);
    timer = new QTimer;
    mplayerProcess = new QProcess(this);

}

Player::~Player()
{}


void Player::play(const QString &fileName)
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
    connect(mplayerProcess,SIGNAL(finished(int,QProcess::ExitStatus)),\
            this,SIGNAL(finished(int,QProcess::ExitStatus)));
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
    args << fileName;
    mplayerProcess->start(programs, args);
}


void Player::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"in mouseDoubleClickEvent!!!!"<<endl;

    if(event->buttons() == Qt::LeftButton)
    {
        isDoubleClick++;
        if(isDoubleClick == 1)
        {
            timer->start(300);
        }

        if(isDoubleClick == 2)
        {
            qDebug()<<"3"<<endl;

            isDoubleClick =0;
            timer->stop();
            if(this->isFullScreen())
            {
                this->showMinimized();
            }
            else
            {
                this->showFullScreen();
            }
        }
    }
}



void Player::controlCmd(const QString &cmd)
{
    mplayerProcess->write(cmd.toLatin1());
}

void Player::resizeEvent(QResizeEvent * event)
{
    renderTarget->resize(event->size());
}
