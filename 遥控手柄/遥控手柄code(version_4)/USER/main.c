#include "delay.h"
#include "sys.h"
#include "lcd.h"
//#include "pic.h"
#include "gui.h"
#include "adc.h"
#include "key.h"
#include "usart.h"
#include "act.h"
int main(void)
{//	u16 i=0;	
	SystemInit();        //初始化RCC 设置系统主频为72MHZ
	delay_init();	     //延时初始化
	LCD_Init();	         //液晶屏初始化
	delay_ms(50);
	uart_init(115200);
	Analog_Sample_Init();
	Analog_Sample_Switch(1);
	delay_ms(10);
	Key_Init();
	work();
}

