#ifndef _ADC_H_
#define _ADC_H_
//此处为ADC初始化文件
#include "stm32f10x.h"




void ADC_Initialize(void);
void ADC_GPIO_Init(void);
void ADC_Channel_Initialize(void);

uint16_t Get_Adc(ADC_TypeDef* ADCx, uint8_t ch);
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx, uint8_t ch);
float Get_Voltage(u16);//将得到的ADC值转化为实际电压 3.3V参考电压值

#endif