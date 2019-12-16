#include "key.h"
#include "delay.h"
#include "stm32f10x_gpio.h"

u16 temp_key_value=0xffff;
u16 key_value=0x0000;

//5-dir key used for windlass motor control 
u8 up1=0;
u8 down1=0;
u8 left1=0;
u8 right1=0;
u8 center1=0;
u8 pup1=0;
u8 pdown1=0;
u8 pleft1=0;
u8 pright1=0;
u8 pcenter1=0;

//5-dir key used for main-working robot contorl
u8 up2=0;
u8 down2=0;
u8 left2=0;
u8 right2=0;
u8 center2=0;
u8 pup2=0;
u8 pdown2=0;
u8 pleft2=0;
u8 pright2=0;
u8 pcenter2=0;

u8 nleft2=0;
u8 nright2=0;
//Valve1,Valve2,stopall key
u8 v1=0;
u8 v2=0;
u8 s=0;
u8 pv1=0;
u8 pv2=0;
u8 ps=0;

void  Key_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
	                               GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|
	                               GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}			
 
void read_key(void)
{

temp_key_value=GPIO_ReadInputData(GPIOB);
key_value=~((temp_key_value)|0xe000);
  
  pup1=up1;
	up1=(key_value&0x0020)>>5;
	pup1=up1&&(!pup1);
	pdown1=down1;
	down1=(key_value&0x0080)>>7;
	pdown1=down1&&(!pdown1);
	pleft1=left1;
	left1=(key_value&0x0040)>>6;
	pleft1=left1&&(!pleft1);
	pright1=right1;
	right1=(key_value&0x0010)>>4;
	pright1=right1&&(!pright1);
	pcenter1=center1;
	center1=(key_value&0x0008)>>3;
	pcenter1=center1&&(!pcenter1);
	
	pup2=up2;
	up2=(key_value&0x1000)>>12;
	pup2=up2&&(!pup2);
	pdown2=down2;
	down2=(key_value&0x0400)>>10;
	pdown2=down2&&(!pdown2);
	
	pleft2=left2;
	nleft2=left2;
	left2=(key_value&0x0800)>>11;
	pleft2=left2&&(!pleft2);
	nleft2=!left2&&(nleft2);
	
	pright2=right2;
	nright2=right2;
	right2=(key_value&0x0200)>>9;
	pright2=right2&&(!pright2);
	nright2=!right2&&(nright2);
	
	pcenter2=center2;
	center2=(key_value&0x0100)>>8;
	pcenter2=center2&&(!pcenter2);
	
	
	pv1=v1;
	v1=(key_value&0x0002)>>1;
	pv1=v1&&(!pv1);
  pv2=v2;
	v2=(key_value&0x0004)>>2;
	pv2=v2&&(!pv2);
	ps=s; 
	s=(key_value&0x0001);
	ps=s&&(!ps);

}





