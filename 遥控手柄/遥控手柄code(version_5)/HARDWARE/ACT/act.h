#ifndef __act_H
#define __act_H
#include "stm32f10x.h"


void display_init(void);

void trans_analog_value(void);
void refresh_analog_value(void);

void writelastsent (u8 *str);
void writefeedback (u8 *str);
void work(void);
void drawp(u8 sw);
#endif

//------------------End of File----------------------------
