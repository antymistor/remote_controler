#include "key.h"
#include "delay.h"
#include "stm32f10x_gpio.h"

u16 temp_key_value=0xffff;
u16 key_value=0x0000;
u8 up=0;
u8 down=0;
u8 left=0;
u8 right=0;
u8 pup=0;
u8 pdown=0;
u8 pleft=0;
u8 pright=0;

u8 ka=0;
u8 kb=0;
u8 kc=0;
u8 pa=0;
u8 pb=0;
u8 pc=0;

void  Key_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}			
 
void read_key(void)
{

temp_key_value=GPIO_ReadInputData(GPIOB);
key_value=~((temp_key_value>>5)|0xff80);

  pup=up;
	up=(key_value&0x0010)>>4;
	pup=up&&(!pup);
	
	pdown=down;
	down=(key_value&0x0002)>>1;
	pdown=down&&(!pdown);
	
	pleft=left;
	left=(key_value&0x0008)>>3;
	pleft=left&&(!pleft);
	
	pright=right;
	right=(key_value&0x0004)>>2;
	pright=right&&(!pright);
	
	
	pa=ka;
	ka=(key_value&0x0040)>>6;
	pa=ka&&(!pa);
	
	pb=kb;
	kb=(key_value&0x0020)>>5;
	pb=kb&&(!pb);
	
	pc=kc;
	kc=(key_value&0x0001);
	pc=kc&&(!pc);

}





