/**
******************************************************************************
  * @file       adc.h
  * @brief      STM ADC Driver
  * @version    1.0
  * @date       Aug-24-2019 Sat
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ADC_TempSensor_Init(void);
u16 ADC_ReadADC1(void);
float ADC_ConvertVot(u16 ADCValue);
float ADC_GetTemperValue(void);

#ifdef __cplusplus
}
#endif

#endif
