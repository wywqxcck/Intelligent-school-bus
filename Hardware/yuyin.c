#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial1_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t Serial1_RxFlag;		//���崮�ڽ��յı�־λ����

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
	
 
void Serial1_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA2���ų�ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA3���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure1;					//����ṹ�����
	USART_InitStructure1.USART_BaudRate = 9600;				//������
	USART_InitStructure1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure1.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure1.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure1.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure1.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure1);				//���ṹ���������USART_Init������USART2
//	USART2->CR2 |= (1 << 15);  // ����SWAP����
	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//ѡ������NVIC��USART2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);								//ʹ��USART2�����ڿ�ʼ����
}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial1_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial1_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial1_SendByte(Array[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial1_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial1_SendByte(String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial1_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void Serial1_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		Serial1_SendByte(Number / Serial1_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}



