#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
#include "stm32f10x_tim.h"

extern unsigned int timer_1s;      //1s定时器
extern unsigned int timer_1s_flag; //标志位

void tim3_init(u16 arr, u16 psc);
void TIM3_IRQHandler(void);

#endif
