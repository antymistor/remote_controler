#include "act.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "adc.h"
#include "key.h"
#include "usart.h"

#include "stm32f10x_gpio.h"
#include "stdio.h"
#include "stdlib.h"
#include "String.h"
#define backcolor BLACK
#define OPModeAnalogmax 999.f
#define CPModeAnalogmax 200.f
#define OPModeAnalogmin 0.f
#define CPModeAnalogmin 50.f
#define pointerlen 22

#define sampleperiod    20   //unit:ms
#define lcdrefreshperiod 100 //unit:ms
u8 mode=0;

u16 Analogvalue1=0;
u16 Analogvalue2=0;
int Analogvalue3=0;
int Analogvalue4=0;
u8  An1Len=0;
u8  An2Len=0;
u8  preAn1Len=0;
u8  preAn2Len=0;
short int xpointer=0;
short int ypointer=0;

u16 value3base=2050;
u16 value4base=2050;
int value3temp=2050;
int value4temp=2050;
void togglemode()
{
	if(!mode){mode=1;}
	else if(mode==1){mode=2;}
	else{mode=0;}
	display_init();
}

void trans_analog_value(u8 sw)
{
	if(mode)
	{
	Analogvalue1=(u16)(AD_Value[1]*(CPModeAnalogmax-CPModeAnalogmin)/4095)+CPModeAnalogmin;
	Analogvalue2=(u16)(AD_Value[0]*(CPModeAnalogmax-CPModeAnalogmin)/4095)+CPModeAnalogmin;
	}
	else
	{
	Analogvalue1=(u16)(AD_Value[1]*(OPModeAnalogmax-OPModeAnalogmin)/4095)+OPModeAnalogmin;
	Analogvalue2=(u16)(AD_Value[0]*(OPModeAnalogmax-OPModeAnalogmin)/4095)+OPModeAnalogmin;
	}
	
//不带修正版本
//	Analogvalue4=AD_Value[2];
//	Analogvalue3=AD_Value[3];
//	if(Analogvalue4>2143){Analogvalue4=(Analogvalue4-2113)*0.504036f;}
//	else if((Analogvalue4<2083)){Analogvalue4=(Analogvalue4-2113)*0.4727875f;}
//	else{Analogvalue4=0;}
//	if(Analogvalue3>2023){Analogvalue3=(1993-Analogvalue3)*0.47526f;}
//	else if(Analogvalue3<1963){Analogvalue3=(1993-Analogvalue3)*0.50125f;}
//	else{Analogvalue3=0;}
//	xpointer=(2048-AD_Value[3])*pointerlen/2048;
//	ypointer=(2048-AD_Value[2])*pointerlen/2048;
	
////带修正版本
	value4temp=AD_Value[2]-value4base;
	value3temp=AD_Value[3]-value3base;
	if(value4temp>50){Analogvalue4=value4temp*999.f/(4096-value4base);}
	else if((value4temp<-50)){Analogvalue4=value4temp*999.f/value4base;}
	else{Analogvalue4=0;}
	if(value3temp>50){Analogvalue3=(-value3temp*999.f/(4096-value3base));}
	else if(value3temp<-50){Analogvalue3=-value3temp*999.f/value3base;}
	else{Analogvalue3=0;}
	xpointer=Analogvalue3*pointerlen/1000;
	ypointer=-Analogvalue4*pointerlen/1000;
	
	if(sw)
	{      
	preAn1Len=An1Len;
	preAn2Len=An2Len;
	An1Len=(u8)(AD_Value[1]*128.f/4095);
	An2Len=(u8)(AD_Value[0]*128.f/4095);
	}
}
void display_init(void)
{
	LCD_Clear(backcolor);    //清屏
	Show_Str(0,0,WHITE,backcolor,"MODE:",12,1);
	
  LCD_DrawLine(0,63,128,63);
	LCD_DrawLine(0,64,128,64);
	Show_Str(0,65,WHITE,backcolor,"Last Sent:",12,1);
	 LCD_DrawLine(0,97,128,97);
	 LCD_DrawLine(0,98,128,98);
	Show_Str(0,99,WHITE,backcolor,"Feedback:",12,1);
	//mode=2;
	refresh_mode();

}




