
#include "cli_parser.h"
#include "usart_serial.h"
#include "i2c_master.h"
#include "adc.h"
#include "pwm_osc1.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

void rxCallback(uint8_t ch){
    USART_Tx(ch);
}

void adcCallback(uint8_t ch, uint16_t value){

}

int main(void)
{
    ADC_SetCallback(adcCallback);

    USART_SetCallback(rxCallback);
    stdout = USART_GetOutputStream();
    USART_Enable(USART_BAUD);

    ADC_Set(ADC_AVCC);

    DDRB |= (1<<PB0);

    PWM_OSC1_Set(100, 100 , 256);
    PWM_OSC1_Enable();
    DDRB |= (1<<PB1);
    DDRB |= (1<<PB2);

    sei();
    
    printf("avrkit, build %s at %s\n\r",__DATE__,__TIME__);

    while(1){
        _delay_ms(500);
        PORTB |= (1 << PB0);
        _delay_ms(500);
        PORTB &= ~(1 << PB0);
    }

    return 0;
}
