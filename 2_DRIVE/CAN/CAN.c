#include "CAN.h"
#include "usart.h"

//括号中为重映射引脚
// PA11(PB8) - CAN-RX
// PA12(PB9) - CAN-TX

/*CAN初始化  */
void MYCAN_Init(void)
{
	CAN_GPIO_Init();
	CAN_NVIC_Configuration();
	CAN_MODE_INIT();
}

//CAN总线GPIO初始化
void CAN_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* 复用功能和GPIOB端口时钟使能*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	                        											 

  /* CAN1 模块时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

  /* Configure CAN pin: RX */	 // PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   // PB9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

}

/*CAN RX0 中断优先级配置  */
 void CAN_NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* enabling interrupt */
  NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//CAN外设初始化
void CAN_MODE_INIT(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;  

  /* CAN register init */
  CAN_DeInit(CAN1);	//将外设CAN的全部寄存器重设为缺省值
  CAN_StructInit(&CAN_InitStructure);//把CAN_InitStruct中的每一个参数按缺省值填入

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//没有使能时间触发模式
  CAN_InitStructure.CAN_ABOM=DISABLE;//没有使能自动离线管理
  CAN_InitStructure.CAN_AWUM=DISABLE;//没有使能自动唤醒模式
  CAN_InitStructure.CAN_NART=DISABLE;//没有使能非自动重传模式
  CAN_InitStructure.CAN_RFLM=DISABLE;//没有使能接收FIFO锁定模式
  CAN_InitStructure.CAN_TXFP=DISABLE;//没有使能发送FIFO优先级
	
  CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;//CAN设置为正常模式
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //重新同步跳跃宽度1个时间单位
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq; //时间段1为3个时间单位
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq; //时间段2为2个时间单位
  CAN_InitStructure.CAN_Prescaler=60;  //时间单位长度为60	
  CAN_Init(CAN1,&CAN_InitStructure);
                                      //波特率为：72M/2/60(1+3+2)=0.1 即100K

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;//指定过滤器为0
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//指定过滤器为标识符屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽为32位
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;// 过滤器标识符的高16位值
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//	 过滤器标识符的低16位值
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//过滤器屏蔽标识符的高16位值
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//	过滤器屏蔽标识符的低16位值
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;// 设定了指向过滤器的FIFO为0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;// 使能过滤器
  CAN_FilterInit(&CAN_FilterInitStructure);//	按上面的参数初始化过滤器

  /* CAN FIFO0 message pending interrupt enable */ 
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); //使能FIFO0消息挂号中断
 	
}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x12;			// 标准标识符 
	TxMessage.ExtId=0x12;			// 设置扩展标示符 
	TxMessage.IDE=CAN_Id_Standard; 	// 标准帧
	TxMessage.RTR=CAN_RTR_Data;		// 数据帧
	TxMessage.DLC=len;				// 要发送的数据长度
	for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;	 
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
  if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)
		return 0;		//没有接收到数据,直接退出 
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
  for(i=0;i<8;i++)//最多8个字节
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

/* USB中断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
	
		printf((unsigned char*)RxMessage.Data);
		printf("\r\n");
   
} 


