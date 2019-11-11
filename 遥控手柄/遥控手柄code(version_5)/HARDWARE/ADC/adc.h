#ifndef __adc_H
#define __adc_H
#include "stm32f10x.h"

extern u16 X_value;
extern u16 Y_value;
extern u16 A1_value;
extern u16 A2_value;
extern u16 AD_Value[4];


void Adc_Init(void);
void Dma_Init(void);
void Analog_Sample_Init(void);
void Analog_Sample_Switch(u8 sw);
void Get_Analog_Value(void);


#endif

//------------------End of File----------------------------
