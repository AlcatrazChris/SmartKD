#include "trace.h"



bool Arrive(int dirc) {
		if(dirc == DIRC_FORWARD || dirc == DIRC_BACK)
				return GRAY_LEFT_M || GRAY_RIGHT_M;
		else return GRAY_FRONT_M || GRAY_BACK_M;
}

void Drive(int dirc, int speed) {
		int forward = 0, left = 0;
		int m1 = 0, m2 = 0, m3 = 0, m4 = 0;

		switch(dirc) { // Basic Speed
		case DIRC_FORWARD: forward =  speed; break; // forward
		case DIRC_BACK:    forward = -speed; break; // back
		case DIRC_LEFT:    left    =  speed; break; // left
		case DIRC_RIGHT:   left    = -speed; break; // right
		default: break;
		}

		switch(dirc) {
		case DIRC_FORWARD: case DIRC_BACK: // back forward
				if(GRAY_FRONT_L) { m2 -= TRACE_BACK_FORCE; m4 -= TRACE_BACK_FORCE; }
				if(GRAY_FRONT_R) { m2 += TRACE_BACK_FORCE; m4 += TRACE_BACK_FORCE; }
				
				if(GRAY_BACK_L)  { m1 -= TRACE_BACK_FORCE; m3 -= TRACE_BACK_FORCE; }
				if(GRAY_BACK_R)  { m1 += TRACE_BACK_FORCE; m3 += TRACE_BACK_FORCE; }
				break;
		case DIRC_RIGHT: case DIRC_LEFT: // left right
				if(GRAY_LEFT_L)  { m3 -= TRACE_BACK_FORCE + 500; m4 -= TRACE_BACK_FORCE + 500; }
				if(GRAY_LEFT_R)  { m3 += TRACE_BACK_FORCE; m4 += TRACE_BACK_FORCE; }
				
				if(GRAY_RIGHT_L) { m1 -= TRACE_BACK_FORCE; m2 -= TRACE_BACK_FORCE; }
				if(GRAY_RIGHT_R) { m1 += TRACE_BACK_FORCE + 500; m2 += TRACE_BACK_FORCE + 500; }
				break;
		default: break;
		}

		SetChassis(forward, left, 0, m1, m2, m3, m4);
}

void antiF(int dirc,int distance, int dx, int dy, int dis, int antidx, int antidy) {
	switch(dirc) {
		case DIRC_FORWARD:
			if(dx == 0 && dy == MAP_WIDTH - 1 && dis == 1) return;
			if(dx == MAP_LENGTH - 1 && dy == MAP_WIDTH - 1 && dis == 1) return;
			if(INFRARED_FRONT)	while(1)  	
				{Chassis_Stop();
					
//					if()update(dx,dy);
//					findWay(dx,dy,antidx,antidy,&dirc,&distance);
//					getNext(dx, dy, &dx, &dy, dirc, distance);
					delay_ms(500);
					antiF(DIRC_FORWARD,distance, dx, dy, dis,antidx,antidy);
					//Move(8000, dirc, distance, dx, dy,antidx,antidy);
			break;}
		case DIRC_BACK:
			if(dis == 1 || dirc != DIRC_BACK) return;
			if(INFRARED_BACK) while(1) 	
				{Chassis_Stop();
//					update(dx,dy);
//					findWay(dx,dy,antidx,antidy,&dirc,&distance);
//					getNext(dx, dy, &dx, &dy, dirc, distance);
					
					delay_ms(500);
					antiF(DIRC_BACK,distance, dx, dy, dis,antidx,antidy);
					//Move(8000, dirc, distance, dx, dy,antidx,antidy);
			break;}
		case DIRC_LEFT:
			if(  dis == 1|| dirc != DIRC_LEFT) return;
			if(INFRARED_LEFT) while(1) 	
				{Chassis_Stop();
//					update(dx,dy);
//					findWay(dx,dy,antidx,antidy,&dirc,&distance);
//					getNext(dx, dy, &dx, &dy, dirc, distance);
					
					delay_ms(500);
					antiF(DIRC_LEFT,distance, dx, dy, dis,antidx,antidy);
					//Move(8000, dirc, distance, dx, dy,antidx,antidy);
			break;}
		case DIRC_RIGHT:
			if( dis == 1|| dirc != DIRC_RIGHT) return;
			if(INFRARED_RIGHT) while(1) 		
				{Chassis_Stop();
//					update(dx,dy);
//					findWay(dx,dy,antidx,antidy,&dirc,&distance);
//					getNext(dx, dy, &dx, &dy, dirc, distance);
					delay_ms(500);
					antiF(DIRC_RIGHT,distance, dx, dy, dis,antidx,antidy);
					//Move(8000, dirc, distance, dx, dy,antidx,antidy);
			break;}
		default: break;
	}
}

void Move(int speed, int dirc, int distance, int dx, int dy, int antidx ,int antidy) {
		static int passTime;
		int nowSpeed = 2000, nowDistance = 0;
	
		antiF(dirc,distance, dx, dy, distance - nowDistance,antidx,antidy);
		
		passTime = 0;
		while(Arrive(dirc) || passTime < 150) {
				++passTime;
				Drive(dirc, nowSpeed);
				delay_ms(5);
		}
		
		while(1) {
				if(Arrive(dirc)) {
						if(++nowDistance == distance) {
								Chassis_Stop();
								break;
						}
						
						antiF(dirc,distance, dx, dy, distance - nowDistance,antidx,antidy);
						
						if(!nowDistance) nowSpeed = 3750;
						else if(nowDistance == distance - 1)
								nowSpeed = 3750;
						else nowSpeed = speed;
						
						passTime = 0;
						while(Arrive(dirc) || passTime < 100) {
								++passTime;
								Drive(dirc, nowSpeed);
								delay_ms(5);
						}
				} else {
					if(!nowDistance) nowSpeed = 3750;
					else if(nowDistance == distance - 1)
							nowSpeed = 3750;
					else nowSpeed = speed;
					
					Drive(dirc, nowSpeed);
				}
		}
}
