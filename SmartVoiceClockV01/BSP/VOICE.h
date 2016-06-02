#ifndef _VOICE_H
#define _VOICE_H

#include "stm32f0xx.h"

#define VOICEPOWERCONTR_GPIO_PIN			GPIO_Pin_12
#define VOICEPOWERCONTR_PORT			    GPIOA
#define VOICEPOWERCONTR_RCCCLK 				RCC_AHBPeriph_GPIOA	

typedef struct
{
  uint16_t buf[10];
  uint8_t  cnt;
}TTSData_Typedf;

void TTSReport_time(RTC_TimeTypeDef *RTC_TimeStruct);
void VoicePowerSwitch(FunctionalState NewState);
void VoicePowerConfig(void);

#endif











