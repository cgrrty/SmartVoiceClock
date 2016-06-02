
/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	


//头文件
#include "usart.h"

extern uint8_t gvoicesendcmdtimer;

extern USART_RXTypeDef USARTRX_structure;
extern USART_RXTypeDef USARTRX2_structure;  
 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO 配置,工作模式配置。9600-8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    RCC_AHBPeriphClockCmd(USART_GPIO_RCC, ENABLE);  //使能GPIOA的时钟
    RCC_APB2PeriphClockCmd(USART_RCC, ENABLE);//使能USART的时钟



    //使能串口中断，并设置优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(USART_GPIO_PORT, USART_TX_GPIO_PinSource, GPIO_AF_1);//配置PA9成第二功能引脚	TX
    GPIO_PinAFConfig(USART_GPIO_PORT, USART_RX_GPIO_PinSource, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

    /*USART1_TX ->PA9  USART1_RX ->PA10*/		
    GPIO_InitStructure.GPIO_Pin = USART_TX|USART_RX;	       //选中串口默认输出管脚         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //定义输出最大速率 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
    
    /*串口通讯参数设置*/
    USART_InitStructure.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_Init(USART, &USART_InitStructure);
    USART_ClearFlag(USART,USART_FLAG_TC);
//    USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART, ENABLE);
    USART_ITConfig(USART, USART_IT_RXNE, ENABLE);    
}
void USART2_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    RCC_AHBPeriphClockCmd(USART2_GPIO_RCC, ENABLE);  
    RCC_APB1PeriphClockCmd(USART2_RCC, ENABLE);//使能USART的时钟

    //使能串口中断，并设置优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(USART2_GPIO_PORT, USART2_TX_GPIO_PinSource, GPIO_AF_1);//配置PA2成第二功能引脚	TX
    GPIO_PinAFConfig(USART2_GPIO_PORT, USART2_RX_GPIO_PinSource, GPIO_AF_1);//配置PA3成第二功能引脚  RX	

    /*USART1_TX ->PA2 USART1_RX ->PA3*/		
    GPIO_InitStructure.GPIO_Pin = USART2_TX|USART2_RX;	       //选中串口默认输出管脚         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //定义输出最大速率 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
    
    /*串口通讯参数设置*/
//    USART_InitStructure.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure.USART_BaudRate = 115200; //波特率    
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_Init(USART2, &USART_InitStructure);
    USART_ClearFlag(USART2,USART_FLAG_TC);
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		
}
void USART_SendToDevice(uint8_t CMD,uint16_t dat)
{
    uint8_t Send_buf[8]={0};    
    Send_buf[0]=0x7e;
    Send_buf[1]=0xff;
    Send_buf[2]=0x06;
    Send_buf[3]=CMD;
    Send_buf[4]=0;
    Send_buf[5]=(uint8_t)(dat>>8);
    Send_buf[6]=(uint8_t)(dat);  
    Send_buf[7]=0xef;
    if(gvoicesendcmdtimer == 0)
    {
        USART_SendCmd(Send_buf,8);
        gvoicesendcmdtimer =20;
    }        
}
void USART_TTSCmd(uint16_t *buf,uint8_t num)
{
    uint8_t sendbuf[64]={0};
    uint8_t len;
    sendbuf[0]=0x7e;
    sendbuf[1]=0xff;
    sendbuf[2]=2*num+3;
    sendbuf[3]=0x21;
    for(len=0;len < num;len++)
    {
        sendbuf[2*len+4]=(uint8_t)(buf[len]>>8);
        sendbuf[2*len+5]=(uint8_t)buf[len];      
    }
    sendbuf[2*len+4]=0xef; 
    USART_SendCmd(sendbuf,2*num+5);    
}
void USART_SendCmd(uint8_t *buf,uint8_t len)
{
    uint8_t cnt;
    for(cnt=0;cnt<len;cnt++)
    {
        USART_SendData(USART1,buf[cnt]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);           
    }
}
void USART1_IRQHandler()
{
    uint8_t value=0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		value=USART_ReceiveData(USART1);
        
//		USART_SendData(USART2,USART_ReceiveData(USART1));
//		//Loop until the end of transmission
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){};
            
        if(USARTRX_structure.rxcompflag == 0)
        {
            if(value==0x7e &&USARTRX_structure.rxstartflag!=1)
            {
                USARTRX_structure.rxcompflag=0;
                USARTRX_structure.len=0;
                USARTRX_structure.rxbuf[USARTRX_structure.len]=value;
                USARTRX_structure.len++;
            }
            else if(USARTRX_structure.rxcompflag!=1)
            {
                USARTRX_structure.rxbuf[USARTRX_structure.len]=value;
                USARTRX_structure.len++;
                if(value == 0xef)
                {
                   USARTRX_structure.rxcompflag=1; 
                   USARTRX_structure.rxstartflag=0;                   
                }
            }            
        }

    }
}
//数据头 @ 数据尾 #
void USART2_IRQHandler()
{
    static uint8_t value=0;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
    {
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		value=USART_ReceiveData(USART2);
        
//		USART_SendData(USART2,value);
//		//Loop until the end of transmission
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){};
            
        if(USARTRX2_structure.rxcompflag == 0)
        {
            if(value=='@' &&USARTRX2_structure.rxstartflag!=1)
            {
                USARTRX2_structure.rxcompflag=0;
                USARTRX2_structure.len=0;
                USARTRX2_structure.rxbuf[USARTRX_structure.len]=value;
                USARTRX2_structure.len++;
            }
            
            else if(USARTRX2_structure.rxcompflag!=1)
            {
                USARTRX2_structure.rxbuf[USARTRX2_structure.len]=value;
                USARTRX2_structure.len++;
                if(value == '#')
                {
                   USARTRX2_structure.rxcompflag=1; 
                   USARTRX2_structure.rxstartflag=0;                   
                }
            }            
        }
    }
}    

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}


