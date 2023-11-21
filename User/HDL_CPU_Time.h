/**
 * @file HDL_CPU_Time.h
 * @author Liu Yuanlin (liuyuanlins@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-08
 * 
 * @copyright Copyright (c) 2022 Liu Yuanlin Personal.
 * 
 */

#ifndef HDL_CPU_TIME_H
#define HDL_CPU_TIME_H

#ifndef __TYPEDEF__
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;
typedef signed long         int32_t;
typedef unsigned long       uint32_t;
typedef unsigned short      uint16_t;
#define __TYPEDEF__
#endif

void HDL_CPU_Time_Init();
uint16_t HDL_CPU_Time_GetTick();
void HDL_CPU_IncTick(void);
void HDL_CPU_Time_ResetTick();
void HDL_CPU_Time_DelayMs(uint16_t DelayMs);

#define HDL_CPU_TIME_OEN_TICK_TIME 1000ULL // 1000 us

#define HDL_TIME_TO_TICK(_time) ((((_time)*1000ULL) / HDL_CPU_TIME_OEN_TICK_TIME))


#define HDL_TICK_TO_TIME(_tick) ((((_time)*HDL_CPU_TIME_OEN_TICK_TIME) / 1000ULL))

typedef uint16_t PeriodREC_t;

uint8_t period_query_user(PeriodREC_t *period_recorder, PeriodREC_t period);
#endif //!HDL_CPU_TIME_H
