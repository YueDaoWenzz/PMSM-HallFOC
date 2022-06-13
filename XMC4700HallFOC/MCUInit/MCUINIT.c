/*
 * MCUInit.c
 *
 *  Created on: 
 *      First Author: Lujun
        Second Author:YangZezhong
 */
#include <XMC1300.h>
#include "MCUInit.h"

/*********************************************************************************************************
** 函数名称: void MCUInit(void)
** 功能描述: 配置XMC1300中POSIF0、CCU40相关寄存器
**
** 说　 明:  对SysTick、POSIF0、CCU40进行初始化
**    注：SysTick为系统时钟，对其进行时钟值更新、时钟配置后，作为普通定时器使用，周期匹配触发相应中断。
**********************************************************************************************************/
#define OPEN 1
#define CLOSE 0
/**用0与1来选择模式，0表示关，1表示开***/
#if CLOSE
#define Need_IPM_Init   2
#elif OPEN
#define TESTON_SMALLMORTOR   1
#endif
void MCUInit(void)
{
	  //**********时钟和全局设置**************************************************************************
		SCU_GENERAL->PASSWD = 0x000000C0UL;
		//SCU_CLK->CGATCLR0 = 0x00000187UL;		// stop gating ADC 	CCU4 CCU8 POSIF MATH
//	  SCU_CLK->CGATCLR0|= 0x00000080UL;     // stop gating POSIF
//	  SCU_CLK->CGATCLR0|= 0x00000004UL;     // stop gating CCU4
//	  SCU_CLK->CGATCLR0|=	0x00000002UL;     // stop gating CCU8
//	  SCU_CLK->CGATCLR0|= 0x00000001UL;		  // stop gating ADC
//    SCU_CLK->CGATCLR0|= 0x00000008UL;     // stop gating USIC
    SCU_CLK->CGATCLR0 = 0x0000008FUL;		  // stop gating CCU4 and POSIF0 CCU8 VADC  UART
		SCU_CLK->CLKCR = 0x3FF10100UL;			  // 32 MHz MCLK, 64 MHz PCLK
  	while((SCU_CLK->CLKCR)&0x40000000UL);	// wait for VDDC to stabilize
		SCU_GENERAL->PASSWD = 0x000000C3UL;
  
  //主时钟中断
   SystemCoreClockUpdate();
	 SysTick_Config(SystemCoreClock/1000);//1ms中断
  
	//各外设模块初始化
      MATH_Init();
      POSIF0_vInit();                          //POSIF0初始化
      IO_vInit();                              //端口初始化
      CCU8_vInit();
      ADC_vInit();
      CCU40_vInit();                           //CCU40初始化
      UART_Init();                            //UART初始化
#ifdef TESTON_SMALLMORTOR  
      EN_GATE_ON;  //栅极信号使能
      DC_CAL_ON;  //电流校准使能      
#endif
      delayTimer(20);      //方便调试

     //开中断设置
   NVIC_ClearPendingIRQ(CCU80_0_IRQn);//开CCU8中断
	 NVIC_EnableIRQ(CCU80_0_IRQn);
   
   NVIC_ClearPendingIRQ(CCU40_0_IRQn); //开CCU4中断
	 NVIC_EnableIRQ(CCU40_0_IRQn);
   
   NVIC_ClearPendingIRQ(VADC0_G1_0_IRQn);//开ADC中断
   NVIC_EnableIRQ(VADC0_G1_0_IRQn);
   
	 NVIC_ClearPendingIRQ(USIC0_0_IRQn);  //开UART中断												//清除节点的等待状态
	 NVIC_EnableIRQ(USIC0_0_IRQn);		
	  //中断优先级
	  NVIC_SetPriority (POSIF0_0_IRQn, 0);      									 // POSIF  优先级0级
	  NVIC_SetPriority (CCU40_0_IRQn, 2);
	  NVIC_SetPriority (CCU80_0_IRQn, 1);
//		NVIC_SetPriority (CCU80_1_IRQn, 0X80);
    NVIC_SetPriority(SysTick_IRQn,3);
    NVIC_SetPriority(USIC0_0_IRQn, 3);
    NVIC_SetPriority(VADC0_G1_0_IRQn, 3);				
	  
}

void SysTick_Handler (void)
{
  if(!System_State_Flags.HALL_F)   //LED计时
  {
    LED_cnt ++ ;
  }
  else
  {
    LED_cnt = 0;
  }
#ifdef Need_IPM_Init 
  if(System_State_Flags.INIT_BRIDGE_F)
  {
    Bridge_cnt ++;
    if(Bridge_cnt>1 && Bridge_cnt <= 50)
    {
      P0_1_set();//U下桥开
      P0_3_reset();//V下桥关
      P0_9_reset();//W下桥关
    }
    if(Bridge_cnt>50 && Bridge_cnt <= 100)
    {
      P0_1_reset();//U下桥关
      P0_3_set();//V下桥开
      P0_9_reset();//W下桥关
    }
    if(Bridge_cnt>100 && Bridge_cnt <= 150)
    {
      P0_1_reset();//U下桥关
      P0_3_reset();//V下桥关
      P0_9_set();//W下桥开
    }
    if(Bridge_cnt > 150)
    {
      P0_1_reset();//U下桥关
      P0_3_reset();//V下桥关
      P0_9_reset(); 
      System_State_Flags.INIT_BRIDGE_F = 0;
      Bridge_cnt = 0;
    }
  }
#endif
#if UART_Scope 
 if( XAxis_Interval++ > 100)
 {
   XAxis_Interval = 0;
//   Oscilloscope_Data[CH0] = SPD.Speed_fdb;        //被调量
//   Oscilloscope_Data[CH1] = PID16_SPD.Output_out;//PID输出
//   Oscilloscope_Data[CH2] = SPD.Speed_ref;//设定值
////   Oscilloscope_Data[CH3] = AD_result;
   UARTSend_Flag=1;
 }
#endif  
}

