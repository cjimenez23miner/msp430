#ifndef __MUSIC_H__
#define __MUSIC_H__


/*
 * About Note Code:
 *     ---------------------
 * MSB | P:2 bit | T:6 bit | LSB
 *     ---------------------
 * 
 * P: Tempo modifier (0-3), T: Tone Value (0-48, 49 - 63 will have special function)
 * 
 * When T <= 48, P will reduce the note play time with >> operation
 * When T == 49, P will increase next tone's play time with << operation
 * When T == 63, represent end of music
 * When T == 0, this will be a rest normal, play time will be modified by P value just same as normal note
 * */

#define TONE_LONG_TEMPO		0x31
#define TONE_LONG_TEMPO2	0x32

#define TONE_MARK_A_START	0x33
#define TONE_MARK_B_START	0x34
#define TONE_MARK_C_START	0x35
#define TONE_MARK_D_START	0x36

#define TONE_MARK_A_END		0x37
#define TONE_MARK_B_END		0x38
#define TONE_MARK_C_END		0x39
#define TONE_MARK_D_END		0x3A

#define TONE_PLAY_MARK_A	0x3B
#define TONE_PLAY_MARK_B	0x3C
#define TONE_PLAY_MARK_C	0x3D
#define TONE_PLAY_MARK_D	0x3E

#define TONE_END			0x3F

#define L(x)	((x<<6)|TONE_LONG_TEMPO)
#define S(t,x)	((x<<6)|t)
#define MS(x)	(TONE_MARK_A_START	+ x)
#define ME(x)	(TONE_MARK_A_END	+ x)
#define P(x)	(TONE_PLAY_MARK_A	+ x)

#if 0

// Mary have a little lamb   
const unsigned char music[] = {
	17,15,13,15,17,17,L(1),17,  15,15,L(1),15,  17,20,L(1),20,
  	17,15,13,15,17,17,L(1),17,  15,15,17,15,L(2),13, TONE_END
};

#endif

#if 0

// Doraemon no uta
const unsigned char music[] = {
	13,77,13,81,22,81,113,20,  20,86,20,81,18,81,113,15,  15,79,15,81,24,88,22,84,
	18,81,18,81,77,12,77,113,15,0,  13,77,13,81,22,81,113,20,  20,86,20,81,18,81,113,15,
 	15,79,15,81,24,86,22,84,  20,82,18,79,12,15,113,13,241,0,63
};

#endif

/*
const unsigned char music[] = {
	26, 25, 26, 21, S(25,1), 26, S(0,1),	S(26,1),S(26,1),28,S(26,1),0x32,S(28,1),31,L(1),30,S(0,1),
	S(30,1),S(31,1),33,S(26,1),0x32,S(26,1),23,L(1),26,S(0,1),	25,S(23,1),0x32,S(25,1),26,L(1),26,S(0,1),
	
	26, 25, 26, 21, S(25,1), 26, S(0,1),	S(26,1),S(26,1),28,S(26,1),0x32,S(28,1),31,L(1),30,S(0,1),
	S(30,1),S(31,1),33,S(26,1),0x32,S(26,1),23,L(1),26,S(0,1),	28,S(26,1),0x32,S(28,1),26,L(1),26,S(0,1),
	
	0,63
};
*/

const unsigned char music[] = {
	0,0,0,0,
	MS(0),
	27,26,27,22,S(26,1),27,S(0,1),
	S(27,1),S(27,1),29,S(27,1),0x32,S(29,1),32,L(1),31,S(0,1),
	S(31,1),S(32,1),34,S(27,1),0x32,S(27,1),24,L(1),27,0,
	ME(0),

	26,S(24,1),0x32,S(26,1),27,L(1),27,0,
	
	P(0),	
	
	29,S(27,1),0x32,S(29,1),27,L(1),27,0,
	MS(0),
	
	27, 29, 31, S(29,1), 0x32, S(31,1), 32, L(1),29, S(0,2),
	27, 26, 22, S(22,1), 0x32, S(29,1), 26, 27,S(29,1),0x32,S(31,1),S(0,2),
	
	ME(0),
	S(31,1),S(32,1),L(1),34,34,36,31,S(27,1),0x32,S(27,1),
	22,0x32,S(24,1),0x32,S(27,1),31,0x32,S(29,1),S(0,1),
	P(0),
	S(31,1),S(32,1),L(1),0x32,S(34,1),S(0,3),	
	27,0x32,S(27,1),0x32,S(24,1),31,L(1),0x32,S(31,1),
	L(1),0x32,S(31,2), S(32,1),31,L(1),29,0,
	
	MS(0),	
	31,29,27, 27,S(27,1),27, 36,S(34,1),L(1),34,S(0,2),
	31,0x32,S(32,1),0x32,S(34,1),32,L(1),31,S(0,2),
	ME(0),

	S(29,1),S(27,1),0x32,S(27,1), S(27,1),0x32,S(29,1), 31,31,S(29,1),22, 27,27,S(27,1),0x32,S(26,1),S(27,1),L(1),29,
	
	S(22,1),P(0),

	S(29,1),S(27,1),0x32,S(27,1), S(27,1),0x32,S(29,1), 31,31,S(29,1),0x32,S(22,1),27,27,S(27,1),0x32,S(26,1),S(27,1),L(1),27,S(0,1),
	
	S(29,1),S(27,1),0x32,S(27,1), 0x32,S(29,1),31,0x32,S(29,1),	0x32,S(24,1),26,L(1),27,S(0,1),
	S(27,1),S(27,1),27, S(26,1),0x32,S(24,3),0x32,S(26,1),L(1),0x32,S(27,1),
	0,0,0,0,63
};

int musicmax = sizeof(music);
#endif
