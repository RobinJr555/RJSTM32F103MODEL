#include "OneWire.h"

void OW_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		//使能外设时钟
		RCC_APB2PeriphClockCmd(OWCLOCK, ENABLE );	
		
		//单总线需要在硬件上连接上拉电阻，因此这里设置为开漏输出
		GPIO_InitStructure.GPIO_Pin = OWPIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(OWPORT, &GPIO_InitStructure);
}

void OW_SDA_IN(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = OWPIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     
    GPIO_Init(OWPORT,&GPIO_InitStructure);
}

void OW_SDA_OUT(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = OWPIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_Init(OWPORT,&GPIO_InitStructure);
}


OW_StatusTypeDef OW_START(u8 ms)			//起始信号
{
		OW_SDA_OUT();
		//拉低总线
		OW_SDA = 0;
		delay_ms(ms);
	
		//等待响应 （拉低一段时间，再拉高一段时间）
		OW_SDA = 1;
		delay_us(20);
	  OW_SDA_IN();
		
		//响应信号为0则等待拉高，为1则读取失败
		if(!READ_OW_SDA) 
		{
				while(!READ_OW_SDA); //拉低中……
				while(READ_OW_SDA);  //拉高中……
				return OW_SUCCESS; //成功
		}
		
		return OW_ERROR; //失败
}

//读取一字节数据
u8 OW_ReadByte(void)
{
		u8 i;
		u8 sbuf = 0;
		for(i = 0; i < 8; i++)
				{
						sbuf <<= 1; //MSB first
						delay_us(20);
						while(!READ_OW_SDA);// 50us 低电平信号
						delay_us(30);				// 26us 高电平信号
						if(READ_OW_SDA)			// bit = 1
						{
								sbuf |= 0x01;
								delay_us(30);
						}
						else // bit = 0;
						{
								sbuf &= 0xfe;
						
						}
				}
		return sbuf;

}

