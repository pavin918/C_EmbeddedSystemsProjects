/*
 * Project3.c
 *
 * Created: 5/2/2016 9:59:04 AM
 * Author : Gillian Bendicio and Paul Dao
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char SONG = 1;
unsigned char temp = 1;
unsigned short tempo[3] = {500, 1000, 1500};
float volume[3] = {0.01, 0.05, 0.50};
unsigned long newper1 = 0;
unsigned long newper2 = 0;
unsigned char vol = 2;
char line1[17] = "1:DearlyBeloved\0";
char line2[15] = "2:Jingle Bells\0";
#define A 440
#define Aper 114
#define AS 466.164
#define ASper 107
#define C 523.251
#define Cper 96
#define CS 554.365
#define CSper 90
#define D 587.33
#define Dper 85
#define DS 622.254
#define DSper 80 //Eb or D#
#define E 659.255
#define Eper 76
#define F 698.456
#define Fper 72
#define G 783.991
#define Gper 64
#define F4 349.228
#define F4per 143
#define G4 391.995
#define G4per 128
#define W 100000//200000
#define H 50000//100000
#define Q 25000//50000
#define ET 12500//25000
#define S 6250//12500 //sixteenth
#define TS 3125//6250 //32nd
#define W2 400000
#define H2 200000
#define Q2 100000
#define ET2 50000
#define S2 25000
#define TS2 12500
#define P 15000
#define P2 1000
#define P3 900
#define DET 18750//37500
#define DET2 37500
#define DQ2 75000
#define DQ 37500//75000
typedef struct note
{
	unsigned short freq;
	unsigned long duration;
	unsigned short per;
} Note;

unsigned char pressed (unsigned char r, unsigned char c){
	DDRC = 0x00;
	PORTC = 0xFF;
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	wait_avr(10);
	if(GET_BIT(PINC, c+4)){
		return 0;
	}
	return 1;
}

unsigned char get_key(){
	unsigned char r, c;
	for(r = 0; r < 4; r++){
		for(c = 0;c < 4; c++){
			if(pressed(r,c)){
				unsigned char result = (r * 4) + c - r + 1; //4 for A, 7 for B, 10 for C, 13 for D
				if (c != 3)
				{
					return result;
				}
				else
				{
					return result + (9 - 2 * r); // returns 13 for A, 14 for B, 15 for C, 16 for D
				}
			}
		}
	}
	return 0;

}

Note mySong [66] = {//132] = {
	{C, DET2, Cper}, {1, P, 1}, {C, S2, Cper}, {1, P2, 1},
	{G4, DET2, G4per}, {1, P, 1}, {G4, S2, G4per}, {1, P2, 1},
	{F4, DET2, F4per}, {1, P, 1}, {F4, S2, F4per}, {1, P2, 1},
	{D, DET2, Dper}, {1, P, 1}, {D, S2, Dper}, {1, P2, 1},
		
	{C, DET2, Cper}, {1, P, 1}, {C, S2, Cper}, {1, P2, 1},
	{G4, DET2, G4per}, {1, P, 1}, {G4, S2, G4per}, {1, P2, 1},
	{F4, DET2, F4per}, {1, P, 1}, {F4, S2, F4per}, {1, P2, 1}, 
	{D, DET2, Dper}, {1, P, 1}, {D, S2, Dper}, {1, P2, 1},
		
	{DS, DET2, DSper}, {1, P, 1}, {DS, S2, DSper}, {1, P2, 1},
	{D, DET2, Dper}, {1, P, 1}, {D, S2, Dper}, {1, P2, 1},
	{G, DET2, Gper}, {1, P, 1}, {G, S2, Gper}, {1, P2, 1},
	{F, TS2, Fper}, {G, TS2, Gper},
	{F, ET2, Fper}, {1, P3, 1}, {F, S2, Fper}, {1, P2, 1}, //end of 1st line
		
	{DS, DET2, DSper}, {1, P, 1}, {DS, S2, DSper}, {1, P2, 1},
	{D, DET2, Dper}, {1, P, 1}, {D, S2, Dper}, {1, P2, 1},
	{C, DET2, Cper}, {1, P, 1}, {C, S2, Cper}, {1, P2, 1},
	{AS, DET2, ASper}, {1, P, 1}, {AS, S2, ASper}, {1, P2, 1},	
};

Note newSong [] = {
	{E, Q, Eper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, {E, H, Eper}, {1, P2, 1},
	{E, Q, Eper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, {E, H, Eper}, {1, P2, 1},
	{E, Q, Eper}, {1, P2, 1}, {G, Q, Gper}, {C, DQ, Cper}, {1, P2, 1},
	{D, ET, Dper}, {E, W, Eper}, {1, P2, 1}, //end of 1st line 22
		
	{F, Q, Fper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1}, {F, DQ, Fper}, {1, P2, 1},
	{F, ET, Fper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, 
	{E, Q, Eper}, {1, P2, 1}, {E, ET, Eper}, {1, P2, 1}, {E, ET, Eper}, 
	{1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, {D, Q, Dper}, {1, P2, 1}, {D, Q, Dper},
	{1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, // end of 2nd line 26
		
	{D, H, Dper}, {1, P2, 1}, {G, H, Gper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1},
	{E, Q, Eper}, {1, P2, 1}, {E, H, Eper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1},
	{E, Q, Eper}, {1, P2, 1}, {E, H, Eper}, {1, P2, 1}, // end of 3rd line 16
		
	{E, Q, Eper}, {1, P2, 1}, {G, Q, Gper}, {1, P2, 1}, {C, DQ, Cper}, {1, P2, 1},
	{D, ET, Dper}, {1, P2, 1}, {E, W, Eper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1}, 
	{F, Q, Fper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1}, 
	{F, Q, Fper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, {E, Q, Eper}, {1, P2, 1}, 
	{E, ET, Eper}, {1, P2, 1}, {E, ET, Eper}, {1, P2, 1}, // end of 4th line 28
	
	{G, Q, Gper}, {1, P2, 1}, {G, Q, Gper}, {1, P2, 1}, {F, Q, Fper}, {1, P2, 1},
	{D, Q, Dper}, {1, P2, 1}, {C, W, Cper}, {1, P2, 1} //10
};

void playNote(unsigned short freq, unsigned long duration, unsigned short per){
	unsigned long i;
	unsigned short k;

	k = get_key();

	if (k == 13)
	{
		if (temp < 2)
		{
			temp += 1;
			wait_avr(7500);
		}
	}
	else if(k == 14)
	{
		if (temp > 0)
		{
			temp -= 1;
			wait_avr(7500);
		}
	}

	else if (k == 15)
	{
		if (vol < 2)
		{
			vol += 1;
			wait_avr(7500);
		}
	}
	else if(k == 16)
	{
		if (vol > 0)
		{
			vol -= 1;
			wait_avr(7500);
		}
	}
	
	unsigned long duration2 = duration * tempo[temp] / 1000;
	unsigned long cycles = duration2 / (2 * per);
	
	float s = 1 - volume[vol];
	unsigned short hper = volume[vol]*per; //- shave;
	unsigned short lper = s*per; //+ shave;

	for(i=0; i< cycles; i++){
		SET_BIT(PORTA, 0);
		wait_avr(hper);
		CLR_BIT(PORTA, 0);
		wait_avr(lper);
	}
}
void playMusic(Note song[], unsigned long n){
	unsigned long i;
	for(i=0; i < n; ++i){
		playNote(song[i].freq, song[i].duration, song[i].per);
	}
}

void display(){
	pos_lcd(0, 0);
	puts_lcd2(line1);//"1:Dearly Beloved\0");
	pos_lcd(1, 0);
	puts_lcd2(line2);//"2:Jingle Bells\0");
}

void set_song(){
	
	unsigned char key = 0;
	unsigned char flag = 0;
	char buf3[8];
	char buf4[16];
	while (flag != 1){
		key = get_key();
		if(key == 1){
			SONG = 1;
			flag = 1;
		}
		else if (key == 2){
			SONG = 2;
			flag = 1;
		}
		wait_avr(1000);
	}
}

int main(void)
{
    /* Replace with your application code */
	ini_lcd();
	display();
	DDRA = 1;
    while (1) 
    {
		
		wait_avr(1000);
		set_song();
		if(SONG == 1){
			playMusic(mySong, 66); //132
			wait_avr(1000);
		}
		else if (SONG == 2){
			playMusic(newSong, 100);
			wait_avr(1000);
		}
    }
}



