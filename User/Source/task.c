/**
******************************************************************************
  * @file       task.c
  * @brief      任务源文件
  * @version    1.0
  * @date       Aug-20-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "task.h"

/** @addtogroup OS Tasks
  * @{
  */
  
/* Global varaiables ---------------------------------------------------------*/
OS_STK Stack_Start[STACK_LEN_START];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WDOG_VAL_LED	(0x0F)
#define WDOG_VAL_KEY	(0xF0)
#define WDOG_VAL		(WDOG_VAL_LED | WDOG_VAL_KEY)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
OS_EVENT *MBox_KeyValve = NULL;
OS_EVENT *MBox_WDOGVal = NULL;

OS_EVENT *Sem_LED = NULL;
OS_EVENT *Sem_KEY = NULL;

OS_STK Stack_LED[STACK_LEN_LED];
OS_STK Stack_KEY[STACK_LEN_KEY];
OS_STK Stack_WDOG[STACK_LEN_WDOG];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Task_Start(void *pd)
{
	INT8U Retval;
	
	do
	{
		MBox_KeyValve = OSMboxCreate((void *)0);
		MBox_WDOGVal = OSMboxCreate((void *)0);
		
		Sem_LED = OSSemCreate(0x00);
		Retval = OSTaskCreate(
					Task_LED,
					NULL,
					&Stack_LED[STACK_LEN_LED-1],
					TASK_PRIO_LED);
		Retval = OSTaskCreate(
					Task_Key,
					NULL,
					&Stack_KEY[STACK_LEN_KEY-1],
					TASK_PRIO_KEY);
		Retval = OSTaskCreate(
					Task_WDog,
					NULL,
					&Stack_WDOG[STACK_LEN_KEY-1],
					TASK_PRIO_WDOG);
		if(Retval == OS_ERR_NONE)
		{
		}
		else
		{
			// Create Error
		}
		
		OSTaskDel(OS_PRIO_SELF); 
		
	}while(0);
	
	// No Retval
}

void Task_LED(void *pd)
{
	INT8U Msg_WDOGVal = WDOG_VAL;
	INT8U Err;
	while(1)
	{
		OSSemPend(Sem_LED,0,&Err);
		LED3_OR();
		
		OSMboxPost(MBox_WDOGVal,&Msg_WDOGVal);
	}
	
	// No Retval
}
void Task_Key(void *pd)
{
	INT8U Msg_WDOGVal = WDOG_VAL_KEY;
	INT8U *pmsg = NULL;
	INT8U Err;
	while(1)
	{
		pmsg = (void *)OSMboxPend(MBox_KeyValve,0,&Err);
		if(pmsg != NULL)
		{
			if(*((INT8U *)pmsg) != 0)
			{
				OSSemPost(Sem_LED);
			}
		}
		OSMboxPost(MBox_WDOGVal,&Msg_WDOGVal);
	}
	
	// No Retval
}

void Task_WDog(void *pd)
{
	INT8U Msg_WDOGVal = 0x00;
	INT8U *pmsg = NULL;
	INT8U Err;
	while(1)
	{
		pmsg = (void *)OSMboxPend(MBox_WDOGVal,0,&Err);
		if(pmsg != NULL)
		{
			Msg_WDOGVal |= *(INT8U *)(pmsg);
			if(Msg_WDOGVal == WDOG_VAL)
			{
				// TODO 喂狗
				Msg_WDOGVal = 0x00;
				/* Reload IWDG counter */
				IWDG_ReloadCounter(); 
			}
			else
			{
				
			}
		}
		else
		{
		}
	}
	
	// No Retval
}


/**
  * @}
  */
