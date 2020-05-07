#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include<mythread.h>
#include <QtCore>
extern void Uart3Tx(char *ptr, int NumByte);
extern char Uart3Rx(char *rx_buffer1);
extern char flag1,flag2,AVC_flag,tlc_flag,flag5,sensor_flag,flag7,flag_yavc,flag_ytlc,flag_ysensor;
//extern char yellow_tlc;
extern char testflag_register,testflag2;
extern char flag_avc,flag_tlc,flag_sensor;
extern char Ch;
extern int i;
extern char uartdtrx[10];
extern char uartdttx[10];
extern char testfflag;
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
   // void on_pushButton_clicked();
     MyThread *mThread;
    ~MainWindow();

public slots:
    void onMsgChanged(char *ptr);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    //void on_pushButton_6_clicked();

private:

    Ui::MainWindow *ui;
    QPushButton *m_button;

};

#endif // MAINWINDOW_H
