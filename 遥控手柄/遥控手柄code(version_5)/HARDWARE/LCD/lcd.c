#include "lcd.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"	 

_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = WHITE,BACK_COLOR = BLACK;  
u16 DeviceCode;	 

/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPIv_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //输出数据
      else LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}

/****************************************************************************
* 名    称：u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
* 功    能：STM32_硬件SPI读写一个字节数据底层函数
* 入口参数：SPIx,Byte
* 出口参数：返回总线收到的数据
* 说    明：STM32_硬件SPI读写一个字节数据底层函数
****************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	 	//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPIx->DR;          	     //返回收到的数据			
} 

/****************************************************************************
* 名    称：void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
* 功    能：设置SPI的速度
* 入口参数：SPIx,SpeedSet
* 出口参数：无
* 说    明：SpeedSet:1,高速;0,低速;
****************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	SPIx->CR1&=0XFFC7;
	if(SpeedSet==1)//高速
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
	}
	else//低速
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPIx->CR1|=1<<6; //SPI设备使能
} 

/****************************************************************************
* 名    称：SPI2_Init(void)
* 功    能：STM32_SPI2硬件配置初始化
* 入口参数：无
* 出口参数：无
* 说    明：STM32_SPI2硬件配置初始化
****************************************************************************/
void SPI2_Init(void)	
{
SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	//配置SPI2管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;    
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_12);//片选信号为低
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//信号为低
	
	//SPI2配置选项
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	//使能SPI2
	SPI_Cmd(SPI2, ENABLE);   
}

//******************************************************************
//函数名：  LCD_WR_REG
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(u8 data)
{ 
  
   LCD_RS_CLR;
#if USE_HARDWARE_SPI   
   SPI_WriteByte(SPI2,data);
#else
   SPIv_WriteData(data);
#endif 
  
}


//******************************************************************
//函数名：  LCD_WR_DATA
//功能：    向液晶屏总线写入写8位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u8 data)
{
	
  
   LCD_RS_SET;
#if USE_HARDWARE_SPI   
   SPI_WriteByte(SPI2,data);
#else
   SPIv_WriteData(data);
#endif 
  
}

//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//功能：    8位总线下如何写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA_16Bit(u16 data)
{	
   
   LCD_RS_SET;
#if USE_HARDWARE_SPI   
   SPI_WriteByte(SPI2,data>>8);
   SPI_WriteByte(SPI2,data);
#else
   SPIv_WriteData(data>>8);
   SPIv_WriteData(data);
#endif 
  
}

//******************************************************************
//函数名：  LCD_WriteReg
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA_16Bit(LCD_RegValue);	    		 
}	   
	 
//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 


void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
   LCD_WR_DATA(x1>>8);
   LCD_WR_DATA(x1);
   LCD_WR_DATA(x2>>8);
   LCD_WR_DATA(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA(y1>>8);
   LCD_WR_DATA(y1);
   LCD_WR_DATA(y2>>8);
   LCD_WR_DATA(y2);

   LCD_WR_REG(0x2C);					 						 
}

//******************************************************************
//函数名：  LCD_DrawPoint
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	  
}


//******************************************************************
//函数名：  LCD_GPIOInit
//功能：    液晶屏IO初始化，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);      
}

//******************************************************************
//函数名：  LCD_Reset
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_RESET(void)
{
	//LCD_RST_CLR;
	delay_ms(20);	
	//LCD_RST_SET;
	delay_ms(20);
}
 	 
//******************************************************************
//函数名：  LCD_Init
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{  
#if USE_HARDWARE_SPI //使用硬件SPI
	SPI2_Init();
#else	
	LCD_GPIOInit();//使用模拟SPI
#endif  										 

 //	LCD_RESET(); //液晶屏复位

	//************* Start Initial Sequence **********//		
	//开始初始化液晶屏
LCD_WR_REG(0xCF);  
LCD_WR_DATA(0x00); 
LCD_WR_DATA(0xD9); 
LCD_WR_DATA(0X30); 

LCD_WR_REG(0xED);  
LCD_WR_DATA(0x64); 
LCD_WR_DATA(0x03); 
LCD_WR_DATA(0X12); 
LCD_WR_DATA(0X81); 
 
LCD_WR_REG(0xE8);  
LCD_WR_DATA(0x85); 
LCD_WR_DATA(0x10); 
LCD_WR_DATA(0x78); 

LCD_WR_REG(0xCB);  
LCD_WR_DATA(0x39); 
LCD_WR_DATA(0x2C); 
LCD_WR_DATA(0x00); 
LCD_WR_DATA(0x34); 
LCD_WR_DATA(0x02); 

LCD_WR_REG(0xF7);  
LCD_WR_DATA(0x20); 
 
LCD_WR_REG(0xEA);  
LCD_WR_DATA(0x00); 
LCD_WR_DATA(0x00); 
 
LCD_WR_REG(0xC0);    //Power control 
LCD_WR_DATA(0x21);   //VRH[5:0] 
 
LCD_WR_REG(0xC1);    //Power control 
LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] 
 
