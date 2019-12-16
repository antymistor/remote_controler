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
#include "math.h"
#define backcolor BLACK
#define OPModeAnalogmax 999.f
#define CPModeAnalogmax 200.f
#define OPModeAnalogmin 0.f
#define CPModeAnalogmin 50.f
#define Actuatorspmax 99.f
#define Actuatorspmin 1.f
#define pointerlen 22



#define sampleperiod    20   //unit:ms
#define lcdrefreshperiod 100 //unit:ms

u8 mode=0;
u8  mode1=0;  //位移量控制模式 0：开环控制  1：闭环控制  2：自由控制
u8  mode2=0;  //喷头控制模式   0：自动控制  2：手动控制  
u8  mode1flag=0;
u8  mode2flag=0;

u16 Analogvalue1=0;
u16 Analogvalue2=0;
int Analogvalue3=0;
int Analogvalue4=0;
int Analogvalue5=0;
int Analogvalue6=0;

u16 value3base=2050;
u16 value4base=2050;
u16 value5base=2050;
u16 value6base=2050;

int value3temp=2050;
int value4temp=2050;
int value5temp=2050;
int value6temp=2050;

u8 Valve1=0;
u8 Valve2=0;

//游戏模式相关参数
u16 cir[20][4];
u16 playx=120,playy=120;


