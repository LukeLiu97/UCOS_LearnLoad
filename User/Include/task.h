/**
******************************************************************************
  * @file       task.h
  * @brief      任务头文件
  * @version    1.0
  * @date       Aug-20-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"	 
	 
#include "stdio.h"
#include "ucos_ii.h"
//#include "os_cpu.h"
//#include "os_cfg.h"

#include "led.h"
#include "key.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define STACK_LEN_START		(128)
#define STACK_LEN_LED		(128)
#define STACK_LEN_KEY		(128)
#define STACK_LEN_WDOG		(128)
#define TASK_PRIO_START		(0)
#define TASK_PRIO_LED		(1)
#define TASK_PRIO_KEY		(2)
#define TASK_PRIO_WDOG		(3)

/* Exported functions ------------------------------------------------------- */
void Task_Start(void *pd);
void Task_LED(void *pd);
void Task_Key(void *pd);
void Task_WDog(void *pd);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
