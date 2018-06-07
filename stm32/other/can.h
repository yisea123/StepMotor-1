#ifndef __CAN_H
#define __CAN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usart.h"

/* Private define ------------------------------------------------------------*/
uint8_t CAN_SendData(CAN_TypeDef* CANx,CanTxMsg* CanData);
void USART1_Send_CANmsg_str(uint8_t port, CanRxMsg *RxMsg);
#endif /* __CAN_H */
/******************* (C) COPYRIGHT 2016 LinCongCong*****END OF FILE************/
