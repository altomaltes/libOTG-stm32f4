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
 * @file usbd_cdc_core.c
 *
 */

#include "usbd_cdc_core.h"

#define USB_CDC_IDLE  0
#define USB_CDC_BUSY  1
#define USB_CDC_ZLP   2

#define CDC_NOTI_EPSIZE  8		/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define CDC_TXRX_EPSIZE 64 	/** Size in bytes of the CDC data IN and OUT endpoints. */

extern CDC_IF_Prop_TypeDef APP_FOPS;

ALIGN_THIS( byte CmdBuff[ CDC_CMD_PACKET_SIZE ] );

ALIGN_THIS( static volatile dword  usbd_cdc_AltSet )= 0;

dword APP_Rx_ptr_in = 0;
dword APP_Rx_ptr_out= 0;
dword APP_Rx_length = 0;

byte  USB_Tx_State = USB_CDC_IDLE;

static dword cdcCmd = 0xFF;
static dword cdcLen = 0;

/* USB CDC device Configuration Descriptor */
ALIGN_THIS( byte USBD_CDCcfgDesc[ USB_CDC_CONFIG_DESC_SIZ ] )=
{
/* Configuration Descriptor
 */
   0x09                        /* bLength: Configuration Descriptor size */
,  DTYPE_CONFIGURATION /* bDescriptorType: Configuration */
,  USB_CDC_CONFIG_DESC_SIZ     /* wTotalLength:no of returned bytes */
,  0x00
,  0x02   /* bNumInterfaces: 2 interface */
,  0x01   /* bConfigurationValue: Configuration value */
,  0x00   /* iConfiguration: Index of string descriptor describing the configuration */
,  0xC0   /* bmAttributes: self powered */
,  0x32   /* MaxPower 0 mA */

/* Interface Descriptor ^ 9
 */
, 0x09                           /* bLength: Interface Descriptor size */
, USB_INTERFACE_DESCRIPTOR_TYPE  /* Interface descriptor type */
, 0x00                           /* bInterfaceNumber: Number of Interface */
, 0x00                           /* bAlternateSetting: Alternate setting */
, 0x01                           /* bNumEndpoints: One endpoints used */
, 0x02   /* bInterfaceClass: Communication Interface Class */
, 0x02   /* bInterfaceSubClass: Abstract Control Model */
, 0x01   /* bInterfaceProtocol: Common AT commands */
, 0x00   /* iInterface: */

/* Header Functional Descriptor ^ 18
 */
, 0x05   /* bLength: Endpoint Descriptor size */
, 0x24   /* bDescriptorType: CS_INTERFACE */
, 0x00   /* bDescriptorSubtype: Header Func Desc */
, 0x10   /* bcdCDC: spec release number */
, 0x01

/* Call Management Functional Descriptor ^ 23
 */

, 0x05   /* bFunctionLength */
, 0x24   /* bDescriptorType: CS_INTERFACE */
, 0x01   /* bDescriptorSubtype: Call Management Func Desc */
, 0x00   /* bmCapabilities: D0+D1 */
, 0x01   /* bDataInterface: 1 */

/* ACM Functional Descriptor ^ 28
 */
, 0x04   /* bFunctionLength */
, 0x24   /* bDescriptorType: CS_INTERFACE */
, 0x02   /* bDescriptorSubtype: Abstract Control Management desc */
, 0x02   /* bmCapabilities */

/* Union Functional Descriptor ^ 32
 */
, 0x05  /* bFunctionLength */
, 0x24  /* bDescriptorType: CS_INTERFACE */
, 0x06  /* bDescriptorSubtype: Union func desc */
, 0x00  /* bMasterInterface: Communication class interface */
, 0x01  /* bSlaveInterface0: Data Class Interface */

/*Endpoint 2 Descriptor ^ 37
 */
, 0x07                           /* bLength: Endpoint Descriptor size */
, USB_ENDPOINT_DESCRIPTOR_TYPE   /* bDescriptorType: Endpoint */
, CDC_CMD_EP                     /* bEndpointAddress */
, EPTYPE_INTERRUPT | EPATTR_NO_SYNC | EPUSAGE_DATA  /* bmAttributes: Interrupt */
, LOBYTE( CDC_NOTI_EPSIZE )      /* wMaxPacketSize: */
, HIBYTE( CDC_NOTI_EPSIZE )
#ifdef USE_USB_OTG_HS
,  0x10                           /* bInterval: */
#else
,  0xFF                           /* bInterval: */
#endif                            /* USE_USB_OTG_HS */

/* Data class interface descriptor ^ 44
 */
, 0x09                           /* bLength: Endpoint Descriptor size */
, USB_INTERFACE_DESCRIPTOR_TYPE  /* bDescriptorType: */
, 0x01                           /* bInterfaceNumber: Number of Interface */
, 0x00                           /* bAlternateSetting: Alternate setting */
, 0x02                           /* bNumEndpoints: Two endpoints used */
, 0x0A                           /* bInterfaceClass: CDC */
, 0x00                           /* bInterfaceSubClass: */
, 0x00                           /* bInterfaceProtocol: */
, 0x00                           /* iInterface: */

/* Endpoint OUT Descriptor ^ 53
 */
, 0x07                             /* bLength: Endpoint Descriptor size */
, USB_ENDPOINT_DESCRIPTOR_TYPE     /* bDescriptorType: Endpoint */
, CDC_OUT_EP                       /* bEndpointAddress */
, 0x02                             /* bmAttributes: Bulk */
, LOBYTE(CDC_TXRX_EPSIZE) /* wMaxPacketSize: */
, HIBYTE(CDC_TXRX_EPSIZE)
, 0x00                             /* bInterval: ignore for Bulk transfer */

/*Endpoint IN Descriptor ^ 60
 */
, 0x07                              /* bLength: Endpoint Descriptor size */
, USB_ENDPOINT_DESCRIPTOR_TYPE      /* bDescriptorType: Endpoint */
, CDC_IN_EP                         /* bEndpointAddress */
, 0x02                              /* bmAttributes: Bulk */
, LOBYTE(CDC_TXRX_EPSIZE)  /* wMaxPacketSize: */
, HIBYTE(CDC_TXRX_EPSIZE)
, 0x00                               /* bInterval: ignore for Bulk transfer */
};  // ^ 67


