/* Auth: Lin CongCong
 * QQ: 691147548
 * Email:lincongcongfzu@gmail.com
 * Date: 2018/1/1
 * Description: Solve a rubik's cube
 * Please remain this when you copy it
 */
 
#include "set.h"

const globalSettingParamDef	globalSettingParamDefault = 
{
    0xA0A0, //u16Inited
    1,      //i16X_ACCOffset
    2,      //i16Y_ACCOffset
    3,      //i16Z_ACCOffset
    4,      //i16X_GYROOffset
    5,      //i16Y_GYROOffset
    6,      //i16Z_GYROOffset
};

globalSettingParamDef   globalSettingParam;

uint8_t u8MatlabDebugMode = 0;

void Write_All_Param(void)
{
    uint16_t u16Offset = (uint32_t)&globalSettingParam.u16Inited - (uint32_t)&globalSettingParam;
    uint16_t u16Length = sizeof(globalSettingParamDef) - u16Offset;
    STMFLASH_Write(SETTING_ADDRESS_BASE+u16Offset, (uint16_t*)&globalSettingParam.u16Inited, u16Length / 2);
    
    //STMFLASH_Write(SETTING_ADDRESS_BASE, (uint16_t*)&globalSettingParamDefault, sizeof(globalSettingParamDefault)/2);
    //STMFLASH_Read (SETTING_ADDRESS_BASE, (uint16_t*)&globalSettingParam, sizeof(globalSettingParam)/2);
    
    //USART1_Printf("%d,%d,%d",globalSettingParam.i16X_ACCOffset,globalSettingParam.i16Y_ACCOffset,globalSettingParam.i16Z_ACCOffset);
    
}


//find 'substr' from a fixed-length buffer   
//('full_data' will be treated as binary data buffer)  
//return NULL if not found  
char* mymemstr(char* full_data, int full_data_len, char* substr)  
{  
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) {  
        return NULL;  
    }  
  
    if (*substr == '\0') {  
        return NULL;  
    }  
  
    int sublen = strlen(substr);  
  
    int i;  
    char* cur = full_data;  
    int last_possible = full_data_len - sublen + 1;  
    for (i = 0; i < last_possible; i++) {  
        if (*cur == *substr) {  
            //assert(full_data_len - i >= sublen);  
            if (memcmp(cur, substr, sublen) == 0) {  
                //found  
                return cur;  
            }  
        }  
        cur++;  
    }  
  
    return NULL;  
}



/************LCC 命令格式**************

1、上位机查找设备值 ：  LCC+CZSB=STM32
            返回内容 ： +CZSB=OK!        任务完成 
                        +CZSB=ERROR!     任务失败

2、打开或关闭数据采集： LCC+SJCJ=OPEN CLOSE
            返回内容 ： +SJCJ=OPEN!      开始数据采集
                        +SJCJ=CLOSE!     结束数据采集

2、控制电机转速命令 ：  LCC+MOTOR=X,XXX   //左轮右轮选择,动作速度
           返回内容 ：  +MOTOR=OK!        任务完成 
                        +MOTOR=ERROR!     任务失败 

											
*****************************************/
void LCC_Cmd_Process(uint8_t *RecvBuffer ,uint16_t RecvLen) 
{
    uint8_t* pos = NULL;
    uint8_t offset = 0;
    uint16_t u16temp;
    if(RecvLen<2)
    {
        return;
    }
    RecvBuffer[RecvLen] = '\0' ;
    
    offset = 0;
    pos = (uint8_t*)mymemstr((char*)RecvBuffer, RecvLen-1, "LCC+");
    if(pos != NULL)
	{
        offset += 4;
        if(strncmp((char*)pos + offset, "CZSB=", 5) == 0)  //查找设备
        {
            USART1_Printf("+CZSB=OK!\r\n");
        }
        else if(strncmp((char*)pos + offset, "SJCJ=", 5) == 0)  //数据采集
		{
            offset += 5;
            if(strncmp((char*)pos + offset, "OPEN", 4) == 0) 
            {
                u8MatlabDebugMode=1;
                USART1_Printf("+SJCJ=OPEN!\r\n");
            }
            else if(strncmp((char*)pos + offset, "CLOSE", 5) == 0) 
            {
                u8MatlabDebugMode=0;
                USART1_Printf("+SJCJ=CLOSE!\r\n");
            }   
        }   
        else if(strncmp((char*)pos + offset, "MOTOR=", 6) == 0) 
        {
            //USART1_Printf("+MOTOR=OK!\r\n");
            offset += 6;
            if(strncmp((char*)pos + offset, "1", 1) == 0) 
            {
                offset += 2;
                u16temp=str2Digit((uint8_t*)pos + offset);
                if(u16temp>3600) u16temp=3600;
                USART1_Printf("+MOTOR=OK!speed:%d\r\n",u16temp);
                TIM_SetCompare1(TIM1,(uint32_t)u16temp);
            }
            else if(strncmp((char*)pos + offset, "2", 1) == 0) 
            {
                offset += 2;
                //speed=str2Digit((uint8_t*)pos + offset);
            }
            
        }
       
        else if(strncmp((char*)pos + offset, "JDWZ=", 5) == 0)  //极大位置
		{
            offset += 5;
            
        }  
        else
        {
            USART1_Printf("Error!!\r\n");
        }
    }
    
}
 
/**************************************************************************************
功能：从内存串中查找目标串(ASCII 字符串), 内存串中带0x00 也可正常搜到
输入参数：
		源内存串地址:		mem
		查找范围长度:		maxLen
		查找的串:	str
输出参数：
		要查找字串的位置: pos
返回值：
		目标字串的位置，查找不到返回NULL
**************************************************************************************/
char* memistr(const char* mem, uint16_t maxLen, const char* str)
{
	uint16_t i = 0;
	uint16_t j = 0;

	if((mem == NULL) || (str == NULL) || (maxLen <= 0))
	{
		return NULL;
	}
	
	for(i = 0; i < maxLen - strlen(str) + 1; i++)
	{
		for(j = 0; j < strlen(str); j++)
		{
			if(toupper(mem[i + j]) != toupper(str[j]))
			{
				break;
			}
		}
		if(j == strlen(str))
		{
			return (char*)(mem + i);
		}
	}

	return NULL;
}

void StrToUpper_n(char* str, uint16_t len)
{
	uint16_t i = 0;
	if(str == NULL)
	{
		return;
	}
	while(i < len)
	{
		str[i] = toupper(str[i]);
		i++;
	}
}

int32_t str2Digit(uint8_t* src)
{
	uint8_t i = 0;
	uint8_t negFlag = 0;
	int32_t val = 0;
	if(src == NULL)
	{
		return 0;
	}
	if(src[0] == '-')
	{
		negFlag = 1;
		i = 1;
	}
	for(; i < 10; i++)
	{
		if((src[i] >= 0x30) && (src[i] <= 0x39))
		{
			val *= 10;
			val += (src[i] - 0x30);
		}
		else
		{
			break;
		}
	}
	if(negFlag == 1)
	{
		val = -val;
	}
	return val;
}

