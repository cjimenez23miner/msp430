#include <msp430.h>
#include <legacymsp430.h>
#include <sys/types.h>
#include <uart.h>
#include <jtag.h>

void init()
{
	// Setup clock, MCLK set to highest 21MHz
	DCOCTL 	= 0xFF;
	BCSCTL1 = RSEL3 | RSEL2 | RSEL1 | RSEL0;
	BCSCTL2 = 0;
	BCSCTL3 = XCAP_3;
}

unsigned long int idcode = 0;

int main()
{
	init();
	uart_init(UART_B9600);
	jtag_init();
	print("Hello World\n");
	
	while (1) {
		char c = uart_recv_byte();

		if (c == 'A') {
			print("Reading IDCODE...\n");
			jtag_reset_sequence();
			idcode = jtag_read_dr(JTAG_DR_IDCODE);
			print("IDCODE: 0x");
			printInt(idcode);
		}


	}
	return 0;
}
