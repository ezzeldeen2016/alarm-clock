#include "GPIO.h"

void port_direction(char base , char state)
{
    if(state)
        *((volatile unsigned char*)(base+1))=0xff;
    else
        *((volatile unsigned char*)(base+1))=0x00;
}

char port_read(char base)
{
	char data;
	data =*((volatile unsigned char*)(base));
    return data;
}

void port_write(char base , char value)
{
    *((volatile unsigned char*)(base+2))=value;
}
//________________________________________________________________

void bin_direction(char base , char bin, char state)
{
    if(state)
        *((volatile unsigned char*)(base+1))|=(1<<bin);
    else
        *((volatile unsigned char*)(base+1))&=~(1<<bin);
}

char bin_read(char base , char bin)
{
	unsigned char bin_value;
	bin_value =(*(volatile unsigned char *)base) & (1<<bin) ;
	if (bin_value==0) return 0 ;
	else return 1 ;
}

void bin_write(char base , char bin, char data)
{
    if(data)
    *((volatile unsigned char*)(base+2))|=(1<<bin);
    else
    *((volatile unsigned char*)(base+2))&=~(1<<bin);
}

void bin_toggle(char base , char bin)
{
    *((volatile unsigned char*)(base+2))^=(1<<bin);
}

void write_register(char base,char data)
{
	*((volatile unsigned char*)(base))=data;
}

char read_register(char base)
{
	char data;
	data= *((volatile unsigned char*)(base));
	return data;
}

void write_bin(char base,char bin,char data)
{
	if(data)
	base|=(1<<bin);
	else
	base&=~(1<<bin);
}

char read_bin(char base,char bin)
{
	unsigned char bin_value;
	bin_value =base & (1<<bin) ;
	if (bin_value==0) return 0 ;
	else return 1 ;
}

