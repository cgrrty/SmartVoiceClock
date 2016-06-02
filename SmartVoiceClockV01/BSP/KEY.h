#ifndef _KEY_H
#define _KEY_H

#include "stm32f0xx.h"
#include "main.h"

enum FLAGS {
FLAG_1S=BIT(0),     //1秒时间到
FLAG_IPRP=BIT(1),   //整点报时使能
FLAG_ALSW=BIT(2),   //闹钟开关
FLAG_LOWP=BIT(3),   //欠压
FLAG_RDTM=BIT(4),   //报时
FLAG_ALON=BIT(5),   //正在响闹铃
FLAG_K=BIT(6),      //按键标志
FLAG_STATE=BIT(7),  //状态改变
FLAG_KINC=BIT(8),   //按了INC键
FLAG_RDTP=BIT(9),   //读取温度
FLAG_BRT=BIT(10) ,   //测光   
FLAG_500MS=BIT(11), 
FLAG_250MS=BIT(12),   
};

enum KEYVAL{
KEY1PRESS=1,
KEY2PRESS=2,
KEY3PRESS=3,
KEY4PRESS=4,
KEY1LONGPRESS=KEY1PRESS+10,
KEY4LONGPRESS=KEY4PRESS+10
};

enum STATES{
STATE_NORMAL  =0,//正常显示
STATE_SETHOUR =1,//设置时钟小时
STATE_SETMIN  =2,//设置时钟分钟

STATE_ALSW=4,    
STATE_SETALHOUR =5,
STATE_SETALMIN  =6,

STATE_SETTIMER=7, 
    
STATE_TIMERSTUDY=8,  //计时模式做题 
 
};
enum dispmode
{
    NORMAL_TIMEDISP =0,   //时间显示
    NORMAL_TEMPERDISP=1   //温度显示    
};


#define KEY_GPIO_PORT  GPIOA

#define KEY1_GPIO_PIN  GPIO_Pin_4
#define KEY2_GPIO_PIN  GPIO_Pin_5
#define KEY3_GPIO_PIN  GPIO_Pin_6  
#define KEY4_GPIO_PIN  GPIO_Pin_7

#define KEYMASK  (BIT(4)|BIT(5)|BIT(6)|BIT(7))

void KEY_Init(void);
void KeyPro(uint8_t k);
uint8_t GetlsKey(uint8_t k);

#endif


