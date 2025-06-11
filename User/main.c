#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "DHT11.h"
#include "TCRT5000.h"
#include "HX711.h"
#include "LED.h"
#include "KEY.h"
#include "Timer.h"
#include "usart3.h"
#include "MFRC522.h"
#include "yuyin.h"
#include "gizwits_product.h"//������ͷ�ļ�

DHT11_Data_TypeDef DHT11_Data;
u16 real_weight;    //����
unsigned char status;		//���ط�
unsigned int temp,i,aaa,bbb,ccc,ddd;
unsigned char buf[20];  //��ſ����
//9D  49  8F  C0

void clear(void)
{
	aaa = 0;
	bbb = 0;
	ccc = 0;
	ddd = 0;
}

int main(void)
{
	Serial1_Init();
  SystemInit();//����ϵͳʱ��Ϊ72M	
	delay_init(72);	
	LED_Init();
	OLED_Init();
	usart3_init(9600);
	TIM1_Int_Init(9,7199);//
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
  gizwitsInit();//�����Ƴ�ʼ��
	TIM4_Int_Init(999,71);
	KEY_GPIO_Init();
	button_init(&btn1, read_button1_GPIO, 0);
	button_attach(&btn1, SINGLE_CLICK,BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START,BTN1_LONG_PRESS_START_Handler);
	button_start(&btn1);
	
	DHT11_GPIO_Config();
	TCRT5000_Init();	
	Init_HX711pin();     //HX711��ʼ��
	OLED_ShowString(1, 1, "temp:");
	OLED_ShowString(2, 1, "humidity:");
	OLED_ShowString(3, 1, "student:");
	OLED_ShowString(4, 1, "HX711:");
	Get_Maopi();				//��ëƤ����	
	MFRC522_Init();
	
	while (1)		
	{
		
	userHandle();
	gizwitsHandle((dataPoint_t *)&currentDataPoint);//�����ư�
		
//��ʪ�ȼ��		
		if(Read_DHT11(&DHT11_Data) == SUCCESS)
		{
			OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2); //ʪ��
			OLED_ShowString(2, 12, ".");
			OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
			OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);  //�¶�
			OLED_ShowString(1, 8, ".");
			OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 1);	
		}
//������
		if(DHT11_Data.humi_int>50)
		{LED1_OFF();}
		else if (DHT11_Data.humi_int<50)
		{LED1_ON ();}	
//������³�		
		xunji();
//��⳵��ѹ��	
    Get_Weight();
		real_weight = Weight_Shiwu*0.25;		
		OLED_ShowNum(4,8,real_weight,6);
		if(real_weight>2000){Serial1_SendByte(0XC3);}
//Ѱ��ģ��
		status = MFRC522_Request(PICC_REQALL, buf);//Ѱ��
			if (status != MI_OK)
			{    
                                                                                  
					MFRC522_Reset();
					MFRC522_AntennaOff(); 
					MFRC522_AntennaOn(); 
					continue;
			}
			for(i=0;i<2;i++)
			{
					temp=buf[i];
			}
			status = MFRC522_Anticoll(buf);//����ͻ
			if (status != MI_OK)
			{    
						continue;    
			}
			temp=buf[i];	
			if(temp==0X9D)
			{	
				aaa=1;
			}
			if(temp==0X49)
			{	
				bbb=1;
			}		
			if(temp==0X8F)
			{	
				ccc=1;
			}		
			if(temp==0XC0)
			{	
				ddd=1;
			}	
	}
}
