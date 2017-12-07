#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QProcess>

//#define PC
#define ARM

class Player : public QWidget
{
    Q_OBJECT

public:
    Player(QWidget * parent = 0);
    ~Player();

    void play(const QString &fileName);
    void controlCmd(const QString &cmd);
    QProcess    * mplayerProcess;

signals:
    void started();
    void finished(int, QProcess::ExitStatus);
    void readyReadStandardOutput();
    void error(QProcess::ProcessError);

private:
    QWidget     * renderTarget;
};

#endif // PLAYER_H
