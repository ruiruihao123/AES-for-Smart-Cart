/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称： Usb.c
* 文件标识：
* 摘    要： USB HID协议通信，设备枚举及后续通信
*
* 历史版本：1.0
* 作    者：wang xindong
* 完成日期：2009年2月12日

* 历史版本：1.1
* 作    者：wang xindong
* 完成日期：2009年3月1日
* 描    述：（1）添加获取8字节随机数APDU命令
*           （2）删除#include <RSA_LIB.h>

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
* 描    述：（1）函数统一调用函数库接口
*           （2）更新DES、3DES处理命令接口

* 历史版本：1.4
* 作    者：wang xindong
* 完成日期：2009年4月26日
* 描    述：（1）添加RSA运算命令接口
*           （2）添加SHA1运算接口

* 历史版本：1.41
* 作    者：wang xindong
* 完成日期：2009年6月26日
* 描    述：（1）添加GB_ECC运算命令接口

* 当前版本：1.43
* 作    者：wang xindong
* 完成日期：2009年8月4日
* 描    述：删除APDU命令处理程序

* 当前版本：1.44
* 作    者：song yadong
* 完成日期：2010年3月10日
* 描    述：USB初始化关闭双buffer
**********************************************************/

#include <CIU96S192UFB.h>
#include "MacoDefine.h"
#include "variableDeclare.h"
#include "FunctionDeclare.h"
#include <memory.h>
#include <APP_LIB.h>

//设备描述符
unsigned char code DeviceDesp[] =
{
    0x12,        // Descriptor length (18 bytes)
    0x01,        // Descriptor type (Device)
    0x10,        // Complies with USB Spec. Release (0110h = 0x release 1.10)
    0x01,
    0x00,        // Class code (0)
    0x00,        // Subclass code (0)
    0x00,        // Protocol (No specific protocol)
    0x10,        // Maximum packet size for Endpoint 0 (16 bytes)
    0x25,        // Vendor ID (Lakeview Research 0925)
    0x09,
    0x34,        // Product ID (1234)
    0x12,
    0x01,        // Device release number (0001)
    0x00,
    0x01,        // Manufacturer string descriptor index
    0x02,        // Product string descriptor index
    0x00,        // Serial Number string descriptor index (None)
    0x01        // Number of possible configurations (1)
};

//配置描述符(配置中断端点)
unsigned char code ConfigDesp_InterEp[] =
{
     0x09,        // Descriptor length (9 bytes)
     0x02,        // Descriptor type (Configuration)
     0x22,        // Total data length (34 bytes)
     0x00,
     0x01,        // Interface supported (1)
     0x01,        // Configuration value (1)
     0x00,        // Index of string descriptor (None)
     0x80,        // Configuration (Bus powered)
     0x32,        // Maximum power consumption (100mA)

//Interface_Descriptor:
     0x09,        // Descriptor length (9 bytes)
     0x04,        // Descriptor type (Interface)
     0x00,        // Number of interface (0)
     0x00,        // Alternate setting (0)
     0x01,        // Number of interface endpoint (1)
     0x03,        // Class code ()
     0x00,        // Subclass code ()
     0x00,        // Protocol code ()
     0x00,        // Index of string()

//HID Class_Descriptor:
     0x09,        // Descriptor length (9 bytes)
     0x21,        // Descriptor type (HID)
     0x00,        // HID class release number (1.00)
     0x01,
     0x00,        // Localized country code (None)
     0x01,        // # of HID class dscrptr to follow (1)
     0x22,        // Report descriptor type (HID)
     0x23,        // Total length of report descriptor
     0x00,

//Endpoint_Descriptor:
     0x07,        // Descriptor length (7 bytes)
     0x05,        // Descriptor type (Endpoint)
     0x82,        // Encoded address (Respond to IN, 2 endpoint)
     0x03,        // Endpoint attribute (Interrupt transfer)
     0x40,        // Maximum packet size (64 bytes)
     0x00,
     0x01		// Polling interval (10 ms)
};