void trans_analog_value(void)
{
	// The code below is used to readjust the value used to control winding motor
	if(mode1==0){Analogvalue1=(u16)(AD_Value[0]*(OPModeAnalogmax-OPModeAnalogmin)/4095)+OPModeAnalogmin;}
	else if(mode1==1){Analogvalue1=(u16)(AD_Value[0]*(CPModeAnalogmax-CPModeAnalogmin)/4095)+CPModeAnalogmin;}
	else if(mode1==2){
	value4temp=AD_Value[2]-value4base;
	value3temp=AD_Value[3]-value3base;
	if(value4temp>100){Analogvalue4=value4temp*999.f/(4096-value4base);}
	else if((value4temp<-100)){Analogvalue4=value4temp*999.f/value4base;}
	else{Analogvalue4=0;}
	if(value3temp>100){Analogvalue3=(-value3temp*999.f/(4096-value3base));}
	else if(value3temp<-100){Analogvalue3=-value3temp*999.f/value3base;}
	else{Analogvalue3=0;}
	Analogvalue3=-Analogvalue3;             //Ver.
	Analogvalue4=-Analogvalue4;             //Hor.
	}
	
	// The code below is used to readjust the value used to control working arm
	Analogvalue2=(u16)(AD_Value[1]*(Actuatorspmax-Actuatorspmin)/4095)+Actuatorspmin;  
	
	// The code below is used to readjust the value used to control the movement of robot
	value6temp=AD_Value[4]-value6base;
	value5temp=AD_Value[5]-value5base;
	if(value6temp>100){Analogvalue6=value6temp*99.f/(4096-value6base);}
	else if((value6temp<-100)){Analogvalue6=value6temp*99.f/value6base;}
	else{Analogvalue6=0;}
	if(value5temp>100){Analogvalue5=(-value5temp*99.f/(4096-value5base));}
	else if(value5temp<-100){Analogvalue5=-value5temp*99.f/value5base;}
	else{Analogvalue5=0;}
	Analogvalue5=-Analogvalue5;             //Rot.
	Analogvalue6=-Analogvalue6;             //Mov.
	
}
void display_init(void)
{
	u8 i=0;
	
	LCD_DrawLine(30,7,65,7);LCD_DrawLine(30,8,65,8);
	LCD_DrawLine(210,7,175,7);LCD_DrawLine(210,8,175,8); for(i=0;i<=1;i++){showhanzi(78+18*i,0,i+17,BLUE);} showhanzi(114,0,20,BLUE); 	showhanzi(132,0,3,BLUE); showhanzi(150,0,4,BLUE);
		
	for(i=0;i<=3;i++){showhanzi(18*i,22,i+3,YELLOW);}      Show_Str(75,22,YELLOW,backcolor,":",16,1);  
	showhanzi(90,22,7,GREEN);showhanzi(108,22,9,GREEN);    showhanzi(126,22,3,GRED); showhanzi(144,22,4,GRED);                                                                //开环控制
	
	for(i=0;i<=1;i++){showhanzi(18*i,48,i+3,RED);} for(i=0;i<=1;i++){showhanzi(36+18*i,48,i+12,RED);} 
	for(i=0;i<=1;i++){showhanzi(18*i+90,40,i+15,BLUE);}   Show_Str(126,40,BLUE,backcolor,":",16,1);     LCD_DrawLineC(76,46,82,46,YELLOW);LCD_DrawLineC(76,70,82,70,YELLOW);
	showhanzi(90,60,14,RED); showhanzi(108,60,16,RED);  Show_Str(126,60,RED,backcolor,":",16,1);  LCD_DrawLineC(76,46,76,70,YELLOW);LCD_DrawLineC(77,46,77,70,YELLOW);  //控制参数
	
  for(i=0;i<=3;i++){showhanzi(18*i,78,i+17,BLUE);}	Show_Str(75,78,BLUE,backcolor,":",16,1);  Show_Str(130,78,YELLOW,backcolor,"mm",16,1);                                  //位移增量
	
	LCD_DrawLine(30,108,65,108);LCD_DrawLine(30,109,65,109);
	LCD_DrawLine(210,108,175,108);LCD_DrawLine(210,109,175,109);for(i=0;i<=1;i++){showhanzi(78+18*i,100,i+21,GRED);} showhanzi(114,100,20,GRED); showhanzi(132,100,3,GRED); showhanzi(150,100,4,GRED);
		
	for(i=0;i<=1;i++){showhanzi(18*i,120,i+23,RED);}for(i=0;i<=1;i++){showhanzi(36+18*i,120,i+3,RED);} Show_Str(75,120,RED,backcolor,":",16,1);  
	showhanzi(90,120,31,GREEN);showhanzi(108,120,32,GREEN);    showhanzi(126,120,3,RED); showhanzi(144,120,4,RED);                 //喷头控制
	for(i=0;i<=3;i++){showhanzi(18*i,140,i+23,BLUE);}Show_Str(75,140,BLUE,backcolor,":",16,1);                                     //喷头移速
	for(i=0;i<=1;i++){showhanzi(18*i,160,i+27,GREEN);}for(i=0;i<=1;i++){showhanzi(36+18*i,160,i+25,GREEN);}Show_Str(75,160,GREEN,backcolor,":",16,1);     //主体移速
	for(i=0;i<=1;i++){showhanzi(18*i,180,i+27,YELLOW);}Show_Str(75,180,YELLOW,backcolor,":",16,1);showhanzi(36,180,29,YELLOW); showhanzi(54,180,26,YELLOW);       //主体转速	
	
	LCD_DrawLine(30,210,65,210);LCD_DrawLine(30,211,65,211);
	LCD_DrawLine(210,210,175,210);LCD_DrawLine(210,211,175,211);for(i=0;i<=2;i++){showhanzi(78+18*i,202,i+33,RED);}  showhanzi(132,202,3,RED); showhanzi(150,202,4,RED); 
	showhanzi(0,222,36,GREEN);showhanzi(18,222,38,GREEN); showhanzi(36,222,35,GREEN);  Show_Str(55,222,GREEN,backcolor,":",16,1); showhanzi(90,222,34,RED);     //阀1
	showhanzi(0,242,37,YELLOW);showhanzi(18,242,38,YELLOW); showhanzi(36,242,35,YELLOW);  Show_Str(55,242,YELLOW,backcolor,":",16,1);	 showhanzi(90,242,34,RED);//阀2
	
	LCD_DrawLine(30,272,65,272);LCD_DrawLine(30,272,65,272);for(i=0;i<=4;i++){showhanzi(78+18*i,264,i+39,GREEN);}
	LCD_DrawLine(210,273,175,273);LCD_DrawLine(210,273,175,273);
//		writelastsent("oploop");
//		writefeedback("+123");
		
		value3base=AD_Value[3];
		value4base=AD_Value[2];
		value5base=AD_Value[5];
		value6base=AD_Value[4];
}



