#include "iic.h"
#include "mcu.h"

//��ʼ��I2C��Ӧ�Ľӿ����š�
void IIC_Init(void)
{			
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			     
// 	//����PB10 PB11 Ϊ��©���  ˢ��Ƶ��Ϊ10Mhz
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	//����PB6 PB7 Ϊ��©���  ˢ��Ƶ��Ϊ10Mhz
//	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda����� 
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_Us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_Us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�	  
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_Us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	Delay_Us(4);							   	
}

//�ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
char IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
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
	
//����NACKӦ��	    
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

//IIC����һ���ֽ�	  
void IIC_Send_Byte(char txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
   
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  
char IIC_Read_Byte(char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK  
    return receive;
}

u8 IIC_ReadByte(u8 dev, u8 reg)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(dev);	   //����д����
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(reg); res++;  //���͵�ַ
	IIC_Wait_Ack();	  

	IIC_Start();
	IIC_Send_Byte(dev+1); res++;          //�������ģʽ			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
    IIC_Stop();//����һ��ֹͣ����

	return res;
}

u8 IIC_ReadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;
	u8 temp;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  //�������ģʽ	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++)
	{
		 if(count!=(length-1))
		 {
		 	temp = IIC_Read_Byte(1);  	//��ACK�Ķ�����
		 }
		 else  
		 {
			temp = IIC_Read_Byte(0); 	//���һ���ֽ�NACK
		 }
		data[count] = temp;
	}
    IIC_Stop();//����һ��ֹͣ����
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
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();//����һ��ֹͣ����
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
