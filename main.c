//Demo仅供参考

/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：main.c
* 文件标识：
* 摘    要：主函数,判断进入USB通信分支或ISO7816串口通信分支
*
* 历史版本：1.00
* 作    者：wang xindong
* 完成日期：2009年2月12日

* 历史版本：1.01
* 作    者：wang xindong
* 完成日期：2009年4月17日
* 描    述：添加恢复下载接口函数

* 历史版本：1.1
* 作    者：wang xindong
* 完成日期：2009年5月10日
* 描    述：删除时钟配置

* 历史版本：1.2
* 作    者：wang xindong
* 完成日期：2009年7月13日
* 描    述：增加时钟配置

* 当前版本：1.21
* 作    者：wang xindong
* 完成日期：2009年8月4日
* 描    述：删除对中断标识位USBIRQ的判断
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"


void main() priority 0
{
    unsigned char i;

    //配置24MHz CPU时钟，48MHz协处理器时钟
    i = CFCR;			
    i = i & 0x80;		//保护CFCR寄存器最高位
    CFCR = i;			//先配置为6MHz
    i = i | 0x12;		//保护CFCR寄存器最高位
    CFCR = i;			//再配置为24MHz

    if(MSP)
    {
        init_usb();
        apdu_ok = 0;
        bISO7816 = 0;

        while(1);
        //{
        //    if(USBIRQ)
        //    {
        //        usb_inter();
        //    }
        //}

    }
    else
    {
        bISO7816 = 1;
        ISO7816Comm();
    }

}

/****************************************************************************
*   功能：  将FLASH前4个字节写0，恢复程序下载
*   输入：  无
*   输出：  0x00：写成功；
*           0xFF：写失败。
*****************************************************************************/
unsigned char ReDownload(void)
{
	_asm
            PUSH.w  R0,R1,R2
            PUSH.w SSEL
            PUSH.b CS

            MOV.b  SSELH, #01H
            MOV.b  CS, #00H

            MOV.b  R2L,#3       //重复写3次

RLD_BEGIN:  MOV.W  R0, #0000H   //地址000000H
            MOV.w  R1, #0000H   //写入数据0000H

            SETB EW
            MOVC.w [R0+],R1
            MOVC.w [R0+],R1
            CLR  EW

            MOV.W  R0, #0000H
            MOVC.w  R1,[R0+]
            CJNE.w  R1,#0000h,RLD_RETRY
            MOVC.w  R1,[R0+]
            CJNE.w  R1,#0000h,RLD_RETRY

            MOV.b   $ReDownload$RETURN,#00h

            BR  RLD_RET

RLD_RETRY:  DJNZ.b  R2L,RLD_BEGIN
            MOV.b   $ReDownload$RETURN,#0ffh

RLD_RET:    POP.b  CS
            POP.w  SSEL
            POP.w  R0,R1,R2

            RET
	_endasm;

    return 0;
}

