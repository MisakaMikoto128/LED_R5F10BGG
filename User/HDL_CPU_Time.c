/**
 * @file HDL_CPU_Time.c
 * @author Liu Yuanlin (liuyuanlins@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022 Liu Yuanlin Personal.
 *
 */
#include "HDL_CPU_Time.h"

#define CPU_MAX_US_DELAY 0xFFFFUL

uint16_t uwCpuTick;


/**
 * @brief CPU滴答时钟初始化。
 *
 */
void HDL_CPU_Time_Init()
{

}

/**
 * @brief 获取CPU滴答时钟。
 *
 * @return uint16_t
 */
uint16_t HDL_CPU_Time_GetTick()
{
    return uwCpuTick;
}

/**
 * @brief 重置CPU滴答时钟。
 *
 */
void HDL_CPU_Time_ResetTick()
{
    uwCpuTick = 0;
}

/**
 * @brief This function is called to increment a global variable "uwTick"
 *        used as application time base.
 * @note In the default implementation, this variable is incremented each 1ms
 *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other
 *      implementations in user file.
 * @retval None
 */
void HDL_CPU_IncTick(void)
{
    uwCpuTick++;
}

/**
 * @brief This function provides minimum delay (in millisecond) based
 *       on variable incremented.
 *  @note 这个函数在中断中使用时必须保证调用这个函数的中断优先级低于CPU毫秒定时器中断的优先级。
 * @param DelayMs
 */
void HDL_CPU_Time_DelayMs(uint16_t DelayMs)
{
    uint16_t tickstart = HDL_CPU_Time_GetTick();
    uint16_t wait = DelayMs;

    while ((HDL_CPU_Time_GetTick() - tickstart) < wait)
    {
    }
}

