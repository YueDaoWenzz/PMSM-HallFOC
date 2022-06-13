/*
 * DIV_Init.c
 *
 *  Created on: 2013-3-29
 *      Author: mengdeyu
 */
#include <XMC1300.h>		//SFR declarations of the selected device
#include "MCUINIT.h"
#include "Parameters.h"

#define CORDIC_SHIFT				8			// CORDIC register shift times.
#define VECTORING_MODE				0x62		// For CORDIC control register: Circular Vectoring Mode (default). MPS: Divide by 2 (default).
#define ROTATION_MODE				0x6A		// For CORDIC control register: Circular Rotation Mode. MPS: Divide by 2 (default).

void MATH_Init()
{

    SCU_GENERAL->PASSWD = 0xC0U;
	  SCU_CLK->CGATCLR0	|=	0x100;					//MATH
	  SCU_GENERAL->PASSWD = 0xC3U;

	  MATH->DIVCON	|=	0x06;						//MDU


}
//**************************************************************//

uint16_t MDU_Divider(uint32_t Dividend,uint32_t Divisor)
{

	MATH->DVD=Dividend;
	MATH->DVS=Divisor;

	MATH->DIVCON	|=0x01;			//start

	while(MATH->DIVST & 0x01){}


	return((uint16_t)MATH->QUOT);
}


void Cordic_Cart2Polar(int16_t Alpha, int16_t Beta, uint32_t *M, uint32_t *Angle)
{
	MATH->CON = VECTORING_MODE;					// General control of CORDIC Control Register.
	MATH->CORDZ = 0;							// Z = zero
	MATH->CORDY = (Alpha << 8U);
	MATH->CORDX = (Beta  << 8U);				// Input CORDX data, and auto start of CORDIC calculation.

	/*###* CPU can compute for other jobs here when CORDIC is computing */


	while (MATH->STATC & 0x01) continue;				// Wait if CORDIC is running calculation, takes max ~1.63us.


	*M = MATH->CORRX;					// Read CORDIC results. X- Unsigned, Vref is 32-bit data.
	*Angle = MATH->CORRZ;				// Theta, 1Q31.

	*Angle = *Angle >> 15U;
}
