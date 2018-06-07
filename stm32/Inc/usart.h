#ifndef __USART_H
#define __USART_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define USART_RECV_SIZE	512
#define USART_SEND_SIZE	512
extern uint8_t USART1_RxOK;
extern uint8_t USART1_TxData[USART_SEND_SIZE];
extern uint8_t USART1_RxData[USART_RECV_SIZE];
extern uint8_t USART1_RxBuffer[USART_RECV_SIZE];

void USART1_Printf(char* fmt,...);
    
#endif /* __USART_H */
/******************* (C) COPYRIGHT 2016 LinCongCong*****END OF FILE************/

