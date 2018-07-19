#include "usart.h"

/******************************* @printf  *************************************/

#ifdef PRINTF_SUPPORT //以下代码,支持printf函数,而不需要选择use MicroLIB
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式       
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(PRINTF_SUPPORT, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(PRINTF_SUPPORT, USART_FLAG_TC) == RESET)
  {}    
	return ch;
}

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch) 
{ 
ch = ch; 
} 
#endif
/********************************* @end ***************************************/

/**
  * @brief 定义串口相关PORT，CLK等
  */
USART_TypeDef* COM_USART[COMn] = {UCORTEX_COM1, UCORTEX_COM2, UCORTEX_COM3}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {UCORTEX_COM1_TX_GPIO_PORT, UCORTEX_COM2_TX_GPIO_PORT, UCORTEX_COM3_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {UCORTEX_COM1_RX_GPIO_PORT, UCORTEX_COM2_RX_GPIO_PORT, UCORTEX_COM3_RX_GPIO_PORT};
 
const uint32_t COM_USART_CLK[COMn] = {UCORTEX_COM1_CLK, UCORTEX_COM2_CLK, UCORTEX_COM3_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {UCORTEX_COM1_TX_GPIO_CLK, UCORTEX_COM2_TX_GPIO_CLK, UCORTEX_COM3_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {UCORTEX_COM1_RX_GPIO_CLK, UCORTEX_COM2_RX_GPIO_CLK, UCORTEX_COM3_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {UCORTEX_COM1_TX_PIN, UCORTEX_COM2_TX_PIN, UCORTEX_COM3_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {UCORTEX_COM1_RX_PIN, UCORTEX_COM2_RX_PIN, UCORTEX_COM3_RX_PIN};

const uint8_t COM_IRQn[COMn] = {UCORTEX_COM1_IRQn, UCORTEX_COM2_IRQn, UCORTEX_COM3_IRQn};


/**
  * @brief  初始化串口GPIO
  * @param  COM: 指定要初始化的COM PORT   
  *     @arg COM1 串口1
  *     @arg COM2 串口2
  *     @arg COM3 串口3
  * @retval None
  * @note None
  */
void COM_GPIOInit(COM_TypeDef COM)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 开启GPIO时钟和复用功能时钟RCC_APB2Periph_AFIO */
  RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

  /* 配置 USART Tx 复用推挽输出 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* 配置 USART Rx 浮空输入 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);
}

/**
  * @brief  配置串口
  * @param  COM: 指定要初始化的COM PORT   
  *     @arg COM1 串口1
  *     @arg COM2 串口2
  *     @arg COM3 串口3
  * @param  Baudrate 串口波特率
  * @retval None
  * @note None
  */
void COM_Init(COM_TypeDef COM, uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  COM_GPIOInit(COM);//初始化串口GPIO

  /* 开启串口时钟，注意串口1挂载在APB1外设总线下，其他串口挂载APB2外设总线下*/
  if (COM == COM1)
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); //开启串口时钟
  else
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);	//开启串口时钟
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置优先级组2
	/* USARTx NVIC配置信息 */
	NVIC_InitStructure.NVIC_IRQChannel = COM_IRQn[COM];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面的配置信息初始化NVIC寄存器
	
	
  /*    串口配置信息
	      -------------
        - 波特率根据参数指定
        - 数据长度8bit
        - 1个停止位
        - 无奇偶校验位
        - 无硬件数据流控制(RTS 和 CTS 信号)
        - 允许串口发送和接收
  */
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(COM_USART[COM], &USART_InitStructure);			//根据串口配置信息初始化串口 

  USART_Cmd(COM_USART[COM], ENABLE);										//开启串口
}


/********************* (C) COPYRIGHT 2018 ROBINJR. **********END OF FILE**********/
