
#include "mythread.h"
#include<QtCore>

MyThread::MyThread(QObject *parent) :QThread(parent)
{
}
/********  This thread used for some GUI update fro Qt Thread***********/
void MyThread::run()
{
    extern unsigned char blnMessageReadyToSend;
    extern char TextArray[100];

    while(1)
    {
        QMutex mutex;
        mutex.lock();
        if(this->stop)
            break;
        if(blnMessageReadyToSend)
        {
            blnMessageReadyToSend=0;
            emit MsgChanged(TextArray);
        }
          mutex.unlock();
    }
}
/***********************************************************************/
