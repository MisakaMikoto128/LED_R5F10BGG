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
#pragma di
#pragma nop
#pragma sfr

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "stdio.h"
#include <string.h>
#include "CPSQ8100_frame.h"
#include "r_cg_adc.h"

#include    "r_rl78_can_drv.h"                  /* Header file for CAN software driver */
#include    "tk_header.h"                       /* Header file for can sample program */

void My_uart1Send(uint8_t * const tx_buf, uint16_t tx_num);
void delay_ms(uint16_t s);
void R_MAIN_UserInit(void);
void    tk_v0t_drvinit(void);
void MYCAN_SEND(uint8_t ID,uint8_t data0,uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4,uint8_t data5);
void led_0(void);
void TemperatureCheck(void);
void uint16TOuint8(uint16_t *SocBuf,uint8_t *DscBuf,int8_t size);

extern uint16_t Timecount;
extern int testclock;
extern void ParameterShow(void);
int test = 8;
uint8_t TXerror[8] = {'c','a','n','e','r','r','o','r'};
uint8_t TXok[5] = {'c','a','n','o','k'};
uint8_t i =0,Num=0;
uint8_t uart0RXbuff[200] = {0}, ADValue_8[8]={0};
uint16_t ADC_BUFF[8]={0x55,0x33,0x44};
extern uint16_t ADValue[8];
Can_RtnType retval;
can_frame_t frame;

/****************************************************************************** 
* Function Name: main
* Description  : Main function
* Arguments    : none
* Return Value : none
******************************************************************************/



void main(void)
{


    DI();/* interrupt disable */
    //hdwinit();/* Initialize sample program */
    
    R_MAIN_UserInit();
    tk_v0s_start();
    R_UART0_Receive(uart0RXbuff, 200);
    tk_v0t_drvinit();                           /* Initialize CAN */
    
    //while(1){
	//   P3 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0;
   // };


    frame.IDE = 0;     	 /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;     	 /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;   	 /* Transmit History Data Store Enable */
    frame.IDL = 0x123;   /* ID Data (low)                      */
    frame.IDH = 0;       /* ID Data (high)                     */
    frame.DLC = 3;     	 /* DLC Data                           */
    frame.LBL = 0;     	 /* Label Data                         */
    frame.TS = 0;      	 /* Timestamp Data                     */
    frame.DB[0] = 0x12;  /* Data Byte                          */
    frame.DB[1] = 0x34;  /* Data Byte                          */
    frame.DB[2] = 0x56;  /* Data Byte                          */
    frame.DB[3] = 0;
    frame.DB[4] = 0;
    frame.DB[5] = 0;
    frame.DB[6] = 0;
    frame.DB[7] = 0;
    //retval = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
    //if (retval != CAN_RTN_OK) {
        //tk_v0s_error();
    //}
printf("repeat");
P8 = _20_Pn5_OUTPUT_1;
    while(1) {                               /* Main loop */
        /* Wait until receiving a CAN frame (Any STD-ID) */
        //retval = R_CAN_ReadRxFIFO(CAN_RXFIFO0, &frame);
		MYCAN_SEND(0x01,NXP_Overvoltage,1,2,6,6,8);
		//UART4_Send_Frame_Write(0x0600,  0x0800, 0x3C00, 0x0400,0X80000159 );//?????
		//R_ADC_Start();
	        delay_ms(1000);
		P8 = _10_Pn4_OUTPUT_1 | _00_Pn5_OUTPUT_0;
		delay_ms(100);
		P8 = _00_Pn4_OUTPUT_0 | _20_Pn5_OUTPUT_1;
		
		//R_ADC_Get_Result(ADC_BUFF);
		uint16TOuint8(ADValue,ADValue_8,3);
		//My_uart1Send(ADValue_8,6);
		My_uart1Send(ADValue,8);
		//printf("%d",frame.DB[0]);
		
		
		/*
		CPSQ8100_Handle_Process();
		FOD_RESOLVE();
		RemoveNXPFod();
		ParameterShow();
		ScreenKey_Handle();
		//TemperatureCheck();
		*/

    }
}

void uint16TOuint8(uint16_t *SocBuf,uint8_t *DscBuf,int8_t size)
{
	uint8_t Num=0;
	for(Num=0;Num<size;Num++)
	{
		DscBuf[Num*2+1] = ((SocBuf[Num]>>8)&0xff);
		DscBuf[Num*2]   = SocBuf[Num]&0xff;
	}
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
    CAN0EN = 1; //??CAN??

    /* CAN Initialize */
    retval = CAN_RTN_RAM_INIT;
    while (retval != CAN_RTN_OK) {
        retval = R_CAN_Init();
    }

    /* Set global operating mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT) {
        retval = R_CAN_GlobalStart();
    }

    /* CH0 -> Channel communication mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT) {
        retval = R_CAN_ChStart(CAN_CH0);
    }

    return ;
}
void systick_delay_ms(uint16_t ms)
{
	delay_ms(ms);
}

void delay_ms(uint16_t s)
{
  	uint16_t sec_1 = Timecount;
	uint16_t sec_2 = sec_1 + s;
	
	if(sec_2 > MAX_COUNT)
	{
		sec_2 = sec_2 - MAX_COUNT;
		while(1)
		{
			if(Timecount == 0) break;
			else NOP();
		}
		while(sec_2 > Timecount)
		{
				 NOP();
		}
	}
	else if(sec_2 == MAX_COUNT)
	{
		while(1)
		{
			if(Timecount == MAX_COUNT || Timecount == 0)//2??¨¹¦Ì¨¨¨®¨²0
			{
				break;
			}
			else NOP();
		}
	}
	else
	{
		while(sec_2 > Timecount || Timecount == 1)
		{
				 NOP();;
		}
	}
}



void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    R_PORT_Create();
    //uart0 init
    R_SAU0_Create();
    R_UART0_Create();
    //uatt1 init
    //R_SAU1_Create();
    //R_UART1_Create();
    //time0 init
    R_TAU0_Create();
    R_TAU1_Create();
    
    R_ADC_Create();
    
    //enable peripheral
    //R_UART1_Start();
    R_UART0_Start();
    R_TAU0_Channel0_Start();
    R_TAU1_Channel0_Start();
    R_CAN_ChStart_CH0();
    //R_ADC_Set_OperationOn();
    //R_ADC_Start();
    //R_CAN_GlobalStart();
	
	

	
	
    EI();
    /* End user code. Do not edit comment generated here */
}

void My_uart1Send(uint8_t * const tx_buf, uint16_t tx_num)
{
	R_UART0_Send(tx_buf,tx_num);	
}

void MYCAN_SEND(uint8_t ID,uint8_t data0,uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4,uint8_t data5)
{
    can_frame_t frame;
    
    frame.IDE = 0;     	 /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;     	 /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;   	 /* Transmit History Data Store Enable */
    frame.IDL = ID;   /* ID Data (low)                      */
    frame.IDH = 0;       /* ID Data (high)                     */
    frame.DLC = 5;     	 /* DLC Data                           */
    frame.LBL = 0;     	 /* Label Data                         */
    frame.TS = 0;      	 /* Timestamp Data                     */
    memset(frame.DB,0,8);
    frame.DB[0] = data0;  /* Data Byte                          */
    frame.DB[1] = data1;  /* Data Byte                          */
    frame.DB[2] = data2;  /* Data Byte                          */
    frame.DB[3] = data3;
    frame.DB[4] = data4;
    frame.DB[5] = data5;
    
    R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
}

void led_0(void)
{
	P8 = _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
}


