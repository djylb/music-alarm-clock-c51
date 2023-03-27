//#include "REG51.h"
#include "head.h"
#include "oled.h"
#include "alarm.h"
#include "music.h"
#include "uart.h"
#include "time2.h"
#include "lunar.h"
#include "button.h"
#include "main.h"
//#include <stdio.h>
//#include <string.h>

//本程序处理时间范围为1970-2099
//按键 K1:模式切换 K2:设置+、开/秒表开始 K3:设置-、关/秒表停 K4:设置切换/秒表清零
unsigned char mode = 0;    // 显示模式 0时钟 1秒表  2闹钟0  3闹钟1  4闹钟2
unsigned char setmode = 0; //  0不设置 时钟：1年 2月 3日 4时 5分 6秒 //7UTC
                           //          闹钟：1时 2分 3秒 4周日 5周一 6周二 7周三 8周四 9周五 10周六
unsigned char code ChDay[][2] = {
    41, 41,                             //占位不显示" "
    11, 1, 11, 2, 11, 3, 11, 4, 11, 5,  //初一 - 初五
    11, 6, 11, 7, 11, 8, 11, 9, 11, 10, //初六 - 初十
    10, 1, 10, 2, 10, 3, 10, 4, 10, 5,  //十一 - 十五
    10, 6, 10, 7, 10, 8, 10, 9, 2, 10,  //十六 - 二十
    13, 1, 13, 2, 13, 3, 13, 4, 13, 5,  //廿一 - 廿五
    13, 6, 13, 7, 13, 8, 13, 9, 3, 10,  //廿六 - 三十
};

unsigned char code ChMonth[] = {41, 12, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 14};

void SetMode(unsigned char m) // 显示模式 0时钟 1秒表  2闹钟0  3闹钟1  4闹钟2
{
    // OLED_Clear();
    switch (m)
    {
    case 0: //时间
        mode = 0;
        ShowTime();
        break;
    case 1: //秒表
        mode = 1;
        ShowWatch();
        break;
    case 2: //闹钟0
    case 3: //闹钟1
    case 4: //闹钟2
        mode = m;
        OLED_Clear();
        ShowAlarm();
        break;
    default:
        break;
    }
}

void main(void)
{
    unsigned char i;

    OLED_Init();         //初始化OLED
    OLED_ColorTurn(0);   // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示

    OLED_Clear();
    ShowMe();

    AlarmInit();  //初始化闹钟
    KeyInit();    //初始化按键
    BuzzerInit(); //初始化蜂鸣器
    Timer2Init(); //初始化时钟 定时器2
    PlayBuzzer(1);

    UartInit();

    delay_ms(800);
    OLED_Clear();
    ShowTime();

    while (1)
    {
        //刷新显示
        RefShow();
        //串口处理
        UartProc();
        //  PlayMusic(45);
        //音乐闹钟响铃
        if (ifAlarm())
        {
            PlayMusic(45);
        }

        //整点报时 8点-21点启用
        if (ifBuzz())
        {
            i = GetHour() % 12; //响的次数
            if (i == 0)
            {
                i = 12;
            }
            PlayBuzzer(i);
        }

        //按键处理
        if (ifKey(0)) // K1:模式切换
        {
            mode++;
            setmode = 0;
            // OLED_Clear();
            switch (mode)
            {
            // case 0: //时间
            //     ShowTime();
            //     break;
            case 1: //秒表
                ShowWatch();
                break;
            case 2: //闹钟0
                OLED_Clear();
            case 3: //闹钟1
            case 4: //闹钟2
                ShowAlarm();
                break;
            default:
                mode = 0;
                ShowTime();
                break;
            }
        }

        if (ifKey(1)) // K2:设置+、开/秒表开始
        {
            switch (mode)
            {
            case 0: //时间
                switch (setmode)
                {
                case 0:
                    break;
                case 1: //增加年
                    SetYear(GetYear() + 1);
                    break;
                case 2: //增加月
                    SetMonth(GetMonth() + 1);
                    break;
                case 3: //增加日
                    SetDay(GetDay() + 1);
                    break;
                case 4: //增加时
                    SetHour(GetHour() + 1);
                    break;
                case 5: //增加分
                    SetMin(GetMin() + 1);
                    break;
                case 6: //增加秒
                    SetSec(GetSec() + 1);
                    break;
                // case 7: //增加UTC
                //     SetTimeZone(GetTimeZone() + 1);
                //     break;
                default:
                    break;
                }
                break;
            case 1: //秒表
                StartWatch();
                break;
            case 2: //闹钟0
            case 3: //闹钟1
            case 4: //闹钟2
                if (setmode > 0 && setmode < 4)
                {
                    SetAlarmTime(mode - 2, setmode - 1, GetAlarmTime(mode - 2, setmode - 1) + 1);
                }
                if (setmode > 3 && setmode < 11)
                {
                    SetAlarmWeek(mode - 2, setmode - 4, 1);
                }
                break;
            default:
                break;
            }
        }

        if (ifKey(2)) // K3:设置-、关/秒表停
        {
            switch (mode)
            {
            case 0: //时间
                switch (setmode)
                {
                case 0:
                    break;
                case 1: //减少年
                    SetYear(GetYear() - 1);
                    break;
                case 2: //减少月
                    SetMonth(GetMonth() - 1);
                    break;
                case 3: //减少日
                    SetDay(GetDay() - 1);
                    break;
                case 4: //减少时
                    SetHour(GetHour() - 1);
                    break;
                case 5: //减少分
                    SetMin(GetMin() - 1);
                    break;
                case 6: //减少秒
                    SetSec(GetSec() - 1);
                    break;
                // case 7: //减少UTC
                //     SetTimeZone(GetTimeZone() - 1);
                //     break;
                default:
                    break;
                }
                break;
            case 1: //秒表
                StopWatch();
                break;
            case 2: //闹钟0
            case 3: //闹钟1
            case 4: //闹钟2
                if (setmode > 0 && setmode < 4)
                {
                    SetAlarmTime(mode - 2, setmode - 1, GetAlarmTime(mode - 2, setmode - 1) - 1);
                }
                if (setmode > 3 && setmode < 11)
                {
                    SetAlarmWeek(mode - 2, setmode - 4, 0);
                }
                break;
            default:
                break;
            }
        }

        if (ifKey(3)) // K4:设置切换/秒表清零
        {
            switch (mode)
            {
            case 0: //时间
                setmode++;
                if (setmode > 6)
                    setmode = 0;
                break;
            case 1: //秒表
                ClearWatch();
                break;
            case 2: //闹钟0
            case 3: //闹钟1
            case 4: //闹钟2
                setmode++;
                if (setmode > 10)
                    setmode = 0;
                break;
            default:
                break;
            }
        }
    }
}

