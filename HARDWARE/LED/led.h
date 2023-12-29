#ifndef __LED_H
#define __LED_H
#include "main.h"


//LED¶Ë¿Ú¶¨Òå
#define LED_RED_ON PEout(11)=0	//ºìµÆ
#define LED_RED_OFF PEout(11)=1	//ºìµÆ


#define LED_GREEN_ON PFout(14)=0	//ÂÌµÆ	 
#define LED_GREEN_OFF PFout(14)=1	//ÂÌµÆ	 

#define LED_RED_TOGGLE 		GPIO_ToggleBits(GPIOE,GPIO_Pin_11)	//ºìµÆ
#define LED_GREEN_TOGGLE GPIO_ToggleBits(GPIOF,GPIO_Pin_14)	//ÂÌµÆ	 
 
void LED_Init();//³õÊ¼»¯LEDµÆ			    
void LED_Run(void);

#endif
