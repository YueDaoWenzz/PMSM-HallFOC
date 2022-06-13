/****************************************************************************
*    Filename:     POSIF0_Decoder.c
*    IDE:          DAVE_3.1.6
*    MCU:          XMC13OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    V1.0       2015-3-19   Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/


/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include "POSIF0_Decoder.h"
#include "FOC.h"
#include "GPIO.h"
#include "MATH_Init.h"
#include "PWMUpdate.h"
#define HALL_START_MODE 1  //1 HPWM_LON   0 DOUBLE PWM
/*****************************************************************************
*     Macro Definitions
*****************************************************************************/
const uint32_t Mcsms_opp[]={0x000, 0x210, 0x102, 0x012, 0x021 , 0x201, 0x120 };//多通道数组 326451正转
const uint32_t Mcsms_oppt[]={0x000, 0x120, 0x201, 0x021, 0x012 , 0x102, 0x210 };//反转

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


/*****************************************************************************
 * Function:      void  POSIF0_vInit(void)
 * Description:   Initialize the POSIF0 Module
 *                (使能POSIF0，配置为正交编码模式，配置A、B相输入端口，启动POSIF0)
 * Caveats:       2013-8-30
 *                仅用于测试POSIF0和CCU4联合的计数功能（计算位置信息）
 *****************************************************************************/
void POSIF0_vInit(void)
{
	 // Configure hall sensor mode, hall inputs and sample trigger source
	WR_REG(POSIF0->PCONF, POSIF_PCONF_FSEL_Msk, POSIF_PCONF_FSEL_Pos, 0);//霍尔模式
	WR_REG(POSIF0->PCONF, POSIF_PCONF_HIDG_Msk, POSIF_PCONF_HIDG_Pos, 1);//
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MCUE_Msk, POSIF_PCONF_MCUE_Pos, 1);     //软件跟新多通道序列    
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL0_Msk, POSIF_PCONF_INSEL0_Pos, 1); // Select IN0A at P0.13  HA
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL1_Msk, POSIF_PCONF_INSEL1_Pos, 0); // Select IN1A at P1.1   HB
	WR_REG(POSIF0->PCONF, POSIF_PCONF_INSEL2_Msk, POSIF_PCONF_INSEL2_Pos, 0); // Select IN2A at P1.0   HC
	
	WR_REG(POSIF0->PCONF, POSIF_PCONF_DSEL_Msk, POSIF_PCONF_DSEL_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_SPES_Msk, POSIF_PCONF_SPES_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSETS_Msk, POSIF_PCONF_MSETS_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSES_Msk, POSIF_PCONF_MSES_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_MSYNS_Msk, POSIF_PCONF_MSYNS_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIS_Msk, POSIF_PCONF_EWIS_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIE_Msk, POSIF_PCONF_EWIE_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_EWIL_Msk, POSIF_PCONF_EWIL_Pos, 0);
	WR_REG(POSIF0->PCONF, POSIF_PCONF_LPC_Msk, POSIF_PCONF_LPC_Pos, 0);
	
	WR_REG(POSIF0->PFLGE, POSIF_PFLGE_EHIE_Msk, POSIF_PFLGE_EHIE_Pos, 1);	//霍尔输入更新使能
  WR_REG(POSIF0->PFLGE, POSIF_PFLGE_HIESEL_Msk, POSIF_PFLGE_HIESEL_Pos, 0); //霍尔输入跟新中断转发到SR0
	
	
	WR_REG(POSIF0->PCONF, POSIF_PCONF_LPC_Msk, POSIF_PCONF_LPC_Pos, 111);//低通滤波器配置(64个时钟周期)2us
	
	//POSIF 换向中断
 NVIC_ClearPendingIRQ(POSIF0_0_IRQn);		                     //This function clears interrupt node pending status
 NVIC_EnableIRQ(POSIF0_0_IRQn);			                         //This function enables interrupt node for interrupt request generation

	// Set run bit	
 WR_REG(POSIF0->PRUNS, POSIF_PRUNS_SRB_Msk, POSIF_PRUNS_SRB_Pos, 1);	
	   
}