void ShowMe(void)
{
    OLED_Clear();
    OLED_ShowString(22, 4, " By D-Jy", 16, 0);
}

void ShowAlarm(void)
{
    // OLED_Clear();
    OLED_ShowChinese(0, 0, 0, 16, 0);   //日
    OLED_ShowChinese(18, 0, 1, 16, 0);  //一
    OLED_ShowChinese(36, 0, 2, 16, 0);  //二
    OLED_ShowChinese(54, 0, 3, 16, 0);  //三
    OLED_ShowChinese(72, 0, 4, 16, 0);  //四
    OLED_ShowChinese(90, 0, 5, 16, 0);  //五
    OLED_ShowChinese(108, 0, 6, 16, 0); //六

    OLED_ShowString(8, 2, "A0", 16, 0); //闹钟0
    // OLED_ShowNum(32, 2, 0, 2, 16, 1, 0); // HH
    OLED_ShowChar(48, 2, ':', 16, 0);
    // OLED_ShowNum(56, 2, 0, 2, 16, 1, 0); // mm
    OLED_ShowChar(72, 2, ':', 16, 0);
    // OLED_ShowNum(80, 2, 5, 2, 16, 1, 0); // ss
    // OLED_ShowChinese(108, 2, 28, 16, 1); //开
    // OLED_ShowChinese(108, 2, 29, 16, 1); //关

    OLED_ShowString(8, 4, "A1", 16, 0); //闹钟1
    // OLED_ShowNum(32, 4, 12, 2, 16, 1, 0); // HH
    OLED_ShowChar(48, 4, ':', 16, 0);
    // OLED_ShowNum(56, 4, 0, 2, 16, 1, 0); // mm
    OLED_ShowChar(72, 4, ':', 16, 0);
    // OLED_ShowNum(80, 4, 5, 2, 16, 1, 0); // ss
    // OLED_ShowChinese(108, 4, 28, 16, 1); //开
    // OLED_ShowChinese(108, 4, 29, 16, 1); //关

    OLED_ShowString(8, 6, "A2", 16, 0); //闹钟2
    // OLED_ShowNum(32, 6, 12, 2, 16, 1, 0); // HH
    OLED_ShowChar(48, 6, ':', 16, 0);
    // OLED_ShowNum(56, 6, 0, 2, 16, 1, 0); // mm
    OLED_ShowChar(72, 6, ':', 16, 0);
    // OLED_ShowNum(80, 6, 5, 2, 16, 1, 0); // ss
    // OLED_ShowChinese(108, 6, 28, 16, 1); //开
    // OLED_ShowChinese(108, 6, 29, 16, 1); //关

    RefShowAlarm();
}

