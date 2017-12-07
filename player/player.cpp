#include "player.h"
#include <QtWidgets>

Player::Player(QWidget * parent)
    : QWidget(parent)
{
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

void Player::controlCmd(const QString &cmd)
{
    mplayerProcess->write(cmd.toLatin1());
}
