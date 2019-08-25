/**
******************************************************************************
  * @file       main.c
  * @brief      主程序源文件
  * @version    1.0
  * @date       Aug-20-2019 Tue
  * @update     
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f10x.h"

/** @addtogroup main
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
extern OS_STK Stack_Start[STACK_LEN_START];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void NVIC_Config(void);
static void RCC_Config(void);
void IWWG_Init(void);
void DebugMode_PrintfSysInfo(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	INT8U Ret;
	
	/* Mask global interrupt */
	__set_PRIMASK(1);
	
	NVIC_Config();
	RCC_Config();
	JTAG_Disable();
	
	LED_Init();
	
	VoiceModule_Init();
	
	Key_Init();
	MPR_IRQ_Init();
	EXTI3_Init();
	
	SPI2_Init();
	
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	ADC1_DMA_Init();
	
	USART1_Init(115200);
	
//	IWWG_Init();

	/* Enable global interrupt */
	__set_PRIMASK(0);

	for(INT32U i=0;i < 6;i++)
	{
		TIM2_Delay_ms(100);
		LED2_OR();
	}
	LED2_OFF();
	
	DebugMode_PrintfSysInfo();
	
	OS_CPU_SysTickInit();
	OSInit();
	
	Ret = OSTaskCreate(
			Task_Start,
			NULL,
			&Stack_Start[STACK_LEN_START-1],
			TASK_PRIO_START);

	if(Ret == OS_ERR_NONE)
	{
	}
	else
	{
		// Create Task Error
	}
	
	OSStart();
	
	while(1)
	{
	}
	
	/* No Retval */
}

/**
  * @brief  NVIC configure function
  * @param  NULL
  * @return NULL
  */
static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set EXTI3 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

/**
  * @brief  JTAG disable function
  * @param  NULL
  * @return NULL
  */
static void JTAG_Disable(void)
{
	/* 重映射 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// 复位后才可再次重映射
	
	return ;
}

/**
  * @brief  RCC congigure function
  * @param  NULL
  * @return NULL
  */
static void RCC_Config(void)
{
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	return ;
}

void DebugMode_PrintfSysInfo(void)
{
#ifdef DEBUG
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	
	printf("-----------------------------------------------\r\n");
	printf("Debug Mode\r\n");
	printf("SYSCLK_Frequency:\t%d\r\n",RCC_Clocks.SYSCLK_Frequency);
	printf("HCLK_Frequency:\t\t%d\r\n",RCC_Clocks.HCLK_Frequency);
	printf("PCLK1_Frequency:\t%d\r\n",RCC_Clocks.PCLK1_Frequency);
	printf("PCLK2_Frequency:\t%d\r\n",RCC_Clocks.PCLK2_Frequency);
	printf("ADCCLK_Frequency:\t%d\r\n",RCC_Clocks.ADCCLK_Frequency);
	printf("-----------------------------------------------\r\n");
#endif
	
	return ;
}


/**
  * @brief  Independent Watch Dog initialization function
  * @param  NULL
  * @return NULL
  */
void IWWG_Init(void)
{
	__IO uint32_t LsiFreq = 40000;
	
	/* IWDG timeout equal to 2 s (the timeout may varies due to LSI frequency
		dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: LSI/256 */
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
  /* Set counter reload value to obtain 1s IWDG TimeOut. */
	IWDG_SetReload(LsiFreq/256*2);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

/* Exported functions --------------------------------------------------------*/

// 初始化系统节拍
void  OS_CPU_SysTickInit (void)
{
    RCC_ClocksTypeDef  RCC_Clocks;

    // 获取系统频率
    RCC_GetClocksFreq(&RCC_Clocks);

    // 配置HCLK作为SysTick时钟
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    
    // 配置自动加载数值
    SysTick->LOAD = (RCC_Clocks.HCLK_Frequency / OS_TICKS_PER_SEC) - 1;
    
    // 使能SysTick
    SysTick->CTRL |=  0x01;
    
    // 使能SysTick中断
    SysTick->CTRL |= 0x02;
}

#ifdef DEBUG
/**
  * @brief  Retargets the C library printf function to the USART
  * @param  字符串
  * @param  文件指针
  * @retval 字符串
  */
int fputc(int ch,FILE* f)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}
#endif

    
/**
  * @}
  */
