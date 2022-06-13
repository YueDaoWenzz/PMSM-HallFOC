/****************************************Copyright (c)**************************************************
**
**---------------文件信息--------------------------------------------------------------------------------
**	文   件  	名:	ADC.h
**
**  修  改  纪  录  ：
********************************************************************************************************/

#include <XMC1300.h>		//SFR declarations of the selected device
#include "MCUINIT.h"
#include "Parameters.h"
/*********************************************************************************************************
** 函数名称: void ADC_Init(void)
** 功能描述: 配置ADC相关寄存器
**
2、3电阻的时候
Iu====P2.11====G1CH3  +  Iv====P2.10=====G0Ch3
Iw====P2.3=====G1CH5
EMFW==P2.8=====G1CH0  +		EMFV==P2.6======G0CH0
UDC===P2.4=====G1CH6
POT===P2.5=====G1CH7	+		EMFU==P2.2====G0CH7

单电阻的时候
I_DC	队列触发
其余全部背景background运行

**
**注释的页码对应	
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

	VADC->CLC	&=	0xfffffffe;           //使能模块时钟
	while(VADC->CLC!=0){}								//wait for clock ok

	VADC->CLC	|=	0x08;								  //忽略休眠模式

	COMPARATOR->ORCCTRL	|=0x01;         //使能超量程比较器 ENORC0

	// Initiate start up calibration
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_DPCAL0_Msk, VADC_GLOBCFG_DPCAL0_Pos, 1);    //组0无后校准
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_DPCAL1_Msk, VADC_GLOBCFG_DPCAL1_Pos, 1);    //组1无后校准
	WR_REG(VADC->GLOBCFG, VADC_GLOBCFG_SUCAL_Msk, VADC_GLOBCFG_SUCAL_Pos, 1);      //启动初始校准阶段
	while((RD_REG(SHS0->SHSCFG, SHS_SHSCFG_STATE_Msk, SHS_SHSCFG_STATE_Pos))!= 0);
	*variable01 = 0x80008000;
	*variable02 = 0x80008000;

	 VADC->GLOBCFG |= 0x00030000;					// SUCAL=0, DPCAL1=1, DPCAL0=1 组0、1无后校准 初始校准无操作

	 VADC->GLOBCFG |= 0x00008000;			    // 分频器参数的写入控制，可以被写入
	 for(AD_count=0;AD_count<1000;AD_count++);

	
	if(SHUNT_NUMBER != 1)
	{
		
			SHS0->SHSCFG	|=0x8000;             //SHS位域可被写入
			SHS0->STEPCFG	=	0x0089;             //采样保持单元
		
			VADC_G0->CHASS	|=	0xe9;						//CH7,CH6,CH5,CH3,CH0 是组0 的一个优先通道
			VADC_G1->CHASS	|=	0xe9;           //CH7,CH6,CH5,CH3,CH0 是组1 的一个优先通道

			VADC_G0->ARBPR	|=	0x0700088b;     //请求源0，优先极最高，插入，可以撤销其他源的转换 源1、2优先级最低
			VADC_G1->ARBPR	|=	0x0700088b;     //

			VADC_G1->SYNCTR	|=	0;							//内核是同步主内核，无就绪输入控制
			VADC_G0->SYNCTR	|=	0x11;           //内核是同步从内核，就绪输入R1被看成是该转换组一次并行转换的开始
		
			VADC_G1->QINR0	|=	0xa3;						//外部触发  自动重填 组1：通道0,3,5,6,7 为要转换的通道号
			VADC_G1->QINR0	|=	0x25;						//等待触发事件的发生
			VADC_G1->QINR0	|=	0x20;
			VADC_G1->QINR0	|=	0x26;
			VADC_G1->QINR0	|=	0x27;
		
		  VADC->BRSMR		|=	0x0211;	

			VADC_G1->QCTRL0	|=	0x0085ef00;			//trigger operation mode and external trigger level can be written
																					//trigger input :  CCU80.ST3  触发源  任何一个边沿可以触发
			VADC_G1->QMR0		|=	0x05;						//组1有挂起，发出转换请求  使能外部触发
			
			VADC_G0->CHCTR[3]	|=	0x00030400;   //组0，通道3，请求一次同步转换 保存结果到 G0RES3
			VADC_G1->CHCTR[3]	|=	0x00030400;   //组1，通道3，请求一次同步转换 保存结果到 G1RES3
			VADC_G0->CHCTR[5]	|=	0x00050400;   //组0，通道5，请求一次同步转换 保存结果到 G0RES5
			VADC_G1->CHCTR[5]	|=	0x00050400;   //组1，通道5，请求一次同步转换 保存结果到 G1RES5
			VADC_G0->CHCTR[0]	|=	0x00000400;   
			VADC_G1->CHCTR[0]	|=	0x00000400;
			VADC_G0->CHCTR[6]	|=	0x00060400;
			VADC_G1->CHCTR[6]	|=	0x00060400;
			VADC_G0->CHCTR[7]	|=	0x00070400;
			VADC_G1->CHCTR[7]	|=	0x00070400;
			

			VADC_G1->RCR[5]		|=0x80000000;     //组 1 发生结果事件后，发出服务请求
			VADC_G1->REVNP0 =	0;								//组 1 服务请求线0
			
			VADC_G0->ARBCFG= 0x00000003;			//The Arbitration Configuration Register selects the timing and the behavior of the arbiter.
			while(VADC_G0->ARBCFG & 0x10000000){}	//wait for start up calibration complete
			while((RD_REG(VADC_G0->ARBCFG, VADC_G_ARBCFG_CAL_Msk, VADC_G_ARBCFG_CAL_Pos))!= 0);
			VADC_G1->ARBCFG		= 0x00000003;			//set master's ANONC bit at the end of the initialization P1951
			while(VADC_G1->ARBCFG & 0x10000000){}		//CAL completed
			while((RD_REG(VADC_G1->ARBCFG, VADC_G_ARBCFG_CAL_Msk, VADC_G_ARBCFG_CAL_Pos))!= 0);			
			//初始校准完毕
				
//			NVIC_SetPriority(VADC0_G1_0_IRQn, 0);				//IRQ 19,level 35, VadcG1 SR0
//			NVIC_EnableIRQ(VADC0_G1_0_IRQn);			
	}
	
} 





