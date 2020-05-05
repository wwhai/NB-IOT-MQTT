#include "sys.h"
#include "m5310a.h"
#include "delay.h"
#include "gpio.h"
#include "comma.h"

unsigned int scan_net_state_5000ms_timer = 0;    //Ѳ��5S��ʱ��
unsigned int scan_net_state_cmd_5000ms_flag = 0; //Ѳ��5S��־λ
unsigned char net_connect_server_flag = 0;       //����״̬��־λ
unsigned char cgatt_flag = 0;                    //���總�ű�־λ

unsigned long int mqtt_rev_message_len = 0; //MQTT����

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

//��ʼ��M5310A��REST���� GPIO_Pin_7 ��ʼ��Ϊ�͵�ƽ���ر�M5310Aģ��
void init_m5310a_res_pin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PF�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;             // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //�����趨������ʼ��GPIOB

    GPIO_ResetBits(GPIOB, GPIO_Pin_7); //��ʼ������Ϊ�͵�ƽ
}

void init_m5310a(void)
{
    //��ʼ��M5310A REST����ʹ��ر�
    init_m5310a_res_pin();
    //m5310���ӵ��ǵ�Ƭ���Ĵ����������������ʼ�������� ������9600
    uart3_init(9600);
    //�ر�M5310Aģ��
    M5310A_DIS;
}

//������״̬����Ѳ��
void scan_net_state(void)
{
    unsigned char sub_message_out[1000];
    unsigned long int i = 0, j = 0;
    unsigned char mqtt_rev_message_len_tmp[10];
    unsigned char cimi[15];

    //���û�и�������
    if (cgatt_flag == 0)
    {
        //CGATT״̬��Ϩ��
        CGATT_LED_OFF;
    }

    //����ɹ���������
    if (cgatt_flag == 1)
    {
        //CGATT״̬�ƴ�
        CGATT_LED_ON;
    }

    //���û��������MQTT������
    if (net_connect_server_flag == 0)
    {
        //����ģ��
        M5310A_EN;
        //������״̬��Ϩ��
        NET_LED_OFF;

        //�ж��Ƿ������� ���û�������磬���Ͳ�ѯ����ָ��
        if (scan_net_state_cmd_5000ms_flag == 1 && cgatt_flag == 0)
        {
            scan_net_state_cmd_5000ms_flag = 0;
            GSM_UART_SEND("AT+CGATT?\r\n");
        }

        //��ȡMQTT������״̬
        if (scan_net_state_cmd_5000ms_flag == 1 && cgatt_flag == 1)
        {
            scan_net_state_cmd_5000ms_flag = 0;

            GSM_UART_SEND("AT+MQTTSTAT?\r\n");
        }
    }

    //����ɹ�������MQTT������
    if (net_connect_server_flag == 1)
    {
        NET_LED_ON;
    }

    if (GSM_UART_RX_STA & 0x8000)
    {
        //������������
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+CGATT:1"))
        {
            //����Ƿ�������
            cgatt_flag = 1;

            //��ȡ����
            GSM_UART_RX_STA = 0;
            memset(GSM_UART_RX_BUFF, 0, GSM_UART_REC_LEN);
            m5310a_get_cimi(cimi);
        }

        //û�г�ʼ��MQTT
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:0"))
        {
            //���ͳ�ʼ������
            GSM_UART_SEND("AT+MQTTCFG=\"732110.cicp.net\",39522,\"ZFW0101001000007\",30,\"dxx3PYrF\",\"N<9y9Cxt\",1\r\n");
        }

        //�Ѿ���ʼ��MQTT
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:1"))
        {
            //������������
            GSM_UART_SEND("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");
        }

        //�ɹ������Ϸ�������ʼ����
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTSTAT:5"))
        {
            //��������
            GSM_UART_SEND("AT+MQTTSUB=\"Clients/ZFW010100100\",0\r\n");
            GSM_UART_SEND("AT+MQTTPING=1\r\n");
            net_connect_server_flag = 1;
        }

        //������
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"ERROR"))
        {
            //�ر�M5310Aģ��
            M5310A_DIS;
            //������������¼�Ⲣ������
            cgatt_flag = 0;
            net_connect_server_flag = 0;
        }

        //��������Ͽ�
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTDISC:OK"))
        {
            //�ر�M5310Aģ��
            M5310A_DIS;
            //������������¼�Ⲣ������
            cgatt_flag = 0;
            net_connect_server_flag = 0;
        }

        //�յ���������
        if (strstr((char *)GSM_UART_RX_BUFF, (char *)"+MQTTPUBLISH"))
        {
            //+MQTTPUBLISH:0,0,0,0,Clients/ZFW0101001000007/Sub,146,
            GetCountRegionByFmt(GSM_UART_RX_BUFF, ',', 6, mqtt_rev_message_len_tmp);

            //��ȡMQTT���ܵ��ĳ���
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
