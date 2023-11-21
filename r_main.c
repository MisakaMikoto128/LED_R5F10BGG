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
* Creation Date: 2023/11/22
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
#include "r_cg_adc.h"
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
#include "Screen.h"

void tk_v0t_drvinit(void);
uint8_t period_query_user(PeriodREC_t *period_recorder, PeriodREC_t period);
void CPSQ8100_Config();
uint32_t g_requestIdx = 0;

struct tagCPSQ8100_Request
{
    uint16_t regAddr;
    uint16_t dataSize;
};

struct tagCPSQ8100_Request requestList[] = {
    {CPSQ8100_REG_BATTERY_LEVEL, 0x0001},
    {CPSQ8100_REG_STATUS_CHARGE, 0x0004},
    {RP_VAL, 0x0004},
    {RP_POWER, 0x0004},
    {PA_POWER, 0x0004},
    {PLOSS_VAL, 0x0002},
    {Q_REF, 0x0002},
    // {QR_VAL, 0x0001},
    // {QM_VAL, 0x0001},
    {FOD_CNT, 0x0001},
    //{FUNC_EN, 0x0004},
    // {COIL_EN, 0x0004},
    {POWER_LIMIT, 0x0004},
    //{OVP_UVP_THRSH, 0x0004},
    //{OTP_THRSH, 0x0004},
    //{OTHER_PROTECT_THRSH, 0x0004},
    // {BIGMETAL_THRSH, 0x0004},
    // {EPP_Q_FOD_THRSH, 0x0004},
    // {MISC_CONFIG, 0x0004},
    {CONFIG_META2, 0x0004},
    {ADC_TEMP1, 0x0002},
    {ADC_TEMP2, 0x0002},
    //    {ADC_VPA1, 0x0002},
    //    {ADC_IPA1, 0x0002},
    //    {ADC_VPK1, 0x0002},

    //    {CURR_Q1_VALUE, 0x0004},
    //    {CURR_Q2_VALUE, 0x0004},
    //    {CURR_Q3_VALUE, 0x0004},
};

struct WireLessChargerParam
{
    uint32_t RP_VAL_;
    uint32_t RP_POWER_;
    uint32_t PA_POWER_;
    uint16_t PLOSS_VAL_;
    uint16_t Q_REF_;
    uint8_t TX_CHARGING;
    uint8_t RX_NOT_CENTER;
    uint8_t ENTER_HUAWEI;
    uint8_t ENTER_XIAOMI;
    uint8_t ENTER_OPPO;
    uint8_t FIND_RX;
    uint8_t CHARGE_FULL;
    uint8_t EPP_CALI_FAIL;
    uint8_t CTRL_REMOVE;
    uint8_t REQUST_EN_MOS;
    uint8_t REQUST_DIS_MOS;
    uint8_t WORK_COIL;
    uint8_t QI_STATE;

    uint8_t OPP_LIMIT;
    uint8_t OCP_LIMIT;
    uint8_t OVPK_LIMIT;
    uint8_t BatteryLevel_;

    uint16_t NTC1;
    uint16_t NTC2;

    uint32_t Efficiency;
};

