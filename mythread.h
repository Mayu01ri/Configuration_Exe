


#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void run();
    bool stop=0;

signals:
    void MsgChanged(char *);

public slots:


};

#endif // MYTHREAD_H
