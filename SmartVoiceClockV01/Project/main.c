/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	

//头文件
#include "stm32f0xx.h"
#include "usart.h"
#include "led.h"
#include "systick.h"
#include <stdio.h>
#include "RTC_Time.h"
#include "SEG.h"
#include "KEY.h"
#include "flash.h"
#include "VOICE.h"
#include "ADC.h"
#include "string.h"

#include "main.h"

#define HARDWARE_VER    "0.1"
#define SOFTWARE_VER    "0.1"
#define Main_DispVer()    pr_debug("\033[035mMain Ver \r\nHARDWARE_VER:%s \r\nSOFTWARE_VER:%s\033[0m \r\n",HARDWARE_VER,SOFTWARE_VER)

#define	LEDMAXLIGHT		1000
extern  const uint8_t segnum[10];

uint16_t  seghello[1024*2]={SEGB|SEGC|SEGE|SEGF|SEGG,//H
                    SEGA|SEGD|SEGE|SEGF|SEGG,//E
                    SEGD|SEGE|SEGF,//L
                    SEGD|SEGE|SEGF,//L    
                    SEGA|SEGB|SEGC|SEGD|SEGE|SEGF,//O                            
                    };
uint16_t seghellobuf[15]={0};

/**
  * @brief  Main program.
  * @param  None
  * @retval None
*/
uint8_t	    gk=0;//按键值
uint8_t     gkvalue=0;
uint8_t     t_cnt; //1s计时
uint8_t     g_tcnt;//500ms计时
uint8_t     g1scnt;
uint16_t    mainflag=0;
uint8_t		state=STATE_NORMAL;

//显示部分变量类型 非常重要 （改为uint8_t 会显示异常）

uint16_t     gdisbuf[4]={0};//存放对应段码 显存
uint16_t     gdisnum[4]={0};//存放数字
uint16_t     gledlight =1000;  //led亮度
uint16_t     gerlight =0;   //环境亮度

uint8_t      gtermprature;

uint32_t     gsetmin=1;   //定时值
uint16_t     gtimercnt=0;  //
uint8_t      gsegdispflag = 0;

uint8_t galhh=0; //al小时
uint8_t galmin=0;//al分钟

RTC_TimeTypeDef  RTC_TimeStruct;
RTC_TimeTypeDef  RTC_TimeStructPre;     //记录当前
RTC_TimeTypeDef  RTC_TimeStructDes;     //存储目标时间

USART_RXTypeDef USARTRX_structure;
USART_RXTypeDef USARTRX2_structure;

TTSData_Typedf TTSData_structure;       //语音合成命令结构体
uint16_t gvoicepowertimer = 30;

uint32_t gsystick=0;
//extern  const uint8_t segnum[10];
uint8_t  gdispmode;

uint8_t gvoiceplayenable =0;
uint8_t gttsreporttimeflag=0;


void Main_displyHandle(void);
void USARTRX_Handle(void);
void USARTRX2_Handle(void);
void Main_500msHandle(void);
void Main_1sHandle(void);
void Main_AlarmHandle(void);

