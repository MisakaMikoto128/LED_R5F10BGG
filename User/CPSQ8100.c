/**
 * @file CPSQ8100.c
 * @author Liu Yuanlin (liuyuanlins@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-17
 * @last modified 2023-11-17
 *
 * @copyright Copyright (c) 2023 Liu Yuanlin Personal.
 *
 */
#include "CPSQ8100.h"
#include "HDL_Uart.h"
#include "HDL_CPU_Time.h"
#include <stdio.h>

#define CPSQ8100_COM_ID COM2

#define CPSQ8100_STATE_IDLE 0x00
#define CPSQ8100_STATE_WAIT_ACK 0x01

#define CPSQ8100_RESPONSE_OK 0x00
#define CPSQ8100_RESPONSE_ERROR 0x01
#define CPSQ8100_RESPONSE_TIMEOUT 0x02

static uint8_t CPSQ8100_CalculateChecksum(const uint8_t *pdata, uint32_t len)
{
    uint32_t checksum = 0;
    uint32_t i = 0;
    uint8_t ret = 0;
    for (i = 0; i < len; ++i)
    {
        checksum += pdata[i];
    }
    ret = (uint8_t)(checksum & 0xFF);
    return ret;
}

static uint8_t g_cpsq8100_tx_buffer[32];
static uint8_t g_cpsq8100_rx_buffer[32];
static uint8_t g_cpsq8100_body_buffer[32];

static uint32_t g_last_request_time = 0;
static uint8_t g_cpsq8100_state = CPSQ8100_STATE_IDLE;
static uint8_t g_cpsq8100_response_result = CPSQ8100_RESPONSE_OK;

void CPSQ8100_RestSM() { g_cpsq8100_state = CPSQ8100_STATE_IDLE; }

void CPSQ8100_Init(void)
{
    Uart_Init(CPSQ8100_COM_ID, 115200, 8, 1, 0);
}

/**
 * @brief
 *
 * @param cmd
 * @param bodyLen
 * @param body
 * @return uint8_t CPSQ8100_OK 代表发送成功，CPSQ8100_BUSY代表上次请求还未结束
 */
uint8_t CPSQ8100_SendRequest(uint8_t cmd, uint16_t bodyLen, uint8_t *body)
{
    uint16_t i = 0;
    uint16_t pckLen = 7 + bodyLen;
    
    if (g_cpsq8100_state == CPSQ8100_STATE_WAIT_ACK)
    {
        return CPSQ8100_BUSY;
    }


    // Set Magic Number
    g_cpsq8100_tx_buffer[0] = 0x9E;
    g_cpsq8100_tx_buffer[1] = 0x5C;
    // Set Command
    g_cpsq8100_tx_buffer[2] = cmd;
    // Set Subtype
    g_cpsq8100_tx_buffer[3] = CPSQ8100_SUBTYPE_REQUEST;
    // Set Body Length as little endian
    g_cpsq8100_tx_buffer[4] = bodyLen & 0xFF;
    g_cpsq8100_tx_buffer[5] = (bodyLen >> 8) & 0xFF;
    // Set Body
    for (i = 0; i < bodyLen; i++)
    {
        g_cpsq8100_tx_buffer[6 + i] = body[i];
    }
    // Set Checksum
    g_cpsq8100_tx_buffer[6 + bodyLen] = CPSQ8100_CalculateChecksum(g_cpsq8100_tx_buffer, 6 + bodyLen);
    // Send by Uart
    Uart_Write(CPSQ8100_COM_ID, g_cpsq8100_tx_buffer, pckLen);
    g_cpsq8100_state = CPSQ8100_STATE_WAIT_ACK;
    g_last_request_time = HDL_CPU_Time_GetTick();
    return CPSQ8100_OK;
}

