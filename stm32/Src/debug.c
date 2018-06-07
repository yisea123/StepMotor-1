/* Auth: Lin CongCong
 * QQ: 691147548
 * Email:lincongcongfzu@gmail.com
 * Date: 2018/1/1
 * Description: Solve a rubik's cube
 * Please remain this when you copy it
 */
 
 /* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "debug.h"


/* Private define ------------------------------------------------------------*/

uint8_t Hex2Char(uint8_t u8Hex)
{
	if((u8Hex > 0x00 && u8Hex <= 0x09) || (u8Hex == 0x00))
	{
		u8Hex += '0';
	}
	else
	{
		u8Hex = u8Hex - 0x0a + 'A'; 
	}
	return u8Hex;
}

void MakeDebugBuffer(void)
{
    uint16_t n,u16Temp,u16Counter = 0;
    //开头
    USART1_TxData[u16Counter]='L';
    u16Counter++;
    USART1_TxData[u16Counter]='C';
    u16Counter++;
    USART1_TxData[u16Counter]='C';
    u16Counter++;
    
    for(n=0; n<32; n++)
	{
		switch(n)
		{
			case 0:
				u16Temp = (uint16_t)(f32XAngle*100 +30000); 
				break;
			case 1:
				u16Temp = (uint16_t)(f32YAngle*100 +30000);
				break;
			case 2:
				u16Temp = (uint16_t)(f32ZAngle*100 +30000);
				break;
			case 3:
				u16Temp = (uint16_t)(i16XGyro_Last +30000);//*Gyro_Gr*100
                break;
			case 4:
				u16Temp = (uint16_t)(i16YGyro_Last +30000);//*Gyro_Gr*100
				break;
			case 5:
				u16Temp = (uint16_t)(i16ZGyro_Last +30000);//*Gyro_Gr*100
				break;
			case 6:
				u16Temp = (uint16_t)(hRot_angle+30000);
				break;
			case 7:
				u16Temp = (uint16_t)(hRot_Speed+30000);
				break;
			case 8:
				u16Temp = (uint16_t)0;
				break;
			case 9:
				u16Temp = (uint16_t)0;
				break;
			case 10:
				u16Temp = (uint16_t)0;
				break;
			case 11:
				u16Temp = (uint16_t)0;
                break;
			case 12:
				u16Temp = (uint16_t)0;
				break;
			case 13:
				u16Temp = (uint16_t)0;
				break;
			case 14:
				u16Temp = (uint16_t)0;
				break;
			case 15:
				u16Temp = (uint16_t)0;
				break;
			case 16:
				u16Temp = (uint16_t)0;
				break;
			case 17:
				u16Temp = (uint16_t)0;
				break;
			default:
				u16Temp = 0;
				break;
		}
		USART1_TxData[u16Counter+n*4]   = Hex2Char((u16Temp & 0xF000) >> 12);
		USART1_TxData[u16Counter+n*4+1] = Hex2Char((u16Temp & 0x0F00) >> 8);
		USART1_TxData[u16Counter+n*4+2] = Hex2Char((u16Temp & 0x00F0) >> 4);
		USART1_TxData[u16Counter+n*4+3] = Hex2Char((u16Temp & 0x000F));
	}
	u16Counter += 128;
    
    //结尾
    USART1_TxData[u16Counter]='F';
    u16Counter++;
    USART1_TxData[u16Counter]='Z';
    u16Counter++;
    USART1_TxData[u16Counter]='U';
    u16Counter++;
    
    //发送数据
    DMA1_Channel4_Configuration((uint32_t)USART1_TxData, u16Counter);
}
