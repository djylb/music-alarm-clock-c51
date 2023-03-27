#ifndef __UART_H__
#define __UART_H__

void UartInit(void); // 1200bps@12.000MHz
void SendData(unsigned char DAT);//发送单字节
void SendStr(unsigned char *DAT);//发送字符串
void UartProc(void); //串口数据处理

#endif
