
#include "settings.h"
#include "state_machine.h"
#include <stdio.h>
#include <stdlib.h>

volatile static uint8_t state_last = 0;
volatile static uint8_t state_current = 0;
volatile static uint8_t state_next = 0;
volatile static uint32_t state_wait = 0; 

volatile static uint64_t clock_counter = 0;

static SM_Function state_functions[STATE_MACHINE_MAX_STATES];
volatile static uint8_t state_count = 0;

void SM_Update(void){
	if(state_wait == 0){
		state_last = state_current;
		state_current = state_next;
	}else
		state_wait --;
	clock_counter ++;
}

void SM_Execute (void){
	if(state_current != state_last){
		if(state_functions[state_last]!=NULL)
			(*state_functions[state_last])(STATE_EXIT);

		if(state_functions[state_current]!=NULL)
			(*state_functions[state_current])(STATE_ENTER);
		
		state_last = state_current;
	}
	else if(state_functions[state_current]!=NULL)
			(*state_functions[state_current])(STATE_NORMAL);
}

uint32_t SM_CurrentState(void){
	return state_current;
}

void SM_Init(uint8_t states){
	uint8_t i;
	state_count = states;
	for(i = 0;i<states;i++)
		state_functions[i] = NULL; 
}

void SM_SetFunction(uint8_t state, SM_Function func){
	if(state >= STATE_MACHINE_MAX_STATES)
		return;
	state_functions[state] = func;
}

void SM_SetNext (uint8_t next){
	state_next = next;
    state_wait = 0;
}

void SM_SetNextWithDelay (uint8_t next, uint32_t delay){
	state_next = next;
    state_wait = delay;
}

uint64_t getClock (void){
	return clock_counter;
}


