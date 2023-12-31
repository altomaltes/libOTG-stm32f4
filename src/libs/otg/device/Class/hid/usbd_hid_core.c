/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 *
 * @file usbd_hid_core.c
 *
 * original draft from MCD Application Team
 *
 *          ===================================================================
 *                                HID Class  Description
 *          ===================================================================
 *           This module manages the HID class V1.11 following the "Device Class Definition
 *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
 *           This driver implements the following aspects of the specification:
 *             - The Boot Interface Subclass
 *             - The Mouse protocol
 *             - Usage Page : Generic Desktop
 *             - Usage : Joystick)
 *             - Collection : Application
 *
 * @note     In HS mode and when the DMA is used, all variables and data structures
 *           dealing with the DMA during the transaction process should be 32-bit aligned.
 */

#include "usbd_core.h"
#include "usbd_hid_core.h"

ALIGN_THIS( static dword  USBD_HID_AltSet    )= 0;
ALIGN_THIS( static dword  USBD_HID_Protocol  )= 0;
ALIGN_THIS( static dword  USBD_HID_IdleState )= 0;

/* USB HID device Configuration Descriptor */
ALIGN_THIS( static byte USBD_HID_CfgDesc[ USB_HID_CONFIG_DESC_SIZ ] ) =
{ 0x09                              /* bLength: Configuration Descriptor size */
, USB_CONFIGURATION_DESCRIPTOR_TYPE /* bDescriptorType: Configuration */
, USB_HID_CONFIG_DESC_SIZ           /* wTotalLength: Bytes returned  */
, 0x00
, 0x01         /*bNumInterfaces: 1 interface*/
, 0x01         /*bConfigurationValue: Configuration value*/
, 0x00         /*iConfiguration: Index of string descriptor describing the configuration*/
, 0xE0         /*bmAttributes: bus powered and Support Remote Wake-up */
, 0x32         /*MaxPower 100 mA: this current is used for detecting Vbus*/

/************** Descriptor of Joystick Mouse interface ****************/ /* 09 */

, 0x09         /*bLength: Interface Descriptor size*/
, USB_INTERFACE_DESCRIPTOR_TYPE /*bDescriptorType: Interface descriptor type*/
, 0x00         /*bInterfaceNumber: Number of Interface*/
, 0x00         /*bAlternateSetting: Alternate setting*/
, 0x01         /*bNumEndpoints*/
, 0x03         /*bInterfaceClass: HID*/
, 0x01         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
, 0x02         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
, 0            /*iInterface: Index of string descriptor*/

  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
,  0x09                /* bLength: HID Descriptor size*/
,  HID_DESCRIPTOR_TYPE /* bDescriptorType: HID*/
,  0x11                /* bcdHID: HID Class Spec release number*/
,  0x01
,  0x00                /* bCountryCode: Hardware target country*/
,  0x01                /*bNumDescriptors: Number of HID class descriptors to follow*/
,  0x22                       /*bDescriptorType*/
,  HID_MOUSE_REPORT_DESC_SIZE /*wItemLength: Total length of Report descriptor*/
,  0x00

  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
,  0x07                          /*bLength: Endpoint Descriptor size*/
,  USB_ENDPOINT_DESCRIPTOR_TYPE  /*bDescriptorType:*/

,  HID_IN_EP         /* bEndpointAddress: Endpoint Address (IN)*/
,  0x03              /* bmAttributes: Interrupt endpoint*/
,  HID_IN_PACKET     /* wMaxPacketSize: 4 Byte max */
,  0x00
,  HID_FS_BINTERVAL  /* bInterval: Polling Interval (10 ms)*/
  /* 34 */
} ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
/* USB HID device Configuration Descriptor
 */
ALIGN_THIS( static byte USBD_HID_Desc[ USB_HID_DESC_SIZ ] )= /* 18 */
{ 0x09                 /*bLength: HID Descriptor size*/
, HID_DESCRIPTOR_TYPE  /*bDescriptorType: HID*/
, 0x11                 /*bcdHID: HID Class Spec release number*/
, 0x01
, 0x00                 /* bCountryCode: Hardware target country*/
, 0x01                 /* bNumDescriptors: Number of HID class descriptors to follow*/
, 0x22                 /* bDescriptorType*/
, HID_MOUSE_REPORT_DESC_SIZE /*wItemLength: Total length of Report descriptor*/
, 0x00,
};
#endif

