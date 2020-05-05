#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "timer.h"
#include "adc.h"
#include "dma.h"
#include "sys.h"
#include "m5310a.h"
#include "gpio.h"

int main(void)
{
	//延迟初始化
	delay_init();
	//调试串口初始化
	uart1_init(115200);
	u1_printf("start system...\n");
	//LED初始化
	init_led_pin();
	//gpio初始化
	init_gpio_pin();
	//M5310A初始化
	init_m5310a();
	//定时器初始化
	tim3_init(1000, 7200);
	//开门狗初始化
	IWDG_Configuration();

	while (1)
	{
		//网络巡检
		scan_net_state();
		//喂狗
		IWDG_Feed();
	}
}