//配置描述符(仅使用控制端点)
unsigned char code ConfigDesp_NoInterEp[] =
{
     0x09,        // Descriptor length (9 bytes)
     0x02,        // Descriptor type (Configuration)
     0x1B,        // Total data length (27 bytes)
     0x00,
     0x01,        // Interface supported (1)
     0x01,        // Configuration value (1)
     0x00,        // Index of string descriptor (None)
     0x80,        // Configuration (Bus powered)
     0x32,        // Maximum power consumption (100mA)

//Interface_Descriptor:
     0x09,        // Descriptor length (9 bytes)
     0x04,        // Descriptor type (Interface)
     0x00,        // Number of interface (0)
     0x00,        // Alternate setting (0)
     0x01,        // Number of interface endpoint (1)
     0x03,        // Class code ()
     0x00,        // Subclass code ()
     0x00,        // Protocol code ()
     0x00,        // Index of string()

//HID Class_Descriptor:
     0x09,        // Descriptor length (9 bytes)
     0x21,        // Descriptor type (HID)
     0x00,        // HID class release number (1.00)
     0x01,
     0x00,        // Localized country code (None)
     0x01,        // # of HID class dscrptr to follow (1)
     0x22,        // Report descriptor type (HID)
     0x23,        // Total data length (27 bytes)
     0x00
};

//字符串描述符(语言)
unsigned char code StrLanguageDesp[] = 
{
    0x04,     //bLength
    0x03,     //bDescriptorType
    0x09,     //wLANGID[0] -> US English
    0x04
};

//字符串描述符(厂商)
unsigned char code StrManuDesp[] =
{
    0x1A,     // Length
    0x03,     // Type (3= 0xstring)
    0x55,     //'U'
    0x00,
    0x53,     //'S'
    0x00,
    0x42,     //'B'
    0x00,
    0x20,     //' '
    0x00,
    0x43,     //'C'
    0x00,
    0x6f,     //'o'
    0x00,
    0x6d,     //'m'
    0x00,
    0x70,     //'p'
    0x00,
    0x6c,     //'l'
    0x00,
    0x65,     //'e'
    0x00,
    0x74,     //'t'
    0x00,
    0x65,     //'e'
    0x00
};

//字符串描述符(产品)
unsigned char code StrProductDesp[] =
{
    0x16,     // Length
    0x03,     // Type (3= 0xstring)
    //"HID Sample"
    0x48,     //'H'
    0x00,
    0x49,     //'I'
    0x00,
    0x44,     //'D'
    0x00,
    0x20,     //' '
    0x00,
    0x53,     //'S'
    0x00,
    0x61,     //'a'
    0x00,
    0x6d,     //'m'
    0x00,
    0x70,     //'p'
    0x00,
    0x6c,     //'l'
    0x00,
    0x65,     //'e'
    0x00
};

//报告描述符(input report/output report)
unsigned char code ReportDesp_InOut[] =
{
     0x06,        //Usage Page (vendor defined FFA0)
     0xA0,
     0xFF,
     0x09,        //Usage (vendor defined)
     0x01,
     0xA1,        //Collection (Application)
     0x01,
        0x06,     //Usage Page (vendor defined FFA1)
        0xA1,
        0xFF,

        //The input report
        0x09,     // usage - vendor defined
        0x03,
        0x15,     //logical minimum (0)
        0x00,
        0x25,     //logical maximum (255)
        0xff,
        0x75,     // Report Size (8)  (bits)
        0x08,
        0x95,     // Report Count (255)  (fields)
        0xff,
        0x81,     // Input (Data, Variable, Absolute)
        0x02,

        //The output report
        0x09,     //usage - vendor defined
        0x04,
        0x15,     //logical minimum (0)
        0x00,
        0x25,     //logical maximum (255)
        0xff,
        0x75,     //Report Size (8)  (bits)
        0x08,
        0x95,     //Report Count (255)  (fields)
        0xff,
        0x91,     //Output (Data, Variable, Absolute)
        0x02,
     0xC0        //End Collection
};

//报告描述符(Feature report)
unsigned char code ReportDesp_Feature[] =
{
     0x06,        //Usage Page (vendor defined FFA0)
     0xA0,
     0xFF,
     0x09,        //Usage (vendor defined)
     0x01,
     0xA1,        //Collection (Application)
     0x01,
        0x06,     //Usage Page (vendor defined FFA1)
        0xA1,
        0xFF,

        0x85,     //report ID (1)
        0x01,
        0x09,     //usage
        0x03,
        0x15,     //logical minimum (0)
        0x00,
        0x25,     //logical maximum (255)
        0xff,
        0x95,     //report count (15)
        0x0f,
        0x75,     //report size (8)  (bits)
        0x08,
        0xb1,     //feature report
        0x02,

        0x85,     //report ID (2)
        0x02,
        0x09,     //usage
        0x04,
        0x15,     //logical minimum (0)
        0x00,
        0x25,     //logical maximum (255)
        0xff,
        0x95,     //report count (255)
        0xff,
        0x75,     //report size (8)  (bits)
        0x08,
        0xb1,     //feature report
        0x02,

     0xC0       //End Collection
};