ALIGN_THIS( static byte HID_MOUSE_ReportDesc[ HID_MOUSE_REPORT_DESC_SIZE ] ) =
{ 0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01
, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03
, 0x15, 0x00, 0x25, 0x01, 0x95, 0x03, 0x75, 0x01
, 0x81, 0x02, 0x95, 0x01, 0x75, 0x05, 0x81, 0x01
, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x38
, 0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x03
, 0x81, 0x06, 0xC0, 0x09, 0x3c, 0x05, 0xff, 0x09
, 0x01, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95
, 0x02, 0xb1, 0x22, 0x75, 0x06, 0x95, 0x01, 0xb1
, 0x01, 0xc0
};


/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  req: usb requests
  * @retval status
  */
static schar USBD_HID_Setup( USB_SETUP_REQ * req )
{ word len = 0;
  byte * pbuf = NULL;

  switch ( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS: switch (req->bRequest)
    { case HID_SETPROTOCOL: USBD_HID_Protocol = (byte  )(req->wValue);            break;
      case HID_GETPROTOCOL: USBDctlSendData(   (byte *)&USBD_HID_Protocol, 1 );  break;

      case HID_SETIDLE:     USBD_HID_IdleState= (byte  )(req->wValue >> 8);       break;
      case HID_GETIDLE:     USBDctlSendData(   (byte *)&USBD_HID_IdleState, 1 ); break;

      default:              USBDctlError( req );      return( -1 );
    }
    break;

    case USB_REQ_TYPE_STANDARD: switch( req->bRequest )
    { case USB_REQ_GET_DESCRIPTOR:
        if ( req->wValue >> 8 == HID_REPORT_DESC )
        { len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
          pbuf = HID_MOUSE_ReportDesc;
        }

        else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE )
        {

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
          pbuf= USBD_HID_Desc;
#else
          pbuf= USBD_HID_CfgDesc + 0x12;
#endif
          len= MIN(USB_HID_DESC_SIZ , req->wLength);
        }

        else  /* Do Nothing */
        {
        }

        USBDctlSendData( pbuf
                        , len );

      break;

      case USB_REQ_GET_INTERFACE:
         USBDctlSendData ((byte *)&USBD_HID_AltSet, 1);
      break;

      case USB_REQ_SET_INTERFACE:
        USBD_HID_AltSet= (byte)(req->wValue);
      break;

      default:
        USBD_HID_AltSet= (byte)(req->wValue);
      break;
    }
    break;

    default: USBDctlSendData((byte *)&USBD_HID_AltSet, 1);   break;
  }

  return( 0 );
}

/**
  * @brief  USBD_HID_SendReport
  *         Send HID Report
  * @param  buff: pointer to report
  * @retval status
  */
schar USBD_HID_SendReport( schar * report, word len )
{ if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
  { USBDepTx( HID_IN_EP, report, len );
  }

  return( 0 );
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  epnum: endpoint index
  * @retval status
  *
  * Ensure that the FIFO is empty before a new transfer, this condition could
  * be caused by  a new transfer before the end of the previous transfer
  */
schar USBD_HID_DataIn( word frame )
{ if ( frame == 0xFFFF )
  { USBDepFlush( HID_IN_EP );
  }
  else
  { HIDpoll( frame );
  }

  return( 0 );
}

/**
  * @brief  USBD_HID_GetPollingInterval
  *         return polling interval from endpoint descriptor
  * @retval polling interval
  */
dword USBD_HID_GetPollingInterval()
{ dword pollingInterval = 0;


/*   Sets the data transfer polling interval for high speed transfers.
 * Values between 1..16 are allowed. Values correspond to interval
 * of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL
 */
  if ( OTGgetCoreSpeed() == USB_OTG_SPEED_HIGH )   /* HIGH-speed endpoints */
  { pollingInterval= (((1 <<(HID_HS_BINTERVAL - 1)))/8);
  }
  else                                              /* LOW and FULL-speed endpoints */
  { pollingInterval=  HID_FS_BINTERVAL;            /* Sets the data transfer polling interval for low and full speed transfers */
  }

  return ((dword)(pollingInterval));
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  cfgidx: Configuration index
  * @retval status
  */
static schar  USBD_HID_Crtl( byte what, word cfgidx )
{ switch( what )
  { case ACTION_INIT:         /* Open EP IN */
      USBDepOpen( HID_IN_EP
                 , HID_IN_PACKET
                 , EPTYPE_INTERRUPT
                 , USBD_HID_DataIn );
      USBDepTimer( HID_IN_EP, 2 );
    break;

    case ACTION_DEINIT:       /* Close HID EPs */
       USBDepClose( HID_IN_EP );
    break;

    case ACTION_ISO_INICOM:
    case ACTION_ISO_OUTICOM:
    break;
  }

  return( 0 );
}

/**
  * @brief  USBD_HID_cb
  *         return configuration descriptor
  */
USBDclassDefREC USBD_HID_cb=
{ USBD_HID_Crtl
, USBD_HID_Setup
, USBD_HID_CfgDesc
};

