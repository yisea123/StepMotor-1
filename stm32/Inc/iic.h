#ifndef __IIC_H
#define __IIC_H
#include "stm32f10x.h"

   	   		   
//驱动接口，GPIO模拟IIC

//SCL-->PB10
//SDA-->PB11
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
//IO操作函数	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //输入SDA 

////SCL-->PB6
////SDA-->PB7
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}
////IO操作函数	 
//#define IIC_SCL    PBout(6) 		//SCL
//#define IIC_SDA    PBout(7) 		//SDA	 
//#define READ_SDA   PBin(7) 	    //输入SDA 


//IIC所有操作函数
void IIC_Init(void);          		//初始化IIC的IO口	

void IIC_Start(void);			   	//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(char txd);		//IIC发送一个字节
char IIC_Read_Byte(char ack);		//IIC读取一个字节
char IIC_Wait_Ack(void); 			//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


uint8_t IIC_ReadByte(uint8_t dev, uint8_t reg);
uint8_t IIC_ReadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

uint8_t IIC_WriteByte(uint8_t dev, uint8_t reg, uint8_t data);
uint8_t IIC_WriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);

uint8_t IIC_WriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t IIC_WriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);

#endif

//------------------End of File----------------------------







