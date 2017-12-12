#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//     QFrame  * frame  =   new  QFrame;
//     QPushButton  *  button0  =   new  QPushButton(frame);
//     QPushButton  *  button1  =   new  QPushButton(frame);
//     QPushButton  *  button2  =   new  QPushButton(frame);
//     QPushButton  *  button3  =   new  QPushButton(frame);
//     QPushButton  *  button4  =   new  QPushButton(frame);
//     QPushButton  *  button5  =   new  QPushButton(frame);

//     frame -> setObjectName( "myframe" );
//     frame -> resize( 400 , 700 );
//     frame -> setStyleSheet( " QFrame#myframe{border-image:url(/home/zs/app/1.jpg)}");

//     button0 -> setGeometry( 60 , 150 , 68 , 68 );
//     button1 -> setGeometry( 160 , 150 , 68 , 68 );
//     button2 -> setGeometry( 260 , 150 , 68 , 68 );
//     button3 -> setGeometry( 60 , 280 , 68 , 68 );
//     button4 -> setGeometry( 160 , 280 , 68 , 68 );
//     button5 -> setGeometry( 260 , 280 , 68 , 68 );

//     QIcon icon;
//     QPixmap pixmap0( "/home/zs/app/3.png" );
//     icon.addPixmap(pixmap0);
//     button0 -> setIcon(icon);
//     button0 -> setIconSize(QSize( 68 , 68 ));
//     button0 -> setFixedSize(pixmap0.size());
//     button0 -> setMask(pixmap0.mask());


//     QPixmap pixmap1( "/home/zs/app/2.png" );
//     icon.addPixmap(pixmap1);
//     button1 -> setIcon(icon);
//     button1 -> setIconSize(QSize( 68 , 68 ));
//     button1 -> setFixedSize(pixmap1.size());
//     button1 -> setMask(pixmap1.mask());


//     QPixmap pixmap2( "/home/zs/app/4.png" );
//     icon.addPixmap(pixmap2);
//     button2 -> setIcon(icon);
//     button2 -> setIconSize(QSize( 68 , 68 ));
//     button2 -> setFixedSize(pixmap2.size());
//     button2 -> setMask(pixmap2.mask());

//     QPixmap pixmap3( "/home/zs/app/4.png" );
//     icon.addPixmap(pixmap3);
//     button3 -> setIcon(icon);
//     button3 -> setIconSize(QSize( 68 , 68 ));
//     button3 -> setFixedSize(pixmap3.size());
//     button3 -> setMask(pixmap3.mask());


//     QPixmap pixmap4( "/home/zs/app/2.png" );
//     icon.addPixmap(pixmap4);
//     button4 -> setIcon(icon);
//     button4 -> setIconSize(QSize( 68 , 68 ));
//     button4 -> setFixedSize(pixmap4.size());
//     button4 -> setMask(pixmap4.mask());


//     QPixmap pixmap5( "/home/zs/app/3.png" );
//     icon.addPixmap(pixmap5);
//     button5 -> setIcon(icon);
//     button5 -> setIconSize(QSize( 68 , 68 ));
//     button5 -> setFixedSize(pixmap5.size());
//     button5 -> setMask(pixmap5.mask());


//     frame -> show();

    return a.exec();
}
