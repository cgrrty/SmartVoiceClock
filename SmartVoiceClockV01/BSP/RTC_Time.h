#ifndef _RTC_TIME_H_
#define _RTC_TIME_H_
#include "stm32f0xx.h"



#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
//#define RTC_CLOCK_SOURCE_LSI  // LSI used as RTC source clock. The RTC Clock
                                // may varies due to LSI frequency dispersion

#define BKP_VALUE    0x32F0 

/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_NVIC_Config(void);
void RTC_Configuration(void);
void RTC_CheckAndConfig(void);
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_AlarmShow(void);
static uint8_t USART_Scanf(uint32_t value);
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt);

#endif
