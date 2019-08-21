/**
******************************************************************************
  * @file       global.h
  * @brief      全局类型变量定义
  * @version    1.0 
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "task.h"

/* Exported types ------------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern OS_STK Stack_Start[STACK_LEN_START];

extern OS_EVENT *MBox_KeyValve;

extern OS_EVENT *MQueue;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