/****************************************************************************
*   功能：  准备设备描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_device()
{
    unsigned char i;

    for(i = 0;i < 18;i++)
    {
        *(usbptr++) = DeviceDesp[i];
    }
}

/****************************************************************************
*   功能：  准备配置描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_configuration()
{
    unsigned char i;

#ifdef CTRL_OUT_INT_IN
    for(i = 0;i < 0x22;i++)
    {
        *(usbptr++) = ConfigDesp_InterEp[i];
    }
#endif

#ifdef CTRL_OUT_CTRL_IN
    for(i = 0;i < 0x1B;i++)
    {
        *(usbptr++) = ConfigDesp_NoInterEp[i];
    }
#endif

#ifdef CTRL_FEATRUE
    for(i = 0;i < 0x1B;i++)
    {
        *(usbptr++) = ConfigDesp_NoInterEp[i];
    }
#endif
}

/****************************************************************************
*   功能：  准备字符串（语言）描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_string_language()
{
    unsigned char i;

    for(i = 0;i < 4;i++)
    {
        *(usbptr++) = StrLanguageDesp[i];
    }
}

/****************************************************************************
*   功能：  准备字符串（厂商）描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_string_manu()
{
    unsigned char i;

    for(i = 0;i < 0x1a;i++)
    {
        *(usbptr++) = StrManuDesp[i];
    }
}

/****************************************************************************
*   功能：  准备字符串（产品）描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_string_product()
{
    unsigned char i;

    for(i = 0;i < 22;i++)
    {
        *(usbptr++) = StrProductDesp[i];
    }
}

/****************************************************************************
*   功能：  准备报告描述符数据
*   输入：  usbptr，指向描述符输出的起始地址
*   输出：  描述符放置在usbptr指向的地址处
*****************************************************************************/
void des_report_descriptor()
{
    unsigned char i;

#ifdef CTRL_OUT_INT_IN
    for(i = 0;i < 0x23;i++)
    {
        *(usbptr++) = ReportDesp_InOut[i];
    }
#endif

#ifdef CTRL_OUT_CTRL_IN
    for(i = 0;i < 0x23;i++)
    {
        *(usbptr++) = ReportDesp_InOut[i];
    }
#endif

#ifdef CTRL_FEATRUE
    for(i = 0;i < 0x27;i++)
    {
        *(usbptr++) = ReportDesp_Feature[i];
    }
#endif
}

/****************************************************************************
*   功能：  USB中断处理程序
*   输入：  无
*   输出：  无
*****************************************************************************/
void usb_inter() evinterrupt 2 priority 13
{
    EUI = 0;

    USBCMD = _READ_IRQ_REG;
    usbtmp = USBDATA;
    usbtmp2 = USBDATA;

    if(usbtmp_b0)
    {
        usb_conf = 0;
    }
    else if(usbtmp_b1)
    {

    }
    else if(usbtmp_b2)
    {

    }
    else
    {
        if(usbtmp2_b0)
        {
            irq_control_out();
        }
        else if(usbtmp2_b1)
        {
            irq_control_in();
        }

        else if(usbtmp2_b5)
        {
        	USBCMD = _READ_EP2_IN;  //read ep status to clear irq
    		usbtmp = USBDATA;

    		if((usbtmp & 1) && (usbsendlen != 0))
    		{
       			irq_interrupt_in();
       		}
       	}
    }

    EUI = 1;
}

/****************************************************************************
*   功能：  初始化USB参数
*   输入：  无
*   输出：  无
*****************************************************************************/
void init_usb()
{
    EA = 1;
    EUI = 1;

    USBEN = 1;
    HWBRON = 0;

    USBCMD = _SET_MODE;
    USBDATA = 0x0F;

    Ep2InNum = 1;

    GPIOEN = 1;         //enable the GPIO
    GPIODDRL = 0x21;    //config GPIO output
    SPIEN = 1;          //enable the SPI interface
    SPICON = 0x00;      //config spi interface 2div, MSB, mode0, sampling at rising edge of clk

//    SetTimer();       //Set Timer
//    TR1 = 1;          //Open Timer
//    ET1 = 1;

    usb_inflag = 0;
    usb_conf = 0;
    
    usbtmp = 0;
    usbtmp2 = 0;
    usb_state = 0;
    
    usbptrep1 = &com_buf[0];
    received_len = 0;
}

