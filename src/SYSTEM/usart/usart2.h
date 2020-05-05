#ifndef __USART2_H
#define __USART2_H

#include "sys.h" 
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"

#define USART2_REC_LEN  			1024  
#define USART2_TEC_LEN  			1024

#define EN_USART2_RX 			1	
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; 
extern u8  USART2_TX_BUF[USART2_TEC_LEN]; 

extern u16 USART2_RX_STA;         		

void uart2_init(u32 bound);
void u2_printf(char* fmt,...);
#endif


