#ifndef __ALARM_H__
#define __ALARM_H__

#define ANUM 3 //闹钟数量

typedef struct
{
    unsigned char time[3]; // 0:h 1:m 2:s
    unsigned char week;
} alarm;

void AlarmInit(void);
void RefreshAlarm(void);                                              // 刷新闹钟状态
//void GetAlarm(unsigned char n, alarm *a);                             // 第n个 0 - (ANUM) 没有防呆注意别溢出
void SetAlarm(unsigned char n, alarm a);                              // 第n个
void SetAlarmTime(unsigned char n, unsigned char t, unsigned char i); // n:闹钟 t:0,1,2 时分秒 i:数据
void SetAlarmWeek(unsigned char n, unsigned char w, unsigned char i); // n:第几个闹钟 w:0-6 0为周日 i: 0关 1开
unsigned char GetAlarmTime(unsigned char n, unsigned char t);         // n:闹钟 t:0,1,2 时分秒
bit GetAlarmWeekDay(unsigned char n, unsigned char w);                // n:第几个闹钟 w:星期0-6 0为周日
unsigned char GetAlarmWeek(unsigned char n);                          // n:第几个闹钟

//放在主函数if里判断按键是否按下
bit ifAlarm(void); // 是否有闹钟

#endif