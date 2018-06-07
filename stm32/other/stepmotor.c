#include "stepmotor.h"
/*see 
http://picprog.strongedge.net/step_prof/step-profile.html
*/
#ifdef OUTPUT_DATA
char tmp[64];
float timecnt=0;
float periodcnt=0;
#endif

#ifdef MOTOR1_S_CONTROL_MODE
	MOTOR_CONTROL_S motor1;	
	/*正常S型曲线参数生成的表格*/
	uint16_t Motor1TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor1StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为2/3 S型曲线参数生成的表格*/
	uint16_t Motor1_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor1_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为1/3 S型曲线参数生成的表格*/
	uint16_t Motor1_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor1_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
#else
	MOTOR_CONTROL_SPTA motor1;
#endif

#ifdef MOTOR2_S_CONTROL_MODE
	MOTOR_CONTROL_S motor2;	
	/*正常S型曲线参数生成的表格*/
	uint16_t Motor2TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor2StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为2/3 S型曲线参数生成的表格*/
	uint16_t Motor2_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor2_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为1/3 S型曲线参数生成的表格*/
	uint16_t Motor2_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor2_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
#else
	MOTOR_CONTROL_SPTA motor2;
#endif

#ifdef MOTOR3_S_CONTROL_MODE
	MOTOR_CONTROL_S motor3;	
	/*正常S型曲线参数生成的表格*/
	uint16_t Motor3TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor3StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为2/3 S型曲线参数生成的表格*/
	uint16_t Motor3_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor3_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为1/3 S型曲线参数生成的表格*/
	uint16_t Motor3_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor3_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
#else
	MOTOR_CONTROL_SPTA motor3;
#endif

#ifdef MOTOR4_S_CONTROL_MODE
	MOTOR_CONTROL_S motor4;	
	/*正常S型曲线参数生成的表格*/
	uint16_t Motor4TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor4StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为2/3 S型曲线参数生成的表格*/
	uint16_t Motor4_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor4_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	/*将参数降为1/3 S型曲线参数生成的表格*/
	uint16_t Motor4_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
	uint16_t Motor4_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
#else
	MOTOR_CONTROL_SPTA motor4;
#endif



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
根据S型曲线参数获取某个时刻的频率
*************************************************************************************/
float GetFreAtTime(float fstart,float faa,float taa,float tua,float tra,float t)
{
		//根据公式计算从开始到最高速过冲中，t时刻的转动频率
	  if(t>=0&&t<=taa){
			//加加速阶段
			return fstart+0.5*faa*t*t;
		}else if(taa<t&&t<=(taa+tua)){
			//匀加速阶段
			return fstart+0.5*faa*taa*taa+(t-taa)*faa*taa;
		}else if((taa+tua)<t&&t<=(taa+tua+tra)){
			//减加速阶段
			return fstart+0.5*faa*taa*taa+(tua)*faa*taa+0.5*faa*taa*tra-0.5*faa*taa*(taa+tua+tra-t)*(taa+tua+tra-t)/(tra);
		}		
		return 0;
}

/**************************************************************************************
计算S型曲线算法的每一步定时器周期及步进数
**************************************************************************************/
void CalcMotorPeriStep_CPF(float fstart,float faa,float taa,float tua,float tra,uint16_t MotorTimeTable[],uint16_t MotorStepTable[])
{
  int  i;
	float fi;
	
	for(i=0;i<STEP_AA;i++)
	{
		fi=GetFreAtTime(fstart,faa,taa,tua,tra,taa/STEP_AA*i);
		MotorTimeTable[i]=F2TIME_PARA/fi;
		MotorStepTable[i]=fi*(taa/STEP_AA)/STEP_PARA;
	}
	for(i=STEP_AA;i<STEP_AA+STEP_UA;i++)
	{
		fi=GetFreAtTime(fstart,faa,taa,tua,tra,taa+(tua/STEP_UA)*(i-STEP_AA));
		MotorTimeTable[i]=F2TIME_PARA/fi;
		MotorStepTable[i]=fi*(tua/STEP_UA)/STEP_PARA;
	}
	for(i=STEP_AA+STEP_UA;i<STEP_AA+STEP_UA+STEP_RA;i++)
	{
		fi=GetFreAtTime(fstart,faa,taa,tua,tra,taa+tua+tra/STEP_RA*(i-STEP_AA-STEP_UA));
		MotorTimeTable[i]=F2TIME_PARA/fi;
		MotorStepTable[i]=fi*(tra/STEP_RA)/STEP_PARA;
	}
	fi=GetFreAtTime(fstart,faa,taa,tua,tra,taa+tua+tra);
	MotorTimeTable[STEP_AA+STEP_UA+STEP_RA]=F2TIME_PARA/fi;
	MotorStepTable[STEP_AA+STEP_UA+STEP_RA]=fi*(tra/STEP_RA)/STEP_PARA;
	
	
	for(i=STEP_AA+STEP_UA+STEP_RA+1;i<2*(STEP_AA+STEP_UA+STEP_RA)+1;i++)
	{ 
		MotorTimeTable[i]=MotorTimeTable[2*(STEP_AA+STEP_UA+STEP_RA)-i];
		MotorStepTable[i]=MotorStepTable[2*(STEP_AA+STEP_UA+STEP_RA)-i];
	}
}

