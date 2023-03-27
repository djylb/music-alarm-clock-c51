#ifndef __TIME2_H__
#define __TIME2_H__

void Timer2Init(void); // 10毫秒

//秒表相关
//秒表开始
void StartWatch(void);
//清空秒表
void ClearWatch(void);

//获取秒表时间 10ms
//unsigned long int GetWatch(void);

//秒表停止
// unsigned long int StopWatch(void);
void StopWatch(void);
unsigned char GetWatchHour(void);
unsigned char GetWatchMin(void);
unsigned char GetWatchSec(void);
int GetWatch10ms(void);

//时间相关
bit ifBuzz(void);
void RefreshTime(void);
//设置时区
// void SetTimeZone(unsigned char tz); // 北京时间：+8
// int GetTimeZone(void);
// void SetTimeStamp(unsigned long t);//当地时间转时间戳储存

bit SetTime(int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second);
void SetYear(int year);
void SetMonth(unsigned char month);
void SetDay(unsigned char day);
void SetHour(unsigned char hour);
void SetMin(unsigned char min);
void SetSec(unsigned char sec);
// unsigned long GetTimeStamp(void);
int GetYear(void);
unsigned char GetMonth(void);
unsigned char GetDay(void);
unsigned char GetHour(void);
unsigned char GetMin(void);
unsigned char GetSec(void);
unsigned char Get10ms(void);
unsigned char GetWeek(void);

#endif