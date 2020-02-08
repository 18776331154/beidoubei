#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"
#include "usmart.h"		 	 
#include "gps.h"	
#include "usart3.h" 	
#include "key.h" 	 
#include "string.h"	
#include "oled.h"
//ALIENTEK ս��V3/��Ӣ��������չʵ��16
//ATK-S1216F8 GPSģ�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  

u8 USART_TX_BUF[250]; 					//����1,���ͻ�����
nmea_msg gpsx; 											//GPS��Ϣ
__align(4) u8 dtbuf[50];   								//��ӡ������
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode�ַ��� 
	  
//��ʾGPS��λ��Ϣ 
void Gps_Msg_Show(void)
{
		float tp=0;		   
	
		OLED_ShowNum(8,0,gpsx.utc.hour,2,16);		
		OLED_ShowChar(24,0,':',16);
		OLED_ShowNum(32,0,gpsx.utc.min,2,16);		
		OLED_ShowChar(48,0,':',16);	
		OLED_ShowNum(56,0,gpsx.utc.sec,2,16);		   	
				 
		
		tp=gpsx.longitude;	   
		sprintf((char *)dtbuf,"%.4f%1c",tp/=10000,gpsx.ewhemi);	//�õ������ַ���
		OLED_ShowString(8,2,dtbuf,16);
		
		tp=gpsx.latitude;	   
		sprintf((char *)dtbuf,"%.4f%1c",tp/=10000,gpsx.nshemi);	//�õ�γ���ַ���
		OLED_ShowString(8,4,dtbuf,16);
		
			 
	 
		OLED_ShowNum(8,6,gpsx.posslnum,2,16);		   
			
		tp=gpsx.altitude;	   
		sprintf((char *)dtbuf,"%.1fm",tp/=10);	    			//�õ��߶��ַ���
		OLED_ShowString(40,6,dtbuf,16);		   
				
 	  
}

int main(void)
{	 
	 u16 i,rxlen;	
	delay_init();	    	 //��ʱ������ʼ��	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(38400);	 	//���ڳ�ʼ��Ϊ115200
	initial_lcd();	
  OLED_Clear(); //clear all dots 
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1) 
	{	
		if(USART_RX_STA&0x8000)		//���յ�һ��������
		{
			rxlen=USART_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)
			USART_TX_BUF[i]=USART1_RX_BUF[i];		
 			USART_RX_STA=0;//������һ�ν���
			USART_TX_BUF[i]=0;			//�Զ���ӽ�����
//	  GPS_Analysis(&gpsx,(u8*)USART_TX_BUF);//�����ַ���
			NMEA_GNGGA_Analysis(&gpsx,(u8*)USART_TX_BUF);
			Gps_Msg_Show();				//��ʾ��Ϣ
			
 		}

	}
}
