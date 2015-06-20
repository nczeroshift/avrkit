
#include "cli_parser.h"
#include "usart_serial.h"
#include "i2c_master.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

void rxCallback(uint8_t ch){
    USART_Tx(ch);
}

int main(void)
{
    USART_SetCallback(rxCallback);
    stdout = USART_GetOutputStream();
    USART_Init(USART_BAUD);

    DDRB |= (1<<PB0);
    sei();
    
    printf("avrkit, build %s at %s\n\r",__DATE__,__TIME__);

    while(1){
        _delay_ms(1000);
        PORTB |= (1 << PB0);
        _delay_ms(1000);
        PORTB &= ~(1 << PB0);
    }

    return 0;
}
