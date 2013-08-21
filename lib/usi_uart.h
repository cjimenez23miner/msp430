#ifndef __UART_H__
#define __UART_H__

#define UART_RXD_PORT		1
#define UART_RXD_PIN		7

#define UART_TXD_PORT		1
#define UART_TXD_PIN		6

int uart_init(unsigned int baud_interval);
char uart_recv_byte();
void uart_recv(char* buf, unsigned int len);
void uart_send_byte(char byte);
void uart_send(char* buf, unsigned int len);

#endif
