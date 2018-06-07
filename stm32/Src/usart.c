/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "mcu.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
uint8_t USART1_RxOK=0;
uint8_t USART1_TxData[USART_SEND_SIZE];
uint8_t USART1_RxData[USART_RECV_SIZE];
uint8_t USART1_RxBuffer[USART_RECV_SIZE];

/*
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	 
    return ch;
}
int GetKey (void)  { 
    while (!(USART1->SR & USART_FLAG_RXNE));
    return ((int)(USART1->DR & 0x1FF));
}
*/


//串口1中断 ， 接收发送中断
void USART1_IRQHandler(void)
{
	uint16_t freeSpace = 0; 
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//    }
//	else 
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		//USART_ClearITPendingBit(USART1,USART_IT_IDLE);
        uint8_t Free_Read_Rst = 0;//读DR清除空闲中断
        Free_Read_Rst = USART1->DR; //清USART_IT_IDLE标志
		//关闭DMA通道
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC5);

		freeSpace = DMA_GetCurrDataCounter(DMA1_Channel5);
		if(freeSpace < USART_RECV_SIZE)
		{
            if(USART1_RxOK==0)
            {
                memset(USART1_RxData, 0, USART_RECV_SIZE);
                memcpy(USART1_RxData, USART1_RxBuffer , USART_RECV_SIZE-freeSpace);
                USART1_RxOK=1;
            }
			//接收长度清零
			DMA_SetCurrDataCounter(DMA1_Channel5, USART_RECV_SIZE);
			//重新制定目标地址
			DMA_SetMemoryBaseAddr(DMA1_Channel5, (uint32_t)USART1_RxBuffer);
		}
		//重新启动DMA通道
		DMA_Cmd(DMA1_Channel5, ENABLE);
        
	}
}


void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
	{	  
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		//Usart1_Task_Data_Out();
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5) != RESET)
	{	  
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}

//串口,printf 函数
void USART1_Printf(char* fmt,...) 
{  
	uint32_t i; 
	static uint8_t buffer[128];
    memset(buffer,0x00,128); 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char *)buffer,fmt,ap);
	va_end(ap);
	i=strlen((char *)buffer);		//此次发送数据的长度
		  
    DMA1_Channel4_Configuration((uint32_t)buffer, i);
}
