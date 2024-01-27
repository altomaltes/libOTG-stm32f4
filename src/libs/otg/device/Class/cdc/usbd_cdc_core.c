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

#include "usb_dcd.h"
#include "usbd_cdc_core.h"
#include "usbd_hid_core.h"

#define CDC_NOTI_EPSIZE  8		/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define CDC_TXRX_EPSIZE 64 	/** Size in bytes of the CDC data IN and OUT endpoints. */


/**   Configuration descriptor structure. for CDC device.
 *
 *  Describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. Readed out by the USB host during the enumeration process when selecting
 *  a configuration.
 */
struct

{	usbIadDesc         CDC_Mergue;
  usbInterfaceDesc   CDC_CCI_Interface;
  usbCdcHeaderDesc   CDC_Functional_Header;
  usbCdcCallMgmtDesc CDC_CallMgmtDesc;
  usbCdcAcmDesc      CDC_Functional_ACM;
  usbCdcUnionDesc    CDC_Functional_Union;
  usbEndpointDesc    CDC_NotificationEndpoint;

 	usbInterfaceDesc   CDC_DCI_Interface;
  usbEndpointDesc    CDC_DataOutEndpoint;
  usbEndpointDesc    CDC_DataInEndpoint;

  byte             EOFstruct;   // Marks the end ( 0 size  )
}
PACKED const static cdcConfDesc=
{ .CDC_Mergue=
  { .bLength          = sizeof( usbIadDesc ) /** Size of the descriptor, in bytes.*/
  , .bType            = DTYPE_INTERFASEASSOC /** IAD descriptor */
  , .bFirstInterface  = 0                    /** Index of the first associated interface. */
  , .bInterfaceCount  = 2                    /** Total number of associated interfaces. */
  , .bFunctionClass   = USB_CLASS_CDC        /** Function class ID. */
  , .bFunctionSubClass= USB_SUBCLASS_CDC     /** Function subclass ID. */
  , .bFunctionProtocol= USB_CDC_PROTO_AT     /** Function protocol ID. */
  , .iFunction         = 0 }                  /** Index of the string descriptor describing the interface association. */


,.CDC_CCI_Interface   =
		{ .bLength           = sizeof( usbInterfaceDesc )
		, .bType             = DTYPE_INTERFACE
		, .bInterfaceNumber  = 0 //  to walk INTERFACE_ID_CDC_CCI
		, .bAlternateSetting = 0
		, .bNumEndpoints     = 1
		, .bInterfaceClass   = USB_CLASS_CDC
		, .bInterfaceSubClass= USB_CDC_SUBCLASS_ACM
		, .bInterfaceProtocol= USB_CDC_PROTO_NONE
		, .iInterface        = NO_DESCRIPTOR		}

, .CDC_Functional_Header =
		{ .bLength           = sizeof( usbCdcHeaderDesc )
		, .bType             = DTYPE_CS_INTERFACE
		,	.bDescriptorSubType= DTYPE_CDC_HEADER //CDC_DSUBTYPE_CSInterface_Header
		,	.bcdCDC            = VERSION_BCD( 2,0,0 ) }

, .CDC_CallMgmtDesc =
  { .bLength           = sizeof( usbCdcCallMgmtDesc ) /** Size of this functional descriptor, in bytes.*/
  , .bType             = DTYPE_CS_INTERFACE           /** CS_INTERFACE descriptor type.*/
  , .bDescriptorSubType= DTYPE_CDC_CALL_MANAGEMENT    /** Call Management functional descriptor subtype.*/
  , .bmCapabilities    = 0 /* D0+D1*/                 /** The call management capabilities that this configuration supports.*/
  , .bDataInterface    = 1 }                          /** Interface number of Data Class interface optionally used for call management.*/

, .CDC_Functional_ACM   =
		{	.bLength            = sizeof( usbCdcAcmDesc )
		, .bType              = DTYPE_CS_INTERFACE
		,	.bDescriptorSubType = DTYPE_CDC_ACM
		,	.bmCapabilities     =  0x00 } //  0x02		}  // ->0x06

,	.CDC_Functional_Union =
		{ .bLength            = sizeof( usbCdcUnionDesc )
		, .bType              = DTYPE_CS_INTERFACE
		, .bDescriptorSubType = DTYPE_CDC_UNION
		, .bMasterInterface0  = 0   // to walk INTERFACE_ID_CDC_CCI
		, .bSlaveInterface0   = 1 } // to walk INTERFACE_ID_CDC_DCI

, .CDC_NotificationEndpoint =
		{ .bLength           = sizeof( usbEndpointDesc )
		, .bType             = DTYPE_ENDPOINT
		, .bEndpointAddress  = EPDIR_IN  + 2  // T walj
		, .bmAttributes      = ( EPTYPE_INTERRUPT | EPATTR_NO_SYNC | EPUSAGE_DATA )
		,	.wMaxPacketSize    = CDC_NOTI_EPSIZE
#ifdef USE_USB_OTG_HS
		,	.bInterval = 0x10
#else
		,	.bInterval = 0xFF
#endif                            /* USE_USB_OTG_HS */
		}

,.CDC_DCI_Interface =
		{ .bLength           = sizeof( usbInterfaceDesc )
		, .bType             = DTYPE_INTERFACE
		, .bInterfaceNumber  = 1 //  to walk INTERFACE_ID_CDC_DCI
		, .bAlternateSetting = 0
		,	.bNumEndpoints     = 2
		,	.bInterfaceClass   = USB_CLASS_CDC_DATA
		,	.bInterfaceSubClass= USB_SUBCLASS_NONE
		,	.bInterfaceProtocol= USB_PROTO_NONE
		,	.iInterface        = NO_DESCRIPTOR		}

, .CDC_DataOutEndpoint =
		{ .bLength           = sizeof( usbEndpointDesc )
		, .bType             = DTYPE_ENDPOINT
		,	.bEndpointAddress  = EPDIR_OUT + 1 // to walk CDC_RX_EPADDR
		,	.bmAttributes      = EPTYPE_BULK | EPATTR_NO_SYNC | EPUSAGE_DATA
		,	.wMaxPacketSize    = CDC_TXRX_EPSIZE
		,	.bInterval         = 0x01	}

, .CDC_DataInEndpoint =
		{ .bLength          = sizeof( usbEndpointDesc )
		, .bType            = DTYPE_ENDPOINT
		, .bEndpointAddress = EPDIR_IN + 1 // to walk CDC_TX_EPADDR
		, .bmAttributes     = EPTYPE_BULK | EPATTR_NO_SYNC | EPUSAGE_DATA
		,	.wMaxPacketSize   = CDC_TXRX_EPSIZE
		,	.bInterval        = 0x01		}
};

