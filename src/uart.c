#include "head.h"
#include "alarm.h"
#include "button.h"
#include "time2.h"
#include "main.h"
#include "oled.h"

//#include <stdio.h>
//#include <stdlib.h>

//#include <string.h>
#define Max_len 32
unsigned char uart_rx_sta; //接收状态标记
unsigned char RX_Buf[Max_len];
unsigned char TX_Buf[Max_len];

void UartInit(void) // 1200bps@12.000MHz
{
	PCON |= 0x80; //使能波特率倍速位SMOD
	SCON = 0x50;  // 8位数据,可变波特率
	// AUXR &= 0xBF;		//定时器时钟12T模式
	// AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F; //设置定时器模式
	TMOD |= 0x20; //设置定时器模式
	TL1 = 0xCC;	  //设置定时初始值
	TH1 = 0xCC;	  //设置定时重载值
	ET1 = 0;	  //禁止定时器%d中断
	TR1 = 1;	  //定时器1开始计时
	EA = 1;
	ES = 1;
}

//发送单字节
void SendData(unsigned char DAT)
{
	ES = 0;
	TI = 0;
	SBUF = DAT;
	while (TI == 0)
		;
	TI = 0;
	ES = 1;
}

//发送字符串
void SendStr(unsigned char *DAT)
{
	while (*DAT)
	{
		SendData(*DAT++);
	}
}

// 字符串转数字
/*
unsigned int Str2Num(char *dig)
{
	unsigned int num = 0;
	while (*dig != '\0')
	{
		if (*dig >= '0' && *dig <= '9')
		{
			num = num * 10 + (*dig - '0'); //(*dig-48)也行
			dig++;
		}
		else
		{
			num = 0;
			break;
		}
	}
	return num;
}*/

void UartProc() //串口数据处理
{
	unsigned char i, tmp;
	unsigned int year;
	unsigned char mon, day, hour, min, sec;
	alarm tempalarm;

	if (uart_rx_sta > 0)
	{
		delay_ms(100);
		for (i = 1; i < uart_rx_sta; i++)
		{
			if (RX_Buf[i] >= '0' && RX_Buf[i] <= '9')
			{
				RX_Buf[i] = RX_Buf[i] - 0x30; // ASKII 转数字
			}
			else
			{
				RX_Buf[i] = 0;
			}
		}

		switch (RX_Buf[0])
		{
		case 'T': //时间设置 eg.T2022-01-01 00:00:00
			SetMode(0);
			year = RX_Buf[1] * 10 * 10 * 10 + RX_Buf[2] * 10 * 10 + RX_Buf[3] * 10 + RX_Buf[4];
			mon = RX_Buf[6] * 10 + RX_Buf[7];
			day = RX_Buf[9] * 10 + RX_Buf[10];
			hour = RX_Buf[12] * 10 + RX_Buf[13];
			min = RX_Buf[15] * 10 + RX_Buf[16];
			sec = RX_Buf[18] * 10 + RX_Buf[19];
			if (SetTime(year, mon, day, hour, min, sec))
				SendStr("OK");
			break;
		case 'A': //闹钟设置 A+闹钟数+HH:mm:ss+ +日一二三四五六(循环周期开1关0)
				  // eg. A0 06:00:00 0111110
			tmp = RX_Buf[1];
			hour = RX_Buf[3] * 10 + RX_Buf[4];
			min = RX_Buf[6] * 10 + RX_Buf[7];
			sec = RX_Buf[9] * 10 + RX_Buf[10];
			if (tmp < 3 && hour < 24 && min < 60 && sec < 60)
			{
				tempalarm.time[0] = hour;
				tempalarm.time[1] = min;
				tempalarm.time[2] = sec;
				SetAlarm(tmp, tempalarm);
				for (i = 0; i < 7; i++)
				{
					SetAlarmWeek(tmp, i, RX_Buf[i + 12]);
				}
				SetMode(tmp + 2);
				SendStr("OK");
			}
			break;
		case 'S': //秒表 清零:S0 开始:S1 暂停:S2
			SetMode(1);
			if (RX_Buf[1] < 3)
			{
				if (RX_Buf[1] == 0)
				{
					ClearWatch();
				}
				if (RX_Buf[1] == 1)
				{
					StartWatch();
				}
				if (RX_Buf[1] == 2)
				{
					StopWatch();
				}
				SendStr("OK");
			}
			break;
		case 'B': //模拟按键0-3 eg. B0 B1 B2 B3
			if (RX_Buf[1] < 4)
			{
				DebugKey(RX_Buf[1]);
				SendStr("OK");
			}
			break;
		case 'G': //获取当前时间 主要用于单片机时钟手动校准补偿
			year = GetYear();
			mon = GetMonth();
			day = GetDay();
			hour = GetHour();
			min = GetMin();
			sec = GetSec();
			i = Get10ms();

			TX_Buf[0] = '0' + year / 1000;
			TX_Buf[1] = '0' + year % 1000 / 100;
			TX_Buf[2] = '0' + year % 100 / 10;
			TX_Buf[3] = '0' + year % 10;
			TX_Buf[4] = '-';
			TX_Buf[5] = '0' + mon / 10;
			TX_Buf[6] = '0' + mon % 10;
			TX_Buf[7] = '-';
			TX_Buf[8] = '0' + day / 10;
			TX_Buf[9] = '0' + day % 10;
			TX_Buf[10] = ' ';
			TX_Buf[11] = '0' + hour / 10;
			TX_Buf[12] = '0' + hour % 10;
			TX_Buf[13] = ':';
			TX_Buf[14] = '0' + min / 10;
			TX_Buf[15] = '0' + min % 10;
			TX_Buf[16] = ':';
			TX_Buf[17] = '0' + sec / 10;
			TX_Buf[18] = '0' + sec % 10;
			TX_Buf[19] = '.';
			TX_Buf[20] = '0' + i % 100 / 10;
			TX_Buf[21] = '0' + i % 10;
			TX_Buf[22] = 0;
			SendStr(TX_Buf);
			break;
		}
		RX_Buf[0] = 0;
		uart_rx_sta = 0;
	}
}

void UARTRoutine() interrupt 4 //串行中断服务程序
{
	if (RI) //判断是接收中断产生
	{
		RI = 0;						  //标志位清零
		RX_Buf[uart_rx_sta++] = SBUF; // P0显示串口数据
		if (uart_rx_sta > Max_len)
			uart_rx_sta = Max_len;
	}
	if (TI) //如果是发送标志位，清零
		TI = 0;
}
