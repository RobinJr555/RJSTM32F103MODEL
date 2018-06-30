/*******************************************************************************
 * @name    : The head of delay function
 * @author  : RobinJR.
 * @version : V1.0
 * @date    : 2018-6-22
 * @brief   : delay for us and ms time,which can be the base used for other function.
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

#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"


void Delay_Init(void);	//SysTick延时初始化
void delay_ms(uint16_t xms);//毫秒延时函数
void delay_us(uint32_t xus);//微秒延时函数

#endif

/********************* (C) COPYRIGHT 2018 ROBINJR. **********END OF FILE**********/
