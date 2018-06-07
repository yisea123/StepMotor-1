#include "motor_spta.h"
/*see 
http://picprog.strongedge.net/step_prof/step-profile.html
*/
#ifdef OUTPUT_DATA
char tmp[64];
float timecnt=0;
float periodcnt=0;
#endif
MOTOR_CONTROL_SPTA motor1;
MOTOR_CONTROL_SPTA motor2;
MOTOR_CONTROL_SPTA motor3;
MOTOR_CONTROL_SPTA motor4;



/**************************************************************************************
初始化电机控制的GPIO口:
	一部分是普通的GPIO;
	一部分是用于PWM输出的GPIO口;
	电机1：PA8(pwm),PE9(CW),PE8(ENABLE),MXX:PA11,PA12,PE7
	电机2：PA0(pwm),PA1(CW),PC3(ENABLE),MXX:PC0,PC1,PC2
	电机3：PA6(pwm),PA7(CW),PC4(ENABLE),MXX:PA3,PA4,PA5
	电机4：PB6(pwm,gpio),PB9(CW),PB8(ENABLE),MXX:PD7,PB5,PB7
 **************************************************************************************/
void Initial_MotorIO(void)
{
	/*
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12;		  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;		  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;		  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);


	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_6;		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	*/
}
	
/*电机1的PWM输出初始化，使用的是定时器1*/
void Initial_PWM_Motor1(void)
{
	/*
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_DeInit(TIM1);
	 //中断NVIC设置：允许中断，设置优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =PWM1_PreemptionPriority;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWM1_SubPriority;          
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
	NVIC_Init(&NVIC_InitStructure);
	TIM_BaseInitStructure.TIM_Period =1000;
#ifdef OUTPUT_DATA
	TIM_BaseInitStructure.TIM_Prescaler =710000; //71000
#else
	TIM_BaseInitStructure.TIM_Prescaler =5; //12mhz
#endif
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;       //PWM2模式
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;  //信号输出到对应的输出引脚
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补信号输出到对应的输出引脚
	TIM_OCInitStructure.TIM_Pulse =50;   //脉冲宽度
	TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;   //互补输出高电平有效
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;    //互补输出高电平有效
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;  //输出空闲状态为1
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   //互补输出空闲状态为0
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);   //OC1通道初始化

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	//清中断，以免一启用中断后立即产生中断
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	//使能TIM1中断源
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1, DISABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE); //使能PWM输出
	*/
}
//设置各个输出的PWM参数
/*电机1的PWM输出初始化，使用的是定时器2*/
void Initial_PWM_Motor2(void)
{
	/*
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
     //中断NVIC设置：允许中断，设置优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;    //更新事件 	TIM2_IRQHandler
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =PWM2_PreemptionPriority;   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWM2_SubPriority;          //响应优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断
	NVIC_Init(&NVIC_InitStructure);
	TIM_BaseInitStructure.TIM_Period =1000;
#ifdef OUTPUT_DATA
	TIM_BaseInitStructure.TIM_Prescaler =710000; //71000
#else
	TIM_BaseInitStructure.TIM_Prescaler =5; 
#endif 
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;       //PWM2模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;  //信号输出到对应的输出引脚 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补信号输出到对应的输出引脚                  
    TIM_OCInitStructure.TIM_Pulse =50;   //脉冲宽度 
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;   //互补输出高电平有效 
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;    //互补输出高电平有效      
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;  //输出空闲状态为1 
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   //互补输出空闲状态为0   
    TIM_OC1Init(TIM2,&TIM_OCInitStructure);   //OC1通道初始化
	
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
	//清中断，以免一启用中断后立即产生中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    //使能TIM1中断源
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM2, DISABLE);
    //TIM_CtrlPWMOutputs(TIM2,ENABLE); //使能PWM输出
	*/
}

