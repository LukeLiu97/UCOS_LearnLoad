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
#include "oled.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define STACK_LEN_START		(128)
#define STACK_LEN_LED		(128)
#define STACK_LEN_KEY		(128)
#define STACK_LEN_OLED		(128)
#define STACK_LEN_WDOG		(128)

#define TASK_PRIO_START		(0)
#define TASK_PRIO_IDLE		(2) // 空闲优先级，供互斥锁使用
#define TASK_PRIO_LED		(3)
#define TASK_PRIO_KEY		(7)
#define TASK_PRIO_OLED		(11)
#define TASK_PRIO_WDOG		(15)

#define MQUEUE_LEN_KEYVALUE	(128)

/* Exported functions ------------------------------------------------------- */
void Task_Start(void *pd);
void Task_LED(void *pd);
void Task_Key(void *pd);
void Task_OLED(void *pd);
void Task_WDog(void *pd);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
