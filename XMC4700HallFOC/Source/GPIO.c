/*
 * GPIO.c
 *
 *  Created on: Nov 11, 2016
 *      Author: summerzhongzai
 */

#include <XMC4700.h>
#include "GPIO.h"

void GPIO_Init(void)
{
  //PWM out_put
  P0_5_set_mode(OUTPUT_PP_AF3); //U_H  CCU80.OUT00
  P0_5_set_driver_strength(MEDIUM);
  P0_2_set_mode(OUTPUT_PP_AF3); //U_L  CCU80.OUT00
  P0_2_set_driver_strength(MEDIUM);
  
  P0_4_set_mode(OUTPUT_PP_AF3); //V_H  CCU80.OUT00
  P0_4_set_driver_strength(MEDIUM);
  P0_1_set_mode(OUTPUT_PP_AF3); //V_L  CCU80.OUT00
  P0_1_set_driver_strength(MEDIUM);
  
  P0_3_set_mode(OUTPUT_PP_AF3); //W_H  CCU80.OUT00
  P0_3_set_driver_strength(MEDIUM);
  P0_0_set_mode(OUTPUT_PP_AF3); //W_L  CCU80.OUT00
  P0_0_set_driver_strength(MEDIUM);
  //INPUT
  P0_7_set_mode(INPUT);//TRAP
}