/*
 * POSIF.c
 *
 *  Created on: Nov 11, 2016
 *      Author: summerzhongzai
 */

#include <XMC4700.h>
#include "MCUInit.h"
#include "POSIF.h"
void POSIF_Init(void)
{
	SCU_RESET->PRSET0|=(((0x01)<<9)|((0x01)<<10));			//申请对POSIF模块0\1  进行复位
	SCU_RESET->PRCLR0|=(((0x01)<<9)|((0x01)<<10));			//完成复位并退出复位状态  
	WR_REG(POSIF0->PCONF, POSIF_PCONF_FSEL_Msk, POSIF_PCONF_FSEL_Pos, 0);//霍尔模式
	WR_REG(POSIF0->PCONF, POSIF_PCONF_HIDG_Msk, POSIF_PCONF_HIDG_Pos, 1);//
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MCUE_Msk, POSIF_PCONF_MCUE_Pos, 1);     //软件跟新多通道序列 
  
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL0_Msk, POSIF_PCONF_INSEL0_Pos, 1); // Select IN0A at P0.13  HA
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL1_Msk, POSIF_PCONF_INSEL1_Pos, 0); // Select IN1A at P1.1   HB
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL2_Msk, POSIF_PCONF_INSEL2_Pos, 0); // Select IN2A at P1.0   HC
	
	WR_REG(POSIF0->PCONF, POSIF_PCONF_DSEL_Msk, POSIF_PCONF_DSEL_Pos, 0);//延时引脚POSIFx.HSDA
	WR_REG(POSIF0->PCONF, POSIF_PCONF_SPES_Msk, POSIF_PCONF_SPES_Pos, 0);//Edge selector for the sampling trigger// Rising edge
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSETS_Msk, POSIF_PCONF_MSETS_Pos, 0);//Pattern update signal select POSIFx.MSETA
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSES_Msk, POSIF_PCONF_MSES_Pos, 0);//Multi-Channel pattern update trigger edge //rising edge
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSYNS_Msk, POSIF_PCONF_MSYNS_Pos, 0);//PWM synchronization signal selector//POSIFx.MSYNCA
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIS_Msk, POSIF_PCONF_EWIS_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIE_Msk, POSIF_PCONF_EWIE_Pos, 0);//禁止错误霍尔事件
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIL_Msk, POSIF_PCONF_EWIL_Pos, 0);//外部错误霍尔事件高电平
	
	WR_REG(POSIF0->PFLGE, POSIF_PFLGE_EHIE_Msk, POSIF_PFLGE_EHIE_Pos, 1);	//霍尔输入更新使能
  WR_REG(POSIF0->PFLGE, POSIF_PFLGE_HIESEL_Msk, POSIF_PFLGE_HIESEL_Pos, 0); //霍尔输入跟新中断转发到SR0
	
	
	WR_REG(POSIF0->PCONF, POSIF_PCONF_LPC_Msk, POSIF_PCONF_LPC_Pos, 111);//低通滤波器配置(64个时钟周期)2us
  	// Set run bit	
 WR_REG(POSIF0->PRUNS, POSIF_PRUNS_SRB_Msk, POSIF_PRUNS_SRB_Pos, 1);	
 	//POSIF 换向中断
 NVIC_ClearPendingIRQ(POSIF0_0_IRQn);		                     //This function clears interrupt node pending status
 NVIC_EnableIRQ(POSIF0_0_IRQn);			                         //This function enables interrupt node for interrupt request generation
}
void POSIF_Hal_Start(void)
{
}
void POSIF0_0_IRQHandler(void)
{

}