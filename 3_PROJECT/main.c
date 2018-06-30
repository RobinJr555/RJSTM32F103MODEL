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
#include "DHTxx.h"
#include "led.h"
#include "usart.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
u8 DHT21data[4]={0x00,0x00,0x00,0x00};
float humidity = 0;
float temperature = 0;
int main(void)
{
	LED_Init();
	Delay_Init();
	DHTxx_Init();
	COM_Init(COM1,9600);
	delay_ms(2000);
	delay_ms(1000);
  while (1)
  {
	
		if(ReadDHTxx(DHT11,DHT21data) == DHTxx_ERROR)
		{
			delay_ms(200);
			LED_Toggle();
			printf("DHTxx_ERROR\r\n");
		}
		else
		{
			delay_ms(2000);
			delay_ms(1000);
			LED_Toggle();
			printf("DHTxx_SUCCESS\r\n");
			humidity = Get_DHT11_Humidity(DHT21data[0],DHT21data[1]);
			temperature = Get_DHT11_Humidity(DHT21data[2],DHT21data[3]);
			printf("Êª¶ÈHumidity:%0.1f% \r\n",humidity);
			printf("temperature:%0.1f¡æ \r\n",temperature);
		}

	}

}

/********************* (C) COPYRIGHT 2018 ROBINJR. **********END OF FILE**********/
