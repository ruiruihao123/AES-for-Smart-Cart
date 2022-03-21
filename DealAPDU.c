/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：DealAPDU.c
* 文件标识：
* 摘    要：处理APDU命令
*
* 历史版本：1.00
* 作    者：wang xindong
* 完成日期：2009年8月4日

* 当前版本：1.01
* 作    者：song yadong
* 完成日期：2010年3月10日
* 描    述：修改SHA1算法调用问题
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"
#include <memory.h>
#include <APP_LIB.h>

void DealAPDU(void)
{
    unsigned char retval;

    unsigned int address;
    unsigned int CRCResult;
    unsigned char key[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
    unsigned char plaint[16];
   // unsigned char key[16]={	0x2b, 0x7e, 0x15, 0x16,0x28, 0xae, 0xd2, 0xa6,0xab, 0xf7, 0x15, 0x88,0x09, 0xcf, 0x4f, 0x3c};
    unsigned int i;

    if(APDU_CLA == 0xBF)
    {
        if(APDU_INS == 0x00)     //恢复程序下载:BF 00 55 AA 00
        {
            if(APDU_P1 == 0x55 && APDU_P2 == 0xAA)
            {
                if(APDU_P3 == 0x00)
                {
                    if(ReDownload() == 0x00)
                    {
                        SW1 = 0x90;
                        SW2 = 0x00;
                    }
                    else
                    {
                        SW1 = 0x65;
                        SW2 = 0x81;
                    }
                }
                else
                {
                    SW1=0x67;
                    SW2=0x00;
                }
            }
            else
            {
                SW1=0x6b;
                SW2=0x00;
            }
            USB_len = 2;
        }

        else if(APDU_INS == 0x40)   //AES
        {
            if((APDU_P1==0x00||APDU_P1==0x01)&&APDU_P2 == 0x00)
                {
                    HED_T0_SendNBytesRam(&APDU_INS,1);
                    HED_T0_ReceiveNBytesRam(&recieve_buf[6],APDU_P3);
                    for(i=0;i<16;i++)
                    {
                       plaint[i]=recieve_buf[6+i];
                    }
                    if(APDU_P1==0x00)
                        AES_Encrypt(key,plaint,en_out_put);
                    //en_des(plaint,key);
                    else if(APDU_P1==0x01)
                        AES_Decrypt(key,plaint,en_out_put);
                    //de_des(plaint,key);
                    if(APDU_P3 != 0)
                    {
                        if(bISO7816)
                        {
                            HED_T0_SendNBytesRam(&APDU_INS,1);
                        }

                        if(bISO7816)
                        {
                       // HED_T0_SendNBytesRam(&send_buf[2],APDU_P3);
                            HED_T0_SendNBytesRam(&en_out_put,APDU_P3);
                        }

                        USB_len = APDU_P3 + 2;
                        SW1=0x90;
                        SW2=0x00;
                    }
                    else
                    {
                        USB_len = 2;
                        SW1=0x67;
                        SW2=0x00;
                    }
                }
                else
                {
                   SW1=0x6D;
                   SW2=0x00;
                }


        }
    }
    else
    {
        USB_len = 2;
        SW1=0x6D;
        SW2=0x00;
    }
}
