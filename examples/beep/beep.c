#include <bfdev.h>
#include "beep.h"
#include "py32f0xx_hal.h"

#define BEEP_TIMER TIM1
#define BEEP_CHANNEL TIM_CHANNEL_2

static int
buzzer_play(unsigned int freq, unsigned int length)
{
    TIM_HandleTypeDef TimHandle;
    TIM_OC_InitTypeDef sConfig;
    uint32_t period_cycles;
    uint16_t prescaler, overflow;

    period_cycles = 24000000 / freq;
    prescaler = period_cycles / 0xffff + 1;
    overflow = BFDEV_DIV_ROUND_CLOSEST(period_cycles, prescaler);

    TimHandle.Instance = BEEP_TIMER;
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    TimHandle.Init.Prescaler = prescaler;
    TimHandle.Init.Period = overflow;

    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfig.Pulse = overflow >> 1;

    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
        return -1;

    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, BEEP_CHANNEL) != HAL_OK)
        return -1;

    if (HAL_TIM_PWM_Start(&TimHandle, BEEP_CHANNEL) != HAL_OK)
        return -1;

    HAL_Delay(length);
    if (HAL_TIM_PWM_Stop(&TimHandle, BEEP_CHANNEL) != HAL_OK)
        return -1;

    return 0;
}

int
beep(const struct beep_table *table)
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
