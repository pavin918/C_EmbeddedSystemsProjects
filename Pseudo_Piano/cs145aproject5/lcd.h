/*
 * lcd.h
 *
 * Created: 5/30/2016 7:58:00 PM
 *  Author: Gillian Bendicio and Paul Dao
 */ 
#ifndef __lcd__
#define __lcd__

void ini_lcd(void);
void clr_lcd(void);
void pos_lcd(unsigned char r, unsigned char c);
void put_lcd(char c);
void puts_lcd1(const char *s);
void puts_lcd2(const char *s);

#endif
