#ifndef __USART_H
#define __USART_H
	
#include "sys.h" 
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"

#define USART1_REC_LEN  			1024  	
#define USART1_TEC_LEN  			1024 

#define EN_USART1_RX 			1		
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; 
extern u8  USART1_RX_BUF[USART1_TEC_LEN]; 

extern u16 USART1_RX_STA;         		

void uart1_init(u32 bound);
void u1_printf(char* fmt,...);
#endif


