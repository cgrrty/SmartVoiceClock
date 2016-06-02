
/******************** (C) COPYRIGHT  2016 ********************************

* �ļ���  ��
 * ����    ��
 * ��汾  ��V1.3.0
 * ����    ��DongXu
 
********************************************************************************/	


//ͷ�ļ�
#include "led.h"

 /**
  * @file   LED_GPIO_Config
  * @brief  LED����������
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_AHBPeriphClockCmd(LED1_GPIO_RCC|LED2_GPIO_RCC|LED3_GPIO_RCC,ENABLE);//ʹ��GPIO������ʱ��
	
	GPIO_InitStructure.GPIO_Pin =LED1_GPIO_PIN;	//ѡ��Ҫ�õ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//��������Ϊ��ͨ���ģʽ				
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //��������Ϊ����		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin =LED2_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =LED3_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
	
}

 /**
  * @file   LEDXToggle
  * @brief  LED����ת
  * @param  ledx:1--PC13;2--PB0;1--PB1
  * @retval ��
  */
void LEDXToggle(uint8_t ledx)
{
  if(ledx==1)
  {
    LED1_GPIO_PORT->ODR^=LED1_GPIO_PIN;
  }
  else if(ledx==2)
  {
    LED2_GPIO_PORT->ODR^=LED2_GPIO_PIN;
  }
  else
  {
    LED3_GPIO_PORT->ODR^=LED3_GPIO_PIN;
  }

}