void Initial_PWM_Motor3(void)
{
	/*
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM3);
     //中断NVIC设置：允许中断，设置优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;    //更新事件 	TIM3_IRQHandler
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =PWM3_PreemptionPriority;   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWM3_SubPriority;          //响应优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断
	NVIC_Init(&NVIC_InitStructure);
	TIM_BaseInitStructure.TIM_Period =1000;
#ifdef OUTPUT_DATA
	TIM_BaseInitStructure.TIM_Prescaler =710000; //71000
#else
	TIM_BaseInitStructure.TIM_Prescaler =5; 
#endif 
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;       //PWM2模式 
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;  //信号输出到对应的输出引脚 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补信号输出到对应的输出引脚                  
	TIM_OCInitStructure.TIM_Pulse =50;   //脉冲宽度 
	TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;   //互补输出高电平有效 
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;    //互补输出高电平有效      
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;  //输出空闲状态为1 
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   //互补输出空闲状态为0   
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);   //OC1通道初始化

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	//清中断，以免一启用中断后立即产生中断
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//使能TIM1中断源
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	TIM_Cmd(TIM3, DISABLE);
	//TIM_CtrlPWMOutputs(TIM3,ENABLE); //使能PWM输出
	*/
}

void Initial_PWM_Motor4(void)
{
	/*
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		//中断NVIC设置：允许中断，设置优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;                 //更新事件
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =PWM4_PreemptionPriority;        //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =PWM4_SubPriority;              //响应优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //允许中断
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 29;                        //自动装载计数器周期值
#ifdef OUTPUT_DATA
	TIM_TimeBaseStructure.TIM_Prescaler = 2800; //2800                    //预分频值	 29
#else
	TIM_TimeBaseStructure.TIM_Prescaler = 14; //2800                    //预分频值	 29
#endif
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //时钟分隔
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //初始化
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE);
	*/
}
/*外部中断初始化，电机复位时使用*/
void EXTI_Configuration(void)
{
	/*
	EXTI_InitTypeDef EXTI_InitStructure;        //EXTI初始化结构定义
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);	 //E0-E5  时针使能

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;//选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	EXTI_ClearITPendingBit(EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line4);//清除中断标志
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//事件选择
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发模式
	EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line4; //线路选择 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//启动中断
	EXTI_Init(&EXTI_InitStructure);//初始化		
	*/
  
}


 /**************************************************************************************
 初始化电机的参数，主要是细分选择，使用的定时器，顺时针方向值，电机ID等
 **************************************************************************************/
void Initial_Motor(unsigned char MotorID, unsigned char StepDive,unsigned int maxposition , TIM_TypeDef* TIMx)
{
	MOTOR_CONTROL_SPTA *pmotor_spta=0; 
	switch(MotorID)
    {
	case 1:
		pmotor_spta=&motor1;
		pmotor_spta->clockwise=M1_CLOCKWISE;	//设置正反转
		pmotor_spta->GPIOBASE=MOTOR1_GPIOBASE;
		pmotor_spta->PWMGPIO=MOTOR1_PWMGPIO;
		break;
	case 2:
		pmotor_spta=&motor2;
		pmotor_spta->clockwise=M2_CLOCKWISE;	//设置正反转
		pmotor_spta->GPIOBASE=MOTOR2_GPIOBASE;
		pmotor_spta->PWMGPIO=MOTOR3_PWMGPIO;
	
		break;
	case 3:
		pmotor_spta=&motor3;
		pmotor_spta->clockwise=M3_CLOCKWISE;	//设置正反转
		pmotor_spta->GPIOBASE=MOTOR3_GPIOBASE;
		pmotor_spta->PWMGPIO=MOTOR3_PWMGPIO;
	case 4:
		pmotor_spta=&motor4;
		pmotor_spta->clockwise=M4_CLOCKWISE;	//设置正反转
		pmotor_spta->GPIOBASE=MOTOR4_GPIOBASE;
		pmotor_spta->PWMGPIO=MOTOR4_PWMGPIO;
	default:
		return;
		break;
	}
	pmotor_spta->id=MotorID;
	pmotor_spta->TIMx=TIMx;	//设置定时器
	pmotor_spta->divnum=StepDive;
	pmotor_spta->MaxPosition=maxposition;
	pmotor_spta->MaxPosition_Pulse=maxposition*StepDive;
}

