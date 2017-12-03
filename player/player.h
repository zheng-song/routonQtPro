#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QProcess>

#define PC
//#define ARM

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
    void error(QProcess::ProcessError);
    void finished(int, QProcess::ExitStatus);
    void readyReadStandardOutput();

protected:
    void resizeEvent(QResizeEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    QWidget     * renderTarget;
    QTimer      *timer;
    int         isDoubleClick;
};

#endif // PLAYER_H
