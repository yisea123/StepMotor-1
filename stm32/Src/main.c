/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "driver.h"


/* Private define ------------------------------------------------------------*/
uint8_t status=0;
float speed=0;
int16_t pwm=0;
int16_t i16XXgyro=0,pre_i16Xgyro;
float II=0;
int main(void)
{	 
    DRV_Init1();
    USART1_Printf("hello world !\n" );
    //Stepper_Motor(50,50);
	while(1)
	{	
        
        //指示灯闪缩
        Loop_Begin(500)
		{
			 LED1=!LED1; 
		}Loop_End()
        
        Loop_Begin(100)
		{
            if(USART1_RxOK)
            {
                //USART1_Printf("%s\n",USART1_RxBuffer );
                LCC_Cmd_Process(USART1_RxData,strlen((char*)USART1_RxData));
                USART1_RxOK=0;
            }    
		}Loop_End()
        
        Loop_Begin(50)
		{ 
            if(u8MatlabDebugMode)
            {
                MakeDebugBuffer();
            } 
           //USART1_Printf("X:%.2f,Y:%.2f,Z:%.2f\r\n",f32XAngle ,f32YAngle ,f32ZAngle);
		}Loop_End()
        
        Loop_Begin(10)
		{
            //i16XXgyro = i16XGyro_Last-pre_i16Xgyro;
            //pre_i16Xgyro = i16XGyro_Last;
            II=II+f32YAngle;
            speed=(float)f32YAngle*550 +(float)i16XGyro_Last*12;
            pwm=(int16_t)(1800.0f+speed);
                
                
            
            //ENC_Get_Electrical_Angle();
            // USART1_Printf("motor angle = %d ,speed = %d \r\n",hRot_angle,hRot_Speed);
            //Stepper_Motor((int32_t)f32YAngle*10,(int32_t)f32YAngle*10);
            if(f32YAngle>12 || f32YAngle<-12)
            {
                II=0;
                TIM_SetCompare1(TIM1,1800);
            }else
            {
                if(pwm<10)pwm=10;
                else if(pwm>3590) pwm=3590;
                TIM_SetCompare1(TIM1,(uint32_t)pwm);
            }
		}Loop_End()
              
        
    }
}


	




