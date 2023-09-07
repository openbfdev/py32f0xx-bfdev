#include <stdio.h>
#include <bfdev/config.h>
#include "py32f0xx_hal.h"

UART_HandleTypeDef huart1;
extern int benchmark(void);

int __io_putchar(int ch)
{
    if (ch == '\n')
        HAL_UART_Transmit(&huart1, (void *)"\r", 1, -1);
    HAL_UART_Transmit(&huart1, (void *)&ch, 1, -1);
    return ch;
}

int main(void)
{
    RCC_OscInitTypeDef OscInitType = {};
    RCC_ClkInitTypeDef ClkInitType = {};
    GPIO_InitTypeDef GPIOInitType = {};

    HAL_Init();

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

    GPIOInitType.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIOInitType.Mode = GPIO_MODE_AF_PP;
    GPIOInitType.Pull = GPIO_PULLUP;
    GPIOInitType.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIOInitType.Alternate = GPIO_AF1_USART1;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &GPIOInitType);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.Mode = UART_MODE_TX_RX;
    __HAL_RCC_USART1_CLK_ENABLE();
    HAL_UART_Init(&huart1);

    printf("Benchmark for PY32F0xx.\n");
    printf("Bfdev version: %s\n", __bfdev_stringify(BFDEV_VERSION));
    printf("This may take a few minutes...\n\n");
    benchmark();

    return 0;
}
