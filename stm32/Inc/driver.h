#ifndef __DRIVER_H
#define __DRIVER_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "mcu.h"
#include "usart.h"
#include "flash.h"
#include "iic.h"
#include "mpu6050.h"
#include "set.h"
#include "mcu.h"
#include "mymath.h"
#include "encoder.h"
#include "motor_spta.h"

void DRV_Init1(void);
//void Stepper_Motor(int32_t i32LeftSpeed ,int32_t i32RightSpeed);

#endif /* __DRIVER_H */
/******************* (C) COPYRIGHT 2018 LinCongCong*****END OF FILE************/
