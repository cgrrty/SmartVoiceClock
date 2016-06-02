

/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	




#include "ADC.h"
#include "usart.h"

#define ADC_ChannalNum     3
#define ADC1_DR_Address    0x40012440



#define Regular_Channal1Index       0
#define Regular_Channal8Index       1
#define Regular_ChannalTemperIndex  2
//#define Regular_ChannelRerfintIndex 2



__IO uint16_t RegularConvData_Tab[ADC_ChannalNum];

void ADC_DMAConfig()
{
    DMA_InitTypeDef   DMA_InitStructure;
    /* DMA1 clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

    /* DMA1 Channel1 Config */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_ChannalNum;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* DMA1 Channel1 enable */
    DMA_Cmd(DMA1_Channel1, ENABLE);       
}
void ADC_BSPInit()
{    
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    
    //PA1 ADC Config
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    /* ADC1 Peripheral clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
    /* ADC1 DeInit */  
    ADC_DeInit(ADC1);          
    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);    
    /* Configure the ADC1 in continous mode withe a resolution equal to 12 bits*/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1,&ADC_InitStructure);
    

    ADC_ChannelConfig(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1,ADC_Channel_1,ADC_SampleTime_239_5Cycles);					//测量电池电压通道
    ADC_ChannelConfig(ADC1,ADC_Channel_8,ADC_SampleTime_239_5Cycles);					//测亮光强度通道

    ADC_ClockModeConfig(ADC1,ADC_ClockMode_SynClkDiv2);    
    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);  


   /* ADC DMA request in circular mode */
   ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
   
   /* Enable ADC_DMA */
   ADC_DMACmd(ADC1, ENABLE);  

    
    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);  
    /* Wait the ADRDY flag */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
    
	ADC_WaitModeCmd(ADC1,ENABLE);
		ADC_AutoPowerOffCmd(ADC1,ENABLE);
    ADC_VrefintCmd(ENABLE);
    ADC_TempSensorCmd(ENABLE);
//    ADC_VrefintCmd(ENABLE);
    /* ADC1 regular Software Start Conv */ 
    ADC_StartOfConversion(ADC1);
    
    ADC_DMAConfig();
    
}

#define  TEMP110_CAL_ADDR    ((uint16_t*) ((uint32_t) 0x1ffff7c2))
#define  TEMP30_CAL_ADDR     ((uint16_t*) ((uint32_t) 0x1ffff7b8))
#define  VDD_CALLB           ((uint16_t) (330))
#define  VDD_APPLI           ((uint16_t) (330))

int16_t temperature;

int32_t ADC_GetTemper()
{
    pr_debug("ADC1 START\r\n");  
    temperature = (((int16_t)RegularConvData_Tab[Regular_ChannalTemperIndex] * VDD_APPLI/VDD_CALLB) -(int16_t)*TEMP30_CAL_ADDR) ;
    temperature = temperature*(int16_t)(110 - 30);
    temperature = temperature/(int16_t)(*TEMP110_CAL_ADDR-*TEMP30_CAL_ADDR);
    temperature = temperature +30;
    return temperature-8;
}

float ADC_GetChannal1Value()
{
    float v;
    v = RegularConvData_Tab[Regular_Channal1Index]*3.4/4096*3; 
    return v;
}
float ADC_GetChannal8Value()
{
    float v;
//    v = RegularConvData_Tab[Regular_Channal0Index]*3.4/4096*3; 
  v = RegularConvData_Tab[Regular_Channal8Index]; 
	return v;
}










