
#include <stdio.h>

#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>	        	//Used for UART
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "uart.h"
#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <pthread.h>
#include <sys/stat.h>


void AutoConnectUarts();
unsigned char blnMessageReadyToSend;
char blnSensorComConnect=0;
char blnAvcComConnect=0;
char blnTlcComConnect=0;
char TextArray[100];
volatile int sfd;
volatile int Uart3sfd=-1;
volatile int sfd_tlc=-1;
extern char blnTlcComConnect;
volatile int rx_length_uart3;
volatile int rx_length_uart;
char serialnumbuff_avc[50];
char serialnumbuff_tlc[50];
char serialnumbuff_sensor[50];
pthread_t Uart3Tx_thread_tid,tlc_thread_tid,Main_progarm_tid,AVC_thread_tid,sensor_thread_tid;
//unsigned char UartRxData[10];
char flag1=0,flag2=0,AVC_flag=0,tlc_flag=0,flag5=0,sensor_flag=0,flag7=0,flag_yavc=0,flag_ytlc=0,flag_ysensor=0;
//char yellow_TLC;
char testflag_register=0,testflag2=1;
char flag_avc=0,flag_tlc=0,flag_sensor=0;
char Ch=0;
int i=0;
 int testflag;


void WaitMs(int a)
{
    int i;
    for(i=0; i<a; i++)
        {
            usleep(1000);
        }
}

void TextMessageGui(const char* format, ...)
{

    va_list arglist;//create pointer
    va_start( arglist, format );
    vsprintf(TextArray,format,arglist);
    va_end( arglist );
    //printf("a");
    blnMessageReadyToSend=1;
}

int Tlc_uart_init()
{
    /*Read file from memory*/
   // NumbeOfCharReadFile=read_file_upto_EOF(com_port_avc_file_path,(char *)UartComArray);
    //printf("NumbeOfCharReadFile=%d\n",NumbeOfCharReadFile);
    //printf("\ncom_port_avc file Read:%s\n",UartComArray);
    /*int icntr;
    for(icntr=0; icntr<NumbeOfCharReadFile+1; icntr++)
            printf("UartComArray[%d]=%d\n",icntr,UartComArray[icntr]);*/
    //UartComArray[NumbeOfCharReadFile]='\0';
    /************************/
    sfd_tlc = open("/dev/ttyUSB_tlc",O_RDWR | O_NOCTTY | O_NDELAY);

    //sfd = open((char *)UartComArray,O_RDWR | O_NOCTTY | O_NDELAY);
    struct termios s_param;
    if (tcgetattr(sfd_tlc,&s_param))
    {
           perror("com_port_tlc: Couldn't get term attributes\n");
           return -1;
    }
   cfsetispeed(&s_param, B9600);
   cfsetospeed(&s_param, B9600);
   s_param.c_cflag &= ~PARENB;
   s_param.c_cflag &= ~CSTOPB;
   s_param.c_cflag &= ~CSIZE;
   s_param.c_cflag |= CS8;
   s_param.c_cflag &= ~CRTSCTS;
   s_param.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
   s_param.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
   s_param.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
   s_param.c_oflag &= ~OPOST; // make raw
   s_param.c_cc[VMIN]  = 1;
   s_param.c_cc[VTIME] = 1;


   if(tcsetattr(sfd_tlc,TCSANOW,&s_param))
   {
      perror("\ncom_port_TLC: Couldn't set term attributes\n");
      blnTlcComConnect=0;
      return -1;
   }
   else
       blnTlcComConnect=1;

   printf("TLC com_port_Initialise completed\n\n");
}



