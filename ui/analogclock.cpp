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
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(hourColor));//绘制特殊图形，用brush
    painter.save();
    painter.rotate(30.0 * (time.hour() + time.minute()/60.0));//坐标系旋转，根据真实时间确定旋转角度
    static const QPoint hourHand[3]={
      QPoint(7,8),
      QPoint(-7,8),
      QPoint(0,-40)
    };
    painter.drawConvexPolygon(hourHand,3); //画不规则多边形
    painter.restore();//恢复save前原来的坐标系
    painter.setPen(hourColor);//画刻度线，用Pen，不用Brush
    for(int i=0; i<12; i++){
            painter.drawLine(88,0,96,0);//12小时对应的刻度
            painter.rotate(30.0);
        }

//画1-12数字，根据数学半径sin，cos计算所画text的位置
    int textR = 80;     //big ridaus
    int textW = 12;   //width = height
    int textH = 8;
    const double pi = 3.1415926;
    for(int i=0; i<12; i++){
        double angle = 30.0 * i * pi/ 180;
        int x = textR * cos(angle) - textW/2;
        int y = textR * sin(angle) - textH/2;
        //qDebug()<<i<<angle<<x<<y;
        painter.drawText(QRect(x, y, textW, textH), Qt::AlignCenter,
                         QString("%1").arg((i+3)>12?(i+3-12):(i+3)));
    }
//画分，同上
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(minuteColor));
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second()/60.0));
    static const QPoint minuteHand[3] = {
        QPoint(7,8),
        QPoint(-7,8),
        QPoint(0,-70)
    };
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    painter.setPen(minuteColor);
    for (int j=0; j<60; j++){
        if ((j%5) != 0){
            painter.drawLine(92, 0, 96, 0);
        }
        painter.rotate(6.0);
    }
//画秒，同上
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(secColor));
    painter.save();
    painter.rotate(time.second() * 6.0);
    static const QPoint secHand[3] = {
        QPoint(3,4),
        QPoint(-3,4),
        QPoint(0,-85)
    };
    painter.drawConvexPolygon(secHand, 3);
    painter.restore();
}
