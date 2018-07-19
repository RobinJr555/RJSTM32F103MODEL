#ifndef __MODBUSPROTOCOL_H
#define __MODBUSPROTOCOL_H
#include "stm32f10x.h"
//存放FREEMODBUS 四组寄存器操作源码
//用到了串口1 及 定时器4的更新中断
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
//此处的寄存器数量需要设置的比实际大1
#define REG_COILS_START 				0X0000
#define REG_COILS_NREGS 				17

#define REG_DISCRETE_START 			0X0000
#define REG_DISCRETE_NREGS 			17

#define REG_INPUT_START 				0x0000
#define REG_INPUT_NREGS 				9

#define REG_HOLDING_START 			0x0000
#define REG_HOLDING_NREGS 			9


#endif