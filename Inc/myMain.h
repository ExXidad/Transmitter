#include "tim.h"
#include "usart.h"
#include <cstring>
#include <stdio.h>
#include "retarget.h"
#include "ManchesterEncode.h"

[[noreturn]] int myMain();

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);