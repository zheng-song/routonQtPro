#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class FindDialog;
class QPushButton;

class MainWindow:public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();

private slots:
    void slotOkButtonClicked();

private:
    FindDialog *findDialog;
    QPushButton *okButton;
};

#endif // MAINWINDOW_H
