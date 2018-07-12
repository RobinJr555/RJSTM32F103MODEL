#include "HCSR04.h"

 
void Hcsr04Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
  
    //输出IO初始化
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
	  //输入IO初始化
    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
}
 
/********************************************************************************************************************************************
@f_name: void Hcsr04_Ranging(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量 单位为cm
@return: None
*********************************************************************************************************************************************/
void Hcsr04_Ranging(float *p)
{
	u8 i;
	u32 j;
	float Hcsr04_Temp;	
	for(i=0;i<5;i++)
	{
		TRIG_Send=1;
		delay_us(10);
		TRIG_Send=0;
		while(!ECHO_Reci); //一直为0 //发脉冲时间
		while(ECHO_Reci)   //上拉 脉冲返回中……
		{
			delay_us(10);
			j++;
			if(j >1000000)
			{
				break;
			}
		}
		Hcsr04_Temp+=340/2*j*10;//  模块最大可测距3m 
		j=0;
		delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Hcsr04_Temp/50000; 		
}
