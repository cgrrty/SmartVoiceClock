
/******************** (C) COPYRIGHT  2016 ********************************

* �ļ���  ��
 * ����    ��
 * ��汾  ��V1.3.0
 * ����    ��DongXu
 
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
    TTSData_structure.buf[TTSData_structure.cnt]=0x0115;//����������ʱ�̡� 
    TTSData_structure.cnt++;                
    //����Сʱ����
    TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[0];//������Сʱʮλ�� 
    TTSData_structure.cnt++; 
    if(ttimebuf[0] == 1)//СʱΪ 1 ֱ�ӱ� "ʮ"
    {
        TTSData_structure.cnt--;                         
    }

    TTSData_structure.buf[TTSData_structure.cnt]=0x010a;//���� ��ʮ�� 
    TTSData_structure.cnt++; 
    if(ttimebuf[0] == 0) //СʱΪ 0 ��������ʮ��
    {
        TTSData_structure.cnt--;                      
    }

    TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[1];//������Сʱ��λ��
    TTSData_structure.cnt++; 
    if(ttimebuf[1] == 0) //СʱΪ ��ʮ ���������㡱
    {
        TTSData_structure.cnt--;                      
    }    
    TTSData_structure.buf[TTSData_structure.cnt]=0x010b;//�������㡰
    TTSData_structure.cnt++;                  
    //����Сʱ���� ����

    //���ŷ��Ӳ���
    if(ttimebuf[2] == 0 && ttimebuf[3] == 0)
    {
        TTSData_structure.buf[TTSData_structure.cnt]=0x0117;//����"��"
        TTSData_structure.cnt++;     
    }
    else
    {
        TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[2];//����"����ʮλ"
        TTSData_structure.cnt++; 
        if(ttimebuf[2] == 1)//СʱΪ 1 ֱ�ӱ� "ʮ"
        {
            TTSData_structure.cnt--;                         
        }
        TTSData_structure.buf[TTSData_structure.cnt]=0x010a;//���� ��ʮ�� 
        TTSData_structure.cnt++; 
        if(ttimebuf[2] == 0) //СʱΪ 0 ��������ʮ��
        {
            TTSData_structure.cnt--;                      
        }
        TTSData_structure.buf[TTSData_structure.cnt]=0x0100|ttimebuf[3];//���������Ӹ�λ��
        TTSData_structure.cnt++; 
        if(ttimebuf[3] == 0)
        {
            TTSData_structure.cnt--;                         
        }                    
        TTSData_structure.buf[TTSData_structure.cnt]=0x010e;//�������֡�
        TTSData_structure.cnt++;         
    }
                  
    //���ŷ��Ӳ��ֽ���

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
            GPIO_InitStructure.GPIO_Pin = USART_TX|USART_RX;	       //ѡ�д���Ĭ������ܽ�         
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�������������� 
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
            GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��				

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
            GPIO_InitStructure.GPIO_Pin = USART_TX|USART_RX;	       //ѡ�д���Ĭ������ܽ�         
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��			

//            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//            GPIO_InitStructure.GPIO_Pin	= VOICEPOWERCONTR_GPIO_PIN;
//            GPIO_Init(VOICEPOWERCONTR_PORT,&GPIO_InitStructure);

            GPIO_WriteBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN,Bit_SET);		
	}
}