void POSIF_Hal_Start(void)
{
  uint8_t Hall_port = 0;
  Hall_port = PORT1->IN & 0x07;
  Hall1.Patt = ((((PORT0->IN >> 13)&0x01) | (Hall_port & 0x02) | ((Hall_port << 2)& 0x04))&0x07);
//  Hall1.Patt = 3;
	if(Hall1.Patt != Hall1.Pattpre)
	{				
				 Hall1.Pattpre=Hall1.Patt;
				 if(Hall1.Direction == 1)	
				 {	
           POSIF0->MCSM = Mcsms_opp[Hall1.Patt];//写入多通道
           POSIF0->MCMS |= 0x04; 
           //换相
            switch(Hall1.Patt)
            {
              case 3:   //B+A-
              {
                Refresh_Duty(0,Hall1.PWM_Duty,2000);   //写入占空比，同时导通对应相
              }
              break;
              case 2:   //C+A-
              {
                Refresh_Duty(0,2000,Hall1.PWM_Duty);
              }
              break;
              case 6:   //C+B-
              {
                Refresh_Duty(2000,0,Hall1.PWM_Duty);
              }
              break;
              case 4:   //A+B-
              {
                Refresh_Duty(Hall1.PWM_Duty,0,2000);
              }
              break;
              case 5: //A+C-
              {
                Refresh_Duty(Hall1.PWM_Duty,2000,0);
              }
              break;
              case 1: //B+C-
              {
               Refresh_Duty(2000,Hall1.PWM_Duty,0);
              }
              break;
              default:break;              

            } 					 						 																							 						
				 }
				 else if(Hall1.Direction==2) 
				 {							
            //换相
           POSIF0->MCSM = Mcsms_oppt[Hall1.Patt];//写入多通道
           POSIF0->MCMS |= 0x04; 

            switch(Hall1.Patt)
            {
              case 3:  //B+C-
              {
                Refresh_Duty(2000,Hall1.PWM_Duty,0); 
              }
              break;    
              case 6:  //C+A-
              {
                Refresh_Duty(0,2000,Hall1.PWM_Duty);
              }
              break;
              case 2:  //B+A-
              {
                Refresh_Duty(0,Hall1.PWM_Duty,2000);
              }
              break;
              case 5:  //A+B-
              {
                Refresh_Duty(Hall1.PWM_Duty,0,2000);
              }
              break;
              case 1:  //A+C-
              {
                Refresh_Duty(Hall1.PWM_Duty,2000,0);
              }
              break;
              case 4:  //C+B-
              {
                Refresh_Duty(2000,0,Hall1.PWM_Duty);
              }
              break;
              default: break;
            }				
				 }

         if(!Hall_start_flag)
         {
           Start_flag ++ ;
           if(Start_flag >= 600)   //切换到中断中运行
           {
             Start_flag = 0;
             Hall_start_flag = 1;
           }
         }
			
	}
}
void POSIF0_0_IRQHandler(void)
{
    uint8_t Hall_port1 = 0;
		if((RD_REG(POSIF0->PFLG, POSIF_PFLG_HIES_Msk, POSIF_PFLG_HIES_Pos))==1)
		{		
//        if(Hall_start_flag)
//      {      
        if(Hall1.Direction==2)                                                    
				 {			  
						  WR_REG(CCU40_CC41->TCCLR, CCU4_CC4_TCCLR_TRBC_Msk, CCU4_CC4_TCCLR_TRBC_Pos, 1);//清除运行位
							Hall1.Huanxiangtime = RD_REG(CCU40_CC41->TIMER, CCU4_CC4_TIMER_TVAL_Msk, CCU4_CC4_TIMER_TVAL_Pos);
							WR_REG(CCU40_CC41->TCCLR, CCU4_CC4_TCCLR_TCC_Msk, CCU4_CC4_TCCLR_TCC_Pos, 1);//清空定时器
							WR_REG(CCU40_CC41->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //重新启动
           
					    Hall1.KCount=0;	//增量角度计数清零					
					    WR_REG(CCU40_CC42->TCCLR, CCU4_CC4_TCCLR_TRBC_Msk, CCU4_CC4_TCCLR_TRBC_Pos, 1);//清除运行位		
					    WR_REG(CCU40_CC42->TCCLR, CCU4_CC4_TCCLR_TCC_Msk, CCU4_CC4_TCCLR_TCC_Pos, 1);//清空定时器
					    WR_REG(CCU40_CC42->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, MDU_Divider(Hall1.Huanxiangtime<<7,Hall1.Numencoder));//左移7位为了匹配，60度计数值与计算角度计数值匹配
					    WR_REG(CCU40->GCSS, CCU4_GCSS_S2SE_Msk, CCU4_GCSS_S2SE_Pos, 1);
              WR_REG(CCU40_CC42->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //重新启动	 
					  

					 //反转角度估计？
					    switch(Hall1.Patt)//初始位置角计算    //这个一定要估测好 否则雪崩啊   //参考小电机的估测
							{
								  case 5: Hall1.Thetachushi=0+Hall1.Thetaoft;   break;
								  case 1: Hall1.Thetachushi=60+Hall1.Thetaoft;  break;
								  case 3: Hall1.Thetachushi=120+Hall1.Thetaoft; break;
								  case 2: Hall1.Thetachushi=180+Hall1.Thetaoft; break;
								  case 6: Hall1.Thetachushi=240+Hall1.Thetaoft; break;
								  case 4: Hall1.Thetachushi=300+Hall1.Thetaoft; break;
								  default:break;
								 
							}

					 	if(!System_State_Flags.CLOSE_LOOP_F)  // 方波霍尔运行模式
            {
                POSIF0->MCSM = Mcsms_opp[Hall1.Patt];//写入多通道
                WR_REG(POSIF0->MCMS, POSIF_MCMS_MNPS_Msk, POSIF_MCMS_MNPS_Pos, 1);//使能多通道跟新
                WR_REG(POSIF0->MCMS, POSIF_MCMS_STMR_Msk, POSIF_MCMS_STMR_Pos, 1);//多通道序列传送请求
                //换相
                switch(Hall1.Patt)
                {
                  case 3:  //B+C-
                  {
                    Refresh_Duty(2000,Hall1.PWM_Duty,0);
                  }
                  break;    
                  case 6:  //C+A-
                  {
                    Refresh_Duty(0,2000,Hall1.PWM_Duty);
                  }
                  break;
                  case 2:  //B+A-
                  {
                    Refresh_Duty(0,Hall1.PWM_Duty,2000);
                  }
                  break;
                  case 5:  //A+B-
                  {
                    Refresh_Duty(Hall1.PWM_Duty,0,2000);
                  }
                  break;
                  case 1:  //A+C-
                  {
                    Refresh_Duty(Hall1.PWM_Duty,2000,0);
                  }
                  break;
                  case 4:  //C+B-
                  {
                    Refresh_Duty(2000,0,Hall1.PWM_Duty);
                  }
                  break;
                  default: break;
                }
                
                                                            
						   //启动次数
						  if(!Hall1.Startcorrect)
							{
								 Hall1.Startcount++;
								 if(Hall1.Startcount>=Hall1.StartNum) 
								 {
										Hall1.Startcount = 0;
										Hall1.Startcorrect = 1;						    
								 }
							} 
              else
              {
                 Hall1.Startcount++;
                 if(Hall1.Startcount>=Hall1.StartNum)     //方波运行多次后切换到正弦波？用转速条件切换，还是换相次数进行切换呢？
                 {
                    POSIF0->MCSM	=	0x0000;							//关多通道
                    POSIF0->MCMS	|=	0x01;							//shadow transfer
                   
                    Hall_Angle_Cal(&Hall1);
      
                    Foc16.Angle_in = (uint16_t)Hall1.ElecTheta;										
                    FOC16_Calc(3);	
                    Refresh_CCU80_PWM_Dutycycle(Foc16.PWM_Tu_out,Foc16.PWM_Tv_out,Foc16.PWM_Tw_out);
                   
                    P0_0_set_mode(OUTPUT_PP_AF5);					//ccu80.out00
                    P0_1_set_mode(OUTPUT_PP_AF5);					//ccu80.out01

                    P0_2_set_mode(OUTPUT_PP_AF7);					//ccu80.out10
                    P0_3_set_mode(OUTPUT_PP_AF7);					//ccu80.out11

                    P0_8_set_mode(OUTPUT_PP_AF5);					//ccu80.out20
                    P0_9_set_mode(OUTPUT_PP_AF5);					//ccu80.out21
                   
                    POSIF0->MCSM	=	0x0333;							  //开多通道
                    POSIF0->MCMS	|=	0x01;							  //shadow transfer
                   
                    Hall1.Startcount = 0;
                    System_State_Flags.HALL_F=0;
                   
                    System_State_Flags.CLOSE_LOOP_F	=1;	  //切换到正弦波模式				    
                 }
             
              }		              
            }				
			   }
				 else  if(Hall1.Direction == 1)                                            
				 {   
              uint8_t k = 0;

                      /*换相时间计算*/
							WR_REG(CCU40_CC41->TCCLR, CCU4_CC4_TCCLR_TRBC_Msk, CCU4_CC4_TCCLR_TRBC_Pos, 1);//清除运行位		    
							Hall1.Huanxiangtime = RD_REG(CCU40_CC41->TIMER, CCU4_CC4_TIMER_TVAL_Msk, CCU4_CC4_TIMER_TVAL_Pos);
           
              Hall1.SpeedBufer[Hall1.Speed_Cnt++] = Hall1.Huanxiangtime; 	//计算转速
					    if(Hall1.Speed_Cnt==6)
									Hall1.Speed_Cnt=0;
              if(Hall1.Speedrpm > CommuTimeCal_Change_SpeedrpmThreshold ) /*转速大于阈值，采用6次区间平均速度计算*/
              {
                Hall1.Huanxiangtime_Array[Hall1.Huanxiangtime_cnt++] = Hall1.Huanxiangtime;
                if(Hall1.Huanxiangtime_cnt >= 6)
                {
                  for(k=0;k < 6;k++)
                  {
                    Hall1.Huangxiangtime_sum += Hall1.Huanxiangtime_Array[k];
                  }
                  Hall1.Huanxiangtime = Hall1.Huangxiangtime_sum/6;
                  Hall1.Huanxiangtime_cnt = 0;
                  Hall1.Huangxiangtime_sum = 0;
                }
              }
              Hall1.SecorTime_Bufer[Hall1.SectorTime_cnt++]=Hall1.Huanxiangtime;  /*记录当前两次扇区时间*/
              if(Hall1.SectorTime_cnt == 2)
                Hall1.SectorTime_cnt = 0;
              
							WR_REG(CCU40_CC41->TCCLR, CCU4_CC4_TCCLR_TCC_Msk, CCU4_CC4_TCCLR_TCC_Pos, 1);//清空定时器
							WR_REG(CCU40_CC41->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //重新启动
              
					    Hall1.KCount=0;	//增量角度计数清零					
					    WR_REG(CCU40_CC42->TCCLR, CCU4_CC4_TCCLR_TRBC_Msk, CCU4_CC4_TCCLR_TRBC_Pos, 1);//清除运行位		
					    WR_REG(CCU40_CC42->TCCLR, CCU4_CC4_TCCLR_TCC_Msk, CCU4_CC4_TCCLR_TCC_Pos, 1);//清空定时器
					    WR_REG(CCU40_CC42->PRS, CCU4_CC4_PRS_PRS_Msk, CCU4_CC4_PRS_PRS_Pos, MDU_Divider(Hall1.Huanxiangtime<<7,Hall1.Numencoder));//左移7位为了匹配，CC41是128分频，CC42是1分频，写入到CC42的周期值在CC41的计数值中应放大128倍
					    WR_REG(CCU40->GCSS, CCU4_GCSS_S2SE_Msk, CCU4_GCSS_S2SE_Pos, 1);
              WR_REG(CCU40_CC42->TCSET, CCU4_CC4_TCSET_TRBS_Msk, CCU4_CC4_TCSET_TRBS_Pos, 1); //重新启动	 
                  

					  
				      Hall_port1 = PORT1->IN & 0x07;
              Hall1.Patt = ((((PORT0->IN >> 13)&0x01) | (Hall_port1 & 0x02) | ((Hall_port1 << 2)& 0x04))&0x07); 

					    switch(Hall1.Patt)//初始位置角计算    //
							{
                case 3: Hall1.Thetachushi=30+Hall1.Thetaoft;  break;
                case 2: Hall1.Thetachushi=90+Hall1.Thetaoft;  break;
                case 6: Hall1.Thetachushi=150+Hall1.Thetaoft; break;
                case 4: Hall1.Thetachushi=210+Hall1.Thetaoft; break;
                case 5: Hall1.Thetachushi=270+Hall1.Thetaoft; break;
                case 1: Hall1.Thetachushi=330+Hall1.Thetaoft; break;
                default:break; 
							}
              if (Hall1.Thetachushi>=360)
              {
                Hall1.Thetachushi -=360;
              }
					 
							if(!System_State_Flags.CLOSE_LOOP_F)  // 方波霍尔运行模式
							{
							    //换相 
                POSIF0->MCSM = Mcsms_opp[Hall1.Patt];//写入多通道
                POSIF0->MCMS |= 0x04; 
                switch(Hall1.Patt)
                {
                    case 3:   //B+A-
                    {
                      Refresh_Duty(0,Hall1.PWM_Duty,2000);
                    }
                    break;
                    case 2:   //C+A-
                    {
                      Refresh_Duty(0,2000,Hall1.PWM_Duty);
                    }
                    break;
                    case 6:   //C+B-
                    {
                      Refresh_Duty(2000,0,Hall1.PWM_Duty);
                    }
                    break;
                    case 4:   //A+B-
                    {
                      Refresh_Duty(Hall1.PWM_Duty,0,2000);
                    }
                    break;
                    case 5: //A+C-
                    {
                      Refresh_Duty(Hall1.PWM_Duty,2000,0);
                    }
                    break;
                    case 1: //B+C-
                    {
                     Refresh_Duty(2000,Hall1.PWM_Duty,0);
                    }
                    break;
                    default:break;                    
                }                 
																							
									//启动次数
									if(!Hall1.Startcorrect)
									{
										 Hall1.Startcount++;
										 if(Hall1.Startcount>=Hall1.StartNum) 
										 {
												Hall1.Startcount = 0;
												Hall1.Startcorrect = 1;						    
										 }
									} 
									else
									{
										 Hall1.Startcount++;
										 if(Hall1.Startcount>=Hall1.StartNum)     //方波运行多次后切换到正弦波？用转速条件切换，还是换相次数进行切换呢？
										 {
											  POSIF0->MCSM	=	0x0000;							//关多通道
												POSIF0->MCMS	|=	0x01;							//shadow transfer
											 
    									  Hall_Angle_Cal(&Hall1);
					
											  Foc16.Angle_in = (uint16_t)Hall1.ElecTheta;										
											  FOC16_Calc(3);	
                        Refresh_CCU80_PWM_Dutycycle(Hall1.PWM_U,Hall1.PWM_V,Hall1.PWM_W);
											 
//											  P0_0_set_mode(OUTPUT_PP_AF5);					//ccu80.out00
//												P0_1_set_mode(OUTPUT_PP_AF5);					//ccu80.out01

//												P0_2_set_mode(OUTPUT_PP_AF7);					//ccu80.out10
//												P0_3_set_mode(OUTPUT_PP_AF7);					//ccu80.out11

//												P0_8_set_mode(OUTPUT_PP_AF5);					//ccu80.out20
//												P0_9_set_mode(OUTPUT_PP_AF5);					//ccu80.out21
											 

											 
											  Hall1.Startcount = 0;
											  System_State_Flags.HALL_F=0;
												System_State_Flags.CLOSE_LOOP_F	=1;	  //切换到正弦波模式
                        
                        //配置为互补的PWM波                        
                        CCU80_CC80->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
                                                          //设为 非对称 out00=ST1，out01=!ST1  互补输出 
                        CCU80_CC81->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
                                                          //设为 非对称 out00=ST1，out01=!ST1  互补输出 
                        CCU80_CC82->CHC=0x00000001;				//不对称PWM 以及 比较结果输出选择 		p2392
                                                          //设为 非对称 out00=ST1，out01=!ST1  互补输出 
//                        CCU80->GCSS|=0x00001111;     			//使能CCU8各单元阴影传输
                                                          
											  POSIF0->MCSM	=	0x0333;							  //开多通道
												POSIF0->MCMS	|=	0x01;							  //shadow transfer                                                          
										 } 
							    }								
							}
					    
				 }	
//            Hall1.Huanxiangtime_old = Hall1.Huanxiangtime; //记录上一区间的换相时间
    		         			
//		     Hall1.Pattpre=Hall1.Patt;
//       }
		}
		//清除中断标志 
  		SET_BIT(POSIF0->RPFLG, POSIF_RPFLG_RHIE_Pos); 
}
