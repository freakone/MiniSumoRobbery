#include "sensors.h"
#define SENS 8
volatile unsigned int sensor[SENS];
void sensors_init()
{	
	DDRC = 0;
	ADCSRA = (1 << ADEN);  	
}

void sensors_update()
{
	for(int i = 0; i < SENS; i++)
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
	if(bDebugEnable == 0)
		return;

	uart_put(28);
	uart_put(28);
	for(int i = 0; i < SENS; i++)
	{		
		unsigned char one = sensor[i] & 0xff;
		unsigned char two = sensor[i] >> 8;

		uart_put(one);
		uart_put(two);		
		uart_put(one ^ two);
	}
	
	
}

// 0 > LEWY TYL
//1 > prawy tyl
//2 > prawy przod
//3 > lewy przod
void sensors_loop()
{
	if(sensor[0] < 900 || sensor[1] < 900)
	{
		m1_set(50);
		m2_set(75);
	}
	else
	if(sensor[2] < 750 || sensor[3] < 750)
	{
		m1_set(-50);
		m2_set(-75);
	}
	else
	{
		//szukanko
		
	}

}
