#include "can_handle.h"

// CAN1 201-204
CAN_Data_TypeDef CAN_Chassis[4];//底盘

unsigned char CAN1_Tx_Message_Flag = 0;									//CAN1发送消息标志
unsigned char CAN2_Tx_Message_Flag = 0;									//CAN2发送消息标志

int CAN1_Signal = 0;																		//CAN1信号量标志
int CAN2_Signal = 0;																		//CAN2信号量标志

/*
*@title：CAN1发送中断函数
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void CAN1_TX_IRQHandler(void)
{  
  if (CAN_GetITStatus (CAN1,CAN_IT_TME)!=RESET)                             
	{
		CAN1_Tx_Message_Flag=1;
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);    
	}
}


/*
*@title：CAN2发送中断函数
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void CAN2_TX_IRQHandler(void)
{  
  if (CAN_GetITStatus (CAN2,CAN_IT_TME)!=RESET)                            
	{
		CAN2_Tx_Message_Flag=1;
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);    
	}
}

/*
*@Description：CAN数据解码
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
void CAN_Data_Decode(CAN_Data_TypeDef *CAN_Data,CanRxMsg *CAN_Rx_Message)
{
	CAN_Data->Origin_MechAngle = (CAN_Rx_Message->Data[0]<<8)|(CAN_Rx_Message->Data[1]);
	CAN_Data->Current_Speed 					= (CAN_Rx_Message->Data[2]<<8)|(CAN_Rx_Message->Data[3]);
	CAN_Data->Current								  = (CAN_Rx_Message->Data[4]<<8)|(CAN_Rx_Message->Data[5]);
	CAN_Data->Temperature					    = (CAN_Rx_Message->Data[6]);
	
	//当前数据为云台电机使用，处理角度数据
	if(CAN_Data->Critical_MechAngle !=0)
	{
		if(CAN_Data->Origin_MechAngle < CAN_Data->Critical_MechAngle)
		{
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle + 8192;
		}
		else
		{
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle;
		}		
	}
	//普通电机使用
	else
	{
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle;
	}
}
/*
*@title：CAN1接收中断函数
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void CAN1_RX0_IRQHandler(void)                                             
{   
	CanRxMsg CAN1_Rx_Message;	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_Rx_Message);
		if ( (CAN1_Rx_Message.IDE == CAN_Id_Standard) && (CAN1_Rx_Message.RTR == CAN_RTR_Data) && (CAN1_Rx_Message.DLC == 8) )//标准帧、数据帧、数据长度为8字节
		{
			//CAN1信号量填充
			CAN1_Signal = 50;
			switch (CAN1_Rx_Message.StdId)
			{
				case 0x201:
				case 0x202:
				case 0x203:
				case 0x204:
				{
					CAN_Data_Decode(&CAN_Chassis[CAN1_Rx_Message.StdId - 0x201],&CAN1_Rx_Message);
				}break;
				
				default:
				{
					break;
				}   
			}
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
	}
}

/*
*@title：CAN2接收中断函数
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void CAN2_RX0_IRQHandler(void)                                                 
{   
	CanRxMsg CAN2_Rx_Message;	
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN2,CAN_FIFO0,&CAN2_Rx_Message);

		if ( (CAN2_Rx_Message.IDE == CAN_Id_Standard) && (CAN2_Rx_Message.RTR == CAN_RTR_Data) && (CAN2_Rx_Message.DLC == 8) )//标准帧、数据帧、数据长度为8字节
		{
			//CAN2信号量填充
			CAN2_Signal = 50;
			switch (CAN2_Rx_Message.StdId)
			{
				
				default:
				{
					break; 
				}
			}
		}
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		
	}
}

/*
*@title：CAN1底盘数据发送
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
void CAN1_TX_Chassis(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //标准帧
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //数据帧
	CAN1_Tx_Message.DLC = 0x08;                                                     //帧长度为8
	CAN1_Tx_Message.StdId = 0x200;                               					//帧ID为传入参数的CAN_ID

	CAN1_Tx_Message.Data[0] = (CAN_Chassis[0].Target_Current>>8)&0xff;             //201接收电流高8位
	CAN1_Tx_Message.Data[1] = (CAN_Chassis[0].Target_Current)&0xff;                 //201接收电流低8位
	CAN1_Tx_Message.Data[2] = (CAN_Chassis[1].Target_Current>>8)&0xff;             //202接收电流高8位
	CAN1_Tx_Message.Data[3] = (CAN_Chassis[1].Target_Current)&0xff;                //202接收电流低8位
	CAN1_Tx_Message.Data[4] = (CAN_Chassis[2].Target_Current>>8)&0xff;             //203接收电流高8位
	CAN1_Tx_Message.Data[5] = (CAN_Chassis[2].Target_Current)&0xff;                 //203接收电流低8位
	CAN1_Tx_Message.Data[6] = (CAN_Chassis[3].Target_Current>>8)&0xff;             //204接收电流高8位
	CAN1_Tx_Message.Data[7] = (CAN_Chassis[3].Target_Current)&0xff;                 //204接收电流低8位

	CAN1_Tx_Message_Flag = 0;
	
	//CAN1掉线处理
	if(CAN1_Signal > 0)
	{
	CAN_Transmit(CAN1,&CAN1_Tx_Message);	
	}
	
	while(CAN1_Tx_Message_Flag == 0)
	{
		if(CAN1_Signal <= 0)
		{
			break;
		}
	}
}
