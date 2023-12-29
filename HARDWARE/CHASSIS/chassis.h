#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "main.h"

void Chassis_Init();
void Chassis_Stop();
void SetChassis(double forward, double left, double yaw, int _m1, int _m2, int _m3, int _m4);

#endif