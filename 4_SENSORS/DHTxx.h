/*******************************************************************************
 * @name    : The head of DHTxx driver file
 * @author  : RobinJR.
 * @version : V1.0
 * @date    : 2018-6-26
 * @brief   : DHT11 and DHT21(AM2301)
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
#ifndef __DHTxx_H
#define __DHTxx_H
#include "stm32f10x.h"
#include "OneWire.h"

#define DHT21 1
#define DHT11 20

typedef enum
{
	DHTxx_SUCCESS = 0,
	DHTxx_ERROR,
}DHTxx_StatusTypeDef;

void DHTxx_Init(void);
DHTxx_StatusTypeDef ReadDHTxx(u8 sensortype,u8* data); //read data

float Get_DHT21_Humidity(u8 Hdata,u8 Ldata);
float Get_DHT21_Temperature(u8 Hdata,u8 Ldata);

float Get_DHT11_Humidity(u8 Hdata,u8 Ldata);
float Get_DHT11_Temperature(u8 Hdata,u8 Ldata);

#endif