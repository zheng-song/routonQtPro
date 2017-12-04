#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include "rs232.h"

m_RS232::m_RS232(QWidget *parent)
    :QWidget(parent)
{
    openPortBt = new QPushButton;
    closePortBt = new QPushButton;
    sendDataBt = new QPushButton;
    receiveEdit = new QTextEdit;
    sendEdit = new QTextEdit;
//    mySerialPort = new QSerialPort;

    connect(openPortBt,SIGNAL(clicked()),this,SLOT(on_openPortBt_clicked()));
    connect(closePortBt,SIGNAL(clicked()),this,SLOT(on_closePortBt_clicked()));
    connect(sendDataBt,SIGNAL(clicked()),this,SLOT(on_sendDataBt_clicked()));
//    connect(mySerialPort,SIGNAL(readyRead()),this,SLOT(receiveDate()));

    QVBoxLayout *vLayout1 = new QVBoxLayout;
    vLayout1->addWidget(openPortBt);
    vLayout1->addWidget(closePortBt);
    vLayout1->addStretch();

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(receiveEdit);
    hLayout2->addLayout(vLayout1);

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(sendEdit);
    hLayout1->addWidget(sendDataBt);
    hLayout1->addStretch();

    QVBoxLayout *vLayout2 = new QVBoxLayout;
    vLayout2->addLayout(hLayout2);
    vLayout2->addLayout(hLayout1);
    this->setLayout(vLayout2);
}

m_RS232::~m_RS232(){}


void m_RS232::receiveDate()
{

}

void m_RS232::on_openPortBt_clicked()
{

}

void m_RS232::on_closePortBt_clicked()
{

}

void m_RS232::on_sendDataBt_clicked()
{

}