/**   Configuration descriptor structure. for CDC device.
 *
 *  Describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. Readed out by the USB host during the enumeration process when selecting
 *  a configuration.
 */
const static struct
{	usbInterfaceDesc CDC_CCI_Interface;
  usbCdcHeaderDesc CDC_Functional_Header;
  usbCdcAcmDesc    CDC_Functional_ACM;
  usbCdcUnionDesc  CDC_Functional_Union;
  usbEndpointDesc  CDC_NotificationEndpoint;

 	usbInterfaceDesc CDC_DCI_Interface;
  usbEndpointDesc  CDC_DataOutEndpoint;
  usbEndpointDesc  CDC_DataInEndpoint;

  byte             EOFstruct;   // Marks the end ( 0 size  )
}

PACKED cdcConfDesc=
{ .CDC_CCI_Interface   =
		{ .bLength           = sizeof( usbInterfaceDesc )
		, .bType             = DTYPE_INTERFACE
		, .bInterfaceNumber  = 0 + 0 // to walk INTERFACE_ID_CDC_CCI
		, .bAlternateSetting = 0
		, .bNumEndpoints     = 1
		, .bInterfaceClass   = USB_CLASS_CDC
		, .bInterfaceSubClass= USB_CDC_SUBCLASS_ACM
		, .bInterfaceProtocol= USB_CDC_PROTOCOL_AT
		, .iInterface        = NO_DESCRIPTOR		}

, .CDC_Functional_Header =
		{ .bLength           = sizeof( usbCdcHeaderDesc )
		, .bType             = DTYPE_CS_INTERFACE
		,	.bDescriptorSubType= DTYPE_CDC_HEADER //CDC_DSUBTYPE_CSInterface_Header
		,	.bcdCDC            = VERSION_BCD(1,1,0) }

, .CDC_Functional_ACM   =
		{	.bLength            = sizeof( usbCdcAcmDesc )
		, .bType              = DTYPE_CS_INTERFACE
		,	.bDescriptorSubType = DTYPE_CDC_ACM
		,	.bmCapabilities     = 0x06		}

,	.CDC_Functional_Union =
		{ .bLength            = sizeof( usbCdcUnionDesc )
		, .bType              = DTYPE_CS_INTERFACE
		, .bDescriptorSubType = DTYPE_CDC_UNION
		, .bMasterInterface0  = 0   // to walk INTERFACE_ID_CDC_CCI
		, .bSlaveInterface0   = 1 } // to walk INTERFACE_ID_CDC_DCI

, .CDC_NotificationEndpoint =
		{ .bLength           = sizeof( usbEndpointDesc )
		, .bType             = DTYPE_ENDPOINT + 1
		, .bEndpointAddress  = EPDIR_IN    // T walj
		, .bmAttributes      = ( EPTYPE_INTERRUPT | EPATTR_NO_SYNC | EPUSAGE_DATA )
		,	.wMaxPacketSize    = CDC_NOTI_EPSIZE
		,	.bInterval = 0xFF		}

,.CDC_DCI_Interface =
		{ .bLength           = sizeof( usbInterfaceDesc )
		, .bType             = DTYPE_INTERFACE
		, .bInterfaceNumber  = 0 + 1 // to walk INTERFACE_ID_CDC_DCI
		, .bAlternateSetting = 0
		,	.bNumEndpoints     = 2
		,	.bInterfaceClass   = USB_CLASS_CDC_DATA
		,	.bInterfaceSubClass= USB_SUBCLASS_NONE
		,	.bInterfaceProtocol= USB_PROTO_NONE
		,	.iInterface        = NO_DESCRIPTOR		}

, .CDC_DataOutEndpoint =
		{ .bLength           = sizeof( usbEndpointDesc )
		, .bType             = DTYPE_ENDPOINT + 2
		,	.bEndpointAddress  = EPDIR_OUT // to walk CDC_RX_EPADDR
		,	.bmAttributes      = EPTYPE_BULK | EPATTR_NO_SYNC | EPUSAGE_DATA
		,	.wMaxPacketSize    = CDC_TXRX_EPSIZE
		,	.bInterval = 0x05	}

, .CDC_DataInEndpoint =
		{ .bLength           = sizeof(usbEndpointDesc)
		, .bType             = DTYPE_ENDPOINT + 3
		, .bEndpointAddress  = EPDIR_IN // to walk CDC_TX_EPADDR
		, .bmAttributes      = EPTYPE_BULK | EPATTR_NO_SYNC | EPUSAGE_DATA
		,	.wMaxPacketSize    = CDC_TXRX_EPSIZE
		,	.bInterval = 0x05		}
};


