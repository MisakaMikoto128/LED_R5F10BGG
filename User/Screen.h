/**
* @file Screen.h
* @author Liu Yuanlin (liuyuanlins@outlook.com)
* @brief
* @version 0.1
* @date 2023-11-21
* @last modified 2023-11-21
*
* @copyright Copyright (c) 2023 Liu Yuanlin Personal.
*
*/
#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

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

/*
local NXPID = 0X01
local RenasasID = 0x02
local AutochipsID = 0x03

local gWigetList = {
    PhoneExistFlag = 0x01,
    ProtectFlag = 0x02,
    PhoneRPPower = 0x03,
    PhonePAPower = 0x04,
    ChargeEfficiency = 0x05,
    ChargeMode = 0x06,
    NoPhone = 0xFF,
}

local gPhoneBrand =
{
    [0x00] = "OtherPhone",
    [0x01] = "Huawei",
    [0x02] = "Xiaomi",
    [0x03] = "OPPO",
    [0x04] = "VIVO",
    [0x05] = "Apple",
    [0x06] = "Samsung",
}
    elseif AutochipsID == identifier then
        if data[0] == gWigetList.NoPhone then
            set_text(0, 30, "待测")
            set_text(0, 32, "待测")
            set_text(0, 33, "待测")
            set_text(0, 34, "待测")
            set_text(0, 35, "待测")
            stop_animation(0, 31)
            stop_animation(0, 36)
            stop_animation(0, 37)
            stop_animation(0, 38)
            stop_animation(0, 39)
            stop_animation(0, 40)
            stop_animation(0, 41)
        elseif data[0] == gWigetList.PhoneExistFlag then
            -- 电池
            set_animation(0, 31, data[1])
            -- 手机品牌
            set_text(0, 30, gPhoneBrand[data[2]])
            -- 充电模式
            if data[3] == 0 then
                set_text(0, 35, "普通快充")
            elseif data[3] == 1 then
                set_text(0, 35, "超级快充")
            end
        elseif data[0] == gWigetList.PhonePAPower then
            lPA_Power = 0
            lPA_Power = (data[1]| data[2] << 8 | data[3] << 16 | data[4] << 24) / 1000.00
            set_text(0, 32, lPA_Power)
        elseif data[0] == gWigetList.PhoneRPPower then
            lRP_Power = 0
            lRP_Power = (data[1]| data[2] << 8| data[3] << 16 | data[4] << 24) / 1000.00
            set_text(0, 33, lRP_Power)
        elseif data[0] == gWigetList.ChargeEfficiency then
            lEfficiency = 0
            lEfficiency = (data[1]| data[2] << 8| data[3] << 16 | data[4] << 24) / 1000.00
            set_text(0, 34, lEfficiency)
        elseif data[0] == gWigetList.ProtectFlag then
            -- FOD
            set_animation(0, 36, data[1])
            -- 欠压
            set_animation(0, 37, data[2])
            -- 过流
            set_animation(0, 38, data[3])
            -- 过压
            set_animation(0, 39, data[4])
            -- 过温
            set_animation(0, 40, data[5])
            -- 过功率
            set_animation(0, 41, data[6])
        end
    end
*/



//data[0]
#define SCREEN_WIGET_PHONE_EXIST_FLAG 0x01
#define SCREEN_WIGET_PROTECT_FLAG 0x02
#define SCREEN_WIGET_PHONE_RP_POWER 0x03
#define SCREEN_WIGET_PHONE_PA_POWER 0x04
#define SCREEN_WIGET_CHARGE_EFFICIENCY 0x05
#define SCREEN_WIGET_CHARGE_MODE 0x06
#define SCREEN_WIGET_NO_PHONE 0xFF

//Phone Brand
#define SCREEN_PHONE_BRAND_OTHER 0x00
#define SCREEN_PHONE_BRAND_HUAWEI 0x01
#define SCREEN_PHONE_BRAND_XIAOMI 0x02
#define SCREEN_PHONE_BRAND_OPPO 0x03
#define SCREEN_PHONE_BRAND_VIVO 0x04
#define SCREEN_PHONE_BRAND_APPLE 0x05
#define SCREEN_PHONE_BRAND_SAMSUNG 0x06

//@ProtectFlag
#define SEREEN_FOD_PROTECT_FLAG 0x01
#define SEREEN_UNDERVOLTAGE_PROTECT_FLAG 0x02
#define SEREEN_OC_PROTECT_FLAG 0x04
#define SEREEN_OV_PROTECT_FLAG 0x08
#define SEREEN_OT_PROTECT_FLAG 0x10
#define SEREEN_OP_PROTECT_FLAG 0x20
#define SEREEN_NO_PROTECT_FLAG 0x00

#define SEREEN_CHARGER_MODE_NORMAL 0x00
#define SEREEN_CHARGER_MODE_SUPER 0x01
void Screen_Init(uint8_t screenCANID);
/**
 * @brief 
 * 
 * @param flag @ProtectFlag
 */
void Screen_SetProtectFlag(uint8_t flag);

/**
 * @brief 
 * 
 * @param batteryLevel 0-100，示例暂时大于0有动画，等于0无动画
 * @param phoneBrand 
 * @param chargeMode 0 普通快充 1 超级快充
 */
void Screen_SetPhoneExistFlag(uint8_t batteryLevel, uint8_t phoneBrand, uint8_t chargeMode);

void Screen_SetPhoneNotExist();

/**
 * @brief 
 * 
 * @param power 单位mW
 */
void Screen_SetPhoneRPPower(uint32_t power);

/**
 * @brief 
 * 
 * @param power 单位mW
 */
void Screen_SetPhonePAPower(uint32_t power);

/**
 * @brief 
 * 
 * @param efficiency 单位1%*1000
 */
void Screen_SetChargeEfficiency(uint32_t efficiency);


#ifdef __cplusplus
}
#endif
#endif //!SCREEN_H
