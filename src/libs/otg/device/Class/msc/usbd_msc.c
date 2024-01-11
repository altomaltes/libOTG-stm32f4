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
#include "./usbd_msc.h"

byte devSpeed= USB_OTG_SPEED_FULL;


/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
ALIGN_THIS( const byte USBD_MSC_CfgFSDesc[ USB_MSC_CONFIG_DESC_SIZ ] ) =
{ 0x09                     /* bLength: Configuation Descriptor size */
, DTYPE_CONFIGURATION      /* bDescriptorType: Configuration */ // USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION 7 DTYPE_OTHER
, USB_MSC_CONFIG_DESC_SIZ

, 0x00
, 0x01   /* bNumInterfaces: 1 interface */
, 0x01   /* bConfigurationValue: */
, 0x04   /* iConfiguration: */
, 0xC0   /* bmAttributes: */
, 0x32   /* MaxPower 100 mA */

/********************  Mass Storage interface *******************
 */
,  0x09   /* 10 bLength: Interface Descriptor size */
,  0x04   /* 11 bDescriptorType: */
,  0x00   /* 12 bInterfaceNumber: Number of Interface */
,  0x00   /* 13 bAlternateSetting: Alternate setting */
,  0x02   /* 14 bNumEndpoints*/
,  0x08   /* 15 bInterfaceClass: MSC Class */
,  0x06   /* 16 bInterfaceSubClass : SCSI transparent*/
,  0x50   /* 17 nInterfaceProtocol */
,  0x05   /* 18 iInterface: */

/********************  Mass Storage Endpoints *******************
 */
,  0x07                        /* Endpoint descriptor length = 7*/
,  0x05                        /* Endpoint descriptor type */
,  MSC_EPIN_ADDR               /* Endpoint address (IN, address 1) */
,  0x02                        /* Bulk endpoint type */
,  LOBYTE( MSC_MAX_FS_PACKET )
,  HIBYTE( MSC_MAX_FS_PACKET )
,  0x00                        /* Polling interval in milliseconds */

,  0x07                        /*Endpoint descriptor length = 7 */
,  0x05                        /*Endpoint descriptor type */
,  MSC_EPOUT_ADDR              /*Endpoint address (OUT, address 1) */
,  0x02                        /*Bulk endpoint type */
,  LOBYTE( MSC_MAX_FS_PACKET )
,  HIBYTE( MSC_MAX_FS_PACKET )
,  0x00                        /*Polling interval in milliseconds*/

};


/* USB Standard Device Descriptor
 */
ALIGN_THIS( const byte USBD_MSC_DeviceQualifierDesc[ USB_LEN_DEV_QUALIFIER_DESC ] ) =
{ USB_LEN_DEV_QUALIFIER_DESC
, DTYPE_QUALIFIER        // 1 -> 6
, 0x00, 0x02                   // 2 -> BCD ver
, 0x00              // Class
, 0x00              // subclass
, 0x00              // proto
, MSC_MAX_FS_PACKET
, 0x01              // numconf
, 0x00
};


/**
 * @brief  USBD_MSC_Crtl
 *         Initialize  the mass storage configuration
 * @param  cfgidx: configuration index
 * @retval status
 */
short USBD_MSC_Crtl( dword what, word cfgidx )
{ switch( what )
  { case ACTION_INIT:         /* Open EP IN */
      if ( devSpeed == USB_OTG_SPEED_HIGH )
      { USBDepOpen( MSC_EPOUT_ADDR, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut ); /* Open EP OUT */
        USBDepOpen( MSC_EPIN_ADDR , MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn  ); /* Open EP IN */
      }
      else
      { USBDepOpen( MSC_EPOUT_ADDR, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut ); /* Open EP OUT */
        USBDepOpen( MSC_EPIN_ADDR,  MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn  ); /* Open EP IN */
      }

      MSC_BOT_Init();
    return( 0 );

    case ACTION_DEINIT:       /* Close HID EPs */
      USBDepClose( MSC_EPOUT_ADDR ); /* Close MSC EPs */
      USBDepClose( MSC_EPIN_ADDR  ); /* Open EP IN */
      MSC_BOT_DeInit();
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
short USBD_MSC_Setup( USB_SETUP_REQ * req  )
{ switch ( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS : /* Class request */
      switch ( req->bRequest )
      { case BOT_GET_MAX_LUN :
          if (( req->wValue  == 0 )
          &&  ( req->wLength == 1 )
          && (( req->bmRequest & 0x80) == 0x80))
          { //stor.GetMaxLun();
            stor.Ioctl( USB_IOCTL_GET_LUN, &hmsc.max_lun );
            USBDctlSendData( &hmsc.max_lun, 1 );
          }
          else
          {  // USBD_CtlError( req); return -1;
          }
    break;

    case BOT_RESET :
      if((req->wValue  == 0) &&
         (req->wLength == 0) &&
        ((req->bmRequest & 0x80) != 0x80))
      {
         MSC_BOT_Reset();
      }
      else
      {  // USBD_CtlError( req);         return -1;
      }
    break;

    default:
//       USBD_CtlError( req);
       return -1;
    }
    break;

  case USB_REQ_TYPE_STANDARD:   /* Interface & Endpoint request */
    switch (req->bRequest)
    { case USB_REQ_GET_INTERFACE :
        USBDctlSendData( (byte *)&hmsc.interface
                       , 1 );
      break;

    case USB_REQ_SET_INTERFACE :
      hmsc.interface= (byte)(req->wValue);
    break;

    case USB_REQ_CLEAR_FEATURE:
      USBDepFlush( req->wIndex ); /* Flush the FIFO and Clear the stall status */
      USBDepClose( req->wIndex ); /* Re-activate the EP */

      if ( req->wIndex & 0x80 )
      { if ( devSpeed == USB_OTG_SPEED_HIGH  )
        { USBDepOpen( MSC_EPIN_ADDR , MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn ); /* Open EP IN */
        }
        else
        { USBDepOpen(MSC_EPIN_ADDR,  MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataIn  ); /* Open EP IN */
      } }

      else
      { if( devSpeed == USB_OTG_SPEED_HIGH  )
        { USBDepOpen( MSC_EPOUT_ADDR, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut ); /* Open EP OUT */
        }
        else
        { USBDepOpen(MSC_EPOUT_ADDR, MSC_MAX_FS_PACKET, EPTYPE_BULK, MSC_BOT_DataOut ); /* Open EP OUT */
      } }

      /* Handle BOT error */
      MSC_BOT_CplClrFeature( req->wIndex);
    break;
    }
  break;

  default:
    break;
  }
  return 0;
}

/**
 * @brief  USBD_MSC_cb
 *         return configuration descriptor
 */
USBDclassDefREC USBD_MSC_cb=
{ USBD_MSC_Crtl
, USBD_MSC_Setup
, USBD_MSC_CfgFSDesc
};

