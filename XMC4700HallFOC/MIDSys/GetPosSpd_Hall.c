/****************************************************************************
*    Filename:     GetPosSpd_Hall.c
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


/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include <DAVE3.h>


#include "GetPosSpd_Hall.h"
#include "MCUInit.h"
#include "PLL.h"
#include "FluxObserver.h"


int16_t		Angle_Step;
uint32_t	Amplitude,Angle;
//---------------------------------------------------------------------
void Get_Angle_Init(void)
{
    Motor_Info.Angle_Offset = 0;
}
void Get_Speed_Init(void)
{

}
uint16_t Get_Angle_Hall(void)					//0---65535;
{

	return (0);
}




int16_t Get_Speed_Hall(void)
{



	return (Motor_Info.Speed_RPM);


	//return (Motor_Info_PU.Speed_PU);
}

void Angle_Caliberation()
{


}
