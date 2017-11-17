#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog =new QProgressDialog("get data","cancel",0,5,this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setMinimumDuration(0);
    dialog->setValue(0);
}

MainWindow::~MainWindow()
{
    delete dialog;
    delete ui;
}