int main(void)
{
    uint8_t k=0;
    uint32_t i=0;
    uint8_t max[1024+512]={0};
    gsystick=0;
    SYSTICK_Init(); 
    USART1_Config();
    USART2_Config();  

    SEG_GPIOInit();
    KEY_Init();

    VoicePowerConfig();
    VoicePowerSwitch(ENABLE);    
    RTC_CheckAndConfig();      
    TIM_Config();
    Main_DispVer();
    PrintfClk();  //打印时钟信息   
//    if(*(uint32_t*)(FLASH_USER_START_ADDR)!=0xffffffff) //如果 Flash 不为空值 测赋值
//    {
//        tmp=*(uint32_t*)(FLASH_USER_START_ADDR);
//        galhh=(uint8_t)(tmp>>8);
//        galmin=(uint8_t)(tmp>>16);
//        if(galhh==0xff)
//        {
//           galhh =0;
//        }
//        if(galmin==0xff)
//        {
//           galmin =0;
//        }
//    }
//    memset(max,1,sizeof(max));
//    pr_debug("%d\r\n",sizeof(max));
//    for(i=0;i<(sizeof(max));i++)
//    {
//        pr_debug("%d",max[i]);
//        if((i+1)%8==0)
//        pr_debug("\r\n");
//    }
//    i=0;
//    seghello[9+i] = segnum[2];
//    i++;
//    seghello[9+i] = segnum[3];
//    i++;
//    seghello[9+i] = SEGG;
//    i++;
//    seghello[9+i] = segnum[0];
//    i++;    
//    seghello[9+i] = segnum[6];
//    i++;      
//    seghello[9+i] = SEGG;
//    i++;  
//    seghello[9+i] = segnum[0];
//    i++;    
//    seghello[9+i] = segnum[6]; 
//    for(i=0;i<9;i++)
//    {
//        while(1)
//        {       
//            if(mainflag&FLAG_500MS)
//            {
//                IOCLR(mainflag,FLAG_500MS);
//                gdisbuf[0]=gdisbuf[1];                
//                gdisbuf[1]=gdisbuf[2];                
//                gdisbuf[2]=gdisbuf[3];
//                if(i<6)
//                gdisbuf[3]=seghello[i];
//                else
//                gdisbuf[3]=0;
//                break;
//            }          
//        }
//    }    
//while(1)
//{

//    for(i=3;i<14;i++)
//    {
//        while(1)
//        {
//            if(mainflag&FLAG_K)
//            {     
//                IOCLR(mainflag,FLAG_K);
//                k=gk;										//获取键值
//                KeyPro(k);							//按键处理
//        //        Main_displyHandle();		//显示控制  
//            }        

//            if(mainflag&FLAG_500MS)
//            {
//                IOCLR(mainflag,FLAG_500MS);

//                RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct); 
//                seghello[9] = segnum[RTC_TimeStruct.RTC_Hours/10]; 
//                seghello[10] = segnum[RTC_TimeStruct.RTC_Hours%10];
//                
//                seghello[12] = segnum[RTC_TimeStruct.RTC_Minutes/10];                
//                seghello[13] = segnum[RTC_TimeStruct.RTC_Minutes%10]; 
//                
//                seghello[15] = segnum[RTC_TimeStruct.RTC_Seconds/10];                
//                seghello[16] = segnum[RTC_TimeStruct.RTC_Seconds%10]; 
//                memset(seghellobuf,0,14);
//                memcpy(seghellobuf+4,seghello+9,15);
//                            
//                gdisbuf[0]=seghellobuf[i-3];                
//                gdisbuf[1]=seghellobuf[i-2];                
//                gdisbuf[2]=seghellobuf[i-1]; 
//                gdisbuf[3]=seghellobuf[i];                            
//                break;
//            }          
//        }
//    }
//}

    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);    
    gdisnum[0]=RTC_TimeStruct.RTC_Hours/10;
    gdisnum[1]=RTC_TimeStruct.RTC_Hours%10;            
    gdisnum[2]=RTC_TimeStruct.RTC_Minutes/10;
    gdisnum[3]=RTC_TimeStruct.RTC_Minutes%10; 
    
    ADC_BSPInit();
    
    while (1)
    {
        PWR_EnterSleepMode(PWR_SLEEPEntry_WFI); //进入休眠 降低功耗
        if(mainflag&FLAG_K)
        {     
            IOCLR(mainflag,FLAG_K);
            k=gk;										//获取键值
            KeyPro(k);							//按键处理
            Main_displyHandle();		//显示控制  
        }
        Main_500msHandle();
        Main_1sHandle(); 
        Main_AlarmHandle();
        USARTRX_Handle();
        USARTRX2_Handle();        
    }
}

void TIM_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
    
    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Time Base configuration */    
    TIM_TimeBaseStructure.TIM_Period = 125;       //自动重载值
