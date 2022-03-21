/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：ISO7816.c
* 文件标识：
* 摘    要：完成ISO7816串口通信处理：
*
* 历史版本：1.00
* 作    者：wang xindong
* 完成日期：2009年2月12日

* 历史版本：1.1
* 作    者：wang xindong
* 完成日期：2009年3月1日
* 描    述：完善APDU命令，与USB通信一致

* 历史版本：1.2
* 作    者：wang xindong
* 完成日期：2009年3月27日
* 描    述：（1）添加SSF33算法APDU命令处理
*           （2）添加SCB2算法APDU命令处理
*           （3）添加读取工厂码数据APDU命令处理

* 历史版本：1.3
* 作    者：wang xindong
* 完成日期：2009年3月31日
* 描    述：更新FLASH函数调用接口

* 历史版本：1.31
* 作    者：wang xindong
* 完成日期：2009年4月17日
* 描    述：（1）删除串口收发函数
*           （2）函数统一调用函数库接口
*           （3）更新DES、3DES处理命令接口

* 历史版本：1.4
* 作    者：wang xindong
* 完成日期：2009年4月26日
* 描    述：（1）添加RSA运算命令接口
*           （2）添加SHA1运算接口

* 历史版本：1.41
* 作    者：wang xindong
* 完成日期：2009年6月26日
* 描    述：（1）添加GB_ECC运算命令接口

* 当前版本：1.42
* 作    者：wang xindong
* 完成日期：2009年8月4日
* 描    述：删除APDU命令处理
*********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"
#include <memory.h>
#include <APP_LIB.h>


unsigned char ATRData[] = {0x3b,0x60,0x00,0x00};

/****************************************************************************
*   功能：串口通信APDU命令处理
*   输入：无
*   输出：无
*****************************************************************************/
void ISO7816Comm()
{

    unsigned int i;

    for(i = 0; i <= 0x5f; i++);

    SCON = 0x08;                    //打开串口

    HED_T0_SendNBytesRam(ATRData,4);    //ATR:3B 60 00 00

	while (1)
	{
        HED_T0_ReceiveNBytesRam(&recieve_buf[1],5);     //接收5个字节的命令头

        DealAPDU();

        HED_T0_SendNBytesRam(&SW1,1);
        delay6ETU();
        HED_T0_SendNBytesRam(&SW2,1);
	}
}

