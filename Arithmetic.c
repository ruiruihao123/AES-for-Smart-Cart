/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：Arithmetic.c
* 文件标识：
* 摘    要：DES、3DES等算法
*
* 当前版本：1.0
* 作    者：wang xindong
* 完成日期：2009年2月12日

* 当前版本：1.1
* 作    者：wang xindong
* 完成日期：2009年5月10日
* 描    述：更新DES、3DES函数说明
**********************************************************/
#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"

/****************************************************************************
*   功能：  8字节Des加解密运算
*   输入：  Mode：0：加密 1：解密
*           DataAddr：8字节明文数据在RAM中的首地址
*           KeyAddr：8字节密钥数据在RAM中的首地址
*   输出：  DataAddr：运算结束后存放加密或解密结果
*   说明：  （1）将RAM中的8字节数据，8字节密钥进行DES加解密;
*           （2）输出8字节运算结果覆盖源数据;
*           （3）未开启DES的防DPA攻击功能;
*           （4）若进行复杂的DES运算（8字节整数倍数据输入、CBC模式、开启防DPA攻击功能）
*                请用户参看《CIU96S192UFB芯片函数库使用手册。
*****************************************************************************/
void DesCrypt(unsigned char mode,unsigned char *DataAddr,unsigned char *KeyAddr)
{

	unsigned char i;

    DESEN = 1;

    for(i = 0; i < 8; i++)          //写Key
    {
        DESKEYL = *(KeyAddr + i);
    }

    for(i = 0; i < 8; i++)          //写明文/密文
    {
        DESDATAL = *(DataAddr + i);
    }

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //读取加/解密数据
    {
         *(DataAddr + i) = DESDATAL;
    }

    DESEN = 0;

/*  //汇编代码示例
    _asm
        PUSH.w   R0,R1,R2

        MOV.b   R2L,$DesCrypt$PARAM1
        MOV.W   R0,$DesCrypt$PARAM2
        MOV.W   R1,$DesCrypt$PARAM3
        MOV.b   R2H,#8

        SETB    DESEN

INPUT_DATA:MOV.b   DESDATAL,[R0]
        ADDS    R0,#1
        DJNZ.B  R2H,INPUT_DATA

        MOV.b   R2H,#8
INPUT_KEY:MOV.b   DESKEYL,[R1]
        ADDS    R1,#1
        DJNZ.B  R2H,INPUT_KEY

        CJNE.B  R2L,#0,DECRYPT
        CLR     AS                 	;AS=0，选择加密运算
        JMP     BEGIN
DECRYPT:SETB    AS

BEGIN:  SETB   AE                  	;AE=1，启动DES运算
Wait1:  JB     AE,  Wait1           ;运算是否完成？

        MOV.W   R0,$DesCrypt$PARAM2
        MOV.b   R2H,#8

OUTPUT_DATA:MOV.b   [R0],DESDATAL
        ADDS    R0,#1
        DJNZ.B  R2H,OUTPUT_DATA

        CLR     DESEN

        POP.w   R0,R1,R2
    _endasm;
*/
}

/****************************************************************************
*   功能：  3Des加解密运算
*   输入：  Mode：0：加密 1：解密
*           DataAddr：8字节明文数据在RAM中的首地址
*           Key1Addr：8字节密钥key1数据在RAM中的首地址
*           Key2Addr：8字节密钥key2数据在RAM中的首地址
*   输出：  DataAddr：运算结束后存放加密或解密结果
*   说明：  （1）将RAM中的8字节数据，8字节密钥key1、8字节密钥key2进行3DES加解密 ;
*           （2）输出8字节运算结果覆盖源数据;
*           （3）未开启DES的防DPA攻击功能;
*           （4）若进行复杂的3DES运算（8字节整数倍数据输入、CBC模式、开启防DPA攻击功能）
*                请用户参看《CIU96S192UFB芯片函数库使用手册。
*****************************************************************************/
void Des3Crypt(unsigned char mode,unsigned char *DataAddr,
                unsigned char *Key1Addr,unsigned char *Key2Addr)
{

	unsigned char i;

    DESEN = 1;

    for(i = 0; i < 8; i++)          //写Key1
    {
        DESKEYL = *(Key1Addr + i);
    }

    for(i = 0; i < 8; i++)          //写明文/密文
    {
        DESDATAL = *(DataAddr + i);
    }

    L2C = 1;

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //写Key2
    {
        DESKEYL = *(Key2Addr + i);
    }

    if(mode == 0x01)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    L2C = 0;

    for(i = 0; i < 8; i++)          //写Key1
    {
        DESKEYL = *(Key1Addr + i);
    }

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //读取加/解密数据
    {
         *(DataAddr + i) = DESDATAL;
    }

    DESEN = 0;

/*  //汇编代码示例
    _asm

        PUSH    R0,R1,R2,R3

        MOV.b   R3L,$Des3Crypt$PARAM1
        MOV.W   R0,$Des3Crypt$PARAM2
        MOV.W   R1,$Des3Crypt$PARAM3
        MOV.W   R2,$Des3Crypt$PARAM4

        SETB	DESEN

        MOV.b   R3H,#8
INPUT_DATA3:MOV.b   DESDATAL,[R0]
        ADDS    R0,#1
        DJNZ.B  R3H,INPUT_DATA3

        MOV.b   R3H,#8
INPUT_KEY1:MOV.b   DESKEYL,[R1]
        ADDS    R1,#1
        DJNZ.B  R3H,INPUT_KEY1

        SETB    DESCON.3
        CJNE.B  R3L,#0,DECRYPT1
        CLR     AS
        BR      BEGIN1
DECRYPT1:SETB   AS
BEGIN1: SETB    AE
        $EVEN
        JB      AE, $

        MOV.b   R3H,#8
INPUT_KEY2:MOV.b   DESKEYL,[R2]
        ADDS    R2,#1
        DJNZ.B  R3H,INPUT_KEY2

        CJNE.B  R3L,#0,ENCRYPT1
        SETB    AS
        BR      BEGIN2
ENCRYPT1:CLR    AS
BEGIN2: SETB    AE
        $EVEN
        JB      AE, $

        CLR     DESCON.3

        MOV.W   R1,$DesCrypt$PARAM3
        MOV.b   R3H,#8
INPUT_KEY3:MOV.b   DESKEYL,[R1]
        ADDS    R1,#1
        DJNZ.B  R3H,INPUT_KEY3

        CJNE.B  R3L,#0,DECRYPT2
        CLR     AS
        BR      BEGIN3
DECRYPT2:SETB   AS
BEGIN3: SETB    AE
        $EVEN
        JB      AE, $

        MOV.W   R0,$Des3Crypt$PARAM2
        MOV.b   R3H,#8
OUTPUT_DATA3:MOV.b   [R0],DESDATAL
        ADDS    R0,#1
        DJNZ.B  R3H,OUTPUT_DATA3

        CLR 	DESEN
        POP    R0,R1,R2,R3
    _endasm;
*/
}





