/*******************************************************************************
 * @name    : The head of CAN function
 * @author  : RobinJR.
 * @version : V1.0
 * @date    : 2018-7-21
 * @brief   : CAN BUS DRIVE
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

#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f10x.h"

void MYCAN_Init(void);

void CAN_GPIO_Init(void);
void CAN_NVIC_Configuration(void);
void CAN_MODE_INIT(void);

u8 Can_Send_Msg(u8* msg,u8 len);
u8 Can_Receive_Msg(u8 *buf);

#endif