//    TIM_TimeBaseStructure.TIM_Period = 250;       //自动重载值
    TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
      
}
void EnterStopMode()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
    GPIO_Init(GPIOB,&GPIO_InitStructure); 
    GPIO_Init(GPIOC,&GPIO_InitStructure); 

    //配置 KEY4_GPIO_PIN 中断唤醒退出 stop模式 
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_InitStructure.GPIO_Pin =KEY4_GPIO_PIN;
    GPIO_Init(KEY_GPIO_PORT,&GPIO_InitStructure);  

    /* Disable GPIOs clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |RCC_AHBPeriph_DMA1,
                           DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_DAC|RCC_APB1Periph_TIM3,
                            DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,
                            DISABLE);  
    /* DISABLE the ADC peripheral */
    ADC_Cmd(ADC1, DISABLE);  
//    RCC_LSEConfig(RCC_LSE_OFF);    
//    RCC_DeInit();

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);

    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //下降沿
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel =  EXTI4_15_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);                      
}

void EXTI4_15_IRQHandler()
{
    EXTI_ClearFlag(EXTI_Line7);
}


void Main_displyHandle()
{
	switch(state)
	{
        case STATE_NORMAL://正常显示
            switch(gdispmode)
                {
                    case   NORMAL_TIMEDISP:
                            gdisbuf[0]=segnum[gdisnum[0]];
                            gdisbuf[1]=segnum[gdisnum[1]]; 
                            gdisbuf[2]=segnum[gdisnum[2]];  
                            gdisbuf[3]=segnum[gdisnum[3]]; 
                            if(t_cnt&BIT(6))
                            {
                                    IOSET(gdisbuf[2],SEGDP); //点闪烁
                                    IOSET(gdisbuf[3],SEGDP);                           
                            }
                            break;
                    case   NORMAL_TEMPERDISP:
                            gdisbuf[0]=segnum[gdisnum[0]];
                            gdisbuf[1]=segnum[gdisnum[1]]; 
                            gdisbuf[2]=SEGA+SEGB+SEGF+SEGG;  
                            gdisbuf[3]=SEGA+SEGD+SEGE+SEGF;                                   
                            break;                                
                    default:
                            break;                            
                }
        
                break;
        case STATE_SETHOUR: //设置小时
                gdisbuf[0]=segnum[gdisnum[0]];
                gdisbuf[1]=segnum[gdisnum[1]]; 
                gdisbuf[2]=segnum[gdisnum[2]];  
                gdisbuf[3]=segnum[gdisnum[3]];                 
                if(t_cnt&BIT(6))
                {
                     gdisbuf[0]=0;//共阴数码管 为0不显示
                     gdisbuf[1]=0;                          
                }                         
                break;
        case STATE_SETMIN: //设置分钟
                gdisbuf[0]=segnum[gdisnum[0]];
                gdisbuf[1]=segnum[gdisnum[1]]; 
                gdisbuf[2]=segnum[gdisnum[2]];  
                gdisbuf[3]=segnum[gdisnum[3]];                 
                if(t_cnt&BIT(6))
                {
                     gdisbuf[2]=0;//共阴数码管 为0不显示
                     gdisbuf[3]=0;                          
                }                 
                break;                 
        case STATE_SETALHOUR:
                gdisbuf[0]=segnum[gdisnum[0]]; 
                gdisbuf[1]=segnum[gdisnum[1]]; 
                gdisbuf[2]=segnum[gdisnum[2]]|SEGDP;  
                gdisbuf[3]=segnum[gdisnum[3]]|SEGDP;                 
                if(t_cnt&BIT(6))
                {
                        gdisbuf[0]=0;//共阴数码管 为0不显示 
                        gdisbuf[1]=0;//共阴数码管 为0不显示                     
                }                         
                break;
        case STATE_SETALMIN:
                        gdisbuf[0]=segnum[gdisnum[0]]; 
                        gdisbuf[1]=segnum[gdisnum[1]]; 
                        gdisbuf[2]=segnum[gdisnum[2]]|SEGDP;  
                        gdisbuf[3]=segnum[gdisnum[3]]|SEGDP;                 
                        if(t_cnt&BIT(6))
                        {
                             gdisbuf[2]=SEGDP;//共阴数码管 为0不显示
                             gdisbuf[3]=SEGDP;                          
                        } 
                break; 
        case STATE_SETTIMER:
                        gdisbuf[0]=SEGA|SEGB|SEGC|SEGE|SEGF|SEGG; //显示 AL
                        gdisbuf[1]=SEGD|SEGE|SEGF; 
    //                        gdisbuf[2]=segnum[gsetmin/10]; //显示定时值 
    //                        gdisbuf[3]=segnum[gsetmin%10];  
                        if(gsetmin/10 == 0)//顯示 時 首位 為零 則不顯示  降低功耗
                        {
                                gdisbuf[2]=0;
                                gdisbuf[3]=segnum[gsetmin%10];                             
                        }
                        else
                        {
                                gdisbuf[2]=segnum[gsetmin/10]; 
                                gdisbuf[3]=segnum[gsetmin%10];                         
                        }
                break;                         
        case STATE_TIMERSTUDY:
            //gitmercnt 為十進制  顯示需要轉換為時間的進制
            //顯示 時 首位 為零 則不顯示  降低功耗
            if((gtimercnt/60)/10 == 0)//1
            {
                gdisbuf[0]=NULL;
                if((gtimercnt/60)%10 == 0)//2
                {
                    gdisbuf[1]=NULL;
                    if((gtimercnt%60)/10 == 0 )//3
                    {
                            gdisbuf[2]=NULL;  
                            gdisbuf[3]=segnum[(gtimercnt%60)%10];                                     
                    }
                    else
                    {
                            gdisbuf[2]=segnum[(gtimercnt%60)/10]; //
                            gdisbuf[3]=segnum[(gtimercnt%60)%10];                                   
                    }
                }
                else
                {
                    gdisbuf[1]=segnum[(gtimercnt/60)%10];                         
                    gdisbuf[2]=segnum[(gtimercnt%60)/10]; //
                    gdisbuf[3]=segnum[(gtimercnt%60)%10];                                
                }
                    
            }
            else
            {
                    gdisbuf[0]=segnum[(gtimercnt/60)/10]; //显示定时值 
                    gdisbuf[1]=segnum[(gtimercnt/60)%10];                         
                    gdisbuf[2]=segnum[(gtimercnt%60)/10]; //
                    gdisbuf[3]=segnum[(gtimercnt%60)%10];                        
            }
            switch(t_cnt/64)   //数码管中间两点 交替闪烁
            {
                case 0:
                        IOSET(gdisbuf[2],SEGDP);                            
                break;                         
                case 1:
                        IOSET(gdisbuf[3],SEGDP);                           
                break;                             
                default:
                break;
            }
        break;
        default:
        break;
	}   
}

