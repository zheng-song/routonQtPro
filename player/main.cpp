#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("qrc:/icon/img/appIcon.png"));
    MainWindow w;

#ifdef PC
    int width = QApplication::desktop()->availableGeometry().width();
    int height = QApplication::desktop()->availableGeometry().height();
    w.setGeometry((width-w.getWindowIniWidth())/2,(height-w.getWindowIniHeight())/2,\
                  w.getWindowIniWidth(),w.getWindowIniHeight());
    w.show();
#endif

#ifdef ARM
    w.showMaximized();
#endif

    return a.exec();
}
