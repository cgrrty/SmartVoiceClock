
/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	

#include "VOICE.h"
#include "usart.h"

void TTSReport_time(RTC_TimeTypeDef *RTC_TimeStruct)
{
    TTSData_Typedf TTSData_structure;
    uint8_t ttimebuf[4];
    ttimebuf[0]=RTC_TimeStruct->RTC_Hours/10;
    ttimebuf[1]=RTC_TimeStruct->RTC_Hours%10;            
    ttimebuf[2]=RTC_TimeStruct->RTC_Minutes/10;
    ttimebuf[3]=RTC_TimeStruct->RTC_Minutes%10;
    
    TTSData_structure.cnt=0;
    TTSData_structure.buf[TTSData_structure.cnt]=0x0115;//播报”现在时刻“ 
    TTSData_structure.cnt++;                
    //播放小时部分
    TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[0];//播报”小时十位“ 
    TTSData_structure.cnt++; 
    if(ttimebuf[0] == 1)//小时为 1 直接报 "十"
    {
        TTSData_structure.cnt--;                         
    }

    TTSData_structure.buf[TTSData_structure.cnt]=0x010a;//播报 “十“ 
    TTSData_structure.cnt++; 
    if(ttimebuf[0] == 0) //小时为 0 跳过报“十”
    {
        TTSData_structure.cnt--;                      
    }

    TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[1];//播报“小时个位“
    TTSData_structure.cnt++; 
    if(ttimebuf[1] == 0) //小时为 几十 跳过报“零”
    {
        TTSData_structure.cnt--;                      
    }    
    TTSData_structure.buf[TTSData_structure.cnt]=0x010b;//播报”点“
    TTSData_structure.cnt++;                  
    //播放小时部分 结束

    //播放分钟部分
    if(ttimebuf[2] == 0 && ttimebuf[3] == 0)
    {
        TTSData_structure.buf[TTSData_structure.cnt]=0x0117;//播报"整"
        TTSData_structure.cnt++;     
    }
    else
    {
        TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[2];//播报"分钟十位"
        TTSData_structure.cnt++; 
        if(ttimebuf[2] == 1)//小时为 1 直接报 "十"
        {
            TTSData_structure.cnt--;                         
        }
        TTSData_structure.buf[TTSData_structure.cnt]=0x010a;//播报 “十“ 
        TTSData_structure.cnt++; 
        if(ttimebuf[2] == 0) //小时为 0 跳过报“十”
        {
            TTSData_structure.cnt--;                      
        }
        TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[3];//播报“分钟个位”
        TTSData_structure.cnt++; 
        if(ttimebuf[3] == 0)
        {
            TTSData_structure.cnt--;                         
        }                    
        TTSData_structure.buf[TTSData_structure.cnt]=0x010e;//播报”分“
        TTSData_structure.cnt++;         
    }
                  
    //播放分钟部分结束

    USART_TTSCmd(TTSData_structure.buf,TTSData_structure.cnt);
}
void VoicePowerConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(VOICEPOWERCONTR_RCCCLK,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin	= VOICEPOWERCONTR_GPIO_PIN;
	GPIO_Init(VOICEPOWERCONTR_PORT,&GPIO_InitStructure);
}
void VoicePowerSwitch(FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	if(NewState == ENABLE)
	{
            /*USART1_TX ->PA9  USART1_RX ->PA10*/		
            GPIO_InitStructure.GPIO_Pin = USART_TX|USART_RX;	       //选中串口默认输出管脚         
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //定义输出最大速率 
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
            GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化				

//            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//            GPIO_InitStructure.GPIO_Pin	= VOICEPOWERCONTR_GPIO_PIN;
//            GPIO_Init(VOICEPOWERCONTR_PORT,&GPIO_InitStructure);
            
            GPIO_WriteBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN,Bit_RESET);           
	}
	else if(NewState == DISABLE)
	{
            /*USART1_TX ->PA9  USART1_RX ->PA10*/		
            GPIO_InitStructure.GPIO_Pin = USART_TX|USART_RX;	       //选中串口默认输出管脚         
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化			

//            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//            GPIO_InitStructure.GPIO_Pin	= VOICEPOWERCONTR_GPIO_PIN;
//            GPIO_Init(VOICEPOWERCONTR_PORT,&GPIO_InitStructure);

            GPIO_WriteBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN,Bit_SET);		
	}
}