/**************************************************************************************
电机运行参数初始化
**************************************************************************************/
void MotorRunParaInitial(void)
{ 
#ifdef MOTOR1_S_CONTROL_MODE
	/*FIXME:用户可以改变该参数实现S型曲线的升降特性*/ 
	CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor1TimeTable,Motor1StepTable); 
	/*更改参数降为2/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor1_23TimeTable,Motor1_23StepTable); 
	/*更改参数降为1/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor1_13TimeTable,Motor1_13StepTable); 
#endif
#ifdef MOTOR2_S_CONTROL_MODE
	/*FIXME:用户可以改变该参数实现S型曲线的升降特性*/ 
	CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor1TimeTable,Motor1StepTable); 
	/*更改参数降为2/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor2_23TimeTable,Motor2_23StepTable); 
	/*更改参数降为1/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor2_13TimeTable,Motor2_13StepTable); 
#endif
#ifdef MOTOR3_S_CONTROL_MODE
	/*FIXME:用户可以改变该参数实现S型曲线的升降特性*/ 
	CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor1TimeTable,Motor1StepTable); 
	/*更改参数降为2/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor3_23TimeTable,Motor3_23StepTable); 
	/*更改参数降为1/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor3_13TimeTable,Motor3_13StepTable); 
#endif
#ifdef MOTOR4_S_CONTROL_MODE
	/*FIXME:用户可以改变该参数实现S型曲线的升降特性*/ 
	CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor1TimeTable,Motor1StepTable); 
	/*更改参数降为2/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor4_23TimeTable,Motor4_23StepTable); 
	/*更改参数降为1/3生成的表格*/
	CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor4_13TimeTable,Motor4_13StepTable); 
#endif	
}


 /**************************************************************************************
 初始化电机的参数，主要是细分选择，使用的定时器，顺时针方向值，电机ID等
 **************************************************************************************/
void Initial_Motor(unsigned char MotorID, unsigned char StepDive,unsigned int maxposition , TIM_TypeDef* TIMx)
{
	MOTOR_CONTROL_S *pmotor_s=0; 
	MOTOR_CONTROL_SPTA *pmotor_spta=0; 
	uint16_t *MotorTimeTable;
	uint16_t *MotorStepTable;
	uint8_t u8mode=0;
	uint16_t i=0;
	
	switch(MotorID)
    {
		case 1:
			//StepDive 设置步进电机电机1的细分
#ifdef	MOTOR1_S_CONTROL_MODE
			pmotor_s = &motor1;
			MotorTimeTable=Motor1TimeTable;
			MotorStepTable=Motor1StepTable;
			u8mode=1;
#else
			pmotor_spta=&motor1;
			u8mode=2
#endif
			break;
		case 2:
			//StepDive 设置步进电机电机2的细分
#ifdef	MOTOR2_S_CONTROL_MODE
			pmotor_s = &motor2;
			MotorTimeTable=Motor2TimeTable;
			MotorStepTable=Motor2StepTable;
			u8mode=1;
#else
			pmotor_spta=&motor2;
			u8mode=2;
#endif
			break;
		case 3:
			//StepDive 设置步进电机电机3的细分
#ifdef	MOTOR3_S_CONTROL_MODE
			MotorTimeTable=Motor3TimeTable;
			MotorStepTable=Motor3StepTable;
			pmotor_s = &motor3;
			u8mode=1;
#else
			pmotor_spta=&motor3;
			u8mode=2;
#endif
			break;
		case 4:
			//StepDive 设置步进电机电机4的细分
#ifdef	MOTOR4_S_CONTROL_MODE
			MotorTimeTable=Motor4TimeTable;
			MotorStepTable=Motor4StepTable;
			pmotor_s = &motor4;
			u8mode=1;
#else
			pmotor_spta=&motor4;
			u8mode=2;
#endif
			break;
		default:
            break;
	}
	
    switch(u8mode)
    {
        case 1:
			pmotor_s->id=MotorID;	//设置ID
			switch(MotorID)
			{
				case 1:
					pmotor_s->clockwise=M1_CLOCKWISE;	//设置正反转
					break;
				case 2:
					pmotor_s->clockwise=M2_CLOCKWISE;	//设置正反转
					break;
				case 3:
					pmotor_s->clockwise=M3_CLOCKWISE;	//设置正反转
					break;
				case 4:
					pmotor_s->clockwise=M4_CLOCKWISE;	//设置正反转
					break;
				default:
					break;
			}
			pmotor_s->TIMx=TIMx;	//设置定时器
			pmotor_s->divnum=StepDive;//设置细分
			pmotor_s->MaxPosition=maxposition;//设置最大位置
			pmotor_s->MaxPosition_Pulse=maxposition*StepDive;

			pmotor_s->CurrentPosition=0;
			pmotor_s->CurrentPosition_Pulse=0;
			pmotor_s->StartTableLength=STEP_AA+STEP_UA+STEP_RA+1;
			pmotor_s->StopTableLength=STEP_AA+STEP_UA+STEP_RA; 
			pmotor_s->Counter_Table=MotorTimeTable;
			pmotor_s->Step_Table=MotorStepTable;

			pmotor_s->CurrentIndex=0;
			pmotor_s->speedenbale=0;
			pmotor_s->StartSteps=0;                  //必须清零，后面是累加，否则会把前一次的加上
			pmotor_s->StopSteps=0;                   //同上
			for(i=0;i<pmotor_s->StartTableLength;i++)
				pmotor_s->StartSteps+=pmotor_s->Step_Table[i];
			for(i=0;i<pmotor_s->StopTableLength;i++)
				pmotor_s->StopSteps+=pmotor_s->Step_Table[i+pmotor_s->StartTableLength];

			pmotor_s->TIMx->ARR =pmotor_s->Counter_Table[0]; //设置周期
			pmotor_s->TIMx->CCR1 =pmotor_s->Counter_Table[0]>>1;       //设置占空比
            break;	
		case 2:
			pmotor_spta->id=MotorID;	//设置ID
			switch(MotorID)
			{
				case 1:
					pmotor_spta->clockwise=M1_CLOCKWISE;	//设置正反转
					pmotor_spta->GPIOBASE=MOTOR1_GPIOBASE;
					pmotor_spta->PWMGPIO=MOTOR1_PWMGPIO;
					break;
				case 2:
					pmotor_spta->clockwise=M2_CLOCKWISE;	//设置正反转
					pmotor_spta->GPIOBASE=MOTOR2_GPIOBASE;
					pmotor_spta->PWMGPIO=MOTOR2_PWMGPIO;
					break;
				case 3:
					pmotor_spta->clockwise=M3_CLOCKWISE;	//设置正反转
					pmotor_spta->GPIOBASE=MOTOR3_GPIOBASE;
					pmotor_spta->PWMGPIO=MOTOR3_PWMGPIO;
					break;
				case 4:
					pmotor_spta->clockwise=M4_CLOCKWISE;	//设置正反转
					pmotor_spta->GPIOBASE=MOTOR4_GPIOBASE;
					pmotor_spta->PWMGPIO=MOTOR4_PWMGPIO;
					break;
				default:
					break;
			}
			pmotor_spta->TIMx=TIMx;	//设置定时器
			pmotor_spta->divnum=StepDive;
			pmotor_spta->MaxPosition=maxposition;
			pmotor_spta->MaxPosition_Pulse=maxposition*StepDive;
			
            break;		
        default:
            break;
    }
}

