#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED
#define A 0x39
#define B 0x36
#define C 0x33
#define D 0x30
#define input 0
#define output 1
void port_direction(char base , char state);
char port_read(char base);
void port_write(char base , char value);
void bin_direction(char base , char bin,char state);
char bin_read(char base , char bin);
void bin_write(char base , char bin, char data);
void bin_toggle(char base , char bin);
void write_register(char base,char data);
char read_register(char base);
void write_bin(char base,char bin,char data);
char read_bin(char base,char bin);


#endif // GPIO_H_INCLUDED
