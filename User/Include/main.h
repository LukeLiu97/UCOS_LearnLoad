/**
******************************************************************************
  * @file       main.h
  * @brief      主程序头文件
  * @version    1.0
  * @date       Aug-20-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "ucos_ii.h"
//#include "os_cpu.h"
//#include "os_cfg.h"	

#include "global.h"
#include "task.h"
	
#include "led.h"
#include "uart.h"
#include "time.h"
#include "tim.h"
#include "key.h"
#include "voice.h"
#include "spi.h"
#include "oled.h"
#include "font.h"
#include "adc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
