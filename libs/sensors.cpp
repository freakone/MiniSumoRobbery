#include "sensors.h"
volatile unsigned int sensor[6];
void sensors_init()
{	
	ADCSRA = (1 << ADEN);   
}

void sensors_update()
{
	for(int i = 0; i < 6; i++)
	{
		ADMUX &= 0b11110000; // czyscimy MUX0-3
		ADMUX |= i;
		ADCSRA |= (1 << ADSC);		
		while(ADCSRA & (1<<ADSC));	
		sensor[i] = (ADCL | (ADCH << 8));	
	}
}

void sensors_debug()
{
	uart_put(255);
	uart_put(255);
	for(int i = 0; i < 6; i++)
	{		
		unsigned char one = sensor[i] & 0xff;
		unsigned char two = sensor[i] >> 8;

		uart_put(one);
		uart_put(two);		
		uart_put(one ^ two);
	}
	
	
}

void sensors_loop()
{

}