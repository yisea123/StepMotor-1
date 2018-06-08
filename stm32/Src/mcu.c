#include "mcu.h"

//初始化滴答定时器
void Delay_Init(void)
{
    time.msPeriod = 0;
    time.ticksPerUs = SystemCoreClock / 1e6;
    time.ticksPerMs = SystemCoreClock / 1e3;
    time.msPerPeriod = 10;
    SysTick_Config(SystemCoreClock/(1000/time.msPerPeriod));//初始化滴答定时器，使能滴答定时器中断
}

// SysTick中断。
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
}

//获取当前时间，us。
uint64_t Time_Nowus(void)
{
    return time.msPeriod * (uint64_t)1000 + (SysTick->LOAD - SysTick->VAL) / time.ticksPerUs;
}
//获取当前时间，ms。
uint32_t Time_Nowms(void)
{
    return time.msPeriod + (SysTick->LOAD - SysTick->VAL) / time.ticksPerMs;
}

//延时nus
//nus为要延时的us数.		    								   
void Delay_Us(uint32_t nus)
{		
    volatile uint64_t target = Time_Nowus() + nus - 2;
    while(Time_Nowus() <= target)
    {;} // 空循环
}
// 延时delay ms
void Delay_Ms(uint16_t nms)
{	 		  	  
	Delay_Us(nms * 1000); 	    
}

/*系统时钟初始化*/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	//RCC system reset(for debug purpose) //
	RCC_DeInit();
	//RCC_HSICmd(ENABLE);					//使能内部RC振荡器

    //Enable HSE //
	RCC_HSEConfig(RCC_HSE_ON);

	//Wait till HSE is ready
 	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
    // Enable Prefetch Buffer
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    //Flash 2 wait state
    FLASH_SetLatency(FLASH_Latency_2);

    //HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    //PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);

    //PCLK1 = HCLK/2
    RCC_PCLK1Config(RCC_HCLK_Div2);

    //PLLCLK = 8MHz/2 * 18 = 72 MHz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    //Enable PLL
    RCC_PLLCmd(ENABLE);

    //Wait till PLL is ready
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    //Select PLL as system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // Wait till PLL is used as system clock source
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
	}
}


void MY_MCU_Init(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //保留下载功能
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); //配置ADC时钟，为PCLK2的8分频，即9mHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
    
    
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
    
    //
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
   
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
     /*
    //key
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 
    */
    

    
}

void MY_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
     
    //LED 引脚配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 
    GPIO_SetBits(GPIOC,GPIO_InitStructure.GPIO_Pin);	

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  GPIOA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	/*	     
 	//配置PB10 PB11 为开漏输出  刷新频率为10Mhz
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	//配置PB6 PB7 为开漏输出  刷新频率为10Mhz
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //PA8 TIM1_CH1 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //PB13 TIM1_CH1N
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //PB6 PB7   TIM4_CH1 TIM4_CH2 输入捕获
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    */
    
    /*
    //PA0 TIM2_CH1 LeftStepMoter_CLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //PA1 LeftStepMoter_DIR PA2 LeftStepMotor_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_2);
     GPIO_SetBits(GPIOA,GPIO_Pin_2);
    
    //PA6 TIM3_CH1 RightStepMotor_CLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //PA5 RightStepMotor_DIR PA4 RightStepMotor_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    
    /*
    //TIM4_CH4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
    
    //ADC GPIOA.1 GPIOA.2 GPIOA.3 adc1 adc2 adc3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //key
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
    */


}

//USART初始化配置
void USART1_Init(uint32_t bound)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure); 
    
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//接收中断
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//空闲中断
    //USART_ITConfig(USART1, USART_IT_WU, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
    USART_Cmd(USART1, ENABLE); 
    USART_GetFlagStatus(USART1,USART_FLAG_TC);	  //解决上电后第一个字节丢失问题
    
    DMA1_Channel5_Configuration();
}

void DMA1_Channel4_Configuration(uint32_t u32SrcAddress, uint32_t u32BufferSize)
{
    DMA_InitTypeDef          DMA_InitStructure;
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = u32SrcAddress; 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = u32BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); //使能DMA传输完成中断
    DMA_Cmd(DMA1_Channel4, ENABLE); //启动DMA通道
}


