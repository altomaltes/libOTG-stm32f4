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
 * @file usbd_msc_core.c
 *
 * original draft from MCD Application Team
 *
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

#include "usbd_msc_core.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_bot.h"


byte CmdBuff[1];
byte mscCmd;
byte mscLen;

/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */

ALIGN_THIS( byte USBD_MSC_CfgDesc[ USB_MSC_CONFIG_DESC_SIZ ] ) =
{ 0x09                         /* bLength: Configuation Descriptor size */
, DTYPE_CONFIGURATION  /* bDescriptorType: Configuration */
, USB_MSC_CONFIG_DESC_SIZ

, 0x00
, 0x01   /* bNumInterfaces: 1 interface */
, 0x01   /* bConfigurationValue: */
, 0x04   /* iConfiguration: */
, 0xC0   /* bmAttributes: */
, 0x32   /* MaxPower 100 mA */

/********************  Mass Storage interface *******************
 */
, 0x09   /* bLength: Interface Descriptor size */
, 0x04   /* bDescriptorType: */
, 0x00   /* bInterfaceNumber: Number of Interface */
, 0x00   /* bAlternateSetting: Alternate setting */
, 0x02   /* bNumEndpoints*/
, 0x08   /* bInterfaceClass: MSC Class */
, 0x06   /* bInterfaceSubClass : SCSI transparent*/
, 0x50   /* nInterfaceProtocol */
, 0x05   /* iInterface: */

/********************  Mass Storage Endpoints *******************
*/
, 0x07                      /* Endpoint descriptor length = 7   */
, 0x05                      /* Endpoint descriptor type         */
, MSC_IN_EP                 /* Endpoint address (IN, address 1) */
, EPTYPE_BULK              /* Bulk endpoint type */
, LOBYTE( MSC_MAX_PACKET )
, HIBYTE( MSC_MAX_PACKET )
, 0x00                      /* Polling interval in milliseconds */

, 0x07                      /* Endpoint descriptor length = 7    */
, 0x05                      /* Endpoint descriptor type          */
, MSC_OUT_EP                /* Endpoint address (OUT, address 1) */
, EPTYPE_BULK              /* Bulk endpoint type                */
, LOBYTE( MSC_MAX_PACKET )
, HIBYTE( MSC_MAX_PACKET )
, 0x00                      /* Polling interval in milliseconds*/
};




/**
 * @brief  USBD_MSC_DeInit
 *         DeInitilaize  the mass storage configuration
 * @param  cfgidx: configuration index
 * @retval status
 */
schar USBD_MSC_DeInit ( byte cfgidx )
{
  USBDepClose( MSC_IN_EP  ); /* Close MSC EPs */
  USBDepClose( MSC_OUT_EP );


  MSC_BOT_DeInit();   /* Un Init the BOT layer */
  return( 0 );
}

/**
  * @brief  USBD_MSC_DataCmd
  *         Data received on control endpoint
  * @retval status
  */
schar usbd_msc_DataCmd( byte ep )
{ USBD_STORAGE_fops->GetMaxLun( mscCmd, CmdBuff, mscLen ); /* Process the data */
  mscCmd= 255;          /* Reset the command variable to default value */

  return( 0 );
}


/**
 * @brief  USBD_MSC_Setup
 *         Handle the MSC specific requests
 * @param  req: USB request
 * @retval status
 */
static schar USBD_MSC_Setup( USB_SETUP_REQ * req )
{ ALIGN_THIS( static byte USBD_MSC_AltSet );

  switch( req->bmRequest & USB_REQ_TYPE_MASK )
  { case USB_REQ_TYPE_CLASS:
      if ( req->wLength )               /* Check if the request is a data setup packet */
      { if ( req->bmRequest & 0x80 )    /* Check if the request is Device-to-Host (IN) */
        { USBD_STORAGE_fops->GetMaxLun( req->bRequest
                                      , CmdBuff
                                      , req->wLength );  /* Get the data to be sent to Host from interface layer */
          USBDctlSendData( CmdBuff           /* Send the data to the host */
                          , req->wLength );
        }

        else                     /* Host-to-Device request */
        { mscCmd= req->bRequest; /* Set the value of the current command to be processed */
          mscLen= req->wLength;

/* Prepare the reception of the buffer over EP0
   Next step: the received data will be managed in usbd_cdc_EP0_TxSent()
   function.
 */
          USBDctlPrepareRx( CmdBuff
                           , req->wLength );
      } }

      else /* No Data request */
      { USBD_STORAGE_fops->GetMaxLun( req->bRequest
                                    , NULL
                                    , 0 ); /* Transfer the command to the interface layer */
      }

    return( 0 );

    case USB_REQ_TYPE_STANDARD: switch (req->bRequest)     /* Interface & Endpoint request */
    { case USB_REQ_GET_INTERFACE :
        USBDctlSendData( &USBD_MSC_AltSet, 1 );
      break;

      case USB_REQ_SET_INTERFACE :
        USBD_MSC_AltSet = (byte)(req->wValue);
      break;

      case USB_REQ_CLEAR_FEATURE:
        USBDepFlush( (byte)req->wIndex ); /* Flush the FIFO and Clear the stall status */
        USBDepClose( (byte)req->wIndex ); /* Re-activate the EP */

        if ((((byte)req->wIndex) & 0x80) == 0x80 )
        { USBDepOpen(((byte)req->wIndex)
                     , MSC_EPIN_SIZE
                     , EPTYPE_BULK, MSC_BOT_DataIn );
        }
        else
        { USBDepOpen(((byte)req->wIndex)
                     , MSC_EPOUT_SIZE
                     , EPTYPE_BULK, MSC_BOT_DataOut );
        }

        MSC_BOT_CplClrFeature( (byte)req->wIndex);           /* Handle BOT error */
      break;
      }
    break;

    default: break;
  }

  return( 0 );
}

/**
 * @brief  USBD_MSC_Init
 *         Initialize  the mass storage configuration
 * @param  cfgidx: configuration index
 * @retval status
 */
schar USBD_MSC_Crtl( byte what, word cfgidx )
{ switch( what )
  { case ACTION_INIT:         /* Open EP IN */
      USBD_MSC_DeInit( cfgidx );

      USBDepAction( 0x00, usbd_msc_DataCmd );

      USBDepOpen(  MSC_IN_EP, MSC_EPIN_SIZE,  EPTYPE_BULK, MSC_BOT_DataIn  );  /* Open EP IN */
      USBDepOpen( MSC_OUT_EP, MSC_EPOUT_SIZE, EPTYPE_BULK, MSC_BOT_DataOut );  /* Open EP OUT */

      MSC_BOT_Init();        /* Init the BOT  layer */
    break;

    case ACTION_DEINIT:       /* Close HID EPs */
      USBD_MSC_DeInit( cfgidx );
    break;

    case ACTION_ISO_INICOM:
    case ACTION_ISO_OUTICOM:
    break;
  }

  return( 0 );
}

/**
 * @brief  USBD_MSC_cb
 *         return configuration descriptor
 */
USBDclassDefREC USBD_MSC_cb=
{ USBD_MSC_Crtl
, USBD_MSC_Setup
, USBD_MSC_CfgDesc
};



