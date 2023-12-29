#ifndef __CAN_HANDLE_H
#define __CAN_HANDLE_H
#include "main.h"


typedef struct	
{
	short Current_Speed;
	short Current_MechAngle;
	short Current;
	short Temperature;
	short Critical_MechAngle;
	short Origin_MechAngle;
	short Target_Current;
	short Target_MechAngle;
	short Last_MechAngle;
	int Total_MechAngle;
	int Init_Flag;
	int Total_Num;
	int Init_Angle;
	
}CAN_Data_TypeDef;

//201-204
extern CAN_Data_TypeDef CAN_Chassis[4];//底盘

extern int CAN1_Signal;

void CAN1_TX_Chassis(void);

#endif