#include <msp430.h>
#include <legacymsp430.h>
#include "music.h"

#define TEMPO 90
#define PAUSE 4

volatile int timer = 0;
volatile int noteIdx = 0;
volatile int musicIdx = 0;
volatile int musicOn = 1;
volatile int isUp = 1;
volatile int tempo = TEMPO;

const unsigned int note[48] = {
	20067,18941,17878,16874,
	15927,15033,14189,13393,
	12641,11932,11262,10630,
	10033,9470,8939,8437,
	7964,7517,7095,6697,
	6321,5966,5631,5315,
	5017,4735,4469,4219,
	3982,3758,3547,3348,
	3160,2983,2816,2658,
	2508,2368,2235,2109,
	1991,1879,1774,1674,
	1580,1491,1408,1329
};

int marker[8] = {0};
int saved_music_idx = 0;
int playback_mode = 0;

int main(void)
{

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
	while(1){
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
		}
	}

}


interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void)
{
	P1OUT ^= BIT6|BIT0;
}


interrupt(WDT_VECTOR) WDT_ISR(void)
{
	int longtempo = 0;
	int appendtempo = 0;

	if(!musicOn){
  		TACCTL0 &= ~CCIE;
		return;
	}

	if(timer == tempo){
  		TACCTL0 &= ~CCIE;

	}else if(timer == tempo+PAUSE){
NEW_NOTE:
		if(musicIdx >= musicmax || music[musicIdx] == TONE_END)				
			musicIdx = 0;
	
		int nt = music[musicIdx]&0x3f;
		int np = music[musicIdx]>>6;

		if(nt == TONE_LONG_TEMPO){
			longtempo = 1;
			tempo = TEMPO<<np;
			musicIdx++;
			goto NEW_NOTE;
		}else if(nt == TONE_LONG_TEMPO2){
			appendtempo = 1;
			musicIdx++;
			goto NEW_NOTE;
		}else if(nt >= TONE_MARK_A_START && nt <= TONE_MARK_D_START){
			marker[nt-TONE_MARK_A_START] = musicIdx+1;
			musicIdx++;
			goto NEW_NOTE;
		}else if(nt >= TONE_MARK_A_END && nt <= TONE_MARK_D_END){	
			if((playback_mode-1+TONE_MARK_A_END) == nt){
				musicIdx = saved_music_idx;
				playback_mode = 0;
				saved_music_idx = 0;
				goto NEW_NOTE;
			}else{
				marker[nt-TONE_MARK_A_END+4] = musicIdx-1;
				musicIdx++;
				goto NEW_NOTE;
			}
		}else if(nt >= TONE_PLAY_MARK_A && nt <= TONE_PLAY_MARK_D){
			playback_mode = nt-TONE_PLAY_MARK_A+1;
			saved_music_idx = musicIdx+1;
			musicIdx = marker[nt-TONE_PLAY_MARK_A];
			goto NEW_NOTE;
		}else{
			if(!longtempo){
				tempo = TEMPO>>np;
			}

			if(appendtempo)
				tempo += TEMPO;

			TACCTL0 |= CCIE;

			if(!nt)
				TACCR0 = 0;
			else
				TACCR0 = note[nt-1];
			
			musicIdx++;
			
		}
		timer = 0;
	}

	timer++;
}

