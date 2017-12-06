#ifndef WIDGET_H
#define WIDGET_H

#include "player.h"
#include <QMainWindow>

class QPushButton;
class QSlider;
class QHBoxLayout;

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

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
    void slotErrorOccurred(QProcess::ProcessError errorNum);
    void slotGetTimeInfo();
    void slotCloseAPP();
    void slotSliderReleased();
    void slotSliderMoved(int);
    void slotStepChange(int);
    void slotPlayVideo();

private:
//    void paintEvent(QPaintEvent *);


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
    QTimer      * playVideoDelay;
    QString currentFileName;

    int         videoSpeed;
    int         isDoubleClicked;
    int         status;
    int         resultFd;
    int         My_cmdPipeFd;
};

#endif // WIDGET_H
