#ifndef __MUSIC_H__
#define __MUSIC_H__

void BuzzerInit(void);
void PlayMusic(unsigned char musicspeed); //固定标准为4分音符的速度：例如speed = 108 表示一分钟扫过108个4分音符
void PlayBuzzer(unsigned char time);
void StopBuzzer(void);

sbit BUZZ = P1 ^ 0; //蜂鸣器引脚
#endif
