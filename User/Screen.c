/**
 * @file Screen.c
 * @author Liu Yuanlin (liuyuanlins@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-21
 * @last modified 2023-11-21
 *
 * @copyright Copyright (c) 2023 Liu Yuanlin Personal.
 *
 */
#include "Screen.h"
#include "ccan.h"


static uint8_t SCREEN_CAN_ID = 0x01;
void Screen_Init(uint8_t screenCANID)
{
    SCREEN_CAN_ID = screenCANID;
    // TODO
    CAN_Init();
    Screen_SetPhoneNotExist();
}

/**
 * @brief
 *
 * @param flag @ProtectFlag
 */
void Screen_SetProtectFlag(uint8_t flag)
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_PROTECT_FLAG;
    txData[1] = (flag & SEREEN_FOD_PROTECT_FLAG) ? 1 : 0;
    txData[2] = (flag & SEREEN_UNDERVOLTAGE_PROTECT_FLAG) ? 1 : 0;
    txData[3] = (flag & SEREEN_OC_PROTECT_FLAG) ? 1 : 0;
    txData[4] = (flag & SEREEN_OV_PROTECT_FLAG) ? 1 : 0;
    txData[5] = (flag & SEREEN_OT_PROTECT_FLAG) ? 1 : 0;
    txData[6] = (flag & SEREEN_OP_PROTECT_FLAG) ? 1 : 0;
    txData[7] = (flag & SEREEN_NO_PROTECT_FLAG) ? 1 : 0;
    CAN_Write(&txHeader, txData);
}

/**
 * @brief
 *
 * @param batteryLevel 0-100，示例暂时大于0有动画，等于0无动画
 * @param phoneBrand
 * @param chargeMode 0 普通快充 1 超级快充
 */
void Screen_SetPhoneExistFlag(uint8_t batteryLevel, uint8_t phoneBrand, uint8_t chargeMode)
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_PHONE_EXIST_FLAG;
    txData[1] = batteryLevel > 0 ? 1 : 0;
    txData[2] = phoneBrand;
    txData[3] = chargeMode;
    CAN_Write(&txHeader, txData);
}

void Screen_SetPhoneNotExist()
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_NO_PHONE;
    CAN_Write(&txHeader, txData);
}

/**
 * @brief
 *
 * @param power 单位mW
 */
void Screen_SetPhoneRPPower(uint32_t power)
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_PHONE_RP_POWER;
    txData[1] = power & 0xFF;
    txData[2] = (power >> 8) & 0xFF;
    txData[3] = (power >> 16) & 0xFF;
    txData[4] = (power >> 24) & 0xFF;
    CAN_Write(&txHeader, txData);
}

/**
 * @brief
 *
 * @param power 单位mW
 */
void Screen_SetPhonePAPower(uint32_t power)
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_PHONE_PA_POWER;
    txData[1] = power & 0xFF;
    txData[2] = (power >> 8) & 0xFF;
    txData[3] = (power >> 16) & 0xFF;
    txData[4] = (power >> 24) & 0xFF;
    CAN_Write(&txHeader, txData);
}

/**
 * @brief
 *
 * @param efficiency 单位1%*1000
 */
void Screen_SetChargeEfficiency(uint32_t efficiency)
{
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[8];
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.Identifier = SCREEN_CAN_ID;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    txData[0] = SCREEN_WIGET_CHARGE_EFFICIENCY;
    txData[1] = efficiency & 0xFF;
    txData[2] = (efficiency >> 8) & 0xFF;
    txData[3] = (efficiency >> 16) & 0xFF;
    txData[4] = (efficiency >> 24) & 0xFF;
    CAN_Write(&txHeader, txData);
}
