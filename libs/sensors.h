#include <avr/io.h> 
#include <avr/interrupt.h>
#include "usart.h"

extern volatile unsigned int sensor[6];
void sensors_init();
void sensors_loop();
void sensors_debug();
void sensors_update();
SIGNAL (SIG_ADC);