void RefShowAlarm(void)
{
    OLED_ShowChinese(0, 0, (setmode && !GetAlarmWeekDay(mode - 2, 0)) ? 41 : 0, 16, setmode == 4 || (GetAlarmWeekDay(mode - 2, 0) && !setmode));    //日
    OLED_ShowChinese(18, 0, (setmode && !GetAlarmWeekDay(mode - 2, 1)) ? 41 : 1, 16, setmode == 5 || (GetAlarmWeekDay(mode - 2, 1) && !setmode));   //一
    OLED_ShowChinese(36, 0, (setmode && !GetAlarmWeekDay(mode - 2, 2)) ? 41 : 2, 16, setmode == 6 || (GetAlarmWeekDay(mode - 2, 2) && !setmode));   //二
    OLED_ShowChinese(54, 0, (setmode && !GetAlarmWeekDay(mode - 2, 3)) ? 41 : 3, 16, setmode == 7 || (GetAlarmWeekDay(mode - 2, 3) && !setmode));   //三
    OLED_ShowChinese(72, 0, (setmode && !GetAlarmWeekDay(mode - 2, 4)) ? 41 : 4, 16, setmode == 8 || (GetAlarmWeekDay(mode - 2, 4) && !setmode));   //四
    OLED_ShowChinese(90, 0, (setmode && !GetAlarmWeekDay(mode - 2, 5)) ? 41 : 5, 16, setmode == 9 || (GetAlarmWeekDay(mode - 2, 5) && !setmode));   //五
    OLED_ShowChinese(108, 0, (setmode && !GetAlarmWeekDay(mode - 2, 6)) ? 41 : 6, 16, setmode == 10 || (GetAlarmWeekDay(mode - 2, 6) && !setmode)); //六

    OLED_ShowString(8, 2, "A0", 16, mode == 2);                                     //闹钟0
    OLED_ShowNum(32, 2, GetAlarmTime(0, 0), 2, 16, 1, (mode == 2 && setmode == 1)); // HH
    OLED_ShowNum(56, 2, GetAlarmTime(0, 1), 2, 16, 1, (mode == 2 && setmode == 2)); // mm
    OLED_ShowNum(80, 2, GetAlarmTime(0, 2), 2, 16, 1, (mode == 2 && setmode == 3)); // ss
    if (GetAlarmWeek(0))
        OLED_ShowChinese(108, 2, 28, 16, 0); //开
    else
        OLED_ShowChinese(108, 2, 29, 16, 0); //关

    OLED_ShowString(8, 4, "A1", 16, mode == 3);                                     //闹钟1
    OLED_ShowNum(32, 4, GetAlarmTime(1, 0), 2, 16, 1, (mode == 3 && setmode == 1)); // HH
    OLED_ShowNum(56, 4, GetAlarmTime(1, 1), 2, 16, 1, (mode == 3 && setmode == 2)); // mm
    OLED_ShowNum(80, 4, GetAlarmTime(1, 2), 2, 16, 1, (mode == 3 && setmode == 3)); // ss
    if (GetAlarmWeek(1))
        OLED_ShowChinese(108, 4, 28, 16, 0); //开
    else
        OLED_ShowChinese(108, 4, 29, 16, 0); //关

    OLED_ShowString(8, 6, "A2", 16, mode == 4);                                     //闹钟2
    OLED_ShowNum(32, 6, GetAlarmTime(2, 0), 2, 16, 1, (mode == 4 && setmode == 1)); // HH
    OLED_ShowNum(56, 6, GetAlarmTime(2, 1), 2, 16, 1, (mode == 4 && setmode == 2)); // mm
    OLED_ShowNum(80, 6, GetAlarmTime(2, 2), 2, 16, 1, (mode == 4 && setmode == 3)); // ss
    if (GetAlarmWeek(2))
        OLED_ShowChinese(108, 6, 28, 16, 0); //开
    else
        OLED_ShowChinese(108, 6, 29, 16, 0); //关
}

