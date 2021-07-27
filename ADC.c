/*
 * ADC.c
 *
 * Created: 18/07/2021 02:14:22 م
 *  Author: oezze
 */ 
#include "ADC.h"

void adc_init()
{
	bin_direction(A,0,input);
	ADMUX = (1<<REFS0)|(1<<REFS1);
	ADCSRA=(1<<ADEN)|(1<<ADPS0);
}

char adc_read()
{
	short int r=0;
	char c;
	short int ch=0;
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	r=ADC;
	c=(char)r/4;
	return c;
}