/**
  * @brief  usbd_cdc_Setup
  *         Handle the CDC specific requests
  * @param  req: usb requests
  * @retval status
  */
short usbd_CDCsetup( USB_SETUP_REQ * req
                   , DeviceListRec * dList )
{ struct cdcHandleRec * hnd= (struct  cdcHandleRec *)dList->doArgs;

  switch ( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS:             /* CDC Class Requests -------------------------------*/
      if ( req->wLength )                /* Check if the request is a data setup packet */
      { hnd->cdcCmd= req->bRequest;      /* Set the value of the current command to be processed */
        hnd->cdcLen= req->wLength;

        if ( req->bmRequest & 0x80 )     /* Check if the request is Device-to-Host (IN) */
        { hnd->ctrl( 0                   /* Signal by EP 0 */
                   , hnd );              /* Get the data to be sent to Host from interface layer */
          USBDctlSendData( hnd->cmdBuff  /* Send the data to the host */
                         , hnd->cdcLen );
        }

/* Prepare the reception of the buffer over EP0
   Next step: the received data will be managed in usbd_cdc_EP0_TxSent()
   function.
 */
        else                                       /* Host-to-Device request */
        { USBDepAction( 0x00, hnd->dataCmd, hnd ); /* EP0 received data manager */
          USBDctlPrepareRx( hnd->cmdBuff
                          , hnd->cdcLen );
      } }

      else /* No Data request */
      { hnd->ctrl( req->bRequest
                 , hnd ); /* Transfer the command to the interface layer */
      }
    return( 0 );

    default:
      USBDctlError( req );
  return( -1 );


  case USB_REQ_TYPE_STANDARD:
  switch ( req->bRequest )
  { case USB_REQ_GET_DESCRIPTOR:
      USBDctlError( req );
    return( -1);

    case USB_REQ_GET_INTERFACE :
      USBDctlSendData( &hnd[ 0 ].altSet
                      , 1 );
    break;

    case USB_REQ_SET_INTERFACE :
      if ( (byte)(req->wValue) < 1 ) // çççç USBD_ITF_MAX_NUM )
      { hnd[ 0 ].altSet= (byte)(req->wValue);
      }
      else /* Call the error management function (command will be nacked */
      { USBDctlError( req);
      }
    break;
  } }

  return( 0 );
}


/**
  * @brief  USBD_CDCcrtl
  *         DeInitialize the HID layer
  * @param  cfgidx: Configuration index
  * @retval status
  */
static short USBD_CDCcrtl( word what, word cfgIdx
                         , DeviceListRec * dList )
{ struct cdcHandleRec * hnd= (struct cdcHandleRec *)dList->doArgs;

  switch( what )
  { case ACTION_INIT:         /* Open EP IN */
    { USBDepOpen( dList->epOt  , CDC_TXRX_EPSIZE, EPTYPE_BULK     , hnd->dataOut ); /* Open EP OUT */
      USBDepOpen( dList->epIn+1, CDC_NOTI_EPSIZE, EPTYPE_INTERRUPT, hnd->dataCmd ); /* Open Command IN EP */
      USBDepOpen( dList->epIn+0, CDC_TXRX_EPSIZE, EPTYPE_BULK     , hnd->dataIn  ); /* Open EP IN */

      hnd->epIn= dList->epIn;
      USBDepPrepareRx( dList->epOt, hnd->dataBuf, sizeof( hnd->dataBuf ));  /* Prepare Out endpoint to receive next packet  */
    break;

    case ACTION_DEINIT:             /* Close HID EPs */
      USBDepClose( dList->epOt   ); /* Open EP OUT        */
      USBDepClose( dList->epIn+0 ); /* Open Command IN EP */
      USBDepClose( dList->epIn+1 ); /* Open EP IN         */
    return( /*USBdeviceDesc.driverHandle->Ioctl( USB_IOCTL_DEINIT )*/ 0 );

    case ACTION_ISO_INICOM:
    case ACTION_ISO_OUTICOM:
    break;
  } }

  return( 0 );
}

/**
  * @brief  CDC interface class callbacks structure
  */
const USBDclassDefREC USBD_CDC_cb=
{ USBD_CDCcrtl
, usbd_CDCsetup
, &cdcConfDesc
};

