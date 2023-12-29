#include "infrared.h"

void Infrared_Init() {
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

bool inf_isUp(bool nowState, int id) {
	static bool isUPed[INF_NUM];
	static int cnt[INF_NUM];

	if(nowState == isUPed[id]) {
		if(cnt[id]) cnt[id] = 0;
	} else {
		if(++cnt[id] > INF_ANTI_SHAKE) {
			cnt[id] = 0;
			isUPed[id] = nowState;
		}
	}
	return isUPed[id];
}
