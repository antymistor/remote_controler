#include "delay.h"
#include "sys.h"
#include "lcd.h"
//#include "pic.h"
#include "gui.h"
#include "adc.h"
#include "key.h"
#include "usart.h"
#include "act.h"

#include "stdlib.h"

//u16 tri[2][7]={20,45,150,200,100,70,RED,
//               15,40,140,190,90,65,BLUE};
int main(void)
{	u16 i=0;	
	SystemInit();        //��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();	     //��ʱ��ʼ��
	LCD_Init();	         //Һ������ʼ��
	delay_ms(50);
	uart_init(115200);
	Analog_Sample_Init();
	Analog_Sample_Switch(1);
	delay_ms(10);
	Key_Init();
  LCD_Clear(BLACK);
  for(i=0;i<=3;i++){showhanzi(55+40*i,280,i+44,WHITE);}
  drawp(2);
	LCD_Clear(BLACK);
	work();

}
