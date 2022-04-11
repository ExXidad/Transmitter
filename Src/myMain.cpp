#include "myMain.h"

ManchesterEncode manchesterEncode(SIG_GPIO_Port, SIG_Pin, &htim9);

[[noreturn]] int myMain()
{
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim9);
    RetargetInit(&huart3);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
    uint8_t data[4]{'4', 'l', 'e', 'n'};

    while (1)
    {
        // HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        // HAL_Delay(150);
        // HAL_GPIO_TogglePin(LDB_GPIO_Port, LDB_Pin);
        // HAL_Delay(150);
        HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        manchesterEncode.startEncoding(data, 4);
        HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        HAL_Delay(100);
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // HAL_GPIO_TogglePin(LSR_GPIO_Port, LSR_Pin);
    manchesterEncode.pasteThisToTIMCallback(htim);
}