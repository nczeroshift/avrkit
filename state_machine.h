
#ifndef _AVRKIT_STATE_MACHINE_H_
#define _AVRKIT_STATE_MACHINE_H_

#include <stdint.h>

enum SM_Flag{
	STATE_NORMAL = 0,
	STATE_ENTER = 1,
	STATE_EXIT = 2
};

typedef void (*SM_Function)(enum SM_Flag flag); 

void SM_Update (void);
void SM_Execute (void);

uint32_t SM_CurrentState(void);

void SM_Init(uint8_t states);
void SM_SetFunction(uint8_t state, SM_Function func);

void SM_SetNext (uint8_t next);
void SM_SetNextWithDelay (uint8_t next, uint32_t delay);

uint64_t getClock (void);

uint8_t SM_ComputeTimerStart(int64_t fcpu);

#endif
