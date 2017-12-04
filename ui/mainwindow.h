#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMutex>
#include <QMutexLocker>
#include "analogclock.h"

class FindDialog;
class QPushButton;

class MainWindow:public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();

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