void refresh_mode(void)
{
	LCD_Fill(38,2,128,12,backcolor);
	LCD_Fill(0,10,75,24,backcolor);
	LCD_Fill(0,36,75,48,backcolor);
	if(mode==1)
	{
		Show_Str(38,0,YELLOW,backcolor,"CLOSE LOOP Ctr",12,1);
		Show_Str(0,12,BLUE,backcolor,"LenToCover1:",12,1);
		Show_Str(0,36,BLUE,backcolor,"LenToCover2:",12,1);
		Show_Str(105,12,WHITE,backcolor,"mm",12,1);
		Show_Str(105,36,WHITE,backcolor,"mm",12,1);
		An2Len=An1Len=0;
	}
	else if(mode==0)
	{
		Show_Str(38,0,GREEN,backcolor,"OPEN LOOP Ctr",12,1);
		Show_Str(0,12,GBLUE,backcolor,"1Step Power1:",12,1);
		Show_Str(0,36,GBLUE,backcolor,"1Step Power2:",12,1);
		Show_Str(105,12,WHITE,backcolor,"P*S",12,1);
		Show_Str(105,36,WHITE,backcolor,"P*S",12,1);
		An2Len=An1Len=0;
	}
	else
	{Show_Str(38,0,BRED,backcolor,"FREE ROCKER Ctr",12,1);
	 Show_Str(0,12,YELLOW,backcolor,"X-Direction:",12,1);
	 Show_Str(32,25,WHITE,backcolor,"mm/s",12,1);
	 Show_Str(0,36,RED,backcolor,"Y-Direction:",12,1);
	 Show_Str(32,49,WHITE,backcolor,"mm/s",12,1);
	LCD_Fill(72,13,128,62,GRED);
	LCD_Fill(74,15,126,60,backcolor);
  value4base=AD_Value[2];
	value3base=AD_Value[3];
	}
	LCD_DrawLine(0,11,128,11);
	LCD_DrawLine(0,12,128,12);
}

void refresh_analog_value(void)
{
	trans_analog_value(1);
if(mode!=2)
{
	LCD_Fill(76,13,104,24,backcolor);
	LCD_Fill(76,36,104,48,backcolor);
	LCD_ShowNum(85,12,Analogvalue1,3,12);
	LCD_ShowNum(85,36,Analogvalue2,3,12);
	if(An1Len>preAn1Len)
	{
		LCD_Fill(preAn1Len,25,An1Len,35,YELLOW);
	}
	else if(An1Len<preAn1Len)
	{
		LCD_Fill(An1Len,25,preAn1Len,35,backcolor);
	}
	if(An2Len>preAn2Len)
	{
		LCD_Fill(preAn2Len,49,An2Len,61,GREEN);
	}
	else if(An2Len<preAn2Len)
	{
		LCD_Fill(An2Len,49,preAn2Len,61,backcolor);
	}
	
//	writelastsent("A3730");
//	writefeedback("c5130");
}
else
{
	LCD_Fill(0,25,31,37,backcolor);
	LCD_Fill(0,49,31,61,backcolor);
	LCD_Fill(74,15,126,60,backcolor);
	LCD_DrawLine(100,37,(u8)(100+xpointer),(u8)(37+ypointer));
	if(Analogvalue3>=0)
	{Show_Str(0,25,GREEN,backcolor,"+",12,1);
	LCD_ShowNum(10,25,(u16)Analogvalue3,3,12);}
	else{Show_Str(0,25,BLUE,backcolor,"-",12,1);
	LCD_ShowNum(10,25,(u16)(-Analogvalue3),3,12);}
	
	if(Analogvalue4>=0)
	{Show_Str(0,49,GREEN,backcolor,"+",12,1);
	LCD_ShowNum(10,49,(u16)Analogvalue4,3,12);}
	else{Show_Str(0,49,BLUE,backcolor,"-",12,1);
	LCD_ShowNum(10,49,(u16)(-Analogvalue4),3,12);}
}
}

