/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* �ļ����ƣ�VariableDeclear.h
* �ļ���ʶ��
* ժ    Ҫ�����幤����ʹ�õ�ȫ�ֱ���
*
* ��ǰ�汾��1.00
* ��    �ߣ�wang xindong
* ������ڣ�2009��2��12��
**********************************************************/

#ifndef VARIABLE
#define VARIABLE
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                    /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

//ȫ�ֱ���
extern INT8U bdata  c_status;
extern bit bISO7816 = c_status^7;
extern bit apdu_ok = c_status^0;

extern INT16U bdata temp_bitmap;    //λ�򻺳�
extern bit tmp_b0 = temp_bitmap^0;
extern bit tmp_b1 = temp_bitmap^1;
extern bit tmp_b2 = temp_bitmap^2;
extern bit tmp_b3 = temp_bitmap^3;
extern bit tmp_b4 = temp_bitmap^4;
extern bit tmp_b5 = temp_bitmap^5;
extern bit tmp_b6 = temp_bitmap^6;
extern bit tmp_b7 = temp_bitmap^7;
extern bit tmp_b8 = temp_bitmap^8;
extern bit tmp_b9 = temp_bitmap^9;
extern bit tmp_b10 = temp_bitmap^10;
extern bit tmp_b11 = temp_bitmap^11;
extern bit tmp_b12 = temp_bitmap^12;
extern bit tmp_b13 = temp_bitmap^13;
extern bit tmp_b14 = temp_bitmap^14;
extern bit tmp_b15 = temp_bitmap^15;

//ͨѶ
extern INT8U idata com_buf[256];
extern INT8U idata recieve_buf[264];
extern INT8U idata send_buf[264];
extern unsigned char idata resp_data[256];

extern INT8U* ramptr1 ; //ָ��RAM���ݵ�ָ��1
extern INT8U* ramptr2 ; //ָ��RAM���ݵ�ָ��2
//INT8U* data tmpptr;

/*----------------------------USB------------------------------*/
//INT16U send_total_len;        //����������Ч����
extern INT16U usbsendlen;       //usb�������ݳ���
extern INT8U* usbDataPtr;       //��������ָ��
extern INT8U* usbptr;           //tempָ�� ���ڲ�ʹ��
extern INT8U* usbptrep1;

extern INT16U data data_len;    //ϣ�����յ����ݳ���

extern INT16U received_len;     //��ʾ�Ѿ�ȡ�������ݳ���
extern INT16U tmp_len;

extern INT8U bdata	usb_state;
extern bit usb_conf = usb_state^0;
extern bit usb_inflag = usb_state^1;

extern INT8U bdata usbtmp;
extern bit usbtmp_b0 = usbtmp^0;
extern bit usbtmp_b1 = usbtmp^1;
extern bit usbtmp_b2 = usbtmp^2;
extern bit usbtmp_b3 = usbtmp^3;
extern bit usbtmp_b4 = usbtmp^4;
extern bit usbtmp_b5 = usbtmp^5;
extern bit usbtmp_b6 = usbtmp^6;
extern bit usbtmp_b7 = usbtmp^7;
extern INT8U bdata usbtmp2;
extern bit usbtmp2_b0 = usbtmp2^0;
extern bit usbtmp2_b1 = usbtmp2^1;
extern bit usbtmp2_b2 = usbtmp2^2;
extern bit usbtmp2_b3 = usbtmp2^3;
extern bit usbtmp2_b4 = usbtmp2^4;
extern bit usbtmp2_b5 = usbtmp2^5;
extern bit usbtmp2_b6 = usbtmp2^6;
extern bit usbtmp2_b7 = usbtmp2^7;

extern INT8U data ret;

extern unsigned char SW1;
extern unsigned char SW2;

extern unsigned char Ep2InNum;
extern unsigned char FlashAddH;
extern unsigned int USB_len;
extern unsigned char Flashcmd[5];
extern unsigned char en_out_put[16];

#endif
