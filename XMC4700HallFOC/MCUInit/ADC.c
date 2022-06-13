/****************************************Copyright (c)**************************************************
**
**---------------�ļ���Ϣ--------------------------------------------------------------------------------
**	��   ��  	��:	ADC.h
**
**  ��  ��  ��  ¼  ��
********************************************************************************************************/

#include <XMC1300.h>		//SFR declarations of the selected device
#include "MCUINIT.h"
#include "Parameters.h"
/*********************************************************************************************************
** ��������: void ADC_Init(void)
** ��������: ����ADC��ؼĴ���
**
2��3�����ʱ��
Iu====P2.11====G1CH3  +  Iv====P2.10=====G0Ch3
Iw====P2.3=====G1CH5
EMFW==P2.8=====G1CH0  +		EMFV==P2.6======G0CH0
UDC===P2.4=====G1CH6
POT===P2.5=====G1CH7	+		EMFU==P2.2====G0CH7

�������ʱ��
I_DC	���д���
����ȫ������background����

**
**ע�͵�ҳ���Ӧ	
**********************************************************************************************************/
//		*****************LOCAL variables***********************

uint16_t	AD_count;
#define CALIB01 0x480340E0
#define CALIB02 0x480340E4
uint32_t * variable01 = (uint32_t *) CALIB01;
uint32_t * variable02 = (uint32_t *) CALIB02;

//**********************************************************************************************************/


void ADC_vInit(void)
{

	VADC->CLC	&=	0xfffffffe;           //ʹ��ģ��ʱ��
	while(VADC->CLC!=0){}								//wait for clock ok

	VADC->CLC	|=	0x08;								  //��������ģʽ

	COMPARATOR->ORCCTRL	|=0x01;         //ʹ�ܳ����̱Ƚ��� ENORC0

	// Initiate start up calibration
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_DPCAL0_Msk, VADC_GLOBCFG_DPCAL0_Pos, 1);    //��0�޺�У׼
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_DPCAL1_Msk, VADC_GLOBCFG_DPCAL1_Pos, 1);    //��1�޺�У׼
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_SUCAL_Msk, VADC_GLOBCFG_SUCAL_Pos, 1);      //������ʼУ׼�׶�
	while((RD_REG(SHS0->SHSCFG, SHS_SHSCFG_STATE_Msk, SHS_SHSCFG_STATE_Pos))!= 0);
	*variable01 = 0x80008000;
	*variable02 = 0x80008000;

	 VADC->GLOBCFG |= 0x00030000;					// SUCAL=0, DPCAL1=1, DPCAL0=1 ��0��1�޺�У׼ ��ʼУ׼�޲���

	 VADC->GLOBCFG |= 0x00008000;			    // ��Ƶ��������д����ƣ����Ա�д��
	 for(AD_count=0;AD_count<1000;AD_count++);

	
	if(SHUNT_NUMBER != 1)
	{
		
			SHS0->SHSCFG	|=0x8000;             //SHSλ��ɱ�д��
			SHS0->STEPCFG	=	0x0089;             //�������ֵ�Ԫ
		
			VADC_G0->CHASS	|=	0xe9;						//CH7,CH6,CH5,CH3,CH0 ����0 ��һ������ͨ��
			VADC_G1->CHASS	|=	0xe9;           //CH7,CH6,CH5,CH3,CH0 ����1 ��һ������ͨ��

			VADC_G0->ARBPR	|=	0x0700088b;     //����Դ0�����ȼ���ߣ����룬���Գ�������Դ��ת�� Դ1��2���ȼ����
			VADC_G1->ARBPR	|=	0x0700088b;     //

			VADC_G1->SYNCTR	|=	0;							//�ں���ͬ�����ںˣ��޾����������
			VADC_G0->SYNCTR	|=	0x11;           //�ں���ͬ�����ںˣ���������R1�������Ǹ�ת����һ�β���ת���Ŀ�ʼ
		
			VADC_G1->QINR0	|=	0xa3;						//�ⲿ����  �Զ����� ��1��ͨ��0,3,5,6,7 ΪҪת����ͨ����
			VADC_G1->QINR0	|=	0x25;						//�ȴ������¼��ķ���
			VADC_G1->QINR0	|=	0x20;
			VADC_G1->QINR0	|=	0x26;
			VADC_G1->QINR0	|=	0x27;
		
		  VADC->BRSMR		|=	0x0211;	

			VADC_G1->QCTRL0	|=	0x0085ef00;			//trigger operation mode and external trigger level can be written
																					//trigger input :  CCU80.ST3  ����Դ  �κ�һ�����ؿ��Դ���
			VADC_G1->QMR0		|=	0x05;						//��1�й��𣬷���ת������  ʹ���ⲿ����
			
			VADC_G0->CHCTR[3]	|=	0x00030400;   //��0��ͨ��3������һ��ͬ��ת�� �������� G0RES3
			VADC_G1->CHCTR[3]	|=	0x00030400;   //��1��ͨ��3������һ��ͬ��ת�� �������� G1RES3
			VADC_G0->CHCTR[5]	|=	0x00050400;   //��0��ͨ��5������һ��ͬ��ת�� �������� G0RES5
			VADC_G1->CHCTR[5]	|=	0x00050400;   //��1��ͨ��5������һ��ͬ��ת�� �������� G1RES5
			VADC_G0->CHCTR[0]	|=	0x00000400;   
			VADC_G1->CHCTR[0]	|=	0x00000400;
			VADC_G0->CHCTR[6]	|=	0x00060400;
			VADC_G1->CHCTR[6]	|=	0x00060400;
			VADC_G0->CHCTR[7]	|=	0x00070400;
			VADC_G1->CHCTR[7]	|=	0x00070400;
			

			VADC_G1->RCR[5]		|=0x80000000;     //�� 1 ��������¼��󣬷�����������
			VADC_G1->REVNP0 =	0;								//�� 1 ����������0
			
			VADC_G0->ARBCFG= 0x00000003;			//The Arbitration Configuration Register selects the timing and the behavior of the arbiter.
			while(VADC_G0->ARBCFG & 0x10000000){}	//wait for start up calibration complete
			while((RD_REG(VADC_G0->ARBCFG, VADC_G_ARBCFG_CAL_Msk, VADC_G_ARBCFG_CAL_Pos))!= 0);
			VADC_G1->ARBCFG		= 0x00000003;			//set master's ANONC bit at the end of the initialization P1951
			while(VADC_G1->ARBCFG & 0x10000000){}		//CAL completed
			while((RD_REG(VADC_G1->ARBCFG, VADC_G_ARBCFG_CAL_Msk, VADC_G_ARBCFG_CAL_Pos))!= 0);			
			//��ʼУ׼���
				
//			NVIC_SetPriority(VADC0_G1_0_IRQn, 0);				//IRQ 19,level 35, VadcG1 SR0
//			NVIC_EnableIRQ(VADC0_G1_0_IRQn);			
	}
	
} 





