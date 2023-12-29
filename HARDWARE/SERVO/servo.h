#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

void Servo_Init();

#define CATCH_OFF TIM_SetCompare2(TIM2,1970)
#define CATCH_ON  TIM_SetCompare2(TIM2,2480)
#define CATCH_IN  TIM_SetCompare2(TIM2,2300)

#endif
