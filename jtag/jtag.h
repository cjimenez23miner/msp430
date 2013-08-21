#ifndef __JTAG__
#define __JTAG__

#include <msp430.h>

#define JTAG_TCK	BIT7
#define JTAG_TMS	BIT3
#define JTAG_TDI	BIT4
#define JTAG_TDO	BIT5

#define JTAG_DR_DPACC	0xA
#define JTAG_DR_APACC	0xB
#define JTAG_DR_IDCODE	0xE

void jtag_init();
void jtag_delay(unsigned int delay);
void jtag_reset_sequence();
void jtag_set_ir(unsigned int instruction);
unsigned long int jtag_read_dr(unsigned int dr);

#endif
