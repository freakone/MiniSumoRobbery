#include "motor.h"
#include <avr/io.h> 
#include <util/delay.h>

volatile char m1_power = 0;
volatile char m2_power = 0; // ustawiona predkosc

volatile char m1_state = 0;
volatile char m2_state = 0; // ustawiont status 0 - STOP, 1 - START

void motor_init(){

	M1_IN1_DDR |= (1 << M1_IN1);
	M1_IN2_DDR |= (1 << M1_IN2);	
	M1_PWM_DDR |= (1 << M1_PWM);
	
	M2_IN1_DDR |= (1 << M2_IN1);
	M2_IN2_DDR |= (1 << M2_IN2);	
	M2_PWM_DDR |= (1 << M2_PWM);
	
	
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	TCCR1B |= (1 << CS11) | (1 << CS10); //inicjalizacja pwma
	
	M1 = 0;
	M2 = 0; // zatrzymujemy silniki
	
	m1_state = 0;
	m2_state = 0; //ustawiamy statusy
}


void m1_start()
{
	if (m1_power > 0)
	{
		M1_IN1_PORT |= (1 << M1_IN1);
		M1_IN2_PORT &= ~(1 << M1_IN2);
		unsigned char tmp = m1_power*255/100; // liczymy moc
		M1 = tmp; // ustawiamy pwma
	}
	else
	{
		M1_IN1_PORT &= ~(1 << M1_IN1);
		M1_IN2_PORT |= (1 << M1_IN2);
		unsigned char tmp = -m1_power*255/100;
		M1 = tmp; // analogicznie
	}
	m1_state = 1;
}

void m1_stop()
{
	M1 = 0;
	m1_state = 0;	
}

void m1_set(char power){
	int tmp = power * m1_power; // mnozymy przez stara wartosc zeby okreslic czy nowy kierunek obrotow jest w ta sama strone
	if (power < -100) power = -100;
	if (power > 100) power = 100; // sprawdzenie, 100% = max
	
	if (tmp < 0) //jesli zmieniamy kierunek obrotow
	{
		m1_power = power; //ustawienie mocy
		if (m1_state == 1) 
		{
			m1_stop(); 
			_delay_ms(20);
			m1_start();
		} // jesli silnik chodzi to stop delay i start, jesli zatrzymany to tylko ustawiamy moc
	}
	else // jesli nie zmieniamy to normalnie
	{
		m1_power = power;
		if (m1_state == 1) 
			m1_start();
	}
}

void m2_start()
{
	if (m2_power > 0)
	{
		M2_IN1_PORT |= (1 << M2_IN1);
		M2_IN2_PORT &= ~(1 << M2_IN2);
		unsigned char tmp = m2_power*255/100; // liczymy moc
		M2 = tmp; // ustawiamy pwma
	}
	else
	{
		M2_IN1_PORT &= ~(1 << M2_IN1);
		M2_IN2_PORT |= (1 << M2_IN2);
		unsigned char tmp = -m2_power*255/100;
		M2 = tmp; // analogicznie
	}
	m2_state = 1;
}

void m2_stop()
{
	M2 = 0;
	m2_state = 0;	
}

void m2_set(char power){
	int tmp = power * m2_power; // mnozymy przez stara wartosc zeby okreslic czy nowy kierunek obrotow jest w ta sama strone
	if (power < -100) power = -100;
	if (power > 100) power = 100; // sprawdzenie, 100% = max
	
	if (tmp < 0) //jesli zmieniamy kierunek obrotow
	{
		m2_power = power; //ustawienie mocy
		if (m2_state == 1) 
		{
			m2_stop(); 
			_delay_ms(20);
			m2_start();
		} // jesli silnik chodzi to stop delay i start, jesli zatrzymany to tylko ustawiamy moc
	}
	else // jesli nie zmieniamy to normalnie
	{
		m2_power = power;
		if (m2_state == 1) 
			m2_start();
	}
}



void motor_debug()
{
	
}

