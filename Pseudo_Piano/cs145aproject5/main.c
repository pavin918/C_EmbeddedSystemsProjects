/*
 * cs145aproj5.c
 *
 * Created: 5/30/2016 7:54:41 PM
 * Author : Gillian Bendicio and Paul Dao
 */ 

/*
* This project implements a "piano"
*Each key press will play a note and store it in a song array
*The user has a choice of changing the octave for that note with keys 8 and 9
*They can also change the volume and tempo using key A, B, C, D
*They can also play back the song and reset the song to make a new song
*/

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <string.h>

#define LEN 15
#define H 50000
#define Aper 57//114 
#define ASper 107
#define Bper 51//101
#define Cper 96
#define CSper 90
#define Dper 85
#define DSper 80
#define Eper 76
#define Fper 72
#define Gper 64 // total of 9 notes for keys 1 - 9

unsigned int current_len = 0;
unsigned short tempo[3] = {500, 1000, 1500};
float volume[3] = {0.01, 0.05, 0.50};
float oct_fact[2] = {2.0, 0.5};	
unsigned char oct = 0;
unsigned char temp = 1;
unsigned char vol = 2;
	
typedef struct note
{
	unsigned long duration;
	unsigned long per;
} Note;

Note song[LEN];

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

void playNote(unsigned long duration, unsigned short per){
	unsigned long i;
	
	unsigned long duration2 = duration * tempo[temp] / 1000;
	unsigned long cycles = duration2 / (2 * per);
	
	float s = 1 - volume[vol];
	unsigned long hper = volume[vol]*per; //- shave;
	unsigned long lper = s*per; //+ shave;

	for(i=0; i< cycles; i++){
		SET_BIT(PORTA, 0);
		wait_avr(hper);
		CLR_BIT(PORTA, 0);
		wait_avr(lper);
	}
}
void playMusic(Note song[], unsigned long n){
	unsigned long i;
	unsigned char k;
	for(i=0; i < n; ++i){
		k = get_key();
		switch(k)
		{
			case 13:
			{
				if (temp < 2)
				{
					temp += 1;
					wait_avr(7500);
				}
				break;
			}
			case 14:
			{
				if (temp > 0)
				{
					temp -= 1;
					wait_avr(7500);
				}
				break;
			}

			case 15:
			{
				if (vol < 2)
				{
					vol += 1;
					wait_avr(7500);
				}
				break;
			}
			case 16:
			{
				if (vol > 0)
				{
					vol -= 1;
					wait_avr(7500);
				}
				break;
			}
			default:
			{
				break;
			}
		}
		playNote(song[i].duration, song[i].per);
	}
}

void getNote(unsigned char key){
	//unsigned char key = get_key();
	if(key != 0){
		switch(key){
			case 1:{
				float per = Cper*oct_fact[oct];
				Note ntemp = {H, per};
				song[current_len] = ntemp;	
				current_len++;	
				break;
			}
			case 2:
			{
				Note ntemp = {H, Dper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;				
				
			}
			case 3:
			{
				Note ntemp = {H, Eper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;				
			}
			case 4:
			{
				Note ntemp = {H, Fper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;
			}
			case 5:
			{
				Note ntemp = {H, Gper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;
			}
			case 6:
			{
				Note ntemp = {H, Aper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;
			}
			case 7:
			{
				Note ntemp = {H, Bper * oct_fact[oct]};
				song[current_len] = ntemp;
				current_len++;
				break;
			}
			case 13:
			{
				if (temp < 2)
				{
					temp += 1;
					wait_avr(7500);
				}
				break;
			}
			
			case 8:
			{
				if (oct == 1)
				{
					oct--;
					wait_avr(7500);
				}
				break;
			}
			case 9:
			{
				if (oct == 0)
				{
					oct += 1;
					wait_avr(7500);
				}
				break;
			}
			/*default:
			{
				break;
			}*/
			}
		playNote(song[current_len-1].duration, song[current_len-1].per); //play the note stored
	}
}

void display_status(){
	char buf[20];
	sprintf(buf, "Note %d of %d", current_len, LEN);
	pos_lcd(0,0);
	puts_lcd2(buf);
}

void display_instr(){
	pos_lcd(0, 0);
	puts_lcd2("C:1 D:2 E:3 F:4");//Press * to compose");
	pos_lcd(1,0);
	puts_lcd2("G:5 A:6 B:7");//Press A,B,C,D for others");
}

int main(void)
{
	ini_lcd();
	display_instr();
	DDRA = 1;
    /* Replace with your application code */
    while (1) 
    {
		unsigned char key;
		current_len = 0;
		while(current_len < LEN){
			key = get_key();
			if (key == 10)
			{
				//song = {0};
				current_len = 0;
			}
			else if(key == 12)
			{
				clr_lcd();
				display_status();
				playMusic(song, current_len);
			}
			else
			{
				display_instr();
				getNote(key);
			}
			wait_avr(100);
		}
		
		playMusic(song, LEN);
    }
}

