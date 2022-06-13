/*****************************************************************************
*    Filename:     FOC.h
*    IDE:          DAVE_3.1.2
*    MCU:          XMC45OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2012-9-7    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/
#ifndef FOC_H_
#define FOC_H_

/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
//#include "arm_math.h"
#include "Parameters.h"


#include "Table.h"
#include "PIDCtrl.h"
#include "SVPWM.h"
/*****************************************************************************
*     Macro Definitions
*****************************************************************************/
#define		SPD_CNT					32										//速度环计数周期上限 2ms
#define   Speed_CNT       32                    //转速计算周期 2ms
#define		Sensorless				0										// 0  sensored , 1 sensorless
#define		Operation_Mode			1
/////////////////////////////////////////////////
//		Mode 1:			Speed loop + current loop
//		Mode 2:			Current loop only
//		Mode 3:			Voltage open loop
///////////////////////////////////////////////


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/





/*****************************************************************************
*     Static(Local) Variable Definitions
*****************************************************************************/
extern int16_t	Speedloop_count;											//速度环计数

typedef struct {
		int16_t	Iu_in;
		int16_t	Iv_in;
		int16_t	Iw_in;
		int16_t	Iu_in_old;
		int16_t	Iv_in_old;
		int16_t	Iw_in_old;	
	
		int16_t	Instruction_in;
    int16_t Idref_in;
    int16_t Iqref;
    int16_t Spd_in;
    uint16_t Angle_in;
    int16_t Id;
    int16_t Iq;
    int16_t Id_Bak;
    int16_t Iq_Bak;	
    int16_t Vd;
    int16_t Vq;
    int16_t Id_PU;
    int16_t Iq_PU;
    int16_t Vd_PU;
    int16_t Vq_PU;	
    int16_t Vq_Fil;
    int16_t Ialpha;
    int16_t Ibeta;
    int16_t Valpha;
    int16_t Vbeta;
    int16_t Falpha;
    int16_t Fbeta;
    int16_t	Fluxm;
    int16_t	PWM_Tu_out;
    int16_t	PWM_Tv_out;
    int16_t	PWM_Tw_out;
} TFOC16;



typedef struct {
    int16_t Cos;
    int16_t Sin;
    uint16_t	Pointer;
} TTriAng16;

typedef struct {
	  uint32_t ElecTheta;  //0 Output: Motor Electrical angle (Q15)
		uint16_t MechTheta;   //0 Output: Motor Mechanical Angle (Q15) 
		uint16_t DirectionQep; //0 Output: Motor rotation direction (Q0)
		uint32_t QepCountIndex; //0 Variable: 计数脉冲值 (Q0) 
		uint16_t LineEncoder;  //0 Parameter: Number of line encoder (Q0) 
		uint16_t PolePairs;   //4 Parameter: Number of pole pairs (Q0) 
		int16_t  CalibratedCount;//校准角对应的脉冲数                   
} QEP;


typedef struct {
       uint32_t ElecTheta;  		// Input: Electrical angle (pu) 
	     int32_t  DeltaTheta;
       uint16_t DirectionQep;//Variable: Direction of rotation (Q0) - independently with global Q
       uint32_t OldElecTheta;   	// History: Electrical angle at previous step (pu)
       uint32_t BaseRpm;//Parameter: Base speed in rpm (Q0) - independently with global Q
       uint32_t SpeedRpm; // Output : Speed in rpm  (Q0) - independently with global Q
} SPEED_QEP;   	// Data type created


typedef struct {
	  uint8_t  Patt;  //霍尔信号输入
		uint8_t  Pattpre;   //0 Output: Motor Mechanical Angle (Q15) 
		uint8_t  Direction; //0 Output: Motor rotation direction (Q0)
	  uint8_t  Startcorrect;  //霍尔启动
	  uint16_t Startcount;    //霍尔启动计数
	  uint16_t StartNum;      //霍尔启动成功所需次数
	  uint16_t Startpwmcount; 
	  uint16_t Startpwmtime;
	  uint16_t Startpwm;
	  uint16_t StartpwmMax;
		uint32_t KCount;      //0 Variable: 定时器计数值
		uint32_t Numencoder;   //0 Parameter: Number of line encoder (Q0) 
    uint32_t Huanxiangtime;
	  uint32_t Thetachushi;
	  int32_t  Thetaoft;
	  uint32_t ElecTheta;
    int32_t Compen_Theta;  //加速度补偿角
    uint16_t PWM_U;//U相PWM
    uint16_t PWM_V;//V相PWM
    uint16_t PWM_W;//W相PWM
    uint16_t PWM_Duty;//占空比
    int32_t Parameter_a;
    int32_t Parameter_b;
    uint32_t Huanxiangtime_Array[6];
    uint8_t  Huanxiangtime_cnt;
    uint32_t Huangxiangtime_sum;
    uint8_t  Speed_Cnt;
	  uint16_t SpeedBufer[6];
	  uint16_t Speedrpm;
    uint32_t SecorTime_Bufer[3];//记录三次扇区时间
    uint8_t  SectorTime_cnt;
} HALL;




//--------------U V W axis Components--------------
typedef struct {
    int16_t  U;
    int16_t  V;
    int16_t  W;
    int16_t  DC;
    uint16_t U_Oft;
    uint16_t V_Oft;
    uint16_t W_Oft;
    uint16_t DC_Oft;
} TPhase16;


//--------------Alpha, Beta axis Components--------------
typedef struct {
    int16_t Alpha;
    int16_t Beta;
} TComplex16;


//--------------D,Q Axis Components--------------
typedef struct {
    int16_t D;
    int16_t Q;
} TDQ16;

/*****************************************************************************
*     Local Function Declarations
*****************************************************************************/


/*****************************************************************************
*     Global Variable Definitions
*****************************************************************************/
extern int16_t Speedloop_count;                                          //速度环计数


extern TFOC16 	Foc16;

extern TPID16 PID16_SPD,PID16_ID,PID16_IQ;

extern TPhase16    Phase_Current_16,Phase_PDC_16;

extern TTriAng16   SinTab16;

extern QEP Qep1;

extern SPEED_QEP Speed1;

extern HALL Hall1;


/*****************************************************************************
 *    Global Function Definitions
 *****************************************************************************/
void FOC_Init(void);
void FOC16_Calc(uint8_t CtrlMode);
void Hall_Angle_Cal(HALL *p);
void QEP_Speed_Calc(SPEED_QEP *v);
uint16_t Hall_Speed_Calc(uint16_t *SpeedCal);

/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/

/*****************************************************************************
*     Constant table
*****************************************************************************/

#endif /* FOC_H_ */
