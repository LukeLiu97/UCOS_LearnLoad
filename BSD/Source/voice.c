/**
******************************************************************************
  * @file       voice.c
  * @brief      ����ģ����ز���Դ�ļ�
  * @version    1.1
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "voice.h"

/** @addtogroup voice module
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void VoiceModule_Init(void)
{
    /* GPIO ��ʼ�� VOICE_DATA PB0 VOICE_BUSY PB1 */
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);

    /* BUSY PB1 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PB0 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ����VOICE_DATA�����״̬ */
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    return;
}

void Voice_Play(VoiceCmd_TypeDef VoiceCmd)
{

        /* �ȴ��������� */
        while ((GPIOB->IDR & (0x1 << 1)) != 0)
        {
        }

        /* ͬ��ͷ */
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        TIM2_Delay_ms(8);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        TIM2_Delay_ms(1);

        /* �������� */
        for (u32 i = 0; i < 8; i++)
        {
            // MSB
            if ((VoiceCmd & (0x1 << (7 - i))) != 0)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
                TIM4_Delay_us(1500);
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                TIM4_Delay_us(500);
            }
            else
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
                TIM4_Delay_us(500);
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                TIM4_Delay_us(1500);
            }
        }


    return;
}

/**
  * @}
  */
