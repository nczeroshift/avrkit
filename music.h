/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#ifndef _AVRKIT_MUSIC_H_

#include <stdint.h>


void MUSIC_Clear(void);
void MUSIC_InitTimer(uint64_t fcpu);
uint8_t MUSIC_Play(char notes[], uint8_t beats[], uint8_t length, int32_t tempo, uint64_t time);

#endif

