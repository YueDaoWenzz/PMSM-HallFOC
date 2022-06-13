/*
 * GPIO.c
 *
 *  Created on: 2012-7-17
 *      Author: meng
 *

**			TRAP						P0_12
 *
 */
 #include <XMC1300.h>
 #include "GPIO.h"
 #include "gpio_xmc1300_tssop38.h"
 #define Mortor_Mode 1  // 1= small mortor 2= 5KW PMSM
void IO_vInit(void)
{
	 //POSIF HallL 端口    
		P0_13_set_mode(INPUT);  //HA
		P1_1_set_mode(INPUT);   //HB
		P1_0_set_mode(INPUT);		//HC
		P0_13_set_large_hysteresis();    
		P1_1_set_large_hysteresis();
		P1_0_set_large_hysteresis();
	
	  //PWM output
		P0_0_set_mode(OUTPUT_PP_AF5);					//ccu80.out00
		P0_1_set_mode(OUTPUT_PP_AF5);					//ccu80.out01

		P0_2_set_mode(OUTPUT_PP_AF7);					//ccu80.out10
		P0_3_set_mode(OUTPUT_PP_AF7);					//ccu80.out11

		P0_8_set_mode(OUTPUT_PP_AF5);					//ccu80.out20
		P0_9_set_mode(OUTPUT_PP_AF5);					//ccu80.out21
	
	  //PWM-ON
		P0_0_reset();					  //ccu80.out00
		P0_1_reset();					  //ccu80.out01

		P0_2_reset();					  //ccu80.out10
		P0_3_reset();					  //ccu80.out11

		P0_8_reset();					  //ccu80.out20
		P0_9_reset();					  //ccu80.out21
	  
	  //TRAP Fault input
	  P0_12_set_mode(INPUT_PU);						 //trap
//    P0_12_set_mode(OUTPUT_PP_GP);   //为了测试
//    P0_12_set();

// 5KW电机时候要关掉P0.11,P0.10
    #if Mortor_Mode ==1
    P0_11_set_mode(OUTPUT_PP_GP);       //EN_GATE   栅极使能
    P0_10_set_mode(OUTPUT_PP_GP);       //DC_CAL
    #endif

    P2_3_set_mode(INPUT);            //VBUS
    P2_5_set_mode(INPUT);            //温度
    P2_7_set_mode(INPUT);            //母线电流
    P2_10_set_mode(INPUT); //I_W
    P2_11_set_mode(INPUT); //I_U
    
    P0_7_set_mode(OUTPUT_PP_GP);     //指示灯    低有效	 LED_ON

//不用端口
    P0_6_set_mode(OUTPUT_PP_GP);
    P0_5_set_mode(OUTPUT_PP_GP);
    P0_6_set();
    P0_5_reset();
    
    P0_4_set_mode(INPUT);
    #if !Mortor_Mode
    P0_10_set_mode(INPUT);   //在小电机中要关掉
    P0_11_set_mode(INPUT);
    #endif

    P2_0_set_mode(INPUT);
    P2_2_set_mode(INPUT);
    P2_4_set_mode(INPUT);
    P2_6_set_mode(INPUT);
    P2_7_set_mode(INPUT);
}

