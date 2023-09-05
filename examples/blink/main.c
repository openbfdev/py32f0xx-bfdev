#include "py32f0xx_hal.h"

int main(void)
{
    RCC_OscInitTypeDef OscInitType;
    RCC_ClkInitTypeDef ClkInitType;
    GPIO_InitTypeDef GPIOInitType;

    OscInitType.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    OscInitType.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;
    OscInitType.HSIState = RCC_HSI_ON;
    OscInitType.HSIDiv = RCC_HSI_DIV1;
    OscInitType.PLL.PLLState = RCC_PLL_ON;
    OscInitType.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    HAL_RCC_OscConfig(&OscInitType);

    ClkInitType.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1;
    ClkInitType.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    ClkInitType.AHBCLKDivider = RCC_SYSCLK_DIV1;
    ClkInitType.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&ClkInitType, FLASH_LATENCY_1);

    HAL_Init();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIOInitType.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3;
    GPIOInitType.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIOInitType);

    for (;;) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_Delay(500);
    }

    return 0;
}
