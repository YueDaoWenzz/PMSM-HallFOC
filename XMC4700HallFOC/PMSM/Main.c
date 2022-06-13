/*
 * Main.c
 *
 *  Created on: 
 *      First Author: LuJun
       Second Author��YangZezhong
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
	Parameters_Init();       //������ʼ��
	
			//���ж�
  __disable_irq();
	
	FOC_Init();              //PID��Hall��ʼ��
	MCUInit();               //MCU�����ʼ������
	
	Refresh_CCU80_PWM_Dutycycle(2000,2000,2000);
	
//	Get_Current_Offset();    //AD��׼ֵ��ʼ��   //������������
	__enable_irq();
	
	//����ͨ��
	POSIF0->MCSM	=	0x0333;							//��ͨ��״̬
  POSIF0->MCMS	|=	0x01;							//shadow transfer
	//POSIF0->MCMS	|=	0x04;							//shadow transfer  Decode�Ͷ�ͨ��һ����ʱ�����þ仰�����¶�ͨ��
  
  System_Flags_Init();//ϵͳ״̬λ��ʼ��
	WR_REG(CCU40_CC42->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //����	CC42

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
      Bridge_Init();//��ʼ��һ��
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