/****************************************************************************
*   功能：  处理控制输出中断
*   输入：  无
*   输出：  无
*****************************************************************************/
void irq_control_out()
{
    USBCMD = _READ_EP0_OUT; 				//read ep status to clear irq
    usbtmp = USBDATA;

    if((usbtmp & 0x20) != 0)                //Deal Setup Transfer
    {   
        USBCMD = _SELECT_EP0_OUT;
        USBCMD = _READ_BUF;
        usbtmp = USBDATA;                  //get data len
        
        if( usbtmp!=_USB_SETUP_TRANSFER_LEN)    //SETUP packet len must be 8 byte
        { 
            stall_ctrl_ep();
            return;
        }
        usbptr = &com_buf[0];
        for( ; usbtmp!=0 ; usbtmp--)            //get setup data to RAM
        {
            *(usbptr++) = USBDATA;
        }
        clear_usb_buf();
        
        if((com_buf[0]&_USB_REQ_TYPE_MASK) == 0x00)
        {//Standard USB Request
           usbtmp = com_buf[1]; //bRequest
           switch (usbtmp)
            {
                case _USB_GET_STATUS:
                    usb_get_status();
                    break;
                case _USB_CLEAR_FEATURE:
                    usb_feature();
                    break;
                case _USB_SET_FEATURE:
                    usb_feature();
                    break;                    
                case _USB_SET_ADDRESS:
                    usb_set_addr();
                    break;
                case _USB_GET_DESCRIPTOR:
                    usb_get_desptor();
                    break;
                case _USB_GET_CONFIGURATION:
                    usb_get_config();
                    break;
                case _USB_SET_CONFIGURATION:
                    usb_set_config();
                    break;
                case _USB_GET_INTERFACE:
                    usb_get_intface();
                    break;
                case _USB_SET_INTERFACE:
                    usb_set_intface();
                    break;
                default:
                    stall_ctrl_ep();
                    break;
            }
        }
        else if((com_buf[0] & _USB_REQ_TYPE_MASK) == 0x20)
        {
            if(com_buf[1] == 0x09)          /* set_report */
			{
                received_len = com_buf[7]*256 + com_buf[6];
                usbptrep1 = recieve_buf;
            }
            else if(com_buf[1] == 0x01)     /* get_report */
            {
                usbsendlen = com_buf[7]*256 + com_buf[6];
                usbDataPtr = &com_buf[8];
                usb_wr_ep();
            }
            else if(com_buf[1] == 0x0a)     /* get_idle */
            {
                send_ctrl_zero();
            }
        }
        else
        {
            stall_ctrl_ep();
        }
    }

    else
    {
        if((usbtmp & 0x01) == 0x01)
        {
            if (received_len != 0)
            {
                USBCMD = _SELECT_EP0_OUT;
                USBCMD = _READ_BUF;
                //get receive data len
                usbtmp =  USBDATA;

                DMASA = 0x4be;
                DMASH = 0;
                DMADA = (INT16U) usbptrep1;
                DMADH = 0;
                DMALEN = usbtmp;
                DMACTRL = 0x08;

                received_len -= usbtmp;
                usbptrep1 += usbtmp;

                USBCMD = _SELECT_EP0_OUT;
                USBCMD = _CLEAR_BUF;

                if (received_len == 0)
                {
                    apdu_ok = 1;
                    send_ctrl_zero();
                }
            }
        }
    }
}

/****************************************************************************
*   功能：  Stall Cotnrol EP OUT and Control EP In
*   输入：  无
*   输出：  无
*****************************************************************************/
void stall_ctrl_ep()
{
    USBCMD = _SET_EP0_OUT;      //something error, stall EP0 to
    USBDATA = _SET_EP_STALL;   //ignore following OUT and IN transfer
    USBCMD = _SET_EP0_IN;
    USBDATA = _SET_EP_STALL;
}

