#ifndef __INFRARED_H
#define __INFRARED_H

#include "main.h"

#define INFRARED_FRONT_Pin GPIO_Pin_0  // A
#define INFRARED_BACK_Pin  GPIO_Pin_13 // D
#define INFRARED_LEFT_Pin  GPIO_Pin_1  // B
#define INFRARED_RIGHT_Pin GPIO_Pin_12 // D

void Infrared_Init();
bool inf_isUp(bool nowState, int id);

#define INFRARED_FRONT inf_isUp(!GPIO_ReadInputDataBit(GPIOA, INFRARED_FRONT_Pin), 0)
#define INFRARED_BACK  inf_isUp(!GPIO_ReadInputDataBit(GPIOD, INFRARED_BACK_Pin),  1)
#define INFRARED_LEFT  inf_isUp(!GPIO_ReadInputDataBit(GPIOB, INFRARED_LEFT_Pin),  2)
#define INFRARED_RIGHT inf_isUp(!GPIO_ReadInputDataBit(GPIOD, INFRARED_RIGHT_Pin), 3)

#define INF_NUM        4
#define INF_ANTI_SHAKE 0

#endif
