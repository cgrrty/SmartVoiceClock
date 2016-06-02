#ifndef _SEG_H
#define _SEG_H
#include "stm32f0xx.h"
#include "main.h"

enum SEG
{
    SEGA = BIT(0),//¶ÎÂë
    SEGB = BIT(1),
    SEGC = BIT(2),
    SEGD = BIT(3),
    SEGE = BIT(4),
    SEGF = BIT(5),
    SEGG = BIT(6),
    SEGDP= BIT(7),
    
    SEGDIG1=BIT(8),//Î»Ñ¡
    SEGDIG2=BIT(9),
    SEGDIG3=BIT(10),
    SEGDIG4=BIT(11),    
};  

#define SEG_GPIO_SEGA	  GPIO_Pin_0 
#define SEG_GPIO_SEGB	  GPIO_Pin_1
#define SEG_GPIO_SEGC	  GPIO_Pin_2
#define SEG_GPIO_SEGD	  GPIO_Pin_3
#define SEG_GPIO_SEGE	  GPIO_Pin_4
#define SEG_GPIO_SEGF	  GPIO_Pin_5
#define SEG_GPIO_SEGG	  GPIO_Pin_6
#define SEG_GPIO_SEGDP	GPIO_Pin_7

#define SEG_GPIO_SEGDIG1	  GPIO_Pin_8
#define SEG_GPIO_SEGDIG2	  GPIO_Pin_9
#define SEG_GPIO_SEGDIG3	  GPIO_Pin_10
#define SEG_GPIO_SEGDIG4	  GPIO_Pin_11

#define	SEG_GPIO_RCC      RCC_AHBPeriph_GPIOB   

#define SEG_GPIO_PORT     GPIOB

void SEG_GPIOInit(void);
//void SEG_DisPlay(uint16_t dig,uint8_t num);
void SEG_DisPlay(uint16_t dig,uint8_t num,uint16_t ms);
void SEGCongfigForOUT(void);
void SEGCongfigForAD(void);
uint32_t SEGDetectLight(void);
uint16_t SEG_Breathe(void);

#endif
