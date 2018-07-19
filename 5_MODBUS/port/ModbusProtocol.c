#include "ModbusProtocol.h"
/*
标准使用方法
int main(void)
{
	
	eMBInit(MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE); 
	eMBEnable(); 
  while (1)
  {

		(void)eMBPoll();
		
	}

}


*/

//该协议第0位不读，从第1位开始读写
/* ----------------------- Static variables ---------------------------------*/
USHORT   usRegCoilsStart = REG_COILS_START;
																							 //取数据方向      <----    <----
UCHAR    usRegCoilsBuf[REG_COILS_NREGS] 			= {0x11,0x34}; //0001 0001 0011 0100

USHORT   usRegDiscreteStart = REG_DISCRETE_START;
																							 //取数据方向      <----    <----
UCHAR    usRegDiscreteBuf[REG_DISCRETE_NREGS] = {0x12,0x00}; //0001 0010 0000 0000

USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS]				= {0xddfd,0xedad,0x000d,0x0356,0x23da,0xffff,0x333b,0x4444};

USHORT   usRegHoldingStart = REG_HOLDING_START;
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS] 	= {0x147b,0x3f8e,0x147e,0x400e,0x1eb8,0x4055,0x147b,0x408e};



/****************************************************************************
* 名	  称：eMBRegInputCB 
* 功    能：读取输入寄存器，对应功能码是 04 eMBFuncReadInputRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读取的寄存器个数
* 出口参数：
* 注	  意：上位机发来的 帧格式是: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*							3 区
****************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart - 1 );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegHoldingCB 
* 功    能：对应功能码有：06 写保持寄存器 eMBFuncWriteHoldingRegister 
*													16 写多个保持寄存器 eMBFuncWriteMultipleHoldingRegister
*													03 读保持寄存器 eMBFuncReadHoldingRegister
*													23 读写多个保持寄存器 eMBFuncReadWriteMultipleHoldingRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读写的寄存器个数
*						eMode: 功能码
* 出口参数：
* 注	  意：4 区
****************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;


	if((usAddress >= REG_HOLDING_START)&&\
		((usAddress+usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{ //此处必须减去1
		iRegIndex = (int)(usAddress - usRegHoldingStart - 1);
		switch(eMode)
		{                                       
			case MB_REG_READ://读 MB_REG_READ = 0
        while(usNRegs > 0)
				{
					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);     //先读高位        
					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF);   //再读低位
          iRegIndex++;
          usNRegs--;					
				}                            
        break;
			case MB_REG_WRITE://写 MB_REG_WRITE = 0
				while(usNRegs > 0)
				{         
					usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNRegs--;
        }				
			}
	}
	else//错误
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegCoilsCB 
* 功    能：对应功能码有：01 读线圈 eMBFuncReadCoils
*													05 写线圈 eMBFuncWriteCoil
*													15 写多个线圈 eMBFuncWriteMultipleCoils
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 线圈地址
*						usNCoils: 要读写的线圈个数
*						eMode: 功能码
* 出口参数：
* 注	  意：如继电器 
*						0 区
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	//错误状态
	eMBErrorCode    eStatus = MB_ENOERR;
	//寄存器个数
	int16_t iNCoils = (int16_t)usNCoils;
	//寄存器偏移量
	int             iRegIndex;

	//检测寄存器是否在指定范围内
	if((usAddress >= REG_COILS_START)&&\
		((usAddress+usNCoils) <= (REG_COILS_START + REG_COILS_NREGS)))
	{
		//为适应单片机地址从0位开始，此处必须减去1
		iRegIndex = (int)(usAddress - usRegCoilsStart - 1);
		switch(eMode)
		{                                       
			case MB_REG_READ://读 MB_REG_READ = 0
        while(iNCoils > 0)
				{	
						*pucRegBuffer++ = xMBUtilGetBits( usRegCoilsBuf, iRegIndex,( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
						iNCoils -= 8;
						iRegIndex += 8;
				}                            
        break;
			case MB_REG_WRITE://写 MB_REG_WRITE = 1
				while(iNCoils > 0)
				{         
						xMBUtilSetBits( usRegCoilsBuf, iRegIndex,( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) ,*pucRegBuffer++);
						iNCoils -= 8;
        }				
			}
	}
	else//错误
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}
/****************************************************************************
* 名	  称：eMBRegDiscreteCB 
* 功    能：读取离散寄存器，对应功能码有：02 读离散寄存器 eMBFuncReadDiscreteInputs
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNDiscrete: 要读取的寄存器个数
* 出口参数：
* 注	  意：1 区
****************************************************************************/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	int16_t iNDisctete = (int16_t)usNDiscrete;


	if((usAddress >= REG_COILS_START)&&\
		((usAddress+usNDiscrete) <= (REG_DISCRETE_START + REG_DISCRETE_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegDiscreteStart - 1);

    while(iNDisctete > 0)
	 {	
			*pucRegBuffer++ = xMBUtilGetBits( usRegDiscreteBuf, iRegIndex,( uint8_t )( iNDisctete > 8 ? 8 : iNDisctete ) );
			iNDisctete -= 8;
			iRegIndex += 8;
	 }                                
	}
	else//错误
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}

