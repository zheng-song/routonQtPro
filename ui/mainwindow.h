#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QCheckBox>
#include <QLabel>
#include <QScrollBar>
#include <QTreeView>

class MainWindow:public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);

private:
    QLabel *label1;
    QCheckBox *checkBox1;

    QLabel *label2;
    QCheckBox *checkBox2;

    QLabel *label3;
    QCheckBox *checkBox3;

    QLabel *label4;
    QCheckBox *checkBox4;

    QLabel *label5;
    QCheckBox *checkBox5;

    QTreeView *fileList;
};

#endif // MAINWINDOW_H
