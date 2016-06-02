#ifndef  _MAIN_H
#define  _MAIN_H

#include "stm32f0xx.h"



#define IOSET(port,x)  (port)|=(x)
#define IOCLR(port,x)  (port)&=~(x)
#define IOXOR(port,x)  (port)^=(x)
#define GETBIT(port,x)  ((port&BIT(x))>>x)
#define BIT(x) (0x0001<<(x))



void TIM_Config(void);
void EnterStopMode(void);
    
#endif

