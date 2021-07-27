/*
 * RTC.c
 *
 * Created: 04/07/2021 12:42:30 ص
 *  Author: oezze
 */ 
#include "LCD.h"
#include "GPIO.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ADC.h"
#define F_CPU 1000000UL
void Display_row1(char temp,char hours,char minutes,char AM_PM);
void Display_row2(char day,char monthes,int year,char week_day);
char leap_year(int year);
#define right 0
#define left  1
#define increment 0
#define decrement 1
unsigned int seconds=0;

char state=0;
//_______________________________Timer interrupt_____________________________

ISR (TIMER0_OVF_vect)    // Timer1 ISR
{
	seconds++;
	TCNT0 = 0;   //for 250ms delay
}
int main(void)
{
	lcd_init();
	lcd_command(0x0C); //curser off
	char day,month;
	int year=2013;
	char hours,minutes,t;
	char AM_PM=1;
	char cursor=5;
	char line=0;
	char week_day=7;
	char alarm_hour=12;
	char alarm_minuts=0;
	char alarm_pm_am=0;
	char alarm_set_reset=1;
	hours=11;
	minutes=59;
	day=28;
	month=2;
	t=37;
	//_______________________________________________________
	char input_state=0;
	bin_direction(B, 0,input);
	bin_direction(B, 1,input);
	bin_direction(B, 2,input);
	bin_direction(B, 3,input);
	bin_direction(B, 4,input);
	bin_direction(B, 5,input);
	bin_direction(D , 7,output);
	adc_init();
	//________________________________________________________
	TCNT0 = 0; 
	TCCR0 |= (1<<CS02) | (1<<CS00);
	TIMSK|=(1<<TOIE0);
	sei(); //enable global interrupt
	while(1)
	{
		if(state==0)
		{
		t=adc_read();
		Display_row1(t,hours,minutes,AM_PM);
		Display_row2(day,month,year,week_day);
		if(alarm_set_reset==1)
		{
			if(alarm_hour==hours && alarm_minuts==minutes && AM_PM==alarm_pm_am)
			{
				bin_write(D, 7, 1);
			}
			
		}
		//______________________________alarm is off________________________________
		if(bin_read(B , 5)==1)
		{
			input_state|=(1<<5);
		}
		if((input_state&(1<<5))==(1<<5) && (bin_read(B , 5)==0))
		{
			bin_write(D, 7, 0);
			alarm_set_reset=0;
			input_state&= ~(1<<5);
		}
		if(seconds==180)
		{
			seconds=0;
			if(minutes==59 && hours==11)
			{
				minutes=0;
				hours=12;
				AM_PM^=(1<<0);
				if(AM_PM==0)
				{
					if(week_day==7)
					{
						week_day=1;
					}
					else
					{
						week_day++;
					}
					//__________________________________________________________________________________________
					
					if(month==1 ||month==3 ||month==5 || month==7 ||month==8 ||month==10 ||month==12)
					{
						if(day==31)
						{
							day=1;
							if(month==12)
							{
								month=1;
								year++;
							}
							else
							{
								month++;
								
							}
						}
						else
						{
							day++;
						}
					}
					else if(month==2)
					{
						if(leap_year(year) && day==28)
						{
							day=29;
						}
						else if(day==28){
							day=1;
							month++;
						}
					}
					else
					{
						day++;
					}
					//_________________________________________________________________________________
					
				}
				else if(hours==12)
				{
					hours=1;
				}
				else
				{
					hours++;
				}
		}
		else 
		{
			minutes++;
		}
	
	
	}
		//_____________reading input______________________________
		if(bin_read(B , 0)==1)
		{
			input_state|=(1<<0);
		}
		if((input_state&(1<<0))==(1<<0) && (bin_read(B , 0)==0))
		{
			settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
			lcd_command(0x80);
			move_cursor(cursor,right);
			state=1;
			input_state&= ~(1<<0);
		}
		//__________end reading input_____________________________
		}
		else if(state==1)
		{
			
			//______________________move cursor right______________________________________
			if(bin_read(B , 3)==1)
			{
				input_state|=(1<<3);
			}
			if((input_state&(1<<3))==(1<<3) && (bin_read(B , 3)==0))
			{
				if(line==0)
				{
					switch(cursor)
					{
						case 5:
						cursor=8;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 8:
						cursor=10;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 10:
						cursor=14;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 14:
						line=1;
						cursor=5;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
						
					}
				}
				else if(line==1)
				{
				switch(cursor)
				{
					case 5:
					cursor=8;
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0xC0);
					move_cursor(cursor,right);
					break;
					case 8:
					cursor=13;
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0xC0);
					move_cursor(cursor,right);
					break;
					case 13:
					line=0;
					cursor=5;
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				}
				input_state&= ~(1<<3);
			}
			//___________________________move cursor left_________________________________________________
			if(bin_read(B , 4)==1)
			{
				input_state|=(1<<4);
			}
			if((input_state&(1<<4))==(1<<4) && (bin_read(B , 4)==0))
			{
				if(line==0)
				{
					switch(cursor)
					{
						case 5:
						cursor=5;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 8:
						cursor=5;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 10:
						cursor=8;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 14:
						cursor=10;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						
					}
				}
				else if(line==1)
				{
					switch(cursor)
					{
						case 5:
						cursor=14;
						line=0;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 8:
						cursor=5;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
						case 13:
						cursor=8;
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
					}
				}
				input_state&= ~(1<<4);
			}
			//______________________increment_____________________________________________________________
			if(bin_read(B , 1)==1)
			{
				input_state|=(1<<1);
			}
			if((input_state&(1<<1))==(1<<1) && (bin_read(B , 1)==0))
			{
				switch(cursor)
				{
					case 5: //hours and date day
					switch(line)
					{
						case 0:
						if(hours==12)
						{
							hours=1;
						}
						else
						{
							hours++;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 1:
						//date(&day,&month,&year,leap_year(year));
						if(month==1 ||month==3 ||month==5 || month==7 ||month==8 ||month==10 ||month==12)
						{
							if(day==31)
							{
								day=1;
								if(month==12)
								{
									month=1;
									year++;
								}
								else
								{
									month++;
									
								}
							}
							else
							{
								day++;
							}
						}
						else if(month==2)
						{
							if(leap_year && day==28)
							{
								day=29;
							}
							else if(day==28){
								day=1;
								month++;
							}
						}
						else
						{
							day++;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
					}

					break;
					case 8: //minutes and months
					switch(line)
					{
						case 0:
						if(minutes==59)
						{
							minutes=0;
						}
						else
						{
							minutes++;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 1:
						if(month==12)
						{
							month=1;
						}
						else
						{
							month++;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
					}
					break;
					case 10:
					AM_PM^=(1<<0);
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 13:
					year++;
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0xC0);
					move_cursor(cursor,right);
					break;
					case 14:
					if(week_day==7)
					{
						week_day=1;
					}
					else
					{
						week_day++;
					}
					settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				input_state&= ~(1<<1);
			}
			//____________________________________________decrement___________________________________________________________
			if(bin_read(B , 2)==1)
			{
				input_state|=(1<<2);
			}
			if((input_state&(1<<2))==(1<<2) && (bin_read(B , 2)==0))
			{
				if(line==0)
				{
					switch(cursor)
					{
						case 5:
						if(hours==1)
						{
							hours=12;
						}
						else
						{
							hours--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 8:
						if(minutes==0)
						{
							minutes=59;
							
						}
						else
						{
							minutes--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 10:
						AM_PM^=(1<<0);
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
						case 14:
						if(week_day==1)
						{
							week_day=7;
						}
						else
						{
							week_day--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0x80);
						move_cursor(cursor,right);
						break;
					}
					
				}
				else if(line==1)
				{
					switch(cursor)
					{
						case 5:
						if(day==1)
						{
							day=1;
						}
						else
						{
							day--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
						case 8:
						if(month==1)
						{
							month=1;
									
						}
						else
						{
							month--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
						case 13:
						if(year==0)
						{
							year=0;
						}
						else
						{
							year--;
						}
						settings(&day,&month,&year,&hours,&minutes,&AM_PM,&week_day);
						lcd_command(0xC0);
						move_cursor(cursor,right);
						break;
						
					}
					
				}
				input_state&= ~(1<<2);
			}
			if(bin_read(B , 0)==1)
			{
				input_state|=(1<<0);
			}
			if((input_state&(1<<0))==(1<<0) && (bin_read(B , 0)==0))
			{
				state=2;
				lcd_command(0x0E);
				lcd_command(0x01);
				lcd_command(0x02);
				lcd_command(0x18);
				cursor=5;
				alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
				lcd_command(0x80);
				move_cursor(cursor,right);
				input_state&= ~(1<<0);
			}
			
			
		}
		else if(state==2)
		{
		
			//_____________________back to state 0__________________________________________________
			if(bin_read(B , 0)==1)
			{
				input_state|=(1<<0);
			}
			if((input_state&(1<<0))==(1<<0) && (bin_read(B , 0)==0))
			{
				
				state=0;
				input_state&= ~(1<<0);
			}
			//____________________________increment__________________________________________________
			if(bin_read(B , 1)==1)
			{
				input_state|=(1<<1);
			}
			if((input_state&(1<<1))==(1<<1) && (bin_read(B , 1)==0))
			{
				
				switch(cursor)
				{
					case 5:
					if(alarm_hour==12)
					{
						alarm_hour=1;
					}
					else
					{
						alarm_hour++;
					}
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 8:
					if(alarm_minuts==59)
					{
						alarm_minuts=0;
					}
					else
					{
						alarm_minuts++;
					}
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 10:
					alarm_pm_am^=(1<<0);
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 13:
					alarm_set_reset^=(1<<0);
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				input_state&= ~(1<<1);
			}
			//____________________________decrement____________________________________________
			if(bin_read(B , 2)==1)
			{
				input_state|=(1<<2);
			}
			
			if((input_state&(1<<2))==(1<<2) && (bin_read(B , 2)==0))
			{
				
				switch(cursor)
				{
					case 5:
					if(alarm_hour==1)
					{
						alarm_hour=12;
					}
					else
					{
						alarm_hour--;
					}
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 8:
					if(alarm_minuts==0)
					{
						alarm_minuts=59;
					}
					else
					{
						alarm_minuts--;
					}
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 10:
					alarm_pm_am^=(1<<0);
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 13:
					alarm_set_reset^=(1<<0);
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				input_state&= ~(1<<2);
			}
			//____________________________shift right_________________________________________
			if(bin_read(B , 3)==1)
			{
				input_state|=(1<<3);
			}
			
			if((input_state&(1<<3))==(1<<3) && (bin_read(B , 3)==0))
			{
				switch(cursor)
				{
					case 5:
					cursor=8;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 8:
					cursor=10;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 10:
					cursor=13;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				input_state&= ~(1<<3);
			}
			//___________________________shift left____________________________________________
			if(bin_read(B , 4)==1)
			{
				input_state|=(1<<4);
			}
			
			if((input_state&(1<<4))==(1<<4) && (bin_read(B , 4)==0))
			{
				switch(cursor)
				{
					case 5:
					cursor=5;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 8:
					cursor=5;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 10:
					cursor=8;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
					case 13:
					cursor=10;
					alarm_set(&alarm_hour,&alarm_minuts,&alarm_pm_am,&alarm_set_reset);
					lcd_command(0x80);
					move_cursor(cursor,right);
					break;
				}
				input_state&= ~(1<<4);
			}
		}

		
	}
}

void Display_row1(char t,char hours,char minutes,char AM_PM)
{
	char temp;
	lcd_command(0x80);			
	temp=(hours/10)+48;
	lcd_data(temp);
	temp=(hours%10)+48;
	lcd_data(temp);
	lcd_data(':');
	temp=(minutes/10)+48;
	lcd_data(temp);
	temp=(minutes%10)+48;
	lcd_data(temp);
	lcd_data(' ');
	switch(AM_PM)
	{
		case 0:
		lcd_print("AM");
		break;
		case 1:
		lcd_print("PM");
		break;
	}
	lcd_print("    ");
	lcd_print("T=");
	temp=(t/10)+48;
	lcd_data(temp);
	temp=(t%10)+48;
	lcd_data(temp);
	lcd_data('C');
}
void Display_row2(char day,char monthes,int year,char week_day)
{
	char temp;
	lcd_command(0xC0);
	switch(week_day)
	{
		case 1:
		lcd_print("SAT");
		break;
		case 2:
		lcd_print("SUN");
		break;
		case 3:
		lcd_print("MON");
		break;
		case 4:
		lcd_print("TUE");
		break;
		case 5:
		lcd_print("WED");
		break;
		case 6:
		lcd_print("THU");
		break;
		case 7:
		lcd_print("FRI");
		break;
	}
	
	
	lcd_print("   ");
	
	temp=(day/10)+48;
	lcd_data(temp);
	temp=(day%10)+48;
	lcd_data(temp);
	lcd_data('/');
	temp=(monthes/10)+48;
	lcd_data(temp);
	temp=(monthes%10)+48;
	lcd_data(temp);
	lcd_data('/');
	temp=(year/1000)+48;
	lcd_data(temp);
	temp=((year%1000)/100)+48;
	lcd_data(temp);
	temp=((year%100)/10)+48;
	lcd_data(temp);
	temp=(year%10)+48;
	lcd_data(temp);
	lcd_command(0x0C);
}

void settings(char* day,char* month,int* year,char* hour,char* minute,char* day_night,char*week_day)
{
	int temp;
	lcd_command(0x01);
	lcd_command(0x02);
	lcd_command(0x18);
	lcd_command(0x80);
	lcd_print("Time:");
	temp=(*(hour)/10)+48;
	lcd_data(temp);
	temp=(*(hour)%10)+48;
	lcd_data(temp);
	lcd_data(':');
	temp=(*(minute)/10)+48;
	lcd_data(temp);
	temp=(*(minute)%10)+48;
	lcd_data(temp);
	switch(*(day_night))
	{
		case 0:
		lcd_print("AM");
		break;
		case 1:
		lcd_print("PM");
		break;
	}
	lcd_print(" ");
	switch(*(week_day))
	{
		case 1:
		lcd_print("SAT");
		break;
		case 2:
		lcd_print("SUN");
		break;
		case 3:
		lcd_print("MON");
		break;
		case 4:
		lcd_print("TUE");
		break;
		case 5:
		lcd_print("WED");
		break;
		case 6:
		lcd_print("THU");
		break;
		case 7:
		lcd_print("FRI");
		break;
	}
	lcd_command(0xC0);
	lcd_print("Date:");
	temp=(*day/10)+48;
	lcd_data(temp);
	temp=(*day%10)+48;
	lcd_data(temp);
	lcd_data('/');
	temp=(*month/10)+48;
	lcd_data(temp);
	temp=(*month%10)+48;
	lcd_data(temp);
	lcd_data('/');
	temp=(*year/1000)+48;
	lcd_data(temp);
	temp=((*year%1000)/100)+48;
	lcd_data(temp);
	temp=((*year%100)/10)+48;
	lcd_data(temp);
	temp=(*year%10)+48;
	lcd_data(temp);
	lcd_command(0x0f);
}
void move_cursor(char count,char left_right)
{
	char i;
	for(i=0;i<count;i++)
	{
		if(left_right==1)
		{
			lcd_command(0x10);
		}
		else if(left_right==0)
		{
			lcd_command(0x14);
		}
	}
}
void alarm_set(char*hours,char*minuts,char*am_pm,char*set_reset)
{
	char temp;
	lcd_command(0x80);
	lcd_print("Time:");
	temp=(*(hours)/10)+48;
	lcd_data(temp);
	temp=(*(hours)%10)+48;
	lcd_data(temp);
	lcd_data(':');
	temp=(*(minuts)/10)+48;
	lcd_data(temp);
	temp=(*(minuts)%10)+48;
	lcd_data(temp);
	switch(*(am_pm))
	{
		case 0:
		lcd_print("AM");
		break;
		case 1:
		lcd_print("PM");
		break;
	}
	lcd_print(" ");
	switch(*(set_reset))
	{
		case 0:
		lcd_print("OFF");
		break;
		case 1:
		lcd_print("ON ");
		break;
	}
	lcd_command(0x0f);
	
	
}

char leap_year(int year)
{
	if(((year%4==0) && ((year%400==0) || (year%100!=0))))
	{
		return 1;
	} else {
		return 0;
	}
}

