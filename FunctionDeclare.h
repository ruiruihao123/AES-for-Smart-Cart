/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* �ļ����ƣ�FunctionDeclear.h
* �ļ���ʶ��
* ժ    Ҫ����������
*
* ��ʷ�汾��1.00
* ��    �ߣ�wang xindong
* ������ڣ�2009��2��12��

* ��ʷ�汾��1.1
* ��    �ߣ�wang xindong
* ������ڣ�2009��3��23��
* ��    ����ɾ��GetRandom��������

* ��ʷ�汾��1.2
* ��    �ߣ�wang xindong
* ������ڣ�2009��3��31��

* ��ǰ�汾��1.3
* ��    �ߣ�wang xindong
* ������ڣ�2009��4��26��
* ��    ��������Timer0�жϺ�������
 **********************************************************/

#ifndef FUNCTION
#define FUNCTION

extern void usb_inter();
extern void init_usb();
extern void irq_control_out();
extern void stall_ctrl_ep();
extern void clear_usb_buf();
extern void send_ctrl_zero();
extern void get_ep_order();
extern void usb_get_status();
extern void usb_feature();
extern void usb_set_addr();
extern void usb_get_intface();
extern void usb_get_config();
extern void usb_set_intface();
extern void usb_set_config();
extern void usb_get_desptor();
extern void usb_wr_one_byte(unsigned char adata);
extern void usb_wr_ep();
extern void irq_control_in();
extern void des_device();
extern void des_configuration();
extern void des_hid_class();
extern void des_hid_report();
extern void des_string_language();
extern void des_string_manu();
extern void des_string_product();
extern void des_string_serial();
extern void des_report_descriptor();
extern void irq_interrupt_in();

extern void ISO7816Comm();
extern void delay6ETU(void);
extern void timer0_irq() evinterrupt 8 priority 10;

extern void void DesCrypt(unsigned char mode,unsigned char *DataAddr,unsigned char *KeyAddr);
extern void Des3Crypt(unsigned char mode,unsigned char *DataAddr,
                unsigned char *Key1Addr,unsigned char *Key2Addr);

extern void DealAPDU(void);

extern void SetTimer(void);
extern void delay6ETU(void);

extern unsigned char ReDownload(void);
extern void cmdAPDU(void);

void spi_ChipErase(void);
void spi_FastRead(unsigned char *DataAddr,unsigned long ReadAddr, unsigned char Len);
void spi_PowerDown(void);
void spi_ReadData(unsigned char *DataAddr,unsigned long ReadAddr, unsigned char Len);
void spi_ReadStatusRegister(void);
void spi_RecvData(void);
void spi_SendData(void);
void spi_WriteDisable(void);
void spi_WriteEnable(void);
void spi_WriteStatusRegister(unsigned char StatusByte);
void workingstatus_TmrIsr(void);
void ReleasePowerDown(void);
unsigned char DeviceID(void);
unsigned char Release_DeviceID(void);
unsigned char spi_BlockErase(unsigned long BlockAddr);
unsigned char spi_SectorErase(unsigned long SectorAddr);
unsigned char spi_PageProgram(unsigned long PageAddr, unsigned char *DataAddr);
unsigned char ReleasePowerDown_DeviceID(unsigned char mode);
unsigned char cmp_block(unsigned char len);

void AES_Encrypt(unsigned char *key, unsigned char *plaintext, unsigned char *ciphertext);
void AES_Decrypt(unsigned char *key, unsigned char *ciphertext, unsigned char *plaintext);
int isEqual(unsigned char *pre_plaintext, unsigned char *plaintext);

#endif
