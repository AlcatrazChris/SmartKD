#include "main.h"
#define USE_DR16 0

void BSP_Init() {
	Power_Init();
	LED_Init();
#if USE_DR16 == 1
	DR16_Init();
#endif
	CAN12_Init();
	Beep_Init();
	Chassis_Init();
	Gray_Init();
	Infrared_Init();
	Astar_Init();
	Servo_Init();
}

void Mib_Car();

int main(void) { 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	BSP_Init();
	//infrared switch
	if (INFRARED_BACK) 
	{delay_ms(50);
		if (INFRARED_BACK) Mib_Car();}
}

void Mib_Car() {
	Chassis_Stop();

#if USE_DR16 == 1
	while(1) {
		if(DBUS.RC.Switch_Left == RC_SW_DOWN
			&& DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			CAN_Chassis[0].Target_Current = 0;
			CAN_Chassis[1].Target_Current = 0;
			CAN_Chassis[2].Target_Current = 0;
			CAN_Chassis[3].Target_Current = 0;
			CAN1_TX_Chassis();
		} else if (DBUS.RC.Switch_Right != RC_SW_UP) {
			double forward = (int)DBUS.RC.ch1 * 10;
			double left    =-(int)DBUS.RC.ch0 * 10;
			double yaw     = (int)DBUS.RC.ch2 * 10;
			
			SetChassis(forward, left, yaw,0,0,0,0);
		} else {
#endif
			for(int i = 0;i < 10;++i) GRAY_LEFT_M;
			for(int i = 0;i < 3000;++i) {
				SetChassis(0, 0, 0,0,0,0,0);
				delay_ms(5);
			}
			static int startx, starty, goalx, goaly;
			if(GRAY_LEFT_M) {
				startx = MAP_LENGTH - 1; starty = 0;
				goalx  = 0;              goaly  = MAP_WIDTH - 1;
			} else {
				startx = 0;              starty = 0;
				goalx  = MAP_LENGTH - 1; goaly = MAP_WIDTH - 1;
			}
			int nx = startx, ny = starty, dx = startx, dy = starty, dirc = 0, distance;
#if USE_DR16 == 0
	while(1) {
#else
			while(DBUS.RC.Switch_Right == RC_SW_UP) {
#endif
			//*/
				if(nx == dx && ny == dy) {
					if(dx == startx && dy == starty) {
						// come to start point
						Chassis_Stop();
						delay_ms(500);
						CATCH_ON;
						//delay_ms(500);
						
						dx = goalx; dy = goaly;
					} else { 
						// arrive the goal
						for(int i = 0;i < 700;++i) {
							SetChassis( 1000,0,0, 0,0,0,0);
							delay_ms(5);
						}
						Chassis_Stop();
						CATCH_OFF;
						for(int i = 0;i < 170 ;++i) {
							SetChassis( 1000,0,0, 0,0,0,0);
							delay_ms(5);
						}
						Chassis_Stop();
						
						delay_ms(2000);
						CATCH_ON;
						delay_ms(2000);
						for(int i = 0;i < 700;++i) {
							SetChassis(-1000,0,0, 0,0,0,0);
							delay_ms(5);
						}
						CATCH_OFF;
						
						dx = startx; dy = starty;
					}
				}
				update(nx, ny);
				findWay(nx, ny, dx, dy, &dirc, &distance);
				getNext(nx, ny, &nx, &ny, dirc, distance);
				if(dx == goalx && dy == goaly)
						 Move(5800, dirc, distance, nx, ny,dx,dy);
				else Move(6800, dirc, distance, nx, ny,dx,dy);
#if USE_DR16 == 1
			}
		}
#endif
		delay_ms(5);
	}
}
	 