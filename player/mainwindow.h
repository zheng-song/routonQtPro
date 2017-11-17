#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void play_video(const QString &videoName);
    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_clicked();

private:
    QProcess    *videoPlayProcess;

    QPushButton *pushButton_open;               //打开文件
    QPushButton *pushButton_play;               //播放、暂停
    QPushButton *pushButton_silence;            //静音、取消静音
    QPushButton *pushButton_volup;              //增大音量
    QPushButton *pushButton_voldown;            //减小音量
    QPushButton *pushButton_volleft;            //左声道
    QPushButton *pushButton_volright;           //右声道
};

#endif // MAINWINDOW_H
