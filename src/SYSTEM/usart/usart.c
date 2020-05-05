#include "sys.h"
#include "usart.h"
#include "led.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"				  
#endif

#if 1
#pragma import(__use_no_semihosting)             
             
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
 
_sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

#if EN_USART1_RX   

u8 USART1_RX_BUF[USART1_REC_LEN]; 
u8 USART1_TX_BUF[USART1_TEC_LEN]; 

u16 USART1_RX_STA=0;         
  
void uart1_init(u32 bound){

	    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能USART1，GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART1);  //复位串口1
	
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
    GPIO_Init(GPIOA, &GPIO_InitStructure); //???PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //???PA10


   //USART ?????

	USART_InitStructure.USART_BaudRate = bound;//?????9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

    USART_Init(USART1, &USART_InitStructure); //?????
		
		
#if EN_USART1_RX		  //???????  
   //Usart1 NVIC ??
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????
		
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//??????
#endif
    USART_Cmd(USART1, ENABLE);                    //???? 

}

//u32 NOW_RX_LEN=0;

void USART1_IRQHandler(void)                
{
	u8 Temp_Clear_IDLE=Temp_Clear_IDLE;
#ifdef OS_TICKS_PER_SEC	 	
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{
		Temp_Clear_IDLE=USART1->SR;
		Temp_Clear_IDLE=USART1->DR;
		USART1_RX_STA|=0x8000;
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
			USART1_RX_BUF[USART1_RX_STA&0X7FFF]=USART_ReceiveData(USART1);//(USART1->DR)	
			USART1_RX_STA++;
		}
#ifdef OS_TICKS_PER_SEC	 
	OSIntExit();  											 
#endif
} 
#endif	

void u1_printf(char* fmt,...)  
{  
	u16 i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART1_TX_BUF);	
	while((USART1->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = USART1_TX_BUF[i];
		while((USART1->SR&0X40)==0);	
	}	
}
