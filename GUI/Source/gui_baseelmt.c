/**
******************************************************************************
  * @file       GUI_BaseElmt.c
  * @brief      ͼ�λ���Ԫ��
  * @version    1.0
  * @date       Aug-13-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_baseelmt.h"

/** @addtogroup Base Element
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GUI_ReversalEnable(void)
{
    ReversalFlag = 1;
    return;
}
void GUI_ReversalDisable(void)
{
    ReversalFlag = 0;
    return;
}

void GUI_DisplayChar(u8 RowNumber, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray)
{
    OLED_Show_XxN8_Character(RowNumber, Column, RowHeight, Width, FontArray);

    return;
}

void GUI_DisplayString(u8 RowNumber, u8 Column, const u8 *StringFont, u8 StringLength)
{
    OLED_ShowString(RowNumber, 2, Column, StringFont, StringLength);
    return;
}

void GUI_ShowOperationTipRow(u8 RowNumber, u8 RowHeight)
{
    u32 j[] = {2, 1, 2, 2, 1, 1, 2, 1, 2, 2}; // ��¼�ֿ������
    u32 k = 0;                                // ��¼��Ӧ����λ��

    for (u32 i = 0; i < 10; i++) // ��ʾ10����
    {
        OLED_Show_XxN8_Character(RowNumber, k * 8, RowHeight, j[i] * 8, &(String1_16xN[k][0]));
        k += j[i];
    }

    return;
}

void GUI_DisplayPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture)
{
    OLED_ShowPicture(x, y, px, py, Picture);

    return;
}

// ����Ϊ6λ���������
void GUI_DisplayPassBox(u8 RowNumber, u8 *Str)
{
    for (u32 i = 0; i < 6; i++)
    {
        if (Str[i] == 0) // ������ַ����ж�Ӧ�ַ�ֵΪ0
        {
            // ��ʾ��_��
            OLED_Show_XxN8_Character(RowNumber, (i * 16) + 20, 2, 8, &Char_8x16[0][0]);
        }
        else // ������ַ����ж�Ӧ�ַ�ֵ��0
        {
            // ��ʾ��*��
            OLED_Show_XxN8_Character(RowNumber, (i * 16) + 20, 2, 8, &Char_8x16[1][0]);
        }
    }
    return;
}

void GUI_Show8StrTextBox(u8 RowNumber, u8 RowHeight, u8 *Str)
{
    for (u32 i = 0; i < 8; i++)
    {
        if (Str[i] == 0) // ������ַ����ж�Ӧ�ַ�ֵΪ0
        {
            // ��ʾ��_��
            OLED_Show_XxN8_Character(RowNumber, (i * 16) + 4, RowHeight, 8, &Char_8x16[0][0]);
        }
        else // ������ַ����ж�Ӧ�ַ�ֵ��0
        {
            // ��ʾ��*��
            OLED_Show_XxN8_Character(RowNumber, (i * 16) + 4, RowHeight, 8, &Char_8x16[1][0]);
        }
    }
    return;
}

void GUI_DisplayNumber(u8 RowNumber, u8 Column, u32 Number, u8 ShowLength, u8 FontSize)
{
    if (FontSize == 1) // 4 x 8����
    {
        for (u32 i = ShowLength - 1; i < ShowLength; i--)
        {
            OLED_Show_XxN8_Character(RowNumber, Column + i * 4, 1, 4, &Number_4x8[Number % 10][0]);
            Number /= 10;
        }
    }
    else if (FontSize == 2) // 8 x 16����
    {
        for (u32 i = ShowLength - 1; i < ShowLength; i--)
        {
            OLED_Show_XxN8_Character(RowNumber, Column + i * 8, 2, 8, &Number_8x16[Number % 10][0]);
            Number /= 10;
        }
    }
    else if (FontSize == 3)
    {
        for (u32 i = ShowLength - 1; i < ShowLength; i--)
        {
            OLED_Show_XxN8_Character(RowNumber, Column + i * 12, 3, 12, &Number_12x24[Number % 10][0]);
            Number /= 10;
        }
    }
    else
    {
        return;
    }
}

/**
  * @}
  */
