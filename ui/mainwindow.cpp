#include "mainwindow.h"
#include "finddialog.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
    findDialog = new FindDialog;
    okButton = new QPushButton("OK",this);

    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(slotOkButtonClicked()));
}

MainWindow::~MainWindow()
{
    if(findDialog->close())
        qDebug()<<"close widget ok";
    delete findDialog;
}

void MainWindow::slotOkButtonClicked()
{
    findDialog->show();
}
