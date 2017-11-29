#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QtWidgets>
#include "player.h"


class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
//    bool eventFilter(QObject *watched, QEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *);

protected slots:
    void slotPlay(); 		// 播放
    void slotStop(); 		// 停止
    void slotStep(); 		// 步进
    void slotBackward(); 	// 后退
    void slotMute();        // 静音
    void slotOpenFile();

    void slotSliderChanged(int value);
    void slotVolumeDown();
    void slotVolumeUp();


    void slotVideoDataReceive();
    void slotVideoStarted();
    void slotVideoFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotGetTimeInfo();
    void slotCloseAPP();
    void slotSliderReleased();

//    void slotSliderMoved(int position);

private:
    Player * player;

    QPushButton * openFileButton;
    QPushButton * playButton;
    QPushButton * stopButton;
    QPushButton * stepButton;
    QPushButton * backwardButton;
    QPushButton * muteButton;
    QPushButton * volUp;
    QPushButton * volDown;
    QPushButton * doubleSpeed;
    QSlider     * videoSlider;
    QPushButton * closeButton;

    QHBoxLayout * buttonLayout;

    QTimer      * videoTime;
    QString currentFileName;

    int       videoSpeed;
    int         isDoubleClicked;
    int         status;
    int         resultFd;
};

#endif // WIDGET_H
