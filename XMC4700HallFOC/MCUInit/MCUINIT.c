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
** ��������: void MCUInit(void)
** ��������: ����XMC1300��POSIF0��CCU40��ؼĴ���
**
** ˵�� ��:  ��SysTick��POSIF0��CCU40���г�ʼ��
**    ע��SysTickΪϵͳʱ�ӣ��������ʱ��ֵ���¡�ʱ�����ú���Ϊ��ͨ��ʱ��ʹ�ã�����ƥ�䴥����Ӧ�жϡ�
**********************************************************************************************************/
#define OPEN 1
#define CLOSE 0
/**��0��1��ѡ��ģʽ��0��ʾ�أ�1��ʾ��***/
#if CLOSE
#define Need_IPM_Init   2
#elif OPEN
#define TESTON_SMALLMORTOR   1
#endif
void MCUInit(void)
{
	  //**********ʱ�Ӻ�ȫ������**************************************************************************
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
  
  //��ʱ���ж�
   SystemCoreClockUpdate();
	 SysTick_Config(SystemCoreClock/1000);//1ms�ж�
  
	//������ģ���ʼ��
      MATH_Init();
      POSIF0_vInit();                          //POSIF0��ʼ��
      IO_vInit();                              //�˿ڳ�ʼ��
      CCU8_vInit();
      ADC_vInit();
      CCU40_vInit();                           //CCU40��ʼ��
      UART_Init();                            //UART��ʼ��
#ifdef TESTON_SMALLMORTOR  
      EN_GATE_ON;  //դ���ź�ʹ��
      DC_CAL_ON;  //����У׼ʹ��      
#endif
      delayTimer(20);      //�������

     //���ж�����
   NVIC_ClearPendingIRQ(CCU80_0_IRQn);//��CCU8�ж�
	 NVIC_EnableIRQ(CCU80_0_IRQn);
   
   NVIC_ClearPendingIRQ(CCU40_0_IRQn); //��CCU4�ж�
	 NVIC_EnableIRQ(CCU40_0_IRQn);
   
   NVIC_ClearPendingIRQ(VADC0_G1_0_IRQn);//��ADC�ж�
   NVIC_EnableIRQ(VADC0_G1_0_IRQn);
   
	 NVIC_ClearPendingIRQ(USIC0_0_IRQn);  //��UART�ж�												//����ڵ�ĵȴ�״̬
	 NVIC_EnableIRQ(USIC0_0_IRQn);		
	  //�ж����ȼ�
	  NVIC_SetPriority (POSIF0_0_IRQn, 0);      									 // POSIF  ���ȼ�0��
	  NVIC_SetPriority (CCU40_0_IRQn, 2);
	  NVIC_SetPriority (CCU80_0_IRQn, 1);
//		NVIC_SetPriority (CCU80_1_IRQn, 0X80);
    NVIC_SetPriority(SysTick_IRQn,3);
    NVIC_SetPriority(USIC0_0_IRQn, 3);
    NVIC_SetPriority(VADC0_G1_0_IRQn, 3);				
	  
}

void SysTick_Handler (void)
{
  if(!System_State_Flags.HALL_F)   //LED��ʱ
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
      P0_1_set();//U���ſ�
      P0_3_reset();//V���Ź�
      P0_9_reset();//W���Ź�
    }
    if(Bridge_cnt>50 && Bridge_cnt <= 100)
    {
      P0_1_reset();//U���Ź�
      P0_3_set();//V���ſ�
      P0_9_reset();//W���Ź�
    }
    if(Bridge_cnt>100 && Bridge_cnt <= 150)
    {
      P0_1_reset();//U���Ź�
      P0_3_reset();//V���Ź�
      P0_9_set();//W���ſ�
    }
    if(Bridge_cnt > 150)
    {
      P0_1_reset();//U���Ź�
      P0_3_reset();//V���Ź�
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
//   Oscilloscope_Data[CH0] = SPD.Speed_fdb;        //������
//   Oscilloscope_Data[CH1] = PID16_SPD.Output_out;//PID���
//   Oscilloscope_Data[CH2] = SPD.Speed_ref;//�趨ֵ
////   Oscilloscope_Data[CH3] = AD_result;
   UARTSend_Flag=1;
 }
#endif  
}

