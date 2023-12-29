#ifndef __GRAY_H
#define __GRAY_H

#include "main.h"

#define GRAY_BACK_L_Pin  GPIO_Pin_11 // H
#define GRAY_BACK_M_Pin  GPIO_Pin_12 // H
#define GRAY_BACK_R_Pin  GPIO_Pin_0  // I

#define GRAY_RIGHT_L_Pin GPIO_Pin_14 // D
#define GRAY_RIGHT_M_Pin GPIO_Pin_15 // D
#define GRAY_RIGHT_R_Pin GPIO_Pin_10 // H

#define GRAY_FRONT_L_Pin GPIO_Pin_5  // I
#define GRAY_FRONT_M_Pin GPIO_Pin_3  // A
#define GRAY_FRONT_R_Pin GPIO_Pin_2  // A

#define GRAY_LEFT_L_Pin  GPIO_Pin_2  // I
#define GRAY_LEFT_M_Pin  GPIO_Pin_7  // I
#define GRAY_LEFT_R_Pin  GPIO_Pin_6  // I


void Gray_Init();
bool isUp(bool nowState, int id);

#define GRAY_FRONT_L isUp(GPIO_ReadInputDataBit(GPIOI, GRAY_FRONT_L_Pin), 0)
#define GRAY_FRONT_M isUp(GPIO_ReadInputDataBit(GPIOA, GRAY_FRONT_M_Pin), 1)
#define GRAY_FRONT_R isUp(GPIO_ReadInputDataBit(GPIOA, GRAY_FRONT_R_Pin), 2)

#define GRAY_BACK_L  isUp(GPIO_ReadInputDataBit(GPIOH, GRAY_BACK_L_Pin),  3)
#define GRAY_BACK_M  isUp(GPIO_ReadInputDataBit(GPIOH, GRAY_BACK_M_Pin),  4)
#define GRAY_BACK_R  isUp(GPIO_ReadInputDataBit(GPIOI, GRAY_BACK_R_Pin),  5)

#define GRAY_LEFT_L  isUp(GPIO_ReadInputDataBit(GPIOI, GRAY_LEFT_L_Pin),  6)
#define GRAY_LEFT_M  isUp(GPIO_ReadInputDataBit(GPIOI, GRAY_LEFT_M_Pin),  7)
#define GRAY_LEFT_R  isUp(GPIO_ReadInputDataBit(GPIOI, GRAY_LEFT_R_Pin),  8)

#define GRAY_RIGHT_L isUp(GPIO_ReadInputDataBit(GPIOD, GRAY_RIGHT_L_Pin), 9)
#define GRAY_RIGHT_M isUp(GPIO_ReadInputDataBit(GPIOD, GRAY_RIGHT_M_Pin), 10)
#define GRAY_RIGHT_R isUp(GPIO_ReadInputDataBit(GPIOH, GRAY_RIGHT_R_Pin), 11)

#define GRAY_NUM   			 12
#define GRAY_ANTI_SHAKE  2

#endif
