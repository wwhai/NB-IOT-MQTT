#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED2 PBout(6)
#define LED3 PBout(5)
#define LED4 PBout(4)
#define LED5 PBout(3)

void init_led_pin(void);

#endif

