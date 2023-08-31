#include "py32f0xx_hal.h"

int main(void)
{
	GPIO_InitTypeDef GPIOInitType;

	HAL_Init();
	__HAL_RCC_GPIOB_CLK_ENABLE();

    GPIOInitType.Pin = GPIO_PIN_0;
    GPIOInitType.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIOInitType);

	for (;;) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_Delay(500);
	}

	return 0;
}
