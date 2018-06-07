#include "can.h"
#include "stdio.h"
#include <string.h>

//CAN发送数据
uint8_t CAN_SendData(CAN_TypeDef* CANx,CanTxMsg* CanData)
{
    uint16_t i=0;
    uint8_t  mailbox=0;
	mailbox=CAN_Transmit(CANx,CanData);

    //等待发送结束
	while((CAN_TransmitStatus(CAN1, mailbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
    {
        i++;	
    }
	if(i>=0XFFF)return 1;
    
	return 0;	
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

    RxMessage.StdId=0x00;
    RxMessage.ExtId=0x00;
	RxMessage.RTR=CAN_RTR_DATA;  // 数据帧或远程帧
    RxMessage.IDE=CAN_ID_STD;	 // 标准或扩展
    RxMessage.DLC=0;
    RxMessage.FMI=0;
    RxMessage.Data[0]=0x00;
    RxMessage.Data[1]=0x00;

    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);	 // CAN接收数据	
    
    //printf("RxMessage :%s",(char*)&RxMessage);
    USART1_Send_CANmsg_str(1, &RxMessage);

}


static void Char2Str(char *Datout,char *Datin,unsigned char len)
{
    unsigned char j;
    
    for(j=0;j<len;j++)
    {
        sprintf(Datout,",%02X",Datin[j]);
        Datout+=3;	
    }	
}

//向串口发生一条CAN 数据
void USART1_Send_CANmsg_str(uint8_t port, CanRxMsg *RxMsg)
{
    char Buf[60];

	memset(Buf,0x00,60); 

	sprintf(&Buf[strlen(Buf)],"CAN%d:",port);    

	if(RxMsg->IDE==CAN_ID_STD) // 标准帧
	{
	    sprintf(&Buf[strlen(Buf)],",S0x%08X",RxMsg->StdId);
		sprintf(&Buf[strlen(Buf)],",%d",RxMsg->DLC);	
	    Char2Str(&Buf[strlen(Buf)],(char*)RxMsg->Data,8);
	}
	else // 扩展帧
	{
	    sprintf(&Buf[strlen(Buf)],",E0x%08X",RxMsg->ExtId);
		sprintf(&Buf[strlen(Buf)],",%d",RxMsg->DLC);
	    Char2Str(&Buf[strlen(Buf)],(char*)RxMsg->Data,8);
	}
	
   printf("%s\r\n",Buf);
}

