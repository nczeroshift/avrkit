
#ifndef _AVRKIT_ADC_H_
#define _AVRKIT_ADC_H_

#include <stdint.h>

typedef void (*ADC_Callback)(uint8_t ch, uint16_t value);

enum ADC_Reference{
	ADC_AVCC = 0,
	ADC_AINT = 1,
	ADC_AREF = 2,
};

void ADC_Set(enum ADC_Reference ref);
void ADC_Start(unsigned char ch);
void ADC_SetCallback(ADC_Callback callback);

#endif

