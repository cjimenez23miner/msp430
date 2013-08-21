#include <msp430.h>
#include <legacymsp430.h>

volatile int timer = 0;
volatile int noteIdx = 0;
volatile int musicIdx = 0;
volatile int musicOn = 1;
volatile int isUp = 1;

int musicmax = 32;
int note[9] = {1,20067,17878,15927,15033,13393,11932,10630,10033};
// Merry has a little lamb ~
int music[32] = {3,2,1,2,3,3,3,0,2,2,2,0,3,5,5,0,3,2,1,2,3,3,3,0,2,2,3,2,1,0};
int main(void) {

  DCOCTL = 0x7<<4; // Set to 21MHz
  BCSCTL1 |= 0xf;
  WDTCTL = WDT_MDLY_32;
  IE1 |= WDTIE;

  P1DIR |= BIT6 + BIT0;
  P1DIR &= ~BIT3;

  BCSCTL2 |= BIT1;
  BCSCTL3 |= LFXT1S_2;	//Set ACLK to use internal VLO (12 kHz clock)

  TACTL = TASSEL_2 | MC_1;	//Set TimerA to use auxiliary clock in UP mode

  WRITE_SR(GIE);	//Enable global interrupts

  char isPressed = 0;
  while(1) {
	//Loop forever, interrupts take care of the rest
	if(!(P1IN & BIT3)){
		if(!isPressed)
			isPressed = 1;
		else
			isPressed = 2;
	}else{
		isPressed = 0;
  	}

	if(isPressed == 1){
		musicOn = musicOn?0:1;

		if(!musicOn){
			musicIdx = 0;
		}	
	}
  }

}


interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void)
{
	P1OUT ^= BIT6|BIT0;
}


interrupt(WDT_VECTOR) WDT_ISR(void)
{
	int tempo = 80;
	int pause = 4;
	if(timer == tempo){
  		TACCTL0 &= ~CCIE;	//Enable the interrupt for TACCR0 match

	}else if(timer == tempo+pause){
/*
		if(noteIdx == 9)
			isUp = 0;

		if(noteIdx == 0)
			isUp = 1;

		if(isUp)
 			noteIdx++;
		else
			noteIdx--;

*/
		if(musicOn){
			noteIdx=music[musicIdx];

			musicIdx++;

			if(musicIdx >= musicmax)				
				musicIdx = 0;

			TACCTL0 |= CCIE;	//Enable the interrupt for TACCR0 match
			TACCR0 = note[noteIdx]>>2;
		}
		timer = 0;
	}

	timer++;
}
