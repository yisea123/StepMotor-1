/* Includes ------------------------------------------------------------------*/
#include "encoder.h"

/* Private define ------------------------------------------------------------*/
//正交编码测速相关变量
uint8_t u8Is_First_Measurement=1;
volatile uint16_t hEncoder_Timer_Overflow=0;
int16_t hPrevious_angle=0, hRot_Speed=0,hRot_angle=0;

//计算角度
void ENC_Get_Electrical_Angle(void)
{
  //int32_t temp;
  //temp = (int32_t)(TIM_GetCounter(TIM4)) * (int32_t)((u32)4294967295uL / (4*ENCODER_PPR)); //65536*65536-1
  //hRot_angle=((int16_t)(temp/65536)); // int16_t result
  hRot_angle= TIM_GetCounter(TIM4) ;
}


//计算速度
int16_t ENC_Calc_Rot_Speed(void)
{   
    int32_t wDelta_angle;
    uint16_t hEnc_Timer_Overflow_sample_one=0, hEnc_Timer_Overflow_sample_two=0;
    uint16_t hCurrent_angle_sample_one=0, hCurrent_angle_sample_two=0;
    signed long long temp;
    int16_t haux;

    if (!u8Is_First_Measurement)
    {
        // 1st reading of overflow counter    
        hEnc_Timer_Overflow_sample_one = hEncoder_Timer_Overflow; 
        // 1st reading of encoder timer counter
        hCurrent_angle_sample_one = TIM4->CNT;
        // 2nd reading of overflow counter
        hEnc_Timer_Overflow_sample_two = hEncoder_Timer_Overflow;  
        // 2nd reading of encoder timer counter
        hCurrent_angle_sample_two = TIM4->CNT;      

        // Reset hEncoder_Timer_Overflow and read the counter value for the next
        // measurement
        hEncoder_Timer_Overflow = 0;
        haux = TIM4->CNT;   
        
        if (hEncoder_Timer_Overflow != 0) 
        {
          haux = TIM4->CNT; 
          hEncoder_Timer_Overflow = 0;            
        }
         
        if (hEnc_Timer_Overflow_sample_one != hEnc_Timer_Overflow_sample_two)
        { //Compare sample 1 & 2 and check if an overflow has been generated right 
          //after the reading of encoder timer. If yes, copy sample 2 result in 
          //sample 1 for next process 
          hCurrent_angle_sample_one = hCurrent_angle_sample_two;
          hEnc_Timer_Overflow_sample_one = hEnc_Timer_Overflow_sample_two;
        }

        if ( (TIM4->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  //如果是向下计数
        {
            // encoder timer down-counting
            wDelta_angle = (int32_t)(hCurrent_angle_sample_one - hPrevious_angle - (hEnc_Timer_Overflow_sample_one) * (4*ENCODER_PPR));
        }
        else  //向上计数
        {
            //encoder timer up-counting
            wDelta_angle = (int32_t)(hCurrent_angle_sample_one - hPrevious_angle + (hEnc_Timer_Overflow_sample_one) * (4*ENCODER_PPR));
        }

        // speed computation as delta angle * 1/(speed sempling time)
        temp = (signed long long)(wDelta_angle * 1);
        //temp *= 10;  // 0.1 Hz resolution
        //temp /= (4*ENCODER_PPR);
        
    } //is first measurement, discard it
    else    //如果第一次
    {
        u8Is_First_Measurement = 0;
        temp = 0;
        hEncoder_Timer_Overflow = 0;
        haux = TIM4->CNT;       
        // Check if Encoder_Timer_Overflow is still zero. In case an overflow IT 
        // occured it resets overflow counter and wPWM_Counter_Angular_Velocity
        if (hEncoder_Timer_Overflow != 0) 
        {
            haux = TIM4->CNT; 
            hEncoder_Timer_Overflow = 0;            
        }
    }
    hPrevious_angle = haux;  

    return((int16_t) temp);
}
   
//计算平均速度
void ENC_Calc_Average_Speed(void)
{   
    int32_t wtemp=0;
    uint8_t i=0;
    static uint8_t count=0;
    static int32_t hSpeed_Buffer[8]={0,0,0,0,0,0,0,0};

    wtemp = ENC_Calc_Rot_Speed(); //获取速度
    //hRot_Speed=wtemp;
   
    /* Compute the average of the read speeds */  
    hSpeed_Buffer[count] = (int16_t)wtemp;
    count++;

    if (count == 8) count = 0;
    wtemp=0;

    for (i=0;i<8;i++)
    {
        wtemp += hSpeed_Buffer[i];
    }
    wtemp /= 8;
    hRot_Speed = ((int16_t)(wtemp));
    
}