void togglemode1(void)
{
	mode1++;
	mode1=mode1%3;
	LCD_Fill(90,22,128,40,backcolor);
	if(!mode1)                                              //mode1=0  Open loop ctr
	{showhanzi(90,22,7,GREEN);showhanzi(108,22,9,GREEN);}  
	else if(mode1==1)                                       //mode1=1  Close loop ctr
	{showhanzi(90,22,8,RED);showhanzi(108,22,9,RED);}
	else if(mode1==2)                                       //mode1=2  Free rocker ctr
	{showhanzi(90,22,10,LBBLUE);showhanzi(108,22,11,LBBLUE);}
}
void togglemode2(void)
{
	mode2++;
	mode2=mode2%2;
	LCD_Fill(90,120,128,138,backcolor);
	if(!mode2)                                              //mode2=0  Auto ctr
	{showhanzi(90,120,31,GREEN);showhanzi(108,120,32,GREEN);}  
	else if(mode2==1)                                       //mode2=1  Manual ctr
	{showhanzi(90,120,30,BLUE);showhanzi(108,120,32,BLUE);}
}
void togglevalve(u8 sw)
{
	if(!sw){Valve1=(Valve1+1)%2;
		if(!Valve1){showhanzi(90,222,34,RED);}
		else{showhanzi(90,222,33,GREEN);} 
		}
	else{Valve2=(Valve2+1)%2;
		if(!Valve2){showhanzi(90,242,34,RED);}
		else{showhanzi(90,242,33,GREEN);} 
		}
}
void refresh_analog_value(void)
{
 trans_analog_value();
//The code below is for movement control display
 LCD_Fill(142,40,200,80,backcolor);
 if(!mode1){LCD_ShowNum(142,40,(u16)(Analogvalue1),3,16);LCD_ShowNum(142,60,(u16)(Analogvalue1),3,16);}
 else if(mode1==1){LCD_ShowNum(142,40,(u16)(Analogvalue1),3,16);}
 else if(mode1==2){
 
	if(Analogvalue3>=0)
	{Show_Str(142,40,GREEN,backcolor,"+",16,1);
	LCD_ShowNum(160,40,(u16)Analogvalue3,3,16);}
	else{Show_Str(142,40,BLUE,backcolor,"-",16,1);
	LCD_ShowNum(160,40,(u16)(-Analogvalue3),3,16);}
	if(Analogvalue4>=0)
	{Show_Str(142,60,GREEN,backcolor,"+",16,1);
	LCD_ShowNum(160,60,(u16)Analogvalue4,3,16);}
	else{Show_Str(142,60,BLUE,backcolor,"-",16,1);
	LCD_ShowNum(160,60,(u16)(-Analogvalue4),3,16);}
 }
 //The code below is for action control display
  LCD_Fill(93,140,173,200,backcolor);
  LCD_ShowNum(93,140,(u16)Analogvalue2,3,16);
  
 	if(Analogvalue5>=0)
	{Show_Str(93,180,GREEN,backcolor,"+",16,1);
	LCD_ShowNum(111,180,(u16)Analogvalue5,3,16);}
	else{Show_Str(93,180,BLUE,backcolor,"-",16,1);
	LCD_ShowNum(111,180,(u16)(-Analogvalue5),3,16);}
	if(Analogvalue6>=0)
	{Show_Str(93,160,GREEN,backcolor,"+",16,1);
	LCD_ShowNum(111,160,(u16)Analogvalue6,3,16);}
	else{Show_Str(93,160,BLUE,backcolor,"-",16,1);
	LCD_ShowNum(111,160,(u16)(-Analogvalue6),3,16);}
}

void writelastsent (u8 *str)
{
	LCD_Fill(80,284,240,324,backcolor);
	Show_Str(80,284,MAGENTA,backcolor,str,16,1);
}
void writefeedback (u8 *str)
{
	LCD_Fill(90,80,129,100,backcolor);
	Show_Str(90,80,GBLUE,backcolor,str,16,1);
}

