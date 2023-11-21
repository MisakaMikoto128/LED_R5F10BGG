/******************************************************************************* 
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  
* intended for use with Renesas products. No other uses are authorized. This  
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS  
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the  
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     
*******************************************************************************/

#ifndef     __TK_HEADER_H__
#define     __TK_HEADER_H__

#define         LED1            ( 1 )
#define         LED2            ( 2 )
#define         LED_FAIL        ( 0xFF )

#define         INVERT_LED1     ( 0x0010U ) /* invert P8_4 */
#define         INVERT_LED2     ( 0x0020U ) /* invert P8_5 */

#define         OUT_LED1_ON     ( 0x00100000UL ) /* P140=0 */
#define         OUT_LED1_OFF    ( 0x00100010UL ) /* P140=1 */
#define         OUT_LED2_ON     ( 0x00200000UL ) /* P130=0 */
#define         OUT_LED2_OFF    ( 0x00200020UL ) /* P130=1 */
#define			OUT_LED1		( P14.0 )
#define			OUT_LED2		( P13.0 )
#define			OUT_LED1_MODE	( PM14.0 )
#define			OUT_LED2_MODE	( PM13.0 )

#define         BIT_ON          ( 1 )
#define         BIT_OFF         ( 0 )

#define         LED_TURN_ON     ( 0 )
#define         LED_TURN_OFF    ( 1 )

typedef enum    UserBoolType {
    FALSE = 0 ,
    TRUE
} Bool_t ;

extern void     tk_v0s_start(void);
extern void     tk_v0s_error(void);
extern void     tk_v0s_ledout(unsigned char, Bool_t);
extern void     tk_v0s_ledinvert(unsigned char);

#endif      /* __TK_HEADER_H__ */
