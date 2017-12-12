#ifndef THREAD_H
#define THREAD_H
#include <QThread>

class M_Thread:public QThread
{
    Q_OBJECT
public:
    M_Thread();

    char buf[128];
    volatile bool stopped;
    volatile bool write_rs;
    volatile bool read_rs;

signals:
    void finished();

protected:
    virtual void run();

};




#endif // THREAD_H
