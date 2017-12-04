#include "mainwindow.h"
#include "finddialog.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
    okButton = new QPushButton("OK",this);
    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(slotOkButtonClicked()));
    clock = new m_AnalogClock(this);
//    clock->show();
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(clock);
    hlayout->addWidget(okButton);
    this->setLayout(hlayout);
    this->resize(sizeHint().width(),sizeHint().height());
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