//电机的运行操作函数

/*多轴协同使用了算法原理进行时间预估，所以修改该算法时记得
 这两处保持同步*/
/*计算S型曲线反转点，S型曲线在运行时，加减速过程是完全对称的*/
unsigned long long Get_TimeCost_ReverDot_S(unsigned char MotorID,MOTOR_CONTROL_S *pmotor )
{
	unsigned long long time_cost=0;
	unsigned long long time_cost2=0;
	unsigned int pulsecnt=0;
	int i=0,j;
	/*
	switch(MotorID)
	{
		case 1:
			pmotor=&motor1;  
			break;
		case 2:
			pmotor=&motor2;  
			break;
		case 3:
			pmotor=&motor3;  
			break;
		default:
			return 0;
	}
	*/
	
	if(pmotor->PulsesGiven>=pmotor->StartSteps+pmotor->StopSteps)
	{
		for(i=0;i<pmotor->StartTableLength;i++)
			time_cost+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
		for(i=0;i<pmotor->StopTableLength;i++)
			time_cost+=(pmotor->Step_Table[i+pmotor->StartTableLength]*pmotor->Counter_Table[i+pmotor->StartTableLength]);		
		time_cost+=(pmotor->PulsesGiven-pmotor->StartSteps-pmotor->StopSteps)*pmotor->Counter_Table[pmotor->StartTableLength-1];
		
		pmotor->RevetDot=pmotor->PulsesGiven-pmotor->StopSteps;
	}
	else
	{
		//考虑这种情况，第一频率142 步，第二频率148步，要是运动200步该怎么运行
		//所以这里要改变第二频率的步数
		while((pulsecnt+pmotor->Step_Table[i])<=(pmotor->PulsesGiven>>1))
		{					
			time_cost+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
			time_cost2+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
			pulsecnt+=pmotor->Step_Table[i];
			i++;
		}
		time_cost+=time_cost2;
		if(pmotor->Step_Table[i]<pmotor->PulsesGiven-2*pulsecnt)
		{
			pmotor->Step_Table[i]=pmotor->PulsesGiven-2*pulsecnt;
			pmotor->StartSteps=0;                  //必须清零，后面是累加，否则会把前一次的加上
			pmotor->StopSteps=0;                   //同上
			for(j=0;j<pmotor->StartTableLength;j++)
			 pmotor->StartSteps+=pmotor->Step_Table[j];
			for(j=0;j<pmotor->StopTableLength;j++)
			 pmotor->StopSteps+=pmotor->Step_Table[j+pmotor->StartTableLength];
		}
		time_cost+=(pmotor->Counter_Table[i]*(pmotor->PulsesGiven-2*pulsecnt));
		pmotor->RevetDot=pmotor->PulsesGiven-pulsecnt;
	}
	pmotor->Time_Cost_Cal=time_cost;
	return time_cost;
}

