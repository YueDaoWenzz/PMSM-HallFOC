/****************************************************************************
*    Filename:     GetCurrent.c
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
#include "GetCurrent.h"
#include "UserDefine.h"
/*****************************************************************************
*     Global Variable Definitions
*****************************************************************************/
TPhase32 Curt32;
TPhase16 Curt16;

uint16_t	Count_AD=0;
uint16_t	Sec;

/*****************************************************************************
 * Function:      void  XXX_vInit(void)
 * Description:   Initialize the XXX Module
 *                (Please add more details)
 * Caveats:       2013-1-16
 *                test only (Please add more details)
 *                **			G1Ch7		POT		P2.5
Iu====P2.11====G1CH3  +  Iv====P2.10=====G0Ch3
Iw====P2.3=====G1CH5
 *****************************************************************************/


void Get_Current_Init(void)
{
    Get_Current_Offset();
}

void    Get_Current(void)
{
	if(System_State_Flags.CURRENT_SAMPLING_F)
	{
		switch(SHUNT_NUMBER)
		{
			case 1:
						Current_Reconstruct();
						break;
			case 2:
						Foc16.Iu_in			=	(int16_t)(Curt16.U_Oft    -   ((VADC_G1->RES[3] & 0x0fff)<<4));	//
						Foc16.Iv_in			=	(int16_t)(Curt16.V_Oft    -   ((VADC_G0->RES[3] & 0x0fff)<<4));	//
						Foc16.Iw_in			=	0	-	Foc16.Iu_in	-	Foc16.Iv_in;
						break;
			case 3:
						Foc16.Iu_in			=	(int16_t)(Curt16.U_Oft    -   ((VADC_G1->RES[3] & 0x0fff)<<4));	//
						Foc16.Iv_in			=	(int16_t)(Curt16.V_Oft    -   ((VADC_G0->RES[3] & 0x0fff)<<4));	//
						Foc16.Iw_in			=	(int16_t)(Curt16.W_Oft    -   ((VADC_G1->RES[5] & 0x0fff)<<4));	//


							if((SVPWM16.Sector==2) || (SVPWM16.Sector==3))
							{
								Foc16.Iu_in			=	0	-	Foc16.Iv_in	-	Foc16.Iw_in;
							}

							if((SVPWM16.Sector==1) || (SVPWM16.Sector==5))
							{
								Foc16.Iv_in			=	0	-	Foc16.Iu_in	-	Foc16.Iw_in;
							}


						break;
			default:

						break;
		}
		Foc16.Ialpha	=	Foc16.Iu_in;
		Foc16.Ibeta	=	((int32_t)(Foc16.Iu_in*9459) +	(int32_t)(Foc16.Iv_in*18919))>>14;		
		
	}
	else
	{
        Foc16.Ialpha	=	((int32_t)((int32_t)Foc16.Id_Bak	*	(int32_t)SinTab16.Cos)	-	(int32_t)((int32_t)Foc16.Iq_Bak	*	(int32_t)SinTab16.Sin))>>15;
        Foc16.Ibeta		=	((int32_t)((int32_t)Foc16.Id_Bak	*	(int32_t)SinTab16.Sin)	+	(int32_t)((int32_t)Foc16.Iq_Bak	*	(int32_t)SinTab16.Cos))>>15;
		
	}
	

	

}
//-----------------------------------------------------------------------------------
int16_t	Sector_Count,Delta_U,Delta_V,Delta_W,I_Step=512;	//I_Step大小影响电流变化率
int16_t	Adjust=300;
int16_t	Adjust0=-100;
__STATIC_INLINE void Current_Reconstruct()
{
		int16_t	temp0,temp1;

	
		temp0=(int16_t) (((uint16_t)((VADC_G1->RES[1] & 0x0fff)<<4)) - Curt16.DC_Oft-Adjust);
		temp1=(int16_t) (((uint16_t)((VADC_G1->RES[4] & 0x0fff)<<4)) - Curt16.DC_Oft-Adjust);

		Foc16.Iu_in_old	=	Foc16.Iu_in;
		Foc16.Iv_in_old	=	Foc16.Iv_in;
		Foc16.Iw_in_old	=	Foc16.Iw_in;



		switch(Sec)							//623154
		{

				case 1:
					Foc16.Iv_in = temp0 - Adjust0;
					Foc16.Iw_in = 0 - temp1;
					Foc16.Iu_in = temp1 - temp0;
				
				
					break;

				case 2:
					Foc16.Iu_in = temp0 - Adjust0;
					Foc16.Iv_in = 0 - temp1;
					Foc16.Iw_in = temp1 - temp0;
					break;

				case 3:
					Foc16.Iu_in = temp0 - Adjust0;
					Foc16.Iw_in = 0 - temp1;
					Foc16.Iv_in = temp1 - temp0;
					break;
				case 4:
					Foc16.Iw_in = temp0 - Adjust0;
					Foc16.Iu_in = 0 - temp1;
					Foc16.Iv_in = temp1 - temp0;
					break;
				case 5:
					Foc16.Iv_in = temp0 - Adjust0;
					Foc16.Iu_in = 0 - temp1;
					Foc16.Iw_in = temp1 - temp0;
					break;
				case 6:
					Foc16.Iw_in = temp0 - Adjust0;
					Foc16.Iv_in = 0 - temp1;
					Foc16.Iu_in= temp1 - temp0;
					break;

				default:
					Foc16.Iu_in =	0;
					Foc16.Iv_in =	0;
					Foc16.Iw_in =	0;	
					break;

		}
		
		Delta_U	=	Foc16.Iu_in - Foc16.Iu_in_old;
		Delta_V	=	Foc16.Iv_in - Foc16.Iv_in_old;
		Delta_W	=	Foc16.Iw_in - Foc16.Iw_in_old;		
		if(Delta_U>I_Step){Foc16.Iu_in = Foc16.Iu_in_old + I_Step;}
		if(Delta_U<(0-I_Step)){Foc16.Iu_in = Foc16.Iu_in_old - I_Step;}
		if(Delta_V>I_Step){Foc16.Iv_in = Foc16.Iv_in_old + I_Step;}
		if(Delta_V<(0-I_Step)){Foc16.Iv_in = Foc16.Iv_in_old - I_Step;}	
		if(Delta_W>I_Step){Foc16.Iw_in = Foc16.Iw_in_old + I_Step;}
		if(Delta_W<(0-I_Step)){Foc16.Iw_in = Foc16.Iw_in_old - I_Step;}		

		
		
		Sec = SVPWM16.Sector;

// 		if(System_State_Flags.MOD_L)
// 		{
// 			Foc16.Iw_in = 0;
// 			Foc16.Iv_in = 0;
// 			Foc16.Iu_in = 0;

// 		}

}