//电机的运行操作函数

/**************************************************************************************
启动电机按照SPTA方式运行
**************************************************************************************/
void Start_Motor_SPTA(unsigned char MotorID,unsigned char dir,unsigned int Degree)
{
	MOTOR_CONTROL_SPTA *pmotor_spta;
	unsigned int PulsesGiven=0;
	if(Degree==0)
	{ 		  	 
		return;
	}
	//设置电机正反转
	switch(MotorID)
	{
		case 1:
			MOTOR1_CW=dir;	
			pmotor_spta=&motor1;	
			break;
		case 2:
			MOTOR2_CW=dir;
			pmotor_spta=&motor2;
			break;
		case 3:
			MOTOR3_CW=dir;
			pmotor_spta=&motor3;
			break;
		case 4:
			MOTOR4_CW=dir;
			pmotor_spta=&motor4;
			break;
		default:
			return;
	}

	pmotor_spta->en=1;
	pmotor_spta->dir=dir;
	pmotor_spta->running=1;
	pmotor_spta->speedenbale=0;
	PulsesGiven=Degree;
	pmotor_spta->step_move=PulsesGiven*pmotor_spta->divnum;
	pmotor_spta->step_middle=pmotor_spta->step_move>>1;
	/*FIXME:这两个参数可以由用户自行改变测试*/
	pmotor_spta->step_spmax=MAXSPEED_SPTA;
	pmotor_spta->step_accel=ACCSPEED_SPTA;
	pmotor_spta->step_state=ACCELERATING;
	pmotor_spta->step_frac=0;
	pmotor_spta->speed_frac=0;
	pmotor_spta->step_acced=0;
	pmotor_spta->step_speed=0;
	pmotor_spta->step_count=0;		
	TIM_Cmd(pmotor_spta->TIMx, ENABLE);	
}


/**************************************************************************************
重新定位，让正在运行的电机运行到指定位置
**************************************************************************************/
void Reposition_Motor(unsigned char MotorID,unsigned int NewPos)
{
	MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	switch(MotorID)
	{
		case 1:
			pmotor_spta=&motor1;
			break;
		case 2:
			pmotor_spta=&motor2;
			break;
		case 3:
			pmotor_spta=&motor3;
			break;
		case 4:
			pmotor_spta=&motor4;
			break;
		default:
			return;
			break;
	} 
	
	if(NewPos<=pmotor_spta->MaxPosition&&NewPos!=pmotor_spta->CurrentPosition)
	{
		if(NewPos>pmotor_spta->CurrentPosition)
		{
			Start_Motor_SPTA(MotorID,pmotor_spta->clockwise,NewPos-pmotor_spta->CurrentPosition); 
		}
		else
		{
			Start_Motor_SPTA(MotorID,!pmotor_spta->clockwise,pmotor_spta->CurrentPosition-NewPos);  	
		}		 		 
		while(pmotor_spta->running==1);
	}
}

