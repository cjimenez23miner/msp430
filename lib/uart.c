#include <uart.h>
#include <gpio.h>

static unsigned int _uart_baud_interval = 0;

#define UART_WAIT_TIMER_IRQ		while((TACCTL0 & CCIFG) == 0); TACCTL0 &= ~CCIFG;

#ifndef WITHOUT_UART_DETECT_BAUD_RATE
/* 
 * Using timer's capture to measure the time interval between rising edges and falling edges.
 * detect_baud_rate require host sending 0x55 (captial letter U) using 8-N-1 for baud rate detection
 * MCU will then see 5 cycle of 50% duty saqure wave with freq = baud rate (remember LSB of 0x55 is sent first)
 */
unsigned int detect_baud_rate()
{
	int i;
	unsigned int sum = 0;

	// enable capture mode, capture on both rising and falling edge
	TACCTL0 = CM_3 | UART_CAPTURE_SRC | CAP; 	
	
	// count up to 0xffff, reset timer A 
	TACTL = TASSEL_2 | MC_2 | TACLR;

	for (i = 0; i<10; i++) {

		UART_WAIT_TIMER_IRQ;
		/*
			Following code is more accurate there is delay in clear interrput flag 
			and clear timmer counter. We need not consider this delay if timer counter
			is not reset, however extra codde is require to handle timer counter wrapped
			back
			
			unsigned int counter = 0;
			unsigned int delta = 0;

			counter = TACCR0;
			if(counter < last_counter) {
				delta = counter + (0xffff - last_counter) + 1;
			}else {
				delta = counter - last_counter;
			}
			last_counter = counter;
			interval[num_edge] = delta;
		*/

		// read back timer counter value, UART_DETECTION_DELAY is the experimental 
		// time delay between capture interrupt to timer reset
		//
		// Actually the first interval should not have the extra delay, however we will
		// not count the first and last interval (only use middle 8 bits and we can use
		// right shift for division), so it is safe to do so.
		if (i>=1 && i<=8) {
			sum += TACCR0 + UART_DETECTION_DELAY;
		}

		// reset timer A
		TACTL |= TACLR;
	}

	return sum>>3;
}
#endif

int uart_init(unsigned int baud_interval)
{
	GPIO_SET_DIR(UART_TXD_PORT, UART_TXD_PIN, OUT);
	GPIO_SET_HIGH(UART_TXD_PORT, UART_TXD_PIN);

	GPIO_SET_DIR(UART_RXD_PORT, UART_RXD_PIN, IN);
	GPIO_SELECT_FN_1(UART_RXD_PORT, UART_RXD_PIN);

#ifndef WITHOUT_UART_DETECT_BAUD_RATE
	if (!baud_interval) {
		_uart_baud_interval = detect_baud_rate();
	}else
#endif
	{
		_uart_baud_interval = baud_interval;
	}

	return _uart_baud_interval;
}

char uart_recv_byte()
{
	unsigned char byte = 0;;
	int i;

	// capture mode, trigger on falling edge
	TACCTL0 = CM_2 | UART_CAPTURE_SRC | CAP;

	// count up to 0xFFFF (not important here), reset timer A
	TACTL = UART_TIMER_CLK_SRC | MC_2 | TACLR;

	// wait for start bit (i.e. falling edge)
	UART_WAIT_TIMER_IRQ;

	// disable capture mode
	TACCTL0 = 0;

	TACCR0 = _uart_baud_interval;

	// Start counting and sample RXD pin logic level when timer A counter reach
	// baud interval, sample 8 bits in total
	TACTL |= TACLR;
	for (i=0; i<8; i++) {
		
		UART_WAIT_TIMER_IRQ;

		// immediately start the next counting for accurate counting
		// it is safe since host need to hold the output value for baud interval long
		TACTL |= TACLR;

		if(GPIO_IS_SET(UART_RXD_PORT, UART_RXD_PIN)) {
			byte |= 1<<i;
		}
	}

	UART_WAIT_TIMER_IRQ;

	return (char) byte;
}

void uart_recv(char* buf, unsigned int len)
{
	unsigned int idx = 0;
	while (idx < len){
		buf[idx++] = uart_recv_byte();
	}	
}

void uart_send_byte(char byte)
{
	int i;

	// disable capture mode
	TACCTL0 = 0;

	TACCR0 = _uart_baud_interval;

	// send start bit;
	GPIO_SET_LOW(UART_TXD_PORT, UART_TXD_PIN); 

	// use SMCLK, count up to 0xFFFF, reset timer A
	TACTL =  UART_TIMER_CLK_SRC | MC_2 | TACLR;

	for (i=0; i<9; i++) {
		UART_WAIT_TIMER_IRQ;
		TACTL |= TACLR;

		if (i < 8) {
			if ((byte & 0x1)) {
				GPIO_SET_HIGH(UART_TXD_PORT, UART_TXD_PIN);
			}else {
				GPIO_SET_LOW(UART_TXD_PORT, UART_TXD_PIN);
			}

			byte >>=1;
		}else{
			// stop bit
			GPIO_SET_HIGH(UART_TXD_PORT, UART_TXD_PIN);
		}
	}
	UART_WAIT_TIMER_IRQ;
}

void uart_send(char* buf, unsigned int len)
{
	unsigned int idx = 0;
	while (idx < len){
		uart_send_byte(buf[idx++]);
	}
}

#undef UART_WAIT_TIMER_IRQ
