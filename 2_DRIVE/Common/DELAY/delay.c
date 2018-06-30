#include "delay.h"

/**
  * @brief  设置SysTick计数时钟为HCLK/8
  * @param  None
  * @retval None
	* @note   
	*    当HCLK为72MHz时，最长延迟时间为0xFFFFFF/9000000 (S) = 1864ms
  */
void Delay_Init(void)
{
	/* 设置SysTick的计数时钟频率为HCLK/8，如果HCLK是72MHz，则SysTick计数时钟是9MHz*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**
  * @brief  ms延时函数
	* @param  xms: 延时的毫秒数
  * @retval None
	* @note   
	*    通过下面的公式来改变SysTick的中断时间:
  *                          
  *    重载值(Reload Value) = SysTick计数时钟频率 (Hz) x  希望中断的时间间隔 (s)
  *  
  *    - 重载值作为SysTick_Config()的参数传递 
	*    - 重载值不允许超过 0xFFFFFF，因此最长可以延时1864ms
  */
void delay_ms(uint16_t xms)
{
	uint32_t reload;
	
	reload = SystemCoreClock/8000;//延时1ms的重载值
	reload *= xms;//延时xms的重载值。
	SysTick->LOAD = (reload & 0xFFFFFF) - 1;//加载SysTick重载值
	SysTick->VAL = 0;//计数值清零
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始倒数计数
	while(!((SysTick->CTRL) & (1 << 16)));//等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭滴答计数器
	SysTick->VAL = 0x00;//清空计数器
}

/**
  * @brief  us延时函数
	* @param  xus: 延时的微秒数
  * @retval None
	* @note   
	*		通过下面的公式来改变SysTick的中断时间:
  *                          
  *   重载值(Reload Value) = SysTick计数时钟频率 (Hz) x  希望中断的时间间隔 (s)
  *  
  *   - 重载值作为SysTick_Config()的参数传递
	*		- 重载值不允许超过 0xFFFFFF，因此最长可以延时1864ms
  */
void delay_us(uint32_t xus)
{
	uint32_t reload;
	
	reload = SystemCoreClock/8000000;//延时1us的重载值
	reload *= xus;//延时xus的重载值。
	SysTick->LOAD = (reload & 0xFFFFFF) - 1;//加载SysTick重载值
	SysTick->VAL = 0;//计数值清零
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始倒数计数
	while(!((SysTick->CTRL) & (1 << 16)));//等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭滴答计数器
	SysTick->VAL = 0x00;//清空计数器
}

/********************* (C) COPYRIGHT 2018 ROBINJR. **********END OF FILE**********/