/****************************************************************************
*   功能：  处理clear buffer命令
*   输入：  无
*   输出：  无
*****************************************************************************/
void clear_usb_buf()
{
    USBCMD = _SELECT_EP0_OUT;  //acknowledge Control EP and clear buffer
    USBCMD = _ACK_SETUP;
    USBCMD = _CLEAR_BUF;
    USBCMD = _SELECT_EP0_IN;
    USBCMD = _ACK_SETUP;
}

/****************************************************************************
*   功能：  控制端点发送0包
*   输入：  无
*   输出：  无
*****************************************************************************/
void send_ctrl_zero()
{
    usbsendlen = 0;
    USBCMD = _SELECT_EP0_IN;
    USBCMD = _VALIDATE_BUF;
}

/****************************************************************************
*   功能：  EP号转化为EP逻辑号
*           For EP Out, EP order = ep number * 2
*           For EP In,  EP order = ep number * 2 + 1
*   输入：  usbtmp
*           Low byte of wIndex : EP direction
*   输出：  usbtmp : EP Order
*****************************************************************************/
void get_ep_order()
{
    usbtmp = usbtmp*2;
    usbtmp2 = com_buf[4] &  _USB_EP_DIRECTION_MASK;  //wIndex
    
    if(usbtmp2 != 0)
    {
        usbtmp++;
    }
}

/****************************************************************************
*   功能：  处理Get_Status请求
*   输入：  setup data
*   输出：  无
*****************************************************************************/
void usb_get_status()
{
    usbtmp = com_buf[0]&_USB_REQ_RECIPIENT_MASK;  //bmRequestType
    if(usbtmp==_USB_RECIPIENT_DEVICE)
        {
            usbtmp = _USB_DEVICE_REMOTE_WAKEUP;
            usbtmp2 = 0 ;//ret = 0;
        }
     else if(usbtmp==_USB_RECIPIENT_INTERFACE)
        {
            usbtmp = 0;
            usbtmp2 = 0 ; //ret = 0;
        }
     else if(usbtmp==_USB_RECIPIENT_ENDPOINT)
        {
            usbtmp = com_buf[4]&_USB_EP_NUMBER_MASK;   //wIndex  get EP number
            if(usbtmp>=_SUPPORTED_EP_NUMBER)
                {
                    stall_ctrl_ep();
                    return;
                }
            get_ep_order();
            usbtmp += _SELECT_EP;
            USBCMD = usbtmp;
            usbtmp = USBDATA;
            usbtmp = usbtmp&_USB_EP_STALL_MASK;
                                 
            usbtmp2 = 0;
        }
     else
        {
            stall_ctrl_ep();
            return;
        }
        
     USBCMD = _SELECT_EP0_IN;
     USBCMD = _WRITE_BUF;
     USBDATA = usbtmp;
     USBDATA = usbtmp2;//USBDATA = ret;
     
     usbsendlen = 0;
     USBCMD = _VALIDATE_BUF;
     return;
}

/****************************************************************************
*   功能：  处理Clear_Feature / Set_Feature请求
*   输入：  _usb_buf : setup data
*   输出：  无
*****************************************************************************/
void usb_feature()
{
    usbtmp = com_buf[0]&_USB_REQ_RECIPIENT_MASK;
    if(usbtmp!=_USB_RECIPIENT_ENDPOINT)
        {
            stall_ctrl_ep();
            return;
        }
    USBCMD = _GET_ADDRESS;
    usbtmp = USBDATA;
    if(usbtmp==0) //Address State
        {//Address State , only EP0 is validate
            usbtmp = com_buf[4]&_USB_EP_NUMBER_MASK;
            if(usbtmp)
                {
                    stall_ctrl_ep();
                    return;
                }            
        }
    else
        {
            usbtmp = com_buf[4]&_USB_EP_NUMBER_MASK;
            if(usbtmp>=_SUPPORTED_EP_NUMBER)
                {
                    stall_ctrl_ep();
                    return;
                }
        }
    
    get_ep_order();
    usbtmp = usbtmp | _SET_EP_STATUS;
    USBCMD = usbtmp;
    usbtmp = com_buf[1];
    
    if(usbtmp == _USB_CLEAR_FEATURE)//bRequest is Clear Feature ?
        {
            USBDATA = _CLEAR_EP_STALL;
        }
    else //Set Feature
        {
            USBDATA = _SET_EP_STALL;
        }
    send_ctrl_zero();
    return;
}

