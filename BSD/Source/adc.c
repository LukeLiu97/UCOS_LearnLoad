/**
******************************************************************************
  * @file       adc.c
  * @brief      STM32 ADC Driver
  * @version    1.0
  * @date       Aug-24-2019 Sat
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/** @addtogroup ADC Driver
  * @{
  */
extern void TIM4_Delay_us(u16 t);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void ADC_TempSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72MHz/6 < 14MHz
	
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_55Cycles5);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
}

u16 ADC_ReadADC1(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
	
	return ADC_GetConversionValue(ADC1);
}

float ADC_ConvertVot(u16 ADCValue)
{
	static const float ADC_Ratio = 3.3/4095;
	
	return (ADC_Ratio * ADCValue);
}

float ADC_GetTemperValue(void)
{
	u32 ADCValueSum = 0;
	float Temper;
	
	for(uint32_t i = 0;i<10;i++)
	{
		ADCValueSum += ADC_ReadADC1();
		TIM4_Delay_us(4);
	}
	
	Temper = (1.43 - ADC_ConvertVot(ADCValueSum / 10))*1000/4.3 + 25;
	return Temper;
}

/**
  * @}
  */
