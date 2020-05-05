#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   

#define BOOT_FLAG_ADDR 0x2200    //ϵͳ�л�������־ ����0x01������,0x02���ݷ���
#define SYS_INIT_ADDR 0x2210     //ϵͳ�ƴ���־

#define AT24C01		127      //7
#define AT24C02		255     //8      byte
#define AT24C04		511     //9
#define AT24C08		1023   //10
#define AT24C16		2047   //11
#define AT24C32		4095   //12
#define AT24C64	    8191   //13
#define AT24C128	16383   //14
#define AT24C256	32767   //15
#define AT24C512	65535   //16

#define EE_TYPE AT24C64
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC

//����Ϊ�ڶ�Ƭ512
u8 AT24CXX_ReadOneByte2(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte2(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte2(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte2(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write2(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read2(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check2(void);  //�������
void AT24CXX_Init2(void); //��ʼ��IIC

#endif
















