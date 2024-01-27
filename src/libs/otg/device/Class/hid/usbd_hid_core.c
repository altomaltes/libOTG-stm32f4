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
#include "usb_hcd.h"
#include "usb_dcd.h"

#include "usbd_hid_core.h"
#include "usb_hid.h"

/* USB HID device Configuration Descriptor
 */
//ALIGN_THIS( static byte USBD_HID_CfgDesc[ USB_HID_CONFIG_DESC_SIZ ] ) =
//{ 0x09                              /* bLength: Configuration Descriptor size */
//, USB_CONFIGURATION_DESCRIPTOR_TYPE /* bDescriptorType: Configuration */
//, USB_HID_CONFIG_DESC_SIZ, 0x00           /* wTotalLength: Bytes returned  */
//, 0x01         /* bNumInterfaces: 1 interface*/
//, 0x01         /* bConfigurationValue: Configuration value*/
//, 0x00         /* iConfiguration: Index of string descriptor describing the configuration*/
//, 0xE0         /* bmAttributes: bus powered and Support Remote Wake-up */
//, 0x32         /* MaxPower 100 mA: this current is used for detecting Vbus*/


const struct
{	usbInterfaceDesc HIDiface;
  usbHidDescriptor HIDdesc;
  usbEndpointDesc  HIDep;
  byte             EOF;
}
PACKED hidConfDesc=
{.HIDiface=
 { .bLength= sizeof( usbInterfaceDesc )           /** Size of the descriptor, in bytes.*/
 , .bType             = DTYPE_INTERFACE           /** Interface descriptor.*/
 , .bInterfaceNumber  = 0                         /** Index of the interface in the current configuration.*/
 , .bAlternateSetting = 0                         /** Alternate setting for the interface number.*/
 , .bNumEndpoints     = 1                         /** Total number of endpoints in the interface.*/
 , .bInterfaceClass   = USB_CLASS_HID             /** Interface class ID. (3) */
 , .bInterfaceSubClass= USB_HID_SUBCLASS_BOOT     /** Interface subclass ID.*/
 , .bInterfaceProtocol= USB_HID_PROTO_MOUSE       /** Interface protocol ID. */
 , .iInterface        = NO_DESCRIPTOR             /** Index of the string descriptor describing the interface. */
 }

,.HIDdesc=
 { .bLength= sizeof( usbHidDescriptor )            /** Size of the descriptor, in bytes. */
 , .bType             = HID_DTYPE_HID              /** Type of the descriptor, set to \ref DTYPE_HID */
 , .bcdHID            = VERSION_BCD(2,0,0)         /** BCD encoded version that the HID descriptor and device complies to. */
 , .bCountryCode      = 0                          /** Country code of the localized device, or zero if universal. */
 , .bNumDescriptors   = 1                          /** Total number of HID report descriptors for the interface. */
 , .bType0            = HID_DTYPE_REPORT           /** 1'st HID report descriptor type, set to \ref DTYPE_HID_REPORT */
 , .wDescriptorLength0= HID_MOUSE_REPORT_DESC_SIZE /** 1'sr HID report descriptor length in bytes. */
 }

,.HIDep=
 { .bLength= sizeof( usbEndpointDesc )             /** Size of the descriptor, in bytes. */
 , .bType           =  DTYPE_ENDPOINT              /** Endpoint descriptor.*/
 , .bEndpointAddress=  EPDIR_IN + 1                /** Logical address of the endpoint within the device for the current configuration, including direction mask. */
 , .bmAttributes    =  EPTYPE_INTERRUPT            /** Endpoint bmAttributes, \ref USB_ENDPOINT_DEF. */
 , .wMaxPacketSize  =  HID_IN_PACKET               /** ?? Size of the endpoint bank, in bytes. This indicates the maximum packet size that the endpoint can receive at a time. */
 , .bInterval       =  HID_FS_BINTERVAL            /** ?? Polling interval in milliseconds for the endpoint ifit is an INTERRUPT or ISOCHRONOUS type.*/
 } };


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
/* USB HID device Configuration Descriptor
 */
ALIGN_THIS( static byte USBD_HID_Desc[ USB_HID_DESC_SIZ ] )= /* 18 */
{ 0x09                 /*bLength: HID Descriptor size*/
, HID_DTYPE_HID        /*bDescriptorType: HID*/
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
static short USBD_HID_Setup( USB_SETUP_REQ * req
                           , DeviceListRec * dList )
{ struct USBDdriverHidRec * hnd= (struct  USBDdriverHidRec *)dList->doArgs;

  word   len= 0;
  byte * pbuf= NULL;

  switch ( req->bmRequest & USB_REQ_TYPE_MASK )  /* Class request */
  { case USB_REQ_TYPE_CLASS: switch( req->bRequest )
    { case HID_SETPROTOCOL:
         hnd->USBD_HID_Protocol= ( byte )(req->wValue);
      break;

      case HID_GETPROTOCOL:
         USBDctlSendData( &hnd->USBD_HID_Protocol, 1 );
      break;

      case HID_SETIDLE:
         hnd->USBD_HID_IdleState= ( byte )(req->wValue >> 8);
      break;

      case HID_GETIDLE:
         USBDctlSendData( &hnd->USBD_HID_IdleState, 1 );
      break;

      default:
        USBDctlError( req );
      return( -1 );
    }
    break;

    case USB_REQ_TYPE_STANDARD: switch( req->bRequest )  /* Interface & Endpoint request */
    { case USB_REQ_GET_DESCRIPTOR: switch( req->wValue >> 8 )
      { case HID_DTYPE_REPORT:
          pbuf= HID_MOUSE_ReportDesc;
          len = MIN( HID_MOUSE_REPORT_DESC_SIZE, req->wLength );
        break;

        case HID_DTYPE_HID:
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
          pbuf= USBD_HID_Desc;
#else
          pbuf= &hidConfDesc.HIDdesc;  // USBD_HID_CfgDesc + 0x12;
#endif
          len= MIN( USB_HID_DESC_SIZ, req->wLength );
        break;

        default: /* Do Nothing */
        break;
      }
      USBDctlSendData( pbuf, len );
      break;

      case USB_REQ_GET_INTERFACE: USBDctlSendData ( &hnd->USBD_HID_AltSet, 1 ); break;
      case USB_REQ_SET_INTERFACE: hnd->USBD_HID_AltSet= (byte)(req->wValue);    break;
      default                   : hnd->USBD_HID_AltSet= (byte)(req->wValue);    break;
    }
    break;

    default: USBDctlSendData( &hnd->USBD_HID_AltSet, 1);   break;
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
short USBD_HID_DataIn( dword frame, void * args )
{ struct USBDdriverHidRec * drv= (struct USBDdriverHidRec *)args;

  USBDepFlush( frame );

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
static short USBD_HID_Crtl( word what, word cfgIdx
                          , DeviceListRec * dList )     /* EP used by this device */
{ switch( what )
  { case ACTION_INIT:         /* Open EP IN */
      USBDepOpen( dList->epIn //  HID_IN_EP
                , HID_IN_PACKET
                , EPTYPE_INTERRUPT
                , USBD_HID_DataIn );
    break;

    case ACTION_DEINIT:       /* Close HID EPs */
       USBDepClose( dList->epIn );
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
, &hidConfDesc
};

