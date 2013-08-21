#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_GET_PXIN(port)		P##port##IN
#define GPIO_GET_PXOUT(port)	P##port##OUT
#define GPIO_GET_PXDIR(port)	P##port##DIR
#define GPIO_GET_PXSEL(port)	P##port##SEL

#define GPIO_SET_DIR(port, pin, dir)	GPIO_SET_##dir(port, (pin))
#define GPIO_SET_IN(port, pin)			GPIO_GET_PXDIR(port) &= ~(1<<(pin))
#define GPIO_SET_OUT(port, pin)			GPIO_GET_PXDIR(port) |= 1<<(pin)

#define GPIO_SET_HIGH(port, pin)		GPIO_GET_PXOUT(port) |= 1<<(pin)
#define GPIO_SET_LOW(port, pin)			GPIO_GET_PXOUT(port) &= ~(1<<(pin))

#define GPIO_IS_SET(port, pin) 			(GPIO_GET_PXIN(port) & (1<<(pin)))

#define GPIO_SELECT_FN_0(port, pin) 	GPIO_GET_PXSEL(port) &= ~(1<<(pin))
#define GPIO_SELECT_FN_1(port, pin) 	GPIO_GET_PXSEL(port) |= 1<<(pin)

#endif 
