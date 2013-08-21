#ifndef __UART_H__
#define __UART_H__

#include <msp430.h>

//#define UART_CLK_DCO

#ifdef UART_CLK_DCO
// Following Baud interval is measure using 21Mhz MCLK
// i.e. DCOCTL = 0xFF, RSEL = 0x7
#define UART_B4800		0x10D0
#define UART_B9600		0x868
#define UART_B19200		0x434
#define UART_B38400		0x21A
#define UART_B57600		0x166
#define UART_DETECTION_DELAY 22
#define UART_TIMER_CLK_SRC TASSEL_2
#else
// CLK = 11.0592Mhz
#define UART_B4800		2304
#define UART_B9600		1152
#define UART_B19200		576
#define UART_B38400		288
#define UART_B57600		144
#define UART_DETECTION_DELAY 11
#define UART_TIMER_CLK_SRC TASSEL_1
#endif


#define UART_RXD_PORT		1
#define UART_RXD_PIN		1

#define UART_TXD_PORT		1
#define UART_TXD_PIN		2

#define UART_CAPTURE_SRC	CCIS_0

int uart_init(unsigned int baud_interval);
char uart_recv_byte();
void uart_recv(char* buf, unsigned int len);
void uart_send_byte(char byte);
void uart_send(char* buf, unsigned int len);
#endif
