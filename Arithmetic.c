/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* �ļ����ƣ�Arithmetic.c
* �ļ���ʶ��
* ժ    Ҫ��DES��3DES���㷨
*
* ��ǰ�汾��1.0
* ��    �ߣ�wang xindong
* ������ڣ�2009��2��12��

* ��ǰ�汾��1.1
* ��    �ߣ�wang xindong
* ������ڣ�2009��5��10��
* ��    ��������DES��3DES����˵��
**********************************************************/
#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"

/****************************************************************************
*   ���ܣ�  8�ֽ�Des�ӽ�������
*   ���룺  Mode��0������ 1������
*           DataAddr��8�ֽ�����������RAM�е��׵�ַ
*           KeyAddr��8�ֽ���Կ������RAM�е��׵�ַ
*   �����  DataAddr������������ż��ܻ���ܽ��
*   ˵����  ��1����RAM�е�8�ֽ����ݣ�8�ֽ���Կ����DES�ӽ���;
*           ��2�����8�ֽ�����������Դ����;
*           ��3��δ����DES�ķ�DPA��������;
*           ��4�������и��ӵ�DES���㣨8�ֽ��������������롢CBCģʽ��������DPA�������ܣ�
*                ���û��ο���CIU96S192UFBоƬ������ʹ���ֲᡣ
*****************************************************************************/
void DesCrypt(unsigned char mode,unsigned char *DataAddr,unsigned char *KeyAddr)
{

	unsigned char i;

    DESEN = 1;

    for(i = 0; i < 8; i++)          //дKey
    {
        DESKEYL = *(KeyAddr + i);
    }

    for(i = 0; i < 8; i++)          //д����/����
    {
        DESDATAL = *(DataAddr + i);
    }

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //��ȡ��/��������
    {
         *(DataAddr + i) = DESDATAL;
    }

    DESEN = 0;

/*  //������ʾ��
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
        CLR     AS                 	;AS=0��ѡ���������
        JMP     BEGIN
DECRYPT:SETB    AS

BEGIN:  SETB   AE                  	;AE=1������DES����
Wait1:  JB     AE,  Wait1           ;�����Ƿ���ɣ�

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
*   ���ܣ�  3Des�ӽ�������
*   ���룺  Mode��0������ 1������
*           DataAddr��8�ֽ�����������RAM�е��׵�ַ
*           Key1Addr��8�ֽ���Կkey1������RAM�е��׵�ַ
*           Key2Addr��8�ֽ���Կkey2������RAM�е��׵�ַ
*   �����  DataAddr������������ż��ܻ���ܽ��
*   ˵����  ��1����RAM�е�8�ֽ����ݣ�8�ֽ���Կkey1��8�ֽ���Կkey2����3DES�ӽ��� ;
*           ��2�����8�ֽ�����������Դ����;
*           ��3��δ����DES�ķ�DPA��������;
*           ��4�������и��ӵ�3DES���㣨8�ֽ��������������롢CBCģʽ��������DPA�������ܣ�
*                ���û��ο���CIU96S192UFBоƬ������ʹ���ֲᡣ
*****************************************************************************/
void Des3Crypt(unsigned char mode,unsigned char *DataAddr,
                unsigned char *Key1Addr,unsigned char *Key2Addr)
{

	unsigned char i;

    DESEN = 1;

    for(i = 0; i < 8; i++)          //дKey1
    {
        DESKEYL = *(Key1Addr + i);
    }

    for(i = 0; i < 8; i++)          //д����/����
    {
        DESDATAL = *(DataAddr + i);
    }

    L2C = 1;

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //дKey2
    {
        DESKEYL = *(Key2Addr + i);
    }

    if(mode == 0x01)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    L2C = 0;

    for(i = 0; i < 8; i++)          //дKey1
    {
        DESKEYL = *(Key1Addr + i);
    }

    if(mode == 0x00)  AS=0;
    else    AS=1;

    AE=1;
    while(AE);

    for(i = 0; i < 8; i++)          //��ȡ��/��������
    {
         *(DataAddr + i) = DESDATAL;
    }

    DESEN = 0;

/*  //������ʾ��
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





