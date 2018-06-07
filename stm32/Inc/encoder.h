#ifndef __ENCODER_H
#define __ENCODER_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "driver.h"

#define ENCODER_PPR 96
extern volatile uint16_t hEncoder_Timer_Overflow;
extern int16_t hRot_Speed,hRot_angle;

void ENC_Get_Electrical_Angle(void);
int16_t ENC_Calc_Rot_Speed(void);
void ENC_Calc_Average_Speed(void);

#endif /* __ENCODER_H */

/******************* (C) COPYRIGHT 2018 LinCongCong*****END OF FILE************/