void writelastsent (u8 *str)
{
	LCD_Fill(0,78,128,94,backcolor);
	Show_Str(0,78,MAGENTA,backcolor,str,16,1);
}
void writefeedback (u8 *str)
{
	LCD_Fill(0,112,128,128,backcolor);
	Show_Str(0,112,GBLUE,backcolor,str,16,1);
}

void work(void)
{
	u8 count=lcdrefreshperiod/sampleperiod;
	u8 tempstr[10];
	display_init();
	while(1)
	{
		count=lcdrefreshperiod/sampleperiod;
	  while(count--)
	  {
			delay_ms(sampleperiod);
	    read_key();
			if(key_value)
			{
			if(pup)
			{trans_analog_value(0);
				if(!mode)
        {printf("A3%d%d",(Analogvalue1/100),(Analogvalue2/10));
				 sprintf(tempstr,"%s%d%s%d","UP P:",(Analogvalue1/100)*100," T:",(Analogvalue2/10));
				 writelastsent(tempstr);
				}
				else if(mode==1){
					if(Analogvalue1>99){printf("C0%d",Analogvalue1);}
					else{printf("C00%d",Analogvalue1);}
					sprintf(tempstr,"%s%d%s","UP: ",Analogvalue1,"mm");
				  writelastsent(tempstr);
				}
			}
			
			if(pdown)
			{trans_analog_value(0);
				if(!mode)
        {printf("A4%d%d",(Analogvalue1/100),(Analogvalue2/10));
				 sprintf(tempstr,"%s%d%s%d","DOWN P:",(Analogvalue1/100)*100," T:",(Analogvalue2/10));
				 writelastsent(tempstr);
				}
				else if(mode==1){
				if(Analogvalue1>99){printf("C1%d",Analogvalue1);}
				else{printf("C10%d",Analogvalue1);}
			  sprintf(tempstr,"%s%d%s","DOWN: ",Analogvalue1,"mm");
				writelastsent(tempstr);
				}
			}
			if(kb){printf("A2000C2000");writelastsent("STOP ALL");}
			if(pa){pa=0;togglemode();printf("A2000C2000");writelastsent("STOP ALL");}
			}
			if(mode==2)
			{   
				  refresh_analog_value();
					if(Analogvalue4>99){printf("A0%d",Analogvalue4);sprintf(tempstr,"%s%d%s","UP: ",Analogvalue4," P");writelastsent(tempstr);}
					else if(Analogvalue4>9){printf("A00%d",Analogvalue4);sprintf(tempstr,"%s%d%s","UP: ",Analogvalue4," P");writelastsent(tempstr);}
					else if(Analogvalue4>0){printf("A000%d",Analogvalue4);sprintf(tempstr,"%s%d%s","UP: ",Analogvalue4," P");writelastsent(tempstr);}
					else if(Analogvalue4==0){printf("A2000");writelastsent("STOPMOTOR");}
					else if(Analogvalue4>-10){printf("A100%d",(u16)(-Analogvalue4));sprintf(tempstr,"%s%d%s","DOWN: ",Analogvalue4," P");writelastsent(tempstr);}
					else if(Analogvalue4>-100){printf("A10%d",(u16)(-Analogvalue4));sprintf(tempstr,"%s%d%s","DOWN: ",Analogvalue4," P");writelastsent(tempstr);}
					else {printf("A1%d",(u16)(-Analogvalue4));sprintf(tempstr,"%s%d%s","DOWN: ",Analogvalue4," P");writelastsent(tempstr);}
					delay_ms(lcdrefreshperiod);
			}
	  }
		refresh_analog_value();
  } 
	
}


