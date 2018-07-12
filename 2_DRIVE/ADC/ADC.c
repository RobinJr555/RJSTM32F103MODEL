#include "ADC.h"
#include "delay.h"

void ADC_Initialize(void)
{
	ADC_GPIO_Init();
	ADC_Channel_Initialize();
}
/**
  * @brief ADC通道初始化
	* @param none
  * @retval none
  * @note  单次转换，软件触发ADC转换
  */ 
void ADC_Channel_Initialize(void)
{

	ADC_InitTypeDef ADC_InitStructure;

	
	/* 设置ADCCLK分频因子 ADCCLK = PCLK2/6，即 72MHz/6 = 12MHz */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_DeInit(ADC1);  //将ADC1设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发ADC转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//规则转换通道数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct初始化ADC
	
	ADC_Cmd(ADC1, ENABLE);	//使能ADC1
	
	ADC_ResetCalibration(ADC1);	//复位ADC校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束 
}

/**
  * @brief ADC_GPIO口初始化
	* @param none
  * @retval none
  * @note  单次转换，软件触发ADC转换
  */ 
void ADC_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能GPIOA，ADC1,AFIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

	  /* 配置 PA.00 (ADC1_IN0) 作为模拟输入引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}
/**
  * @brief 获得ADC值
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:通道值 ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADC转换值
  * @note 
  */
uint16_t Get_Adc(ADC_TypeDef* ADCx, uint8_t ch)   
{
	/* ADC的规则通道组设置：一个序列，采样时间239.5周期	*/
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_239Cycles5 );
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//开始转换
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));	//等待转换结束
	return ADC_GetConversionValue(ADCx);						//返回ADC1转换结果
}

/**
  * @brief 获取连续10次ADC采样的平均值
  * @param ADCx: ADC1 / ADC2 / ADC3
  * @param ch:通道值 ADC_Channel_0 ~ ADC_Channel_17
  * @retval ADC转换的平均值
  * @note 
  */
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx, uint8_t ch)
{
	uint32_t adc_val = 0;
	uint8_t i;
	
	for(i = 0; i < 10; i++)
	{
		adc_val += Get_Adc(ADCx, ch);
		delay_ms(5);
	}
	return adc_val/10;
} 	 

float Get_Voltage(u16 ADCvalue)
{
 return 3.3 * ADCvalue / 4096 ;
}