void work(void)
{
	u8 count=lcdrefreshperiod/sampleperiod;
	u8 tempstr[20];
	display_init();

	while(1)
	{
	count=lcdrefreshperiod/sampleperiod;
	refresh_analog_value();
		while(count--)
		{
			delay_ms(sampleperiod);
			read_key();
		//	refresh_analog_value();
			if(key_value||nleft2||nright2)
			{
				if(pcenter1){togglemode1();printf("A2000B2000");writelastsent("STOP AB ALL");}
				if(pcenter2){togglemode2();printf("C2000");writelastsent("STOP C ALL");}
				if(pv1){togglevalve(0);printf("D000%d",Valve1);writelastsent("ToggleValve1");}
				if(pv2){togglevalve(1);printf("D100%d",Valve2);writelastsent("ToggleValve2");}
				if(pleft1){
					if(!mode1&&Analogvalue1>99){
						 printf("B4%d%d",(Analogvalue1/100),(Analogvalue1/10));
						 sprintf(tempstr,"%s%d","LEFT P:",(Analogvalue1/100)*100);
						 writelastsent(tempstr);
						 }
				}
				if(pright1){
					if(!mode1&&Analogvalue1>99){
						 printf("B3%d%d",(Analogvalue1/100),(Analogvalue1/10));
						 sprintf(tempstr,"%s%d","RIGHT P:",(Analogvalue1/100)*100);
						 writelastsent(tempstr);
						 }
				}
				if(pup1){
					if(!mode1){
						 printf("A3%d%d",(Analogvalue1/100),(Analogvalue1/10));
						 sprintf(tempstr,"%s%d","UP P:",(Analogvalue1/100)*100);
						 writelastsent(tempstr);
						 }
					else if(mode1==1){
				  	if(Analogvalue1>99){printf("C0%d",Analogvalue1);}else{printf("C00%d",Analogvalue1);}
					  sprintf(tempstr,"%s%d%s","UP: ",Analogvalue1,"mm");
				    writelastsent(tempstr);
					   }
				}
				if(pdown1){
					if(!mode1){
							 printf("A4%d%d",(Analogvalue1/100),(Analogvalue1/10));
							 sprintf(tempstr,"%s%d","DOWN P:",(Analogvalue1/100)*100);
							 writelastsent(tempstr);
							 }
						else if(mode1==1){
							if(Analogvalue1>99){printf("C1%d",Analogvalue1);}else{printf("C10%d",Analogvalue1);}
							sprintf(tempstr,"%s%d%s","DOWN: ",Analogvalue1,"mm");
							writelastsent(tempstr);
							 }
			  }
				if(ps){printf("A2000C2000");writelastsent("STOP ALL");}
				if(pleft2){
					if(!mode2){
						 if(Analogvalue2>9)
				        {printf("C50%d",Analogvalue2);}
				      	else
					     {printf("C500%d",Analogvalue2);}
					     sprintf(tempstr,"%s%d%s","AutoMove: ",(Analogvalue2)," L");
				       writelastsent(tempstr);
					}
				}
				if(pright2){
					if(!mode2){
						 if(Analogvalue2>9)
				        {printf("C51%d",Analogvalue2);}
				      	else
					     {printf("C510%d",Analogvalue2);}
					     sprintf(tempstr,"%s%d%s","AutoMove: ",(Analogvalue2)," R");
				       writelastsent(tempstr);
					}
				}
				if(pup2){
					printf("C5201");
					writelastsent("STOP ARM AUTO");
				}
				if(nleft2||nright2){
					if(mode2==1){
						printf("C5201");
						writelastsent("STOP ARM MAN.");
					}
				}
			}
	 }
	if(mode2==1){
	 read_key();
	// refresh_analog_value();
	 if(left2){
		 if(Analogvalue2>9)
				        {printf("C40%d",Analogvalue2);}
				      	else
					     {printf("C400%d",Analogvalue2);}
					     sprintf(tempstr,"%s%d%s","ManualMove:",(Analogvalue2),"L");
				       writelastsent(tempstr);
	 }
	 if(right2){
		 if(Analogvalue2>9)
				        {printf("C41%d",Analogvalue2);}
				      	else
					     {printf("C410%d",Analogvalue2);}
					     sprintf(tempstr,"%s%d%s","ManualMove:",(Analogvalue2),"R");
				       writelastsent(tempstr);
	 }
	}
	if(mode1==2)
	{
		if(Analogvalue3||Analogvalue4)
		{     
			    mode1flag=1;
		      if(Analogvalue4>99){printf("B0%d",Analogvalue4);}
					else if(Analogvalue4>9){printf("B00%d",Analogvalue4);}
					else if(Analogvalue4>0){printf("B000%d",Analogvalue4);}
					else if(Analogvalue4==0){printf("B2000");}
					else if(Analogvalue4>-10){printf("B100%d",(u16)(-Analogvalue4));}
					else if(Analogvalue4>-100){printf("B10%d",(u16)(-Analogvalue4));}
					else {printf("B1%d",(u16)(-Analogvalue4));}
				  //delay_ms(50);
					if(Analogvalue3>99){printf("A0%d",Analogvalue3);}
					else if(Analogvalue3>9){printf("A00%d",Analogvalue3);}
					else if(Analogvalue3>0){printf("A000%d",Analogvalue3);}
					else if(Analogvalue3==0){printf("A2000");}
					else if(Analogvalue3>-10){printf("A100%d",(u16)(-Analogvalue3));}
					else if(Analogvalue3>-100){printf("A10%d",(u16)(-Analogvalue3));}
					else {printf("A1%d",(u16)(-Analogvalue3));}
					sprintf(tempstr,"%s%d%s%d","V.:",Analogvalue3,"H.:",Analogvalue4);writelastsent(tempstr);
		}
		else if(mode1flag==1)
		{
			mode1flag=0;
			printf("A2000B2000");
			writelastsent("Stop AB");
		}
	}
	 if(Analogvalue5||Analogvalue6)
		{     
			    mode2flag=1; 
				  if(Analogvalue5>9){printf("C70%d",Analogvalue5);}
					else if(Analogvalue5>0){printf("C700%d",Analogvalue5);}
					else if(Analogvalue5==0){printf("C7200");}
					else if(Analogvalue5>-10){printf("C710%d",(u16)(-Analogvalue5));}
					else if(Analogvalue5>-100){printf("C71%d",(u16)(-Analogvalue5));}
					
				  //delay_ms(50);
					if(Analogvalue6>9){printf("C60%d",Analogvalue6);}
					else if(Analogvalue6>0){printf("C600%d",Analogvalue6);}
					else if(Analogvalue6==0){printf("C6200");}
					else if(Analogvalue6>-10){printf("C610%d",(u16)(-Analogvalue6));}
					else if(Analogvalue6>-100){printf("C61%d",(u16)(-Analogvalue6));}

					sprintf(tempstr,"%s%d%s%d","MOV",Analogvalue6," ROT",Analogvalue5);writelastsent(tempstr);
		}
		else if(mode2flag==1)
		{
			mode2flag=0;
			printf("C6200C7200");
			writelastsent("Stop C");
		}
		
  }

} 


