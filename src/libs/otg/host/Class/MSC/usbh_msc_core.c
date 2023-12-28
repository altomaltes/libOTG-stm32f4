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
 * @file usbh_msc_core.c
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

#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbh_core.h"

#define USBH_MSC_ERROR_RETRY_LIMIT 10


ALIGN_THIS( MSC_Machine_TypeDef MSC_Machine );

/**
  * @brief  USBH_MSC_ClassRequest
  *         This function will only initialize the MSC state machine
  * @retval USBH_Status : Status of class request handled.
  */
static schar USBH_MSC_ClassRequest(  )
{ USBH_MSC_Init();
  USBH_MSC_BOTXferParam.MSCState= USBH_MSC_GET_MAX_LUN;

  USBHgetInterface( MS_GET_MAX_LUN, 0
                  , MSC_Machine.buff, 1 );

  return( 0 );
}


/**
  * @brief  USBH_MSC_Handle
  *         MSC state machine handler
  * @retval USBH_Status
  */
schar USBH_MSC_Handle( byte ep )
{ static byte maxLunExceed= FALSE;

  { switch( USBH_MSC_BOTXferParam.MSCState )
    { case USBH_MSC_CTRL_ERROR_STATE:   /* Issue Clearfeature request !!! */
        MSC_Machine.maxLun = 0;         /* If GetMaxLun Request not support, assume Single LUN configuration */
        USBH_ClrFeature( 0x00, USB_Host.Control.hcNumOut );
        USBH_MSC_BOTXferParam.MSCState= USBH_MSC_TEST_UNIT_READY;
      break;

      case USBH_MSC_GET_MAX_LUN:     /* Issue GetMaxLUN request */
        MSC_Machine.maxLun= *( MSC_Machine.buff );

        if (( MSC_Machine.maxLun > 0 )  /* If device has more that one logical unit then it is not supported */
        &&  (maxLunExceed == FALSE))
        { maxLunExceed = TRUE;
          usbHostGotDeviceNotSupported();
          return( 0 );
        }

        USBH_MSC_BOTXferParam.MSCState= USBHtestUnitReady(); /* Issue SCSI command TestUnitReady */
      break;

      case USBH_MSC_RESET:
      case USBH_MSC_IDLE:
      case USBH_MSC_READ10:
      case USBH_MSC_WRITE10:
      case USBH_MSC_MODE_SENSE6:
      case USBH_MSC_REQUEST_SENSE:
      case USBH_MSC_TEST_UNIT_READY:
      case USBH_MSC_READ_CAPACITY10:
      case USBH_MSC_UNRECOVERED_STATE:
      case USBH_MSC_DEFAULT_APPLI_STATE:

      return( -1 );
    } }

  USBH_MSC_XferStart( ep );
  return( 0 );
}
                                               /* Next step */

schar USBH_MSC_Machine( byte ep )
{
  switch( USBH_MSC_BOTXferParam.MSCState )
  { case USBH_MSC_TEST_UNIT_READY:     /* Answer to SCSI command TestUnitReady */
      USBH_MSC_BOTXferParam.MSCState= MSCaskForRequestSense();
    break;

    case USBH_MSC_REQUEST_SENSE:
      MSrespRequestSense();
      USBH_MSC_BOTXferParam.MSCState= MSCaskForReadCapacity10();
    break;

    case USBH_MSC_READ_CAPACITY10: /* Issue READ_CAPACITY10 SCSI command */
      MSCrespReadCapacity10();
      USBH_MSC_BOTXferParam.MSCState= MSCaskForModeSense6();
    break;

    case USBH_MSC_MODE_SENSE6:  /* Issue ModeSense6 SCSI command for detecting if device is write-protected */
      MSCrespModeSense6();
      USBH_MSC_BOTXferParam.MSCState= USBH_MSC_DEFAULT_APPLI_STATE;
    break;

/* Tell application about device available, give it a unique ID
 */
    case USBH_MSC_DEFAULT_APPLI_STATE:
      USBH_MSC_BOTXferParam.MSCState= USBH_MSC_IDLE;
      USBHclassSignaler.classMassStor= 1;
      usbHostGotUserApplication( USBHclassSignaler );  /* | 0x80 << MSC_Machine.hcNumIn | 0x80 << MSC_Machine.hcNumOut  );*/
    return( 0 ); /* do not fallout*/

    case USBH_MSC_READ10:
    case USBH_MSC_WRITE10:
      USBH_MSC_BOTXferParam.MSCState= USBH_MSC_IDLE;
    default: return( 0 );
  }

  USBH_MSC_XferStart( ep );
  return( 0 );
}

/**
  * @brief  USBHmscInterfaceInit
  *         Interface initialization for MSC class.
  * @retval
  */
hostClassLink USBHmscInterfaceInit( USBHdeviceRec * dev, void * handler )
{
  if ( dev->Itf_Desc->bInterfaceProtocol == MSC_PROTOCOL )
  { byte MSBulkInEp
       , MSBulkOutEp;
    USBHepDescRec * epList;

    for( epList= dev->Ep_Desc
       ; epList->bLength
       ; epList++ )
    { if ( epList->bEndpointAddress & 0x80 )
      {             MSBulkInEp    = epList->bEndpointAddress;
        MSC_Machine.MSBulkInEpSize= epList->wMaxPacketSize;
      }
      else
      {             MSBulkOutEp    = epList->bEndpointAddress;
        MSC_Machine.MSBulkOutEpSize= epList->wMaxPacketSize;
    } }

/* Open the new channels
 */

    MSC_Machine.hcNumOut=
     UHOSTopenChannel( MSBulkOutEp
                     , EPTYPE_BULK, MSC_Machine.MSBulkOutEpSize
                     , USBH_MSC_HandleBOTXferOut );

   MSC_Machine.hcNumIn=
    UHOSTopenChannel( MSBulkInEp
                    , EPTYPE_BULK, MSC_Machine.MSBulkInEpSize
                    , USBH_MSC_HandleBOTXferIn );

    USBH_MSC_ClassRequest();

    return( USBH_MSC_Handle );
  }

  return( 0 );
}
