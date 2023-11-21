/**
 * @file CPSQ8100.h
 * @author Liu Yuanlin (liuyuanlins@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-17
 * @last modified 2023-11-17
 *
 * @copyright Copyright (c) 2023 Liu Yuanlin Personal.
 *
 */
#ifndef CPSQ8100_H
#define CPSQ8100_H

#ifdef __cplusplus
extern "C"
{
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


    struct tagCPSQ8100_RxHeader
    {
        uint8_t subtype;
        uint8_t cmd;
        uint16_t bodyLen;
        uint8_t *pBody;
        uint8_t checksum;
    };

#define CPSQ8100_OK 0x00
#define CPSQ8100_BUSY 0x01

#define CPSQ8100_SUBTYPE_REQUEST 0x00
#define CPSQ8100_SUBTYPE_ACK 0x01
#define CPSQ8100_SUBTYPE_NACK 0x02

    void CPSQ8100_Init(void);
    void CPSQ8100_Poll(void);
    uint8_t CPSQ8100_IsIdle(void);
    void CPSQ8100_RestSM();
    void CPSQ8100_RecevieCallback(struct tagCPSQ8100_RxHeader *pHeader);
    uint8_t CPSQ8100_SendRequest(uint8_t cmd, uint16_t bodyLen, uint8_t *body);
    uint8_t CPSQ8100_ReadRegisterRequest(uint16_t regAddr, uint16_t dataSize);
    uint8_t CPSQ8100_WriteRegisterRequest(uint16_t regAddr, uint16_t dataSize, uint8_t *pData);

    uint8_t CPSQ8100_WriteRegisterRequest_Uint32(uint16_t regAddr, uint32_t data);
    uint8_t CPSQ8100_WriteRegisterRequest_Uint16(uint16_t regAddr, uint16_t data);

  uint8_t CPSQ8100_Send(uint8_t cmd, uint16_t bodyLen, uint8_t *body);
    uint8_t CPSQ8100_WriteRegister_Uint32(uint16_t regAddr, uint32_t data);
    uint8_t CPSQ8100_WriteRegister_Uint16(uint16_t regAddr, uint16_t data);


#define CPSQ8100_CMD_MCU_RESET 0x05     // MCU 复位命令：0x05
#define CPSQ8100_REG_W_CMD 0x06         // 寄存器写入命令：0x06
#define CPSQ8100_REG_R_CMD 0x07         // 寄存器读取命令：0x07
#define CPSQ8100_CMD_STATUS_REPORT 0x08 // 状态上报命令：0x08
#define CPSQ8100_CMD_Q_CALIBRATION 0x09 // Q 值校准命令：0x09
#define CPSQ8100_CMD_FLASH_READ 0x0A    // Flash 读取命令：0x0A

// 寄存器地址定义,寄存器的大小可以从返回响应中得到，解析方式在代码中实现
#define CPSQ8100_REG_STATUS_CHARGE 0x01B8
#define CPSQ8100_REG_BATTERY_LEVEL 0x01CC

#define RP_VAL 0x0190
#define RP_POWER 0x0194
#define PA_POWER 0x0198
#define PLOSS_VAL 0x019C
#define Q_REF 0x019E
#define QR_VAL 0x01A0
#define QM_VAL 0x01A1
#define FOD_CNT 0x01A2
#define FUNC_EN 0x003C
#define COIL_EN 0x00B8 
#define POWER_LIMIT 0x01D0
#define OVP_UVP_THRSH 0x0044
#define OTP_THRSH 0x0048
#define OTHER_PROTECT_THRSH 0x004C
#define BIGMETAL_THRSH 0x0040
#define EPP_Q_FOD_THRSH 0x008C
#define MISC_CONFIG 0x0090
#define PING_PARAM1 0x0094
#define CONFIG_META2 0x0024

#define ADC_TEMP1 0x0180
#define ADC_TEMP2 0x0182

#define HAUWEI_PLOSS_FOD_PARAM 0x0060
/*
7:0	HAUWEI_PLOSS_FOD_PARAM_K0
23:8	HAUWEI_PLOSS_FOD_PARAM_K1
31:24	HAUWEI_PLOSS_FOD_PARAM_K2
*/
#define HAUWEI_PLOSS_FOD_PARAM_VALUE(K0, K1, K2) ((K0) | ((K1) << 8) | ((K2) << 24))
#define WORK_VPA_RANGE_MIN 0x002C
#define WORK_VPA_RANGE_MAX 0x002E

#define ADC_VPA1 0x0174
#define ADC_IPA1 0x0178
#define ADC_VPK1 0x017C

#define CURR_Q1_VALUE 0x01BC
#define CURR_Q2_VALUE 0x01C0
#define CURR_Q3_VALUE 0x01C4
#ifdef __cplusplus
}
#endif
#endif //! CPSQ8100_H
