#ifndef CCAN_H
#define CCAN_H
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

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/** @defgroup FDCAN_id_type FDCAN ID Type
  * @{
  */
#define FDCAN_STANDARD_ID ((uint32_t)0x00000000U) /*!< Standard ID element */
#define FDCAN_EXTENDED_ID ((uint32_t)0x00000001U) /*!< Extended ID element */
/**
  * @}
  */

/** @defgroup FDCAN_frame_type FDCAN Frame Type
  * @{
  */
#define FDCAN_DATA_FRAME   ((uint32_t)0x00000000U)  /*!< Data frame   */
#define FDCAN_REMOTE_FRAME ((uint32_t)0x00000001U)  /*!< Remote frame */
/**
  * @}
  */

/** @defgroup FDCAN_data_length_code FDCAN Data Length Code
  * @{
  */
#define FDCAN_DLC_BYTES_0  ((uint32_t)0x0000U) /*!< 0 bytes data field  */
#define FDCAN_DLC_BYTES_1  ((uint32_t)0x0001U) /*!< 1 bytes data field  */
#define FDCAN_DLC_BYTES_2  ((uint32_t)0x0002U) /*!< 2 bytes data field  */
#define FDCAN_DLC_BYTES_3  ((uint32_t)0x0003U) /*!< 3 bytes data field  */
#define FDCAN_DLC_BYTES_4  ((uint32_t)0x0004U) /*!< 4 bytes data field  */
#define FDCAN_DLC_BYTES_5  ((uint32_t)0x0005U) /*!< 5 bytes data field  */
#define FDCAN_DLC_BYTES_6  ((uint32_t)0x0006U) /*!< 6 bytes data field  */
#define FDCAN_DLC_BYTES_7  ((uint32_t)0x0007U) /*!< 7 bytes data field  */
#define FDCAN_DLC_BYTES_8  ((uint32_t)0x0008U) /*!< 8 bytes data field  */


/**
  * @brief  FDCAN Tx header structure definition
  */
typedef struct
{
    uint32_t Identifier;          /*!< Specifies the identifier.
                                     This parameter must be a number between:
                                      - 0 and 0x7FF, if IdType is FDCAN_STANDARD_ID
                                      - 0 and 0x1FFFFFFF, if IdType is FDCAN_EXTENDED_ID               */

    uint32_t IdType;              /*!< Specifies the identifier type for the message that will be
                                     transmitted.
                                     This parameter can be a value of @ref FDCAN_id_type               */

    uint32_t TxFrameType;         /*!< Specifies the frame type of the message that will be transmitted.
                                     This parameter can be a value of @ref FDCAN_frame_type            */

    uint32_t DataLength;          /*!< Specifies the length of the frame that will be transmitted.
                                      This parameter can be a value of @ref FDCAN_data_length_code     */

} FDCAN_TxHeaderTypeDef;

/**
  * @brief  FDCAN Rx header structure definition
  */
typedef struct
{
    uint32_t Identifier;            /*!< Specifies the identifier.
                                       This parameter must be a number between:
                                        - 0 and 0x7FF, if IdType is FDCAN_STANDARD_ID
                                        - 0 and 0x1FFFFFFF, if IdType is FDCAN_EXTENDED_ID               */

    uint32_t IdType;                /*!< Specifies the identifier type of the received message.
                                       This parameter can be a value of @ref FDCAN_id_type               */

    uint32_t RxFrameType;           /*!< Specifies the the received message frame type.
                                       This parameter can be a value of @ref FDCAN_frame_type            */

    uint32_t DataLength;            /*!< Specifies the received frame length.
                                        This parameter can be a value of @ref FDCAN_data_length_code     */

    uint32_t RxTimestamp;           /*!< Specifies the timestamp counter value captured on start of frame
                                       reception.
                                       This parameter must be a number between 0 and 0xFFFF              */
    uint8_t Data[8];
} FDCAN_RxHeaderTypeDef;

void CAN_Init();
HAL_StatusTypeDef CAN_Write(FDCAN_TxHeaderTypeDef *pTxHeader,
                            uint8_t *pTxData);
    HAL_StatusTypeDef CAN_Read(FDCAN_RxHeaderTypeDef *pRxHeader);
#ifdef __cplusplus
}
#endif
#endif // CCAN_H
