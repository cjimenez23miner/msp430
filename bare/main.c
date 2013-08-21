#define P1OUT	0x21
#define P1DIR	0x22
#define P1SEL	0x26
#define P1SEL2	0x41

#define DCOCTL	0x56
#define BCSCTL1	0x57
#define BCSCTL2	0x58
#define BCSCTL3	0x53

#define mmreg_set(reg, flag) *((volatile unsigned char*)(reg)) |= (flag)
#define mmreg_clr(reg, flag) *((volatile unsigned char*)(reg)) &= ~(flag)
#define mmreg_read(reg) (*((volatile unsigned char*)(reg)))
#define mmreg_write(reg, value) (*((volatile unsigned char*)(reg)) = (value))

void delay_ms(int ms);

void init()
{
	mmreg_write(DCOCTL, 0xff);
	mmreg_write(BCSCTL1, 0xf);
	mmreg_write(BCSCTL2, 0);
	mmreg_write(BCSCTL3, 0xc);
}

int msp_main()
{
	init();

	mmreg_set(P1DIR, 0x1|(0x1<<6));
	mmreg_clr(P1SEL, 0x1|(0x1<<6));
	mmreg_clr(P1SEL2, 0x1|(0x1<<6));
	mmreg_write(P1OUT,0x40);

	while(1) {
		delay_ms(500);

		if (mmreg_read(P1OUT) == 0x1) {
			mmreg_write(P1OUT,0x40);
		}else{
			mmreg_write(P1OUT,0x1);
		}
	}
	return 0;
}

