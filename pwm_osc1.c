
#include "settings.h"
#include "pwm_osc1.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void PWM_OSC1_Enable(void)
{
	TCCR1B =  (1 << CS12);    // Top: ICR1, Div: clk/1 (1 << WGM13) |
	TCCR1A = (1<< WGM10) | (1 << COM1A1) | (1 << COM1B1); // Enable outputs
}

void PWM_OSC1_Set(uint16_t compare_a, uint16_t compare_b ,uint16_t top){
	// f = F_CPU / DIV / ICR1 / 2
    OCR1A = compare_a;
    OCR1B = compare_b;
    //ICR1 = top;
    TCNT1 = 0;
}

void PWM_OSC1_Disable(void){
    TCCR1A = 0;
    TCCR1B = 0;
}

