#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   

#define BOOT_FLAG_ADDR 0x2200    //系统切换启动标志 存入0x01主分区,0x02备份分区
#define SYS_INIT_ADDR 0x2210     //系统破处标志

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
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

//以下为第二片512
u8 AT24CXX_ReadOneByte2(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte2(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte2(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte2(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write2(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read2(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check2(void);  //检查器件
void AT24CXX_Init2(void); //初始化IIC

#endif
















