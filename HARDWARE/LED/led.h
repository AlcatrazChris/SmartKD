#ifndef __LED_H
#define __LED_H
#include "main.h"


//LED�˿ڶ���
#define LED_RED_ON PEout(11)=0	//���
#define LED_RED_OFF PEout(11)=1	//���


#define LED_GREEN_ON PFout(14)=0	//�̵�	 
#define LED_GREEN_OFF PFout(14)=1	//�̵�	 

#define LED_RED_TOGGLE 		GPIO_ToggleBits(GPIOE,GPIO_Pin_11)	//���
#define LED_GREEN_TOGGLE GPIO_ToggleBits(GPIOF,GPIO_Pin_14)	//�̵�	 
 
void LED_Init();//��ʼ��LED��			    
void LED_Run(void);

#endif
