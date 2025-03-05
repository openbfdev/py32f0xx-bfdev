#include <bfdev.h>
#include "beep.h"
#include "py32f0xx_hal.h"

#define BEEP_TIMER TIM1
#define BEEP_CHANNEL TIM_CHANNEL_2

static TIM_HandleTypeDef htim1;

static int
buzzer_play(unsigned int freq, unsigned int length)
{
    uint32_t prescaler, overflow;
    uint32_t cycles;

    cycles = 48000000 / freq;
    overflow = cycles;
    prescaler = 0;

	while (overflow > UINT16_MAX) {
        prescaler++;
		overflow = cycles / (prescaler + 1);
    }

    if (prescaler > UINT16_MAX)
        return -1;

    __HAL_TIM_SET_PRESCALER(&htim1, prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim1, overflow - 1);
    __HAL_TIM_SET_COMPARE(&htim1, BEEP_CHANNEL, overflow >> 1);

    if (HAL_TIM_PWM_Start(&htim1, BEEP_CHANNEL) != HAL_OK)
        return -1;

    HAL_Delay(length);
    if (HAL_TIM_PWM_Stop(&htim1, BEEP_CHANNEL) != HAL_OK)
        return -1;

    return 0;
}

int
buzzer_init(void)
{
    TIM_OC_InitTypeDef sConfig = {};

    htim1.Instance = BEEP_TIMER;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    htim1.Init.RepetitionCounter = 0;

    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
        return -1;

    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfig, BEEP_CHANNEL) != HAL_OK)
        return -1;

    return 0;
}

int
buzzer_beep(const struct beep_table *table)
{
    unsigned int freq, length;
    int retval;

    freq = 440;
    length = 200;

    for (retval = 0; !retval && table->type; ++table) {
        switch (table->type) {
            case BEEP_PLAY: default:
                retval = buzzer_play(freq, length);
                break;

            case BEEP_DELAY:
                HAL_Delay(table->value);
                break;

            case BEEP_FREQ:
                freq = table->value;
                break;

            case BEEP_LENGTH:
                length = table->value;
                break;
        }
    }

    return retval;
}