/*重新初始化电机运行时相关参数*/
void Motor_Reinitial(unsigned char MotorID,MOTOR_CONTROL_S *pmotor)
{
	int i=0; 
	//MOTOR_CONTROL_S *pmotor=NULL;  
	uint16_t *MotorTimeTable;
	uint16_t *MotorStepTable;
	uint16_t *MotorTime23Table;
	uint16_t *MotorStep23Table;
	uint16_t *MotorTime13Table;
	uint16_t *MotorStep13Table;
	
	switch(MotorID)
	{
		case 1:
#ifdef MOTOR1_S_CONTROL_MODE
			MotorTimeTable=Motor1TimeTable;
			MotorStepTable=Motor1StepTable;
			MotorTime23Table=Motor1_23TimeTable;
			MotorStep23Table=Motor1_23StepTable;
			MotorTime13Table=Motor1_13TimeTable;
			MotorStep13Table=Motor1_13StepTable;
#endif
			break;
		case 2:
#ifdef MOTOR2_S_CONTROL_MODE
			MotorTimeTable=Motor2TimeTable;
			MotorStepTable=Motor2StepTable;
			MotorTime23Table=Motor2_23TimeTable;
			MotorStep23Table=Motor2_23StepTable;
			MotorTime13Table=Motor2_13TimeTable;
			MotorStep13Table=Motor2_13StepTable;
#endif		
			break;
		case 3:
#ifdef MOTOR3_S_CONTROL_MODE
			MotorTimeTable=Motor3TimeTable;
			MotorStepTable=Motor3StepTable;
			MotorTime23Table=Motor3_23TimeTable;
			MotorStep23Table=Motor3_23StepTable;
			MotorTime13Table=Motor3_13TimeTable;
			MotorStep13Table=Motor3_13StepTable;
#endif
			break;
		case 4:
#ifdef MOTOR4_S_CONTROL_MODE
			MotorTimeTable=Motor4TimeTable;
			MotorStepTable=Motor4StepTable;
			MotorTime23Table=Motor4_23TimeTable;
			MotorStep23Table=Motor4_23StepTable;
			MotorTime13Table=Motor4_13TimeTable;
			MotorStep13Table=Motor4_13StepTable;
#endif
		default:
			return ;
	}					 
	pmotor->pulsecount=0;
	pmotor->CurrentIndex=0;
	pmotor->speedenbale=0;
	
	pmotor->Counter_Table=MotorTimeTable;  		//指向启动时，时间基数计数表
	pmotor->Step_Table=MotorStepTable;  		//指向启动时，每个频率脉冲个数表
	pmotor->StartSteps=0;                  //必须清零，后面是累加，否则会把前一次的加上
	pmotor->StopSteps=0;                   //同上
	for(i=0;i<pmotor->StartTableLength;i++)
		pmotor->StartSteps+=pmotor->Step_Table[i];
	for(i=0;i<pmotor->StopTableLength;i++)
		pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
	if(pmotor->PulsesGiven<pmotor->StartSteps+pmotor->StopSteps){
		//如果给定的运行步数小余最大S型曲线，则尝试选择2/3S型曲线
		pmotor->Counter_Table=MotorTime23Table;  		
		pmotor->Step_Table=MotorStep23Table;  			
		pmotor->StartSteps=0;                  
		pmotor->StopSteps=0;                   
		for(i=0;i<pmotor->StartTableLength;i++)
			pmotor->StartSteps+=pmotor->Step_Table[i];
		for(i=0;i<pmotor->StopTableLength;i++)
			pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
		if(pmotor->PulsesGiven<pmotor->StartSteps+pmotor->StopSteps){
			//如果给定的运行步数小余最大S型曲线，则尝试选择1/3S型曲线
			pmotor->Counter_Table=MotorTime23Table;  		
			pmotor->Step_Table=MotorStep23Table;  			
			pmotor->StartSteps=0;                  
			pmotor->StopSteps=0;                   
			for(i=0;i<pmotor->StartTableLength;i++)
				pmotor->StartSteps+=pmotor->Step_Table[i];
			for(i=0;i<pmotor->StopTableLength;i++)
				pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
		}
	}
	
	pmotor->TIMx->ARR =pmotor->Counter_Table[0]; //设置周期
	pmotor->TIMx->CCR1 =pmotor->Counter_Table[0]>>1;       //设置占空比
	pmotor->Time_Cost_Act=pmotor->TIMx->ARR;
	Get_TimeCost_ReverDot_S(MotorID,pmotor);		 
		
}

