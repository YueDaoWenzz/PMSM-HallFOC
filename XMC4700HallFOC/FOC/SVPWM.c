/****************************************************************************
*    Filename:     SVPWM.c
*    IDE:          DAVE_3.1.2
*    MCU:          XMC45OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2013-1-9    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/


/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include "SVPWM.h"
#include "MCUInit.h"

/*****************************************************************************
*     Macro Definitions
*****************************************************************************/


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/
TSVPWM16_7  SVPWM16;

/*****************************************************************************
*     Static(Local) Variable Definitions
*****************************************************************************/


/*****************************************************************************
*     Local Function Declarations
*****************************************************************************/


/*****************************************************************************
*     Global Variable Definitions
*****************************************************************************/


/*****************************************************************************
 *    Global Function Definitions
 *****************************************************************************/


/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/


/*****************************************************************************
 * Function:      void  XXX_vInit(void)
 * Description:   Initialize the XXX Module
 *                (Please add more details)
 * Caveats:       2013-1-9
 *                test only (Please add more details)
 *****************************************************************************/

/*****************************************************************************
 * Function:      void  SVPWM32_7(void)
 * Description:   Initialize the XXX Module
 *                (Please add more details)
 * Caveats:       2013-1-9
 *                test only (Please add more details)
 *****************************************************************************/

void SVPWM16_Init(void)
{
    SVPWM16.PWM_Period = PWM.PeriodValue_CentralAlign + 1;    //2000
}

void SVPWM16_7(int16_t V_alpha, int16_t V_beta,  int16_t PWM_Period)
{


	SVPWM16.Vref1	=	V_beta;
	SVPWM16.Vref2	=	(short)(((long)V_alpha*(long)28377)>>15)				-(short)(((long)V_beta*(long)16384)>>15);
	SVPWM16.Vref3	=	0	-	(short)(((long)V_alpha*(long)28377)>>15)		-(short)(((long)V_beta*(long)16384)>>15);

	SVPWM16.X =	(short)(((long)SVPWM16.Vref1*(long)PWM_Period)>>14);
	SVPWM16.Y =	0-(short)(((long)SVPWM16.Vref3*(long)PWM_Period)>>14);
	SVPWM16.Z =	0-(short)(((long)SVPWM16.Vref2*(long)PWM_Period)>>14);				//16383max   to 11bit 周期值是2000

	if(SVPWM16.Vref1>0){SVPWM16.A=1;}
	else		{SVPWM16.A=0;}
	if(SVPWM16.Vref2>0){SVPWM16.B=2;}
	else		{SVPWM16.B=0;}
	if(SVPWM16.Vref3>0){SVPWM16.C=4;}
	else		{SVPWM16.C=0;}
	SVPWM16.Sector_Old	=	SVPWM16.Sector;
	SVPWM16.Sector=SVPWM16.A+SVPWM16.B+SVPWM16.C;


	switch(SVPWM16.Sector)          //根据扇区计算时间
	{

			case 1:
				SVPWM16.t1=SVPWM16.Z;
				SVPWM16.t2=SVPWM16.Y;
				//SVPWM16.Sector_NUM=2;
						break;
			case 2:
				SVPWM16.t1=SVPWM16.Y;
				SVPWM16.t2=0-SVPWM16.X;
				//SVPWM16.Sector_NUM=6;
						break;
			case 3:
				SVPWM16.t1=0-SVPWM16.Z;
				SVPWM16.t2=SVPWM16.X;
				//SVPWM16.Sector_NUM=1;
						break;
			case 4:
				SVPWM16.t1=0-SVPWM16.X;
				SVPWM16.t2=SVPWM16.Z;
				//SVPWM16.Sector_NUM=4;
						break;
			case 5:
				SVPWM16.t1=SVPWM16.X;
				SVPWM16.t2=0-SVPWM16.Y;
				//SVPWM16.Sector_NUM=3;
						break;
			case 6:
				SVPWM16.t1=0-SVPWM16.Y;
				SVPWM16.t2=0-SVPWM16.Z;
				//SVPWM16.Sector_NUM=5;
						break;

			default:
				SVPWM16.t1=0;
				SVPWM16.t2=0;
						break;
	}
	SVPWM16.t_Sum		=			SVPWM16.t1 + SVPWM16.t2;


	if(SVPWM16.t_Sum>PWM_Period)    //过调制处理  
	{

// 		SVPWM16.t1sat=MDU_Divider(((long)PWM_Period*(long)SVPWM16.t1), (SVPWM16.t_Sum));

// 		SVPWM16.t2sat	=	PWM_Period - SVPWM16.t1sat;
// 		SVPWM16.t1=SVPWM16.t1sat;
// 		SVPWM16.t2=SVPWM16.t2sat;
		
		MATH->DVD=(long)PWM_Period*(long)SVPWM16.t1;
		MATH->DVS=SVPWM16.t_Sum;
		MATH->DIVCON	|=0x01;			//start
		while(MATH->DIVST & 0x01){}

		SVPWM16.t1=(int16_t)MATH->QUOT;
		SVPWM16.t2=PWM_Period - SVPWM16.t1;
		
	}

	SVPWM16.taon=(PWM_Period-SVPWM16.t1-SVPWM16.t2)>>1;
	SVPWM16.tbon=SVPWM16.taon+SVPWM16.t1;
	SVPWM16.tcon=SVPWM16.tbon+SVPWM16.t2;							//low side ON_TIME :tcon>tbon>taon		
		
	
	switch(SHUNT_NUMBER)
	{
		case 1:
			System_State_Flags.CURRENT_SAMPLING_F=1;
					break;
		case 2:
			System_State_Flags.CURRENT_SAMPLING_F=1;
					break;		
		case 3:
			if(SVPWM16.tbon<300)			
			{
				System_State_Flags.CURRENT_SAMPLING_F=0;

			}
			else
			{
				System_State_Flags.CURRENT_SAMPLING_F=1;

			}
					break;
		default:
			System_State_Flags.CURRENT_SAMPLING_F=1;
					break;		
	}

	switch(SVPWM16.Sector)  //根据扇区计算开关管占空比
	{

			case 1:
				SVPWM16.PDC_U=SVPWM16.tbon;
				SVPWM16.PDC_V=SVPWM16.taon;
				SVPWM16.PDC_W=SVPWM16.tcon;

						break;
			case 2:
				SVPWM16.PDC_U=SVPWM16.taon;
				SVPWM16.PDC_V=SVPWM16.tcon;
				SVPWM16.PDC_W=SVPWM16.tbon;

						break;
			case 3:
				SVPWM16.PDC_U=SVPWM16.taon;
				SVPWM16.PDC_V=SVPWM16.tbon;
				SVPWM16.PDC_W=SVPWM16.tcon;

						break;
			case 4:
				SVPWM16.PDC_U=SVPWM16.tcon;
				SVPWM16.PDC_V=SVPWM16.tbon;
				SVPWM16.PDC_W=SVPWM16.taon;

						break;
			case 5:
				SVPWM16.PDC_U=SVPWM16.tcon;
				SVPWM16.PDC_V=SVPWM16.taon;
				SVPWM16.PDC_W=SVPWM16.tbon;

						break;
			case 6:
				SVPWM16.PDC_U=SVPWM16.tbon;
				SVPWM16.PDC_V=SVPWM16.tcon;
				SVPWM16.PDC_W=SVPWM16.taon;

						break;

			default:
				SVPWM16.PDC_U=PWM_Period>>1;
				SVPWM16.PDC_V=PWM_Period>>1;
				SVPWM16.PDC_W=PWM_Period>>1;
						break;
	}


}









