//#define WITHOUT_UART_DETECT_BAUD_RATE

#include <msp430.h>
#include <legacymsp430.h>
#include <sys/types.h>
#include <uart.h>
#include <print.h>
#include <gpio.h>

void clk_init()
{
	// Clock setup
	// MCLK = SMCLK = ACLK = DCO(7,15) which is highest freq (21Mhz for MSP430G2231)
	// LFXT1 = 32768 Hz watch crystal, cap = 12.5 pF
	// XT2 is disabled
	
	// DCO = 7, and no modulation between two frequency
	DCOCTL 	= 7<<5;

	// Select highest DCO frequency range, ACLK = DCO, LFXT set to low freq, not using XT2
	BCSCTL1 = XT2OFF | DIVA_0 | 0xF;
	
	// MCLK = DCO, SMCLK = DCO, DCO using internal resistor
	BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

	// Using 32768 Hz xtal for LFXT1, cap = 12.5 pF
	BCSCTL3 = LFXT1S_0 | XCAP_3;
}

void init()
{
	clk_init();
	uart_init(UART_B9600);
	print_init(uart_send, uart_send_byte);
}

int main()
{
	init();

	print("Welcome to MSP430 Launchpad.\r\n");

	while (1) {
		char c = uart_recv_byte();
		uart_send_byte(c);
	}
	return 0;
}
