/*
 * PWMupdate.h
 *
 *  Created on: 2013-10-24
 *      Author: mengdeyu
 */

#ifndef PWMUPDATE_H_
#define PWMUPDATE_H_

#include <XMC1300.h>

#include "Parameters.h"

#include "MCUINIT.h"

#include "FOC.h"


void Refresh_CCU80_PWM_Dutycycle(int16_t Tu, int16_t Tv, int16_t Tw);

#endif /* PWMUPDATE_H_ */
