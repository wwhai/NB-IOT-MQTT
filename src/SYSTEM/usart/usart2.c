#include "sys.h"
#include "usart2.h"
#include "led.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif

#if EN_USART2_RX

u8 USART2_RX_BUF[USART2_REC_LEN];
u8 USART2_TX_BUF[USART2_TEC_LEN];

u16 USART2_RX_STA=0;

void uart2_init(u32 bound){

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 /*| RCC_APB1Periph_USART3 */, ENABLE);
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  /*| RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOC */|RCC_APB2Periph_AFIO  , ENABLE);

	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //USART3_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启空闲中断
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1

}

void USART2_IRQHandler(void)
{
	u8 Temp_Clear_IDLE=Temp_Clear_IDLE;
#ifdef OS_TICKS_PER_SEC
	OSIntEnter();
#endif
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		Temp_Clear_IDLE=USART2->SR;
		Temp_Clear_IDLE=USART2->DR;
		USART2_RX_STA|=0x8000;
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=USART_ReceiveData(USART2);
			USART2_RX_STA++;
		}
#ifdef OS_TICKS_PER_SEC
	OSIntExit();
#endif
}
#endif

void u2_printf(char* fmt,...)
{
	u16 i,length;

	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);

	length=strlen((const char*)USART2_TX_BUF);
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{
		USART2->DR = USART2_TX_BUF[i];
		while((USART2->SR&0X40)==0);
	}
}
