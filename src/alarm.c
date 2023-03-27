#include "head.h"
#include "alarm.h"
#include "time2.h"

#define GET_BIT(x, bit) ((x & (1 << bit)) >> bit) /* 获取第bit位 */
#define CLEAR_BIT(x, bit) (x &= ~(1 << bit))      /* 清零第bit位 */
#define SET_BIT(x, bit) (x |= (1 << bit))         /* 置位第bit位 */

alarm alarmclock[ANUM];
bit alarmstatus = 0;

void AlarmInit(void)
{
    unsigned char i, j;
    for (i = 0; i < ANUM; i++)
    {
        for (j = 0; j < 3; j++)
        {
            alarmclock[i].time[j] = 0;
        }
        alarmclock[i].week = 0;
    }
}

//放在主函数if里判断按键是否按下
bit ifAlarm(void) // 是否有闹钟
{
    if (alarmstatus)
    {
        alarmstatus = 0;
        return 1;
    }
    return 0;
}

void RefreshAlarm(void) //刷新闹钟状态
{
    unsigned char i;
    for (i = 0; i < ANUM; i++)
    {
        if (GET_BIT(alarmclock[i].week, GetWeek()) && alarmclock[i].time[0] == GetHour() && alarmclock[i].time[1] == GetMin() && alarmclock[i].time[2] == GetSec())
        {
            alarmstatus = 1;
        }
    }
}

/*
void GetAlarm(unsigned char n, alarm *a) // 0 - (ANUM) 没有防呆注意别溢出
{
    unsigned char i;
    for (i = 0; i < 3; i++)
    {
        a->time[i] = alarmclock[n].time[i];
    }
    a->week = alarmclock[n].week;
}*/

unsigned char GetAlarmTime(unsigned char n, unsigned char t) // n:闹钟 t:0,1,2 时分秒
{
    return alarmclock[n].time[t];
}

void SetAlarm(unsigned char n, alarm a)
{
    unsigned char i;
    //闹钟时间
    for (i = 0; i < 3; i++)
    {
        alarmclock[n].time[i] = a.time[i];
    }
    //闹钟循环周期
    alarmclock[n].week = 1;
    // alarmclock[n].week = a.week;
}

void SetAlarmTime(unsigned char n, unsigned char t, unsigned char i) // n:闹钟 t:0,1,2 时分秒 i:数据
{
    if (t == 0 && i > 23 || i > 59)
    {
        i = 0;
    }

    alarmclock[n].time[t] = i;
}

void SetAlarmWeek(unsigned char n, unsigned char w, unsigned char i) // n:第几个闹钟 w:0-6 0为周日 i: 0关 1开
{
    if (i)
    {
        SET_BIT(alarmclock[n].week, w);
    }
    else
    {
        CLEAR_BIT(alarmclock[n].week, w);
    }
}

bit GetAlarmWeekDay(unsigned char n, unsigned char w) // n:第几个闹钟 w:星期0-6 0为周日
{
    return GET_BIT(alarmclock[n].week, w);
}

unsigned char GetAlarmWeek(unsigned char n) // n:第几个闹钟
{
    return alarmclock[n].week;
}
