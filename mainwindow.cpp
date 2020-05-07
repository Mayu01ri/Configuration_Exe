#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QtCore>
#include <QMessageBox>
#include <unistd.h>
#include<mythread.h>
//char uartdtrx[10]={0};
char uartdttx[10]="xyz";
char VersionArr1[100]="Profiler Configuration Tool";
char VersionArr[100];
char testfflag;

 //struct stat st1 = {0};

void TextMessageGui(const char* format, ...);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   // sprintf(VersionArr,"%s:%s",VERSION,laneidArr);

   //char Programname;
    //QMessageBox::information(this, "Programname", VersionArr);


    ui->setupUi(this);
    sprintf(VersionArr,"%s",VersionArr1);
    this->setWindowTitle(VersionArr);
    ui->pushButton->setText("Register Next COM");
    ui->pushButton->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_2->setText("AVC_COM");
    ui->pushButton_2->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_3->setText("TLC_COM");
    ui->pushButton_3->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_4->setText("Sensor_COM");
    ui->pushButton_4->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton_5->setText("Reset All");

  //  onMsgChanged("fg");
     mThread=new MyThread(this);
    connect(mThread,SIGNAL(MsgChanged(char *)),this,SLOT(onMsgChanged(char *)));
    // m_button = new QPushButton("My Button", this);
    // connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
     //connect(mThread,SIGNAL(MsgChanged(char *)),this,SLOT(onMsgChanged(char *)));
//       ui->pushButton->setText("SEND");
//       ui->pushButton->setStyleSheet("background-color: rgb(255,0,0);");


       //connect(ui->pushButton, SIGNAL (released()), this, SLOT (on_pushButton_clicked()));
       //connect(mThread,SIGNAL(MsgChanged(char *)),this,SLOT(t(char *)));
       //connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
       // m_button->setText("123");
    if(AVC_flag)
    {
         ui->pushButton_2->setStyleSheet("background-color: rgb(255,255,0);");
    }

  /*  else if(flag3==0)
    {
         ui->pushButton_2->setStyleSheet("background-color: rgb(0,255,0);");
    }*/

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //int counter = 0;
   // ui->pushButton->setStyleSheet("background-color: rgb(0,255,0);");
    // flag1=1;

   // if(testflag_register)
    //{
      //system("sudo cp /home/kent/code/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules");
    if(testfflag==0)
    {
        system("sudo mv /etc/udev/rules.d/99-usb-serial.rules /home/kent/code/99-usb-serial.rules");
        ui->pushButton->setText("Connect Next COM");
        ui->pushButton->setStyleSheet("background-color: rgb(255,0,0);");
        testfflag=1;
    }
    else
    {
        system("sudo mv /home/kent/code/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules ");
        ui->pushButton->setText("set");
        ui->pushButton->setStyleSheet("background-color: rgb(0,255,0);");
        testfflag=0;
    }
    //}
    if(flag_avc)
    {
        QMessageBox::about(this,"Title","AVC_COM get registered!!");
        flag_avc=0;

    }
    if(flag_tlc)
    {
        QMessageBox::about(this,"Title","TLC_COM get registered!!");
        flag_tlc=0;
    }
    if(flag_sensor)
    {
        QMessageBox::about(this,"Title","Sensor_COM get registered!!");
        flag_sensor=0;
    }


     // if (!(stat("/dev/ttyUSB_sensor", &st1) == -1))

     // ui->pushButton->set
     //printf("\nButton clicked *** \n");

    //ui->textEdit->append("Hello you pressed button");
   // QPixmap pix("/home/kent/code/Images_backup/Real_image/test.png");
   // ui->label_2->setPixmap(pix.scaled(300,450,Qt::KeepAspectRatio));//300,450
    // m_button->setText("123");


    // ui->pushButton->setText("STOP");

  // if(!flag1)
  //{


       // Uart3Tx(uartdttx,3);

       // ui->textBrowser->append(uartdttx);