void Main_500msHandle()
{
    if(mainflag&FLAG_500MS)
    {
        IOCLR(mainflag,FLAG_500MS);
        RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
        if(state==STATE_TIMERSTUDY)//計時模式
        {
            if(gtimercnt<=gsetmin*60)     //是否到了定时时间 
            {              
                if(RTC_TimeStructDes.RTC_Minutes<RTC_TimeStruct.RTC_Minutes)
                {
                   gtimercnt=(60*60+RTC_TimeStructDes.RTC_Minutes*60+RTC_TimeStructDes.RTC_Seconds)
                            -(RTC_TimeStruct.RTC_Minutes*60+RTC_TimeStruct.RTC_Seconds);
                }
                else
                {
                   gtimercnt=(RTC_TimeStructDes.RTC_Minutes*60+RTC_TimeStructDes.RTC_Seconds)
                            -(RTC_TimeStruct.RTC_Minutes*60+RTC_TimeStruct.RTC_Seconds);                
                }                
            }
            else
            {                      
                state=STATE_NORMAL;
                gtimercnt=0;
//                TTSReport_time(&RTC_TimeStruct); 
                gttsreporttimeflag =1;
            }
        }  
//        if(mainflag&FLAG_RDTM)//语音报时
//        {
//            IOCLR(mainflag,FLAG_RDTM);
//            TTSReport_time(&RTC_TimeStruct);
//        }
    }
}
void Main_1sHandle()
{
    static uint8_t cnt_1s;
    uint8_t setval = 10;
    if(mainflag&FLAG_1S)
    {
        IOCLR(mainflag,FLAG_1S);
		pr_debug("VoicePowerSwitchState :%d\r\n",GPIO_ReadOutputDataBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN));		

        if(g1scnt>0)
        {
            g1scnt--;            
        }
        
        cnt_1s++;
        if(cnt_1s>setval)
        {
            cnt_1s = 0;
        }
        if(state==STATE_NORMAL)//正常显示时 取显示码
        {
            if(cnt_1s < setval-2)
            {
                gdispmode = NORMAL_TIMEDISP;
                gdisnum[0]=RTC_TimeStruct.RTC_Hours/10;
                gdisnum[1]=RTC_TimeStruct.RTC_Hours%10;            
                gdisnum[2]=RTC_TimeStruct.RTC_Minutes/10;
                gdisnum[3]=RTC_TimeStruct.RTC_Minutes%10;                                      
            }
            else if(cnt_1s== setval-2)
            {
                gtermprature = ADC_GetTemper();   
                
                gdisnum[0]=gtermprature/10;
                gdisnum[1]=gtermprature%10;               
                gdispmode = NORMAL_TEMPERDISP;
							
                gsegdispflag =1;
                pr_debug("gerlight :%d\r\n",gerlight=SEGDetectLight());
                gsegdispflag =0;            
                gledlight = 3000/(gerlight/1000)+200;//自动调光算法
                if(gledlight>1000)
                gledlight = LEDMAXLIGHT ;
                pr_debug("gledlight :%d\r\n",gledlight);
            }
            else 
            {
                gtermprature = ADC_GetTemper();   
                
                gdisnum[0]=gtermprature/10;
                gdisnum[1]=gtermprature%10;               
                gdispmode = NORMAL_TEMPERDISP;                
            }                
                                              
        }            
        pr_debug("%02d-%02d-%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);           
    } 
}
uint8_t gvoicecheckplaystate;
uint8_t gvoicesendcmdtimer;
void USARTRX_Handle()
{
	uint8_t temp;
    static  uint8_t voicereadyflag =0;
    static  uint8_t svoicepalaystate =0;
    static  uint8_t svoiceplayendflag =0;
	uint8_t tbuf[2]={0};
	TTSData_Typedf tTTSData_Structure;    
	if(USARTRX_structure.rxcompflag==1)
	{
		USARTRX_structure.rxcompflag=0;
		pr_debug("\r\n\033[32m RX_START \033[0m\r\n");              
		for(temp=0;temp<USARTRX_structure.len;temp++)
		{
            pr_debug("%4x",USARTRX_structure.rxbuf[temp]);                
		}
		pr_debug("\r\nRX_END\r\n"); 
		tbuf[0]=USARTRX_structure.rxbuf[3];  
		tbuf[1]=USARTRX_structure.rxbuf[6];
		
		if(tbuf[0] == 0x3f)//voiceIc 上电复位标志
		{
			voicereadyflag =1;			
		}
	}
    
    if(gttsreporttimeflag == 1)
    {
        if(GPIO_ReadOutputDataBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN) == Bit_SET )
        {
            VoicePowerSwitch(ENABLE);//没上电 则上电            
        }
        if(voicereadyflag == 1)
        {
            voicereadyflag = 0;        
            TTSReport_time(&RTC_TimeStruct);
        }        
    }
    if(gvoiceplayenable == 1)
    {
        if(GPIO_ReadOutputDataBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN) == Bit_SET )
        {
            VoicePowerSwitch(ENABLE);//没上电 则上电            
        }
        if(voicereadyflag == 1)
        {
            if(tbuf[0] == 0x42&&tbuf[1] ==0x00)//命令有返回并且未播放
            {
                tTTSData_Structure.cnt =0;  
                tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0200;  //播放眼保健操
                tTTSData_Structure.cnt++;  
                USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt); 
                pr_debug("TTSCmdSend...\r\n");                    
            }
            else if(tbuf[0] == 0x42&&tbuf[1] ==0x01)
            {
                svoicepalaystate = 1;
            }
            if(svoicepalaystate == 1)//播放中 一秒查询一次播放状态 否则直接查看
            {
                if(g1scnt==0)
                {
                    g1scnt = 1;
                    USART_SendToDevice(0x42,0);//查询播放状态                                                                                         
                }
            }
            else
            {
                USART_SendToDevice(0x42,0);//查询播放状态                                                                                     
            }
        }
        else
        {
//            USART_SendToDevice(0x0c,0);//复位
//            pr_debug("RestCmdSend...\r\n");                                
        }  
    }
    else if(gvoiceplayenable == 0 && gttsreporttimeflag == 0)
    {
        if(GPIO_ReadOutputDataBit(VOICEPOWERCONTR_PORT,VOICEPOWERCONTR_GPIO_PIN) == Bit_RESET )        
        VoicePowerSwitch(DISABLE);		//断电停止播放
    }
