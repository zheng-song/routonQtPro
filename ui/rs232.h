#ifndef RS232_H
#define RS232_H


//#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include <QWidget>

class QPushButton;
class QTextEdit;
class QSerialPort;

class m_RS232:public QWidget
{
public:
    m_RS232(QWidget *parent = 0);
    ~m_RS232();

public slots:
    void receiveDate();
    void on_openPortBt_clicked();
    void on_closePortBt_clicked();
    void on_sendDataBt_clicked();

private:
    QPushButton * openPortBt;
    QPushButton * closePortBt;
    QPushButton * sendDataBt;
    QTextEdit   * receiveEdit;
    QTextEdit   * sendEdit;

//    QSerialPort *mySerialPort;
    QByteArray requestData;
};

#endif // RS232_H
