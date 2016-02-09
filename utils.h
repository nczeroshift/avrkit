
#ifndef _AVRKIT_UTILS_H
#define _AVRKIT_UTILS_H

#define OUT_OFF(Port,Id)        Port &= ~(1<<Id)  
#define OUT_ON(Port,Id)         Port |= (1<<Id)
#define IS_ON(Pin,Id)           ((Pin & (1<<Id)) == (1<<Id))
#define IS_OFF(Pin,Id)           ((Pin & (1<<Id)) == 0)
#define TOGGLE_OUT(Port,Id) if(IS_ON(Port,Id)){OUT_OFF(Port,Id);}else{OUT_ON(Port,Id);}

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#endif