struct WireLessChargerParam g_WireLessChargerParam = {0};
uint8_t protectFlag = 0;
uint8_t phoneBrand = 0;
uint8_t chargeMode = 0;

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
    FDCAN_TxHeaderTypeDef txHeader;
    FDCAN_RxHeaderTypeDef rxHeader;
    uint8_t rxData[32];
    uint32_t comReadLen = 0;
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    HDL_CPU_Time_Init();
    P3.1 = 0; //CPSQ8100 INT Enable
    CAN_Init();
    Uart_Init(COM1, 115200, 8, 1, 0);

    CPSQ8100_Init();
    Screen_Init(0x02);
    CPSQ8100_Config();
    while (1U)
    {
        // if (period_query_user(&sec_1, 500) != 0)
        // {
        //     P8 .5 = ~P8 .5;
        //     printf("Ya Ya!");
        //     tk_can_send(1,1,2,3,4,5,5);
        // }

        // if (period_query_user(&sec_2, 2000) != 0)
        // {
        //     P8 .4 = 0;
        // }

        // canRet = R_CAN_ReadRxFIFO(0, &canRevFrame);
        // if (canRet != CAN_RTN_BUFFER_EMPTY)
        // {
        //     P8 .4 = 1;
        //     R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &canRevFrame);
        // }

        if (CAN_Read(&rxHeader) == HAL_OK)
        {
            
            CAN_Write(&txHeader, rxHeader.Data);
        }

        if (period_query_user(&sec_1, 500) != 0)
        {
            txHeader.DataLength = 3;
            txHeader.Identifier = 0x123;
            txHeader.IdType = FDCAN_STANDARD_ID;
            txHeader.TxFrameType = FDCAN_DATA_FRAME;
            CAN_Write(&txHeader, (uint8_t *)"Ya Ya!");
        }

        if (period_query_user(&sec_2, 2000) != 0)
        {
            P8.4 = 0;
        }

        if (Uart_AvailableBytes(COM1) > 4)
        {
            comReadLen = Uart_Read(COM1, rxData, 64);
            Uart_Write(COM1, rxData, comReadLen);
        }

        if (CPSQ8100_IsIdle())
        {
            if (g_requestIdx < sizeof(requestList) / sizeof(struct tagCPSQ8100_Request))
            {
                CPSQ8100_ReadRegisterRequest(requestList[g_requestIdx].regAddr, requestList[g_requestIdx].dataSize);
            }
            else
            {
                g_requestIdx = 0;

                if (g_WireLessChargerParam.FIND_RX == 0)
                {
                    Screen_SetPhoneNotExist();
                }
                else
                {
                    phoneBrand = g_WireLessChargerParam.ENTER_HUAWEI != 0 ? SCREEN_PHONE_BRAND_HUAWEI : SCREEN_PHONE_BRAND_OTHER;
                    phoneBrand = g_WireLessChargerParam.ENTER_XIAOMI != 0 ? SCREEN_PHONE_BRAND_XIAOMI : phoneBrand;
                    phoneBrand = g_WireLessChargerParam.ENTER_OPPO != 0 ? SCREEN_PHONE_BRAND_OPPO : phoneBrand;

                    chargeMode = phoneBrand == SCREEN_PHONE_BRAND_OTHER ? SEREEN_CHARGER_MODE_NORMAL : SEREEN_CHARGER_MODE_SUPER;

                    Screen_SetPhoneExistFlag(100, phoneBrand, chargeMode);
                    Screen_SetPhonePAPower(g_WireLessChargerParam.PA_POWER_);
                    Screen_SetPhoneRPPower(g_WireLessChargerParam.RP_POWER_);

                    if (g_WireLessChargerParam.PA_POWER_ > 0)
                    {
                        g_WireLessChargerParam.Efficiency = g_WireLessChargerParam.RP_POWER_ * 1000.0f / g_WireLessChargerParam.PA_POWER_;
                        Screen_SetChargeEfficiency(g_WireLessChargerParam.Efficiency * 100);
                    }

                    protectFlag = 0;
                    protectFlag |= (g_WireLessChargerParam.OPP_LIMIT != 0 ? SEREEN_OP_PROTECT_FLAG : 0);
                    protectFlag |= (g_WireLessChargerParam.OCP_LIMIT != 0 ? SEREEN_OC_PROTECT_FLAG : 0);
                    protectFlag |= (g_WireLessChargerParam.OVPK_LIMIT != 0 ? SEREEN_OV_PROTECT_FLAG : 0);
                    protectFlag |= (g_WireLessChargerParam.EPP_CALI_FAIL != 0 ? SEREEN_FOD_PROTECT_FLAG : 0);

                    //%60也判断为有异物
                    if (g_WireLessChargerParam.Efficiency < 600)
                    {
                        protectFlag |= SEREEN_FOD_PROTECT_FLAG;
                    }

                    Screen_SetProtectFlag(protectFlag);
                }
            }
        }

        if (CAN_Read(&rxHeader) == HAL_OK)
        {
        }
        CPSQ8100_Poll();
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

    R_ADC_Create();
    R_ADC_Start();

    tk_v0s_start();
    tk_v0t_drvinit();

    P1.2 = 0; // enable can chip
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

uint16_t UartQueueLength(void);
// 循环队列
#define UART_RX_QUEUE_SIZE 256
uint8_t g_uartRxQueue[UART_RX_QUEUE_SIZE];
uint16_t g_uartRxQueueHead = 0;
uint16_t g_uartRxQueueTail = 0;
// 循环队列初始化
void UartQueueInit(void)
{
    g_uartRxQueueHead = 0;
    g_uartRxQueueTail = 0;
}

#define UartQueueIsEmpty() (g_uartRxQueueHead == g_uartRxQueueTail)
#define UartQueueIsFull() ((g_uartRxQueueTail + 1) % UART_RX_QUEUE_SIZE == g_uartRxQueueHead)

void UartQueuPush(uint8_t data)
{
    if (UartQueueIsFull())
    {
        return;
    }
    g_uartRxQueue[g_uartRxQueueTail] = data;
    g_uartRxQueueTail = (g_uartRxQueueTail + 1) % UART_RX_QUEUE_SIZE;
}

uint8_t UartQueuePop(void)
{
    uint8_t data = 0;
    if (UartQueueIsEmpty())
    {
        return 0;
    }
    data = g_uartRxQueue[g_uartRxQueueHead];
    g_uartRxQueueHead = (g_uartRxQueueHead + 1) % UART_RX_QUEUE_SIZE;
    return data;
}

uint16_t UartQueueLength(void)
{
    return (g_uartRxQueueTail - g_uartRxQueueHead + UART_RX_QUEUE_SIZE) % UART_RX_QUEUE_SIZE;
}

#include "HDL_Uart.h"

void Uart_Init(COMID_t comId, uint32_t baud, uint32_t wordLen, uint32_t stopBit, uint32_t parity)
{
}

uint32_t Uart_Write(COMID_t comId, const uint8_t *writeBuf, uint32_t uLen)
{
    R_UART0_Send(writeBuf, (uint16_t)uLen);
    return 0;
}

uint32_t Uart_Read(COMID_t comId, uint8_t *pBuf, uint32_t uiLen)
{
    uint32_t ret = 0;
    for (;;)
    {
        if (UartQueueIsEmpty())
        {
            break;
        }
        *pBuf = UartQueuePop();
        pBuf++;
        ret++;
        if (ret >= uiLen)
        {
            break;
        }
    }
    return ret;
}

uint32_t Uart_AvailableBytes(COMID_t comId)
{
    return UartQueueLength();
}

void CPSQ8100_Config()
{
    CPSQ8100_WriteRegister_Uint32(FUNC_EN, 0x0007CF7A);
    CPSQ8100_WriteRegister_Uint32(COIL_EN, 0x00FFFFFF);
    // CPSQ8100_WriteRegister_Uint32(OTHER_PROTECT_THRSH, 0xFFFFFF01);
    // CPSQ8100_WriteRegister_Uint32(EPP_Q_FOD_THRSH, 0x58504B1E);
    CPSQ8100_WriteRegister_Uint32(CONFIG_META2, 0xAABBCC01);
    // 华为HAUWEI_PLOSS_FOD_PARAM
    CPSQ8100_WriteRegister_Uint32(HAUWEI_PLOSS_FOD_PARAM, HAUWEI_PLOSS_FOD_PARAM_VALUE(250, 65000, 120));
    CPSQ8100_WriteRegister_Uint16(WORK_VPA_RANGE_MAX, 0xB580);
}

void CPSQ8100_RecevieCallback(struct tagCPSQ8100_RxHeader *pHeader)
{
    // for CPSQ8100_CMD_MCU_RESET

    // for CPSQ8100_REG_W_CMD

    // for CPSQ8100_REG_R_CMD
    uint32_t regAddr = 0;
    uint32_t dataSize = 0;
    uint32_t rRegU32 = 0;
    uint32_t rRegU16 = 0;
    uint32_t rRegU8 = 0;
    int32_t rRegI32 = 0;
    int32_t rRegI16 = 0;
    int32_t rRegI8 = 0;

    if (pHeader->subtype != CPSQ8100_SUBTYPE_ACK)
    {
        return;
    }

    switch (pHeader->cmd)
    {
    case CPSQ8100_CMD_MCU_RESET:
        break;
    case CPSQ8100_REG_W_CMD:
        break;
    case CPSQ8100_REG_R_CMD:
    {
        g_requestIdx++;

        regAddr = pHeader->pBody[0] + (pHeader->pBody[1] << 8);
        dataSize = pHeader->pBody[2] + (pHeader->pBody[3] << 8);
        if (dataSize == 4)
        {
            rRegU32 = pHeader->pBody[4] + (pHeader->pBody[5] << 8) + (pHeader->pBody[6] << 16) + (pHeader->pBody[7] << 24);
            rRegI32 = (int32_t)rRegU32;
        }
        else if (dataSize == 2)
        {
            rRegU16 = pHeader->pBody[4] + (pHeader->pBody[5] << 8);
            rRegI16 = (int16_t)rRegU16;
        }
        else if (dataSize == 1)
        {
            rRegU8 = pHeader->pBody[4];
            rRegI8 = (int8_t)rRegU8;
        }

        switch (regAddr)
        {
        case CPSQ8100_REG_BATTERY_LEVEL:
        {
            g_WireLessChargerParam.BatteryLevel_ = rRegU8;
        }
        break;
        case CPSQ8100_REG_STATUS_CHARGE:
        {
            /*
            bits Label

            0	TX_CHARGING
                1	RX_NOT_CENTER
                2	ENTER_HUAWEI
                3	ENTER_XIAOMI
                4	ENTER_OPPO
                5	FIND_RX
                6	CHARGE_FULL
                7	EPP_CALI_FAIL
                8	CTRL_REMOVE
                9	REQUST_EN_MOS
                10	REQUST_DIS_MOS
                15:11	WORK_COIL
                23:16	QI_STATE
                31:24	CURR_ERRCODE
                    */
            g_WireLessChargerParam.TX_CHARGING = (rRegU32 >> 0) & 0x01;
            g_WireLessChargerParam.RX_NOT_CENTER = (rRegU32 >> 1) & 0x01;
            g_WireLessChargerParam.ENTER_HUAWEI = (rRegU32 >> 2) & 0x01;
            g_WireLessChargerParam.ENTER_XIAOMI = (rRegU32 >> 3) & 0x01;
            g_WireLessChargerParam.ENTER_OPPO = (rRegU32 >> 4) & 0x01;
            g_WireLessChargerParam.FIND_RX = (rRegU32 >> 5) & 0x01;
            g_WireLessChargerParam.CHARGE_FULL = (rRegU32 >> 6) & 0x01;
            g_WireLessChargerParam.EPP_CALI_FAIL = (rRegU32 >> 7) & 0x01;
            g_WireLessChargerParam.CTRL_REMOVE = (rRegU32 >> 8) & 0x01;
            g_WireLessChargerParam.REQUST_EN_MOS = (rRegU32 >> 9) & 0x01;
            g_WireLessChargerParam.REQUST_DIS_MOS = (rRegU32 >> 10) & 0x01;
            g_WireLessChargerParam.WORK_COIL = (rRegU32 >> 11) & 0x1F;
            g_WireLessChargerParam.QI_STATE = (rRegU32 >> 16) & 0xFF;
        }
        break;
        case RP_VAL:
        {
            g_WireLessChargerParam.RP_VAL_ = rRegU32;
        }
        break;
        case RP_POWER:
        {
            g_WireLessChargerParam.RP_POWER_ = rRegU32;
        }
        break;
        case PA_POWER:
        {
            g_WireLessChargerParam.PA_POWER_ = rRegU32;
        }
        break;
        case PLOSS_VAL:
        {
            g_WireLessChargerParam.PLOSS_VAL_ = rRegI16;
        }
        break;
        case Q_REF:
        {
            g_WireLessChargerParam.Q_REF_ = rRegU16;
        }
        break;
        case POWER_LIMIT:
        {
            /*
            0	OPP_LIMIT
            1	OCP_LIMIT
            2	OVPK_LIMIT
            31:3	Reserved
            */
            g_WireLessChargerParam.OPP_LIMIT = (rRegU32 >> 0) & 0x01;
            g_WireLessChargerParam.OCP_LIMIT = (rRegU32 >> 1) & 0x01;
            g_WireLessChargerParam.OVPK_LIMIT = (rRegU32 >> 2) & 0x01;
        }
        break;

        case CONFIG_META2:
        {
            if ((rRegU32 & 0xFFFFFF00) != 0xAABBCC00)
            {
                CPSQ8100_Config();
            }
        }
        case ADC_TEMP1:
        {
            g_WireLessChargerParam.NTC1 = rRegU16;
        }
        break;
        case ADC_TEMP2:
        {
            g_WireLessChargerParam.NTC2 = rRegU16;
        }
        break;
        default:
            break;
        }
    }
    break;
    case CPSQ8100_CMD_STATUS_REPORT:
        break;
    case CPSQ8100_CMD_Q_CALIBRATION:
        break;
    case CPSQ8100_CMD_FLASH_READ:
        break;
    default:
        break;
    }
}

/* End user code. Do not edit comment generated here */
