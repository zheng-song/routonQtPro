#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMutex>
#include <QMutexLocker>

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
    int n;
    mutable QMutex mutex;

};

#endif // MAINWINDOW_H
