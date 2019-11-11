#ifndef __act_H
#define __act_H
#include "stm32f10x.h"


void display_init(void);
void refresh_mode(void);
void trans_analog_value(u8 sw);
void refresh_analog_value(void);

void writelastsent (u8 *str);
void writefeedback (u8 *str);
void work(void);
#endif

//------------------End of File----------------------------
