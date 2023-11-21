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

#pragma sfr

/* Header file for sample program */
#include "tk_header.h"

/****************************************************************************** 
* Function Name: tk_v0s_start
* Description  : Device initialization
* Arguments    : none
* Return Value : none
******************************************************************************/
void tk_v0s_start(void)
{
    PIOR4 = 0x00U; //can ? P11 P10

    /* Set fMX */
    CMC = 0x00U; //???? 
    MSTOP = 1U; //X1 STOP ???
    /* Set fMAIN */
    MCM0 = 0U;  //FMAIN ??Fih   ckc
    /* Set fSUB */
    XTSTOP = 1U; //XT1 oscillator stopped
    OSMC = 0x10U; //Low-speed on-chip oscillator operating
    /* Set fCLK */
    CSS = 0U; //Main system/PLL select clock (fMP)
    /* Set fIH */
    HIOSTOP = 0U; //High-speed on-chip oscillator operating

    CRC0CTL = 0x00U; //????CRC
    IAWCTL = 0x00U;

    /* Set CRXD0 pin (P11) */
    PM1.1 = BIT_ON;
    /* Set CTXD0 pin (P10) */
    P1.0  = BIT_ON;
    PM1.0 = BIT_OFF;

    /* LED Port setting */
    //OUT_LED1      = LED_TURN_OFF;  /* LED1 Output latch */
    //OUT_LED2      = LED_TURN_OFF;  /* LED2 Output latch */
    //OUT_LED1_MODE = BIT_OFF;       /* LED1 Set output mode */
    /* P13 only output mode */     /* LED2 Set output mode */

    return;
}


/****************************************************************************** 
* Function Name: tk_v0s_ledout
* Description  : LED output function
* Arguments    : u1t_which -
*                    Select LED
*                b1t_dat -
*                    Output LED (TRUE: turn on, FALSE: turn off)
* Return Value : none
******************************************************************************/
void tk_v0s_ledout(unsigned char u1t_which, Bool_t blt_dat)
{
    unsigned long psr_value;
    
    switch(u1t_which)
    {
        case LED1 :
            if(blt_dat == FALSE) {
                OUT_LED1 = LED_TURN_OFF;
            } else {
                OUT_LED1 = LED_TURN_ON;
            }
            break;
        case LED2 :
            if(blt_dat == FALSE) {
                OUT_LED2 = LED_TURN_OFF;
            } else {
                OUT_LED2 = LED_TURN_ON;
            }
            break;
        default :
            if(blt_dat == FALSE) {
                OUT_LED1 = LED_TURN_OFF;
                OUT_LED2 = LED_TURN_OFF;
            } else {
                OUT_LED1 = LED_TURN_ON;
                OUT_LED2 = LED_TURN_ON;
            }
            break;
    }

    return;
}


/****************************************************************************** 
* Function Name: tk_v0s_error
* Description  : Error function
* Arguments    : none
* Return Value : none
******************************************************************************/
void tk_v0s_error(void)
{
    /* Error output = All LED lighting. */
    tk_v0s_ledout(LED_FAIL, TRUE);
    while(1){
        ;
    }
}


/****************************************************************************** 
* Function Name: tk_v0s_ledinvert
* Description  : LED invert function
* Arguments    : u1t_which -
*                    Select LED
* Return Value : none
******************************************************************************/
void tk_v0s_ledinvert(unsigned char u1t_which)
{
    switch(u1t_which)
    {
        case LED1 :
            OUT_LED1 ^= BIT_ON;
            break;
        case LED2 :
            OUT_LED2 ^= BIT_ON;
            break;
        default :
            /* no operation */
            break;
    }

    return;
}
