#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMutex>
#include <QMutexLocker>
#include "analogclock.h"

class QAction;
class QLabel;
class SpreadSheet;
class FindDialog;
class QPushButton;

class MainWindow:public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();

//重新实现closeEvent()函数,询问关闭时是否需要保存操作,并且可以把用户此次对软件的设置进行保存.
    void closeEvent(QCloseEvent *e);
    void increment(){QMutexLocker locker(&mutex);}

private slots:
    void slotOkButtonClicked();

private:
//    FindDialog *findDialog;
    QPushButton *okButton;
    m_AnalogClock *clock;

    int n;
    mutable QMutex mutex;

};

#endif // MAINWINDOW_H
