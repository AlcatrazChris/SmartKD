#include "dr16.h"

#define DBUS_RX_BUFF_SIZE 18 //һ֡ң�������ݵĳ���18�ֽ�
//ң�������ݽ��ջ�������
static uint8_t DBUS_rx_buff[2][DBUS_RX_BUFF_SIZE];
DBUS_DecodingData_TypeDef DBUS;

void USART1_Init()
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //GPIOB7����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1��ʼ������
	USART_InitStructure.USART_BaudRate = 100000;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1

	USART_ClearFlag(USART1, USART_FLAG_IDLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
}

void USART1_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream2);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DBUS_rx_buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = DBUS_RX_BUFF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)DBUS_rx_buff[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
	
	DMA_Cmd(DMA2_Stream2, ENABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
}
void USART1_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void DR16_Init()
{
	USART1_Init();
	USART1_NVIC_Init();
	USART1_DMA_Init();
}

void DR16_DataDecode(volatile const uint8_t *dbus_rx_buff)
{
	
	DBUS.RC.ch0          = ((( dbus_rx_buff[0] | (dbus_rx_buff [1]<<8) ) & 0x07FF)-1024);                     
	DBUS.RC.ch1          = ((( dbus_rx_buff[1]>>3 | dbus_rx_buff[2]<<5 ) & 0x07FF)-1024);                      
	DBUS.RC.ch2          = ((( dbus_rx_buff[2]>>6 | dbus_rx_buff[3]<<2 | dbus_rx_buff[4]<<10 ) & 0x07FF)-1024); 
	DBUS.RC.ch3          = (( dbus_rx_buff[4]>>1 | dbus_rx_buff[5]<<7 ) & 0x07FF)-1024;                        
	DBUS.RC.ch4          = ((( dbus_rx_buff[16] | (dbus_rx_buff [17]<<8) ) & 0x07FF)-1024);                         

	DBUS.RC.Switch_Left  = (( dbus_rx_buff[5]>>4 ) & 0x00C )>>2;                                             
	DBUS.RC.Switch_Right = ( dbus_rx_buff[5]>>4 ) & 0x003;                                                   
	
	DBUS.PC.X    = dbus_rx_buff[6] | (dbus_rx_buff[7] << 8);                                            
	
	DBUS.PC.Y       = dbus_rx_buff[8] | (dbus_rx_buff[9] << 8);                                            

	DBUS.PC.Press_Left   = dbus_rx_buff[12];                                                                   
	DBUS.PC.Press_Right  = dbus_rx_buff[13];                                                                   
	DBUS.PC.Keyboard     = dbus_rx_buff[14] | dbus_rx_buff[15] << 8;                                           
	
}
/*
*@title������1�жϴ�����
*@description��
*@param 1��	
*@param 2��	
*@return:��	
*/
void USART1_IRQHandler(void)
{
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	//�ж��Ƿ�Ϊ�����ж�
    {
        uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART1);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == DMA_Memory_0)	//��ȡ��ǰĿ���ڴ��Ƿ�Ϊ DMA_Memory_0
        {
            //��������DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, DBUS_RX_BUFF_SIZE);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == DBUS_RX_BUFF_SIZE)	//У����ճɹ�18���ֽڳ���
            {
                //����ң��������
                DR16_DataDecode(DBUS_rx_buff[1]);
            }
        }
        else
        {
            //��������DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, DBUS_RX_BUFF_SIZE);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == DBUS_RX_BUFF_SIZE)	//У����ճɹ�18���ֽڳ���
            {
                //����ң��������
                DR16_DataDecode(DBUS_rx_buff[0]);
            }
        }
    }
}
