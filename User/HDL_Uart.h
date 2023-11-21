/**
 * @file HDL_Uart.h
 * @author Liu Yuanlin (liuyuanlins@outlook.comId)
 * @brief
 * @version 0.1
 * @date 2023-09-13
 * @last modified 2023-09-13
 *
 * @copyright Copyright (c) 2023 Liu Yuanlin Personal.
 *
 */
#ifndef HDL_UART_H
#define HDL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __TYPEDEF__
    typedef signed char int8_t;
    typedef unsigned char uint8_t;
    typedef signed short int16_t;
    typedef unsigned short uint16_t;
    typedef signed long int32_t;
    typedef unsigned long uint32_t;
    typedef unsigned short uint16_t;
#define __TYPEDEF__
#endif

    // The COM ID
#define COM1 0
#define COM2 1
#define COM_NUM 2

    typedef int COMID_t;
    void Uart_Init(COMID_t comId, uint32_t baud, uint32_t wordLen, uint32_t stopBit, uint32_t parity);
    uint32_t Uart_Write(COMID_t comId, const uint8_t *writeBuf, uint32_t uLen);
    uint32_t Uart_Read(COMID_t comId, uint8_t *pBuf, uint32_t uiLen);
    uint32_t Uart_AvailableBytes(COMID_t comId);
    uint32_t Uart_EmptyReadBuffer(COMID_t comId);

    void UartQueueInit(void);
    void UartQueuPush(uint8_t data);
    uint8_t UartQueuePop(void);
    uint16_t UartQueueLength(void);

#ifdef __cplusplus
}
#endif
#endif //! HDL_UART_H
