
#include "UserDefine.h" 


/*********************************************************************************************************
** 函数名称: 状态机函数
** 功能描述: 切换电机的运行状态
**
** 作  者:   YZZ
** 日  期:  2017/03/09
**********************************************************************************************************/
void State_Function(void)
{
//    Get_Mortor_state(); //获取电机状态
     Get_POT();
    switch(	Motor_state)
    {
      
      /*case MFault:                                                          //故障状态
      {
        if(!System_State_Flags.MOTOR_Fault_F)
        {
           if(System_State_Flags.UART_REF_F )
           {
             Motor_state = MFault;
             System_State_Flags.START_CORRECT_F=0;
           }
           if(!System_State_Flags.UART_REF_F )  //
           {
             Motor_state = MStop;
            System_State_Flags.START_CORRECT_F=0;
           }
        }
        else
        {
//          Motor_Fault();
          Motor_state = MFault;
//          System_State_Flags.START_CORRECT_F = 0;
        }
        
      }
      break;
*/
      case MStop:                                                     //停机状态
      {
        #if !REF_MODE
        if(System_State_Flags.UART_REF_F)
        {
           Motor_state = MBLDC_Start;
           break;
        }
        #elif REF_MODE
        if(!System_State_Flags.STOP_F)
        {
           Motor_state = MBLDC_Start;
           break;
        } 
        #endif
        else
        {
          NVIC_DisableIRQ(CCU80_0_IRQn);  //关掉处理HALL_FOC中断
          Motor_state = MStop;
          Mortor_Stop();   //停机
        }               
      }
      break;
      case MBLDC_Start:                                         //方波启动
      {
        NVIC_EnableIRQ(CCU80_0_IRQn);  //开HALL_FOC中断处理   
        #if !REF_MODE
        if((!System_State_Flags.UART_REF_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;
          System_State_Flags.START_CORRECT_F=0;
          break;
        } 
        #elif REF_MODE
        if((System_State_Flags.STOP_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;

          System_State_Flags.START_CORRECT_F=0;
          break;
        }  
        #endif
        if(!System_State_Flags.HALL_F)
        {
          Motor_state = MPMSM_Run;   //切换到正弦波控制
        }
      }
      break;
      case MPMSM_Run:                                           //正弦波运行
      {

        #if !REF_MODE
        if((!System_State_Flags.UART_REF_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;
          System_State_Flags.START_CORRECT_F=0;
          break;
        } 
        #elif REF_MODE
        if((System_State_Flags.STOP_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;

          System_State_Flags.START_CORRECT_F=0;
          break;
        }  
        #endif
        System_State_Flags.CLOSE_LOOP_F	=1;	  //切换到正弦波模式	
      }
      break;
      default:break;
    }
}

/*********************************************************************************************************
** 函数名称: 电机运行状态判断
** 功能描述: 函数实现Motor_state的赋值
**
** 作  者:   YZZ
** 日  期:  2017/03/09
**********************************************************************************************************/
void Get_Mortor_state(void)
{

   if(System_State_Flags.OVERVOLTAGE_F||System_State_Flags.UNDERVOLTAGE_F)  //故障判断
  {
   System_State_Flags.MOTOR_Fault_F = 1;
   Motor_state=MFault;		
  }
  else
  {
    System_State_Flags.MOTOR_Fault_F= 0;
  }		
   if(POT > 90)
  {
   System_State_Flags.UART_REF_F = 1;//电机开启标志
  }
  else
  {
    System_State_Flags.UART_REF_F  = 0;//停机
  }
}
/*********************************************************************************************************
** 函数名称: 电机停机函数
** 功能描述: 函数实现停机
**
** 作  者:   YZZ
** 日  期:  2017/03/09
**********************************************************************************************************/
void Mortor_Stop(void)
{
  POSIF0->MCSM	=	0x0000;							//关多通道
  POSIF0->MCMS	|=	0x01;							//shadow transfer
  
  Refresh_CCU80_PWM_Dutycycle(2000,2000,2000);//锁定电机
  
//切换PWM模式
  CCU80_CC82->CHC=0x00000004;//对称PWM，非互补输出
  CCU80_CC81->CHC=0x00000004;//对称PWM，非互补输出
  CCU80_CC80->CHC=0x00000004;//对称PWM，非互补输出
  System_Flags_Reset();//系统状态复位
}
/*********************************************************************************************************
** 函数名称: 标志位初始化函数
** 功能描述: 函数实现标志位初始化
**
** 作  者:   YZZ
** 日  期:  2017/03/09
**********************************************************************************************************/
void System_Flags_Init(void)
{
	System_State_Flags.START_F=1;
	System_State_Flags.LOCK_F=0;
	System_State_Flags.HALL_F=1;  //霍尔模式
  System_State_Flags.FIRST_F = 1; //第一次开启
  System_State_Flags.INIT_BRIDGE_F = 0; //下桥初始化
  System_State_Flags.INIT_BRIDGE_ONE_F = 1; //下桥初始化
  Hall1.Startcorrect = 0;
}
/*********************************************************************************************************
** 函数名称: 标志位复位函数
** 功能描述: 函数实现标志位复位
**
** 作  者:   YZZ
** 日  期:  2017/04/10
**********************************************************************************************************/
void System_Flags_Reset(void)
{
  System_State_Flags.START_F=1; //启动标志
	System_State_Flags.LOCK_F=0;
	System_State_Flags.HALL_F=1;  //霍尔模式
  System_State_Flags.FIRST_F = 1; //第一次开启
  System_State_Flags.CLOSE_LOOP_F = 0;
  Hall1.Startcorrect = 0;  
  System_State_Flags.INIT_BRIDGE_F = 0;     //关桥初始化标志位
  System_State_Flags.INIT_BRIDGE_ONE_F = 1; //开启IPM桥初始化第一次
  LED_OFF; //停机关指示灯
  Hall1.Pattpre = 0;
}

/*********************************************************************************************************
** 函数名称: 延迟函数
** 功能描述: 
**
** 作  者:  LuJun 
** 日  期: 
**********************************************************************************************************/

void delayTimer(uint32_t ms) 
{                            
   uint32_t x,y; 
   for(x = ms;x > 0; x--)
   	for(y = 64000 ; y > 0; y--);                                  //ms      	
}

/*********************************************************************************************************
** 函数名称: PWM周期中断处理
** 功能描述: 处理中断中HALL_FOC控制
**
** 作  者:  BLL
** 日  期:  2017/03/22
**********************************************************************************************************/

//注意这里函数名字改过了
void PWM_One_Match_INTER()           //CCU8SR0
{

    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {

    	if(System_State_Flags.START_F )
    	{
				if(!System_State_Flags.STOP_F)    /*电机开启*/
        {
          if(System_State_Flags.FIRST_F)    //停机后再开启多通道
					{
					  	POSIF0->MCSM	 =	0x0333;						//多通道状态 全开
              POSIF0->MCMS	|=	0x01;							//shadow transfer
						  System_State_Flags.FIRST_F = 0;
					}
//				  Get_Current();                  //读取A、B相电流值
				
				  Foc16.Idref_in 		= 	0;        //采用Id=0控制
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //开环Iq给定，速度闭环速度给定
					}
//						 
//					HALL_FOC_RUN();//HALL_FOC模式运行
				}
        else
        {
          Mortor_Stop();
        }
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //清除one match中断状态标志位
    }
}
/*********************************************************************************************************
** 函数名称: UART给定
** 功能描述: 
**
** 作  者:  BLL
** 日  期:  2017/03/24
**********************************************************************************************************/

int16_t Get_UART_Val(void)
{
  int16_t UART_VAL = 0;
  Recieved_UART_Data = U0C0_UART_vReadData();
  UART_VAL = Recieved_UART_Data << 4; // 0-255 to 0-4095
  if (UART_VAL <= Ref_limit_min )
  {
    Stop_Cnt++;
    if(Stop_Cnt >=100)
    {
      System_State_Flags.STOP_F = 1;
      UART_VAL = 0;
    }            
  }
  else
  {
     System_State_Flags.STOP_F = 0;
     Stop_Cnt = 0;//停机变量清零
  }
    return ((int16_t)UART_VAL);
}
/*********************************************************************************************************
** 函数名称: 下桥初始化
** 功能描述: 下桥通电处理
**
** 作  者:  BLL
** 日  期:  2017/03/24
**********************************************************************************************************/
void Bridge_Init(void)
{
  if(System_State_Flags.INIT_BRIDGE_ONE_F)
  {
     P0_1_set_mode(OUTPUT_PP_GP);
     P0_3_set_mode(OUTPUT_PP_GP);
     P0_9_set_mode(OUTPUT_PP_GP);   
     System_State_Flags.INIT_BRIDGE_F =1;
    while(System_State_Flags.INIT_BRIDGE_F);
    if(!System_State_Flags.INIT_BRIDGE_F)
    {
      P0_1_set_mode(OUTPUT_PP_AF5);    //U_L    
      P0_3_set_mode(OUTPUT_PP_AF7);  //V_L
      P0_9_set_mode(OUTPUT_PP_AF5);    //W_L
    }
  System_State_Flags.INIT_BRIDGE_ONE_F = 0;            
  }
 
}
void Refresh_Duty(uint16_t PWM_U,uint16_t PWM_V,uint16_t PWM_W)//更新霍尔占空比
{
  Hall1.PWM_U = PWM_U;
  Hall1.PWM_V = PWM_V;
  Hall1.PWM_W = PWM_W;
}

