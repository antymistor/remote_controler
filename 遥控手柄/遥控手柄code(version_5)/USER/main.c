#include "delay.h"
#include "sys.h"
#include "lcd.h"
//#include "pic.h"
#include "gui.h"
#include "adc.h"
#include "key.h"
#include "usart.h"
#include "act.h"
#include "bmp.h"
//int main(void)
//{//	u16 i=0;	
//	SystemInit();        //初始化RCC 设置系统主频为72MHZ
//	delay_init();	     //延时初始化
//	LCD_Init();	         //液晶屏初始化
//	delay_ms(50);
//	uart_init(115200);
//	Analog_Sample_Init();
//	Analog_Sample_Switch(1);
//	delay_ms(10);
//	Key_Init();
//	// work();
//	
//	
//}

//u8 ref=0;//刷新显示
//u16 vx=15542,vy=11165;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
//u16 chx=140,chy=146;//默认像素点坐标为0时的AD起始值
void xianshi()//显示信息
{   
	u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
//	showhanzi(10,0,0);  //中
//	showhanzi(45,0,1);  //景
//	showhanzi(80,0,2);  //园
//	showhanzi(115,0,3);  //电
//	showhanzi(150,0,4);  //子
//  LCD_ShowString(10,35,"2.4 TFT SPI 240*320");
//	LCD_ShowString(10,55,"LCD_W:");
	LCD_ShowNum(0,0,456,3,12);
//	LCD_ShowString(110,55,"LCD_H:");
	//LCD_ShowNum(160,55,LCD_H,3);	
	lx=10;ly=75;			
}



void showimage() //显示40*40图片
{
  	int i,j,k; 

	for(k=3;k<8;k++)
	{
	   	for(j=0;j<6;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<1600;i++)
			 { 	
				 		
			  	 LCD_WR_DATA(image[i*2+1]);	 
				 LCD_WR_DATA(image[i*2]);				
			 }	
		 }
	}
//	ref=0;				
}

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
	// work();
	
	LCD_Clear(WHITE); //清屏
	//BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

	//xianshi(); //显示信息
    //showimage(); //显示40*40图片
   work();
}