//文件播放结束 断电
////////////////////////////
    if(tbuf[0] == 0x3e)//结束播放标志出现 
    {
        g1scnt = 3;
        svoiceplayendflag =1;
    }     
    if(svoiceplayendflag == 1)//并且2秒后 没有出现第二次 
    {
        voicereadyflag=0;
        if(g1scnt == 0)
        {
            svoiceplayendflag=0;            
            gvoiceplayenable = 0;  
            gttsreporttimeflag = 0;//tts播放标志清零               
        }                                     
    }  
///////////////////////////////    
    tbuf[0]=tbuf[1]=0;
       
}
void Main_AlarmHandle(void)
{                                                                                      
    if(RTC_TimeStruct.RTC_Hours>=7&&RTC_TimeStruct.RTC_Hours<=22)//早上七点 到 晚上 10半之间 半点报时
    {
        if(RTC_TimeStruct.RTC_Minutes ==0 ||RTC_TimeStruct.RTC_Minutes ==30)
        {
//            if(RTC_TimeStruct.RTC_Seconds<1)
//            TTSReport_time(&RTC_TimeStruct);
            if(gttsreporttimeflag == 0&&RTC_TimeStruct.RTC_Seconds<1)
            {
                gttsreporttimeflag = 1;
            }
        }    
    }
}

