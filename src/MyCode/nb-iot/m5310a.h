#ifndef __M5310_H
#define __M5310_H

#include "usart3.h"
#include "sys.h"
#include "led.h"

#define CGATT_LED_ON LED2 = 0
#define CGATT_LED_OFF LED2 = 1
#define NET_LED_ON LED3 = 0
#define NET_LED_OFF LED3 = 1

#define GSM_UART_RX_BUFF USART3_RX_BUF
#define GSM_UART_RX_STA USART3_RX_STA
#define GSM_UART_REC_LEN USART3_REC_LEN
#define GSM_UART_SEND(S) u3_printf("%s", S)

#define M5310A_EN GPIO_SetBits(GPIOB, GPIO_Pin_7)    //高电平
#define M5310A_DIS GPIO_ResetBits(GPIOB, GPIO_Pin_7) //低电平

extern unsigned int scan_net_state_5000ms_timer; //巡检5S定时器
extern unsigned int scan_net_state_cmd_5000ms_flag;  //巡检5S标志位
extern unsigned char net_connect_server_flag;           //网络状态标志位
extern unsigned char cgatt_flag;                 //网络附着标志位

int send_m5310a_cmd(unsigned char *cmd, u16 timeout, unsigned char *res);
void init_m5310a_res_pin(void);
void init_m5310a(void);
void scan_net_state(void);

#endif
