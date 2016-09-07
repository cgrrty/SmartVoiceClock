
/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	

#include "KEY.h"
#include "SEG.h"
#include "usart.h"
#include "flash.h"
#include "VOICE.h"
extern uint16_t mainflag;
extern uint8_t t_cnt;
extern uint8_t g_tcnt;
extern uint8_t gk;
extern uint8_t state;
extern uint8_t gkvalue;
extern uint32_t gsetmin;
extern uint32_t    g_idlecnt;

extern const unsigned int seltab[];
extern  uint16_t     gdisnum[4];//存放数字
extern  uint8_t      gdisbuf[4];
extern  RTC_TimeTypeDef  RTC_TimeStruct;
extern  RTC_TimeTypeDef  RTC_TimeStructPre;
extern  RTC_TimeTypeDef  RTC_TimeStructDes;
extern  uint16_t    gtimercnt;

extern uint16_t     gledlight;
extern uint16_t     gerlight;
extern uint8_t  gsegdispflag;

extern uint8_t galhh; //al小时
extern uint8_t galmin;//al分钟

extern uint32_t gsystick;
extern uint8_t  gvoicecheckplaystate;
extern uint16_t gvoicepowertimer;
extern uint8_t gvoiceplayenable ;

uint8_t buf[]={0x7E, 0xFF ,0x06, 0x0C, 0x00 ,0x00 ,0x00 ,0xEF};
////
uint8_t buf1[]={0x7E, 0xFF ,0x06, 0x0D, 0x00 ,0x00 ,0x00 ,0xEF};
//7E FF 06 11 00 00 01 EF 复位
//7E FF 06 0D 00 00 00 EF 播放
void KEY_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin=KEY1_GPIO_PIN|KEY2_GPIO_PIN
							   |KEY3_GPIO_PIN|KEY4_GPIO_PIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure);        
}


uint8_t pk,k=0;
extern uint32_t systick;
void TIM3_IRQHandler()
{
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//    pr_debug(":%10d\r\n",systick);
	
	if(gsystick<127)
	{
		gsystick ++; 
	}
				
	if((t_cnt&15)==15)
	{
		IOSET(mainflag,FLAG_K);
		//------------------------
		pk=(GPIO_ReadInputData(KEY_GPIO_PORT)&KEYMASK);
		if((pk&KEYMASK)!=KEYMASK)//按键按下
		{
			if((pk&BIT(4))==0)k=1;
			if((pk&BIT(5))==0)k=2;
			if((pk&BIT(6))==0)k=3;
			if((pk&BIT(7))==0)k=4;			
			//if((pk&(BIT(4)|BIT(1)))==0)k=5;
			//if((pk&(BIT(4)|BIT(0)))==0)k=6; 
			if(k)
                gk=k;
		}
        else
            gk=0;
	}
	//-----------------------????
	t_cnt++;  //1s计时
    g_tcnt++; //500ms计时
    if(t_cnt%30 ==29)
    {
        IOSET(mainflag,FLAG_250MS);    
    }
    if(g_tcnt>64)
    {
        IOSET(mainflag,FLAG_500MS);
		g_tcnt=0;     
    }   
	if(t_cnt>127) //if(t_cnt>=2)//256/256=1Hz
	{
		t_cnt=0;
		IOSET(mainflag,FLAG_1S);
//		//-----------------????
//		if(kt<10)//????????????
//		{
//			kt++;
//			if(kt>=10)
//			{
//				if(state)
//				{	
//					state=0;
//					IOSET(mainflag,FLAG_STATE);
//				}
//			}
	} 
	for(k=0;k<4;k++)        //数码管扫描显示
	{
        if(gsegdispflag == 0)
        SEG_DisPlay(seltab[k],k,gledlight);
	}
}

