#ifndef __SET_H__
#define __SET_H__
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "driver.h"
#pragma pack(1)
struct settingParamDef
{
    uint16_t u16Inited;				   	//flash≥ı º±Í÷æ
    int16_t  i16X_ACCOffset;  
    int16_t  i16Y_ACCOffset;
    int16_t  i16Z_ACCOffset;
    int16_t  i16X_GYROOffset;  
    int16_t  i16Y_GYROffset;
    int16_t  i16Z_GYROffset;   
};
typedef struct settingParamDef	globalSettingParamDef;

#define SETTING_ADDRESS_BASE    0x0800F000		//60K

extern globalSettingParamDef   globalSettingParam;
extern uint8_t u8MatlabDebugMode;

/* Private define ------------------------------------------------------------*/
void LCC_Cmd_Process(uint8_t *RecvBuffer ,uint16_t RecvLen);
char* memistr(const char* mem, uint16_t maxLen, const char* str);
void StrToUpper_n(char* str, uint16_t len);
int32_t str2Digit(uint8_t* src);

#endif