/****************************************************************************
*   功能：  处理Set_Address请求
*   输入：  _usb_buf : setup data
*   输出：  无
*****************************************************************************/
void usb_set_addr()
{
    USBCMD = _SET_ADDRESS;
    USBDATA = com_buf[2]|0x80;   //wValue
    send_ctrl_zero();
    return;    
}

/****************************************************************************
*   功能：  处理Get_Interface请求
*   输入：  无
*   输出：  无
*****************************************************************************/
void usb_get_intface()
{
    USBCMD = _SELECT_EP0_IN;
    usb_wr_one_byte(0);
    return;
}

/****************************************************************************
*   功能：  处理Get_Configuration请求
*   输入：  usb_state[conf] : 0 : not configured ; 1 : configured
*   输出：  无
*****************************************************************************/
void usb_get_config()
{
    USBCMD = _SELECT_EP0_IN;
    if(usb_conf) usb_wr_one_byte(1);
    else usb_wr_one_byte(0);
    return;
}

/****************************************************************************
*   功能：  处理Set_Interface请求
*   输入：  _usb_buf : setup data
*   输出：  无
*****************************************************************************/
void usb_set_intface()
{ //only support default interface
    usbtmp = com_buf[2]|com_buf[3]|com_buf[4]|com_buf[5];
    if(usbtmp==0)
        send_ctrl_zero();
    else
        stall_ctrl_ep();
    return;
}

/****************************************************************************
*   功能：  处理Set Configuration请求
*   输入：  _usb_buf : setup data
*   输出：  usb_state[conf] :
*           0 : device is placed in Address state
*           1 : device is placed in Configured state
*****************************************************************************/
void usb_set_config()
{
    usbtmp = com_buf[2]; //wValue
    if(usbtmp == 0)
        {
            USBCMD = _SET_EP_ENABLE;
            USBDATA = _EP_DISABLE;
            usb_conf = 0;
        }
    else
        {
            if(usbtmp==1)
                {
                    USBCMD = _SET_EP_ENABLE;
                    USBDATA = _EP_DISABLE;
                    USBCMD = _SET_EP_ENABLE;
                    USBDATA = _EP_ENABLE;
                    usb_conf = 1;
                }
            else
                {
                    stall_ctrl_ep();
                    return;
                }
            
        }
    send_ctrl_zero();
    return;
}

/****************************************************************************
*   功能：  处理get descriptor请求
*   输入：  setup data
*   输出：  usbSendLen : 返回主机的数据长度
*           usbDespLenH/L : 指向下一个描述符
*****************************************************************************/
void usb_get_desptor()
{
    if(0x22 == com_buf[3])
    {
        des_report_descriptor();

#ifdef CTRL_OUT_INT_IN
                usbsendlen = 0x23;
#endif

#ifdef CTRL_OUT_CTRL_IN
                usbsendlen = 0x23;
#endif

#ifdef CTRL_FEATRUE
                usbsendlen = 0x27;
#endif

    }
    else
    {

    switch(com_buf[3])  //wValue + 1
    {
        case _USB_DESCRIPTOR_DEVICE:
            {
                usbptr = &com_buf[8];
                des_device();
                usbsendlen = 0x12;                
            }
            break;
        case _USB_DESCRIPTOR_CONFIGURATION:
            {
                usbptr = &com_buf[8];
                des_configuration();

#ifdef CTRL_OUT_INT_IN
                usbsendlen = 0x22;
#endif

#ifdef CTRL_OUT_CTRL_IN
                usbsendlen = 0x1B;
#endif

#ifdef CTRL_FEATRUE
                usbsendlen = 0x1B;
#endif

            }
            break;
        case _USB_DESCRIPTOR_STRING:
            {
                usbtmp = com_buf[2];
                usbptr = &com_buf[8];
                if(usbtmp==_USB_DESCRIPTOR_STRING0)
                    {
                        des_string_language();
                        usbsendlen = 0x04;
                    }
                 else if(usbtmp==_USB_DESCRIPTOR_STRING1)
                    {
                        des_string_manu();
                        usbsendlen = 0x1A;
                    }
                 else if(usbtmp==_USB_DESCRIPTOR_STRING2)
                    {
                        des_string_product();
                        usbsendlen = 0x16;
                    }
                 else
                    {
                        stall_ctrl_ep();
                        return;
                    }
            }
            break;

        default:
            stall_ctrl_ep();
            return;    
    }
    }


    
    if((com_buf[7] == 0)&&(com_buf[6]<usbsendlen))
        {
            usbsendlen = com_buf[6];
        }
    usbDataPtr = &com_buf[8];

    usb_wr_ep();
    return;
}