//--------------------------------------------------
//
//--------------------------------------------------
uint8_t getlskey(uint8_t k1)
{
	uint8_t k;  //
	static uint8_t k0=0;//
	static uint8_t kwait=0,ktick=0;//
	k=k1;
	if((k0!=k1)&&(k0>0)&&(k1>0))//   
		k=0;
	if(k==0)          //
	{		
		if(kwait!=201)//短按
		{
			if(k0==1 || k0 == 4)//
				k=k0;//
		}
		kwait=0;
	}
	else            //
	{		
		if(kwait<=200)ktick++;//
		if(ktick>=kwait)//达到一定时间后 按键才有效
		{
			ktick=0;	
			if(kwait==0)kwait=8;//
			else if(kwait>1)kwait=1;
			if(k==1||k == 4)//
			{
				if(kwait<2)//
				{
					kwait=201;
					k=k+10;//
				}
				else
				{
					k=0;
				}
			}
			if(k==2||k==3)
			{
				kwait=201;
			}
		}
		else
		{
			k=0;
		}
	}
	k0=k1;//
	return k;
}
//uint8_t tp_hh;
//uint8_t tp_min;
//--------------------------------------------------
//--------------------------------------------------
void KeyPro(uint8_t k)
{
	
    static uint8_t tp_hh=0;
    static uint8_t tp_min=0; 

    TTSData_Typedf tTTSData_Structure;

    RTC_AlarmTypeDef RTC_AlarmStructure;
    
    uint8_t keyval=0;
    gkvalue=getlskey(k);
    keyval=gkvalue;
    
	if(keyval)
	{
       g_idlecnt = 0 ;//如果有按键按下 休眠计数清零
	}
	switch(keyval)
	{
        case 0:
            break;
        case KEY1PRESS:
            //设置定时时间
            if(state==STATE_NORMAL)
            {
                state=STATE_SETTIMER;
                break; 
            } 
            if(state==STATE_SETTIMER)
            {
//                state=STATE_SETALHOUR;  
                state=STATE_NORMAL;                  
                break;
            }
            //设置AL 时间 小时分钟
            if(state==STATE_SETALHOUR)
            {
                state=STATE_SETALMIN; 
                
                
                //语音                
                tTTSData_Structure.cnt =0;  

                tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x010c;//分钟
                tTTSData_Structure.cnt++;
                //语音结束
                USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt);                
                break;
            }            
            if(state==STATE_SETALMIN)
            {                                
                //    /* Disable the Alarm A */
                RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
                RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = galhh;
                RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = galmin;
                RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
                //    /* Set the Alarm A */
                RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
                RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;    
                RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

                /* Configure the RTC Alarm A register */
                RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
                printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
//                RTC_AlarmShow();

                /* Enable the RTC Alarm A Interrupt */
                RTC_ITConfig(RTC_IT_ALRA, ENABLE);

                /* Enable the alarm  A */
                RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
                
                
                state=STATE_NORMAL;  
                //语音                
                tTTSData_Structure.cnt =0;  
                tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x011a;//退出
                tTTSData_Structure.cnt++;
                tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0113;//设置
                tTTSData_Structure.cnt++;                
                USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt); 
                //语音结束                
                break;
            }              
            //设置时钟小时 、分钟           
            if(state==STATE_SETHOUR)
            {
                state=STATE_SETMIN;  
               //语音
               tTTSData_Structure.cnt =0;  

               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x010c;//分钟
               tTTSData_Structure.cnt++;
                
               USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt);
               //语音 结束                  
                break;
            }
            if(state==STATE_SETMIN)
            {
                 if((tp_hh!=RTC_TimeStruct.RTC_Hours)||(tp_min!=RTC_TimeStruct.RTC_Minutes))
                 {
                     RTC_TimeStruct.RTC_Hours=tp_hh;
                     RTC_TimeStruct.RTC_Minutes=tp_min;
                     RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);  //如果未设置 则不从新设置时间               
                 }
                 state=STATE_NORMAL; 
//                 state=STATE_SETALHOUR;  
//                 
//               //更新数码管显示
//               gdisnum[0]=galhh/10;
//               gdisnum[1]=galhh%10;
//               gdisnum[2]=galmin/10;
//               gdisnum[3]=galmin%10;
//               //更新数码管显示结束   
               
               //语音
               tTTSData_Structure.cnt =0;  
               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0112;//闹钟
               tTTSData_Structure.cnt++;
                
               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0113;//设置
               tTTSData_Structure.cnt++;

               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0116;//小时
               tTTSData_Structure.cnt++;
                
               USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt);
               //语音 结束                   
                 break;
            }
           
            break;        
        case KEY2PRESS:
            //播放眼保健操
            if(state == STATE_NORMAL)
            {
                if(gvoiceplayenable == 0)
                {
                    gvoiceplayenable = 1;
                    break;
                }
                else if(gvoiceplayenable == 1)
                {
                    gvoiceplayenable = 0;
                    break;                
                }
                gvoicecheckplaystate=1;
                break;
                
//                tTTSData_Structure.cnt =0;  
//                tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0200;  //播放眼保健操
//                tTTSData_Structure.cnt++;  
//                USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt);  
//                break;                
            }
            //定時分鐘 增加
            if(state==STATE_SETTIMER)
            {
                if(gsetmin<59)
                    gsetmin++;
                else
                    gsetmin=0;
                break;
            }
            //调整小时分钟 增加
            if(state==STATE_SETHOUR)
            {
                if( tp_hh<23)
                tp_hh++;
                else tp_hh=0;
                gdisnum[0]=tp_hh/10;
                gdisnum[1]=tp_hh%10;
                break;
            }
            if(state==STATE_SETMIN)
            {
                if( tp_min<59)
                tp_min++;
                else tp_min=0;
                gdisnum[2]=tp_min/10;
                gdisnum[3]=tp_min%10;                
                break;
            }  
            //设置AL 时间 小时分钟 加一
            if(state==STATE_SETALHOUR)
            {
                if( galhh<23)
                galhh++;
                else galhh=0;
                gdisnum[0]=galhh/10;
                gdisnum[1]=galhh%10;
                
                break;
            }            
            if(state==STATE_SETALMIN)
            {
                if( galmin<59)
                galmin++;
                else galmin=0;
                gdisnum[2]=galmin/10;
                gdisnum[3]=galmin%10;                 
                break;
            }            
            break;
        case KEY3PRESS:
