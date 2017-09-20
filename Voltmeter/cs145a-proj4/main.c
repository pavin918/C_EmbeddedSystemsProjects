/*
 * cs145aproj4.c
 *
 * Created: 5/18/2016 11:10:44 AM
 * Author : Gillian Bendicio and Paul Dao
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>



volatile unsigned short TimerFlag = 0;
unsigned short MIN;
unsigned short MAX;
unsigned short average;
unsigned short current;
unsigned long sum = 0;
unsigned long cnt = 0;
char min_string[17];
char max_string[17];
char avg_string[17];
char curr_string[17];
unsigned short dvrange = 1024;
unsigned char avrange = 5;

enum voltmeter_sm {vm_start, vm_init, vm_read} voltmeter_state;
	
void d2a(unsigned short dvolt, char* c)
{
	unsigned short voltage = (dvolt * 5.0) / 1.023;
	//char buf[17];
	sprintf(c, "%u.%u%u%u",
	voltage / 1000,
	(voltage % 1000) / 100,
	(voltage % 100) / 10,
	(voltage % 10));
}

void get_min(unsigned short i){
	if(cnt == 1){
		MIN = i;
	}
	else if (MIN > i){
		MIN = i;
	}
}

void get_max(unsigned short i){
	if(cnt == 1){
		MAX = i;
	}
	else if (MAX < i){
		MAX = i;
	}
}

void get_average(unsigned short i){
	average = sum/cnt;
}

void get_sum(unsigned short i){
	sum += i;
	cnt++;
}

unsigned short sample_ADC()
{
	ADCSRA = (1 << ADPS2) | (1 << ADPS1);
	ADCSRA |= (1 << ADEN) | (1 << ADSC);
	while(GET_BIT(ADCSRA, ADSC));
	unsigned short temp = ADC;
	CLR_BIT(ADCSRA, ADSC);
	return temp;
}

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
		for(c = 0;c < 3; c++){
			if(pressed(r,c)){
				return (r * 4) + c - r + 1;
			}
		}
	}
	return 0;
}

void vm_tick()
{
	switch(voltmeter_state)
	{
		case(vm_start):
		{
			voltmeter_state = vm_init;
			break;
		}
		case(vm_init):
		{
			//clr_lcd();
			pos_lcd(0,0);
			if (get_key() == 2)
			{
				voltmeter_state = vm_read;
			}
			else
			{
				voltmeter_state = vm_init;
				MIN = 0;
				MAX = 0;
				average = 0;
				current = 0;
				sum = 0;
				cnt = 0;
			}
			break;
		}
		case(vm_read):
		{
			//clr_lcd();
			pos_lcd(0, 0);
			if (get_key() == 1)
			{
				voltmeter_state = vm_init;
			}
			else
			{
				voltmeter_state = vm_read;
				current = sample_ADC();
				get_min(current);
				get_max(current);
				get_sum(current);
				get_average(current);
			}
			break;
		}
	}
	
	switch(voltmeter_state)
	{
		case(vm_start):
		{
			break;
		}
		case(vm_init):
		{
			//d2a(current, curr_string);
			pos_lcd(0, 0);
			puts_lcd2("-.---");
			
			//d2a(average, avg_string);
			pos_lcd(0, 6);
			puts_lcd2("-.---");
			
			//d2a(MAX, max_string);
			pos_lcd(1, 0);
			puts_lcd2("-.---");
			
			//d2a(MIN, min_string);
			pos_lcd(1, 6);
			puts_lcd2("-.---");
			
			break;
		}
		case(vm_read):
		{
			d2a(current, curr_string);
			pos_lcd(0, 0);
			puts_lcd2(curr_string);
			
			d2a(average, avg_string);
			pos_lcd(0, 6);
			puts_lcd2(avg_string);
			
			d2a(MAX, max_string);
			pos_lcd(1, 0);
			puts_lcd2(max_string);
			
			d2a(MIN, min_string);
			pos_lcd(1, 6);
			puts_lcd2(min_string);
			break;
		}
	}
}


int main(void)
{
    /* Replace with your application code */
	ADMUX = 1 << REFS0;
	ini_lcd();
	voltmeter_state = vm_start;
    while (1) 
    {
		vm_tick();
		
		wait_avr(500);
		//while(!TimerFlag);
		//TimerFlag = 0;
    }
}

