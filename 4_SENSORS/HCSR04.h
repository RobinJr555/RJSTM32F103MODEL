#ifndef _HCSR04_H_
#define _HCSR04_H_
//HCSR04需5.0V电压供电，若使用3.3V供电则不能正常工作，原因未知。
#include "stm32f10x.h"
#include "delay.h"
#include "bitband.h"

#define HCSR04_PORT     	GPIOB
#define HCSR04_CLK      	RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     	GPIO_Pin_5
#define HCSR04_TRIG_NUM     	5
#define HCSR04_ECHO     	GPIO_Pin_6
#define HCSR04_ECHO_NUM 			6

#define TRIG_Send		GPIOout(HCSR04_PORT,HCSR04_TRIG_NUM)	//定义PB12位带操作
#define ECHO_Reci		GPIOin(HCSR04_PORT,HCSR04_ECHO_NUM)	  //定义PB12位带操作

void Hcsr04Init(void);
void Hcsr04_Ranging(float *p);

#endif
