#include "gpio.h"

void init_gpio_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							 //ʹ��PF�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);											 //�����趨������ʼ��GPIOB

	GPIO_ResetBits(GPIOA, GPIO_Pin_0); 
}