void ShowTime(void)
{
    OLED_Clear();

    // OLED_ShowNum(0, 0, 2020, 4, 16, 1, 0); // yyyy
    OLED_ShowChar(32, 0, '_', 8, 0);
    // OLED_ShowNum(38, 0, 12, 2, 16, 1, 0); // MM
    OLED_ShowChar(54, 0, '_', 8, 0);
    // OLED_ShowNum(60, 0, 30, 2, 16, 1, 0); // dd
    OLED_ShowChinese(78, 0, 15, 16, 0); //星
    OLED_ShowChinese(95, 0, 16, 16, 0); //期
    // OLED_ShowChinese(112, 0, 6, 16, 0);   //六

    // OLED_ShowString(40, 2, "UTC:", 8, 0);
    //  OLED_ShowChar(64, 0, (GetTimeZone() < 0) ? '-' : '+', 8, 0);
    //  OLED_ShowNum(70, 2, GetTimeZone(), 2, 8, 0, 0);

    // OLED_ShowNum(32, 3, 12, 2, 16, 1, 0); // HH
    OLED_ShowChar(48, 3, ':', 16, 0);
    // OLED_ShowNum(56, 3, 0, 2, 16, 1, 0); // mm
    OLED_ShowChar(72, 3, ':', 16, 0);
    // OLED_ShowNum(80, 3, 5, 2, 16, 1, 0); // ss

    // OLED_ShowChinese(10, 6, 19, 16, 1); //闰
    // OLED_ShowChinese(28, 6, 10, 16, 0); //十
    // OLED_ShowChinese(46, 6, 1, 16, 0);  //一
    OLED_ShowChinese(64, 6, 18, 16, 0); //月
    // OLED_ShowChinese(82, 6, 11, 16, 0); //初
    // OLED_ShowChinese(100, 6, 5, 16, 0); //五
    RefShowTime();
}

void RefShowTime(void)
{
    OLED_ShowNum(0, 0, GetYear(), 4, 16, 1, setmode == 1);   // yyyy
    OLED_ShowNum(38, 0, GetMonth(), 2, 16, 1, setmode == 2); // MM
    OLED_ShowNum(60, 0, GetDay(), 2, 16, 1, setmode == 3);   // dd
    OLED_ShowChinese(112, 0, GetWeek(), 16, 0);              //星期

    // OLED_ShowChar(64, 2, (GetTimeZone() < 0) ? '-' : '+', 8, setmode == 7);
    // OLED_ShowNum(70, 2, GetTimeZone(), 2, 8, 0, setmode == 7); // UTC

    OLED_ShowNum(32, 3, GetHour(), 2, 16, 1, setmode == 4); // HH
    OLED_ShowNum(56, 3, GetMin(), 2, 16, 1, setmode == 5);  // mm
    OLED_ShowNum(80, 3, GetSec(), 2, 16, 1, setmode == 6);  // ss

    // OLED_ShowNum(0, 5, GetTimeStamp(), 10, 8, 0, setmode == 7);

    if (GetLunarFlag())
        OLED_ShowChinese(10, 6, 19, 16, 1); //闰
    else
        OLED_ShowChinese(10, 6, 41, 16, 0); //" "
    if (GetLunarMonth() == 11)
        OLED_ShowChinese(28, 6, 10, 16, 0); //十
    else
        OLED_ShowChinese(28, 6, 41, 16, 0); //" "

    OLED_ShowChinese(46, 6, ChMonth[GetLunarMonth()], 16, 0); //一
    OLED_ShowChinese(82, 6, ChDay[GetLunarDay()][0], 16, 0);  //初
    OLED_ShowChinese(100, 6, ChDay[GetLunarDay()][1], 16, 0); //五
}

void ShowWatch(void)
{
    OLED_Clear();

    OLED_ShowChinese(47, 1, 22, 16, 0); //秒
    OLED_ShowChinese(65, 1, 25, 16, 0); //表

    // OLED_ShowNum(20, 4, 0, 2, 16, 1, 0); // HH
    OLED_ShowChar(36, 4, ':', 16, 0);
    // OLED_ShowNum(44, 4, 0, 2, 16, 1, 0); // mm
    OLED_ShowChar(60, 4, ':', 16, 0);
    // OLED_ShowNum(68, 4, 0, 2, 16, 1, 0); // ss
    OLED_ShowChar(84, 4, '.', 16, 0);
    // OLED_ShowNum(92, 4, 0, 2, 16, 1, 0); // 10ms
    RefShowWatch();
}

void RefShowWatch(void)
{
    OLED_ShowNum(20, 4, GetWatchHour(), 2, 16, 1, 0); // HH
    OLED_ShowNum(44, 4, GetWatchMin(), 2, 16, 1, 0);  // mm
    OLED_ShowNum(68, 4, GetWatchSec(), 2, 16, 1, 0);  // ss
    OLED_ShowNum(92, 4, GetWatch10ms(), 2, 16, 1, 0); // 10ms
}

void RefShow(void)
{
    switch (mode)
    {
    case 0: //时间
        RefShowTime();
        break;
    case 1: //秒表
        RefShowWatch();
        break;
    case 2: //闹钟0
    case 3: //闹钟1
    case 4: //闹钟2
        RefShowAlarm();
        break;
    default:
        break;
    }
}
