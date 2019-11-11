#ifndef __key_H
#define __key_H
#include "stm32f10x.h"


extern u16 key_value;
extern u8 up;
extern u8 down;
extern u8 left;
extern u8 right;
extern u8 pup;
extern u8 pdown;
extern u8 pleft;
extern u8 pright;

extern u8 ka;
extern u8 kb;
extern u8 kc;
extern u8 pa;
extern u8 pb;
extern u8 pc;
void  Key_Init(void);

void read_key(void);


#endif

//------------------End of File----------------------------
