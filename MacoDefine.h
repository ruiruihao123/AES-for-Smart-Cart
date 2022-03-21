/*******************************************************
* Copyright (c) 2009-2011 HED
* All rights reserved.
*
* 文件名称：MacoDefine.h
* 文件标识：
* 摘    要：定义工程中使用的宏
*
* 当前版本：1.00
* 作    者：wang xindong
* 完成日期：2009年2月12日
**********************************************************/

#ifndef Maco
#define Maco

#define CTRL_OUT_INT_IN
//#define CTRL_OUT_CTRL_IN
//#define CTRL_FEATRUE

#define HID_COMMAND 2
#define OUT_PACKAGE 1
#define USER_DEFINED_COMMAND 3

#define SELF_COMMAND 1
#define ERR_COMMAND 4
//define offset of Setup Data
#define	bmRequestType   0
#define	bRequest        1
#define	wValue          2
#define	wIndex          4
#define	wLength         6

//define Supported EP
#define USB_EP0_OUT     0
#define USB_EP0_IN      1
#define USB_EP1_OUT     2
#define USB_EP1_IN      3
#define USB_EP2_OUT     4
#define USB_EP2_IN      5

//USB Controller Command Definition
#define     _SELECT_EP              0x00
#define 	_SELECT_EP0_OUT			0x00
#define 	_SELECT_EP0_IN			0x01
#define	    _SELECT_EP1_OUT			0x02
#define 	_SELECT_EP1_IN			0x03
#define 	_SELECT_EP2_OUT			0x04
#define 	_SELECT_EP2_IN			0x05

#define     _READ_EP                0x40
#define 	_READ_EP0_OUT       	0x40
#define 	_READ_EP0_IN        	0x41
#define 	_READ_EP1_OUT       	0x42
#define 	_READ_EP1_IN        	0x43
#define 	_READ_EP2_OUT       	0x44
#define 	_READ_EP2_IN        	0x45

#define	    _SET_EP_STATUS			0x40
#define 	_SET_EP0_OUT			0x40
#define 	_SET_EP0_IN         	0x41
#define	    _SET_EP1_OUT        	0x42
#define 	_SET_EP1_IN         	0x43
#define 	_SET_EP2_OUT        	0x44
#define 	_SET_EP2_IN         	0x45
                          
#define 	_READ_BUF     			0x80
#define 	_WRITE_BUF     			0x80
#define 	_VALIDATE_BUF       	0x81
#define 	_CLEAR_BUF              0x82
#define 	_ACK_SETUP    			0x83
  
#define 	_SET_MODE           	0xC0
#define 	_SET_ADDRESS        	0xC1
#define	    _GET_ADDRESS			0xC1
#define 	_SET_EP_ENABLE     		0xC2
#define 	_READ_IRQ_REG       	0xC3
#define 	_SEND_RESUME        	0xC4
#define 	_READ_USB_STATUS    	0xC5

//definition of the supported endpoints
#define	    _SUPPORTED_EP_NUMBER	0x03
#define 	_USB_EP_CONTROL_OUT     0x00
#define 	_USB_EP_CONTROL_IN      0x01
#define 	_USB_EP_INTERRUPT_OUT   0x02
#define 	_USB_EP_INTERRUPT_IN    0x03

//define descriptors len
#define	    _USB_DESP_DEVICE_LEN 	0x12
#define 	_USB_DESP_CONF_LEN   	0x29
#define 	_USB_STR_LANGUAGE_LEN           0x04
#define 	_USB_STR_MANUFACTURE_LEN        0x0A
#define 	_USB_STR_PRODUCT_LEN            0x1E
//def	_USB_STR_AUTHOR_LEN             #24h

//definition of USB Standard Request Code
#define 	_USB_GET_STATUS			0x00
#define 	_USB_CLEAR_FEATURE		0x01
#define 	_USB_SET_FEATURE		0x03
#define 	_USB_SET_ADDRESS		0x05
#define 	_USB_GET_DESCRIPTOR		0x06
#define 	_USB_SET_DESCRIPTOR		0x07
#define 	_USB_GET_CONFIGURATION	0x08
#define 	_USB_SET_CONFIGURATION	0x09
#define 	_USB_GET_INTERFACE		0x0a
#define 	_USB_SET_INTERFACE		0x0b

