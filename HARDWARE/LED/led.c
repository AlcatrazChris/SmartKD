#include "led.h" 


/*
*@Description��LED��ʼ��
*@param 1��	  ����1
*@param 2��	  ����2
*@return:��	  ����ֵ
*/
void LED_Init()
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);//LED_G 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);//LED_R
	
	//���
	GPIO_ResetBits(GPIOF,GPIO_Pin_14);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);

}


void LED_Run(void)
{
	GPIO_ToggleBits(GPIOF,GPIO_Pin_14);
	GPIO_ToggleBits(GPIOE,GPIO_Pin_11);	
}