LCD_WR_REG(0xC5);    //VCM control 
LCD_WR_DATA(0x32); 
LCD_WR_DATA(0x3C); 
 
LCD_WR_REG(0xC7);    //VCM control2 
LCD_WR_DATA(0XC1); 
 
LCD_WR_REG(0x36);    // Memory Access Control 
LCD_WR_DATA(0x08); 
 
LCD_WR_REG(0x3A);   
LCD_WR_DATA(0x55); 

LCD_WR_REG(0xB1);   
LCD_WR_DATA(0x00);   
LCD_WR_DATA(0x18); 
 
LCD_WR_REG(0xB6);    // Display Function Control 
LCD_WR_DATA(0x0A); 
LCD_WR_DATA(0xA2); 
  
LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
LCD_WR_DATA(0x00); 
 
LCD_WR_REG(0x26);    //Gamma curve selected 
LCD_WR_DATA(0x01); 
 
LCD_WR_REG(0xE0);    //Set Gamma 
LCD_WR_DATA(0x0F); 
LCD_WR_DATA(0x20); 
LCD_WR_DATA(0x1E); 
LCD_WR_DATA(0x09); 
LCD_WR_DATA(0x12); 
LCD_WR_DATA(0x0B); 
LCD_WR_DATA(0x50); 
LCD_WR_DATA(0XBA); 
LCD_WR_DATA(0x44); 
LCD_WR_DATA(0x09); 
LCD_WR_DATA(0x14); 
LCD_WR_DATA(0x05); 
LCD_WR_DATA(0x23); 
LCD_WR_DATA(0x21); 
LCD_WR_DATA(0x00); 
 
LCD_WR_REG(0XE1);    //Set Gamma 
LCD_WR_DATA(0x00); 
LCD_WR_DATA(0x19); 
LCD_WR_DATA(0x19); 
LCD_WR_DATA(0x00); 
LCD_WR_DATA(0x12); 
LCD_WR_DATA(0x07); 
LCD_WR_DATA(0x2D); 
LCD_WR_DATA(0x28); 
LCD_WR_DATA(0x3F); 
LCD_WR_DATA(0x02); 
LCD_WR_DATA(0x0A); 
LCD_WR_DATA(0x08); 
LCD_WR_DATA(0x25); 
LCD_WR_DATA(0x2D); 
LCD_WR_DATA(0x0F); 
 
LCD_WR_REG(0x11);    //Exit Sleep 
delay_ms(120); 
LCD_WR_REG(0x29);    //Display on 

LCD_SetParam();
}
//******************************************************************
//函数名：  LCD_Clear
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{	
	u16 i,j;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	  
	for(i=0;i<lcddev.width;i++)
	{
		for(j=0;j<lcddev.height;j++)
		LCD_WR_DATA_16Bit(Color);	//???? 	 
	}
	
}   		  
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
#if USE_HORIZONTAL==1	//使用横屏
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar+3);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd+3);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar+2);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd+2);	

#else
	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	//LCD_WR_DATA(0x00FF&xStar+2);		
	LCD_WR_DATA(xStar);		
	LCD_WR_DATA(xEnd>>8);
	//LCD_WR_DATA(0x00FF&xEnd+2);
  LCD_WR_DATA(xEnd);		
	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	//LCD_WR_DATA(0x00FF&yStar+3);		
	LCD_WR_DATA(yStar);	
	LCD_WR_DATA(yEnd>>8);
	//LCD_WR_DATA(0x00FF&yEnd+3);	
	LCD_WR_DATA(yEnd);	
#endif

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);
} 


//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;			
	LCD_WriteReg(0x36,0xA8);
	
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
	LCD_WriteReg(0x36,0xC8);
#endif
}	