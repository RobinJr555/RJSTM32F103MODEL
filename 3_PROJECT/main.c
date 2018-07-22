/*******************************************************************************
 * @name     : 
 * @author   : RobinJR.
 * @version  : V1.0.0
 * @firstedit: 
 * @lastedit : 
 * @brief    : 
 * ---------------------------------------------------------------------------- 
 * @copyright
  
 * ----------------------------------------------------------------------------
 * @description

 *
 *-----------------------------------------------------------------------------
 ******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "CAN.h"

#include <stdio.h>
#include "string.h"

//#include "ModbusProtocol.h"


/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  Init Peripherals.
  * @param  None
  * @retval None
  */
	void Peripherals_Init(void)
{
	LED_Init();
	COM_Init(COM1, 9600);
	MYCAN_Init();
}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{       
	u8 canbuf[8] = "HELLO!!!";
	Peripherals_Init();
	LED_On();
  while (1)
  {
		Can_Send_Msg(canbuf,8);
		LED_Toggle();
		delay_ms(1000);
		delay_ms(1000);
	}

}

