#include "TCS3200.h"

u16 amount;
u16 Rgena=205,Ggena=195,Bgena=255;//红色、绿色、蓝色因子

void TCS3200_Init(void)
{
	TCS3200_GPIOinit();
	TCS3200EXIT_init();
	SetTCSMode(1,1);
}

void TCS3200_GPIOinit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(TCS_CLK, ENABLE);
  
    //输出IO初始化
    GPIO_InitStructure.GPIO_Pin =TCS_S0_PIN|TCS_S1_PIN|TCS_S2_PIN|TCS_S3_PIN|LED_CON_PIN;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(TCS_PORT, &GPIO_InitStructure);
		GPIO_ResetBits(TCS_PORT,LED_CON_PIN);//关LED显示
     
	  //输入IO初始化
    GPIO_InitStructure.GPIO_Pin =   TCS_OUT_PIN;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TCS_PORT, &GPIO_InitStructure);  
	
}

void TCS3200EXIT_init(void)
{
	 	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

		//配置中断线
  	GPIO_EXTILineConfig(TCS_OUT_PORTSOURCE,TCS_OUT_PINSOURCE);

  	EXTI_InitStructure.EXTI_Line=TCS_EXTI_LINE;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}
//外部中断计数事件 下降沿触发
 void EXTI9_5_IRQHandler(void)
{			
	 amount ++;
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}

/*
	s0      s1 
   0       0    Power down
   0       1    2%
   1       0    20%
   1       1    100%
*/
void SetTCSMode(u8 s0,u8 s1)
{
	S0 = s0;
	S1 = s1;
}

void whitebalance(void)
{

	S2=0;S3=0;//红色通道
	amount=0;			 //开始计数
	delay_ms(ALLTIME);
	Rgena = amount;   //求出红色因子      
	amount=0;
//----------------------------------
	S2=1;S3=1;//绿色通道
	amount=0;
	delay_ms(ALLTIME);
	Ggena = amount;	 //求出绿色因子
	amount=0;
//----------------------------------
	S2=0;S3=1;//蓝色通道
	amount=0;
	delay_ms(ALLTIME);
	Bgena = amount;	  //求出蓝色因子
	amount=0;
	S2=1;S3=0;//关闭通道 
	
	TCSLED = 0;	
}  

void SetWhiteBalance(void)
{
	TCSLED = 1;
	delay_ms(1000);	
	whitebalance();
	delay_ms(500);	
	TCSLED = 0;	
}

u16 GetTcs3200_RED(void)
{
	u16 Ramount;
	S2=0;S3=0;
	amount=0;
	delay_ms(ALLTIME);
	Ramount=(u32) amount*255/Rgena;	 //取R值
	if(Ramount>255) Ramount = 255;
	return Ramount;
//	amount=0;
}

u16 GetTcs3200_GREEN(void)
{
	u16 Gamount;
	S2=1;S3=1;
	amount=0;
	delay_ms(ALLTIME);
	Gamount=(u32) amount*255/Ggena;	//取G值
	if(Gamount>255) Gamount = 255;
	return Gamount;
//	amount=0;
}

u16 GetTcs3200_BLUE(void)
{
	u16 Bamount;
	S2=0;S3=1;
	amount=0;
	delay_ms(ALLTIME);
	Bamount=(u32) amount*255/Bgena;//去B值
	if(Bamount>255) Bamount = 255;
	return Bamount;
//	amount=0;
} 

void GetTcs3200_VALUE(u16* Get_Value)
{
	delay_ms(10);
	*Get_Value  	 = GetTcs3200_RED();
	delay_ms(10);
	*(Get_Value+1) = GetTcs3200_GREEN();
	delay_ms(10);
	*(Get_Value+2) = GetTcs3200_BLUE();
}

