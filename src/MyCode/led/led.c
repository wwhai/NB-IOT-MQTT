#include "led.h"

void init_led_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);							 //使能PF端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);						 //关闭JTAG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3; // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);											 //根据设定参数初始化GPIOB

	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3); //初始化设置为高电平
}
