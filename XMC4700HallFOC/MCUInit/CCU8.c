/****************************************Copyright (c)**************************************************
**  英飞凌实验室
**---------------文件信息--------------------------------------------------------------------------------
**	文   件  	名:	CCU8.c
**
**  修  改  纪  录  ：
********************************************************************************************************/

//CCU8使用的是高速时钟 PCLK 64MHz PWM周期为(4000/64M)

#include <XMC1300.h>		//SFR declarations of the selected device
#include "CCU8.h"
#define Start_PWM                1    // 0=互补PWM 1=对称PWM
/*********************************************************************************************************

**********************************************************************************************************/
void CCU8_vInit(void)
{
	//-------------------------------------------------------------------------------------
	CCU80->GIDLC|=0x00000100;  				//使能预分频时钟

	//-------------------------------------------------------------------------------------
	CCU80->GCTRL|=0X00000000;  				//设置预分频时钟基准频率，设置多通道模式下软件或硬件方式及更新的内容

	//-------------------------------------------------------------------------------------
	//	6th	Step:	Configure all the registers related to the required Timer Slice(s) functions,	.
	///**********************具体CCU80_CC80寄存器设置*************************************/
    //-------------------------------------------------------------------------------------

//cc80
	CCU80_CC80->CMC=0x00020001;  			//外部事件的功能配置和 时钟级联配置，P2369
											//设为	external start function	 event0
											//TRAP功能接到event2

	CCU80_CC80->INS=0x61210007;				//输入事件选择，事件有效电平、触发沿和输入滤波设置。 p2367
											// Event0	->	CCU80.IN0H	-> SCU.GLCCST80  全局启动CCU8 事件0 上升沿
											//Event2	->	TRAP (LEVEL active)	->	EV2 in0A 电平有效 (P0.7)
											//EV2	LP:11 7fccu8  EV2LM:1 L ，事件2 低电平有效  是电平有效

	CCU80_CC80->TC|=0x007e0005;  			//CCU8单元计时器计数方式、工作模式，多种外部控制功能的禁止和使能 	p2375
											//设为 中心对齐，使能shadow transfer，TRAP enabled forCC80的四个脚
											//TRAP 同步PWM，软件清除TRAP
	CCU80_CC80->TC|=0x06000000;				//使能多通道模式

	CCU80_CC80->PSC=0x00000000;      		//预分频时钟设置
	CCU80_CC80->FPC=0x00000000;				//p2384
#if !Start_PWM
	CCU80_CC80->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
											//设为 非对称 out00=ST1，out01=!ST1  互补输出
#elif Start_PWM
 	CCU80_CC80->CHC=0x00000004;//对称PWM，非互补输出
//WR_REG(CCU80_CC80->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//非互补输出									
#endif

	CCU80_CC80->DTC|=0x0000003f; 			//死区时间使能和死区时钟设置			 p2393
											//设为 Fsys, 死区通道1，2全部使能

	//CCU80_CC80->DC1R|=0x00008080;			//通道1死区时间配置 fall 0xf0, rising 0xf0  2us@64MHZ p2394
	//CCU80_CC80->DC2R|=0x00008080;			//通道2死区时间配置

	CCU80_CC80->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;
	CCU80_CC80->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

	CCU80_CC80->PRS	=	PWM.PeriodValue_CentralAlign;				//周期值设置 	p2387 2000,中心对其模式

	CCU80_CC80->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//比较值1设置 p2389
	CCU80_CC80->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//比较值2设置

	if(Passive_LEVEL_HighSide)
	{
		if(Passive_LEVEL_LowSide)
		{
			CCU80_CC80->PSL = 0x0000000f;   	//输出主动，被动电平设置， passive level =H,H p2381 OUT00 OUT01 被动高
		}
		else
		{
			CCU80_CC80->PSL = 0x00000005;     //输出主动，被动电平设置， passive level =H,L p2381 OUT00高，OUT01低
		}
	}
	else
	{
		if(Passive_LEVEL_LowSide)
		{
			CCU80_CC80->PSL = 0x0000000a;   			//输出主动，被动电平设置， passive level =L,H p2381
		}
		else
		{
			CCU80_CC80->PSL = 0;   					//输出主动，被动电平设置， passive level =L,L p2381
		}
	}




	//cc81
		CCU80_CC81->CMC=0x00020001;  			//外部事件的功能配置和 时钟级联配置，P2369
												//设为	external start function	 event0
												//TRAP功能接到event2

		CCU80_CC81->INS=0x61210007;				//输入事件选择，事件有效电平、触发沿和输入滤波设置。 p2367
												// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
												//Event2	->	TRAP (LEVEL active)	->	EV2 in0A 电平有效 (P0.7)
												//EV2	LP:11 7fccu8  EV2LM:1 L

		CCU80_CC81->TC|=0x007e0005;  			//CCU8单元计时器计数方式、工作模式，多种外部控制功能的禁止和使能 	p2375
												//设为 中心对齐，使能shadow transfer，TRAP enabled forCC80的四个脚
												//TRAP 同步PWM，软件清除TRAP
		CCU80_CC81->TC|=0x06000000;				//使能多通道模式

		CCU80_CC81->PSC=0x00000000;      		//预分频时钟设置
		CCU80_CC81->FPC=0x00000000;				//p2384
#if !Start_PWM
	CCU80_CC81->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
											//设为 非对称 out00=ST1，out01=!ST1  互补输出
#elif Start_PWM
 	CCU80_CC81->CHC=0x00000004;//对称PWM，非互补输出
//WR_REG(CCU80_CC81->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//非互补输出
#endif

		CCU80_CC81->DTC|=0x0000003f; 			//死区时间使能和死区时钟设置			 p2393
												//设为 Fsys, 死区通道1，2全部使能

		CCU80_CC81->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;
		CCU80_CC81->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

		CCU80_CC81->PRS	=	PWM.PeriodValue_CentralAlign;				//周期值设置 	p2387

		CCU80_CC81->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//比较值1设置 p2389
		CCU80_CC81->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//比较值2设置

		if(Passive_LEVEL_HighSide)
		{
			if(Passive_LEVEL_LowSide)
			{
				CCU80_CC81->PSL = 0x0000000f;   			//输出主动，被动电平设置， passive level =H,H p2381
			}
			else
			{
				CCU80_CC81->PSL = 0x00000005;   			//输出主动，被动电平设置， passive level =H,L p2381
			}
		}
		else
		{
			if(Passive_LEVEL_LowSide)
			{
				CCU80_CC81->PSL = 0x0000000a;   			//输出主动，被动电平设置， passive level =L,H p2381
			}
			else
			{
				CCU80_CC81->PSL = 0;   					//输出主动，被动电平设置， passive level =L,L p2381
			}
		}

		//cc82
			CCU80_CC82->CMC=0x00020001;  			//外部事件的功能配置和 时钟级联配置，P2369
													//设为	external start function	 event0
													//TRAP功能接到event2

			CCU80_CC82->INS=0x61210007;				//输入事件选择，事件有效电平、触发沿和输入滤波设置。 p2367
													// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
													//Event2	->	TRAP (LEVEL active)	->	EV2 in0A 电平有效 (P0.7)
													//EV2	LP:11 7fccu8  EV2LM:1 L

			CCU80_CC82->TC|=0x007e0005;  			//CCU8单元计时器计数方式、工作模式，多种外部控制功能的禁止和使能 	p2375
													//设为 中心对齐，使能shadow transfer，TRAP enabled forCC80的四个脚
													//TRAP 同步PWM，软件清除TRAP
			CCU80_CC82->TC|=0x06000000;				//使能多通道模式

			CCU80_CC82->PSC=0x00000000;      		//预分频时钟设置
			CCU80_CC82->FPC=0x00000000;				//p2384

#if !Start_PWM
	CCU80_CC82->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
											//设为 非对称 out00=ST1，out01=!ST1  互补输出
#elif Start_PWM
 	CCU80_CC82->CHC=0x00000004;//对称PWM，非互补输出
//WR_REG(CCU80_CC82->CHC, CCU8_CC8_CHC_OCS2_Msk, CCU8_CC8_CHC_OCS2_Pos, 1);//非互补输出
#endif

			CCU80_CC82->DTC|=0x0000003f; 			//死区时间使能和死区时钟设置			 p2393
													//设为 Fsys, 死区通道1，2全部使能

			CCU80_CC82->DC1R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;      //写入上升沿和下降沿的死区时间
			CCU80_CC82->DC2R	=	(PWM.DeadTimeValue<<8)+ PWM.DeadTimeValue;

			CCU80_CC82->PRS	=	PWM.PeriodValue_CentralAlign;				//周期值设置 	p2387

			CCU80_CC82->CR1S=	PWM.PeriodValue_CentralAlign>>1;				//比较值1设置 p2389
			CCU80_CC82->CR2S=	PWM.PeriodValue_CentralAlign>>1;				//比较值2设置

			if(Passive_LEVEL_HighSide)
			{
				if(Passive_LEVEL_LowSide)
				{
					CCU80_CC82->PSL = 0x0000000f;   			//输出主动，被动电平设置， passive level =H,H p2381
				}
				else
				{
					CCU80_CC82->PSL = 0x00000005;   			//输出主动，被动电平设置， passive level =H,L p2381
				}
			}
			else
			{
				if(Passive_LEVEL_LowSide)
				{
					CCU80_CC82->PSL = 0x0000000a;   			//输出主动，被动电平设置， passive level =L,H p2381
				}
				else
				{
					CCU80_CC82->PSL = 0;   					//输出主动，被动电平设置， passive level =L,L p2381
				}
			}

//===================================================================================================
//			CC83 for ADC trig
//===================================================================================================
			//CC83		--  reserved for AD trig in single shunt mode
			//设置为边沿计数模式，周期值4000，比较值,2，ST跳变信号触发AD采样。
			if(SHUNT_NUMBER != 1)
			{
				CCU80_CC83->CMC=0x00000001;  			//外部事件的功能配置和 时钟级联配置，P2369
														//设为	external start function	 event0 触发
														//TRAP功能disable

				CCU80_CC83->INS=0x61210007;				//输入事件选择，事件有效电平、触发沿和输入滤波设置。 p2367
														// Event0	->	CCU80.IN0H	-> SCU.GLCCST80
														//Event2	->	TRAP (LEVEL active)	->	EV2 in0A 电平有效 (P0.7)
														//EV2	LP:11 7fccu8  EV2LM:1 L

				CCU80_CC83->TC|=0x00000004;  			//CCU8单元计时器计数方式、工作模式，多种外部控制功能的禁止和使能 	p2375
														//设为 边沿对齐，使能shadow transfer，TRAP disabled
														//

				CCU80_CC83->PSC=0x00000000;      		//预分频时钟设置
				CCU80_CC83->FPC=0x00000000;				//p2384

				CCU80_CC83->CHC=0x0000001f;				//不对称PWM 以及 比较结果输出选择 		p2392
														//设为 非对称 out30=!ST1，out31=ST1  互补的输出，倒过来

				CCU80_CC83->DTC|= 0x00000000; 			//死区时间使能和死区时钟设置			 p2393
														//设为 Fsys, 死区通道1，2全部不使能  disable all dead zone

				CCU80_CC83->DC1R|=0x00000000;			//通道1死区时间配置 fall 0xf0, rising 0xf0  2us@120MHZ p2394
				CCU80_CC83->DC2R|=0x00000000;			//通道2死区时间配置

				CCU80_CC83->PRS	=	PWM.PeriodValue_EdgeAlign;	//周期值设置 	,边沿对齐，4000，所以比中心对齐大1倍	p2387

				CCU80_CC83->CR1S=	1;					     //比较值1设置		 p2389
				CCU80_CC83->CR2S=	1+CCU80_CC83->CR1S;	//比较值2

			}
			
			
		//***************	中断相关	**************************************************************************
		CCU80_CC80->INTE	|=	0x00000402; 			//中断使能   one match 和event2 （TRAP）中断			p2402
		CCU80_CC80->SRS		|=	0x00001000;  			//中断服务请求线配置: 									p2405
														//one match		CCU80 SR0 	Vector No.25							p169 table 4-3
														//event2(TRAP)	CCU80 SR1 	Vector No.26

//		NVIC_SetPriority(CCU80_0_IRQn, 1);				//IRQ 62,level 60,PWM_One_Match_INT    		CCU80_0_IRQHandler
//		NVIC_EnableIRQ(CCU80_0_IRQn);
//		NVIC_SetPriority(CCU80_1_IRQn, 0);				//IRQ 61,level 30,Trap_Protection_INT     	CCU80_1_IRQHandler
//		NVIC_EnableIRQ(CCU80_1_IRQn);
      NVIC_DisableIRQ(CCU80_1_IRQn);
		//关PWM多通道
		POSIF0->MCSM	=	0;								  //多通道状态
		POSIF0->MCMS	|=	0x04;							//shadow transfer

		//****************	中断相关ok	**********************************************************************

		//-------------------------------------------------------------------------------------
		CCU80->GCSS|=0x00001111;     			//使能CCU8各单元阴影传输

		//-------------------------------------------------------------------------------------
		CCU80->GIDLC	|=	0x0000000f; 		//解除闲置状态 启动单元预分频时钟 cc80,cc81,cc82,cc83

		//-------------------------------------------------------------------------------------
		SCU_GENERAL->CCUCON	|=	0x00000100; 		//CCU80的4个SLCIE cc80--cc83全局启动			p526
		CCU80_CC80->SWR		|=	0x00000c01;			//清除Period match中断和TRAP中断状态标志位,event2中断状态标志位
		CCU80_CC81->SWR		|=	0x00000c01;			//清除Period match中断和TRAP中断状态标志位,event2中断状态标志位
		CCU80_CC82->SWR		|=	0x00000c01;			//清除Period match中断和TRAP中断状态标志位,event2中断状态标志位
		CCU80_CC83->SWR		|=	0x00000c01;

}






