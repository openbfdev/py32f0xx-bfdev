#include "py32f0xx_hal.h"
#include "py32f0xx_it.h"

void NMI_Handler(void)
{
    /* Nothing */
}

void HardFault_Handler(void)
{
    while (1);
}

void SVC_Handler(void)
{
    /* Nothing */
}

void PendSV_Handler(void)
{
    /* Nothing */
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
