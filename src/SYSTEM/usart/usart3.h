#ifndef __USART3_H
#define __USART3_H

#include "sys.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define USART3_REC_LEN  			1024
#define USART3_TEC_LEN  			1024

#define EN_USART3_RX 			1

extern u8  USART3_RX_BUF[USART3_REC_LEN];
extern u8  USART3_TX_BUF[USART3_TEC_LEN];

extern u16 USART3_RX_STA;

void uart3_init(u32 bound);
void u3_printf(char* fmt,...);
#endif
