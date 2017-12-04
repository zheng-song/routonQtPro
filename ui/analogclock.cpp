#include "analogclock.h"
#include <QtWidgets>

m_AnalogClock::m_AnalogClock(QWidget *parent)
    :QWidget(parent)
{
    // repaint every seconds
    repaintTimer = new QTimer;
    connect(repaintTimer,SIGNAL(timeout()),this,SLOT(repaint()));// use repaint() not update(),think why.
    repaintTimer->start(1000);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}

m_AnalogClock::~m_AnalogClock()
{

}

/// Constructor【函数描述】
/// @param QPaintEvent *e
/// @return NULL
/// @remarks 注意事项:
/// @note    注意事项，功能同@remarks,显示字样不同
/// @par    自定义图块，后面可跟示例代码之类
/// @code(必须使用@endcode结束)
/// 示例代码(无需缩进)
/// @endcode
/// @see     其他参考项【产生指向参考的链接】
/// -        产生一个黑色圆点

void m_AnalogClock::paintEvent(QPaintEvent *e)
{
    QColor hourColor(127,0,127);
    QColor minuteColor(127,0,127);
    QColor secColor(127,0,127);
    int side = qMin(this->width(),this->height());
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//无锯齿
    painter.translate(this->width()/2,this->height()/2);//将坐标系原点移动到正中心
    painter.scale(side/200.0,side/200.0);
}