/**
  * @brief  usbd_cdc_Init
  *         DeInitialize the CDC layer
  * @param  cfgidx: Configuration index
  * @retval status
  */
schar  usbd_cdc_DeInit( byte cfgidx )
{ USBDepClose( CDC_IN_EP  );   /* Open EP IN */
  USBDepClose( CDC_OUT_EP );   /* Open EP OUT */
  USBDepClose( CDC_CMD_EP );   /* Open Command IN EP */

  APP_FOPS.pIf_DeInit();        /* Restore default state of the Interface physical components */

  return( 0 );
}

/**
  * @brief  usbd_cdc_Setup
  *         Handle the CDC specific requests
  * @param  req: usb requests
  * @retval status
  */
schar usbd_CDCsetup( USB_SETUP_REQ * req )
{ switch ( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS :           /* CDC Class Requests -------------------------------*/
      if ( req->wLength )               /* Check if the request is a data setup packet */
      { if ( req->bmRequest & 0x80 )    /* Check if the request is Device-to-Host (IN) */
        { APP_FOPS.pIf_Ctrl( req->bRequest
                           , CmdBuff
                           , req->wLength );  /* Get the data to be sent to Host from interface layer */

          USBD_CtlSendData( CmdBuff           /* Send the data to the host */
                          , req->wLength );
        }

        else                     /* Host-to-Device request */
        { cdcCmd= req->bRequest; /* Set the value of the current command to be processed */
          cdcLen= req->wLength;

/* Prepare the reception of the buffer over EP0
   Next step: the received data will be managed in usbd_cdc_EP0_TxSent()
   function.
 */
          USBD_CtlPrepareRx( CmdBuff
                           , req->wLength );
      } }

      else /* No Data request */
      { APP_FOPS.pIf_Ctrl( req->bRequest
                         , NULL
                         , 0 ); /* Transfer the command to the interface layer */
      }
    return( 0 );

    default:
      USBD_CtlError( req );
  return( -1 );


  case USB_REQ_TYPE_STANDARD: switch ( req->bRequest )
  { case USB_REQ_GET_DESCRIPTOR:
      USBD_CtlError( req );
    return( -1);

    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData( (byte *)&usbd_cdc_AltSet
                      , 1 );
    break;

    case USB_REQ_SET_INTERFACE :
      if ( (byte)(req->wValue) < 1 ) // ���� USBD_ITF_MAX_NUM )
      { usbd_cdc_AltSet = (byte)(req->wValue);
      }
      else /* Call the error management function (command will be nacked */
      { USBD_CtlError( req);
      }
    break;
  } }

  return( 0 );
}

