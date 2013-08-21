#include <msp430.h>
#include <usi_uart.h>
#include <gpio.h>
#include <isr.h>

#define UART_MODE_IDLE 0
#define UART_MODE_RECV 1
#define UART_MODE_SEND 2

static char _uart_mode = UART_MODE_IDLE;
static unsigned long int _uart_inner_send_buffer = 0;
static unsigned char _uart_inner_send_data_length = 0;

char g_uart_send = 0;
void _uart_send_trigger()
{
	g_uart_send++;
	char send_bytes = 0;
	USISRL = (unsigned char)_uart_inner_send_buffer;
	_uart_inner_send_buffer >>= 8;
	USISRH = (unsigned char)_uart_inner_send_buffer;
	_uart_inner_send_buffer >>= 8;

	if(_uart_inner_send_data_length > 16) {
		send_bytes = 16;
	}else{
		send_bytes = _uart_inner_send_data_length;
	}

	USICNT = USI16B | send_bytes;
	_uart_inner_send_data_length -= send_bytes;
}


void uart_isr_recv_start()
{
	P1OUT |= 1;
//	P1IE &= ~(1<<UART_RXD_PIN);
//	USICTL0 |= USIPE7;
}


void uart_isr()
{
	if (_uart_mode == UART_MODE_RECV) {
		P1IE |= (1<<UART_RXD_PIN);
	}else if (_uart_mode == UART_MODE_SEND) {
		if (_uart_inner_send_data_length == 0) {
			USICTL1 &= ~USIIFG;
			USICTL0 &= ~USIPE7;
			_uart_mode = UART_MODE_IDLE;
		}else{
			_uart_send_trigger();
		}
	}
}

int uart_init(unsigned int baud_interval)
{
	GPIO_SET_DIR(UART_RXD_PORT, UART_RXD_PIN, IN);
	GPIO_SET_DIR(UART_TXD_PORT, UART_TXD_PIN, OUT);
	GPIO_SET_HIGH(UART_TXD_PORT, UART_TXD_PIN);

	GPIO_SET_DIR(1, 0, OUT);
	GPIO_SET_LOW(1, 0);
	GPIO_SET_DIR(1, 3, IN);
	P1IE |= (1<<3);
	P1IES |= (1<<3);
	ISR_REGISTER(ISR_GPIO_1_3, uart_isr_recv_start);


//	P1IE |= (1<<UART_RXD_PIN);
//	P1IES |= (1<<UART_RXD_PIN);

//	ISR_REGISTER(ISR_GPIO_1_0 + UART_RXD_PIN, uart_isr_recv_start);
	ISR_REGISTER(ISR_USI, uart_isr);

	// transmit LSB first, set to SPI master mode, enable output
	USICTL0 = USILSB | USIMST | USIOE;

	// clock = ACLK/64
	USICKCTL = USIDIV_6 | USISSEL_1;

	USICTL1 = USIIE;

	return baud_interval;
}

char uart_recv_byte()
{
	return 0;	
}

void uart_recv(char* buf, unsigned int len)
{
	unsigned int idx = 0;
	while (idx < len){
		buf[idx++] = uart_recv_byte();
	}	
}

char sending_char = 0;
void uart_send_byte(char byte)
{
	char i;
	sending_char = byte;

	while(_uart_mode != UART_MODE_IDLE);

	_uart_inner_send_buffer = (0x7<<3);
	for (i=7; i>=0; i--) {
		if(byte & (i<<i)){
			_uart_inner_send_buffer |= 0x7;
		}
		_uart_inner_send_buffer <<= 3;
	}

	_uart_inner_send_data_length = 30;

	USICTL0 |= USIPE7;
	_uart_mode = UART_MODE_SEND;
	_uart_send_trigger();
}

void uart_send(char* buf, unsigned int len)
{
	unsigned int idx = 0;
	while (idx < len){
		uart_send_byte(buf[idx++]);
	}
}
