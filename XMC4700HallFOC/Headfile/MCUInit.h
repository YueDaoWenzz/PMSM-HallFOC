#ifndef  MCUINIT_H_
#define  MCUINIT_H_

//include headerfiles

#include <XMC4700.h>
#include "GPIO.h"
#include "POSIF.h"
#include "Parameters.h"
#include "CCU8.h"
//ȫ�ֱ���
extern uint32_t timingdelay;

void MCUInit(void);
void SysTick_Handler (void);
void Delay_ms(uint32_t time);//��ʱ����


#endif /* MCUINIT_H_ */