//-------------------------------------------------------------------------
uint32_t	t_led,delay=8000000;
void Get_Current_Offset(void)
{
	volatile int32_t		AD0,AD1,AD2,AD3;

	Count_AD		=	0;
	Curt16.U_Oft	=	0;
	Curt16.V_Oft	=	0;
	Curt16.W_Oft	=	0;
	Curt16.DC_Oft	=	0;
	AD0=AD1=AD2=AD3=0;
	


	while(delay>1)						//电路稳定
	{
		delay--;
	}

	while(System_State_Flags.AD_OFFSET_F==0)
	{
		if(SHUNT_NUMBER != 1)
		{
			while(System_State_Flags.AD_OK_F==0){}
			System_State_Flags.AD_OK_F=0;
			AD1	+=	(((VADC_G1->RES[3] & 0x0fff))>>0);
			AD2	+=	(((VADC_G0->RES[3] & 0x0fff))>>0);
			AD3	+=	(((VADC_G1->RES[5] & 0x0fff))>>0);


			Count_AD++;
			if(Count_AD>=128)
			{
					Count_AD=0;
					Curt16.U_Oft =	(uint16_t)(AD1>>3);
					Curt16.V_Oft =	(uint16_t)(AD2>>3);
					Curt16.W_Oft =	(uint16_t)(AD3>>3);
					AD1=AD2=AD3=0;
				
					if(SHUNT_NUMBER==2)
					{
						Curt16.W_Oft = 32767;
					}
				
					if((Curt16.U_Oft > (32767 - Delta_AD))&&(Curt16.U_Oft < ((uint16_t)(32767 + Delta_AD)))&&(Curt16.V_Oft > (32767 - Delta_AD))&&(Curt16.V_Oft < ((uint16_t)(32767 + Delta_AD)))&&(Curt16.W_Oft > (32767 - Delta_AD))&&(Curt16.W_Oft < ((uint16_t)(32767 + Delta_AD))))
					{
						System_State_Flags.AD_OFFSET_F=1;
						NVIC_DisableIRQ(VADC0_G1_0_IRQn);//关闭结束AD转换中断 System_State_Flags.AD_OK_F 不再置1
					}
					else
					{
						Curt16.U_Oft	=	0;
						Curt16.V_Oft	=	0;
						Curt16.W_Oft	=	0;
					}
					if(t_led<16){t_led++;}
					else
					{
						t_led=0;
						LED_TOGGLE;
					}

			}
		}
		else
		{
			while(System_State_Flags.AD_OK_F==0){}
			System_State_Flags.AD_OK_F=0;
			
			AD0		+=		(VADC_G1->RES[4] & 0x0fff)>>0;  //母线电流AD值
				
			Count_AD++;
			if(Count_AD>=128)
			{
					Count_AD=0;
					Curt16.DC_Oft	=	(uint16_t)(AD0>>3);
					if((ADC_AMP_ENABLE)||((Curt16.DC_Oft > (32767 - Delta_AD))&&(Curt16.DC_Oft < ((uint16_t)(32767 + Delta_AD)))))
					{
						System_State_Flags.AD_OFFSET_F=1;
						NVIC_DisableIRQ(VADC0_G1_0_IRQn);
					}
					else
					{
						Curt16.DC_Oft	=	0;
					}
					if(t_led<16){t_led++;}
					else
					{
						t_led=0;
						LED_TOGGLE;
					}

			}
				
		}

	}







	PWM_OFF;
	System_State_Flags.START_F=0;
	System_State_Flags.LOCK_F=0;
	System_State_Flags.OPENLOOP_F=0;
	System_State_Flags.CLOSE_LOOP_F=1;
	t_low=0;
	t_stall=0;
	t_inc=0;



}
