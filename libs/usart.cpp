#include "usart.h"
#include "motor.h"

volatile unsigned char bEnable;
volatile unsigned char bDebugEnable;
volatile unsigned int w;

void wait_ms(int ms) {
  int i;
  for (i=0; i<ms; i++) {
    _delay_ms(1);
  }
}

void uart_init()
{	
	UBRR0H = (unsigned char)(BAUD_PRESCALE>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALE;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);	
	bEnable = 0;
	bDebugEnable = 0;
	w = 0;
}

unsigned char uart_receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) ) 	
		;			                

	return UDR0;
}

ISR(USART_RX_vect)
{
	char buffer[5];
	switch(UDR0)
	{
		case 'a':
			bEnable = 255;			
			m1_start(FOR);
			m2_start(FOR);
			break;
		case 'b':
			bEnable = 0;
			m1_stop();
			m2_stop();
			break;
		case 'c':
			bDebugEnable = bDebugEnable == 0 ? bDebugEnable = 255 : bDebugEnable = 0;
			break;
		case 'd':
			m2_change(5);
			uart_put('\n');	
			uart_put(m1_getspeed());	
			uart_put(' ');	
			uart_put(m2_getspeed());	
			break;
		case 'e':
			m2_change(-5);
			uart_put('\n');	
			uart_put(m1_getspeed());	
			uart_put(' ');	
			uart_put(m2_getspeed());			
			break;
		case 'f':
			m1_start(FOR);
			m2_start(BCK);
			wait_ms(w);
			m1_stop();
			m2_stop();
			break;
		case 'g':
			w+=20;
			uart_put('\n');	
			uart_put(w);
			break;
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
