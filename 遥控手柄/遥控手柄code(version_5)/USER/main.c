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
//	SystemInit();        //��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
//	delay_init();	     //��ʱ��ʼ��
//	LCD_Init();	         //Һ������ʼ��
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

//u8 ref=0;//ˢ����ʾ
//u16 vx=15542,vy=11165;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
//u16 chx=140,chy=146;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
void xianshi()//��ʾ��Ϣ
{   
	u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
//	showhanzi(10,0,0);  //��
//	showhanzi(45,0,1);  //��
//	showhanzi(80,0,2);  //԰
//	showhanzi(115,0,3);  //��
//	showhanzi(150,0,4);  //��
//  LCD_ShowString(10,35,"2.4 TFT SPI 240*320");
//	LCD_ShowString(10,55,"LCD_W:");
	LCD_ShowNum(0,0,456,3,12);
//	LCD_ShowString(110,55,"LCD_H:");
	//LCD_ShowNum(160,55,LCD_H,3);	
	lx=10;ly=75;			
}



void showimage() //��ʾ40*40ͼƬ
{
  	int i,j,k; 

	for(k=3;k<8;k++)
	{
	   	for(j=0;j<6;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//��������
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
	SystemInit();        //��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();	     //��ʱ��ʼ��
	LCD_Init();	         //Һ������ʼ��
	delay_ms(50);
	uart_init(115200);
	Analog_Sample_Init();
	Analog_Sample_Switch(1);
	delay_ms(10);
	Key_Init();
	// work();
	
	LCD_Clear(WHITE); //����
	//BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

	//xianshi(); //��ʾ��Ϣ
    //showimage(); //��ʾ40*40ͼƬ
   work();
}