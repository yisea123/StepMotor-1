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
    GPIO_TypeDef * GPIOBASE;
    int32_t PWMGPIO;
} MOTOR_CONTROL_SPTA ;

#define M1_CLOCKWISE					0	//顺时针
#define M1_UNCLOCKWISE				1		//逆时针
#define M2_CLOCKWISE					0	//顺时针
#define M2_UNCLOCKWISE				1		//逆时针
#define M3_CLOCKWISE					0	//顺时针
#define M3_UNCLOCKWISE				1		//逆时针
#define M4_CLOCKWISE					0	//顺时针
#define M4_UNCLOCKWISE				1		//逆时针

#define PWM1_PreemptionPriority 2           //电机1 中断优先等级
#define PWM1_SubPriority 0					//电机1 中断子优先级
#define PWM2_PreemptionPriority 2           //电机2	中断优先等级
#define PWM2_SubPriority 1					//电机2 中断子优先级
#define PWM3_PreemptionPriority 2           //电机3 中断优先等级
#define PWM3_SubPriority 2					//电机3 中断子优先级
#define PWM4_PreemptionPriority 2           //电机4 中断优先等级
#define PWM4_SubPriority 3					//电机4 中断子优先级

#define M1DIV               32   			//定义电机1的细分数
#define M2DIV               32   			//定义电机2的细分数
#define M3DIV               32   			//定义电机3的细分数
#define M4DIV               32   			//定义电机4的细分数

#define IDLE				0				//空闲状态
#define ACCELERATING		1               //加速状态
#define AT_MAX			    2				//最大速度状态
#define DECELERATING		3               //减速状态

#define M_FRE_START			10000			//电机的启动频率
#define M_FRE_AA			6000			//电机频率的加加速度
#define M_T_AA				2				//电机频率的加加速时间
#define M_T_UA				6				//电机频率的匀加速时间
#define M_T_RA				2				//电机频率的减加速时间 

#define F2TIME_PARA			12000000   		//将频率值转换为定时器寄存器值得转换参数
#define STEP_PARA			10	   			//任意时刻转动步数修正因子
#define STEP_AA				31     			//加加速阶段，离散化点数
#define STEP_UA				31				//匀加速阶段，离散化点数
#define STEP_RA				31				//减加速阶段，离散化点数

#define STEP_SPTA			20			    //SPTA最大速度等级
#define MAXSPEED_SPTA   	80000		    //SPTA最大速度
#define ACCSPEED_SPTA		150000		    //SPTA加速度

//#define OUTPUT_DATA   /*用于将电机运行时的定时器信息通过串口上传*/

#define MOTOR1_CW       PBout(7)
#define MOTOR1_ENABLE   PBout(8)

#define MOTOR2_CW       PBout(7)
#define MOTOR2_ENABLE   PBout(8)

#define MOTOR3_CW       PBout(7)
#define MOTOR3_ENABLE   PBout(8)

#define MOTOR4_CW       PBout(7)
#define MOTOR4_ENABLE   PBout(8)

#define MOTOR1_GPIOBASE GPIOB
#define MOTOR1_PWMGPIO  GPIO_Pin_6

#define MOTOR2_GPIOBASE GPIOB
#define MOTOR2_PWMGPIO GPIO_Pin_6

#define MOTOR3_GPIOBASE GPIOB
#define MOTOR3_PWMGPIO GPIO_Pin_6

#define MOTOR4_GPIOBASE GPIOB
#define MOTOR4_PWMGPIO GPIO_Pin_6

extern MOTOR_CONTROL_SPTA motor1;
extern MOTOR_CONTROL_SPTA motor2;
extern MOTOR_CONTROL_SPTA motor3;
extern MOTOR_CONTROL_SPTA motor4;


void Initial_Motor(unsigned char MotorID, unsigned char StepDive,unsigned int maxposition , TIM_TypeDef* TIMx);
void Start_Motor_SPTA(unsigned char MotorID,unsigned char dir,unsigned int Degree);
void TIMX_IRQHandler_SPTA(MOTOR_CONTROL_SPTA *pmotor);
    


#endif /* __MOTOR_H */
/******************* (C) COPYRIGHT 2016 LinCongCong*****END OF FILE************/
