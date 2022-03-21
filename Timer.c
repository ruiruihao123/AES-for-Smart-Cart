/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：Timer.c
* 文件标识：
* 摘    要：定时器的设置、定时器中断服务程序、ETU延时
*
* 当前版本：1.00
* 作    者：wang xindong
* 完成日期：2009年2月12日
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"
#include <APP_LIB.h>

/****************************************************************************
*   功能：Timer0定时中断服务程序
*   输入：无
*   输出：无
*****************************************************************************/
void timer0_irq() evinterrupt 8 priority 10
{
    unsigned char temp;

    ET0 = 0;
	TR0 = 0;

    temp= 0x60;
	HED_T0_SendNBytesRam(&temp,1);     //Time0中断服务内容，串口发送60H

	TR0 = 1;
    ET0 = 1;
}
unsigned char gLED = 0;
unsigned char softcount = 0;
/****************************************************************************
*   功能：Timer1定时中断服务程序
*   输入：无
*   输出：无
*****************************************************************************/
void timer1_irq() evinterrupt 9 priority 10
{
    unsigned char temp;

    ET1 = 0;

    temp = GPIODRL;

    if((temp & 0x01) == 0x01)
    {
        GPIODRL = temp & 0xfe;
    }

    else if((temp & 0x01) == 0x00)
    {
        GPIODRL = temp | 0x01;
    }


    ET1 = 1;
}


/****************************************************************************
*   功能：使用Time0延时6个ETU
*   输入：无
*   输出：无
*   说明：由于是对外部时钟计数，仅在串口模式下使用
*****************************************************************************/
void delay6ETU(void)
{
    TIMEREN = 1;

    TMOD = 0x02;
    TCON = 0x01;
    TL0 = 0xfa;
    TR0 = 1;
    while(!TF0){};
    TF0 = 0;
    TR0 = 0;

    TIMEREN = 0;
}

/****************************************************************************
*   功能：定时器设置
*   输入：无
*   输出：无
*   说明：示例Timer0计数9000etu，Timer1定时1.365ms
*****************************************************************************/
void SetTimer(void)
{
    TIMEREN = 1;

    TMOD = 0x80;        //T1定时，CPU clock/64
    TCON = 0x01;        //T0计数

    TL0 = 0xDB;         //计数9000etu
    TH0 = 0xDC;
    RTL0 = 0xDB;
    RTH0 = 0xDC;

    TL1 = 0x00;
    TH1 = 0x80;         //定时87.4ms
    RTL1 = 0x00;
    RTH1 = 0x80;
}
