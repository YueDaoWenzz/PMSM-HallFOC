/****************************************Copyright (c)**************************************************
**  Ӣ����ʵ����
**---------------�ļ���Ϣ--------------------------------------------------------------------------------
**	��   ��  	��:	CCU8.c
**
**  ��  ��  ��  ¼  ��
********************************************************************************************************/

//CCU8ʹ�õ��Ǹ���ʱ�� PCLK 64MHz PWM����Ϊ(4000/64M)

#include <XMC1300.h>		//SFR declarations of the selected device
#include "CCU8.h"
#define Start_PWM                1    // 0=����PWM 1=�Գ�PWM
/*********************************************************************************************************

**********************************************************************************************************/
void CCU8_vInit(void)
{
	//-------------------------------------------------------------------------------------
	CCU80->GIDLC|=0x00000100;  				//ʹ��Ԥ��Ƶʱ��

	//-------------------------------------------------------------------------------------
	CCU80->GCTRL|=0X00000000;  				//����Ԥ��Ƶʱ�ӻ�׼Ƶ�ʣ����ö�ͨ��ģʽ�������Ӳ����ʽ�����µ�����

	//-------------------------------------------------------------------------------------
	//	6th	Step:	Configure all the registers related to the required Timer Slice(s) functions,	.
	///**********************����CCU80_CC80�Ĵ�������*************************************/
    //-------------------------------------------------------------------------------------

//cc80
	CCU80_CC80->CMC=0x00020001;  			//�ⲿ�¼��Ĺ������ú� ʱ�Ӽ������ã�P2369
											//��Ϊ	external start function	 event0
											//TRAP���ܽӵ�event2

	CCU80_CC80->INS=0x61210007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
											// Event0	->	CCU80.IN0H	-> SCU.GLCCST80  ȫ������CCU8 �¼�0 ������
											//Event2	->	TRAP (LEVEL active)	->	EV2 in0A ��ƽ��Ч (P0.7)
											//EV2	LP:11 7fccu8  EV2LM:1 L ���¼�2 �͵�ƽ��Ч  �ǵ�ƽ��Ч

	CCU80_CC80->TC|=0x007e0005;  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
											//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80���ĸ���
											//TRAP ͬ��PWM��������TRAP
	CCU80_CC80->TC|=0x06000000;				//ʹ�ܶ�ͨ��ģʽ

	CCU80_CC80->PSC=0x00000000;      		//Ԥ��Ƶʱ������
	CCU80_CC80->FPC=0x00000000;				//p2384
#if !Start_PWM
	CCU80_CC80->CHC=0x00000001;				//���Գ�PWM �Լ� �ȽϽ�����ѡ�� 		p2392
											//��Ϊ �ǶԳ� out00=ST1��out01=!ST1  �������
#elif Start_PWM
 	CCU80_CC80->CHC=0x00000004;//�Գ�PWM���ǻ������
//WR_REG(CCU80_CC80->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//�ǻ������									
#endif

	CCU80_CC80->DTC|=0x0000003f; 			//����ʱ��ʹ�ܺ�����ʱ������			 p2393
											//��Ϊ Fsys, ����ͨ��1��2ȫ��ʹ��

	//CCU80_CC80->DC1R|=0x00008080;			//ͨ��1����ʱ������ fall 0xf0, rising 0xf0  2us@64MHZ p2394
	//CCU80_CC80->DC2R|=0x00008080;			//ͨ��2����ʱ������

	CCU80_CC80->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;
	CCU80_CC80->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

	CCU80_CC80->PRS	=	PWM.PeriodValue_CentralAlign;				//����ֵ���� 	p2387 2000,���Ķ���ģʽ

	CCU80_CC80->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ1���� p2389
	CCU80_CC80->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ2����

	if(Passive_LEVEL_HighSide)
	{
		if(Passive_LEVEL_LowSide)
		{
			CCU80_CC80->PSL = 0x0000000f;   	//���������������ƽ���ã� passive level =H,H p2381 OUT00 OUT01 ������
		}
		else
		{
			CCU80_CC80->PSL = 0x00000005;     //���������������ƽ���ã� passive level =H,L p2381 OUT00�ߣ�OUT01��
		}
	}
	else
	{
		if(Passive_LEVEL_LowSide)
		{
			CCU80_CC80->PSL = 0x0000000a;   			//���������������ƽ���ã� passive level =L,H p2381
		}
		else
		{
			CCU80_CC80->PSL = 0;   					//���������������ƽ���ã� passive level =L,L p2381
		}
	}




	//cc81
		CCU80_CC81->CMC=0x00020001;  			//�ⲿ�¼��Ĺ������ú� ʱ�Ӽ������ã�P2369
												//��Ϊ	external start function	 event0
												//TRAP���ܽӵ�event2

		CCU80_CC81->INS=0x61210007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
												// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
												//Event2	->	TRAP (LEVEL active)	->	EV2 in0A ��ƽ��Ч (P0.7)
												//EV2	LP:11 7fccu8  EV2LM:1 L

		CCU80_CC81->TC|=0x007e0005;  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
												//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80���ĸ���
												//TRAP ͬ��PWM��������TRAP
		CCU80_CC81->TC|=0x06000000;				//ʹ�ܶ�ͨ��ģʽ

		CCU80_CC81->PSC=0x00000000;      		//Ԥ��Ƶʱ������
		CCU80_CC81->FPC=0x00000000;				//p2384
#if !Start_PWM
	CCU80_CC81->CHC=0x00000001;				//���Գ�PWM �Լ� �ȽϽ�����ѡ�� 		p2392
											//��Ϊ �ǶԳ� out00=ST1��out01=!ST1  �������
#elif Start_PWM
 	CCU80_CC81->CHC=0x00000004;//�Գ�PWM���ǻ������
//WR_REG(CCU80_CC81->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//�ǻ������
#endif

		CCU80_CC81->DTC|=0x0000003f; 			//����ʱ��ʹ�ܺ�����ʱ������			 p2393
												//��Ϊ Fsys, ����ͨ��1��2ȫ��ʹ��

		CCU80_CC81->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;
		CCU80_CC81->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

		CCU80_CC81->PRS	=	PWM.PeriodValue_CentralAlign;				//����ֵ���� 	p2387

		CCU80_CC81->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ1���� p2389
		CCU80_CC81->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ2����

		if(Passive_LEVEL_HighSide)
		{
			if(Passive_LEVEL_LowSide)
			{
				CCU80_CC81->PSL = 0x0000000f;   			//���������������ƽ���ã� passive level =H,H p2381
			}
			else
			{
				CCU80_CC81->PSL = 0x00000005;   			//���������������ƽ���ã� passive level =H,L p2381
			}
		}
		else
		{
			if(Passive_LEVEL_LowSide)
			{
				CCU80_CC81->PSL = 0x0000000a;   			//���������������ƽ���ã� passive level =L,H p2381
			}
			else
			{
				CCU80_CC81->PSL = 0;   					//���������������ƽ���ã� passive level =L,L p2381
			}
		}

		//cc82
			CCU80_CC82->CMC=0x00020001;  			//�ⲿ�¼��Ĺ������ú� ʱ�Ӽ������ã�P2369
													//��Ϊ	external start function	 event0
													//TRAP���ܽӵ�event2

			CCU80_CC82->INS=0x61210007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
													// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
													//Event2	->	TRAP (LEVEL active)	->	EV2 in0A ��ƽ��Ч (P0.7)
													//EV2	LP:11 7fccu8  EV2LM:1 L

			CCU80_CC82->TC|=0x007e0005;  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
													//��Ϊ ���Ķ��룬ʹ��shadow transfer��TRAP enabled forCC80���ĸ���
													//TRAP ͬ��PWM��������TRAP
			CCU80_CC82->TC|=0x06000000;				//ʹ�ܶ�ͨ��ģʽ

			CCU80_CC82->PSC=0x00000000;      		//Ԥ��Ƶʱ������
			CCU80_CC82->FPC=0x00000000;				//p2384

#if !Start_PWM
	CCU80_CC82->CHC=0x00000001;				//���Գ�PWM �Լ� �ȽϽ�����ѡ�� 		p2392
											//��Ϊ �ǶԳ� out00=ST1��out01=!ST1  �������
#elif Start_PWM
 	CCU80_CC82->CHC=0x00000004;//�Գ�PWM���ǻ������
//WR_REG(CCU80_CC82->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//�ǻ������
#endif

			CCU80_CC82->DTC|=0x0000003f; 			//����ʱ��ʹ�ܺ�����ʱ������			 p2393
													//��Ϊ Fsys, ����ͨ��1��2ȫ��ʹ��

			CCU80_CC82->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;      //д�������غ��½��ص�����ʱ��
			CCU80_CC82->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

			CCU80_CC82->PRS	=	PWM.PeriodValue_CentralAlign;				//����ֵ���� 	p2387

			CCU80_CC82->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ1���� p2389
			CCU80_CC82->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//�Ƚ�ֵ2����

			if(Passive_LEVEL_HighSide)
			{
				if(Passive_LEVEL_LowSide)
				{
					CCU80_CC82->PSL = 0x0000000f;   			//���������������ƽ���ã� passive level =H,H p2381
				}
				else
				{
					CCU80_CC82->PSL = 0x00000005;   			//���������������ƽ���ã� passive level =H,L p2381
				}
			}
			else
			{
				if(Passive_LEVEL_LowSide)
				{
					CCU80_CC82->PSL = 0x0000000a;   			//���������������ƽ���ã� passive level =L,H p2381
				}
				else
				{
					CCU80_CC82->PSL = 0;   					//���������������ƽ���ã� passive level =L,L p2381
				}
			}

//===================================================================================================
//			CC83 for ADC trig
//===================================================================================================
			//CC83		--  reserved for AD trig in single shunt mode
			//����Ϊ���ؼ���ģʽ������ֵ4000���Ƚ�ֵ,2��ST�����źŴ���AD������
			if(SHUNT_NUMBER != 1)
			{
				CCU80_CC83->CMC=0x00000001;  			//�ⲿ�¼��Ĺ������ú� ʱ�Ӽ������ã�P2369
														//��Ϊ	external start function	 event0 ����
														//TRAP����disable

				CCU80_CC83->INS=0x61210007;				//�����¼�ѡ���¼���Ч��ƽ�������غ������˲����á� p2367
														// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
														//Event2	->	TRAP (LEVEL active)	->	EV2 in0A ��ƽ��Ч (P0.7)
														//EV2	LP:11 7fccu8  EV2LM:1 L

				CCU80_CC83->TC|=0x00000004;  			//CCU8��Ԫ��ʱ��������ʽ������ģʽ�������ⲿ���ƹ��ܵĽ�ֹ��ʹ�� 	p2375
														//��Ϊ ���ض��룬ʹ��shadow transfer��TRAP disabled
														//

				CCU80_CC83->PSC=0x00000000;      		//Ԥ��Ƶʱ������
				CCU80_CC83->FPC=0x00000000;				//p2384

				CCU80_CC83->CHC=0x0000001f;				//���Գ�PWM �Լ� �ȽϽ�����ѡ�� 		p2392
														//��Ϊ �ǶԳ� out30=!ST1��out31=ST1  �����������������

				CCU80_CC83->DTC|= 0x00000000; 			//����ʱ��ʹ�ܺ�����ʱ������			 p2393
														//��Ϊ Fsys, ����ͨ��1��2ȫ����ʹ��  disable all dead zone

				CCU80_CC83->DC1R|=0x00000000;			//ͨ��1����ʱ������ fall 0xf0, rising 0xf0  2us@120MHZ p2394
				CCU80_CC83->DC2R|=0x00000000;			//ͨ��2����ʱ������

				CCU80_CC83->PRS	=	PWM.PeriodValue_EdgeAlign;	//����ֵ���� 	,���ض��룬4000�����Ա����Ķ����1��	p2387

				CCU80_CC83->CR1S=	1;					     //�Ƚ�ֵ1����		 p2389
				CCU80_CC83->CR2S=	1+CCU80_CC83->CR1S;	//�Ƚ�ֵ2

			}
			
			
		//***************	�ж����	**************************************************************************
		CCU80_CC80->INTE	|=	0x00000402; 			//�ж�ʹ��   one match ��event2 ��TRAP���ж�			p2402
		CCU80_CC80->SRS		|=	0x00001000;  			//�жϷ�������������: 									p2405
														//one match		CCU80 SR0 	Vector No.25							p169 table 4-3
														//event2(TRAP)	CCU80 SR1 	Vector No.26

//		NVIC_SetPriority(CCU80_0_IRQn, 1);				//IRQ 62,level 60,PWM_One_Match_INT    		CCU80_0_IRQHandler
//		NVIC_EnableIRQ(CCU80_0_IRQn);
//		NVIC_SetPriority(CCU80_1_IRQn, 0);				//IRQ 61,level 30,Trap_Protection_INT     	CCU80_1_IRQHandler
//		NVIC_EnableIRQ(CCU80_1_IRQn);
      NVIC_DisableIRQ(CCU80_1_IRQn);
		//��PWM��ͨ��
		POSIF0->MCSM	=	0;								  //��ͨ��״̬
		POSIF0->MCMS	|=	0x04;							//shadow transfer

		//****************	�ж����ok	**********************************************************************

		//-------------------------------------------------------------------------------------
		CCU80->GCSS|=0x00001111;     			//ʹ��CCU8����Ԫ��Ӱ����

		//-------------------------------------------------------------------------------------
		CCU80->GIDLC	|=	0x0000000f; 		//�������״̬ ������ԪԤ��Ƶʱ�� cc80,cc81,cc82,cc83

		//-------------------------------------------------------------------------------------
		SCU_GENERAL->CCUCON	|=	0x00000100; 		//CCU80��4��SLCIE cc80--cc83ȫ������			p526
		CCU80_CC80->SWR		|=	0x00000c01;			//���Period match�жϺ�TRAP�ж�״̬��־λ,event2�ж�״̬��־λ
		CCU80_CC81->SWR		|=	0x00000c01;			//���Period match�жϺ�TRAP�ж�״̬��־λ,event2�ж�״̬��־λ
		CCU80_CC82->SWR		|=	0x00000c01;			//���Period match�жϺ�TRAP�ж�״̬��־λ,event2�ж�״̬��־λ
		CCU80_CC83->SWR		|=	0x00000c01;

}






