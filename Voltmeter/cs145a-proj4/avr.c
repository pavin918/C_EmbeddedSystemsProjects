/*
 * avr.c
 *
 * Created: 4/15/2016 11:18:52 AM
 *  Author: Gillian Bendicio and Paul Dao
 */ 
#include "avr.h"

void
ini_avr(void)
{
	WDTCR = 15;
}

void
wait_avr(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}