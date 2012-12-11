#define M1_IN1_PORT PORTB //SILNIK LEWY
#define M1_IN1_DDR DDRB
#define M1_IN1 PB0

#define M1_IN2_PORT PORTD
#define M1_IN2_DDR DDRD
#define M1_IN2 PD7

#define M1_PWM_DDR DDRB
#define M1 OCR1B
#define M1_PWM PB2 


#define M2_IN1_PORT PORTD //SILNIK PRAWY
#define M2_IN1_DDR DDRD
#define M2_IN1 PD5

#define M2_IN2_PORT PORTD
#define M2_IN2_DDR DDRD
#define M2_IN2 PD6

#define M2_PWM_DDR DDRB
#define M2 OCR1A
#define M2_PWM PB1

void motor_init();
void motor_debug();
void m1_start();
void m1_stop();
void m1_set(char power);
void m2_start();
void m2_stop();
void m2_set(char power);
