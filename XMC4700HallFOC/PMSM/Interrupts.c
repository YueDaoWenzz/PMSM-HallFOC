/****************************************************************************
*    Filename:     Interrupts.c
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
#include "Interrupts.h"
#include "FOC.h"
#include "UserDefine.h"
#include "MCUINIT.h"
#include "PWMUpdate.h"
#include "UserDefine.h"
#include "Parameters.h"
#include "GetCurrent.h"
#include "POSIF0_Decoder.h"
/*****************************************************************************
*     Macro Definitions
*****************************************************************************/
extern int16_t Sec;

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

/*********************************************************************************************************
** 函数名称: void  (void)
** 功能描述:
**
** 说　 明:    中断
**
**********************************************************************************************************/

void PWM_One_Match_INT()           //CCU8SR0
{
    #if !STOP_MODE
    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {

    	if(System_State_Flags.START_F)
    	{
				
				  Get_Current();                  //读取A、B相电流值
				
				  Foc16.Idref_in 		= 	0;        //采用Id=0控制
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //开环Iq给定，速度闭环速度给定
					}
						 
					HALL_FOC_RUN();//HALL_FOC模式运行
				
					
						
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //清除one match中断状态标志位

    }
    #elif STOP_MODE
    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {
    	if(System_State_Flags.START_F )
    	{
				if(!System_State_Flags.STOP_F)    /*电机开启*/
        {

//				  Get_Current();                  //读取A、B相电流值
				
				  Foc16.Idref_in 		= 	0;        //采用Id=0控制
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //Iq给定，速度闭环速度给定
					}
//						 
//          Bridge_Init();//初始化一次
					HALL_FOC_RUN();//HALL_FOC模式运行
				}
        else
        {
          Mortor_Stop();
        }
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //清除one match中断状态标志位
    }    
    #endif

}


//结束AD转换中断
void End_Conversion_ADC_INT ()
{
	
		System_State_Flags.AD_OK_F	=	1; //采样结束标志位
	
}

//+================================================================================================================
//		启动过程
//====================================================================================================================
__STATIC_INLINE void HALL_FOC_RUN()
{

    if(System_State_Flags.HALL_F)    //Hall启动电机
    {	
         if(System_State_Flags.FIRST_F)    //停机后再开启多通道
					{
					  	POSIF0->MCSM	 =	0x0333;						//多通道状态 全开
              POSIF0->MCMS	|=	0x01;							//shadow transfer
						  System_State_Flags.FIRST_F = 0;
					}
    				 
				if(!Hall1.Startcorrect)
				{
					 if(!Hall1.Startcount)
           {WR_REG(CCU40_CC41->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1);}
					 
//					 POSIF_Hal_Start(); //Hall启动
					 
					 if(Hall1.Startpwmcount<Hall1.Startpwmtime)   //启动延时
							Hall1.Startpwmcount++;
					 else
					 {
					    Hall1.Startpwmcount=0;
						  Hall1.Startpwm-=20;   //占空比递增
						 
						  if(Hall1.Startpwm<Hall1.StartpwmMax) //占空比限幅
								 Hall1.Startpwm=Hall1.StartpwmMax;
						 
//						  Foc16.PWM_Tu_out	=	Hall1.Startpwm;
//							Foc16.PWM_Tv_out	=	Hall1.Startpwm;
//							Foc16.PWM_Tw_out	=	Hall1.Startpwm;
              Hall1.PWM_Duty = Hall1.StartpwmMax;
// 						  Foc16.PWM_Tu_out	=	Hall1.PWM_U;
//							Foc16.PWM_Tv_out	=	Hall1.PWM_U
//							Foc16.PWM_Tw_out	= Hall1.PWM_W;
							
//							Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);  //写入占空比
//              Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
					 }
           		POSIF_Hal_Start(); //Hall启动
              Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
				}
				else if(Hall1.Startcorrect) //恒定占空比方波运行
				{
//				   Hall1.Startpwm=Hall1.StartpwmMax;
////           Hall1.Startpwm = 2000- (int16_t)(POT >> 1);
//					 Foc16.PWM_Tu_out	=	Hall1.Startpwm;
//					 Foc16.PWM_Tv_out	=	Hall1.Startpwm;
//					 Foc16.PWM_Tw_out	=	Hall1.Startpwm;
           Hall1.PWM_Duty = Hall1.StartpwmMax;
//           Foc16.PWM_Tu_out	=	Hall1.PWM_U;
//					 Foc16.PWM_Tv_out	=	Hall1.PWM_V;
//					 Foc16.PWM_Tw_out	= Hall1.PWM_W;				
//					 Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);          
            LED_ON;//方波运行状态，指示灯亮
            Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
								 
				}
    	
    }
		else    //正弦波驱动
		{
			  if(System_State_Flags.CLOSE_LOOP_F)
				{
					Hall1.Speedrpm=Hall_Speed_Calc(Hall1.SpeedBufer);	
				   Hall_Angle_Cal(&Hall1);//霍尔计算角度
					
					 Foc16.Angle_in = (uint16_t)Hall1.ElecTheta;
					
					 FOC16_Calc(3);	//场定向控制
					
					 Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);
          if(LED_cnt >=1000)
          {
            LED_cnt = 0;
            LED_FLASH;
          }
				} 		       
		}
    
}