/**************************************************************************************
设置电机运行速度，入口参数是速度等级
**************************************************************************************/
void SetSpeed(unsigned char MotorID,signed char speedindex)
{
	int currentindex,i;
	unsigned int destspeed;
	unsigned int stepstostop=0;
	MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	switch(MotorID)
	{
		case 1:
			pmotor_spta=&motor1; 
			break;
		case 2:
			pmotor_spta=&motor2; 
			break;
		case 3:
			pmotor_spta=&motor3; 
			break;
		case 4:
			pmotor_spta=&motor4; 
			break;
		default:
			return;
	}
	
	TIM_Cmd(pmotor_spta->TIMx, DISABLE);
	if(speedindex>0&&speedindex<=STEP_SPTA)
	{		
		destspeed=speedindex*MAXSPEED_SPTA/STEP_SPTA;
		if(destspeed==pmotor_spta->step_spmax)
		{
			TIM_Cmd(pmotor_spta->TIMx, ENABLE);
			return;
		}
		if(pmotor_spta->step_state==IDLE)
		{			
				/*如果电机已经停止，那么电机的运行参数就要复位，否则参数不变在现有的基础上做速度调整*/
				pmotor_spta->step_frac=0;
				pmotor_spta->speed_frac=0;
				pmotor_spta->step_acced=0;
				pmotor_spta->step_speed=0;
				pmotor_spta->step_count=0;
		}
		if(destspeed<pmotor_spta->step_spmax)
		{
			if(pmotor_spta->step_state!=IDLE)
			{
				pmotor_spta->step_state=DECELERATING;
			}else{
				pmotor_spta->step_state=ACCELERATING;
			}
		}else{
			pmotor_spta->step_state=ACCELERATING;
		}
		pmotor_spta->step_accel=ACCSPEED_SPTA;
		pmotor_spta->step_spmax=destspeed;
		pmotor_spta->speedenbale=1;
		pmotor_spta->running=1;
		pmotor_spta->en=1;
	
	}else{			
		//停止电机, 
		if(pmotor_spta->running==0)return;//已经停止
		pmotor_spta->speedenbale=0;
		pmotor_spta->step_state=DECELERATING;
		pmotor_spta->step_move=pmotor_spta->step_count+pmotor_spta->step_acced;
	}
	TIM_Cmd(pmotor_spta->TIMx, ENABLE);
}

/*************************************************************************************
设置电机的位置，电机运行到指定的位置
*************************************************************************************/
void SetPosition(unsigned char MotorID,unsigned int dest)
{ 
	MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	switch(MotorID)
	{
		case 1:
			pmotor_spta=&motor1; 
			break;
		case 2:
			pmotor_spta=&motor2; 
			break;
		case 3:
			pmotor_spta=&motor3; 
			break;
		case 4:
			pmotor_spta=&motor4; 
			break;
		default:
			return;
	}

	if(dest<=pmotor_spta->MaxPosition&&dest!=pmotor_spta->CurrentPosition)
	{
		if(dest>pmotor_spta->CurrentPosition)
		{
			Start_Motor_SPTA(MotorID,pmotor_spta->clockwise,dest-pmotor_spta->CurrentPosition); 
		}
		else
		{
			Start_Motor_SPTA(MotorID,!pmotor_spta->clockwise,pmotor_spta->CurrentPosition-dest);  	
		}		 		 
		while(pmotor_spta->running==1);
	}

}

/*复位电机*/
void Do_Reset(unsigned char MotorID)
{
	MOTOR_CONTROL_SPTA *pmotor_spta=NULL;
	switch(MotorID)
	{
		case 1:
			pmotor_spta=&motor1;
			break;
		case 2:
			pmotor_spta=&motor2;
			break;
		case 3:
			pmotor_spta=&motor3;
			break;
		case 4:
			pmotor_spta=&motor4;
			break;
		default:
			return;
	} 
	pmotor_spta->rstflg=1;
	pmotor_spta->running=1;
	SetSpeed(MotorID,16);
	while(pmotor_spta->running==1);
	
}



/*see 
http://picprog.strongedge.net/step_prof/step-profile.html
*/

