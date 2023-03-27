#include "head.h"
#include "button.h"
#include "music.h"

sbit KEY1 = P1 ^ 4; //按键
sbit KEY2 = P1 ^ 5; //按键
sbit KEY3 = P1 ^ 6; //按键
sbit KEY4 = P1 ^ 7; //按键

bit NowKEY1 = 1, LastKEY1 = 1;
//bit NowKEY2 = 1, LastKEY2 = 1;
//bit NowKEY3 = 1, LastKEY3 = 1;
bit NowKEY4 = 1, LastKEY4 = 1;
unsigned char FlagKEY[KNUM] = {0};

void KeyInit(void)
{
    KEY1 = 1;
    KEY2 = 1;
    KEY3 = 1;
    KEY4 = 1;
}

void RefreshButton(void)
{
    int i;
    LastKEY1 = NowKEY1;
    //LastKEY2 = NowKEY2;
    //LastKEY3 = NowKEY3;
    LastKEY4 = NowKEY4;
    NowKEY1 = KEY1;
    //NowKEY2 = KEY2;
    //NowKEY3 = KEY3;
    NowKEY4 = KEY4;

    //有按键
    if (LastKEY1 != NowKEY1 && NowKEY1 == 1)
        FlagKEY[0] = 1;
    if (!KEY2)
        FlagKEY[1] = 1;
    if (!KEY3)
        FlagKEY[2] = 1;
    if (LastKEY4 != NowKEY4 && NowKEY4 == 1)
        FlagKEY[3] = 1;

    //任意按键取消声音
    for (i = 0; i < KNUM; i++)
    {
        if (FlagKEY[i] == 1)
        {
            StopBuzzer();
            break;
        }
    }
}

void DebugKey(unsigned char i)
{
    FlagKEY[i] = 1;
}

//放在主函数if里判断按键是否按下
bit ifKey(unsigned char i) // 0-K
{
    if (FlagKEY[i])
    {
        FlagKEY[i] = 0;
        return 1;
    }
    return 0;
}