void USARTRX2_Handle()
{
    uint8_t temp;
    uint8_t tbuf[2]={0};
    if(USARTRX2_structure.rxcompflag==1)
    {
        USARTRX2_structure.rxcompflag=0;
        pr_debug("\r\n\033[32mRX2_START \033[0m\r\n");              
        for(temp=0;temp<USARTRX2_structure.len;temp++)
        {
            pr_debug("%4x",USARTRX2_structure.rxbuf[temp]);                
        }
        pr_debug("\r\n\033[31mRX2_END\033[0m\r\n"); 
        tbuf[0] = USARTRX2_structure.rxbuf[1];
        //command handle
        switch(tbuf[0])
        {
            case 'a' :
                gsegdispflag =1;
                pr_debug("@gerlight :%d\r\n",gerlight=SEGDetectLight());
                gsegdispflag =0;   
            break;
            case 'b' :
                PrintfClk();  //打印时钟信息                   
            break;
            case 'c' :
                pr_debug("@闹钟设定时间  ：%d-%d\r\n",galhh,galmin);
                pr_debug("@定时器设定时间：%d\r\n",gsetmin);
                pr_debug("@温度：%d度\r\n",gtermprature);
            break; 
            case '?' :
                pr_debug("@帮助文件【help】\r\n");
            break;            
            default:
            break;
        }
    }
      
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





