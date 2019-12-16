#include "adc.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"

u16 AD_Value[6];
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6;	
	ADC_Init(ADC1, &ADC_InitStructure);	 

  ADC_RegularChannelConfig(ADC1, 	ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, 	ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );	
  ADC_RegularChannelConfig(ADC1, 	ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, 	ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );	
  ADC_RegularChannelConfig(ADC1, 	ADC_Channel_4, 5, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, 	ADC_Channel_5, 6, ADC_SampleTime_239Cycles5 );	
  ADC_DMACmd(ADC1,ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);	 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);	 
	while(ADC_GetCalibrationStatus(ADC1));	

}			
 
void Dma_Init(void)
{
	
DMA_InitTypeDef            DMA_InitStructure;	
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
DMA_DeInit( DMA1_Channel1 );   
DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 
DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;  
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 6; 
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
DMA_Init( DMA1_Channel1, &DMA_InitStructure );

}

void Analog_Sample_Init(void)
{
	Adc_Init();
	Dma_Init();
}
void Analog_Sample_Switch(u8 sw)
{
	if(sw)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		DMA_Cmd(DMA1_Channel1,ENABLE);
	}
	else
	{
		ADC_SoftwareStartConvCmd(ADC1,DISABLE);
		DMA_Cmd(DMA1_Channel1,DISABLE);
	}
}





