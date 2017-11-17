#include <QPushButton>
#include <QApplication>
#include <QDebug>
#include <unistd.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPushButton *pushBt = new QPushButton();
    pushBt->setText("test");
    pushBt->show();

    qDebug()<<pushBt->size().width();
    return a.exec();
}
