#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0,\
               Qt::WindowFlags f = Qt::WindowFlags());
    ~MainWindow();

private slots:
    void on_playVideo_click();
    void on_quitAPP_click();

private:
    QPushButton *playVideo;
    QPushButton *quitAPP;
    QVBoxLayout *hLayout;
    QGridLayout *gLayout;

    QProcess *videoProgress;
};

#endif // MAINWINDOW_H
