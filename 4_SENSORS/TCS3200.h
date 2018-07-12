#ifndef _TCS3200_H_
#define _TCS3200_H_
//TCS3200颜色识别模块
#include "stm32f10x.h"
#include "bitband.h"
#include "delay.h"

//输出引脚配置
#define TCS_PORT     			GPIOA
#define TCS_CLK      			RCC_APB2Periph_GPIOA
#define TCS_S0_PIN     		GPIO_Pin_1
#define TCS_S1_PIN     		GPIO_Pin_2
#define TCS_S2_PIN     		GPIO_Pin_3
#define TCS_S3_PIN     		GPIO_Pin_4
#define LED_CON_PIN       GPIO_Pin_6

//输入引脚配置及外部中断线配置
#define TCS_OUT_PIN     	 GPIO_Pin_5
#define TCS_OUT_PORTSOURCE GPIO_PortSourceGPIOA
#define TCS_OUT_PINSOURCE  GPIO_PinSource5
#define TCS_EXTI_LINE  		 EXTI_Line5


#define S0						GPIOout(TCS_PORT,1)
#define S1						GPIOout(TCS_PORT,2)
#define S2						GPIOout(TCS_PORT,3)
#define S3						GPIOout(TCS_PORT,4)
#define TCSDATA				GPIOin(TCS_PORT,5)
#define TCSLED				GPIOout(TCS_PORT,6)

#define ALLTIME  10 //曝光时间

void TCS3200_Init(void);
void TCS3200_GPIOinit(void);
void TCS3200EXIT_init(void);//外部中断

//白平衡最好的值 205，195，205
void SetTCSMode(u8 s0,u8 s1);
void SetWhiteBalance(void);
void whitebalance(void);//白平衡设置

u16 GetTcs3200_RED(void);
u16 GetTcs3200_GREEN(void);
u16 GetTcs3200_BLUE(void);
void GetTcs3200_VALUE(u16* Get_Value);


#endif
