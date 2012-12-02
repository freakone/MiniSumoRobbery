#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/usart.h"
#include "libs/sensors.h"
#include "libs/motor.h"

void blink()
{
		PORTD |= (1 << PD2);
		_delay_ms(50);
		PORTD &= ~(1 << PD2);
		_delay_ms(50);
}



ISR(INT1_vect)
{ 
   PORTD ^= (1 << PD4);   

   m2_set(50);
   m2_start();
}

int main()
{	
	DDRD = 0xff;
	uart_init();
	sensors_init();
	motor_init();
	
	EIMSK = (1 << INT1);
	EICRA = (1 << ISC11) | (1 << ISC10);	
	sei();	
	
	while(1)
	{
		blink();	
		sensors_update();	
		sensors_debug();   		
	}
}