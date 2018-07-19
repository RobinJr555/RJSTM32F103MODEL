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

#include <stdio.h>
#include "string.h"

#include "ModbusProtocol.h"



extern USHORT   usRegInputStart;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
extern USHORT   usRegHoldingStart;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  Init Peripherals.
  * @param  None
  * @retval None
  */
	void Peripherals_Init(void)
{
	LED_Init();
	eMBInit(MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE); 
	eMBEnable(); 

}
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
	Peripherals_Init();
	LED_On();
  while (1)
  {

		(void)eMBPoll();
		
	}

}

