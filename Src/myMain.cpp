#include "myMain.h"

[[noreturn]] int myMain()
{
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim9);
    RetargetInit(&huart3);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);


    while (1)
    {
        // HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        // HAL_Delay(150);
        // HAL_GPIO_TogglePin(LDB_GPIO_Port, LDB_Pin);
        // HAL_Delay(150);

        HAL_Delay(500);
    }
}

const char *message = "Hello, world!";

bool byteChosen = false;
uint32_t byteIdx = 0;
char txByte;

uint32_t bitIdx = 0;
bool clk = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // HAL_GPIO_TogglePin(LSR_GPIO_Port, LSR_Pin);

    // Выбрать новый байт если нужно
    if (!byteChosen)
    {
        txByte = message[byteIdx];
        byteChosen = true;

        if (byteIdx >= strlen(message) - 1)
        {
            byteIdx = 0;
            HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        } else
        {
            ++byteIdx;
        }
    }

    // Пройти по всем битам в байте
    if (bitIdx < 8)
    {
        bool txBit = txByte & (0x80 >> bitIdx);

        HAL_GPIO_WritePin(LSR_GPIO_Port, LSR_Pin, ((!(txBit ^ clk)) ? GPIO_PIN_SET : GPIO_PIN_RESET));

        HAL_GPIO_WritePin(MCLK_GPIO_Port, MCLK_Pin, txBit ? GPIO_PIN_SET : GPIO_PIN_RESET);
        // Обновить тактовый импульс
        clk ^= 1;
        if (clk == 0)
        {
            ++bitIdx;
        }
    } else
    {
        bitIdx = 0;
        byteChosen = false;
    }
}