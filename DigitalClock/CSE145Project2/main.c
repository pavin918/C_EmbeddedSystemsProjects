/*
 * cs145a-proj2.c
 *
 * Created: 4/15/2016 11:16:34 AM
 * Author : Gillian Bendicio and Paul Dao
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned char mode = 1;
unsigned char subsec = 0;
enum clock_state {Month, Day, Year, Hour, Minute, Seconds} clk_state;

typedef struct Date{
	unsigned char month;
	unsigned char day;
	unsigned int year;
} Date;

typedef struct Time{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
} Time;

Date d = {00, 00, 0000};
Time t = {00, 00, 00};
	
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

unsigned char is_leap()
{
	if((d.year % 4) || ((d.year % 100 == 0) && (d.year % 400)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

unsigned char set_feb(unsigned char key, unsigned char count)
{
	if (!is_leap())
	{
		if (key < 9)
		{
			d.day = d.day + key;
			return --count;
		}
	}
	else
	{
		d.day = d.day + key;
		return --count;
	}
}

void set_month(){
	unsigned char key;
	unsigned char cnt = 2;
	while(cnt > 0){
		key = get_key();
		if(key != 0 && key != 10 && key != 12){
			if(key == 11){
				key = 0;
			} 
			if(cnt == 2)
			{
				if(key < 2)
				{
					d.month = d.month + 10 * key;
					cnt--;
				}
			}
			else if(cnt == 1)
			{
				if(d.month < 10)
				{
					if(d.month == 0)
					{
						if(key > 0)
						{
							d.month = d.month + key;
							cnt--;
						}
					}
					else
					{
						d.month = d.month + key;
						cnt--;
					}
				}
				else
				{
					if(key < 3)
					{
						d.month = d.month + key;
						cnt--;
					}
				}
			}
			clr_lcd();
			wait_avr(500);
			display();
		}
	}					
}

void set_day(){
	unsigned char key;
	unsigned char cnt = 2;
	while(cnt > 0){
		key = get_key();
		if(key != 0 && key != 10 && key != 12){
			if(key == 11){
				key = 0;
			}
			if(cnt == 2)
			{
				if(d.month != 2 && key < 4)
				{
					d.day = d.day + 10 * key;
					cnt--;
				}
				else if(d.month == 2 && key < 3){ //handle February
					d.day = d.day + 10 * key;
					cnt--;
				}
			}
			else if(cnt == 1)
			{
				if(d.month != 2)
				{
					if (d.day < 30)
					{
						if(d.day == 0)
						{
							if(key > 0)
							{
								d.day = d.day + key;
								cnt--;
							}
						}
						else
						{
							d.day = d.day + key;
							cnt--;
						}
					}
					else
					{
						if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11){
							if(!key){
								d.day = d.day + key;
								--cnt;
							}
						}
						else
						{
							if(key < 2)
							{
								d.day = d.day + key;
								cnt--;
							}
						}
					}
				}
				else if(d.month == 2)
				{
					if(d.day == 0)
					{
						if(key > 0)
						{
							d.day = d.day + key;
							cnt--;
						}
					}
					else
					{
						d.day = d.day + key;
						cnt--;
					}
				}
			}
			clr_lcd();
			wait_avr(500);
			display();
		}
	}
}

void set_year(){
	unsigned char key[4] = "0000";
	unsigned int k;
	unsigned int cnt = 4;
	unsigned char i = 0;
	unsigned int diff;	
	
	while(i < cnt){
		k = get_key();
		if(k != 0 && k != 10 && k != 12){
			if(k == 11){
				k = 0;
			}
			key[i] = (char)k + '0';
			diff = cnt - i;
			d.year = atoi(key);
			clr_lcd();
			wait_avr(500);
			display();
			i++;
		}
	}
}

void set_date()
{
	unsigned char oldm = d.month;
	unsigned char oldd = d.day;
	unsigned int oldy = d.year;
	set_month();
	set_day();
	set_year();
	while(!is_leap() && d.day == 29 && d.month == 2)
	{
		clr_lcd();
		d.month = oldm;
		d.day = oldd;
		d.year = oldy;
		display();
		set_month();
		set_day();
		set_year();
	}
}

void set_time()
{
	set_hour();
	set_min();
	set_sec();
	while(get_key() != 12)
	{
		//no-op
	}
}

void set_hour(){
	unsigned char key;
	unsigned char cnt = 2;
	while(cnt){
		key = get_key();
		if(key != 0 && key != 10 && key != 12){
			if(key == 11){
				key = 0;
			}
			if(cnt == 2)
			{
				if(key < 3)
				{
					t.hour = t.hour + 10 * key;
					cnt--;
				}
			}
			else if (cnt == 1)
			{
				if(t.hour < 20)
				{
					t.hour = t.hour + key;
					cnt--;
				}
				else
				{
					if(key < 4)
					{
						t.hour = t.hour + key;
						cnt--;
					}
				}
			}
			clr_lcd();
			wait_avr(500);
			display();
		}
	}
}

void set_min(){
	unsigned char key;
	unsigned char cnt = 2;
	while(cnt){
		key = get_key();
		if(key != 0 && key != 10 && key != 12){
			if(key == 11){
				key = 0;
			}
			if(cnt == 2)
			{
				if(key < 6)
				{
					t.min = t.min + 10 * key;
					cnt--;
				}
			}
			else if (cnt == 1)
			{
				t.min = t.min + key;
				cnt--;
			}
			clr_lcd();
			wait_avr(500);
			display();
		}
	}
}

void set_sec(){
	unsigned char key;
	unsigned char cnt = 2;
	while(cnt){
		key = get_key();
		if(key != 0 && key != 10 && key != 12){
			if(key == 11){
				key = 0;
			}
			if(cnt == 2)
			{
				if(key < 6)
				{
					t.sec = t.sec + 10 * key;\
					cnt--;
				}
			}
			else if (cnt == 1)
			{
				t.sec = t.sec + key;
				cnt--;
			}
			clr_lcd();
			wait_avr(500);
			display();
		}
	}
}

void display_date()
{
	pos_lcd(0,0);
	char buf[11];
	sprintf(buf, "%02u/%02u/%04u", d.month, d.day, d.year);
	puts_lcd2(buf);
}

void display_time()
{
	pos_lcd(1,0);
	char buf[11];
	sprintf(buf, "%02u:%02u:%02u", t.hour, t.min, t.sec);
	puts_lcd2(buf);
}

void display()
{
	display_date();
	display_time();
}

void run()
{
	/*
	*
	* Handles running of the clock, incrementing it appropriately
	* with respect to the days of the months, how many seconds in a minute,
	* etc.
	* Also handles leap years
	*
	*/
	if (subsec < 9)
	{
		++subsec;
	}
	else
	{
		subsec = 0;
		if(t.sec < 59)
		{
			++t.sec;
		}
		else if(t.sec == 59)
		{
			t.sec = 00;
			if(t.min < 59){
				++t.min;
			}
			else if (t.min == 59){
				t.min = 00;
				if(t.hour < 23){
					++t.hour;
				}
				else if (t.hour == 23){
					int month = (int)d.month;
					t.hour = 00;
					if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
						if(d.day < 31){
							++d.day;
						}
						else if (d.day == 31){
							d.day = 01;
							if(month == 12){
								d.month = 01;
								++d.year;
							}
							else{
								++d.month;
							}
						}
						
					}
					else if (month == 4 || month == 6 || month == 9 || month == 11){
						if(d.day < 30){
							++d.day;
						}
						else if (d.day == 30){
							d.day = 01;
							++d.month;
						}
					}
					else if (month == 2){
						if(is_leap())
						{
							if(d.day < 29){
								++d.day;
							}
							else if (d.day == 29){
								d.day = 01;
								++d.month;
							}
						}
						else
						{
							if(d.day < 28){
								++d.day;
							}
							else if (d.day == 28){
								d.day = 01;
								++d.month;
							}
						}
					}
				}
			}
		}
	}
	
}

int main(void)
{
    /* Replace with your application code */
	ini_lcd();
	display();
	set_date();
	set_time();
	clr_lcd();
	wait_avr(100);
	unsigned char k;
    while (1) 
    {
		display();
		k = get_key();
		if(k == 10){
			d.month = 0;
			d.day = 0;
			d.year = 0;
			t.hour = 0;
			t.min = 0;
			t.sec = 0;
			display();
			set_date();
			set_time();
		}
		run();
		//wait_avr(10); 
	}
}

