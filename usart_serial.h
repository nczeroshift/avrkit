/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#ifndef _AVRKIT_USART_SERIAL_H_
#define _AVRKIT_USART_SERIAL_H_

#include <stdint.h>
#include <stdio.h>

typedef void (*USART_Callback)(uint8_t ch);

void    USART_Enable            (uint64_t baudrate, uint64_t f_cpu);
void    USART_Tx                (uint8_t data);
uint8_t USART_Rx                (void);
void    USART_SetCallback       (USART_Callback callback);
FILE*   USART_GetOutputStream   (void);

#endif 