/**
  * @brief  usbd_cdc_DataCmd
  *         Data received on control endpoint
  * @retval status
  */
schar usbd_cdc_DataCmd( byte ep )
{ APP_FOPS.pIf_Ctrl( cdcCmd, CmdBuff, cdcLen ); /* Process the data */
  cdcCmd= 255;          /* Reset the command variable to default value */

  return( 0 );
}


/**
  * @brief  usbd_cdc_DataIn
  *         Data sent on non-control IN endpoint
  * @param  epnum: endpoint number
  * @retval status
  */
schar usbd_cdc_DataIn( byte epnum )
{ return( 0 );
}


/**
  * @brief  USBD_CDCcrtl
  *         DeInitialize the HID layer
  * @param  cfgidx: Configuration index
  * @retval status
  */
static schar  USBD_CDCcrtl( byte what, word cfgidx )
{ switch( what )
  { case ACTION_INIT:         /* Open EP IN */
    { USBDepAction( 0x00, usbd_cdc_DataCmd );

      USBDepOpen( CDC_IN_EP ,                  64, EPTYPE_BULK     , APP_FOPS.pIf_DataTx  ); /* Open EP IN */
      USBDepOpen( CDC_OUT_EP,                  64, EPTYPE_BULK     , APP_FOPS.pIf_DataRx ); /* Open EP OUT */
      USBDepOpen( CDC_CMD_EP, CDC_CMD_PACKET_SIZE, EPTYPE_INTERRUPT, APP_FOPS.pIf_DataCt ); /* Open Command IN EP */

      APP_FOPS.pIf_Init();               /* Initialize the Interface physical components */
      APP_FOPS.pIf_DataRx( CDC_OUT_EP ); /* Prepare Out endpoint to receive next packet  */
      APP_FOPS.pIf_DataRx( CDC_CMD_EP ); /* Prepare Cmd endpoint to receive next packet  */
    break;

    case ACTION_DEINIT:       /* Close HID EPs */
       USBDepClose( HID_IN_EP );
    break;

    case ACTION_ISO_INICOM:
    case ACTION_ISO_OUTICOM:
    break;
  } }

  return( 0 );
}

/**
  * @brief  CDC interface class callbacks structure
  */
USBDclassDefREC USBD_CDC_cb =
{ USBD_CDCcrtl
, usbd_CDCsetup
, USBD_CDCcfgDesc
};
