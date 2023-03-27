#ifndef __BUTTON_H__
#define __BUTTON_H__

#define KNUM 4 //按键数量

void KeyInit(void);
void RefreshButton(void);       //刷新按键状态
void DebugKey(unsigned char i); //模拟按键按下
//放在主函数if里判断按键是否按下
bit ifKey(unsigned char i); // 0 - (KNUM-1) 没有防呆注意别溢出

#endif