void DMA1_Channel5_Configuration(void)
{
	DMA_InitTypeDef          DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel5);                                          //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;   //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;   //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = USART_RECV_SIZE;                 //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);                        //根据DMA_InitStruct中指定的参数初始化DMA的通道
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);                     //使能DMA 传输完成中断
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);                     //使能 DMA 异常中断
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); 
	DMA_Cmd(DMA1_Channel5, ENABLE); //启动DMA通道
}

void TIM1_Init(uint32_t u32Prescaler, uint32_t u32Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period = u32Period;                 //自动装载计数器周期值
	TIM_TimeBaseStructure.TIM_Prescaler = u32Prescaler;           //预分频值	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //时钟分隔 TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //初始化

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = u32Period/2-1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
    TIM_SetCompare1(TIM1,u32Period/2-1);
}

void TIM2_Init(uint32_t u32Prescaler, uint32_t u32Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period = u32Period;                 //自动装载计数器周期值
	TIM_TimeBaseStructure.TIM_Prescaler = u32Prescaler;           //预分频值	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //时钟分隔
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);               //初始化

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_Pulse = u32Period/2;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
    //TIM_SetCompare1(TIM2,u32Period/2);
}

void TIM3_Init(uint32_t u32Prescaler, uint32_t u32Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /*
	TIM_TimeBaseStructure.TIM_Period = u32Period;                 //自动装载计数器周期值
	TIM_TimeBaseStructure.TIM_Prescaler = u32Prescaler;           //预分频值	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //时钟分隔
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //初始化

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_Pulse = u32Period/2;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
    //TIM_SetCompare1(TIM3,u32Period/2);
    */
    
    TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=u32Period;
	TIM_TimeBaseStructure.TIM_Prescaler= u32Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Init(uint32_t u32Prescaler, uint32_t u32Period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    /*
    TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period=u32Period;
	TIM_TimeBaseStructure.TIM_Prescaler= u32Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);	
    */
    /*
    TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period=4*ENCODER_PPR-1;
	TIM_TimeBaseStructure.TIM_Prescaler= 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_ClockDivision=0; //TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6; //6<-> 670nsec
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
    // Clear all pending interrupts
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    //Reset counter
    //TIM2->CNT = COUNTER_RESET;

    //ENC_Clear_Speed_Buffer();
    TIM_Cmd(TIM4, ENABLE);	 
    //TIM_Cmd(TIM4, DISABLE);
    */
    
    TIM_DeInit(TIM4);
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
}



/* 函数名：ADC1_Init
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
void ADC1_Init(void)
{
	
	ADC_InitTypeDef ADC_InitStructure;

	/* ADC1 configuration */
	ADC_DeInit(ADC1);												//将外设ADC1的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;				//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				//模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;				//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 			//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = KEY_COUNT; 	 			//要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure);								//初始化
	 
	//设置指定ADC的规则组通道，设置他们的转化顺序和采样时间  	//ADC1 ADC2 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1,1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2,2, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3,3, ADC_SampleTime_1Cycles5);

	
	ADC_DMACmd(ADC1, ENABLE);/* Enable ADC1 DMA */
	ADC_Cmd(ADC1, ENABLE);/* Enable ADC1 */
	
	ADC_ResetCalibration(ADC1);/*复位校准寄存器 */   
	while(ADC_GetResetCalibrationStatus(ADC1));	/*等待校准寄存器复位完成 */
	ADC_StartCalibration(ADC1);	/* ADC校准 */
	while(ADC_GetCalibrationStatus(ADC1));	/* 等待校准完成*/
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	DMA_Cmd(DMA1_Channel1, ENABLE);		//启动DMA通道
}

//DMA channel1 configuration 
void DMA1_Channel1_Configuration(uint32_t u32SrcAddress, uint32_t u32BufferSize)
{
    DMA_InitTypeDef     DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);										    //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;		//DMA外设ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)u32SrcAddress;	    //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				    //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = u32BufferSize;					//DMA通道的DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					    //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;				    //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					    //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);					    //根据DMA_InitStructure中指定的参数初始化DMA通道
	DMA_Cmd(DMA1_Channel1, ENABLE);									    //启动DMA通道
}

void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 

  	
}
