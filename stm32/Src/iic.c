#include "iic.h"
#include "mcu.h"

//初始化I2C对应的接口引脚。
void IIC_Init(void)
{			
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			     
// 	//配置PB10 PB11 为开漏输出  刷新频率为10Mhz
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	//配置PB6 PB7 为开漏输出  刷新频率为10Mhz
//	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出 
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_Us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_Us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号	  
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_Us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	Delay_Us(4);							   	
}

//等待应答信号到来 
//返回值：1，接收应答失败
//        0，接收应答成功
char IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;Delay_Us(1);	   
	IIC_SCL=1;Delay_Us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
	  Delay_Us(1);
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_Us(1);
	IIC_SCL=1;
	Delay_Us(1);
	IIC_SCL=0;
}
	
//产生NACK应答	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_Us(1);
	IIC_SCL=1;
	Delay_Us(1);
	IIC_SCL=0;
}					 				     

//IIC发送一个字节	  
void IIC_Send_Byte(char txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_Us(1);   
		IIC_SCL=1;
		Delay_Us(1); 
		IIC_SCL=0;	
		Delay_Us(1);
    }	 
} 	 
   
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
char IIC_Read_Byte(char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay_Us(1);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_Us(1); 
    }					 
    if (ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck();//发送nACK  
    return receive;
}

u8 IIC_ReadByte(u8 dev, u8 reg)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(dev);	   //发送写命令
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(reg); res++;  //发送地址
	IIC_Wait_Ack();	  

	IIC_Start();
	IIC_Send_Byte(dev+1); res++;          //进入接收模式			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
    IIC_Stop();//产生一个停止条件

	return res;
}

u8 IIC_ReadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;
	u8 temp;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //发送地址
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  //进入接收模式	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++)
	{
		 if(count!=(length-1))
		 {
		 	temp = IIC_Read_Byte(1);  	//带ACK的读数据
		 }
		 else  
		 {
			temp = IIC_Read_Byte(0); 	//最后一个字节NACK
		 }
		data[count] = temp;
	}
    IIC_Stop();//产生一个停止条件
    return count;
}

u8 IIC_WriteByte(u8 dev, u8 reg, u8 data)
{
	return IIC_WriteBytes(dev, reg, 1, &data);
}
u8 IIC_WriteBytes(u8 dev, u8 reg, u8 length, u8* data)
{
	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //发送地址
    IIC_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();//产生一个停止条件
    return 1; //status == 0;
}

u8 IIC_WriteBit(u8 dev,u8 reg,u8 bitNum,u8 data)
{
	u8 b;
    b=IIC_ReadByte(dev, reg);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    
    return IIC_WriteByte(dev, reg, b);
}

u8 IIC_WriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	u8 b;
	b=IIC_ReadByte(dev, reg);
    if (b != 0) 
	{
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IIC_WriteByte(dev, reg, b);
    } 
	else 
	{
        return 0;
    }
}

//------------------End of File----------------------------
