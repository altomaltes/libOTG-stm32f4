/**
  *          ===================================================================
  *                                MSC Class  Description
  *          ===================================================================
  *           This module manages the MSC class V1.0 following the "Universal
  *           Serial Bus Mass Storage Class (MSC) Bulk-Only Transport (BOT) Version 1.0
  *           Sep. 31, 1999".
  *           This driver implements the following aspects of the specification:
  *             - Bulk-Only Transport protocol
  *             - Subclass : SCSI transparent command set (ref. SCSI Primary Commands - 3 (SPC-3))
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "./usb_std.h"
#include "./usbd_msc.h"
#include "./usbd_msc_bot.h"

byte devSpeed= USB_OTG_SPEED_FULL;



/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
//ALIGN_THIS( const byte USBD_MSC_CfgFSDesc[ USB_MSC_CONFIG_DESC_SIZ ] ) =
//{ 0x09                     /* bLength: Configuation Descriptor size */
//, DTYPE_CONFIGURATION      /* bDescriptorType: Configuration */ // USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION 7 DTYPE_OTHER
//, USB_MSC_CONFIG_DESC_SIZ
//, 0x00
//, 0x01   /* bNumInterfaces: 1 interface */
//, 0x01   /* bConfigurationValue: */
//, 0x04   /* iConfiguration: */
//, 0xC0   /* bmAttributes: */
//, 0x32   /* MaxPower 100 mA */

const struct
{	usbInterfaceDesc MSCiface;
  usbEndpointDesc  MSCepIN;
  usbEndpointDesc  MSCepOT;
  byte             EOF;
}
PACKED static mscConfDesc=
{ .MSCiface=
  { .bLength= sizeof( usbInterfaceDesc )           /** Size of the descriptor, in bytes.*/
  , .bType             = DTYPE_INTERFACE           /** Interface descriptor.*/
  , .bInterfaceNumber  = 0                         /** Index of the interface in the current configuration.*/
  , .bAlternateSetting = 0                         /** Alternate setting for the interface number.*/
  , .bNumEndpoints     = 2                         /** Total number of endpoints in the interface.*/
  , .bInterfaceClass   = USB_CLASS_MASS_STOR       /** Interface class ID. (3) */
  , .bInterfaceSubClass= MS_CSCP_SCSI_TRANSPARENT  /** Interface subclass ID.*/
  , .bInterfaceProtocol= MS_CSCP_BULK_ONLY_TRANS   /** Interface protocol ID. */
  , .iInterface        = 0x05                      /** Index of the string descriptor describing the interface. */
  }

, .MSCepIN=
  { .bLength= sizeof( usbEndpointDesc )            /** Size of the descriptor, in bytes. */
  , .bType           =  DTYPE_ENDPOINT             /** Endpoint descriptor.*/
  , .bEndpointAddress=  EPDIR_IN + 1               /** Logical address of the endpoint within the device for the current configuration, including direction mask. */
  , .bmAttributes    =  EPTYPE_BULK                /** Endpoint bmAttributes, \ref USB_ENDPOINT_DEF. */
  , .wMaxPacketSize  =  MSC_MAX_FS_PACKET          /** ?? Size of the endpoint bank, in bytes. This indicates the maximum packet size that the endpoint can receive at a time. */
  , .bInterval       =  0x00                       /** ?? Polling interval in milliseconds for the endpoint ifit is an INTERRUPT or ISOCHRONOUS type.*/
  }

, .MSCepOT=
  { .bLength= sizeof( usbEndpointDesc )             /** Size of the descriptor, in bytes. */
  , .bType           =  DTYPE_ENDPOINT              /** Endpoint descriptor.*/
  , .bEndpointAddress=  EPDIR_OUT + 1               /** Logical address of the endpoint within the device for the current configuration, including direction mask. */
  , .bmAttributes    =  EPTYPE_BULK                 /** Endpoint bmAttributes, \ref USB_ENDPOINT_DEF. */
  , .wMaxPacketSize  =  MSC_MAX_FS_PACKET           /** ?? Size of the endpoint bank, in bytes. This indicates the maximum packet size that the endpoint can receive at a time. */
  , .bInterval       =  0x00                        /** ?? Polling interval in milliseconds for the endpoint ifit is an INTERRUPT or ISOCHRONOUS type.*/
} };



/**
 * @brief  USBD_MSC_Crtl
 *         Initialize  the mass storage configuration
 * @param  cfgidx: configuration index
 * @retval status
 */
