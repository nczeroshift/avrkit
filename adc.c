/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#include "adc.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*ADC_Conversion_Callback)(uint8_t ch, uint16_t value) = NULL;
 
void ADC_SetCallback(ADC_Callback callback){
    ADC_Conversion_Callback = callback;
}

void ADC_Set(enum ADC_Reference ref, enum ADC_Prescaler prescaler){
    // Set voltage reference
    switch(ref){
        case ADC_AVCC:
            ADMUX = (1<<REFS0);
            break;
        case ADC_AINT:
            ADMUX = 0;
            break;
        case ADC_AREF:
            ADMUX = (1<<REFS1) | (1<<REFS0);
            break;
    }

    // Set clock devider
    switch(prescaler){
        case ADC_PRESCALE_DEFAULT:
            ADCSRA = 0;
            break;
        case ADC_PRESCALE_2:
            ADCSRA = (1 << ADPS0);
            break;
        case ADC_PRESCALE_4:
            ADCSRA = (1 << ADPS1);
            break;
        case ADC_PRESCALE_8:
            ADCSRA = (1 << ADPS1) | (1 << ADPS0);
            break;
        case ADC_PRESCALE_16:
            ADCSRA = (1 << ADPS2);
            break;
        case ADC_PRESCALE_32:
            ADCSRA = (1 << ADPS2) | (1 << ADPS0);
            break;
        case ADC_PRESCALE_64:
            ADCSRA = (1 << ADPS2) | (1 << ADPS1);
            break;
        case ADC_PRESCALE_128:
            ADCSRA = (1 << ADPS2) |  (1 << ADPS1) | (1 << ADPS0);
            break;
    }

    ADCSRA |= (1 << ADEN) | (1 << ADIE);  // Enable ADC & Interrupt
}

void ADC_Start(unsigned char ch){
    ADMUX = (ADMUX & 0xF8) | (ch & 0x07);   // Config Mutex
    ADCSRA |= (1 << ADSC);                  // Start Conversion
}

ISR(ADC_vect){
    uint16_t sample = ADC;                            
    uint8_t ch = ADMUX & 0x07;
   
    ADCSRA |= (1<<ADIF);    // Clear Conversion

    if(ADC_Conversion_Callback != NULL){
        ADC_Conversion_Callback(ch,sample);
    }
}

