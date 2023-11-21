/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
 * LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
 * ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
 * of this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2012, 2021 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_main.c
 * Version      : CodeGenerator for RL78/F13 V2.03.07.02 [08 Nov 2021]
 * Device(s)    : R5F10BGG
 * Tool-Chain   : CA78K0R
 * Description  : This file implements main function.
 * Creation Date: 2023/11/10
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include <stdio.h>
#include <string.h>
#include "r_rl78_can_drv.h" /* Header file for CAN software driver */
#include "tk_header.h"      /* Header file for can sample program */
#include "HDL_CPU_Time.h"

#include "HDL_Uart.h"
#include "ccan.h"
#include "CPSQ8100.h"

void tk_v0t_drvinit(void);
uint8_t period_query_user(PeriodREC_t *period_recorder, PeriodREC_t period);
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void main(void)
{
    PeriodREC_t sec_1 = 0;
    PeriodREC_t sec_2 = 0;
    Can_RtnType canRet = CAN_RTN_OK;
    can_frame_t canRevFrame;
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    printf("Ya Ya!");
    CAN_Init();

    while (1U)
    {
        if (period_query_user(&sec_1, 500) != 0)
        {
            P8 .5 = ~P8 .5;
            // printf("Ya Ya!");
            // tk_can_send(1,1,2,3,4,5,5);
        }
        if (period_query_user(&sec_2, 2000) != 0)
        {
            P8 .4 = 0;
        }
        canRet = R_CAN_ReadRxFIFO(0, &canRevFrame);
        if (canRet != CAN_RTN_BUFFER_EMPTY)
        {
            P8 .4 = 1;
            R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &canRevFrame);
        }
        R_WDT_Restart();
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    P8 = 0x00;
    R_WDT_Create();
    R_TAU0_Create();
    R_TAU0_Channel0_Start();

    R_SAU0_Create();
    R_UART0_Start();

    tk_v0s_start();
    tk_v0t_drvinit();

    P1 .2 = 0; // enable can chip
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void tk_can_send(uint8_t ID, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4, uint8_t data5)
{
    can_frame_t frame;

    frame.IDE = 0;   /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;   /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0; /* Transmit History Data Store Enable */
    frame.IDL = ID;  /* ID Data (low)                      */
    frame.IDH = 0;   /* ID Data (high)                     */
    frame.DLC = 5;   /* DLC Data                           */
    frame.LBL = 0;   /* Label Data                         */
    frame.TS = 0;    /* Timestamp Data                     */
    memset(frame.DB, 0, 8);
    frame.DB[0] = data0; /* Data Byte                          */
    frame.DB[1] = data1; /* Data Byte                          */
    frame.DB[2] = data2; /* Data Byte                          */
    frame.DB[3] = data3;
    frame.DB[4] = data4;
    frame.DB[5] = data5;

    R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
}

void R_CAN_Create(void)
{
    PER2 |= 0x01; // Enable the CAN module. CAN0EN = 1;
}

/******************************************************************************
 * Function Name: tk_v0t_drvinit
 * Description  : CAN initialize operation (CAN software driver)
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void tk_v0t_drvinit(void)
{
    Can_RtnType retval;

    /* supply CAN clock */
    R_CAN_Create(); /* CAN0EN = 1    */

    /* CAN Initialize */
    retval = CAN_RTN_RAM_INIT;
    while (retval != CAN_RTN_OK)
    {
        retval = R_CAN_Init();
    }

    /* Set global operating mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT)
    {
        retval = R_CAN_GlobalStart();
    }

    /* CH0 -> Channel communication mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT)
    {
        retval = R_CAN_ChStart(CAN_CH0);
    }
}

/**
 * @brief 同period_query_user，只是时间记录再一个uint16_t*指向的变量中。
 *
 * @param period_recorder 记录运行时间的变量的指针。
 * @param period 周期,单位tick。
 * @return true 周期到了
 * @return false 周期未到。
 */
uint8_t period_query_user(PeriodREC_t *period_recorder, PeriodREC_t period)
{
    uint8_t ret = 0;

    // 这里一定是>=，如果是 > ，那么在1 cpu tick间隔的时候时间上是2cpu tick执行一次。
    // 这里不允许period为0，不然就会失去调度作用。
    if ((HDL_CPU_Time_GetTick() - *period_recorder) >= period)
    {
        *period_recorder = HDL_CPU_Time_GetTick();
        ret = 1;
    }
    return ret;
}

#include "ccan.h"

void CAN_Init()
{
}

HAL_StatusTypeDef CAN_Write(FDCAN_TxHeaderTypeDef *pTxHeader,
                            uint8_t *pTxData)
{
    HAL_StatusTypeDef ret = HAL_OK;
    Can_RtnType canRet = CAN_RTN_OK;
    can_frame_t frame;

    frame.IDE = pTxHeader->IdType == FDCAN_STANDARD_ID ? 0 : 1;     /* IDE  0:Standard 1:Extend           */
    frame.RTR = pTxHeader->TxFrameType == FDCAN_DATA_FRAME ? 0 : 1; /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;                                                /* Transmit History Data Store Enable */
    frame.IDL = pTxHeader->Identifier & 0xFFFF;                     /* ID Data (low)                      */
    frame.IDH = (pTxHeader->Identifier >> 16) & 0x1FFF;             /* ID Data (high)                     */
    frame.DLC = pTxHeader->DataLength;                              /* DLC Data                           */
    frame.LBL = 0;                                                  /* Label Data                         */
    frame.TS = 0;                                                   /* Timestamp Data                     */
    memcpy(frame.DB, pTxData, 8);

    canRet = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
    if (canRet != CAN_RTN_OK)
    {
        ret = HAL_ERROR;
    }
    else
    {
        ret = HAL_OK;
    }

    return ret;
}

HAL_StatusTypeDef CAN_Read(FDCAN_RxHeaderTypeDef *pRxHeader)
{
    HAL_StatusTypeDef ret = HAL_OK;
    Can_RtnType canRet = CAN_RTN_OK;
    can_frame_t canRevFrame;
    canRet = R_CAN_ReadRxFIFO(0, &canRevFrame);
    if (canRet != CAN_RTN_BUFFER_EMPTY)
    {
        R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &canRevFrame);
        pRxHeader->DataLength = canRevFrame.DLC;
        pRxHeader->Identifier = canRevFrame.IDL;
        pRxHeader->RxFrameType = canRevFrame.RTR == 0 ? FDCAN_DATA_FRAME : FDCAN_REMOTE_FRAME;
        pRxHeader->RxTimestamp = canRevFrame.TS;
        memcpy(pRxHeader->Data, canRevFrame.DB, 8);
        pRxHeader->IdType = canRevFrame.IDE == 0 ? FDCAN_STANDARD_ID : FDCAN_EXTENDED_ID;
        ret = HAL_OK;
    }
    else
    {
        ret = HAL_ERROR;
    }

    return ret;
}

#include "HDL_Uart.h"

void Uart_Init(COMID_t comId, uint32_t baud, uint32_t wordLen, uint32_t stopBit, uint32_t parity)
{
}

uint32_t Uart_Write(COMID_t comId, const uint8_t *writeBuf, uint32_t uLen)
{
    R_UART0_Send(writeBuf,(uint16_t)uLen);
    return 0;
}

uint32_t Uart_Read(COMID_t comId, uint8_t *pBuf, uint32_t uiLen)
{
    return 0;
}

uint32_t Uart_AvailableBytes(COMID_t comId)
{
    return 0;
}
/* End user code. Do not edit comment generated here */
