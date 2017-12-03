#include "mainwindow.h"
#include "finddialog.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
//    findDialog = new FindDialog;
    okButton = new QPushButton("OK",this);

    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(slotOkButtonClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotOkButtonClicked()
{
    FindDialog findDialog;
    findDialog.show();
    QEventLoop loop;
//    connect(&findDialog,SIGNAL(destroyed(QObject*)),&loop,SLOT(quit()));
    loop.exec(QEventLoop::AllEvents);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this,tr("exit application"),\
                                   tr("there have a task in performing,are you sure exit"),QMessageBox::Yes|QMessageBox::No);
    if(button == QMessageBox::Yes)
        e->accept();//accept the signal,exit the application;
    else
        e->ignore();//ignore the signal,keep going;
}
