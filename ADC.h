/*
 * ADC.h
 *
 * Created: 18/07/2021 02:14:46 م
 *  Author: oezze
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include "GPIO.h"
void adc_init();
char adc_read();




#endif /* ADC_H_ */