void drawp(u8 sw)
{
u16 i=20,j=0;
u16 x,y,r,color;
int tx=0,ty=0;
	srand(AD_Value[1]);
	if(!sw)
{
  u16 x,y,color,r;
	while(1)
	{
		read_key();
		if(s){break;}
		x=(rand()%240+1);
		y=(rand()%240+1);
		r=(rand()%30+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		gui_circle(x, y,color,r,1);
		delay_ms(rand()%500+1);
	}
}
else if(sw==1)
{
	i=20;
	while(i)
	{
		x=(rand()%240+1);
		y=(rand()%240+1);
		r=(rand()%30+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[20-i][0]=x;cir[20-i][1]=y;cir[20-i][2]=r;cir[20-i][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(rand()%500+1);
		i--;
	}
	while(1)
	{
		read_key();
		if(s){break;}
		gui_circle(cir[0][0],cir[0][1],0,cir[0][2],1);
		i=19;
		while(i)
		{
		 cir[19-i][0]=cir[20-i][0];
		 cir[19-i][1]=cir[20-i][1];
		 cir[19-i][2]=cir[20-i][2];
		 cir[19-i][3]=cir[20-i][3];
		 i--;
		}
		x=(rand()%240+1);
		y=(rand()%240+1);
		r=(rand()%30+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[19][0]=x;cir[19][1]=y;cir[19][2]=r;cir[19][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(rand()%200+1);
	}
}
else if(sw==2)
{
		delay_ms(200);
	  value4base=AD_Value[2];
	  value3base=AD_Value[3];
	i=20;
	while(i)
	{
	value4temp=AD_Value[2]-value4base;
	value3temp=AD_Value[3]-value3base;
	if(value4temp>100){Analogvalue4=value4temp*20.f/(4096-value4base);}
	else if((value4temp<-100)){Analogvalue4=value4temp*20.f/value4base;}
	else{Analogvalue4=0;}
	if(value3temp>100){Analogvalue3=(-value3temp*20.f/(4096-value3base));}
	else if(value3temp<-100){Analogvalue3=-value3temp*20.f/value3base;}
	else{Analogvalue3=0;}
	
	playx=playx-Analogvalue4;
	playy=playy+Analogvalue3;
	if(playx>225){
	      if(playx>1000){playx=15;}
        else{playx=225;}
	      }
	if(playy>225){
	      if(playy>1000){playy=15;}
        else{playy=225;}
	      }
	x=playx;
	y=playy;
		r=(rand()%10+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[20-i][0]=x;cir[20-i][1]=y;cir[20-i][2]=r;cir[20-i][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(100);
		i--;
	}
	while(1)
	{
		read_key();
		if(s){break;}
		gui_circle(cir[0][0],cir[0][1],0,cir[0][2],1);
		i=19;
		while(i)
		{
		 cir[19-i][0]=cir[20-i][0];
		 cir[19-i][1]=cir[20-i][1];
		 cir[19-i][2]=cir[20-i][2];
		 cir[19-i][3]=cir[20-i][3];
		 i--;
		}
	
	value4temp=AD_Value[2]-value4base;
	value3temp=AD_Value[3]-value3base;
	if(value4temp>100){Analogvalue4=value4temp*20.f/(4096-value4base);}
	else if((value4temp<-100)){Analogvalue4=value4temp*20.f/value4base;}
	else{Analogvalue4=0;}
	if(value3temp>100){Analogvalue3=(-value3temp*20.f/(4096-value3base));}
	else if(value3temp<-100){Analogvalue3=-value3temp*20.f/value3base;}
	else{Analogvalue3=0;}
	playx=playx-Analogvalue4;
	playy=playy+Analogvalue3;
	if(playx>225){
	      if(playx>1000){playx=15;}
        else{playx=225;}
	      }
	if(playy>225){
	      if(playy>1000){playy=15;}
        else{playy=225;}
	      }
	x=playx;
	y=playy;
		r=(rand()%10+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[19][0]=x;cir[19][1]=y;cir[19][2]=r;cir[19][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(100);
	}
}
else if(sw==3)
{
		delay_ms(200);
	  
	i=20;j=0;
	while(i)
	{

	playx=playx+(rand()%40-20);
	playy=playy+(rand()%40-20);
	if(playx>225){
	      if(playx>1000){playx=15;}
        else{playx=225;}
	      }
	if(playy>225){
	      if(playy>1000){playy=15;}
        else{playy=225;}
	      }
	x=playx;
	y=playy;
		r=(rand()%10+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[20-i][0]=x;cir[20-i][1]=y;cir[20-i][2]=r;cir[20-i][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(100);
		i--;
	}
	while(1)
	{
		read_key();
		if(s){break;}
		gui_circle(cir[0][0],cir[0][1],0,cir[0][2],1);
		i=19;
		while(i)
		{
		 cir[19-i][0]=cir[20-i][0];
		 cir[19-i][1]=cir[20-i][1];
		 cir[19-i][2]=cir[20-i][2];
		 cir[19-i][3]=cir[20-i][3];
		 i--;
		}

	j++;
	playx=playx+20*sin(j/2)+(120-cir[19][0])/(rand()%5+5);
	playy=playy+20*cos(j/3)+(120-cir[19][1])/(rand()%5+5);
	if(playx>225){
	      if(playx>1000){playx=15;}
        else{playx=225;}
	      }
	if(playy>225){
	      if(playy>1000){playy=15;}
        else{playy=225;}
	      }
	x=playx;
	y=playy;
		r=(rand()%10+1);
		if(y+r>240){r=240-y;}
		if(y<r){r=y;}
		if(x+r>240){r=240-x;}
		if(x<r){r=x;}
		color=(u16)(rand()%0xffff);
		cir[19][0]=x;cir[19][1]=y;cir[19][2]=r;cir[19][3]=color;
		gui_circle(x, y,color,r,1);
		delay_ms(100);
	}
}

}

