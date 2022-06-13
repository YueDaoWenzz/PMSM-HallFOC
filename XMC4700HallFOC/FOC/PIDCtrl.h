/*****************************************************************************
*    Filename:     PIDCtrl.h
*    IDE:          DAVE_3.1.2
*    MCU:          XMC45OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2013-1-9    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/
#ifndef PIDCTRL_H_
#define PIDCTRL_H_

/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>


/*****************************************************************************
*     Macro Definitions
*****************************************************************************/


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/
typedef struct {
    int16_t Input_in;
    int16_t Feedback_in;
    int16_t Error;
    int16_t Error_old;
    int32_t U;
		int32_t	KP_Value;
    int32_t Sum;
    int16_t Exceed;
    int16_t Kp;
    int16_t Ki;
    int16_t Kd;
    int16_t Kc;
    int16_t Max;
    int16_t Min;
    int16_t Output_out;
} TPID16;

typedef struct {
    int32_t Input_in;
    int32_t Feedback_in;
    int32_t Error;
    int32_t Error_old;
    int64_t U;
    int64_t Sum;
    int32_t Exceed;
    int32_t Kp;                             //8.24 format
    int32_t Ki;
    int32_t Kd;
    int32_t Kc;
    int32_t Max;
    int32_t Min;
    int32_t Output_out;
} TPID32;

typedef struct {
    int32_t Kp;                             //8.24 format
    int32_t Ki;
    int32_t Kd;
    int32_t Kc;
    int32_t Max;
    int32_t Min;
} TPID32_Para;


typedef TPID32 *PIDCTRL_32;

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
void PID32_Init(TPID32 *p, TPID32_Para *v);
void PID32_Reset(TPID32 *p);
void PID32_Calc(TPID32 *p);
void PID16_Calc(TPID16 *p);
void Initialize_PID_parameters(void);
void PID16_Calc_Alt(TPID16 *p);
/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/


#endif /* PIDCTRL_H_ */