//            IOSET(mainflag,FLAG_RDTM);
            //定時分鐘 减一
            if(state==STATE_SETTIMER)
            {
                if(gsetmin>0)
                    gsetmin--;
                else
                    gsetmin=59;
                break;
            }
            //调整小时分钟 減小
            if(state==STATE_SETHOUR)
            {
                if( tp_hh>0)
                tp_hh--;
                else tp_hh=23;
                gdisnum[0]=tp_hh/10;
                gdisnum[1]=tp_hh%10;
                break;
            }
            if(state==STATE_SETMIN)
            {
                if( tp_min>0)
                tp_min--;
                else tp_min=59;
                gdisnum[2]=tp_min/10;
                gdisnum[3]=tp_min%10;                
                break;
            }
            //设置AL 时间 小时分钟 减一
            if(state==STATE_SETALHOUR)
            {
                if( galhh>0)
                galhh--;
                else galhh=23;
                gdisnum[0]=galhh/10;
                gdisnum[1]=galhh%10;                
                break;
            }            
            if(state==STATE_SETALMIN)
            {
                if( galmin>0)
                galmin--;
                else galmin=59;
                gdisnum[2]=galmin/10;
                gdisnum[3]=galmin%10;                
                break;
            }              
            //
//            break; 
            if(state==STATE_NORMAL)
            {
                state=STATE_TIMERSTUDY;  //进入计时模式
//                FlashWrite(FLASH_USER_START_ADDR,gsetmin);//保存变量到Flash 
                RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
                if(gsetmin!=0)//设置定时目标时间  分钟为60进制 注意越界问题
                {
                   gtimercnt=0;
                   if((gsetmin+RTC_TimeStruct.RTC_Minutes)<=59)
                    {
                        RTC_TimeStructDes.RTC_Minutes =gsetmin+RTC_TimeStruct.RTC_Minutes;                        
                    }
                    else
                    {
                        RTC_TimeStructDes.RTC_Minutes =(gsetmin+RTC_TimeStruct.RTC_Minutes)%60; 
                        
                    }  
                    RTC_TimeStructDes.RTC_Seconds=RTC_TimeStruct.RTC_Seconds;                    
                }
                break;
            }
            if(state==STATE_TIMERSTUDY)
            {
                state=STATE_NORMAL;  
                break;
            }            
            break;             
        case KEY4PRESS://休眠 或者唤醒
                if(gsystick>=127)   //休眠 唤醒操作佉抖	
                {
                    EnterStopMode();
                    NVIC_SystemReset();                  
                }         
            break;
        case KEY1LONGPRESS:
            if(state==STATE_NORMAL)
            {
               state=STATE_SETHOUR; 
               tp_hh=RTC_TimeStruct.RTC_Hours;  
               tp_min=RTC_TimeStruct.RTC_Minutes;               
               pr_debug("tp_hh%dtp_min%d\r\n",tp_hh,tp_min);
               //更新数码管显示
               gdisnum[0]=tp_hh/10;
               gdisnum[1]=tp_hh%10;
               gdisnum[2]=tp_min/10;
               gdisnum[3]=tp_min%10;

               //更新数码管显示结束

               //语音
               tTTSData_Structure.cnt =0;  
               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0114;//时间
               tTTSData_Structure.cnt++;
                
               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0113;//设置
               tTTSData_Structure.cnt++;

               tTTSData_Structure.buf[tTTSData_Structure.cnt]=0x0116;//小时
               tTTSData_Structure.cnt++;
                
               USART_TTSCmd(tTTSData_Structure.buf,tTTSData_Structure.cnt);
               //语音 结束                
            }
            break; 
        case  KEY4LONGPRESS:
//                EnterStopMode();
//                NVIC_SystemReset();               
            break;
        default:
            break;
	}
    if(keyval!=0)
    {
//        printf("keypress: %2d\r\n",keyval);
    }
}









