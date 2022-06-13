/*
 * MCUInit.h
 *
 *  Created on: 2013-8-30
 *      Author: QianQin
 */

#ifndef MCUINIT_H_
#define MCUINIT_H_


#include "CCU40.h"
#include "POSIF0_Decoder.h"
#include "GPIO.h"
#include "MATH_Init.h"
#include "CCU8.h"
#include "ADC.h"
#include "Parameters.h"
#include "GetADCInputs.h"
#include "UserDefine.h"
#include "UART.h"

#define UART_Scope   1


void MCUInit(void);
void SysTick_Handler (void);
void UART_Init(void);

#endif /* MCUINIT_H_ */
