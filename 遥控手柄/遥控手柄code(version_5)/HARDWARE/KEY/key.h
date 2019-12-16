#ifndef __key_H
#define __key_H
#include "stm32f10x.h"


extern u16 key_value;
//5-dir key used for windlass motor control 
extern u8 up1;
extern u8 down1;
extern u8 left1;
extern u8 right1;
extern u8 center1;
extern u8 pup1;
extern u8 pdown1;
extern u8 pleft1;
extern u8 pright1;
extern u8 pcenter1;

//5-dir key used for main-working robot contorl
extern u8 up2;
extern u8 down2;
extern u8 left2;
extern u8 right2;
extern u8 center2;
extern u8 pup2;
extern u8 pdown2;
extern u8 pleft2;
extern u8 pright2;
extern u8 pcenter2;

extern u8 nleft2;
extern u8 nright2;
//Valve1,Valve2,stopall key
extern u8 v1;
extern u8 v2;
extern u8 s;
extern u8 pv1;
extern u8 pv2;
extern u8 ps;


void  Key_Init(void);
void read_key(void);


#endif

//------------------End of File----------------------------
