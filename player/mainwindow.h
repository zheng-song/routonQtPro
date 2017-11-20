#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "bottomwidget.h"
#include "mainslider.h"
#include "volumeslider.h"

#define PC
//#define ARM

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getWindowIniWidth() const {return windowIniWidth;}
    int getWindowIniHeight()const {return windowIniHeight; }

    void play_video(const QString &videoName);
    void closeEvent(QCloseEvent *);

    void setThreeButtton();

private slots:
    void slotCloseWidget();
    void slotOpenFile();
    void slotVideoWidgetMouseLeftDown();
    void slotSignalMovePoint(QPoint point);
    void slotSetValue(int value);
    void slotStopVideoBtClicked();
    void slotPlayVideoBtClicked();

private:
    QProcess        * videoPlayProcess;




    const int       screenWidth;
    const int       screenHeight;
    const int       windowIniWidth;
    const int       windowIniHeight;

    static QPoint   videoWidgetPosition;  //相应videoWidget的

    //窗体

    //右上角的三个按钮
    QPushButton     * closeButtonOf3;
    QPushButton     * minimumButtonOf3;
    QPushButton     * maximumButtonOf3;

    QLabel          * titleLabel;
    QRect           normalRect;                 //保存最大化以前的大小，因为最小化以前this->rect()已经变成了全屏时的rect()
    QWidget         * groupBox;
    QWidget         * bottomWidget;




    // 菜单
    QPushButton     * menuButton;
    QMenu           * menu;
    QAction         * openAction;


    //播放器控件
//    VideoWidget     * videoWidget0;

    QPushButton     * pushButton_play;               //播放、暂停
    QPushButton     * pushButton_stop;
//    QPushButton     * pushButton_silence;            //静音、取消静音
    QPushButton     * pushButton_volume;              //增大音量
//    QPushButton     * pushButton_voldown;            //减小音量
//    QPushButton     * pushButton_volleft;            //左声道
//    QPushButton     * pushButton_volright;           //右声道
    QPushButton     * pushButton_rateX2;
    MainSlider      * slider;
    QWidget         * volumeWidget;
    VolumeSlider    * volumeSlider;

};

#endif // MAINWINDOW_H
