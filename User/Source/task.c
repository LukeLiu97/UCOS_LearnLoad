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
OS_EVENT *Sem_Key = NULL;
OS_EVENT *Sem_Temper = NULL;

OS_FLAG_GRP *FLAG_GRP = NULL;

OS_EVENT *MQueue = NULL;
void *Msg_KeyValue[MQUEUE_LEN_KEYVALUE] = {0};

OS_EVENT *Mutex_OLED;
OS_EVENT *Mutex_Temper;

OS_STK Stack_LED[STACK_LEN_LED];
OS_STK Stack_Key[STACK_LEN_KEY];
OS_STK Stack_OLED[STACK_LEN_OLED];
OS_STK Stack_Temper[STACK_LEN_TEMPER];
OS_STK Stack_WDog[STACK_LEN_WDOG];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Task_Start(void *pd)
{
	INT8U Retval;
	INT8U Err;
	
	do
	{
		MBox_KeyValve = OSMboxCreate((void *)0);
		MBox_WDOGVal = OSMboxCreate((void *)0);
		
		Sem_LED = OSSemCreate(0x00);
		
		Sem_Temper = OSSemCreate(0x00);
		
		FLAG_GRP = OSFlagCreate(0x00,&Err);
		
		MQueue = OSQCreate(&Msg_KeyValue[0],MQUEUE_LEN_KEYVALUE);
		
		Mutex_OLED = OSMutexCreate(TASK_PRIO_IDLE_1,&Err);
		Mutex_Temper = OSMutexCreate(TASK_PRIO_IDLE_2,&Err);
		
		Retval = OSTaskCreate(
					Task_LED,
					NULL,
					&Stack_LED[STACK_LEN_LED-1],
					TASK_PRIO_LED);
		Retval = OSTaskCreate(
					Task_Key,
					NULL,
					&Stack_Key[STACK_LEN_KEY-1],
					TASK_PRIO_KEY);
		Retval = OSTaskCreate(
					Task_Key,
					NULL,
					&Stack_Key[STACK_LEN_KEY-1],
					TASK_PRIO_KEY);
		Retval = OSTaskCreate(
					Task_OLED,
					NULL,
					&Stack_OLED[STACK_LEN_OLED-1],
					TASK_PRIO_OLED);
		Retval = OSTaskCreate(
					Task_Temper,
					NULL,
					&Stack_Temper[STACK_LEN_TEMPER-1],
					TASK_PRIO_TEMPER);
		Retval = OSTaskCreate(
					Task_WDog,
					NULL,
					&Stack_WDog[STACK_LEN_WDOG-1],
					TASK_PRIO_WDOG);
		
		/* Start ADC1 Software Conversion */ 
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		
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

INT8U *pmsg = NULL;
void Task_Key(void *pd)
{
	INT8U Msg_WDOGVal = WDOG_VAL_KEY;

	INT8U Err;
	while(1)
	{
		pmsg = (void *)OSQPend(MQueue,0,&Err);
		if(pmsg != NULL)
		{
			if(*((INT8U *)pmsg) != 0)
			{
				OSSemPost(Sem_LED);
				OSMboxPost(MBox_KeyValve,pmsg);
				OSTimeDly(10);
				switch(*((INT8U *)pmsg))
				{
					case '1':
						OSFlagPost(FLAG_GRP,0x01 << 0,OS_FLAG_SET,&Err);
						break;
					case '2':
						OSFlagPost(FLAG_GRP,0x01 << 1,OS_FLAG_SET,&Err);
						break;
					case '3':
						OSFlagPost(FLAG_GRP,0x01 << 2,OS_FLAG_SET,&Err);
						break;
					default:
						;
				}
			}
		}

		OSMboxPost(MBox_WDOGVal,&Msg_WDOGVal);
	}
	
	// No Retval
}

void Task_OLED(void *pd)
{
	INT8U Err;
	void *pmsg = NULL;
	
	while(1)
	{
		for(uint32_t i = 4;i < 7;i+=2)
		{
			for(uint32_t j = 0;j < 16;j++)
			{
				pmsg = (void *)OSMboxPend(MBox_KeyValve,0,&Err);
				
				OSMutexPend(Mutex_OLED,0,&Err);
				
				if(pmsg != NULL)
				{
					OLED_DisplayNumber(i,j * 8,*(INT8U *)pmsg - '0',1,2);
				}
				else
				{
				}
				
				OSMutexPost(Mutex_OLED);
			}
		}
	}
	
	// No Retval
}

void Task_Temper(void *pd)
{
	INT8U Err;
	float TemperValue;
	float LockValue;
	
	while(1)
	{
			
		/* Start ADC1 Software Conversion */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		
		OSMutexPend(Mutex_Temper,0,&Err);
		
		OSSemPend(Sem_Temper,0,&Err);
		
		LockValue = ADC_ConvertVot(ADC1_Value[0]);
		TemperValue = ADC_ConvertTemp(ADC1_Value[1]);
		
		if(TemperValue > 0)
		{
#ifdef DEBUG
			printf("LockValue:%f\r\n",LockValue);
			printf("TemperValue:%f\r\n",TemperValue);
#endif
			OLED_DisplayNumber(0,0,(u32)LockValue,2,2);
			OLED_DisplayNumber(0,24,(u32)(LockValue*100)%100,2,2);
			OLED_DisplayNumber(2,0,(u32)TemperValue,2,2);
			OLED_DisplayNumber(2,24,(u32)(TemperValue*100)%100,2,2);
		}
		else
		{
		}  
		OSMutexPost(Mutex_Temper);
		OSTimeDly(50);

	}
	
	// No Retval
}

void Task_WDog(void *pd)
{
	INT8U Msg_WDOGVal = 0x00;
	INT8U *pmsg = NULL;
	INT8U Err = 0;
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
			
			OSFlagPend(FLAG_GRP,(0x01 << 0)|(0x01 << 1)|(0x01 << 2),(OS_FLAG_WAIT_SET_AND)|(OS_FLAG_CONSUME),1,&Err);
			if(Err == OS_ERR_NONE)
			{
				__set_PRIMASK(1);
				NVIC_SystemReset();
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
