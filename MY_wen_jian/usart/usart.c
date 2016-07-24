#include "sys.h"
#include "usart.h"
	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
////////////////////////////////////////////////////////////////////


//����1��ʼ��
void USART1_Config(void)		//��ʼ�� ����USART1
{
	GPIO_InitTypeDef     GPIO_InitStructure;   //���ڶ˿����ýṹ�����
	USART_InitTypeDef    USART_InitStructure;  //���ڲ������ýṹ�����

	//��1������GPIO��USART������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	 //��GPIOAʱ�Ӻ�GPIOA����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//�򿪴��ڸ���ʱ��
	USART_DeInit(USART1);  //��λ����1

	//��2������USART1 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //����1���ͽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //��ʼ��GPIOA
														  
	//��3������USART Rx�����սţ���GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //����1���ս�
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //��ʼ��GPIOA

	//��4��������USART1����
	USART_InitStructure.USART_BaudRate             = 115200;							 //���������ã�115200
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;			 //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits             = USART_StopBits_1;				 //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity               = USART_Parity_No;				 //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None; //Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	 //�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);										 //��ʼ��USART1

    //�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART1�����ж� ��
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART1�����ж� ��

	//��5����ʹ�� USART1�� �������
	USART_Cmd(USART1, ENABLE);							   //ʹ�� USART1

    //�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART1, USART_FLAG_TC);                //�崮��1���ͱ�־
	
}

//����2��ʼ��
void USART2_Config(void)   //��ʼ�� ����USART2
{
	GPIO_InitTypeDef    GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
	USART_InitTypeDef   USART_InitStructure;   //���ڲ������ýṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ�� USART2 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //��PA�˿�ʱ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

	//��USART2 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART2����
	USART_InitStructure.USART_BaudRate = 115200;	                    //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
	
	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART2�����ж�
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�

	//ʹ�� USART2�� �������
	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��2���ͱ�־
}

//����3��ʼ��
void USART3_Config(void)   //��ʼ�� ����USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��PB�˿�ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART3����
	USART_InitStructure.USART_BaudRate = 2400;	                    //���������ã�2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
	
    //�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART3�����ж�
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}



void usart3_SendByte(unsigned char SendData)	  //�ô���3����һ���ֽ�����
{	   
	USART_SendData(USART1,SendData);   //�ô���3����һ���ֽ�
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //�ȴ�����3�������
}  