//       ui->pushButton->setStyleSheet("background-color: rgb(255,0,0);");
//       ui->pushButton->setText("STOP");
//       //ui->textBrowser->setText(uartdttx);
//       // printf("\nDebug Mode Off");

//        flag1=0;
   // }
   // else
    //{

//        //printf("\nDebug Mode On");
//        ui->pushButton->setStyleSheet("background-color: rgb(0,255,0);");
//        //ui->pushButton->setText("SEND");
//        Uart3Tx(uartdttx,3);
//        ui->textBrowser->append(uartdttx);
//        //sleep(1);
//        flag1=1;
   // }
}
void MainWindow::on_pushButton_2_clicked()
{

    AVC_flag=1;


    //flag8=1;
    // ui->pushButton_2->setStyleSheet("background-color: rgb(255,255,0);");
   // if(flag3==0)
    //{
       // ui->pushButton_2->setStyleSheet("background-color: rgb(255,255,0);");
    //}

}
void MainWindow::on_pushButton_3_clicked()
{
    tlc_flag=1;
    //ui->pushButton_3->setStyleSheet("background-color: rgb(255,255,0);");

}
void MainWindow::on_pushButton_4_clicked()
{
    sensor_flag=1;
    //ui->pushButton_4->setStyleSheet("background-color: rgb(255,255,0);");
}


void MainWindow::on_pushButton_5_clicked()
{
    system("sudo rm /home/kent/code/99-usb-serial.rules");
    system("sudo rm /etc/udev/rules.d/99-usb-serial.rules");
    ui->pushButton->setStyleSheet("background-color: rgb(255,0,0);");
    ui->pushButton->setText("Register All");
    QMessageBox::about(this,"Title","Please Register Again!!");

}
/*void MainWindow::on_pushButton_6_clicked()
{


    if(testfflag==1)
    {
        system("sudo mv /etc/udev/rules.d/99-usb-serial.rules /home/kent/code/99-usb-serial.rules");
        testfflag=0;
    }
    else
    {
        system("sudo mv /etc/udev/rules.d/99-usb-serial.rules /home/kent/code/99-usb-serial.rules");
        testfflag=1;


    }

}*/


void MainWindow::onMsgChanged(char *ptr)
{

     ui->textBrowser->append(ptr);
     if(AVC_flag)
     {
         ui->pushButton_2->setStyleSheet("background-color: rgb(0,255,0);");

     }
     if(AVC_flag==0)
     {
         ui->pushButton_2->setStyleSheet("background-color: rgb(255,0,0);");
     }
     if(flag2)
     {
          ui->pushButton_2->setStyleSheet("background-color: rgb(0,255,0);");
     }
     else if(flag2==0)
     {
          ui->pushButton_2->setStyleSheet("background-color: rgb(255,0,0);");
     }
     if(flag5)
     {
          ui->pushButton_3->setStyleSheet("background-color: rgb(0,255,0);");
     }
     else if(flag5==0)
     {
          ui->pushButton_3->setStyleSheet("background-color: rgb(255,0,0);");
     }
     if(flag7)
     {
         ui->pushButton_4->setStyleSheet("background-color: rgb(0,255,0);");
     }
     else if(flag7==0)
     {
         ui->pushButton_4->setStyleSheet("background-color: rgb(255,0,0);");
     }
     if(flag_yavc)
     {
          ui->pushButton_2->setStyleSheet("background-color: rgb(255,255,0);");
         // TextMessageGui("flag8=%d",flag8);
     }
     if(flag_ytlc)
     {
         ui->pushButton_3->setStyleSheet("background-color: rgb(255,255,0);");
     }
     if(flag_ysensor)
     {
         ui->pushButton_4->setStyleSheet("background-color: rgb(255,255,0);");
     }
   /*  if(testflag2)
     {

     }
     if(testflag2==0)
     {
      QMessageBox::about(this,"Title","Please Connect RS232");
     }*/


}
