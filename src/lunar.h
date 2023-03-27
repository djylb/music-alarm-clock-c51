#ifndef __LUNAR_H__
#define __LUNAR_H__

void RefreshLunar(int year, int month, int day); //刷新农历日期

int GetLunarMonth(void); //获得农历月
int GetLunarDay(void);	 //获得农历日
unsigned char GetLunarFlag(void);	 //是否为农历闰月

#endif
