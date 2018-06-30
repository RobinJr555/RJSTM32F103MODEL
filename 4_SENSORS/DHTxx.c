#include "DHTxx.h"

void DHTxx_Init(void)
{
	OW_Init();
}

/**
  * @brief  读取DHTxx数据 40位数据
  * @param  
*     @arg sensortype :DHT21 DHT11 (指定不同的启动延时时间）
  *     @arg data 储存40位数据的指针
  * @retval 是否读取成功
  * @note None
  */
DHTxx_StatusTypeDef ReadDHTxx(u8 sensortype,u8* data)
{
	u8 sum;
	if(OW_START(sensortype) == OW_SUCCESS)
	{
		*data  		= OW_ReadByte();
		*(data+1) = OW_ReadByte();
		*(data+2) = OW_ReadByte();
		*(data+3) = OW_ReadByte();
		*(data+4) = OW_ReadByte();
	
		sum = *data + *(data+1) + *(data+2) + *(data+3);
	}
	
	if(sum == *(data+4))
		return DHTxx_SUCCESS;
	else 
		return DHTxx_ERROR;
	
}
//获取湿度
float Get_DHT21_Humidity(u8 Hdata,u8 Ldata)
{
	return 1.0*((Hdata << 8) | Ldata)/10;

}
//获取温度
float Get_DHT21_Temperature(u8 Hdata,u8 Ldata)
{
	return 1.0*((Hdata << 8) | Ldata)/10;
}
//获取湿度

float Get_DHT11_Humidity(u8 Hdata,u8 Ldata)
{
	return Hdata;

}
//获取温度
float Get_DHT11_Temperature(u8 Hdata,u8 Ldata)
{
	return Hdata;
}
