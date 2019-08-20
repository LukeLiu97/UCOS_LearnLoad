/**
******************************************************************************
  * @file       gui_password.c
  * @brief      ��������ͼ�ν���
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_password.h"

/** @addtogroup Password Input
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// ��Ҫ����Buff��飬��ֹ�Ƿ�д�ڴ�
u8 NumStr_Input(u8 *Buff, u8 StrLength)
{
    u32 i;
    u8 KeyValue;

    for (i = 0; i < StrLength;)
    {
        LED3_OFF();

        KeyValue = Key_Scan();

        if (KeyValue != 0) /* ����ģ���⵽�����仯 */
        {
#ifdef DEBUG
            printf("KeyValue = %c\r\n", KeyValue);
#endif

            Voice_Play(VoiceCmd_Di);

            if (KeyValue == '#')
            {
                break;
            }
            else if (KeyValue == '*')
            {
                if (i == 0)
                {
                    // Don't do anything
                }
                else
                {
                    i -= 1;
                }
                Buff[i] = 0; // ����i�������һλ
            }
            else
            {
                Buff[i] = KeyValue; // ��ǰ���뱻��¼����������Ӧλ��
                i++;                // i ǰ��һλ��Ϊ�´�����׼��
            }
        }
        else
        {
        }

        GUI_DisplayPassBox(4, Buff);
    }
    //	StringBuff[i] = 0; //������Ϊ�ַ���β���'\0'

    return i;
}

/* Exported functions --------------------------------------------------------*/

// ��ʱ���ȶ�Ϊ6λ
u8 GUI_Password_Enroll(u8 *PasswordBuff, u8 PasswordLength)
{
    u8 PasswordTemp[2][7] = {0};

    GUI_ClearScreen();

    GUI_DisplayString(0, 16, &MenuString1_16x16[0][0], 6);         // ������ʾ���޸��û����롱
    GUI_DisplayString(2, 16 + 0, &EnterString_16x16[0][0], 2);     // �����롱
    GUI_DisplayString(2, 16 + 32, &NewString_16x16[0][0], 2);      // ���µġ�
    GUI_DisplayString(2, 16 + 64, &PasswordString_16x16[0][0], 2); // �����롱

    Voice_Play(VoiceCmd_INPUT_NEW_PASSWORD);

    if (NumStr_Input(PasswordTemp[0], 6) != 6)
    {
        Voice_Play(VoiceCmd_REPETE);
        return 1;
    }
    else
    {
    }

    PasswordTemp[0][6] = '\0';

    GUI_ClearScreen();

    GUI_DisplayString(0, 16, &MenuString1_16x16[0][0], 6);         // ������ʾ���޸��û����롱
    GUI_DisplayString(2, 16 + 0, &AfreshString_16x16[0][0], 2);    // �����¡�
    GUI_DisplayString(2, 16 + 32, &EnterString_16x16[0][0], 2);    // �����롱
    GUI_DisplayString(2, 16 + 64, &PasswordString_16x16[0][0], 2); // �����롱

    Voice_Play(VoiceCmd_INPUT_NEW_PASSWORD_AGAIN);

    if (NumStr_Input(PasswordTemp[1], 6) != 6)
    {
        Voice_Play(VoiceCmd_REPETE);
        return 1;
    }
    else
    {
    }

    PasswordTemp[1][6] = '\0';

    if (String_Compare(PasswordTemp[0], PasswordTemp[1]) != 0)
    {
        Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
        return 1;
    }
    else
    {
        for (u32 i = 0; i < 6; i++)
        {
            PasswordBuff[i] = PasswordTemp[0][i];
        }

        Voice_Play(VoiceCmd_SETTING_SUCCESS);
        return 0;
    }
}

// ��ʱ���ȶ�Ϊ6λ
u8 GUI_Password_Check(const u8 *Password, u8 PasswordLength)
{
    u8 PasswordTemp[7] = {0};

    if (NumStr_Input(PasswordTemp, 6) != 6)
    {
        Voice_Play(VoiceCmd_REPETE);
        return 1;
    }
    else
    {
    }

    PasswordTemp[6] = '\0';

    if (String_Compare(PasswordTemp, Password) != 0)
    {
        Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
        return 1;
    }
    else
    {
        Voice_Play(VoiceCmd_SETTING_SUCCESS);
        return 0;
    }
}

/**
  * @}
  */
