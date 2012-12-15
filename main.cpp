#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/usart.h"
#include "libs/sensors.h"
#include "libs/motor.h"

void blink()
{
	PORTD |= (1 << PD2);
	_delay_ms(20);
	PORTD &= ~(1 << PD2);
	_delay_ms(20);
}

ISR(INT1_vect)
{ 
	bEnable = 255;
}

bool battery_check()
{
	if(sensor[6] < 580)
	{
		PORTD |= (1 << PD4);
		return true;
	}

	PORTD &= ~(1 << PD4);
	return false;
}

int main()
{
	DDRD = 0xff;	
	//DDRD = ~(1 << PD3);
//	PORTD |= (1 << PD3);
	uart_init();
	sensors_init();
	motor_init();

	
	EICRA = (1 << ISC10)|(1 << ISC11);
	EIMSK = (1 << INT1);
	sei();		
	

	m1_set(210);
	m2_set(255);

	while(1)
	{
		blink();	
		sensors_update();
		
		if(battery_check())
			continue;

		if(bDebugEnable == 255)	
			sensors_debug();   
		
		if(bEnable == 255)
			sensors_loop();
	
			
	}
}
