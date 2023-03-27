#include "head.h"
#include "music.h"

#define RldTmr(fr) 65536 - (FOSC / 12) / (fr << 1)
#define FuDian(n) (n << 1) / 3 //附点n分音符的换算

unsigned int code noteFreq[] = {

    523, 587, 659, 698, 784, 880, 988, 1047, 1175, 1319, 1397, 1568, 1760, 1976, 2, 494, //中音 1-7 和高音 1-7对应的频率列表
};
unsigned int code tmrRld[] = {
    RldTmr(523), RldTmr(587), RldTmr(659), RldTmr(698), RldTmr(784), RldTmr(880), RldTmr(988),        //中音 1-7
    RldTmr(1047), RldTmr(1175), RldTmr(1319), RldTmr(1397), RldTmr(1568), RldTmr(1760), RldTmr(1976), //高音 1-7对应的定时器重载值
};
unsigned char code musicNote[] = {
    //音名
    8, 7, 8, 3, 1, 5, 2, 3, 1, 8, 7, 6, 7, 10, 12, 13,  // 1
    11, 10, 9, 11, 9, 10, 8, 7, 6, 5, 4, 3, 2, 4, 3, 2, // 2
    1, 2, 3, 4, 5, 2, 5, 4, 3, 6, 5, 4, 5, 4, 3, 2,     // 3
    1, 1, 6, 7, 8, 7, 6, 5, 4, 3, 2, 6, 5, 6, 5, 4,     // 4
    12, 10, 11, 12, 10, 11, 12, 5, 6, 7, 8, 9, 10, 11,  // 5
    10, 8, 9, 10, 3, 4, 5, 6, 5, 4, 5, 3, 4, 5,         // 6
    4, 6, 5, 4, 3, 2, 3, 2, 1, 2, 3, 4, 5, 6,           // 7
    4, 6, 5, 6, 7, 8, 5, 6, 7, 8, 9, 10, 11, 12,        // 8
    10, 8, 9, 10, 9, 8, 9, 7, 8, 9, 10, 9, 8, 7,        // 9
    8, 6, 7, 8, 1, 2, 3, 4, 3, 2, 3, 8, 7, 8,           // 10
    6, 8, 7, 6, 5, 4, 5, 4, 3, 4, 5, 6, 7, 1,           // 11
    6, 8, 7, 8, 7, 6, 7, 8, 9, 8, 7, 8, 6, 7,           // 12
    10, 3, 4, 3, 2, 9, 10, 9,                           // 13
    8, 3, 1, 6, 5, 5, 4, 5,                             // 14
    6, 6, 7, 6, 7, 5, 4, 5,                             // 15
    6, 6, 5, 6, 7, 7, 6, 7,                             // 16
    1, 8, 9, 8, 7, 7, 1, 7,                             // 17
    6, 6, 5, 6, 7, 7, 3, 2,                             // 18
    1, 8, 9, 11, 10, 3, 5, 10,                          // 19
    8, 11, 10, 11, 9, 5, 4, 5,                          // 20
};
unsigned char code noteDuration[] = {
    //音名对应的时值，4表示4分音符，8表示8分音符，16表示16分音符
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,       // 1
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,       // 2
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,       // 3
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,       // 4
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 5
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 6
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 7
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 8
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 9
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 10
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 11
    8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 12
    8, 8, 8, 8, 8, 8, 8, 8,                               // 13
    8, 8, 8, 8, 8, 8, 8, 8,                               // 14
    8, 8, 8, 8, 8, 8, 8, 8,                               // 15
    8, 8, 8, 8, 8, 8, 8, 8,                               // 16
    8, 8, 8, 8, 8, 8, 8, 8,                               // 17
    8, 8, 8, 8, 8, 8, 8, 8,                               // 18
    8, 8, 8, 8, 8, 8, 8, 8,                               // 19
    8, 8, 8, 8, 8, 8, 8, 8,                               // 20
};

bit data enable = 1;
unsigned char data T0RH = 0XFF, T0RL = 0X00;

unsigned char data i, idx;
unsigned int data cnt = 0;
unsigned int data durationCnt = 0; //当前音符的时值对应的定时器计数
unsigned int data soundCnt = 0;    //当前音符的发声时值对应的计数值
unsigned char data speed = 48;     //固定标准为4分音符的速度：例如speed = 108 表示一分钟扫过108个4分音符
unsigned char data buzzertime = 0; //蜂鸣器响的次数
bit playmusicflag = 0;
bit playbuzzerflag = 0;

void BuzzerInit(void)
{
    EA = 1;
    TMOD &= 0xF0; //设置定时器模式
    TMOD |= 0x01; //设置定时器模式
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;
    TR0 = 1;
}

void PlayMusic(unsigned char musicspeed)
{
    speed = musicspeed;
    playmusicflag = 1;
}

void PlayBuzzer(unsigned char time)
{
    buzzertime = time;
    playbuzzerflag = 1;
}

void StopBuzzer(void)
{
    enable = 0;
    playmusicflag = 0;
    playbuzzerflag = 0;
}

void PlayLoop()
{
    if (playbuzzerflag)
    {
        if (i < buzzertime)
        {
            if (cnt == 0)
            {
                // idx = musicNote[i] - 1;
                T0RH = tmrRld[i] >> 8;
                T0RL = tmrRld[i];
                durationCnt = (unsigned long)240 * (unsigned long)noteFreq[idx] / ((unsigned long)noteDuration[i] * (unsigned long)speed);
                soundCnt = durationCnt - (durationCnt >> 2); //当前音符时值的前3/4发声，后1/4静音
                enable = 1;
                cnt++;
            }
            else
            {
                if (cnt == durationCnt)
                {
                    cnt = 0;
                    i++;
                }
                else
                {
                    cnt++;
                    if (cnt == soundCnt)
                    {
                        enable = 0;
                    }
                }
            }
        }
        else
        {
            playbuzzerflag = 0;
        }
    }
    else if (playmusicflag)
    {

        if (i < sizeof(musicNote))
        {
            if (cnt == 0)
            {
                idx = musicNote[i] - 1;
                T0RH = tmrRld[idx] >> 8;
                T0RL = tmrRld[idx];
                durationCnt = (unsigned long)240 * (unsigned long)noteFreq[idx] / ((unsigned long)noteDuration[i] * (unsigned long)speed);
                soundCnt = durationCnt - (durationCnt >> 2); //当前音符时值的前3/4发声，后1/4静音
                enable = 1;
                cnt++;
            }
            else
            {
                if (cnt == durationCnt)
                {
                    cnt = 0;
                    i++;
                }
                else
                {
                    cnt++;
                    if (cnt == soundCnt)
                    {
                        enable = 0;
                    }
                }
            }
        }
        else
        {
            playmusicflag = 0;
        }
    }
    else
    {
        i = 0;
        idx = 0;
        cnt = 0;
        durationCnt = 0; //当前音符的时值对应的定时器计数
        soundCnt = 0;    //当前音符的发声时值对应的计数值
        enable = 0;
    }
}

void InterruptTmr0() interrupt 1
{
    TH0 = T0RH;
    TL0 = T0RL;
    if (enable)
        BUZZ = ~BUZZ;
    else
        BUZZ = 1;
    PlayLoop();
}