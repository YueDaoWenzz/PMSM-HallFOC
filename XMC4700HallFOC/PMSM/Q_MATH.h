/*
 * Q_MATH.H
 *
 *  Created on: Apr 14, 2017
 *      Author: summerzhongzai
 */

#ifndef Q_MATH_H_
#define Q_MATH_H_
#include  <XMC1300.h>
#ifdef  GLOBAL_Q
#define GLOBAL_Q    20
#endif
/***Q格式数据定义，小数转换成Q格式***/
#define _Q30(A)       (uint32_t)(A*1073741824.0L)   
#define _Q29(A)       (uint32_t)(A*536870912.0L)
#define _Q28(A)       (uint32_t)(A*268435456.0L)
#define _Q27(A)       (uint32_t)(A*134217728.0L)
#define _Q26(A)       (uint32_t)(A*67108864.0L)
#define _Q25(A)       (uint32_t)(A*33554432.0L)
#define _Q24(A)       (uint32_t)(A*16777216.0L)
#define _Q23(A)       (uint32_t)(A*8388608.0L)
#define _Q22(A)       (uint32_t)(A*4194304.0L)
#define _Q21(A)       (uint32_t)(A*2097152.0L)
#define _Q20(A)       (uint32_t)(A*1048576.0L)
#define _Q19(A)       (uint32_t)(A*524288.0L)
#define _Q18(A)       (uint32_t)(A*262144.0L)
#define _Q17(A)       (uint32_t)(A*131072.0L)
#define _Q16(A)       (uint32_t)(A*65536.0L)
#define _Q15(A)       (uint32_t)(A*32768.0L)
#define _Q14(A)       (uint32_t)(A*16384.0L)
#define _Q13(A)       (uint32_t)(A*8192.0L)
#define _Q12(A)       (uint32_t)(A*4096.0L)
#define _Q11(A)       (uint32_t)(A*2048.0L)
#define _Q10(A)       (uint32_t)(A*1024.0L)
#define _Q9(A)        (uint32_t)(A*512.0L)
#define _Q8(A)        (uint32_t)(A*256.0L)
#define _Q7(A)        (uint32_t)(A*128.0L)
#define _Q6(A)        (uint32_t)(A*64.0L)
#define _Q5(A)        (uint32_t)(A*32.0L)
#define _Q4(A)        (uint32_t)(A*16.0L)
#define _Q3(A)        (uint32_t)(A*8.0L)
#define _Q2(A)        (uint32_t)(A*4.0L)
#define _Q1(A)        (uint32_t)(A*2.0L)

/*****Q格式降位处理****/
#define Q_shit_uint8(A,B)  (uint8_t)(A>>B)
#define Q_shit_uint16(A,B)  (uint16_t)(A>>B)
#define Q_shit_uint32(A,B)  (uint32_t)(A>>B)

#define Q_shit_int8(A,B)  (int8_t)(A>>B)
#define Q_shit_int16(A,B)  (int16_t)(A>>B)
#define Q_shit_int32(A,B)  (int32_t)(A>>B)
#endif /* Q_MATH_H_ */