uint8_t CPSQ8100_WriteRegisterRequest(uint16_t regAddr, uint16_t dataSize, uint8_t *pData)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;
    uint8_t body[4];
	int i = 0;
    cmd = 0x06;
    bodyLen = 4 + dataSize; // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = dataSize & 0xFF;
    g_cpsq8100_body_buffer[3] = (dataSize >> 8) & 0xFF;
    for (i = 0; i < dataSize; i++)
    {
        g_cpsq8100_body_buffer[4 + i] = pData[i];
    }
    ret = CPSQ8100_SendRequest(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

uint8_t CPSQ8100_ReadRegisterRequest(uint16_t regAddr, uint16_t dataSize)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;

    cmd = 0x07;
    bodyLen = 4; // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = dataSize & 0xFF;
    g_cpsq8100_body_buffer[3] = (dataSize >> 8) & 0xFF;
    ret = CPSQ8100_SendRequest(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

uint8_t CPSQ8100_WriteRegisterRequest_Uint32(uint16_t regAddr, uint32_t data)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;

    cmd = 0x06;
    bodyLen = 4 + sizeof(data); // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = sizeof(data) & 0xFF;
    g_cpsq8100_body_buffer[3] = (sizeof(data) >> 8) & 0xFF;
    g_cpsq8100_body_buffer[4] = data & 0xFF;
    g_cpsq8100_body_buffer[5] = (data >> 8) & 0xFF;
    g_cpsq8100_body_buffer[6] = (data >> 16) & 0xFF;
    g_cpsq8100_body_buffer[7] = (data >> 24) & 0xFF;
    ret = CPSQ8100_SendRequest(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

uint8_t CPSQ8100_WriteRegisterRequest_Uint16(uint16_t regAddr, uint16_t data)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;

    cmd = 0x06;
    bodyLen = 4 + sizeof(data); // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = sizeof(data) & 0xFF;
    g_cpsq8100_body_buffer[3] = (sizeof(data) >> 8) & 0xFF;
    g_cpsq8100_body_buffer[4] = data & 0xFF;
    g_cpsq8100_body_buffer[5] = (data >> 8) & 0xFF;
    ret = CPSQ8100_SendRequest(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

uint8_t CPSQ8100_Send(uint8_t cmd, uint16_t bodyLen, uint8_t *body)
{
    uint16_t i = 0;
    uint16_t pckLen = 7 + bodyLen;
    // Set Magic Number
    g_cpsq8100_tx_buffer[0] = 0x9E;
    g_cpsq8100_tx_buffer[1] = 0x5C;
    // Set Command
    g_cpsq8100_tx_buffer[2] = cmd;
    // Set Subtype
    g_cpsq8100_tx_buffer[3] = CPSQ8100_SUBTYPE_REQUEST;
    // Set Body Length as little endian
    g_cpsq8100_tx_buffer[4] = bodyLen & 0xFF;
    g_cpsq8100_tx_buffer[5] = (bodyLen >> 8) & 0xFF;
    // Set Body
    for (i = 0; i < bodyLen; i++)
    {
        g_cpsq8100_tx_buffer[6 + i] = body[i];
    }
    // Set Checksum
    g_cpsq8100_tx_buffer[6 + bodyLen] = CPSQ8100_CalculateChecksum(g_cpsq8100_tx_buffer, 6 + bodyLen);
    // Send by Uart
    Uart_Write(CPSQ8100_COM_ID, g_cpsq8100_tx_buffer, pckLen);
    return CPSQ8100_OK;
}

uint8_t CPSQ8100_WriteRegister_Uint32(uint16_t regAddr, uint32_t data)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;

    cmd = 0x06;
    bodyLen = 4 + sizeof(data); // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = sizeof(data) & 0xFF;
    g_cpsq8100_body_buffer[3] = (sizeof(data) >> 8) & 0xFF;
    g_cpsq8100_body_buffer[4] = data & 0xFF;
    g_cpsq8100_body_buffer[5] = (data >> 8) & 0xFF;
    g_cpsq8100_body_buffer[6] = (data >> 16) & 0xFF;
    g_cpsq8100_body_buffer[7] = (data >> 24) & 0xFF;
    ret = CPSQ8100_Send(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

uint8_t CPSQ8100_WriteRegister_Uint16(uint16_t regAddr, uint16_t data)
{
    uint8_t ret = 0;
    uint8_t cmd = 0;
    uint16_t bodyLen = 0;

    cmd = 0x06;
    bodyLen = 4 + sizeof(data); // The "4" is the length of regAddr and dataSize
    g_cpsq8100_body_buffer[0] = regAddr & 0xFF;
    g_cpsq8100_body_buffer[1] = (regAddr >> 8) & 0xFF;
    g_cpsq8100_body_buffer[2] = sizeof(data) & 0xFF;
    g_cpsq8100_body_buffer[3] = (sizeof(data) >> 8) & 0xFF;
    g_cpsq8100_body_buffer[4] = data & 0xFF;
    g_cpsq8100_body_buffer[5] = (data >> 8) & 0xFF;
    ret = CPSQ8100_Send(cmd, bodyLen, g_cpsq8100_body_buffer);
    return ret;
}

/**
 * @brief 检查接收到的帧是否正确
 *
 * @param pFrame
 * @param frameLen
 * @return uint8_t 1代表正确，0代表错误
 */
uint8_t CPSQ8100_CheckFrame(uint8_t *pFrame, uint16_t frameLen)
{
    uint8_t ret = 0;
    uint8_t checksum = 0;

    if (pFrame[0] == 0x9E && pFrame[1] == 0x5C)
    {
        checksum = CPSQ8100_CalculateChecksum(pFrame, frameLen - 1);
        if (checksum == pFrame[frameLen - 1])
        {
            ret = 1;
        }
    }
    return ret;
}

/**
 * @brief CPSQ8100 Poll function,and it is an function contains static variables,so it should be called in a loop.
 *
 */
void CPSQ8100_Poll(void)
{
    static uint32_t readLen = 0;
    static uint32_t i = 0;
    static uint32_t rev_stage = 0; // 接收一帧数据的阶段，这里采用收到帧头后作为接收开始，没有收到任何数据单独超时
    static uint32_t last_available_bytes = 0;
    static uint32_t last_received_bytes_num_change_time = 0;
    static struct tagCPSQ8100_RxHeader rxHeader;
    if (g_cpsq8100_state == CPSQ8100_STATE_WAIT_ACK)
    {
        switch (rev_stage)
        {
        // 等待接收到第一个字节数据
        case 0:
            last_available_bytes = Uart_AvailableBytes(CPSQ8100_COM_ID);
            if (last_available_bytes > 0)
            {
                last_received_bytes_num_change_time = HDL_CPU_Time_GetTick();
                rev_stage = 1;
            }
            else if (HDL_CPU_Time_GetTick() - g_last_request_time > 200)
            {
                g_cpsq8100_response_result = CPSQ8100_RESPONSE_TIMEOUT;
                // TODO:请求超时
                CPSQ8100_RestSM();
            }
            break;
        // 判断是否大于帧间隔的最大时间，但是需要注意确保Uart_AvailableBytes更新的速度与接收到新自己的速度基本一致，如果延迟太高可能会造成判断失误
        case 1:
            if (Uart_AvailableBytes(CPSQ8100_COM_ID) > last_available_bytes)
            {
                last_available_bytes = Uart_AvailableBytes(CPSQ8100_COM_ID);
                last_received_bytes_num_change_time = HDL_CPU_Time_GetTick();
            }
            else
            {
                if (HDL_CPU_Time_GetTick() - last_received_bytes_num_change_time > 20)
                {
                    readLen = Uart_Read(CPSQ8100_COM_ID, g_cpsq8100_rx_buffer, sizeof(g_cpsq8100_rx_buffer));
                    if (CPSQ8100_CheckFrame(g_cpsq8100_rx_buffer, readLen) == 1)
                    {
                        // 处理正确的回复
                        g_cpsq8100_response_result = CPSQ8100_RESPONSE_OK;

                        rxHeader.cmd = g_cpsq8100_rx_buffer[2];
                        rxHeader.subtype = g_cpsq8100_rx_buffer[3];
                        rxHeader.pBody = &g_cpsq8100_rx_buffer[6];
                        rxHeader.bodyLen = g_cpsq8100_rx_buffer[4] + (g_cpsq8100_rx_buffer[5] << 8);
                        rxHeader.checksum = g_cpsq8100_rx_buffer[6 + rxHeader.bodyLen];

                        CPSQ8100_RecevieCallback(&rxHeader);
                    }
                    else
                    {
                        g_cpsq8100_response_result = CPSQ8100_RESPONSE_ERROR;
                    }
                    rev_stage = 0;
                    CPSQ8100_RestSM();
                }
            }
            break;
        }
    }
}

uint8_t CPSQ8100_IsIdle(void)
{
    uint8_t ret = 0;
    if (g_cpsq8100_state == CPSQ8100_STATE_IDLE)
    {
        ret = 1;
    }
    return ret;
}
