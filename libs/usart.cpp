#include "usart.h"
#include "motor.h"

void uart_init()
{	
	UBRR0H = (unsigned char)(BAUD_PRESCALE>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALE;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);	
}

unsigned char uart_receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) ) 	
		;			                

	return UDR0;
}

ISR(USART_RX_vect)
{
	if(UDR0 == 0x11)
	{
		UCSR0B &= ~(1<<RXCIE0);
		char l = uart_receive();
		char dirl = uart_receive();
		char p = uart_receive();
		char dirp = uart_receive();
		
		if(l == 0)
			m1_stop();
		else
		{
			m1_set(dirl == 0? -1*l : l);
			m1_start();
		}
		
		if(p == 0)
			m2_stop();
		else
		{
			m2_set(dirp == 0? -1*p : p);
			m2_start();
		}
		
		UCSR0B |= (1<<RXCIE0);
	}
}

void uart_put( unsigned char data )
{
	while(!( UCSR0A & (1<<UDRE0)));
	UDR0 = data;		        
}

void uart_puts(const char *s )
{
    while (*s)
      uart_put(*s++);
}