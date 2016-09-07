 /**
  ******************************************************************************
  * @file    RTC_Time.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include "stm32f0xx.h"
#include <stdio.h>
#include "RTC_Time.h"
//#include "led.h"


/** @addtogroup RTC_TimeStamp
  * @{
  */ 

#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */

__IO uint32_t TimeDisplay = 0;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

 /**
  * @brief  配置RTC秒中断的主中断优先级为1，次优先级为0
  * @param  None
  * @retval None
  */
void RTC_NVIC_Config(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	/* Enable the USART1 Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;	 
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  检查并配置RTC
  * @param  None
  * @retval None
  */
void RTC_CheckAndConfig(void)
{
    RTC_InitTypeDef RTC_InitStructure;
//    EXTI_InitTypeDef  EXTI_InitStructure;
    /*在启动时检查备份寄存器RTC_BKP_DR0，如果内容不是0x32F0,则需重新配置时间并询问用户调整时间*/
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
    {
        /* RTC configuration  */
        RTC_Configuration();
        /* Configure the RTC data register and RTC prescaler */
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;//RTC异步分频系数
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;//RTC同步分频系数
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24小时格式

        /* Check on RTC init */
        if (RTC_Init(&RTC_InitStructure) == ERROR)//判断RTC初始化是否成功
        {
            printf("\n\r   /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
        }
        else
        {
            printf("\n\r   /!\\***** RTC Prescaler Config ok ********/!\\ \n\r");
        }
        /* Configure the time register */
        RTC_TimeRegulate(); 
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printf("\r\n Power On Reset occurred....\n\r");
        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printf("\r\n External Reset occurred....\n\r");
        }

        printf("\n\r No need to configure RTC....\n\r");

        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_BackupAccessCmd(ENABLE);

        #ifdef RTC_CLOCK_SOURCE_LSI
        /* Enable the LSI OSC */ 
        RCC_LSICmd(ENABLE);
        #endif /* RTC_CLOCK_SOURCE_LSI */

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        RTC_ClearFlag(RTC_FLAG_TSF);
        /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
        EXTI_ClearITPendingBit(EXTI_Line19);

        /* Display the RTC Time and Alarm */

    }
    RTC_NVIC_Config();
}


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
	/* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE); 
 #if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
  /* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RTC_ITConfig(RTC_IT_TS, ENABLE);
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
	

}

///**
//  * @brief  从微机超级终端获取数字值(把ASCII码转换为数字)，本函数专用于RTC获取时间，
//  * @param   - value 用户在超级终端中输入的数值
//  * @retval 输入字符的ASCII码对应的数值
//  */
//static uint8_t USART_Scanf(uint32_t value)
//{
//	  uint32_t index = 0;
//	  uint32_t tmp[2] = {0, 0};
//	
//	  while (index < 2)
//	  {
//	    /* Loop until RXNE = 1 */
//	    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
//	    {
//		}
//	    tmp[index++] = (USART_ReceiveData(USART2));
//	    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))   /*数字0到9的ASCII码为0x30至0x39*/
//	    { 		  
//		      printf("\n\rPlease enter valid number between 0 and 9 -->:  ");
//              index--; 		 
//	    }
//	  }
//	  
//	  /* 计算输入字符的ASCII码转换为数字*/
//	  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
//		
//	  /* Checks */
//	  if (index > value)
//	  {
//	    printf("\n\rPlease enter valid number between 0 and %d", value);
//	    return 0xFF;
//	  }
//	  return index;
//}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(void)
{ 

    RTC_TimeTypeDef RTC_TimeStructure;
//    RTC_AlarmTypeDef  RTC_AlarmStructure;
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    printf("\n\r==============Time Settings=====================================\n\r");
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    printf("  Please Set Hours:\n\r");
#if 1
    while (tmp_hh == 0xFF)
    {
//        tmp_hh = USART_Scanf(23);
        tmp_hh=8;
        RTC_TimeStructure.RTC_Hours = tmp_hh;
    }
    printf("  %0.2d\n\r", tmp_hh);

    printf("  Please Set Minutes:\n\r");
    while (tmp_mm == 0xFF)
    {
//        tmp_mm = USART_Scanf(59);
        tmp_mm=38;
        RTC_TimeStructure.RTC_Minutes = tmp_mm;
    }
    printf("  %0.2d\n\r", tmp_mm);

    printf("  Please Set Seconds:\n\r");
    while (tmp_ss == 0xFF)
    {
//        tmp_ss = USART_Scanf(59);
        tmp_ss=55;
        RTC_TimeStructure.RTC_Seconds = tmp_ss;
    }
    printf("  %0.2d\n\r", tmp_ss);

    /* Configure the RTC time register */
    if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
    {
        printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
    } 
    else
    {
        printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
        RTC_TimeShow();
        /* Indicator for the RTC configuration */
        RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
    }
	
#endif

//    tmp_hh = 0xFF;
//    tmp_mm = 0xFF;
//    tmp_ss = 0xFF;

//    /* Disable the Alarm A */
//    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

//    printf("\n\r==============Alarm A Settings=====================================\n\r");
//    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
//    printf("  Please Set Alarm Hours:\n\r");
//    while (tmp_hh == 0xFF)
//    {
//        tmp_hh = USART_Scanf(23);
//        RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
//    }
//    printf("  %0.2d\n\r", tmp_hh);

//    printf("  Please Set Alarm Minutes:\n\r");
//    while (tmp_mm == 0xFF)
//    {
//        tmp_mm = USART_Scanf(59);
//        RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
//    }
//    printf("  %0.2d\n\r", tmp_mm);

//    printf("  Please Set Alarm Seconds:\n\r");
//    while (tmp_ss == 0xFF)
//    {
//        tmp_ss = USART_Scanf(59);
//        RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
//    }
//    printf("  %0.2d", tmp_ss);

//    /* Set the Alarm A */
//    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
//    RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;    
//    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

//    /* Configure the RTC Alarm A register */
//    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
//    printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
//    RTC_AlarmShow();

//    /* Enable the RTC Alarm A Interrupt */
//    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

//    /* Enable the alarm  A */
//    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
    RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  printf("\n\r  The current time is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  /* Get the current Alarm */
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  printf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}



/**************11****************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/
void RTC_IRQHandler(void)
{
	
  if( (RTC_IT_ALRA) != RESET)
  {
		RTC_TimeShow();
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line19);
	}
	
}





/*******************************END OF FILE***************************************/
