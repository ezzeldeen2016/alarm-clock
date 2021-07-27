/*
 * LCD.c
 *
 * Created: 3/21/2020 11:02:04 PM
 *  Author: BS
 */ 
#include "LCD.h"

void lcd_init()
{
	if(mode==4)
	{
		if(datanibble==1)
		{
			(*(volatile char*)(data+1)) |=(0xF0);
		}
		bin_direction(lcdport , E, output);
		bin_direction(lcdport, RS, output);
		lcd_command(0x33);
		lcd_command(0x32);
		lcd_command(0x28);
		}
		lcd_command(0x0e);
		lcd_command(0x01);
		_delay_us (2000);
		lcd_command(0x06);
		lcd_command(0x80);
		}
void lcd_command(char com)
{
	char d;
	if(mode==4)
	{
		bin_write(lcdport, RS, 0);
		d=(com&0xF0);
		(*(volatile char*)(command+2)) &=0x0f;
		(*(volatile char*)(command+2))|=d;
		bin_write(lcdport , E, 1);
		_delay_us (1);
		bin_write(lcdport , E, 0);
		_delay_us (100);
		(*(volatile char*)(command+2)) &=0x0f;
		(*(volatile char*)(command+2))|=(com<<4);
		
	}
		bin_write(lcdport , E, 1);
		_delay_us (1);
		bin_write(lcdport, E, 0);
		_delay_us (100);
	
}
void lcd_data(char c)
{
	char d;
	if(mode==4)
	{
		bin_write(lcdport, RS, 1);
		d=(c&0xF0);
		(*(volatile char*)(data+2)) &=0x0f;
		(*(volatile char*)(data+2))|=d;
		bin_write(lcdport , E, 1);
		_delay_us (1);
		bin_write(lcdport, E, 0);
		_delay_us (100);
		(*(volatile char*)(data+2)) &=0x0f;
		(*(volatile char*)(data+2))|=(c<<4);
		
	}
	bin_write(lcdport, E, 1);
	_delay_us (1);
	bin_write(lcdport, E, 0);
	_delay_us (100);
}

void lcd_print(char* str)
{
	unsigned char i=0;
	while(str[i]!=0)
	{

		lcd_data(str[i]);
		i++;
	}
}

