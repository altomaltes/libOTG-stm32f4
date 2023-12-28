/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below:
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 *
 * @file usbdef.h
 *
 * original draft from MCD Application Team
 */

#ifndef  USB_DEF_COMMON_H
#define  USB_DEF_COMMON_H

#include "usb_std.h"

/* JACS, from regs
 */
#define USB_OTG_MAX_TX_FIFOS        12 //( HS -> 12 FS -> 8 )
#define USB_OTG_MAX_EP0_SIZE        64


#define FALSE 0
#define TRUE  (!FALSE)

#define  LE16( addr ) (((word)(*((byte *)(addr))))  + (((word)(*(((byte *)(addr)) + 1))) << 8))

enum
{ USB_OTG_EP0_IDLE= 0
, USB_OTG_EP0_SETUP
, USB_OTG_EP0_DATA_IN
, USB_OTG_EP0_DATA_OUT
, USB_OTG_EP0_STATUS_IN
, USB_OTG_EP0_STATUS_OUT
};


/* bmRequestType :D7 Data Phase Transfer Direction
 */
//#define  USB_REQ_DIR_MASK   0x80
//#define  USB_H2D            0x00
//#define  USB_D2H            0x80

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */

enum
{ USB_REQ_GET_STATUS       = 0x00
, USB_REQ_CLEAR_FEATURE    = 0x01
, USB_REQ_SET_FEATURE      = 0x03

, USB_REQ_SET_ADDRESS      = 0x05
, USB_REQ_GET_DESCRIPTOR   = 0x06
, USB_REQ_SET_DESCRIPTOR   = 0x07
, USB_REQ_GET_CONFIGURATION= 0x08
, USB_REQ_SET_CONFIGURATION= 0x09
, USB_REQ_GET_INTERFACE    = 0x0A
, USB_REQ_SET_INTERFACE    = 0x0B
, USB_REQ_SYNCH_FRAME      = 0x0C
};

/* As per USB specs 9.2.6.4 :Standard request with data request timeout: 5sec
   Standard request with no data stage timeout : 50ms
 */
#define DATA_STAGE_TIMEOUT  5000
#define NODATA_STAGE_TIMEOUT  50


/*  This Union is copied from usb-core.h
 */
typedef union
{ word w;
  struct BW
  { byte msb;
    byte lsb;
  }
  bw;
}
uint_16_8_t;


typedef union
{ byte d8[ 8 ];

  struct _SetupPkt_Struc
  { byte         bmRequestType;
    byte         bRequest;
    uint_16_8_t  wValue;
    uint_16_8_t  wIndex;
    uint_16_8_t  wLength;
  } b;
}
USB_Setup_TypeDef;

typedef  struct
{ byte  bLength;
  byte  bDescriptorType;
}
USBH_DescHeader_t;

typedef struct
{ byte  bLength;
  byte  bDescriptorType;
  word  bcdUSB;              /* USB Specification Number which device complies too */
  byte  bDeviceClass;
  byte  bDeviceSubClass;
  byte  bDeviceProtocol;

/* If equal to Zero, each interface specifies its own class code if equal to 0xFF
 * , the class code is vendor specified. Otherwise field is valid Class Code.
 */

  byte  bMaxPacketSize;
  word  idVendor;            /* Vendor ID (Assigned by USB Org) */
  word  idProduct;           /* Product ID (Assigned by Manufacturer) */
  word  bcdDevice;           /* Device Release Number */
  byte  iManufacturer;      /* Index of Manufacturer String Descriptor */
  byte  iProduct;           /* Index of Product String Descriptor */
  byte  iSerialNumber;      /* Index of Serial Number String Descriptor */
  byte  bNumConfigurations; /* Number of Possible Configurations */
}
USBH_DevDesc_TypeDef;


typedef struct
{ byte   bLength;
  byte   bDescriptorType;
  word  wTotalLength;         /* Total Length of Data Returned */
  byte   bNumInterfaces;      /* Number of Interfaces */
  byte   bConfigurationValue; /* Value to use as an argument to select this configuration*/
  byte   iConfiguration;      /*Index of String Descriptor Describing this configuration */
  byte   bmAttributes;        /* D7 Bus Powered , D6 Self Powered, D5 Remote Wakeup , D4..0 Reserved (0)*/
  byte   bMaxPower;           /*Maximum Power Consumption */
}
USBH_CfgDesc_TypeDef;



/* JACS, hard agnostic
 */


#endif


