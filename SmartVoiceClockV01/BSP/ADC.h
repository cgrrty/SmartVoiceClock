#ifndef _ADC_H
#define _ADC_H

#include "stm32f0xx.h"


void ADC_BSPInit(void);
void ADC_DMAConfig(void);

int32_t ADC_GetTemper(void);
float ADC_GetChannal1Value(void);
float ADC_GetChannal8Value(void);
#endif
