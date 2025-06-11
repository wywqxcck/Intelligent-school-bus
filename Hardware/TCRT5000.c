#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "yuyin.h"

uint8_t P=0;
uint16_t Trace_1;
#define Tracking_1		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//中间
#define Tracking_2		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//左边
#define Tracking_3		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//右边
#define Tracking_4		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//右边

void TCRT5000_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void xunji(void)
{
      if(Tracking_1==0 && Tracking_2==0) //双重检测
      {  	
				Delay_ms(100);
				if(Tracking_1==1 || Tracking_2==1)
				{				
					P++;	
					Serial1_SendByte(0XC1);
					Serial1_SendByte(P);
				 OLED_ShowNum (3,10,P,3);
					Delay_ms(100);
				}
      } 
      if(Tracking_3==0 && Tracking_4==0)
      {   
				Delay_ms(100);
				if(Tracking_3==1 || Tracking_4==1)
				{
					if(P==0){P=0;}
					else P--;
					Serial1_SendByte(0XC2);
					Serial1_SendByte(P);	
					 OLED_ShowNum (3,10,P,3);
					Delay_ms(100);				
				}

      }   	
}
