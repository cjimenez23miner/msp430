#include <jtag.h>

#define SET(x)				P1OUT |= (x)		
#define CLR(x)				P1OUT &= ~(x)
#define GET(x)				((P1IN & (x))?1:0)
#define JTAG_TCK_DELAY		jtag_delay(jtag_clock_interval)
#define JTAG_SET_DELAY		jtag_delay(jtag_clock_interval>>4)
#define JTAG_FULL_CLOCK	    SET(JTAG_TCK); JTAG_TCK_DELAY; CLR(JTAG_TCK); JTAG_TCK_DELAY

unsigned int jtag_clock_interval = 210;

void jtag_init(unsigned int clk_interval)
{
	P1SEL &= ~(JTAG_TCK | JTAG_TMS | JTAG_TDI | JTAG_TDO);
	P1REN &= ~(JTAG_TCK | JTAG_TMS | JTAG_TDI | JTAG_TDO);

	P1DIR |= (JTAG_TCK | JTAG_TMS | JTAG_TDI);
	P1DIR &= ~JTAG_TDO;

	CLR(JTAG_TCK);
	CLR(JTAG_TMS);
	CLR(JTAG_TDI);
	CLR(JTAG_TDO);

	if (clk_interval) {
		jtag_clock_interval = clk_interval;
	}
}

void jtag_delay(unsigned int delay)
{
	TACCR0 = delay;
	TACTL =  TASSEL_2 | MC_2 | TACLR;
	while((TACCTL0 & CCIFG) == 0);
   	TACCTL0 &= ~CCIFG;
}

void jtag_reset_sequence()
{
	int i=0;
	SET(JTAG_TMS);
	SET(JTAG_TDI); // set high here to prevent accident write to register

	for (i=0; i<5; i++) {
		JTAG_FULL_CLOCK;
	}

	SET(JTAG_TCK); JTAG_TCK_DELAY;
	CLR(JTAG_TMS); JTAG_SET_DELAY;
	CLR(JTAG_TCK); JTAG_TCK_DELAY;
	JTAG_FULL_CLOCK;
	//Now TAP state machine should me at Run-Test/Idel
}
/*
void jtag_set_ir(unsigned int instruction)
{
	SET(JTAG_TMS); JTAG_SET_DELAY;
	JTAG_FULL_CLOCK;
	JTAG_FULL_CLOCK;
	// Now at Select-IR-Scan

	CLR(JTAG_TMS); JTAG_SET_DELAY;
	JTAG_FULL_CLOCK;
	// Now at Capture-IR

	int i = 0;
	for (i=0; i<4; i++) {
		if(instruction & 0x1) {
			SET(JTAG_TDI);
		}else{
		
			CLR(JTAG_TDI);
		}
		instruction>>=1;
		JTAG_SET_DELAY;
		JTAG_FULL_CLOCK;
	}

	SET(JTAG_TMS); JTAG_SET_DELAY;
	JTAG_FULL_CLOCK;
	// At EXit1-IR
	JTAG_FULL_CLOCK;
	// At Update-IR
	CLR(JTAG_TMS); JTAG_SET_DELAY;
	JTAG_FULL_CLOCK;
	// Back to Run-Test/Idle
}
*/

unsigned long int jtag_read_dr(unsigned int dr)
{
	unsigned char ack = 0;
	unsigned long int data = 0; 
	int i;

	SET(JTAG_TDI); // prevent write to register
	SET(JTAG_TCK); JTAG_TCK_DELAY;
	SET(JTAG_TMS);
	CLR(JTAG_TCK); JTAG_TCK_DELAY;

	SET(JTAG_TCK); JTAG_TCK_DELAY;
	// At DR-Scan
	CLR(JTAG_TMS); 
	CLR(JTAG_TCK); JTAG_TCK_DELAY;
	
	JTAG_FULL_CLOCK;
	// At Capture-DR
	
	
	if((dr == JTAG_DR_DPACC) || (dr == JTAG_DR_APACC)) {
		for (i=0; i<3; i++) {
			SET(JTAG_TCK); JTAG_TCK_DELAY; CLR(JTAG_TCK); JTAG_TCK_DELAY; 
			ack |= GET(JTAG_TDO)<<i;
		}
	}
		
	for (i=0; i<32; i++) {
		SET(JTAG_TCK); JTAG_TCK_DELAY; 
		
		if(i == 31)
			SET(JTAG_TMS);

		CLR(JTAG_TCK); JTAG_TCK_DELAY;
		unsigned long int d = GET(JTAG_TDO);
		data |= d<<i;
	}
	
	JTAG_FULL_CLOCK;
	// At EXit1-DR

	SET(JTAG_TCK); JTAG_TCK_DELAY;
	// At Update-DR
	CLR(JTAG_TMS);
	CLR(JTAG_TCK); JTAG_TCK_DELAY;
	
	JTAG_FULL_CLOCK;
	// Back to Run-Test/Idle
	
	return data;
}
