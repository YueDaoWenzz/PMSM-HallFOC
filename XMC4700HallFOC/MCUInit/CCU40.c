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
 *                (使能CC40,配置分频1024，时钟周期100，POSIF0。OUT0上升、下降沿触发事件0进行计数功能，周期中断程序翻转P0.6电平)
 * Caveats:       2013-8-30
 *               
 *****************************************************************************/
 //CCU4模块时钟64MHz
void CCU40_vInit(void)
{
	// Set Prescaler run bit
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_SPRB_Msk, CCU4_GIDLC_SPRB_Pos, 1);
	// Configure the Prescaler for f_CCU4/
	WR_REG(CCU40_CC40->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 0);
	WR_REG(CCU40_CC41->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 7); //128分频
	WR_REG(CCU40_CC42->PSC, CCU4_CC4_PSC_PSIV_Msk, CCU4_CC4_PSC_PSIV_Pos, 0); //
	
	WR_REG(CCU40_CC40->INS, CCU4_CC4_INS_EV0IS_Msk, CCU4_CC4_INS_EV0IS_Pos, 4);//IN0E（OUT0）
  WR_REG(CCU40_CC40->INS, CCU4_CC4_INS_EV0EM_Msk, CCU4_CC4_INS_EV0EM_Pos, 1);//上升沿有效	
	WR_REG(CCU40_CC40->CMC, CCU4_CC4_CMC_STRTS_Msk, CCU4_CC4_CMC_STRTS_Pos, 1);//CC40事件0启动定时器
	
	// Configure the Period register
	WR_REG(CCU40_CC40->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, TMR_VAL_CCU40);
	WR_REG(CCU40_CC41->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, 0x0000FFFF); //131ms 对应最低转速38rpm    //这个就很关键了，一次霍尔跳变的时间最多131ms 折算成2对极电机
	WR_REG(CCU40_CC42->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, 0x0000FFFF);
	
	// Configure the Compare register
	WR_REG(CCU40_CC40->CRS, CCU4_CC4_CRS_CRS_Msk, CCU4_CC4_CRS_CRS_Pos, CMP_VAL_CCU40);
	
	// Enable single shot mode
	WR_REG(CCU40_CC40->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 1);//单次模式
	WR_REG(CCU40_CC40->TC, CCU4_CC4_TC_STRM_Msk, CCU4_CC4_TC_STRM_Pos, 1);//清空定时器并启动

	WR_REG(CCU40_CC41->TC, CCU4_CC4_TC_TCM_Msk, CCU4_CC4_TC_TCM_Pos, 0);//计数边沿模式
	WR_REG(CCU40_CC41->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 1);//单次模式
	
	WR_REG(CCU40_CC42->TC, CCU4_CC4_TC_TCM_Msk, CCU4_CC4_TC_TCM_Pos, 0);//计数边沿模式
	WR_REG(CCU40_CC42->TC, CCU4_CC4_TC_TSSM_Msk, CCU4_CC4_TC_TSSM_Pos, 0);//禁止单次模式
	
	WR_REG(CCU40_CC42->INTE, CCU4_CC4_INTE_PME_Msk, CCU4_CC4_INTE_PME_Pos, 1);//周期中断使能
	
	WR_REG(CCU40_CC42->SRS, CCU4_CC4_SRS_POSR_Msk, CCU4_CC4_SRS_POSR_Pos, 0);//周期中断转发到SR0
	
//	NVIC_ClearPendingIRQ(CCU40_0_IRQn); 
//	NVIC_EnableIRQ(CCU40_0_IRQn);
		
	// Shadow transfer for slice 0 for CCU40 Kernel
	WR_REG(CCU40->GCSS, CCU4_GCSS_S0SE_Msk, CCU4_GCSS_S0SE_Pos, 1);//映射传送使能
	WR_REG(CCU40->GCSS, CCU4_GCSS_S1SE_Msk, CCU4_GCSS_S1SE_Pos, 1);
	WR_REG(CCU40->GCSS, CCU4_GCSS_S2SE_Msk, CCU4_GCSS_S2SE_Pos, 1);
		
		// Clear idle bit slice 0 for CCU40 Kernel
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS0I_Msk, CCU4_GIDLC_CS0I_Pos, 1);
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS1I_Msk, CCU4_GIDLC_CS1I_Pos, 1);
	WR_REG(CCU40->GIDLC, CCU4_GIDLC_CS2I_Msk, CCU4_GIDLC_CS2I_Pos, 1);

}

void CCU40_0_IRQHandler(void)   //记录delta_theta  //周期中断
{
	if(CCU40_CC42->INTS & 0x00000001)
	{
		 WR_REG(CCU40_CC42->SWR, CCU4_CC4_SWR_RPM_Msk, CCU4_CC4_SWR_RPM_Pos, 1);
		 Hall1.KCount++;
		 
		 if(Hall1.KCount>Hall1.Numencoder)
			 
			 Hall1.KCount=Hall1.Numencoder;
	}
	
}

