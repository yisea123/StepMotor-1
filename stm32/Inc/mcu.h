#ifndef __MCU_H
#define __MCU_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "driver.h"


/* Private define ------------------------------------------------------------*/

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


//系统
static struct Time
{
    volatile uint32_t msPeriod;	// 整周期的时间，ms。
    uint32_t ticksPerUs;  		// 每us等于的滴答次数
    uint32_t ticksPerMs;  		// 每ms等于的滴答次数
    uint32_t msPerPeriod; 		// 每周期的ms数
}time;

#define Loop_Begin(time_ms) \
		{\
			static uint32_t target = 0;\
			if(target <= Time_Nowms())\
			{\
				target = Time_Nowms() + time_ms;
#define Loop_End() \
			}\
		}

        
//用户
#define LED1        PCout(13)
#define KEY0        PBin(10)
#define KEY_COUNT   3   
#define KEY_BUFFER  20            
typedef struct {
    int16_t key_left; 
    int16_t key_right;
    int16_t key_speed;
} adc_result_t;  

extern volatile int16_t AD_Value[KEY_BUFFER][KEY_COUNT]; 
extern volatile adc_result_t  adc_result;
        
void Delay_Init(void);
void Delay_Ms(uint16_t nms);
void Delay_Us(uint32_t nus);
		
uint64_t Time_Nowus(void);
uint32_t Time_Nowms(void);
       
void RCC_Configuration(void);        
void MY_MCU_Init(void);
void MY_GPIO_Init(void);
        
void USART1_Init(uint32_t bound);
void DMA1_Channel4_Configuration(uint32_t u32SrcAddress, uint32_t u32BufferSize);
void DMA1_Channel5_Configuration(void);  
void TIM1_Init(uint32_t u32Prescaler, uint32_t u32Period);
void TIM2_Init(uint32_t u32Prescaler, uint32_t u32Period);
void TIM3_Init(uint32_t u32Prescaler, uint32_t u32Period);
void TIM4_Init(uint32_t u32Prescaler, uint32_t u32Period);

void ADC1_Init(void);
void DMA1_Channel1_Configuration(uint32_t u32SrcAddress, uint32_t u32BufferSize);        
void EXTIX_Init(void);

#endif /* __MCU_H */
/******************* (C) COPYRIGHT 2016 LinCongCong*****END OF FILE************/
