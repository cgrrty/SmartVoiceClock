
/******************** (C) COPYRIGHT  2016 ********************************

* �ļ���  ��
 * ����    ��
 * ��汾  ��V1.3.0
 * ����    ��DongXu
 
********************************************************************************/	

#include"SEG.h"


extern uint16_t gdisbuf[4];
extern uint16_t     gledlight;

const uint8_t segnum[10]={
 (SEGA|SEGB|SEGC|SEGD|SEGE|SEGF),//0
 (SEGB|SEGC),//1
 (SEGA|SEGB|SEGD|SEGE|SEGG),//2
 (SEGA|SEGB|SEGC|SEGD|SEGG),//3 
 (SEGB|SEGC|SEGF|SEGG),//4
 (SEGA|SEGC|SEGD|SEGF|SEGG),//5 
 (SEGA|SEGC|SEGD|SEGE|SEGF|SEGG),//6
 (SEGA|SEGB|SEGC|SEGF),//7 
 (SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG),//8
 (SEGA|SEGB|SEGC|SEGD|SEGF|SEGG)//9  
 };

const unsigned int seltab[]={BIT(8),BIT(9),BIT(10),BIT(11)};//

void delayus(uint16_t t)
{
  while(t--);
}

void SEG_GPIOInit()
{
	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_AHBPeriphClockCmd(SEG_GPIO_RCC,ENABLE);//ʹ��GPIO������ʱ��
	
//	GPIO_InitStructure.GPIO_Pin =SEG_GPIO_SEGA|SEG_GPIO_SEGB|
//                               SEG_GPIO_SEGC|SEG_GPIO_SEGD|
//                               SEG_GPIO_SEGE|SEG_GPIO_SEGF|
//                               SEG_GPIO_SEGG|SEG_GPIO_SEGDP|
//                               SEG_GPIO_SEGDIG1|SEG_GPIO_SEGDIG2|
//                               SEG_GPIO_SEGDIG3|SEG_GPIO_SEGDIG4;//
	GPIO_InitStructure.GPIO_Pin =SEG_GPIO_SEGA|SEG_GPIO_SEGB|
                               SEG_GPIO_SEGC|SEG_GPIO_SEGD|
                               SEG_GPIO_SEGE|SEG_GPIO_SEGF|
                               SEG_GPIO_SEGG|SEG_GPIO_SEGDP;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//��������Ϊ����		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//��������Ϊ�������
    GPIO_InitStructure.GPIO_Pin =   SEG_GPIO_SEGDIG1|SEG_GPIO_SEGDIG2|
                                    SEG_GPIO_SEGDIG3|SEG_GPIO_SEGDIG4;//	 
																
	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO                                

}

//���������
void SEG_DisPlay(uint16_t dig,uint8_t num,uint16_t us)
{
    int16_t data=0;
    data=((~dig)&0xff00);
    data|=(gdisbuf[num]&0x00ff); 
//    GPIO_Write(SEG_GPIO_PORT,data);  
    GPIOB->ODR = data;
    delayus(us);
//    GPIO_Write(SEG_GPIO_PORT,0x0000);
    GPIOB->ODR = 0;
}
//void SEGCongfigForOUT()
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ	
//	GPIO_InitStructure.GPIO_Pin	=SEG_GPIO_SEGA;
//	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 	
//}
//void SEGCongfigForAD()
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin =	SEG_GPIO_SEGA;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 	
//	
//	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG1,Bit_RESET);
//	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG2,Bit_RESET);
//	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG3,Bit_RESET);
//	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG4,Bit_RESET);	
//}

uint32_t SEGDetectLight()
{
	uint32_t cnt ;
	GPIO_InitTypeDef  GPIO_InitStructure;	

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ	
//	GPIO_InitStructure.GPIO_Pin	=	SEG_GPIO_SEGA|SEG_GPIO_SEGDIG1;
//	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 	
	
	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGA,Bit_RESET);		//3P
	GPIO_WriteBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG1,Bit_SET);	//2N
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin	=	SEG_GPIO_SEGDIG1;	
	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 	
    cnt = 0;
//    while(cnt<1000000)
//	{
//        if((GPIOB->IDR & SEG_GPIO_SEGDIG1) == (uint32_t)Bit_RESET)
//        {
//            break;
//        }
//        else
//        {
//            cnt++;
//        }       
//	}

//	for(cnt=0;cnt <1000000;cnt++)
//	{
//        if((GPIOB->IDR & SEG_GPIO_SEGDIG1) == (uint32_t)Bit_RESET)
//		{
//			break;
//		}
//	}

	for(cnt=0;cnt <1000000;cnt++)
	{
		if(GPIO_ReadInputDataBit(SEG_GPIO_PORT,SEG_GPIO_SEGDIG1) == Bit_RESET)
		{
			break;
		}
	}
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ	
	GPIO_InitStructure.GPIO_Pin	=	SEG_GPIO_SEGA|SEG_GPIO_SEGDIG1;
	GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIO 		
	return cnt;
}
uint16_t SEG_Breathe()
{
    static uint8_t flag =0;
    uint8_t step = 3;
    if(flag == 0)
    {
        if(gledlight>0+5)
        {
            gledlight-=step;
        }
        else
        {
            flag = 1;
        }
    }
    else
    {
        if(gledlight<1000-5)
        {
            gledlight+=step;        
        }
        else
        {
            flag = 0;
        }
    }
    return gledlight;
}