int sensor_uart_init()
{

     Uart3sfd = open("/dev/ttyUSB_sensor",O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
     /* O_RDWR Read/Write access to serial port           */
     /* O_NOCTTY - No terminal will control the process   */
     /* O_NDELAY -Non Blocking Mode,Does not care about-  */
     /* -the status of DCD line,Open() returns immediatly */

//    if(sfd == -1)
   ///* Error Checking */
//    printf("\n  Error! in Opening ttyUSB0  ");
//    else
//    printf("\n  ttyUSB0 Opened Successfully ");

    struct termios s_param_3;


     if (tcgetattr(Uart3sfd,&s_param_3))
     {
            perror("com_port_avc: Couldn't get term attributes\n");
            return -1;
     }

        cfsetispeed(&s_param_3, B9600);
        cfsetospeed(&s_param_3, B9600);
        s_param_3.c_cflag &= ~PARENB;
        s_param_3.c_cflag &= ~CSTOPB;
        s_param_3.c_cflag &= ~CSIZE;
        s_param_3.c_cflag |= CS8;
        s_param_3.c_cflag &= ~CRTSCTS;
        s_param_3.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
        s_param_3.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
        s_param_3.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
        s_param_3.c_oflag &= ~OPOST; // make raw
        s_param_3.c_cc[VMIN]  = 1;
        s_param_3.c_cc[VTIME] = 1;


        if(tcsetattr(Uart3sfd,TCSANOW,&s_param_3))
          {
           perror("\ncom_port_avc: Couldn't set term attributes\n");
           return -1;
          }

        printf("com_port_avc Initialise completed\n\n");
        printf("\n\rSending Starting");

    return 0;
}
/*************************************************/
int avc_uart_init()
{
    /*Read file from memory*/
   // NumbeOfCharReadFile=read_file_upto_EOF(com_port_avc_file_path,(char *)UartComArray);
    //printf("NumbeOfCharReadFile=%d\n",NumbeOfCharReadFile);
    //printf("\ncom_port_avc file Read:%s\n",UartComArray);
    /*int icntr;
    for(icntr=0; icntr<NumbeOfCharReadFile+1; icntr++)
            printf("UartComArray[%d]=%d\n",icntr,UartComArray[icntr]);*/
    //UartComArray[NumbeOfCharReadFile]='\0';
    /************************/
     sfd = open("/dev/ttyUSB_avc",O_RDWR | O_NOCTTY | O_NDELAY);

     //sfd = open((char *)UartComArray,O_RDWR | O_NOCTTY | O_NDELAY);
     struct termios s_param;
     if (tcgetattr(sfd,&s_param))
     {
            perror("com_port_avc: Couldn't get term attributes\n");
            return -1;
     }
    cfsetispeed(&s_param, B9600);
    cfsetospeed(&s_param, B9600);
    s_param.c_cflag &= ~PARENB;
    s_param.c_cflag &= ~CSTOPB;
    s_param.c_cflag &= ~CSIZE;
    s_param.c_cflag |= CS8;
    s_param.c_cflag &= ~CRTSCTS;
    s_param.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    s_param.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    s_param.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    s_param.c_oflag &= ~OPOST; // make raw
    s_param.c_cc[VMIN]  = 1;
    s_param.c_cc[VTIME] = 1;
    if(tcsetattr(sfd,TCSANOW,&s_param))
    {
       perror("\ncom_port_avc: Couldn't set term attributes\n");
       return -1;
    }
    printf("com_port_avc Initialise completed\n\n");
}

void Uart3Tx(char *ptr, int NumByte)
{
    //ptr="xyz";
    printf("\n\rSending....");
    if (Uart3sfd != -1)
    {
        //printf("\n\rSending final....");
        int count = write(Uart3sfd,ptr,NumByte);		//Filestream, bytes to write, number of bytes to write
        if (count < 0)
        {
            printf("\n\rUART TX error\n");
        }
        printf("transmit %s",ptr);

    }
}

char Uart3Rx(char *rx_buffer1)
{

    //----- CHECK FOR ANY RX BYTES -----
    if (Uart3sfd != -1)
    {
        // Read up to 255 characters from the port if they are there
        //unsigned char rx_buffer[256];
        //unsigned char rx_buffer;

        rx_length_uart = read(Uart3sfd,rx_buffer1,1);		//Filestream, buffer to store in, number of bytes to read (max)

        printf("\n\r rx_length=%d\n\r",rx_length_uart);

        if (rx_length_uart < 0)
        {
            //An error occured (will occur if there are no bytes)
            printf("\n\rAn error occured while receiving\n");
            memset(rx_buffer1,'\0',1);
        }
        else if (rx_length_uart == 0)
        {
            //No data waiting
            printf("\n\rNo Data available\n");
            memset(rx_buffer1,'\0',1);
        }
        else
        {
            //Bytes received
            //rx_buffer[rx_length_uart3] = '\0';
            //printf("%i bytes read Uart3: %s\n", rx_length_uart3, rx_buffer);
            //printf("\n\rData Read Successfully\n");
//            for(i=0;i<rx_length_uart;i++)
//            {
//               printf("%c", rx_buffer[i]);
//            //return rx_buffer[i];
//            }
        }
    }

}

void AutoConnectUarts()
{

    struct stat st1 = {0};
    if(!blnSensorComConnect)
    {
       if (!(stat("/dev/ttyUSB_sensor", &st1) == -1))//if(CheckfileExists("/dev/ttyUSB_sensor"))
       {
           blnSensorComConnect=1;
           flag_ysensor=0;
           flag7=1;
           TextMessageGui("Sensor com port connected");
           sensor_uart_init();
           WaitMs(1);
       }
   }
   else
   {
        if ((stat("/dev/ttyUSB_sensor", &st1) == -1))//if(!CheckfileExists("/dev/ttyUSB_sensor"))
       {
           blnSensorComConnect=0;
           flag7=0;
           TextMessageGui("Sensor com port disconnected");
           WaitMs(1);
       }
   }

   if(!blnAvcComConnect)
   {
      if (!(stat("/dev/ttyUSB_avc", &st1) == -1))// if(CheckfileExists("/dev/ttyUSB_avc"))
      {
          blnAvcComConnect=1;
          flag_yavc=0;
          flag2=1;

          TextMessageGui("Avc com port connected");
          avc_uart_init();
          WaitMs(1);
      }
  }
  else
  {
       if ((stat("/dev/ttyUSB_avc", &st1) == -1))//if(!CheckfileExists("/dev/ttyUSB_avc"))
      {
          blnAvcComConnect=0;
          flag2=0;
          TextMessageGui("Avc com port disconnected");
          WaitMs(1);
      }
  }
   if(!blnTlcComConnect)
   {
      if (!(stat("/dev/ttyUSB_tlc", &st1) == -1))// if(CheckfileExists("/dev/ttyUSB_avc"))
      {
          if(Tlc_uart_init()==-1)
          {
            blnTlcComConnect=0;
            flag5=0;
            TextMessageGui("Tlc com port disconnected");
          }
          else
          {
            blnTlcComConnect=1;
            flag_ytlc=0;
            flag5=1;
            TextMessageGui("Tlc com port connected");
          }
          WaitMs(1);
      }
  }
  else
  {
       if ((stat("/dev/ttyUSB_tlc", &st1) == -1))//if(!CheckfileExists("/dev/ttyUSB_avc"))
      {
           blnTlcComConnect=0;
            flag5=0;
          TextMessageGui("Tlc com port disconnected");
           WaitMs(1);
      }
  }
}



void *Uart3Tx_thread(void *thread)
{
   while(1)
   {



         if(flag1==1)

         {

                //flag1=0;
                Uart3Tx(uartdttx,3);
                // printf("Hi");
                TextMessageGui("%s",(char *)uartdttx);


         }

        sleep(1);

   }
      //  if(flag1)
       // {
            // ui->textBrowser->append(uartdttx);

           // sleep(1);
        //}

}


void file_AVC()
{
   char ch=0,i=0;
   FILE *fptr1;

        if ((fptr1 = fopen("/home/kent/code/files/outAVC.txt","r")) == NULL)
        {
            printf("Error! opening file /home/kent/Desktop/Final_tx_rxC/two/outAVC.txt \n");
        }
        else
        {
          //  while (fgets(laneidArr,10, fptr) != NULL)
           fseek( fptr1, 17, SEEK_SET );
            for(i=0;i<8;i++)
             {


                ch=fgetc(fptr1);
                printf("%c",ch);
                serialnumbuff_avc[i]=ch;

            }

           // printf("\n\rSerialnumber_AVC:%c%c%c", serialnumbuff[5],serialnumbuff[6],serialnumbuff[7]);
           // printf("\n\rS.no_AVC=%s",serialnumbuff);
            fclose(fptr1);

        }

}
void file_tlc()
{
   char ch=0,i=0;
   FILE *fptr1;

        if ((fptr1 = fopen("/home/kent/code/files/outtlc.txt","r")) == NULL)
        {
            printf("Error! opening file /home/kent/code/files/outtlc.txt\n");
        }
        else
        {
          //  while (fgets(laneidArr,10, fptr) != NULL)
           fseek( fptr1, 17, SEEK_SET );
            for(i=0;i<8;i++)
             {


                ch=fgetc(fptr1);
                printf("%c",ch);
                serialnumbuff_tlc[i]=ch;

            }

           // printf("\n\rSerialnumber:%c%c%c", serialnumbuff[5],serialnumbuff[6],serialnumbuff[7]);
           // printf("\n\rhiii=%s",serialnumbuff);
            fclose(fptr1);

        }

}

void file_sensor()
{
   char ch=0,i=0;
   FILE *fptr1;

        if ((fptr1 = fopen("/home/kent/code/files/outsensor.txt","r")) == NULL)
        {
            printf("Error! opening file /home/kent/code/files/outsensor.txt \n");
        }
        else
        {
          //  while (fgets(laneidArr,10, fptr) != NULL)
           fseek( fptr1, 17, SEEK_SET );
            for(i=0;i<8;i++)
             {


                ch=fgetc(fptr1);
                printf("%c",ch);
                serialnumbuff_sensor[i]=ch;

            }

           // printf("\n\rSerialnumber:%c%c%c", serialnumbuff[5],serialnumbuff[6],serialnumbuff[7]);
           // printf("\n\rhiii=%s",serialnumbuff);
            fclose(fptr1);

        }

}
void deletefile_sensor()
{
    if (remove("/home/kent/code/files/outsensor.txt") == 0)
          printf("outsensor.txt Deleted successfully");
       else
          printf("outsensor.txt Unable to delete the file");
}
void deletefile_tlc() //commented
{
    if (remove("/home/kent/code/files/outtlc.txt") == 0)
          printf("outtlc.txt Deleted successfully");
       else
          printf("outtlc.txt Unable to delete the file");
}


void deletefile_AVC()
{
    if (remove("/home/kent/code/files/outAVC.txt") == 0)
          printf("Deleted successfully");
       else
          printf("Unable to delete the file");



}


void execute_AVC()  //commented
{
   // flag2=1;


    int i;
    //system("usb-devices |grep FTDI -A2 |grep SerialNumber >>outAVC.txt");
    //system("usb-devices |grep FTDI -A3 |grep Manufacturer >>outAVC.txt");
    int i_ManifactOut=0;
    i_ManifactOut=system("usb-devices |grep FTDI -A3 |grep Manufacturer");
    printf("\nManRet=%d\n",i_ManifactOut);
    if(i_ManifactOut==0)
    {

        system("usb-devices |grep FTDI -A2 |grep SerialNumber >>/home/kent/code/files/outAVC.txt");

         file_AVC();

         TextMessageGui("%s",serialnumbuff_avc);
          flag_yavc=1;
          flag_avc=1;
         testflag_register=1;
         testflag2=1;

          deletefile_AVC();

         QFile file2("/home/kent/code/99-usb-serial.rules");
         if(file2.open(QFile::ReadWrite | QFile::Truncate | QFile::Append))
         {
             QTextStream stream(&file2);
             stream<<"SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", ATTRS{serial}==\""<<(char*)(serialnumbuff_avc)<<"\""<< ", SYMLINK+=\""<<"ttyUSB_avc\""<<"\n";

         }
         file2.close();
         system("sudo cp /home/kent/code/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules");
      }
    else
    {
        testflag2=0;
    }
    //memset(serialnumbuff, '\0', sizeof(serialnumbuff));
}
void execute_tlc()
{
   // flag2=1;


    int i;

    int i_ManifactOut=0;
    i_ManifactOut=system("usb-devices |grep FTDI -A3 |grep Manufacturer");
    printf("\nManRet2=%d\n",i_ManifactOut);
    if(i_ManifactOut==0)
    {
        printf("\n\rIn two function\n\r");
        system("usb-devices |grep FTDI -A2 |grep SerialNumber >>/home/kent/code/files/outtlc.txt");
       // system("usb-devices |grep FTDI -A3 |grep Manufacturer >>outtlc.txt");
         file_tlc();
         printf("\n\rone\n\r");
         TextMessageGui("%s",serialnumbuff_tlc);
         flag_ytlc=1;
         testflag_register=1;
         flag_tlc=1;

         printf("\n\rtwo\n\r");
         deletefile_tlc();


         QFile file5("/home/kent/code/99-usb-serial.rules");
         if(file5.open(QFile::WriteOnly | QFile::Truncate | QFile::Append))
         {
             QTextStream stream(&file5);
             stream<<"SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", ATTRS{serial}==\""<<(char*)(serialnumbuff_tlc)<<"\""<< ", SYMLINK+=\""<<"ttyUSB_tlc\""<<"\n";

         }
         file5.close();


         system("sudo cp /home/kent/code/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules");

       //   QFile file11("sudo /etc/udev/rules.d/99-usb-serial.rules");
         //  if(file11.open(QFile::WriteOnly | QFile::Truncate | QFile::Append))
          // {
            //  TextMessageGui("file11 opened");
           //    return;
          // }
           //file11.close();
        // memset(serialnumbuff, '\0', sizeof(serialnumbuff));

      }
}

void execute_sensor()
{
   // flag2=1;


    int i;
    //system("usb-devices |grep FTDI -A2 |grep SerialNumber >>out.txt");
    //system("usb-devices |grep FTDI -A3 |grep Manufacturer >>out.txt");
    int i_ManifactOut=0;
    i_ManifactOut=system("usb-devices |grep FTDI -A3 |grep Manufacturer");
    printf("\nManRet=%d\n",i_ManifactOut);
    if(i_ManifactOut==0)
    {

        system("usb-devices |grep FTDI -A2 |grep SerialNumber >>/home/kent/code/files/outsensor.txt");
       // system("usb-devices |grep FTDI -A3 |grep Manufacturer >>outsensor.txt");
         file_sensor();
         TextMessageGui("%s",serialnumbuff_sensor);
         flag_ysensor=1;
         flag_sensor=1;
         testflag_register=1;

         deletefile_sensor();
         QFile file22("/home/kent/code/99-usb-serial.rules");
         if(file22.open(QFile::ReadWrite | QFile::Truncate | QFile::Append))
         {
             QTextStream stream(&file22);
             stream<<"SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", ATTRS{serial}==\""<<(char*)(serialnumbuff_sensor)<<"\""<< ", SYMLINK+=\""<<"ttyUSB_sensor\""<<"\n";

         }
         file22.close();
         system("sudo cp /home/kent/code/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules");
      }
    // memset(serialnumbuff, '\0', sizeof(serialnumbuff));
}

void *AVC_thread(void *thread_2) //commented
{


    while(1)
    {
   // printf("1flag=%d",flag3);
    sleep(1);

         if(AVC_flag==1)
        {
           // printf("2flag=%d",flag3);
           // sleep(1);
            TextMessageGui("Serial number for AVC_COM");
            printf("\n\rIn avc thread");
            execute_AVC();

            //extractserialnumber();
            //TextMessageGui("%s",serialno);
            AVC_flag=0;
            // system("usb-devices |grep FTDI -A2 |grep SerialNumber >>outAVC.txt");
            //system("usb-devices |grep FTDI -A3 |grep Manufacturer >>outAVC.txt");
             // file();
            //TextMessageGui("%s",serialnumbuff);

        }
       //  if(flag4==1)
        // {
          //    TextMessageGui("Hello2");
           //   execute3();
           //   flag4=0;

        // }




         //sleep(1);
          AutoConnectUarts();

    }

}

void *tlc_thread(void *thread_2)
{
    while(1)
    {  sleep(1);


    if(tlc_flag==1)
    {
         TextMessageGui("Serial number for TLC_COM");
         execute_tlc();
        // flag9=1;
         tlc_flag=0;

    }

    }


    //sleep(1);
     AutoConnectUarts();
}


void *sensor_thread(void *thread_2)
{
    while(1)
    {
        sleep(1);


        if(sensor_flag==1)
        {
            TextMessageGui("Serial number for Sensor_COM");
            execute_sensor();

            sensor_flag=0;

        }
           AutoConnectUarts();

    }


    //sleep(1);

}



void UartClose()
{
    //----- CLOSE THE UART -----
    close(sfd);
    close(Uart3sfd);
    close(sfd_tlc);
}





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    char Ch=0;

  //  printf("\n\rHello Uart\n\r");
     //  AutoConnectUarts();
      // sensor_uart_init();

     /*  if(pthread_create(&Uart3Tx_thread_tid, NULL, Uart3Tx_thread, NULL))
       {
           printf("Error creating Uart3tx_thread thread thread\n");
           exit(0);
       }
       else
       {
           printf("Uart3tx_thread creation is Successfull!!!!");

       }*/
   if(pthread_create(&AVC_thread_tid, NULL, AVC_thread, NULL))//commented
   {
       printf("Error creating \n");
       exit(0);
   }
   else
   {
       printf("1thread creation is Successfull!!!!");


   }

   if(pthread_create(&tlc_thread_tid, NULL, tlc_thread, NULL))
   {
       printf("Error creating \n");
       exit(0);
   }
   else
   {
       printf("2thread creation is Successfull!!!!");


   }

   if(pthread_create(&sensor_thread_tid, NULL, sensor_thread, NULL))
   {
       printf("Error creating \n");
       exit(0);
   }
   else
   {
       printf("3thread creation is Successfull!!!!");


   }




   /*if(pthread_create(&Main_progarm_tid, NULL,main_program, NULL))  //commented
   {
       printf("Error creating main_program thread thread\n");
       exit(0);
   }
   else
   {
       //printf("main_program thread creat successfully\n");
   }*/

   // sensor_uart_init();

    //printf("\n\rrecv...");
//    while(1)
//    {
//    Uart3Tx(uartdttx,3);
//     //Uart3Tx(uartdttx,3);

//    WaitMs(200);
//    }

    //Uart3Rx(&Ch);

    //uartdtrx[0]=Ch;

   // printf("\n\rRECV:%c",Ch);

     //sensor_uart_init();
    // printf("receiving...");
    //WaitMs(1000);
    //}
  // UartRx(&RecvCpyCh);
  // uartdtrx[i]=RecvCpyCh;
  // printf("\n\r1st letter :%c",uartdtrx[i]);

    /*for(i=0;i<3;i++)
    {
     c=UartRx(&RecvCpyCh);
    uartdtrx[i]=c;

    printf("\n\rcount:%d\n\r",i);
    printf("\n%c",uartdtrx[i]);


    }*/
  // printf("%s",uartdtrx);
      w.mThread->start();

      return a.exec();
}

