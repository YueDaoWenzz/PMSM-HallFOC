/*
 * DIV_Init.h
 *
 *  Created on: 2013-3-29
 *      Author: mengdeyu
 */

#ifndef MATH_INIT_H_
#define MATH_INIT_H_

#include <XMC1300.h>

#define VECTORING_MODE	0x62				// For CORDIC control register: Circular Vectoring Mode (default). MPS: Divide by 2 (default).
#define ROTATION_MODE	0x6A				// For CORDIC control register: Circular Rotation Mode. MPS: Divide by 2 (default).


void 		MATH_Init(void);
uint16_t  	MDU_Divider(uint32_t,uint32_t);


void Cordic_Cart2Polar(int16_t Alpha, int16_t Beta, uint32_t *M, uint32_t *Angle);

#endif /* MATH_INIT_H_ */
