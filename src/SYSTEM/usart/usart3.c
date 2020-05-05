#include "sys.h"
#include "usart3.h"
#include "LED.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif

#if EN_USART3_RX

u8 USART3_RX_BUF[USART3_REC_LEN];
u8 USART3_TX_BUF[USART3_TEC_LEN];

u16 USART3_RX_STA = 0;

void uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 /*| RCC_APB1Periph_USART3 */, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB /*| RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOC */ | RCC_APB2Periph_AFIO, ENABLE);

	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void)
{
	u8 Temp_Clear_IDLE = Temp_Clear_IDLE;
#ifdef OS_TICKS_PER_SEC
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		Temp_Clear_IDLE = USART3->SR;
		Temp_Clear_IDLE = USART3->DR;
		USART3_RX_STA |= 0x8000;
	}
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = USART_ReceiveData(USART3);
		USART3_RX_STA++;
	}
#ifdef OS_TICKS_PER_SEC
	OSIntExit();
#endif
}
#endif

void u3_printf(char *fmt, ...)
{
	u16 i, length;

	va_list ap;

	va_start(ap, fmt);
	vsprintf((char *)USART3_TX_BUF, fmt, ap);
	va_end(ap);

	length = strlen((const char *)USART3_TX_BUF);
	while ((USART3->SR & 0X40) == 0)
		;
	for (i = 0; i < length; i++)
	{
		USART3->DR = USART3_TX_BUF[i];
		while ((USART3->SR & 0X40) == 0)
			;
	}
}
