#ifndef __MOTOR_H
#define __MOTOR_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "driver.h"

/* Private define ------------------------------------------------------------*/
typedef __packed struct 
{
    unsigned char en;	          		    //使能
    unsigned char dir;			  			//方向
    unsigned char running;		  		    //转动完成标志
    unsigned char rstflg;		  			//复位标志，为1时，限位开关强停。
    unsigned char divnum;		  			//分频数	 
    unsigned char speedenbale;		        //是否使能速度控制	
    unsigned char clockwise;			    //顺时针方向对应的值
    unsigned char id;						//电机id	

    uint32_t PulsesGiven;			  		//电机运动的总步数
    uint32_t PulsesHaven;					//电机已经运行的步数
    uint32_t step_move;				        //total move requested
    uint32_t step_spmax;				    //maximum speed
    uint32_t step_accel;				    //accel/decel rate, 8.8 bit format
    uint32_t step_acced;				    //steps in acceled stage

    uint32_t step_middle;				    //mid-point of move, = (step_move - 1) >> 1
    uint32_t step_count;				    //step counter
    uint32_t step_frac;				        //step counter fraction
    uint32_t step_speed;				    //current speed, 16.8 bit format (HI byte always 0)
    uint32_t speed_frac;				    //speed counter fraction
    uint8_t step_state;					    //move profile state
    uint8_t step_dur;					    //counter for duration of step pulse HI

    uint32_t CurrentPosition;			    //当前位置
    uint32_t MaxPosition;					//最大位置，超过该位置置0
    uint32_t CurrentPosition_Pulse;		    //当前位置
    uint32_t MaxPosition_Pulse;				//当前位置

    TIM_TypeDef* TIMx;	
    GPIO_TypeDef* PWM_PORT;
	GPIO_TypeDef* DIR_PORT;
	GPIO_TypeDef* EN_PORT;
	
    int16_t PWM_pin;
	int16_t DIR_pin;
	int16_t EN_pin;
} MOTOR_CONTROL_SPTA ;

#define M1_CLOCKWISE					0	//顺时针
#define M1_UNCLOCKWISE				1		//逆时针
#define M2_CLOCKWISE					0	//顺时针
#define M2_UNCLOCKWISE				1		//逆时针
#define M3_CLOCKWISE					0	//顺时针
#define M3_UNCLOCKWISE				1		//逆时针
#define M4_CLOCKWISE					0	//顺时针
#define M4_UNCLOCKWISE				1		//逆时针

#define M1DIV               16   			//定义电机1的细分数
#define M2DIV               16   			//定义电机2的细分数
#define M3DIV               16   			//定义电机3的细分数
#define M4DIV               16   			//定义电机4的细分数

#define IDLE				0				//空闲状态
#define ACCELERATING		1               //加速状态
#define AT_MAX			    2				//最大速度状态
#define DECELERATING		3               //减速状态

#define STEP_SPTA			20			    //SPTA最大速度等级
#define MAXSPEED_SPTA   	8000		    //SPTA最大速度
#define ACCSPEED_SPTA		15000		    //SPTA加速度

//#define OUTPUT_DATA   /*用于将电机运行时的定时器信息通过串口上传*/
#define MOTOR1_PWMPORT GPIOA
#define MOTOR1_PWMPIN  GPIO_Pin_1

#define MOTOR1_DIRPORT GPIOA
#define MOTOR1_DIRPIN  GPIO_Pin_2

#define MOTOR1_ENPORT GPIOA
#define MOTOR1_ENPIN  GPIO_Pin_0

#define MOTOR2_PWMPORT GPIOB
#define MOTOR2_PWMPIN  GPIO_Pin_6

#define MOTOR2_DIRPORT GPIOB
#define MOTOR2_DIRPIN  GPIO_Pin_7

#define MOTOR2_ENPORT GPIOB
#define MOTOR2_ENPIN  GPIO_Pin_8

#define MOTOR3_PWMPORT GPIOB
#define MOTOR3_PWMPIN  GPIO_Pin_6

#define MOTOR3_DIRPORT GPIOB
#define MOTOR3_DIRPIN  GPIO_Pin_7

#define MOTOR3_ENPORT GPIOB
#define MOTOR3_ENPIN  GPIO_Pin_8

#define MOTOR4_PWMPORT GPIOB
#define MOTOR4_PWMPIN  GPIO_Pin_6

#define MOTOR4_DIRPORT GPIOB
#define MOTOR4_DIRPIN  GPIO_Pin_7

#define MOTOR4_ENPORT GPIOB
#define MOTOR4_ENPIN  GPIO_Pin_8


extern MOTOR_CONTROL_SPTA StepMotor[4];



void Initial_Motor(unsigned char MotorID, unsigned char StepDive,unsigned int maxposition , TIM_TypeDef* TIMx);
void Start_Motor_SPTA(unsigned char MotorID,unsigned char dir,unsigned int Degree);
void SetSpeed(unsigned char MotorID,signed char speedindex);
void TIMX_IRQHandler_SPTA(MOTOR_CONTROL_SPTA *pmotor);
    


#endif /* __MOTOR_H */
/******************* (C) COPYRIGHT 2016 LinCongCong*****END OF FILE************/