/*电机SPTA算法公共处理函数*/
void TIMX_IRQHandler_SPTA(MOTOR_CONTROL_SPTA *pmotor)
{
	int carry=0;
#ifdef OUTPUT_DATA
	timecnt++;
	periodcnt++;
#endif
	/*拉低脉冲信号*/
	pmotor->GPIOBASE->BRR = pmotor->PWMGPIO; 
	
	/*根据速度累加器是否溢出，决定是否产生一个步进脉冲*/
	pmotor->step_frac += pmotor->step_speed;            
	carry = pmotor->step_frac >> 16;               
	pmotor->step_frac -= carry << 16;
	if(carry!=0)
	{ 
		pmotor->step_count+=1;
		/*拉高脉冲信号产生一个步进脉冲*/
		pmotor->GPIOBASE->BSRR = pmotor->PWMGPIO; 
		
		//位置计算
		if(pmotor->clockwise==pmotor->dir)
		{
			pmotor->CurrentPosition_Pulse++;
			if(pmotor->CurrentPosition_Pulse>=pmotor->MaxPosition_Pulse)
			{
				pmotor->CurrentPosition_Pulse=0;
			}
		}
		else
		{
			pmotor->CurrentPosition_Pulse--;
			if(pmotor->CurrentPosition_Pulse==0xffffffff)
			{
				pmotor->CurrentPosition_Pulse=pmotor->MaxPosition_Pulse-1;
			}
		}
		pmotor->CurrentPosition=pmotor->CurrentPosition_Pulse/pmotor->divnum;
#ifdef OUTPUT_DATA
		
		//每个时刻对应的高电平与低电平
		sprintf(tmp,"%f,0\r\n%f,1\r\n",timecnt-1,timecnt);
		
		USART1_Printfstr(tmp);	
		periodcnt=0;
#endif
	}	
	
	//速度控制
	if(pmotor->speedenbale)
	{
		if( (pmotor->step_speed>=pmotor->step_spmax&&pmotor->step_speed-pmotor->step_spmax<=3)||
				(pmotor->step_speed<=pmotor->step_spmax&&pmotor->step_spmax-pmotor->step_speed<=3))
		{
			return;
		}
	}
	/*根据电机的状态进行状态转换以及参数变换*/
	switch(pmotor->step_state)
	{ 		
		case ACCELERATING:
			if(carry){				
				pmotor->step_acced++;
			}
			pmotor->speed_frac+=pmotor->step_accel;		
			carry=pmotor->speed_frac>>17;
			pmotor->speed_frac-=carry<<17;
			if(carry){
				pmotor->step_speed+=carry;
			}
			if(!pmotor->speedenbale)
			{
				/*although we are ACCELERATING,but middle point reached,we need DECELERATING*/
				if(pmotor->step_middle!=0){
					if(pmotor->step_count==pmotor->step_middle)   
					{               
							pmotor->step_state=DECELERATING;
					}		
				}else if(pmotor->step_count>0){
					pmotor->step_state=DECELERATING;
				}
			}
			if(pmotor->step_speed>=pmotor->step_spmax)
			{        
					pmotor->step_speed=pmotor->step_spmax;
					pmotor->step_state=AT_MAX;
			}
			break;	 
		case AT_MAX:                                               
		  if(pmotor->step_move-pmotor->step_count<=pmotor->step_acced)   
			{               
					pmotor->step_state=DECELERATING;
			}
			break;
		case DECELERATING:  
			if(carry&&pmotor->step_acced>0){				
				pmotor->step_acced--;
			}
			pmotor->speed_frac+=pmotor->step_accel;		
			carry=pmotor->speed_frac>>17;
			pmotor->speed_frac-=carry<<17;
			if(carry&&pmotor->step_speed>carry){
				pmotor->step_speed-=carry;
			}	
			if(!pmotor->speedenbale)
			{
				if(pmotor->step_count>=pmotor->step_move)
				{  
						pmotor->step_state=IDLE;  
						pmotor->running=0;
						pmotor->step_spmax=0;
						TIM_Cmd(pmotor->TIMx, DISABLE);
	#ifdef OUTPUT_DATA
						timecnt=0;
	#endif
				}
			}
			break;
	}
}
 

void MOTORSTOP_IRQ(unsigned char MotorID)
{
	MOTOR_CONTROL_SPTA *pmotor_spta=NULL;
	switch(MotorID)
	{
		case 1:
			pmotor_spta=&motor1;
			break;
		case 2:
			pmotor_spta=&motor2;
			break;
		case 3:
			pmotor_spta=&motor3;
			break;
		case 4:
			pmotor_spta=&motor4;
			break;
		default:
			return;
	} 
	if(pmotor_spta->rstflg==1)
	{
		pmotor_spta->speedenbale=0;
		pmotor_spta->PulsesHaven=pmotor_spta->PulsesGiven-128;	
		pmotor_spta->dir=M4_UNCLOCKWISE;
		pmotor_spta->CurrentPosition_Pulse=128;		
		pmotor_spta->rstflg=0;
	} 
	
}