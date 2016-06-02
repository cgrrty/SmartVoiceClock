#ifndef __USART_H
#define __USART_H

#include "stm32f0xx.h"
#include <stdio.h>


#define     DEBUG_SWITCH        
#ifdef      DEBUG_SWITCH
#define     pr_debug(fmt,args...) printf(fmt, ##args)
#else
#define     pr_debug(fmt,args...) /*do nothing */
#endif

#define RXLEN 64

typedef struct 
{
    uint8_t rxstartflag;
    uint8_t rxcompflag;
    uint8_t rxbuf[RXLEN];
    uint8_t len;
}USART_RXTypeDef;
//typedef struct
//{
//    uint8_t heard[2];
//    uint8_t len;
//    uint8_t tail;
//}Pro_TypeDef;



#define BaudRate	 9600

#define USART              	 		USART1
#define USART_RCC         	 		RCC_APB2Periph_USART1

#define USART_GPIO_RCC    			RCC_AHBPeriph_GPIOA
#define USART_TX_GPIO_PinSource		        	GPIO_PinSource9
#define USART_RX_GPIO_PinSource		        	GPIO_PinSource10	
#define USART_TX		       		 	GPIO_Pin_9	// out
#define USART_RX		       		 	GPIO_Pin_10	// in 
#define USART_GPIO_PORT    			GPIOA   

#define USART2_RCC                  RCC_APB1Periph_USART2
#define USART2_GPIO_RCC    			RCC_AHBPeriph_GPIOA
#define USART2_TX_GPIO_PinSource    GPIO_PinSource2
#define USART2_RX_GPIO_PinSource	GPIO_PinSource3	
#define USART2_TX		       		GPIO_Pin_2	// out
#define USART2_RX		       		GPIO_Pin_3// in 
#define USART2_GPIO_PORT    		GPIOA   



void USART1_Config(void);
void USART2_Config(void);
void USART_SendCmd(uint8_t *buf,uint8_t len);
void USART_SendToDevice(uint8_t CMD,uint16_t dat);
void USART_TTSCmd(uint16_t *buf,uint8_t num);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif
