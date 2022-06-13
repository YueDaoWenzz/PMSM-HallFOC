/****************************************************************************
*    Filename:     CCU40.c
*    IDE:          DAVE_3.1.6
*    MCU:          XMC13OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    V1.0       2013-8-30    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/


/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include "MCUInit.h"
#include "FOC.h"
/*****************************************************************************
*     Macro Definitions
*****************************************************************************/

// values calculated for 546 ms timer period with 33.3% duty cycle
#define TMR_VAL_CCU40	60
#define CMP_VAL_CCU40	50
#define TMR_Commutaion_CC43	50000 //100ms


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/


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
 * Function:      void  CCU40_vInit(void)
 * Description:   Initialize the CCU40 Module
 *                (ʹ��CC40,���÷�Ƶ1024��ʱ������100��POSIF0��OUT0�������½��ش����¼�0���м������ܣ������жϳ���תP0.6��ƽ)
 * Caveats:       2013-8-30
 *               
 *****************************************************************************/
 //CCU4ģ��ʱ��64MHz
void CCU40_vInit(void)
{
	// Set Prescaler run bit
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_SPRB_Msk, CCU4_GIDLC_SPRB_Pos, 1);
	// Configure the Prescaler for f_CCU4/
	WR_REG(CCU40_CC40->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 0);
	WR_REG(CCU40_CC41->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 7); //128��Ƶ
	WR_REG(CCU40_CC42->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 0); //
	
	WR_REG(CCU40_CC40->INS, CCU4_CC4_INS_EV0IS_Msk, CCU4_CC4_INS_EV0IS_Pos, 4);//IN0E��OUT0��
  WR_REG(CCU40_CC40->INS, CCU4_CC4_INS_EV0EM_Msk, CCU4_CC4_INS_EV0EM_Pos, 1);//��������Ч	
	WR_REG(CCU40_CC40->CMC, CCU4_CC4_CMC_STRTS_Msk, CCU4_CC4_CMC_STRTS_Pos, 1);//CC40�¼�0������ʱ��
	
	// Configure the Period register
	WR_REG(CCU40_CC40->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, TMR_VAL_CCU40);
	WR_REG(CCU40_CC41->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, 0x0000FFFF); //131ms ��Ӧ���ת��38rpm    //����ͺܹؼ��ˣ�һ�λ��������ʱ�����131ms �����2�Լ����
	WR_REG(CCU40_CC42->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, 0x0000FFFF);
	
	// Configure the Compare register
	WR_REG(CCU40_CC40->CRS, CCU4_CC4_CRS_CRS_Msk, CCU4_CC4_CRS_CRS_Pos, CMP_VAL_CCU40);
	
	// Enable single shot mode
	WR_REG(CCU40_CC40->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 1);//����ģʽ
	WR_REG(CCU40_CC40->TC, CCU4_CC4_TC_STRM_Msk, CCU4_CC4_TC_STRM_Pos, 1);//��ն�ʱ��������

	WR_REG(CCU40_CC41->TC, CCU4_CC4_TC_TCM_Msk, CCU4_CC4_TC_TCM_Pos, 0);//��������ģʽ
	WR_REG(CCU40_CC41->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 1);//����ģʽ
	
	WR_REG(CCU40_CC42->TC, CCU4_CC4_TC_TCM_Msk, CCU4_CC4_TC_TCM_Pos, 0);//��������ģʽ
	WR_REG(CCU40_CC42->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 0);//��ֹ����ģʽ
	
	WR_REG(CCU40_CC42->INTE, CCU4_CC4_INTE_PME_Msk, CCU4_CC4_INTE_PME_Pos, 1);//�����ж�ʹ��
	
	WR_REG(CCU40_CC42->SRS, CCU4_CC4_SRS_POSR_Msk, CCU4_CC4_SRS_POSR_Pos, 0);//�����ж�ת����SR0
	
//	NVIC_ClearPendingIRQ(CCU40_0_IRQn); 
//	NVIC_EnableIRQ(CCU40_0_IRQn);
		
	// Shadow transfer for slice 0 for CCU40 Kernel
	WR_REG(CCU40->GCSS, CCU4_GCSS_S0SE_Msk, CCU4_GCSS_S0SE_Pos, 1);//ӳ�䴫��ʹ��
	WR_REG(CCU40->GCSS, CCU4_GCSS_S1SE_Msk, CCU4_GCSS_S1SE_Pos, 1);
	WR_REG(CCU40->GCSS, CCU4_GCSS_S2SE_Msk, CCU4_GCSS_S2SE_Pos, 1);
		
		// Clear idle bit slice 0 for CCU40 Kernel
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS0I_Msk, CCU4_GIDLC_CS0I_Pos, 1);
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS1I_Msk, CCU4_GIDLC_CS1I_Pos, 1);
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS2I_Msk, CCU4_GIDLC_CS2I_Pos, 1);

}

void CCU40_0_IRQHandler(void)   //��¼delta_theta  //�����ж�
{
	if(CCU40_CC42->INTS & 0x00000001)
	{
		 WR_REG(CCU40_CC42->SWR, CCU4_CC4_SWR_RPM_Msk, CCU4_CC4_SWR_RPM_Pos, 1);
		 Hall1.KCount++;
		 
		 if(Hall1.KCount>Hall1.Numencoder)
			 
			 Hall1.KCount=Hall1.Numencoder;
	}
	
}

