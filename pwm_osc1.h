
#ifndef _AVRKIT_PWM_OSC1_H_
#define _AVRKIT_PWM_OSC1_H_

#include <stdint.h>

void PWM_OSC1_Set(uint16_t compare_a, uint16_t compare_b , uint16_t top);
void PWM_OSC1_Enable(void);
void PWM_OSC1_Disable(void);

#endif

