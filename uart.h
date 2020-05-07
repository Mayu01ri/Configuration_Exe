#ifndef	__UART_H_
#define __UART_H_

//#include "define.h"
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>	        	//Used for UART
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//#include "file.h"

int avc_uart_init();

void UartTx(char *ptr, int NumByte);
char UartRx(char *rx_buffer);
int  sensor_uart_init();
int Tlc_uart_init();
void Uart3Tx(char *ptr, int NumByte);
char Uart3Rx(char *rx_buffer);
void UartTx_tlc(char *ptr, int NumByte);
void UartClose();
void WaitMs(int );

extern volatile int rx_length;
extern volatile int sfd;
extern volatile int sfd_tlc;
extern volatile int Uart3sfd;
extern volatile int rx_length_uart3;
extern volatile int rx_length_uart;

#endif
