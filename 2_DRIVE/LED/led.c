#include "led.h"

/**
  * @brief  LED IO初始化
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* 使能GPIOB时钟 */
  RCC_APB2PeriphClockCmd(LEDCLOCK, ENABLE);

  /* 配置GPIOB.12为推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin = LEDPIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LEDPORT, &GPIO_InitStructure);
	
	GPIO_SetBits(LEDPORT, LEDPIN);//设置GPIOB.12为高电平，关闭LED
}

/**
  * @brief  点亮LED指示灯
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_On(void)
{
	LED = 0;
}

/**
  * @brief  关闭LED指示灯
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Off(void)
{
	LED = 1;
}

/**
  * @brief  LED指示灯反转
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Toggle(void)
{
	LED = !LED;
}