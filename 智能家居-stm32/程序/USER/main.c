#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "dht11.h"
#include "OLED.h"
#include "adc.h"
#include "lsens.h"
#include "beep.h"
#include "timer.h"
#include "exti.h"
#include "esp8266.h"
#include "onenet.h"
#include "stdio.h"

#define USART_DEBUG	USART1



u8 alarmFlag = 0;		//�Ƿ񱨾��ı�־
u8 alarm_is_free = 10;	//�������Ƿ��ֶ�������������ֶ�����������Ϊ0

u8 t;			    
u8 temperature;  	    
u8 humidity;    	   
u8 adcx; 
u8 Led_Status = 0;

char PUB_BUF[256]; 		//�ϴ����ݵ�BUF	
extern char oledBuf[12];
const char *devSubTopic[] = {"/mysmarthome/subb"};
const char devPubTopic[] = "/mysmarthome/pubb";

 int main(void)
 {	 
	 
	unsigned short timeCount = 0;	//���ͼ������
	unsigned char *dataPtr = NULL;
  
	delay_init();	    	 //��ʱ������ʼ��	  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	EXTIX_Init();
	KEY_Init();
	BEEP_Init();
	DHT11_Init();
    Usart1_Init(115200);	 	//���ڳ�ʼ��Ϊ115200
	Usart2_Init(115200);	//stm32_8266ͨѶ����
	Lsens_Init();
	
	

	 
	OLED_Init();
	OLED_Clear();
	TIM2_Int_Init(4999, 7199);
	TIM3_Int_Init(2499, 7199);	
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	ESP8266_Init();	 
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
	 
	BEEP = 1;						//������ʾ����ɹ�
	delay_ms(250);
	BEEP = 0;
	OneNet_Subscribe(devSubTopic, 1);
	
	
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		OLED_ShowString(1, 8, "DHT11 Error");	
		delay_ms(200);
	}								   

	while(1)
	{
		
		if(timeCount % 40 == 0){
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
			adcx=Lsens_Get_Val();
			UsartPrintf(USART_DEBUG, "�¶�:%d  ʪ��:%d  ����:%d\r\n",temperature, humidity, adcx);
			Led_Status = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);	//��ȡled��״̬	
				
				if(alarm_is_free == 10){
					if(humidity<80 && temperature<30 && adcx<10){
						alarmFlag = 0;
					} else{
						alarmFlag = 1;
					}
				}
				if(alarm_is_free < 10)alarm_is_free++;
		}
		
		
		if(++timeCount >= 200)					//���ͼ��5s
		{
			Led_Status = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);	//��ȡled��״̬
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF, "{\"Temp\":%d,\"Hum\":%d,\"Light\":%d,\"Led\":%d,\"Beep\":%d}",
			temperature, humidity, adcx, Led_Status ? 0:1, alarmFlag);
			OneNet_Publish(devPubTopic, PUB_BUF);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	}
}



