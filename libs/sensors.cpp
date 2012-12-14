#include "sensors.h"
#include <stdlib.h>
#define SENS 8
#define PROG 600
#define DAL 40
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
		for(int ii = 0; ii < 2; ii++)
		{	
			while(ADCSRA & (1<<ADSC));	
			sensor[i] = (ADCL | (ADCH << 8));
		}		
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
/*

3--5-4--2
|   7   |
m1	m2
|	|
1-------0

6 > LIPO */

volatile unsigned char situation = 0;
volatile unsigned char ticker = 0;
volatile unsigned char times = 5;
volatile unsigned char side = 0;
void sensors_loop()
{
	if(sensor[0] < PROG && sensor[1] < PROG) // oba dolne
	{
		m1_start(FOR);
		m2_start(FOR);
		situation = 1;
		ticker = 0;
	}
	else
	if(sensor[2] < PROG && sensor[3] < PROG) // oba górne
	{
		m1_start(BCK);
		m2_start(BCK);
		situation = 2;
		ticker = 0;
	}
	else
	if(sensor[2] < PROG) //prawy przedni
	{
		m1_start(BCK);
		m2_start(BCK);
		situation = 5;
		ticker = 0;
	}
	else
	if(sensor[0] < PROG) //prawy tylni
	{
		m1_start(FOR);
		m2_start(FOR);
		situation = 6;
		ticker = 0;
	}
	else
	if(sensor[3] < PROG) //lewy przedni
	{
		m1_start(BCK);
		m2_start(BCK);
		situation = 7;
		ticker = 0;
	}
	else
	if(sensor[1] < PROG) //lewy tylni
	{
		m1_start(FOR);
		m2_start(FOR);
		situation = 8;
		ticker = 0;
	}
	else
	if(situation > 0)
	{
		switch(situation)
		{
			case 2:
				wait_ms(400);
				m1_start(FOR);
				m2_start(BCK);
				wait_ms(1000);
				break;		
			case 5:
			case 6:
				wait_ms(400);
				m1_start(FOR);
				m2_start(BCK);
				wait_ms(600);
				break;	
			case 7:
			case 8:
				wait_ms(400);
				m1_start(BCK);
				m2_start(FOR);
				wait_ms(600);
				break;	
				
		}
		
		situation = 0;
		ticker = 0;
	}
	else
	if(sensor[7] > 500 || (sensor[5] < 750 && sensor[4] < 850)) // dzida
	{
		m1_start(FOR);
		m2_start(FOR);
		ticker = 0;
	}
	else
	if(sensor[4] < 850) // kontruj
	{
		m1_start(BCK);
		m2_start(FOR);
		ticker = 0;
	}
	else 
	if(sensor[5] < 750)
	{
		m1_start(FOR);
		m2_start(BCK);
		ticker = 0;
	}
	else
	{
		ticker++;

		if(ticker < DAL)
		{
			m1_start(FOR);
			m2_start(FOR);
		}
		else if(ticker == DAL)
		{
			side = rand()%2;
			times = 6+rand()%15;
		}
		else if(ticker < DAL + times)
		{	
			if(side == 0)
			{
				m1_start(FOR);
				m2_start(BCK);	
			}
			else
			{
				m1_start(BCK);
				m2_start(FOR);
			}		
		}
		else
			ticker = 0;
		
		
	}

}
