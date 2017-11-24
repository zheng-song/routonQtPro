#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


// if you subclass from QWidget，you need to provide a paintEvent
//for your custom QWidget.
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void paintEvent();
};

#endif // WIDGET_H
