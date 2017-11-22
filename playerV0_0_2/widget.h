#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include "ui_widget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QTime int_to_time(int);

public slots:
    void play_pause_slots(); //暂停
    void stop_slots(); //停止
    void previous_slots(); //上一曲
    void next_slots(); //下一曲
    void seek_slots(int);
    void get_time_slots(); //得到播放时间
    void set_volume_slots(int); //设置音量
    void set_sound_slots(); //静音
    void playerReward_slots(); //快退
    void playerForward_slots(); //快进
    void back_message_slots();//更新显示信息


private:
    Ui::Widget      * ui;

    QProcess        * process;
    QString         file_name;
    QTimer          * timer;
    QStringList     files;
    QDir            directory;
    QLabel          label_3;
    int             file_count;
    int             file_length;
    int             curr_time;
    bool            isPlay;
    bool            isSound;
    bool            isStop;

    QSlider         horizontalSilder;

};

#endif // WIDGET_H
