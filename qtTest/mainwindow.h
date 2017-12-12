#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void writeThread();
    void readThread();
    void closeThread();
    void display();

private:
    Ui::MainWindow *ui;
    M_Thread *yy;
};

#endif // MAINWINDOW_H
