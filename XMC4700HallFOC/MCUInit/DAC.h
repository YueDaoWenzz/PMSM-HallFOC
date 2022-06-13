/*
 * DAC.h
 *
 *  Created on: 2013-8-15
 *      Author: mengdeyu
 */

#ifndef DAC_H_
#define DAC_H_

//---------------------------------------
#include "XMC1300.h"


#include "GPIO.h"
//----------------------------------------
void DAOutputs(int16_t *Data,int16_t Num);
void U0C0_SSC_vSendData(uint16_t Data);
extern int16_t DAC_Test[8];


#define DAC_PreSetting 	U0C0_SSC_vSendData
#define DAC_SendData	U0C0_SSC_vSendData




#endif /* DAC_H_ */