/****************************************************************************
*   功能：  向控制控制buffer中写入一个字节的数据并生效
*   输入：  adata：发送的数据
*   输出：  无
*****************************************************************************/
void usb_wr_one_byte(unsigned char adata)
{
    USBCMD = _WRITE_BUF;
    USBDATA = adata;
    usbsendlen = 0;
    USBCMD = _VALIDATE_BUF;
    return;
}

/****************************************************************************
*   功能：  通过控制通道发送数据
*   输入：  usbsendlen : 发送数据长度
*           USBDATAPtr : 发送数据的起始地址
*   输出：  USBDATAPtr : 发送完成后指向下一个数据
*           usbsendlen : 剩余数据长度
*****************************************************************************/
void usb_wr_ep()
{
    USBCMD = _SELECT_EP0_IN;
    if(usbsendlen==0)
    {
        send_ctrl_zero();
    }
    else
    {
        if(usbsendlen < 17)
        {
            usbtmp = (INT8U)usbsendlen;
        }
        else
        {
            usbtmp = 16;
        }
            
        usbsendlen = usbsendlen - usbtmp;
            
        USBCMD = _WRITE_BUF;
            
        for( ; usbtmp!=0 ; usbtmp--)
        {
            USBDATA = *usbDataPtr;
                
            usbDataPtr++;
        }
            
        USBCMD = _VALIDATE_BUF;
    }
}
	
/****************************************************************************
*   功能：  处理控制通道的IN包中断
*   输入：  usbsendlen : 发送数据长度
*           USBDATAPtr : 发送数据的起始地址
*   输出：  USBDATAPtr : 发送完成后指向下一个数据
*           usbsendlen : 剩余数据长度
*****************************************************************************/
void irq_control_in()
{
    USBCMD = _READ_EP0_IN;  //read ep status to clear irq
    usbtmp = USBDATA;

    if(apdu_ok)
    {
        apdu_ok = 0;
        cmdAPDU();
    }
    else
    {
        if(usbtmp_b0)
        {
            USBCMD = _SELECT_EP0_IN;
            USBDATA = _CLEAR_BUF;
            usb_wr_ep();
        }
    }

}

/****************************************************************************
*   功能：  处理中断通道的IN包中断
*   输入：  usbsendlen : 发送数据长度
*           USBDATAPtr : 发送数据的起始地址
*   输出：  USBDATAPtr : 发送完成后指向下一个数据
*           usbsendlen : 剩余数据长度
*****************************************************************************/
void irq_interrupt_in()
{
    unsigned char i;
    unsigned char ep2status;

    for(i = 0; i < Ep2InNum; i++)
    {
        USBCMD = _SELECT_EP2_IN;
        ep2status = USBDATA;

        if((ep2status & 0x02) == 0x02)      //EP2 is empty?
        //USBCMD = _CLEAR_BUF;  //delete 20081209
        {

            if(usbsendlen < 64)
            {
                usbtmp = (INT8U)usbsendlen;
            }
            else
            {
                usbtmp = 64;
            }

            usbsendlen = usbsendlen - usbtmp;

            USBCMD = _WRITE_BUF;

            DMASA = (unsigned int)usbDataPtr;
            DMASH = 0;
            DMADA = 0x4be;
            DMADH = 0;
            DMALEN = usbtmp;
            DMACTRL = 0x04;
            usbDataPtr += usbtmp;

            USBCMD = _VALIDATE_BUF;

            if(usbsendlen == 0)
            {
                break;
            }
        }
    }
}

/****************************************************************************
*   功能：  USB APDU命令处理
*   输入：  recieve_buf[]：接收的数据
*   输出：  无
*****************************************************************************/
void cmdAPDU(void)
{
    DealAPDU();

    send_buf[0] = (unsigned char)(USB_len >> 8);
    send_buf[1] = (unsigned char)USB_len;
    send_buf[USB_len] = SW1;
    send_buf[USB_len + 1] = SW2;

    usbDataPtr = send_buf;
    usbsendlen = 255;
    irq_interrupt_in();
}
