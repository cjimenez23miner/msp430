#include <isr.h>
#include <print.h>

void (*isr_table[ISR_MAX])(void) = {0};

interrupt(PORT1_VECTOR) isr_port1(void)
{
	__disable_interrupt();
	int i;
	unsigned char ifg = (P1IFG & P1IE);

	for (i=0; i<8; i++) {
		unsigned char mask = 1<<i;
		if(ifg & mask){
			(*isr_table[ISR_GPIO_1_0+i])();
			P1IFG &= ~mask;
		}
	}
	__enable_interrupt();
}

char g_isr_usi = 0;

interrupt(USI_VECTOR) isr_usi(void)
{
	g_isr_usi++;
	__disable_interrupt();
	(*isr_table[ISR_USI])();
	__enable_interrupt();
}

interrupt(ADC10_VECTOR) isr_adc(void)
{
	__disable_interrupt();
	(*isr_table[ISR_ADC])();
	__enable_interrupt();
}

interrupt(TIMERA0_VECTOR) isr_timer_a0(void)
{
	__disable_interrupt();
	(*isr_table[ISR_TIMERA0])();
	__enable_interrupt();
}

interrupt(TIMERA1_VECTOR) isr_timer_a1(void)
{
	__disable_interrupt();
	(*isr_table[ISR_TIMERA1])();
	__enable_interrupt();
}

interrupt(WDT_VECTOR) isr_wdt(void)
{
	__disable_interrupt();
	(*isr_table[ISR_WDT])();
	__enable_interrupt();
}
