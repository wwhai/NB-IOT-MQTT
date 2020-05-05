#include "led.h"

void init_led_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);							 //ʹ��PF�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);						 //�ر�JTAG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3; // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);											 //�����趨������ʼ��GPIOB

	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3); //��ʼ������Ϊ�ߵ�ƽ
}