//definition of USB Hid Request Code
#define _USB_HID_GET_REPORT     0x01
#define _USB_HID_SET_REPORT     0x09
#define _USB_HID_SET_IDLE       0x0A

//definition of size of Setup Data
#define	_USB_SETUP_TRANSFER_LEN		8

//definition bit fields of bmRequestType 
#define _USB_REQ_TYPE_MASK		    0x60
#define _USB_REQ_TYPE_STANDARD		0x00
#define _USB_REQ_TYPE_CLASS         0x20
#define _USB_REQ_TYPE_VENDOR        0x40

#define _USB_REQ_RECIPIENT_MASK		0x1f
#define _USB_RECIPIENT_DEVICE		0x00
#define _USB_RECIPIENT_INTERFACE	0x01
#define	_USB_RECIPIENT_ENDPOINT		0x02

//define of device status
#define _USB_DEVICE_SELF_POWERED    0x01
#define _USB_DEVICE_REMOTE_WAKEUP   0x02

//define bit fields of wIndex
#define	_USB_EP_DIRECTION_MASK		0x80
#define	_USB_EP_NUMBER_MASK		    0x0f

//define Descriptor Types
#define _USB_DESCRIPTOR_DEVICE          0x01
#define _USB_DESCRIPTOR_CONFIGURATION   0x02
#define _USB_DESCRIPTOR_STRING          0x03
#define _USB_DESCRIPTOR_INTERFACE       0x04
#define _USB_DESCRIPTOR_ENDPOINT        0x05
#define _USB_DESCRIPTOR_HID_REPORT      0x22

//define String Index
#define	_USB_DESCRIPTOR_STRING0		0x00
#define	_USB_DESCRIPTOR_STRING1		0x01
#define	_USB_DESCRIPTOR_STRING2		0x02
#define	_USB_DESCRIPTOR_STRING3		0x03

//bit define of EP Status returned by Select EP Command
#define	_USB_EP_STALL_MASK		0x01
#define	_USB_EP_FULL_MASK		0x02
 
//define input data to USB Controller 
#define	_SET_EP_STALL			0x01
#define	_CLEAR_EP_STALL			0x00
#define _EP_ENABLE              0x01
#define _EP_DISABLE             0x00

//define error code of transfer
#define	_ERROR_CODE_MASK		0x1eh
#define	_ERROR_CODE_NAK			0x14h
#define	_ERROR_CODE_STALL		0x16h

//Bit definition of USB IRQ Register1
#define	BUSRST		0
#define	SUSPEND		1
#define	RESUME		2

//Bit definition of USB IRQ Register2
#define	EP0_OUT		0
#define	EP0_IN		1
#define	EP1_OUT		2
#define	EP1_IN		3
#define	EP2_OUT		4
#define	EP2_IN		5

//Bit definition of EP transfer status
#define	EP_OK		0
#define	EP_SETUP	5
#define	EP_TOGGLE	6
#define	EP_NOT_RD	7

#define REPEAT_TIME 5
#define SEND_OK     0
#define SEND_ERR    1


#define APDU_CLA     recieve_buf[1]
#define APDU_INS     recieve_buf[2]
#define APDU_P1      recieve_buf[3]
#define APDU_P2      recieve_buf[4]
#define APDU_P3      recieve_buf[5]

#define     W25X20_BLOCK                0x0003FFFF
#define     W25X40_BLOCK                0x0007FFFF

#define     spi_SS_PIN                  0x20
#define     spi_SS_DRIVE                0x20
#define     SPI_MODE_RECV               0x01
#define     SPI_MODE_SEND               0x00

#define     EXECUTE_OK                  0x00
#define     ERR_ADDRESS                 0x01

#define     ReleaseDevice               0x01
#define     ReadDeviceID                0x02
#define     ReleaseDevice_ReadDeviceID  0x03

#endif
