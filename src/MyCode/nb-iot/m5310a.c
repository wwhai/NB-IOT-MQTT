#include "sys.h"
#include "m5310a.h"
#include "delay.h"
#include "gpio.h"
#include "comma.h"

unsigned int scan_net_state_5000ms_timer = 0;    //巡检5S定时器
unsigned int scan_net_state_cmd_5000ms_flag = 0; //巡检5S标志位
unsigned char net_connect_server_flag = 0;       //网络状态标志位
unsigned char cgatt_flag = 0;                    //网络附着标志位

unsigned long int mqtt_rev_message_len = 0; //MQTT长度

int send_m5310a_cmd(unsigned char *cmd, u16 timeout, unsigned char *res)
{
    u16 time = 0;

    while (time < timeout)
    {
        if (time % 50 == 0)
        {
            time = 0;

            GSM_UART_RX_STA = 0;
            memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);

            GSM_UART_SEND(cmd);
        }

        if (GSM_UART_RX_STA & 0x8000)
        {
            if (strstr((char *)GSM_UART_RX_BUFF, (char *)res))
            {
                GSM_UART_RX_STA = 0;
                memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);
                break;
            }
        }

        time++;
        delay_ms(10);
    }

    if (time >= timeout)
        return -1;
    else
        return 0;
}

int m5310a_get_cimi(unsigned char *cimi)
{
    u16 time = 0;

    while (1)
    {
        if (time % 50 == 0)
        {
            time = 0;

            GSM_UART_RX_STA = 0;
            memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);

            GSM_UART_SEND("AT+CIMI\r\n");
        }

        if (GSM_UART_RX_STA & 0x8000)
        {
            cimi[0] = GSM_UART_RX_BUFF[2];
            cimi[1] = GSM_UART_RX_BUFF[3];
            cimi[2] = GSM_UART_RX_BUFF[4];
            cimi[3] = GSM_UART_RX_BUFF[5];
            cimi[4] = GSM_UART_RX_BUFF[6];
            cimi[5] = GSM_UART_RX_BUFF[7];
            cimi[6] = GSM_UART_RX_BUFF[8];
            cimi[7] = GSM_UART_RX_BUFF[9];
            cimi[8] = GSM_UART_RX_BUFF[10];
            cimi[9] = GSM_UART_RX_BUFF[11];
            cimi[10] = GSM_UART_RX_BUFF[12];
            cimi[11] = GSM_UART_RX_BUFF[13];
            cimi[12] = GSM_UART_RX_BUFF[14];
            cimi[13] = GSM_UART_RX_BUFF[15];
            cimi[14] = GSM_UART_RX_BUFF[16];

            break;
        }

        time++;
        delay_ms(10);
    }

    return 0;
}

//初始化M5310A的REST引脚 GPIO_Pin_7 初始化为低电平，关闭M5310A模组
void init_m5310a_res_pin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PF端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;             // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //根据设定参数初始化GPIOB

    GPIO_ResetBits(GPIOB, GPIO_Pin_7); //初始化设置为低电平
}

void init_m5310a(void)
{
    //初始化M5310A REST引脚使其关闭
    init_m5310a_res_pin();
    //m5310连接的是单片机的串口三，所以这里初始化串口三 波特率9600
    uart3_init(9600);
    //关闭M5310A模组
    M5310A_DIS;
}

//对网络状态进行巡检
void scan_net_state(void)
{
    unsigned char sub_message_out[1000];
    unsigned long int i = 0, j = 0;
    unsigned char mqtt_rev_message_len_tmp[10];
    unsigned char cimi[15];

    //如果没有附着网络
    if (cgatt_flag == 0)
    {
        //CGATT状态灯熄灭
        CGATT_LED_OFF;
    }

    //如果成功附着网络
    if (cgatt_flag == 1)
    {
        //CGATT状态灯打开
        CGATT_LED_ON;
    }

    //如果没有连接上MQTT服务器
    if (net_connect_server_flag == 0)
    {
        //开启模组
        M5310A_EN;
        //服务器状态灯熄灭
        NET_LED_OFF;

        //判断是否附着网络 如果没附着网络，则发送查询附着指令
        if (scan_net_state_cmd_5000ms_flag == 1 && cgatt_flag == 0)
        {
            scan_net_state_cmd_5000ms_flag = 0;
            GSM_UART_SEND("AT+CGATT?\r\n");
        }

        //获取MQTT的连接状态
        if (scan_net_state_cmd_5000ms_flag == 1 && cgatt_flag == 1)
        {
            scan_net_state_cmd_5000ms_flag = 0;

            GSM_UART_SEND("AT+MQTTSTAT?\r\n");
        }
    }

    //如果成功连接上MQTT服务器
    if (net_connect_server_flag == 1)
    {
        NET_LED_ON;
    }

    if (GSM_UART_RX_STA & 0x8000)
    {
        //已连接上网络
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+CGATT:1"))
        {
            //监测是否附着网络
            cgatt_flag = 1;

            //获取卡号
            GSM_UART_RX_STA = 0;
            memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);
            m5310a_get_cimi(cimi);
        }

        //没有初始化MQTT
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:0"))
        {
            //发送初始化命令
            GSM_UART_SEND("AT+MQTTCFG=\"732110.cicp.net\",39522,\"ZFW0101001000007\",30,\"dxx3PYrF\",\"N<9y9Cxt\",1\r\n");
        }

        //已经初始化MQTT
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:1"))
        {
            //发送连接命令
            GSM_UART_SEND("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");
        }

        //成功连接上服务器开始订阅
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:5"))
        {
            //保持心跳
            GSM_UART_SEND("AT+MQTTSUB=\"Clients/ZFW010100100\",0\r\n");
            GSM_UART_SEND("AT+MQTTPING=1\r\n");
            net_connect_server_flag = 1;
        }

        //监测错误
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"ERROR"))
        {
            //关闭M5310A模组
            M5310A_DIS;
            //发生错误后，重新监测并且连接
            cgatt_flag = 0;
            net_connect_server_flag = 0;
        }

        //服务机器断开
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTDISC:OK"))
        {
            //关闭M5310A模组
            M5310A_DIS;
            //发生错误后，重新监测并且连接
            cgatt_flag = 0;
            net_connect_server_flag = 0;
        }

        //收到订阅数据
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTPUBLISH"))
        {
            //+MQTTPUBLISH:0,0,0,0,Clients/ZFW0101001000007/Sub,146,
            GetCountRegionByFmt(GSM_UART_RX_BUFF, ',', 6, mqtt_rev_message_len_tmp);

            //获取MQTT接受到的长度
            mqtt_rev_message_len = atoi(mqtt_rev_message_len_tmp);
            delay_ms(500);
            for (i = (strlen((char *)GSM_UART_RX_BUFF) - (mqtt_rev_message_len + 2)); i < (strlen((char *)GSM_UART_RX_BUFF) - 2); i++)
            {
                sub_message_out[j++] = GSM_UART_RX_BUFF[i];

                u1_printf("%c", GSM_UART_RX_BUFF[i]);
            }

            memset(mqtt_rev_message_len_tmp, 0, sizeof(mqtt_rev_message_len_tmp));
            memset(sub_message_out, 0, sizeof(sub_message_out));
        }

        GSM_UART_RX_STA = 0;
        memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);
    }
}
