/*****************************************************************************
*    Filename:     UserDefine.h
*    IDE:          DAVE_3.1.2
*    MCU:          XMC45OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2013-1-16    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/
#ifndef USERDEFINE_H_
#define USERDEFINE_H_

/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include <UserDefine.h>
#include "GPIO.h"
#include "MCUInit.h"
#include "PWMupdate.h"
#include "Interrupts.h"


/*****************************************************************************
*     Macro Definitions
*****************************************************************************/
#define OFF1      					P0_4_reset()
#define ON1        					P0_4_set()
#define TOGGLE1        			P0_4_toggle()

#define LED_ON      				P0_7_reset()
#define LED_OFF       			P0_7_set()
#define LED_FLASH       		P0_7_toggle()



#define EN_GATE_OFF     				  P0_11_reset()			//栅极信号使能
#define EN_GATE_ON        				P0_11_set()


#define DC_CAL_ON            P0_10_set()
#define DC_CAL_OFF           P0_10_reset()
//table 5-6 p115
#define PWM_One_Match_INT    		CCU80_0_IRQHandler				//IRQ25
#define Trap_Protection_INT     CCU80_1_IRQHandler				//IRQ26

#define Single_Shunt_ADC_INT    	VADC0_G3_3_IRQHandler
#define End_Conversion_ADC_INT  	VADC0_G1_0_IRQHandler


#define Correct_Wrong_Hall_Event	POSIF0_0_IRQHandler
//#define Wrong_Hall_Event			POSIF0_1_IRQHandler
#define Hall_Event_Capture			CCU40_0_IRQHandler
#define Hall_Timer_Overflow			CCU40_1_IRQHandler

#define CORDIC_SHIFT				8			// CORDIC register shift times.
#define VECTORING_MODE				0x62		// For CORDIC control register: Circular Vectoring Mode (default). MPS: Divide by 2 (default).
#define ROTATION_MODE				0x6A		// For CORDIC control register: Circular Rotation Mode. MPS: Divide by 2 (default).

//函数声明
void State_Function(void);
void Get_Mortor_state(void);
void Mortor_Stop(void);
void delayTimer(uint32_t ms);
int16_t Get_UART_Val(void);
void Bridge_Init(void);
void Refresh_Duty(uint16_t PWM_U,uint16_t PWM_V,uint16_t PWM_W);
void System_Flags_Reset(void);
void System_Flags_Init(void);
#endif /* USERDEFINE_H_ */