/**************************************************************************************
两个电机同时运行时，花费时间较少的电机要根据时间长的电机调整运行参数
***************************************************************************************/
void Find_BestTimeCost(unsigned char ID,MOTOR_CONTROL_S *pmotor,unsigned long long time_cost,unsigned char dir,unsigned int Degree)
{
	int id=ID;
	float i=0,j=0;
	float fi=M_FRE_START,fj=M_FRE_START;
	int cal_ij=1;
	unsigned int PulsesGiven=0;
	uint16_t *MotorTimeTable;
	uint16_t *MotorStepTable;
	//MOTOR_CONTROL_S *pmotor=NULL;
	unsigned long long time_cost_tmp=0;
	unsigned long long time_cost_min=0xffffffffff;
	float i_o=0,j_o=0;
	float fi_o=0,fj_o=0;
	/*
	switch(ID)
	{
		 case 1:
			 pmotor=&motor1; 
			 break;
		 case 2:
			 pmotor=&motor2;
			 break;
		 case 3:
			 pmotor=&motor3;
			 break;
		 case 4:
			 pmotor=&motor4;
			 break;
	}
	if(pmotor==NULL)
	{
		return ;
	}
	*/
	MotorTimeTable=pmotor->Counter_Table;
	MotorStepTable=pmotor->Step_Table;
	
	j=M_FRE_AA;
	i=0;
	while(1)
	{		
		if(cal_ij)
		{
			CalcMotorPeriStep_CPF(M_FRE_START,(i+j)/2.0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
		}
		else
		{
			CalcMotorPeriStep_CPF((fi+fj)/2,0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
		}
		pmotor->en=1;
		pmotor->dir=dir;
		pmotor->running=1;
		pmotor->PulsesHaven=0;
		PulsesGiven=Degree;
		pmotor->Time_Cost_Act=0;
		pmotor->PulsesGiven=PulsesGiven*pmotor->divnum;
		//pmotor->PulsesGiven+=300;
		Motor_Reinitial(id,pmotor);		
		//time_cost_tmp=Get_Time_Cost2(id);
		time_cost_tmp=pmotor->Time_Cost_Cal;
		if(time_cost_tmp<time_cost)
		{
			if(time_cost-time_cost_tmp<time_cost_min)
			{
				time_cost_min=time_cost-time_cost_tmp;
				i_o=i;
				j_o=j;
				fi_o=fi;
				fj_o=fj;
			}
		}
		else
		{
			if(time_cost_tmp-time_cost<time_cost_min)
			{
				time_cost_min=time_cost_tmp-time_cost;
				i_o=i;
				j_o=j;
				fi_o=fi;
				fj_o=fj;
			}
		}
		if(time_cost_tmp>=time_cost-32*4&&time_cost_tmp<=time_cost+32*4)
		{
			break ;
		}
		if(cal_ij)
		{
			if(j<0.1)
			{
				//说明即使是使用最基本的启动速度都无法同时停止，则修改为降低速度的梯形
				i_o=0;
				j_o=0;
				j=0;
				i=0;
				cal_ij=0;
				fi=0;
			}
		}
		if(cal_ij)
		{
			if((i>j&&i-j<0.02)||((i<j)&&j-i<0.02))
			{
				break;
			}
			if(time_cost_tmp>time_cost)
			{
				i=(i+j)/2.0;
			}
			else
			{
				j=(i+j)/2.0; 
			}
		}
		else
		{
			if((fi>fj&&fi-fj<0.02)||((fi<fj)&&fj-fi<0.02))
			{
				break;
			}
			if(time_cost_tmp>time_cost)
			{
				fi=(fi+fj)/2.0;
			}
			else
			{
				fj=(fi+fj)/2.0; 
			}
		}
	}
	CalcMotorPeriStep_CPF((fi_o+fj_o)/2,(i_o+j_o)/2.0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
}


/**************************************************************************************
启动电机按照S型曲线参数运行
***************************************************************************************/
void Start_Motor_S(unsigned char MotorID,MOTOR_CONTROL_S *pmotor,unsigned char dir,unsigned int Degree)
{
	unsigned int PulsesGiven=0;
	//MOTOR_CONTROL_S *pmotor=NULL; 
	if(Degree==0)
	{ 		  	 
		return;
	}
	//设置电机正反转
	switch(MotorID)
	{
		case 1:
			MOTOR1_CW=dir;		
			break;
		case 2:
			MOTOR2_CW=dir;
			break;
		case 3:
			MOTOR3_CW=dir;
			break;
		case 4:
			MOTOR4_CW=dir;
			break;
		default:
			return;
	}
	pmotor->en=1;
	pmotor->dir=dir;
	pmotor->running=1;
	pmotor->PulsesHaven=0;
	PulsesGiven=Degree;
	pmotor->Time_Cost_Act=0;
	pmotor->PulsesGiven=PulsesGiven*pmotor->divnum;
	Motor_Reinitial(MotorID,pmotor);		
	pmotor->CurrentIndex=0;
	pmotor->speedenbale=0;
	pmotor->TIMx->ARR =pmotor->Counter_Table[0]; //设置周期
	pmotor->TIMx->CCR1 =pmotor->Counter_Table[0]>>1;       //设置占空比
	TIM_Cmd(pmotor->TIMx, ENABLE);		  //DISABLE
}


/**************************************************************************************
启动电机按照SPTA方式运行
**************************************************************************************/
void Start_Motor_SPTA(unsigned char MotorID,MOTOR_CONTROL_SPTA *pmotor,unsigned char dir,unsigned int Degree)
{
	unsigned int PulsesGiven=0;
	//MOTOR_CONTROL_SPTA *pmotor=NULL; 
	if(Degree==0)
	{ 		  	 
		return;
	}
	//设置电机正反转
	switch(MotorID)
	{
		case 1:
			MOTOR1_CW=dir;		
			break;
		case 2:
			MOTOR2_CW=dir;
			break;
		case 3:
			MOTOR3_CW=dir;
			break;
		case 4:
			MOTOR4_CW=dir;
			break;
		default:
			return;
	}

	pmotor->en=1;
	pmotor->dir=dir;
	pmotor->running=1;
	pmotor->speedenbale=0;
	PulsesGiven=Degree;
	pmotor->step_move=PulsesGiven*pmotor->divnum;
	pmotor->step_middle=pmotor->step_move>>1;
	/*FIXME:这两个参数可以由用户自行改变测试*/
	pmotor->step_spmax=MAXSPEED_SPTA;
	pmotor->step_accel=ACCSPEED_SPTA;
	pmotor->step_state=ACCELERATING;
	pmotor->step_frac=0;
	pmotor->speed_frac=0;
	pmotor->step_acced=0;
	pmotor->step_speed=0;
	pmotor->step_count=0;
		
	TIM_Cmd(pmotor->TIMx, ENABLE);	
}


/**************************************************************************************
重新定位，让正在运行的电机运行到指定位置
**************************************************************************************/
void Reposition_Motor(unsigned char MotorID,MOTOR_CONTROL_S *pmotor_s,MOTOR_CONTROL_SPTA *pmotor_spta,unsigned int NewPos)
{
	//MOTOR_CONTROL_S *pmotor_s=NULL;
	//MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	/*switch(MotorID)
	{
		case 1:
			pmotor_s=&motor1;
			break;
		case 2:
			pmotor_s=&motor2;
			break;
		case 3:
			pmotor_s=&motor3;
			break;
		case 4:
			pmotor_spta=&motor4;
			break;
		default:
			return;
	} 
	*/
	if(pmotor_s!=NULL){
		if(NewPos<=pmotor_s->MaxPosition&&NewPos!=pmotor_s->CurrentPosition)
		{
			if(NewPos>pmotor_s->CurrentPosition)
			{
				Start_Motor_S(MotorID,pmotor_s,pmotor_s->clockwise,NewPos-pmotor_s->CurrentPosition); 
			}
			else
			{
				Start_Motor_S(MotorID,pmotor_s,!pmotor_s->clockwise,pmotor_s->CurrentPosition-NewPos);  	
			}		 		 
			while(pmotor_s->running==1);
		}
	}
	if(pmotor_spta!=NULL){
		if(NewPos<=pmotor_spta->MaxPosition&&NewPos!=pmotor_spta->CurrentPosition)
		{
			if(NewPos>pmotor_spta->CurrentPosition)
			{
				Start_Motor_SPTA(MotorID,pmotor_spta,pmotor_spta->clockwise,NewPos-pmotor_spta->CurrentPosition); 
			}
			else
			{
				Start_Motor_SPTA(MotorID,pmotor_spta,!pmotor_spta->clockwise,pmotor_spta->CurrentPosition-NewPos);  	
			}		 		 
			while(pmotor_s->running==1);
		}
	}
}

/**************************************************************************************
同时启动电机12
***************************************************************************************/
void Start_Motor12(MOTOR_CONTROL_S *pmotor_s1,unsigned char dir1,unsigned int Degree1,unsigned char dir2,unsigned int Degree2,MOTOR_CONTROL_S *pmotor_s2)
{
	unsigned int PulsesGiven=0;
	unsigned long long time_cost=0;
	if(Degree1==0||Degree2==0)
	{ 		  	 
		return;
	}	
	MotorRunParaInitial();
	Motor_Reinitial(pmotor_s1->id,pmotor_s1);		
	Motor_Reinitial(pmotor_s2->id,pmotor_s2);		
	if(Degree1<=Degree2)
	{
		pmotor_s2->en=1;
		pmotor_s2->dir=dir1;
		pmotor_s2->running=1;
		pmotor_s2->PulsesHaven=0;
		PulsesGiven=Degree2;
		pmotor_s2->Time_Cost_Act=0;
		pmotor_s2->PulsesGiven=PulsesGiven*pmotor_s2->divnum;
		time_cost=Get_TimeCost_ReverDot_S(pmotor_s2->id,pmotor_s2);
		Find_BestTimeCost(pmotor_s1->id,pmotor_s1,time_cost,dir1,Degree1);
	}
	else
	{
		pmotor_s1->en=1;
		pmotor_s1->dir=dir1;
		pmotor_s1->running=1;
		pmotor_s1->PulsesHaven=0;
		PulsesGiven=Degree1;
		pmotor_s1->Time_Cost_Act=0;
		pmotor_s1->PulsesGiven=PulsesGiven*pmotor_s1->divnum;
		time_cost=Get_TimeCost_ReverDot_S(pmotor_s1->id,pmotor_s1);
		Find_BestTimeCost(pmotor_s2->id,pmotor_s2,time_cost,dir2,Degree2);
	}
	
	Start_Motor_S(pmotor_s1->id,pmotor_s1,dir1,Degree1);
	Start_Motor_S(pmotor_s1->id,pmotor_s2,dir2,Degree2);
}


/**************************************************************************************
设置电机运行速度，入口参数是速度等级
**************************************************************************************/
void SetSpeed(unsigned char MotorID,MOTOR_CONTROL_S *pmotor_s,MOTOR_CONTROL_SPTA *pmotor_spta,signed char speedindex)
{
	int currentindex,i;
	unsigned int destspeed;
	unsigned int stepstostop=0;
	//MOTOR_CONTROL_S *pmotor_s=NULL; 
	//MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	/*
	switch(MotorID)
	{
		case 1:
			pmotor_s=&motor1; 
			break;
		case 2:
			pmotor_s=&motor2; 
			break;
		case 3:
			pmotor_s=&motor3; 
			break;
		case 4:
			pmotor_spta=&motor4; 
			break;
		default:
			return;
	}
	*/
	if(pmotor_s!=NULL){
		TIM_Cmd(pmotor_s->TIMx, DISABLE);
		if(speedindex>=0&&speedindex<=STEP_AA+STEP_UA+STEP_RA)
		{		
				//直接向下一速度
				currentindex=pmotor_s->CurrentIndex;
				pmotor_s->PulsesHaven=0;
				if(pmotor_s->CurrentIndex>=pmotor_s->StartTableLength )
				{
						currentindex=pmotor_s->StartTableLength+pmotor_s->StopTableLength-pmotor_s->CurrentIndex-1;  
				}
				if(currentindex>speedindex)
				{
						//需要减速
						pmotor_s->PulsesGiven=pmotor_s->PulsesHaven+pmotor_s->StopSteps-2;
				}
				else
				{
					//需要加速
					pmotor_s->PulsesGiven=0xffffffff;
				}
				pmotor_s->CurrentIndex=currentindex;
				pmotor_s->pulsecount=pmotor_s->Step_Table[pmotor_s->CurrentIndex];
				pmotor_s->TargetIndex=speedindex;
				pmotor_s->speedenbale=1;
				pmotor_s->running=1;
				pmotor_s->en=1;
		}
		else
		{
			
			//停止电机,pmotor->CurrentIndex=currentindex-1;直接向下一减速
			pmotor_s->speedenbale=0;
			currentindex=pmotor_s->CurrentIndex; 
			if(pmotor_s->CurrentIndex>=pmotor_s->StartTableLength )
			{
					currentindex=pmotor_s->StartTableLength+pmotor_s->StopTableLength-pmotor_s->CurrentIndex-1;  
			}
			if(1)//currentindex>=1)
			{
				for(i=0;i<currentindex;i++)
				{
					 stepstostop+=pmotor_s->Step_Table[i];
				}
				/*进入减速index*/
				if(pmotor_s->CurrentIndex<pmotor_s->StartTableLength )
				{
						currentindex=pmotor_s->StartTableLength+pmotor_s->StopTableLength-pmotor_s->CurrentIndex-1;  
				}
				pmotor_s->CurrentIndex=currentindex;
				pmotor_s->pulsecount=pmotor_s->Step_Table[pmotor_s->CurrentIndex];
				pmotor_s->PulsesHaven=0;
				pmotor_s->PulsesGiven=pmotor_s->PulsesHaven+stepstostop;
			}
			else
			{
				pmotor_s->PulsesGiven=pmotor_s->PulsesHaven;
			}
			if(stepstostop==0)
			{
				//已经停止
				return;
			}
		}
		TIM_Cmd(pmotor_s->TIMx, ENABLE);
	}
	
	if(pmotor_spta!=NULL){
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
					/*如果电机已经停止，那么电机的运行参数就要复位，否则参数不变
				   在现有的基础上做速度调整*/
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
			}
			else
			{
				pmotor_spta->step_state=ACCELERATING;
			}
			
			pmotor_spta->step_accel=ACCSPEED_SPTA;
			pmotor_spta->step_spmax=destspeed;
			pmotor_spta->speedenbale=1;
			pmotor_spta->running=1;
			pmotor_spta->en=1;
			
		}
		else
		{			
			//停止电机, 
			if(pmotor_spta->running==0)
			{
				//已经停止
				return;
			}
			pmotor_spta->speedenbale=0;
			pmotor_spta->step_state=DECELERATING;
			pmotor_spta->step_move=pmotor_spta->step_count+pmotor_spta->step_acced;
		}
		TIM_Cmd(pmotor_spta->TIMx, ENABLE);
	}
}


/*************************************************************************************
设置电机的位置，电机运行到指定的位置
*************************************************************************************/
void SetPosition(unsigned char MotorID,MOTOR_CONTROL_S *pmotor_s,MOTOR_CONTROL_SPTA *pmotor_spta,unsigned int dest)
{ 
	//MOTOR_CONTROL_S *pmotor_s=NULL; 
	//MOTOR_CONTROL_SPTA *pmotor_spta=NULL; 
	/*
	switch(MotorID)
	{
		case 1:
			pmotor_s=&motor1; 
			break;
		case 2:
			pmotor_s=&motor2; 
			break;
		case 3:
			pmotor_s=&motor3; 
			break;
		case 4:
			pmotor_spta=&motor4; 
			break;
		default:
			return;
	}
	*/
	if(pmotor_s!=NULL){
		if(dest<=pmotor_s->MaxPosition&&dest!=pmotor_s->CurrentPosition)
		{
			if(dest>pmotor_s->CurrentPosition)
			{
				Start_Motor_S(MotorID,pmotor_s,pmotor_s->clockwise,dest-pmotor_s->CurrentPosition); 
			}
			else
			{
				Start_Motor_S(MotorID,pmotor_s,!pmotor_s->clockwise,pmotor_s->CurrentPosition-dest);  	
			}		 		 
			while(pmotor_s->running==1);
		}
	}
	else if(pmotor_spta!=NULL)
	{
		if(dest<=pmotor_spta->MaxPosition&&dest!=pmotor_spta->CurrentPosition)
		{
			if(dest>pmotor_spta->CurrentPosition)
			{
				Start_Motor_SPTA(MotorID,pmotor_spta,pmotor_spta->clockwise,dest-pmotor_spta->CurrentPosition); 
			}
			else
			{
				Start_Motor_SPTA(MotorID,pmotor_spta,!pmotor_spta->clockwise,pmotor_spta->CurrentPosition-dest);  	
			}		 		 
			while(pmotor_spta->running==1);
		}
	}
}


/*复位电机*/
void Do_Reset(unsigned char MotorID,MOTOR_CONTROL_S *pmotor_s,MOTOR_CONTROL_SPTA *pmotor_spta)
{
	//MOTOR_CONTROL_S *pmotor_s=NULL;
	//MOTOR_CONTROL_SPTA *pmotor_spta=NULL;
	/*
	switch(MotorID)
	{
		case 1:
			pmotor_s=&motor1;
			break;
		case 2:
			pmotor_s=&motor2;
			break;
		case 3:
			pmotor_s=&motor3;
			break;
		case 4:
			pmotor_spta=&motor4;
			break;
		default:
			return;
	} 
	*/
	/*do reset*/
	if(pmotor_s!=NULL){
		pmotor_s->rstflg=1;
		pmotor_s->running=1;
		SetSpeed(MotorID,pmotor_s,pmotor_spta,16);
		while(pmotor_s->running==1);
	}
	if(pmotor_spta!=NULL){
		pmotor_spta->rstflg=1;
		pmotor_spta->running=1;
		SetSpeed(MotorID,pmotor_s,pmotor_spta,16);
		while(pmotor_spta->running==1);
	}
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
 
 /*电机S型曲线算法公共处理函数*/
void TIMX_UP_IRQHandler_S(MOTOR_CONTROL_S* pmotor)
{   
	if(1==pmotor->en)
	{ 
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
		
		//速度控制
		if(pmotor->speedenbale&&(pmotor->CurrentIndex==pmotor->TargetIndex||pmotor->TargetIndex+pmotor->CurrentIndex==pmotor->StartTableLength+pmotor->StopTableLength-1))
		{
			return;
		}
		pmotor->PulsesHaven++; //总脉冲个数
		pmotor->pulsecount++;  //以该频率脉冲输出的脉冲个数 
		
		//对称反转
		if(pmotor->RevetDot==pmotor->PulsesHaven)
		{
			pmotor->pulsecount=pmotor->Step_Table[pmotor->CurrentIndex];
		}
		if(pmotor->pulsecount>=pmotor->Step_Table[pmotor->CurrentIndex])
		{ 
			if(pmotor->PulsesHaven<=pmotor->StartSteps)
			{
				//起步阶段
				if(pmotor->CurrentIndex<pmotor->StartTableLength-1)
				{
					pmotor->CurrentIndex++;
					pmotor->pulsecount=0;
					if(pmotor->CurrentIndex>=pmotor->StartTableLength)pmotor->CurrentIndex=pmotor->StartTableLength;
				}
			}
			//对于速度控制，此处不能判断pmotor->PulsesHaven>=(pmotor->PulsesGiven>>1)
			//if(pmotor->PulsesGiven-pmotor->PulsesHaven<=pmotor->StopSteps&&pmotor->PulsesHaven>=(pmotor->PulsesGiven>>1))
			if((pmotor->PulsesGiven-pmotor->PulsesHaven<=pmotor->StopSteps&&pmotor->speedenbale==1)||
				(pmotor->PulsesGiven-pmotor->PulsesHaven<=pmotor->StopSteps&&pmotor->speedenbale==0&&pmotor->PulsesHaven>=(pmotor->PulsesGiven>>1))) 
			{
				//停止阶段
				if(pmotor->CurrentIndex<pmotor->StartTableLength-1)
				{
					pmotor->CurrentIndex=pmotor->StartTableLength+pmotor->StopTableLength-pmotor->CurrentIndex;  
				}
				pmotor->CurrentIndex++;
				pmotor->pulsecount=0;
				if(pmotor->CurrentIndex>=pmotor->StartTableLength+pmotor->StopTableLength)
					pmotor->CurrentIndex=pmotor->StartTableLength+pmotor->StopTableLength-1;
			}  		
			pmotor->TIMx->ARR = pmotor->Counter_Table[pmotor->CurrentIndex] ; //设置周期
			pmotor->TIMx->CCR1 =( pmotor->Counter_Table[pmotor->CurrentIndex])>>1;       //设置占空比	    
		}	  
		//旋转预定脉冲数，停止，running=0，可以进行下一次旋转
		if(pmotor->PulsesHaven>=pmotor->PulsesGiven&&pmotor->PulsesHaven>3)
		{
			pmotor->en=0;
			pmotor->running=0;
			pmotor->CurrentIndex=0;
			TIM_Cmd(pmotor->TIMx, DISABLE);		  //DISABLE 	
			USART1_Printf("1\r\n");
#ifdef OUTPUT_DATA
			timecnt=0;		
#endif			
		}
		else
		{			
			pmotor->Time_Cost_Act+=pmotor->TIMx->ARR;
		}
	}
#ifdef OUTPUT_DATA
	
	//每个时刻对应的频率
	//sprintf(tmp,"%f\t%f\r\n",timecnt,1.0/TIM1->CCR1);
	
	//每个时刻对应的高电平与低电平
	sprintf(tmp,"%f,0\r\n%f,1\r\n",timecnt,timecnt+pmotor->TIMx->CCR1);
	
	USART1_Printf(tmp);
	timecnt+=pmotor->TIMx->ARR;
#endif
}

void MOTORSTOP_IRQ(MOTOR_CONTROL_S *pmotor_s,MOTOR_CONTROL_SPTA *pmotor_spta)
{
	if(pmotor_s!=NULL)
	{
		if(pmotor_s->rstflg==1)
		{
			pmotor_s->speedenbale=0;
			pmotor_s->PulsesHaven=pmotor_s->PulsesGiven-128;	
			pmotor_s->dir=M2_UNCLOCKWISE;
			pmotor_s->CurrentPosition_Pulse=128;		
			pmotor_s->rstflg=0;
		} 
	}
	if(pmotor_spta!=NULL)
	{
		if(pmotor_spta->rstflg==1)
		{
			pmotor_spta->speedenbale=0;
			pmotor_spta->PulsesHaven=pmotor_spta->PulsesGiven-128;	
			pmotor_spta->dir=M4_UNCLOCKWISE;
			pmotor_spta->CurrentPosition_Pulse=128;		
			pmotor_spta->rstflg=0;
		} 
	}
}