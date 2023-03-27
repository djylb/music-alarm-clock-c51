#include "head.h"
#include "main.h"
#include "button.h"
#include "lunar.h"
#include "alarm.h"

#define BSTARTTIME 8 //整点报时开始时间
#define BSTOPTIME 21 //整点报时结束时间

unsigned char data time2count = 0;
unsigned long data time10ms = 0;
bit statuswatch = 0;
bit buzzflag = 0;

bit ifBuzz(void)
{
    if (buzzflag)
    {
        buzzflag = 0;
        return 1;
    }
    return 0;
}

static code unsigned char Days[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

data struct tm
{
    unsigned char tm_sec;                     /* 秒 – 取值区间为[0,59] */
    unsigned char tm_min;                     /* 分 - 取值区间为[0,59] */
    unsigned char tm_hour;                    /* 时 - 取值区间为[0,23] */
    unsigned char tm_mday;                    /* 一个月中的日期 - 取值区间为[1,31] */
    unsigned char tm_mon;                     /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int tm_year;                              /* 年份 */
    unsigned char tm_wday; /* 星期,范围0-6 */ // 0:日...6:六
} nowtime = {0, 0, 0, 1, 0, 2022, 0};
/*
void Timer2Init(void) // 10毫秒@11.0592MHz
{
    T2MOD = 0;     //初始化模式寄存器
    T2CON = 0;     //初始化控制寄存器
    TL2 = 0x00;    //设置定时初始值
    TH2 = 0xDC;    //设置定时初始值
    RCAP2L = 0x00; //设置定时重载值
    RCAP2H = 0xDC; //设置定时重载值
    TR2 = 1;       //定时器2开始计时
    ET2 = 1;       //定时器2中断
    PT2 = 0;
}
*/

void Timer2Init(void) // 10毫秒@12.000MHz
{
    T2MOD = 0;     //初始化模式寄存器
    T2CON = 0;     //初始化控制寄存器
    TL2 = 0xF0;    //设置定时初始值
    TH2 = 0xD8;    //设置定时初始值
    RCAP2L = 0xF0; //设置定时重载值
    RCAP2H = 0xD8; //设置定时重载值
    TR2 = 1;       //定时器2开始计时
    ET2 = 1;       //定时器2中断
    PT2 = 0;
}

/*
void Timer2Init(void) // 10毫秒@24.000MHz
{
    T2MOD = 0;     //初始化模式寄存器
    T2CON = 0;     //初始化控制寄存器
    TL2 = 0xE0;    //设置定时初始值
    TH2 = 0xB1;    //设置定时初始值
    RCAP2L = 0xE0; //设置定时重载值
    RCAP2H = 0xB1; //设置定时重载值
    TR2 = 1;       //定时器2开始计时
    ET2 = 1;       //定时器2中断
    PT2 = 0;
}
*/

//秒表相关
//秒表开始
void StartWatch(void)
{
    statuswatch = 1;
}

void ClearWatch(void)
{
    statuswatch = 0;
    time10ms = 0;
}

//获取秒表时间
/*
unsigned long int GetWatch(void)
{
    return time10ms;
}*/

//秒表停止
// unsigned long int StopWatch(void)
void StopWatch(void)
{
    statuswatch = 0;
    // return time10ms;
}

unsigned char GetWatchHour(void)
{
    return (unsigned char)(time10ms / (60 * 60 * 100));
}
unsigned char GetWatchMin(void)
{
    return (unsigned char)(time10ms % (60 * 60 * 100) / (60 * 100));
}
unsigned char GetWatchSec(void)
{
    return (unsigned char)(time10ms % (60 * 100) / 100);
}
unsigned int GetWatch10ms(void)
{
    return (unsigned int)(time10ms % 100);
}

//时间相关
//判断闰年
static int isleap(int year)
{
    return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}

//基姆拉尔森计算公式
// W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
static unsigned char week(int y, int m, int d) // 0:日...6:六
{
    unsigned char w;
    if (m < 3)
    {
        m += 12;
        y--;
    }

    w = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7;
    return w;
}

void RefreshTime(void)
{
    nowtime.tm_sec++;
    if (nowtime.tm_sec == 60)
    {
        nowtime.tm_sec = 0;
        nowtime.tm_min++;
        if (nowtime.tm_min == 60)
        {
            nowtime.tm_min = 0;
            nowtime.tm_hour++;
            if (nowtime.tm_hour == 24)
            {
                nowtime.tm_hour = 0;
                nowtime.tm_mday++;
                if (nowtime.tm_mday > Days[isleap(nowtime.tm_year)][nowtime.tm_mon])
                {
                    nowtime.tm_mday = 1;
                    nowtime.tm_mon++;
                    if (nowtime.tm_mon > 12)
                    {
                        nowtime.tm_mon = 1;
                        nowtime.tm_year++;
                    }
                }
            }
        }
    }
    nowtime.tm_wday = week(nowtime.tm_year, nowtime.tm_mon + 1, nowtime.tm_mday);
}

bit SetTime(int year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second)
{
    if (year > 2099 || year < 1970 || month > 12 || month < 1 || day < 1 || day > Days[isleap(year)][month] || hour > 23 || minute > 59 || second > 59)
    {
        return 0;
    }

    nowtime.tm_year = year;     /* 年 */
    nowtime.tm_mon = month - 1; /* 从1月算起，范围0-11 */
    nowtime.tm_mday = day;      /* 日: 1-31 */
    nowtime.tm_hour = hour;     /* 小时:0-23点,UTC+0时间 */
    nowtime.tm_min = minute;    /* 分钟:0-59 */
    nowtime.tm_sec = second;    /* 0-60，偶尔出现的闰秒 */
    return 1;
}

void SetYear(int year)
{
    if (year > 2099 || year < 1970)
    {
        return;
    }
    nowtime.tm_year = year;
}

void SetMonth(unsigned char month)
{
    if (month > 12)
    {
        month = 1;
    }
    if (month < 1)
    {
        month = 12;
    }
    nowtime.tm_mon = month - 1;
}

void SetDay(unsigned char day)
{
    nowtime.tm_mday = day;
    if (day > Days[isleap(nowtime.tm_year)][nowtime.tm_mon])
    {
        nowtime.tm_mday = 1;
    }
    if (day < 1)
    {
        nowtime.tm_mday = Days[isleap(nowtime.tm_year)][nowtime.tm_mon];
    }
}

void SetHour(unsigned char hour)
{
    if (hour > 23)
    {
        return;
    }
    nowtime.tm_hour = hour;
}

void SetMin(unsigned char min)
{
    if (min > 59)
    {
        return;
    }
    nowtime.tm_min = min;
}

void SetSec(unsigned char sec)
{
    if (sec > 59)
    {
        return;
    }
    nowtime.tm_sec = sec;
}

int GetYear(void)
{
    return nowtime.tm_year;
}
unsigned char GetMonth(void)
{
    return nowtime.tm_mon + 1;
}
unsigned char GetDay(void)
{
    return nowtime.tm_mday;
}
unsigned char GetHour(void)
{
    return nowtime.tm_hour;
}
unsigned char GetMin(void)
{
    return nowtime.tm_min;
}
unsigned char GetSec(void)
{
    return nowtime.tm_sec;
}
unsigned char Get10ms(void)
{
    return time2count;
}
unsigned char GetWeek(void)
{
    return nowtime.tm_wday; // 0:周日
    // return nowtime.tm_wday + 1;//1:周日
}

//定时器2中断
void Timer2Routine() interrupt 5 //	中断程序
{
    TF2 = 0;
    //RCAP2L = 0xF0; //设置定时重载值 10ms 12MHz
    //手动测试计算校准补偿 经测试每10ms快了2.9us
    RCAP2L = 0xE9; //设置定时重载值 10ms 12MHz
    RCAP2H = 0xD8; //设置定时重载值 10ms 12MHz

    //时间相关
    if (time2count == 100)
    {
        time2count = 0;
        RefreshTime();                                 //同步更新计算时间
        RefreshLunar(GetYear(), GetMonth(), GetDay()); //更新农历时间
        RefreshAlarm();
        //整点报时 8点-21点启用
        if (GetSec() == 0 && GetMin() == 0 && GetHour() >= BSTARTTIME && GetHour() <= BSTOPTIME)
        {
            buzzflag = 1;
        }
    }
    time2count++;

    //秒表
    if (statuswatch)
    {
        time10ms++;
        if (GetWatchHour() == 100)
        {
            time10ms = 0;
        }
    }

    if (time2count % 10 == 0)
    {
        //按钮相关
        RefreshButton();
        //显示相关
        // RefShow();
    }
}
