/*
 * POSIF.h
 *
 *  Created on: Nov 11, 2016
 *      Author: summerzhongzai
 */

#ifndef POSIF_H_
#define POSIF_H_
#include <XMC4700.h>
#include "POSIF.h"
void POSIF_Init(void);
void POSIF0_0_IRQHandler(void);
void POSIF_Hal_Start(void);

#endif /* POSIF_H_ */
