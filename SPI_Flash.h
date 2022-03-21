/* header file */
#ifndef _DECLARE_H_
#define _DECLARE_H_

#include <CIU96S192UFB.H>

#ifdef SYS_GLOBALS
#define SYS_EXT
#else
#define SYS_EXT extern
#endif

#define     W25X16_BLOCK                0x001FFFFF
#define     W25X32_BLOCK                0x003FFFFF
#define     W25X64_BLOCK                0x007FFFFF

#define     LED_PIN                     0x01
#define     LED_DRIVE                   0x01
#define     LED_FREQUENCE               0x05
#define     TIMER0_FREQUENCE            0x3CB0   //15536
#define     TIMER1_FREQUENCE            0x3CB0   //15536

#define     spi_SS_PIN                  0x20
#define     spi_SS_DRIVE                0x20
#define     SPI_MODE_RECV               0x01
#define     SPI_MODE_SEND               0x00

#define     EXECUTE_OK                  0x00
#define     ERR_ADDRESS                 0x01
#define     CMP_EQUAL                   0x00
#define     CMP_GREATER                 0x01
#define     CMP_LESS                    0x02

#define		TIME0_AUTO_16_MASK  		0xFC
#define 	TIME0_AUTO_16_MODE0 		0x00
#define     TIME1_AUTO_16_MASK          0xCF
#define     TIME1_AUTO_16_MODE0         0x10

#define     ReleaseDevice               0x01
#define     ReadDeviceID                0x02
#define     ReleaseDevice_ReadDeviceID  0x03

//SYS_EXT unsigned char idata program[256];
//SYS_EXT unsigned char idata receive[256];
SYS_EXT unsigned char Flashcmd[5];
//SYS_EXT unsigned char *ramptr1;
//SYS_EXT unsigned char *ramptr2;
//SYS_EXT unsigned char *ramptr3;
//SYS_EXT unsigned char com_len;
//SYS_EXT unsigned char SoftCount;
//SYS_EXT unsigned long temp;

void spi_ChipErase(void);
void spi_FastRead(unsigned long ReadAddr, unsigned char amount);
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

#endif
