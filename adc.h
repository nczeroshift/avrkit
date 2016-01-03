/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#ifndef _AVRKIT_ADC_H_
#define _AVRKIT_ADC_H_

#include <stdint.h>

typedef void (*ADC_Callback)(uint8_t ch, uint16_t value);

enum ADC_Reference{
	ADC_AVCC,
	ADC_AINT,
	ADC_AREF,
};

enum ADC_Prescaler{
    ADC_PRESCALE_DEFAULT,
    ADC_PRESCALE_2,
    ADC_PRESCALE_4,
    ADC_PRESCALE_8,
    ADC_PRESCALE_16,
    ADC_PRESCALE_32,
    ADC_PRESCALE_64,
    ADC_PRESCALE_128,
};

void ADC_Set(enum ADC_Reference ref, enum ADC_Prescaler prescaler);
void ADC_Start(unsigned char ch);
void ADC_SetCallback(ADC_Callback callback);

#endif

