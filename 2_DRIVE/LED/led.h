/*******************************************************************************
 * @name    : The head of led
 * @author  : RobinJR.
 * @version : V1.0
 * @date    : 2018-6-26
 * @brief   : General led file
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * ----------------------------------------------------------------------------
 * @description

 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------

 ******************************************************************************/


#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#include "bitband.h"	//使用位带操作，一定要包含此头文件

#define LEDCLOCK     RCC_APB2Periph_GPIOC
#define LEDPORT 		 GPIOC
#define LEDPIN 			 GPIO_Pin_13
#define LEDPINNUM  	 13


#define LED		GPIOout(LEDPORT, LEDPINNUM)	//定义PB12位带操作

void LED_Init(void);	//LED IO初始化
void LED_On(void);		//LED亮
void LED_Off(void);		//LED灭0
void LED_Toggle(void);//LED状态反转

#endif

