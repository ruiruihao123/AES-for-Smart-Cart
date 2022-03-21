//Demo�����ο�

/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* �ļ����ƣ�main.c
* �ļ���ʶ��
* ժ    Ҫ��������,�жϽ���USBͨ�ŷ�֧��ISO7816����ͨ�ŷ�֧
*
* ��ʷ�汾��1.00
* ��    �ߣ�wang xindong
* ������ڣ�2009��2��12��

* ��ʷ�汾��1.01
* ��    �ߣ�wang xindong
* ������ڣ�2009��4��17��
* ��    ������ӻָ����ؽӿں���

* ��ʷ�汾��1.1
* ��    �ߣ�wang xindong
* ������ڣ�2009��5��10��
* ��    ����ɾ��ʱ������

* ��ʷ�汾��1.2
* ��    �ߣ�wang xindong
* ������ڣ�2009��7��13��
* ��    ��������ʱ������

* ��ǰ�汾��1.21
* ��    �ߣ�wang xindong
* ������ڣ�2009��8��4��
* ��    ����ɾ�����жϱ�ʶλUSBIRQ���ж�
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"


void main() priority 0
{
    unsigned char i;

    //����24MHz CPUʱ�ӣ�48MHzЭ������ʱ��
    i = CFCR;			
    i = i & 0x80;		//����CFCR�Ĵ������λ
    CFCR = i;			//������Ϊ6MHz
    i = i | 0x12;		//����CFCR�Ĵ������λ
    CFCR = i;			//������Ϊ24MHz

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
*   ���ܣ�  ��FLASHǰ4���ֽ�д0���ָ���������
*   ���룺  ��
*   �����  0x00��д�ɹ���
*           0xFF��дʧ�ܡ�
*****************************************************************************/
unsigned char ReDownload(void)
{
	_asm
            PUSH.w  R0,R1,R2
            PUSH.w SSEL
            PUSH.b CS

            MOV.b  SSELH, #01H
            MOV.b  CS, #00H

            MOV.b  R2L,#3       //�ظ�д3��

RLD_BEGIN:  MOV.W  R0, #0000H   //��ַ000000H
            MOV.w  R1, #0000H   //д������0000H

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

