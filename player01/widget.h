#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QtWidgets>
#include "player.h"

#define PC
//#define ARM

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);

protected slots:
    void slotPlay(); 		// 播放
    void slotStop(); 		// 停止
    void slotStep(); 		// 步进
    void slotBackward(); 	// 后退
    void slotMute();        // 静音

    void slotOpenFile();

    void slotStarted();
    void slotError(QProcess::ProcessError error);
    void slotFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotBackMessage();


private:
    Player * player;

    QPushButton * openFileButton;
    QPushButton * playButton;
    QPushButton * stopButton;
    QPushButton * stepButton;
    QPushButton * backwardButton;
    QPushButton * muteButton;

    QSlider     * videoSlider;
    QHBoxLayout * buttonLayout;

    QString currentFileName;

    QTimer      * timer;

    int         isDoubleClicked;
};

#endif // WIDGET_H
