
/******************** (C) COPYRIGHT  2016 ********************************

* �ļ���  ��
 * ����    ��
 * ��汾  ��V1.3.0
 * ����    ��DongXu
 
********************************************************************************/	


//ͷ�ļ�
#include "systick.h"
#include <stdio.h>
#include "usart.h"


extern uint16_t nTime;
 /**
  * @file   SYSTICK_Init
  * @brief  ��ʼ��SYSTICK��1Ms�ж�1��
  * @param  ��
  * @retval ��
  */
void SYSTICK_Init(void)
{	
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	/*SystemCoreClock/1000000��1us�ж�1�Σ�SystemCoreClock/ 1000��1ms�ж�һ��*/
    while (SysTick_Config(SystemCoreClock/1000))
    {};    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while((RCC->CFGR & RCC_CFGR_SWS )!= RCC_CFGR_SWS_HSI);
    RCC_PLLCmd(DISABLE); 
    while((RCC->CR & RCC_CR_PLLRDY)!= 0);
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_4);
    RCC_PLLCmd(ENABLE);
    while((RCC->CR & RCC_CR_PLLRDY) == 0);    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    RCC_PCLKConfig(RCC_HCLK_Div2);
        
}


 /**
  * @file   delay_ms
  * @brief  ������ʱ
  * @param  ��ʱʱ��
  * @retval ��
  */
void delay_ms(uint16_t nms)
{
//  nTime=nms;
//  while(nTime);
    int8_t i;
    for(i=0;i<100;i++)
    {
        while(nms--);
    }
}

void PrintfClk()
{
    RCC_ClocksTypeDef  RCC_ClockStructure;
	RCC_GetClocksFreq(&RCC_ClockStructure);
    
	pr_debug("RCC_ClockStructure.SYSCLK_Frequency   %d\r\n",RCC_ClockStructure.SYSCLK_Frequency);	
	pr_debug("RCC_ClockStructure.HCLK_Frequency     %d\r\n",RCC_ClockStructure.HCLK_Frequency);
	pr_debug("RCC_ClockStructure.PCLK_Frequency     %d\r\n",RCC_ClockStructure.PCLK_Frequency);
	pr_debug("RCC_ClockStructure.CECCLK_Frequency   %d\r\n",RCC_ClockStructure.CECCLK_Frequency);
    pr_debug("RCC_GetSYSCLKSource :%02x\r\n",RCC_GetSYSCLKSource());
}


