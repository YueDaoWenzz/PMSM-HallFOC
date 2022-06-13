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
** ��������: void  (void)
** ��������:
**
** ˵�� ��:    �ж�
**
**********************************************************************************************************/

void PWM_One_Match_INT()           //CCU8SR0
{
    #if !STOP_MODE
    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {

    	if(System_State_Flags.START_F)
    	{
				
				  Get_Current();                  //��ȡA��B�����ֵ
				
				  Foc16.Idref_in 		= 	0;        //����Id=0����
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //����Iq�������ٶȱջ��ٶȸ���
					}
						 
					HALL_FOC_RUN();//HALL_FOCģʽ����
				
					
						
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //���one match�ж�״̬��־λ

    }
    #elif STOP_MODE
    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {
    	if(System_State_Flags.START_F )
    	{
				if(!System_State_Flags.STOP_F)    /*�������*/
        {

//				  Get_Current();                  //��ȡA��B�����ֵ
				
				  Foc16.Idref_in 		= 	0;        //����Id=0����
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //Iq�������ٶȱջ��ٶȸ���
					}
//						 
//          Bridge_Init();//��ʼ��һ��
					HALL_FOC_RUN();//HALL_FOCģʽ����
				}
        else
        {
          Mortor_Stop();
        }
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //���one match�ж�״̬��־λ
    }    
    #endif

}


//����ADת���ж�
void End_Conversion_ADC_INT ()
{
	
		System_State_Flags.AD_OK_F	=	1; //����������־λ
	
}

//+================================================================================================================
//		��������
//====================================================================================================================
__STATIC_INLINE void HALL_FOC_RUN()
{

    if(System_State_Flags.HALL_F)    //Hall�������
    {	
         if(System_State_Flags.FIRST_F)    //ͣ�����ٿ�����ͨ��
					{
					  	POSIF0->MCSM	 =	0x0333;						//��ͨ��״̬ ȫ��
              POSIF0->MCMS	|=	0x01;							//shadow transfer
						  System_State_Flags.FIRST_F = 0;
					}
    				 
				if(!Hall1.Startcorrect)
				{
					 if(!Hall1.Startcount)
           {WR_REG(CCU40_CC41->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1);}
					 
//					 POSIF_Hal_Start(); //Hall����
					 
					 if(Hall1.Startpwmcount<Hall1.Startpwmtime)   //������ʱ
							Hall1.Startpwmcount++;
					 else
					 {
					    Hall1.Startpwmcount=0;
						  Hall1.Startpwm-=20;   //ռ�ձȵ���
						 
						  if(Hall1.Startpwm<Hall1.StartpwmMax) //ռ�ձ��޷�
								 Hall1.Startpwm=Hall1.StartpwmMax;
						 
//						  Foc16.PWM_Tu_out	=	Hall1.Startpwm;
//							Foc16.PWM_Tv_out	=	Hall1.Startpwm;
//							Foc16.PWM_Tw_out	=	Hall1.Startpwm;
              Hall1.PWM_Duty = Hall1.StartpwmMax;
// 						  Foc16.PWM_Tu_out	=	Hall1.PWM_U;
//							Foc16.PWM_Tv_out	=	Hall1.PWM_U
//							Foc16.PWM_Tw_out	= Hall1.PWM_W;
							
//							Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);  //д��ռ�ձ�
//              Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
					 }
           		POSIF_Hal_Start(); //Hall����
              Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
				}
				else if(Hall1.Startcorrect) //�㶨ռ�ձȷ�������
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
            LED_ON;//��������״̬��ָʾ����
            Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
								 
				}
    	
    }
		else    //���Ҳ�����
		{
			  if(System_State_Flags.CLOSE_LOOP_F)
				{
					Hall1.Speedrpm=Hall_Speed_Calc(Hall1.SpeedBufer);	
				   Hall_Angle_Cal(&Hall1);//��������Ƕ�
					
					 Foc16.Angle_in = (uint16_t)Hall1.ElecTheta;
					
					 FOC16_Calc(3);	//���������
					
					 Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);
          if(LED_cnt >=1000)
          {
            LED_cnt = 0;
            LED_FLASH;
          }
				} 		       
		}
    
}


