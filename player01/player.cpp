#include <QtGui>

#include "player.h"

Player::Player(QWidget * parent)
    : QWidget(parent)
{

    renderTarget = new QWidget(this);
    renderTarget->setAttribute(Qt::WA_OpaquePaintEvent);
    timer = new QTimer;
    isDoubleClick = 0;

    mplayerProcess = new QProcess(this);
    connect(mplayerProcess, SIGNAL(started()), this, SIGNAL(started()));
    connect(mplayerProcess, SIGNAL(error(QProcess::ProcessError)), this, SIGNAL(error(QProcess::ProcessError)));
    connect(mplayerProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SIGNAL(finished(int, QProcess::ExitStatus)));
    connect(mplayerProcess, SIGNAL(readyReadStandardOutput()), this, SIGNAL(readyReadStandardOutput()));
}

Player::~Player()
{}


void Player::play(const QString &fileName)
{
    if(mplayerProcess->state() == QProcess::Running)
    {
        qDebug()<<"close before media"<<endl;
        mplayerProcess->close();
    }

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
    emit started();
}

void Player::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"in mouseDoubleClickEvent!!!!"<<endl;

    if(event->buttons() == Qt::LeftButton)
    {
        qDebug()<<"1"<<endl;
        isDoubleClick++;
        if(isDoubleClick == 1)
        {
            qDebug()<<"2"<<endl;
            timer->start(300);
        }

        if(isDoubleClick == 2)
        {
            qDebug()<<"3"<<endl;

            isDoubleClick =0;
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


void Player::controlCmd(const QString &cmd)
{
    mplayerProcess->write(cmd.toLatin1());
}

void Player::resizeEvent(QResizeEvent * event)
{
    renderTarget->resize(event->size());
}

/*
void Player::slotStarted(const QString &fileName)
{
    qDebug() << "mplayer started ...";

}
*/

//void Player::slotError(QProcess::ProcessError /*error*/)
//{
//	qDebug() << "player error.";
//}
//void Player::slotFinished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
//{
//	qDebug() << "mplayer finished.";
//}
/*
void Player::slotBackMessage()
{
    while( mplayerProcess->canReadLine() )
    {
        QString message(mplayerProcess->readLine());
        // message 即为读取的信息，我们可以根据需要取我们要的信息如：
        // 文件总时间为：ANS_LENGTH=23.00
        // 当前时间为：ANS_TIME_POSITION=23.00
        qDebug() << message;
    }
}
*/
