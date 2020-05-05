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
	//�ӳٳ�ʼ��
	delay_init();
	//���Դ��ڳ�ʼ��
	uart1_init(115200);
	u1_printf("start system...\n");
	//LED��ʼ��
	init_led_pin();
	//gpio��ʼ��
	init_gpio_pin();
	//M5310A��ʼ��
	init_m5310a();
	//��ʱ����ʼ��
	tim3_init(1000, 7200);
	//���Ź���ʼ��
	IWDG_Configuration();

	while (1)
	{
		//����Ѳ��
		scan_net_state();
		//ι��
		IWDG_Feed();
	}
}
