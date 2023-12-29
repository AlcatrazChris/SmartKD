#include "led.h" 


/*
*@Description：LED初始化
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
void LED_Init()
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);//LED_G 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);//LED_R
	
	//灭灯
	GPIO_ResetBits(GPIOF,GPIO_Pin_14);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);

}


void LED_Run(void)
{
	GPIO_ToggleBits(GPIOF,GPIO_Pin_14);
	GPIO_ToggleBits(GPIOE,GPIO_Pin_11);	
}
