/**@brief   this is analog clock head file
 * @file    analogclock.h
 * @author  zs_hust
 * @version 1.0
 * @date    2017/12/04
 * @note    used to show a analog clock
 * @since   2017/12/04
 */

#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H
#include <QWidget>

class m_AnalogClock:public QWidget
{
    Q_OBJECT

public:
    explicit m_AnalogClock(QWidget *parent = 0);
    ~m_AnalogClock();

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer *repaintTimer; /*!< timer to start a paint event */
};


#endif // ANALOGCLOCK_H