short USBD_MSC_Crtl( dword what, word conf, DeviceListRec * dList )     /* EP used by this device */
{ MSCdriverRec * hmsc= (MSCdriverRec *)dList->doArgs;
  byte pktSize= devSpeed == USB_OTG_SPEED_HIGH ? MSC_MAX_FS_PACKET : MSC_MAX_FS_PACKET;

  switch( what )
  { case ACTION_INIT:         /* Open EP IN */
      USBDepOpen( hmsc->epOt= dList->epOt, pktSize, EPTYPE_BULK, MSC_BOT_DataOut ); /* Open EP OUT */
      USBDepOpen( hmsc->epIn= dList->epIn, pktSize, EPTYPE_BULK, MSC_BOT_DataIn  ); /* Open EP IN */
      MSC_BOT_Init( hmsc );
    return( 0 );

    case ACTION_DEINIT:       /* Close HID EPs */
      USBDepClose( dList->epOt ); /* Close MSC EPs */
      USBDepClose( dList->epIn  ); /* Open EP IN */
      MSC_BOT_DeInit( hmsc );
    return 0;

    case ACTION_ISO_INICOM:
    case ACTION_ISO_OUTICOM:
    break;
  }

  return( 0 );
}

/**
 * @brief  USBD_MSC_Setup
 *         Handle the MSC specific requests
 * @param  req: USB request
 * @retval status
 */
short USBD_MSC_Setup( USB_SETUP_REQ * req, DeviceListRec * dList )
{ MSCdriverRec * hnd= (MSCdriverRec *)dList->doArgs;

  switch ( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS : /* Class request */
      switch ( req->bRequest )
      { case BOT_GET_MAX_LUN :
          if (( req->wValue  == 0 )
          &&  ( req->wLength == 1 )
          && (( req->bmRequest & 0x80 ) == 0x80))
          { //stor.GetMaxLun();
            hnd->Ioctl( USB_IOCTL_GET_LUN
                      , &hnd->max_lun );
            USBDctlSendData( &hnd->max_lun, 1 );
          }
          else
          {  // USBD_CtlError( req); return -1;
          }
        break;

        case BOT_RESET :
          if (( req->wValue  == 0 )
          &&  ( req->wLength == 0 )
          && (( req->bmRequest & 0x80 ) != 0x80 ))
          { MSC_BOT_Reset( dList );
          }
          else
          {  // USBD_CtlError( req);
            return -1;
          }
        break;

        default:
//       USBD_CtlError( req);
       return -1;
    }
    break;

  case USB_REQ_TYPE_STANDARD:   /* Interface & Endpoint request */
    switch ( req->bRequest )
    { case USB_REQ_GET_INTERFACE:
        USBDctlSendData( &hnd->interface, 1 ); break;
      case USB_REQ_SET_INTERFACE:
         hnd->interface= (byte)(req->wValue);   break;

    case USB_REQ_CLEAR_FEATURE:
      USBDepFlush( req->wIndex ); /* Flush the FIFO and Clear the stall status */
//      USBDepClose( req->wIndex ); /* Re-activate the EP */

//      if ( req->wIndex & 0x80 )
//      { if ( devSpeed == USB_OTG_SPEED_HIGH  )
//        { USBDepOpen( hmsc->epIn, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn, hnd ); /* Open EP IN */
//        }
//        else
//        { USBDepOpen( hmsc->epIn,  MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn, hnd  ); /* Open EP IN */
//      } }

//      else
//      { if( devSpeed == USB_OTG_SPEED_HIGH  )
//        { USBDepOpen( hmsc->epOt, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut, hnd ); /* Open EP OUT */
//        }
//        else
//        { USBDepOpen( hmsc->epOt, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut, hnd ); /* Open EP OUT */
//      } }

      /* Handle BOT error */
      MSC_BOT_CplClrFeature( hnd, req->wIndex );
    break;
    }
  break;

    default: break;
  }

  return( 0 );
}

/**
 * @brief  USBD_MSC_cb
 *         return configuration descriptor
 */
const USBDclassDefREC USBD_MSC_cb=
{ USBD_MSC_Crtl
, USBD_MSC_Setup
, (byte*)&mscConfDesc
};

const USBDclassDefREC USBD_MSC_cb2=
{ USBD_MSC_Crtl
, USBD_MSC_Setup
, (byte*)&mscConfDesc
};
