/*
 * Main.c
 *
 *  Created on: 
 *      First Author: LuJun
       Second Author：YangZezhong
 */

#include <XMC1300.h>		//SFR declarations of the selected device
#include "MCUInit.h"
#include "FOC.h"
#include "GetCurrent.h"
#include "GetADCInputs.h"
#include "PWMUpdate.h"
#include "UserDefine.h"

int16_t	POT_old,POT;


int main(void)
{
	Parameters_Init();       //参数初始化
	
			//关中断
  __disable_irq();
	
	FOC_Init();              //PID，Hall初始化
	MCUInit();               //MCU外设初始化函数
	
	Refresh_CCU80_PWM_Dutycycle(2000,2000,2000);
	
//	Get_Current_Offset();    //AD基准值初始化   //采样电流正负
	__enable_irq();
	
	//开多通道
	POSIF0->MCSM	=	0x0333;							//多通道状态
  POSIF0->MCMS	|=	0x01;							//shadow transfer
	//POSIF0->MCMS	|=	0x04;							//shadow transfer  Decode和多通道一起用时，不用句话来跟新多通道
  
  System_Flags_Init();//系统状态位初始化
	WR_REG(CCU40_CC42->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //启动	CC42

	while(1)
	{
#if !REF_MODE
     POT    =   Get_UART_Val();
#elif REF_MODE
     POT = Get_POT();
#endif
    if(!System_State_Flags.STOP_F)
    {
      LED_ON;
#if !REF_MODE
      Bridge_Init();//初始化一次
#endif
    }
    else 
      LED_OFF;
#if UART_Scope 
      if(UARTSend_Flag)
    {
      UARTSend_Flag=0;
      OutPut_Data(Oscilloscope_Data);
    }
#endif
  }
	return 0;
}

