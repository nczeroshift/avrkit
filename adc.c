
#include "settings.h"
#include "adc.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*ADC_Conversion_Callback)(uint8_t ch, uint16_t value) = NULL;
 
void ADC_SetCallback(ADC_Callback callback){
    ADC_Conversion_Callback = callback;
}

void ADC_Set(enum ADC_Reference ref){
    if(ref == ADC_AVCC)         ADMUX = (1<<REFS0);
    else if(ref == ADC_AINT)    ADMUX = 0;
    else if(ref == ADC_AREF)    ADMUX = (1<<REFS1) | (1<<REFS0); 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADIE);  // Enable ADC, fClock/256, Interrupt
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

