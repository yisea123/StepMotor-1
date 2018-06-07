#include "driver.h"
#include "mpu6050.h"

uint32_t time1=0;
uint32_t time2=0;

void DRV_Init1(void)	//驱动层初始化第一步
{
    RCC_Configuration();
    MY_MCU_Init();
    MY_GPIO_Init();
    Delay_Init();
    Delay_Ms(1000);
    mpu6050_init();
    
    TIM1_Init(1,3600);
    //TIM2_Init(72-1,10000);   
    TIM3_Init(72-1,10000);
    TIM4_Init(72-1,10000);
    
   
   

    USART1_Init(115200);
    //EXTIX_Init();
    //DMA1_Channel1_Configuration((uint32_t) &AD_Value[0][0], KEY_BUFFER*KEY_COUNT);
    //ADC1_Init(); 
}



//中断
void TIM3_IRQHandler(void)
{
    if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET )
    {  
        time2=Time_Nowms()-time1; 
        time1=Time_Nowms();
        ENC_Calc_Average_Speed();
        mpu6050_read();          
        TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);          
    }
}

void TIM4_IRQHandler(void)
{    
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{				
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 

         if (hEncoder_Timer_Overflow != 65535)  
          {
                hEncoder_Timer_Overflow++;
          }
      
	}
   
}

void EXTI15_10_IRQHandler(void)
{
	//Delay_Ms(20);//消抖
	EXTI_ClearITPendingBit(EXTI_Line10);  //清除LINE4上的中断标志位  
}	

/*

void TIM1_UP_IRQHandler(void)
{ 
	TIM1->SR = (u16)~TIM_FLAG_Update;   
	TIMX_UP_IRQHandler_S(&motor1);
}

void TIM2_IRQHandler(void)
{
	TIM2->SR = (u16)~TIM_FLAG_Update;   
	TIMX_UP_IRQHandler_S(&motor2);
}
void TIM3_IRQHandler(void)
{
	TIM3->SR = (u16)~TIM_FLAG_Update;   
	TIMX_UP_IRQHandler_S(&motor3);
}

void TIM4_IRQHandler(void)
{   
	//清除中断
	TIM4->SR = (u16)~TIM_FLAG_Update;
	TIMX_IRQHandler_SPTA(&motor4);
}
 

void EXTI0_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
   {        
			EXTI_ClearITPendingBit(EXTI_Line0);
			if(motor3.rstflg==1)
			{
				motor3.speedenbale=0;
				motor3.PulsesHaven=motor3.PulsesGiven-128;	
				motor3.dir=M3_UNCLOCKWISE;
				motor3.CurrentPosition_Pulse=128;		
				motor3.rstflg=0;
			}  
  }
}

void EXTI1_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
   {        
			EXTI_ClearITPendingBit(EXTI_Line1);
			if(motor1.rstflg==1)
			{
				motor1.speedenbale=0;
				motor1.PulsesHaven=motor1.PulsesGiven-128;	
				motor1.dir=M1_UNCLOCKWISE;
				motor1.CurrentPosition_Pulse=128;		
				motor1.rstflg=0;
			}  
  }
}

void EXTI2_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line2)!= RESET)
   {        
			EXTI_ClearITPendingBit(EXTI_Line2);
			if(motor4.rstflg==1)
			{
				motor4.speedenbale=0;
				motor4.PulsesHaven=motor4.PulsesGiven-128;	
				motor4.dir=M4_UNCLOCKWISE;
				motor4.CurrentPosition_Pulse=128;		
				motor4.rstflg=0;
			}  
  }
}

void EXTI4_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line4)!= RESET)
   {        
			EXTI_ClearITPendingBit(EXTI_Line4);
			if(motor2.rstflg==1)
			{
				motor2.speedenbale=0;
				motor2.PulsesHaven=motor2.PulsesGiven-128;	
				motor2.dir=M2_UNCLOCKWISE;
				motor2.CurrentPosition_Pulse=128;		
				motor2.rstflg=0;
			}  
  }
}
*/