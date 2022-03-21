/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：SPI_Flash.c
* 文件标识：
* 摘    要：外挂FLASH的操作
*
* 历史版本：1.00
* 作    者：wang xindong
* 完成日期：2009年8月4日
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"
#include <APP_LIB.h>

unsigned char spi_BlockErase(unsigned long BlockAddr)
{
    if(BlockAddr > W25X40_BLOCK) return ERR_ADDRESS;

    BlockAddr &= 0x00FF0000;

    spi_ReadStatusRegister();

    spi_WriteEnable();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xD8;
    Flashcmd[1] = (unsigned char)(BlockAddr>>16);
    Flashcmd[2] = (unsigned char)(BlockAddr>>8);
    Flashcmd[3] = (unsigned char) BlockAddr;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    GPIODRL |= spi_SS_PIN;

    return EXECUTE_OK;
}

unsigned char spi_SectorErase(unsigned long SectorAddr)
{
    if(SectorAddr > W25X40_BLOCK) return ERR_ADDRESS;

    SectorAddr &= 0x00FFF000;

    spi_ReadStatusRegister();

    spi_WriteEnable();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x20;
    Flashcmd[1] = (unsigned char)(SectorAddr>>16);
    Flashcmd[2] = (unsigned char)(SectorAddr>>8);
    Flashcmd[3] = (unsigned char) SectorAddr;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    GPIODRL |= spi_SS_PIN;

    return EXECUTE_OK;
}

void spi_ChipErase(void)
{
    spi_ReadStatusRegister();

    spi_WriteEnable();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xC7;
    HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

    GPIODRL |= spi_SS_PIN;
}

unsigned char spi_PageProgram(unsigned long PageAddr, unsigned char *DataAddr)
{
    if(PageAddr > W25X40_BLOCK) return ERR_ADDRESS;

    spi_ReadStatusRegister();

    spi_WriteEnable();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x02;
    Flashcmd[1] = (unsigned char)(PageAddr>>16);
    Flashcmd[2] = (unsigned char)(PageAddr>>8);
    Flashcmd[3] = (unsigned char) PageAddr;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    HED_SPI_SendNBytesRam(DataAddr,256);

    GPIODRL |= spi_SS_PIN;
    spi_ReadStatusRegister();

    return EXECUTE_OK;
}

void spi_FastRead
(unsigned char *DataAddr,unsigned long ReadAddr, unsigned char Len)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x0B;
    Flashcmd[1] = (unsigned char)(ReadAddr>>16);
    Flashcmd[2] = (unsigned char)(ReadAddr>>8);
    Flashcmd[3] = (unsigned char) ReadAddr;
    Flashcmd[4] = 0x00;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x05);

    HED_SPI_ReceiveNBytesRam(DataAddr,Len);

    GPIODRL |= spi_SS_PIN;
}

void spi_PowerDown(void)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xB9;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

    GPIODRL |= spi_SS_PIN;
}

void spi_ReadData
(unsigned char *DataAddr,unsigned long ReadAddr, unsigned char Len)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x03;
    Flashcmd[1] = (unsigned char)(ReadAddr>>16);
    Flashcmd[2] = (unsigned char)(ReadAddr>>8);
    Flashcmd[3] = (unsigned char) ReadAddr;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    HED_SPI_ReceiveNBytesRam(DataAddr,Len);

    GPIODRL |= spi_SS_PIN;

}

unsigned char ReleasePowerDown_DeviceID(unsigned char mode)
{
    switch(mode)
    {
        case ReleaseDevice:
            ReleasePowerDown();
            break;
        case ReadDeviceID:
            return DeviceID();
        case ReleaseDevice_ReadDeviceID:
            return Release_DeviceID();
        default:
            break;
    }
}

void ReleasePowerDown(void)
{
    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xAB;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

    GPIODRL |= spi_SS_PIN;
}

unsigned char DeviceID(void)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xAB;
    Flashcmd[1] = 0x00;
    Flashcmd[2] = 0x00;
    Flashcmd[3] = 0x00;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    HED_SPI_ReceiveNBytesRam(&Flashcmd[0],1);

    GPIODRL |= spi_SS_PIN;

    return Flashcmd[0];

}

unsigned char Release_DeviceID(void)
{
    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0xAB;
    Flashcmd[1] = 0x00;
    Flashcmd[2] = 0x00;
    Flashcmd[3] = 0x00;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x04);

    HED_SPI_ReceiveNBytesRam(&Flashcmd[0],1);

    GPIODRL |= spi_SS_PIN;

    return Flashcmd[0];

}

void spi_ReadStatusRegister(void)
{
    GPIODRL &= ~spi_SS_PIN;

    do
    {
        Flashcmd[0] = 0x05;
        HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

        HED_SPI_ReceiveNBytesRam(&Flashcmd[0],0x01);

    }while((Flashcmd[0]&0x01) == 0x01);

    GPIODRL |= spi_SS_PIN;
}

void spi_WriteStatusRegister(unsigned char StatusByte)
{
    spi_ReadStatusRegister();

    spi_WriteEnable();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x01;
    Flashcmd[1] = StatusByte;

    HED_SPI_SendNBytesRam(&Flashcmd[0],0x02);

    GPIODRL |= spi_SS_PIN;
}

void spi_WriteEnable(void)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x06;
    HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

    GPIODRL |= spi_SS_PIN;
}

void spi_WriteDisable(void)
{
    spi_ReadStatusRegister();

    GPIODRL &= ~spi_SS_PIN;

    Flashcmd[0] = 0x04;
    HED_SPI_SendNBytesRam(&Flashcmd[0],0x01);

    GPIODRL |= spi_SS_PIN;
}

