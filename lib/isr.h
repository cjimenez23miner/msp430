#ifndef __ISR_H__
#define __ISR_H__

#include <msp430.h>
#include <legacymsp430.h>

enum {
	ISR_TIMERA0 = 0,
	ISR_TIMERA1,
	ISR_WDT,
	ISR_USI,
	ISR_ADC,
	ISR_GPIO_1_0,
	ISR_GPIO_1_1,
	ISR_GPIO_1_2,
	ISR_GPIO_1_3,
	ISR_GPIO_1_4,
	ISR_GPIO_1_5,
	ISR_GPIO_1_6,
	ISR_GPIO_1_7,
	ISR_MAX
};

extern void (*isr_table[ISR_MAX])(void);

#define ISR_REGISTER(isr, func) isr_table[(isr)] = (func)

#endif

