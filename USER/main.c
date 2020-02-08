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
//ALIENTEK 战舰V3/精英开发板扩展实验16
//ATK-S1216F8 GPS模块测试实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  

u8 USART_TX_BUF[250]; 					//串口1,发送缓存区
nmea_msg gpsx; 											//GPS信息
__align(4) u8 dtbuf[50];   								//打印缓存器
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode字符串 
	  
//显示GPS定位信息 
void Gps_Msg_Show(void)
{
		float tp=0;		   
	
		OLED_ShowNum(8,0,gpsx.utc.hour,2,16);		
		OLED_ShowChar(24,0,':',16);
		OLED_ShowNum(32,0,gpsx.utc.min,2,16);		
		OLED_ShowChar(48,0,':',16);	
		OLED_ShowNum(56,0,gpsx.utc.sec,2,16);		   	
				 
		
		tp=gpsx.longitude;	   
		sprintf((char *)dtbuf,"%.4f%1c",tp/=10000,gpsx.ewhemi);	//得到经度字符串
		OLED_ShowString(8,2,dtbuf,16);
		
		tp=gpsx.latitude;	   
		sprintf((char *)dtbuf,"%.4f%1c",tp/=10000,gpsx.nshemi);	//得到纬度字符串
		OLED_ShowString(8,4,dtbuf,16);
		
			 
	 
		OLED_ShowNum(8,6,gpsx.posslnum,2,16);		   
			
		tp=gpsx.altitude;	   
		sprintf((char *)dtbuf,"%.1fm",tp/=10);	    			//得到高度字符串
		OLED_ShowString(40,6,dtbuf,16);		   
				
 	  
}

int main(void)
{	 
	 u16 i,rxlen;	
	delay_init();	    	 //延时函数初始化	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(38400);	 	//串口初始化为115200
	initial_lcd();	
  OLED_Clear(); //clear all dots 
 	LED_Init();		  			//初始化与LED连接的硬件接口
	while(1) 
	{	
		if(USART_RX_STA&0x8000)		//接收到一次数据了
		{
			rxlen=USART_RX_STA&0X7FFF;	//得到数据长度
			for(i=0;i<rxlen;i++)
			USART_TX_BUF[i]=USART1_RX_BUF[i];		
 			USART_RX_STA=0;//启动下一次接收
			USART_TX_BUF[i]=0;			//自动添加结束符
//	  GPS_Analysis(&gpsx,(u8*)USART_TX_BUF);//分析字符串
			NMEA_GNGGA_Analysis(&gpsx,(u8*)USART_TX_BUF);
			Gps_Msg_Show();				//显示信息
			
 		}

	}
}
