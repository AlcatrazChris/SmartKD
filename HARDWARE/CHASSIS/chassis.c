#include "chassis.h"

// PID
typedef struct {
	float Kp, Ki, Kd;
	int lastErr, ErrSum;
} PID_data;

int PID_Calc(PID_data *data, int Err) {
	int result = data -> Kp * Err
			+ data -> Ki * (data->ErrSum += Err)
			+ data -> Kd * (Err - data->lastErr);
	data->lastErr = Err;
	if(data->ErrSum >  100000) data->ErrSum =  100000;
	if(data->ErrSum < -100000) data->ErrSum = -100000;
	
	if(result > 30000) result = 30000;
	if(result <-30000) result =-30000;
	return result;
}

PID_data motor_PID[4];

void Chassis_Init() {
	static float Kp = 20.0, Ki = 0.0, Kd = 40.0;
	motor_PID->Kp = Kp; motor_PID->Ki = Ki; motor_PID->Kd = Kd;
	motor_PID[1] = *motor_PID; motor_PID[2] = *motor_PID; motor_PID[3] = *motor_PID;
}

void Chassis_Stop() {
	for(int i = 0;i < 100;++i) {
      SetChassis(0, 0, 0, 0,0,0,0);
			delay_ms(5);
	}
	CAN_Chassis[0].Target_Current = 0;
	CAN_Chassis[1].Target_Current = 0;
	CAN_Chassis[2].Target_Current = 0;
	CAN_Chassis[3].Target_Current = 0;
	CAN1_TX_Chassis();
}

void SetChassis(double forward, double left, double yaw, \
						int _m1, int _m2, int _m3, int _m4)
{
	int m1 = _m1, m2 = _m2, m3 = _m3, m4 = _m4;
	
	m1 -= forward; m2 -= forward; m3 += forward; m4 += forward;
	m1 += left;    m2 -= left;    m3 += left;    m4 -= left;
	m1 += yaw;     m2 += yaw;     m3 += yaw;     m4 += yaw;
	
	double ratio = 1.0;
	ratio = fmax(ratio, fabs((double)m1 / 10000));
	ratio = fmax(ratio, fabs((double)m2 / 10000));
	ratio = fmax(ratio, fabs((double)m3 / 10000));
	ratio = fmax(ratio, fabs((double)m4 / 10000));
	m1 /= ratio; m2 /= ratio; m3 /= ratio; m4 /= ratio;
	
	CAN_Chassis[0].Target_Current = PID_Calc(motor_PID+0, m1 - (int)CAN_Chassis[0].Current_Speed);
	CAN_Chassis[1].Target_Current = PID_Calc(motor_PID+1, m2 - (int)CAN_Chassis[1].Current_Speed);
	CAN_Chassis[2].Target_Current = PID_Calc(motor_PID+2, m3 - (int)CAN_Chassis[2].Current_Speed);
	CAN_Chassis[3].Target_Current = PID_Calc(motor_PID+3, m4 - (int)CAN_Chassis[3].Current_Speed);
	CAN1_TX_Chassis();
}
