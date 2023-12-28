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
 * @file usbh_msc_bot.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

#define DEVICE_CHANCE_MSECS 100  /* Device reaction time on writes */


ALIGN_THIS( HostCBWPkt_TypeDef USBH_MSC_CBWData );
ALIGN_THIS( HostCSWPkt_TypeDef USBH_MSC_CSWData );

USBH_BOTXfer_TypeDef USBH_MSC_BOTXferParam;

static dword remainingDataLength;
static byte *datapointer;

void USBH_MSC_Init( )
{ USBH_MSC_CBWData.field.CBWSignature= USBH_MSC_BOT_CBW_SIGNATURE;
  USBH_MSC_CBWData.field.CBWTag      = USBH_MSC_BOT_CBW_TAG;
  USBH_MSC_CBWData.field.CBWLUN      = 0;

  USBH_MSC_BOTXferParam.MSCState= USBH_MSC_RESET;
}


/**
 * @brief  USBH_MSC_DecodeCSW
 *         This function decodes the CSW received by the device and updates the
 *         same to upper layer.
 * @param  None
 * @retval On success USBH_MSC_OK, on failure USBH_MSC_FAIL
 * @notes
 *     Refer to USB Mass-Storage Class : BOT (www.usb.org)
 *    6.3.1 Valid CSW Conditions :
 *     The host shall consider the CSW valid when:
 *     1. dCSWSignature is equal to 53425355h
 *     2. the CSW is 13 (Dh) bytes in length,
 *     3. dCSWTag matches the dCBWTag from the corresponding CBW.
 */

static byte USBH_MSC_DecodeCSW(  )
{ if ( HCD_GetXferCnt( MSC_Machine.hcNumIn ) != USBH_MSC_CSW_LENGTH )      /* Checking if the transfer length is different than 13 */
  { return( USBH_MSC_PHASE_ERROR );
  }

  if ( USBH_MSC_CSWData.field.CSWSignature != USBH_MSC_BOT_CSW_SIGNATURE )  /* Check validity of the CSW Signature and CSWStatus */
  { return( USBH_MSC_PHASE_ERROR );
  }

  if ( USBH_MSC_CSWData.field.CSWTag != USBH_MSC_CBWData.field.CBWTag )   /* Check Condition 1. dCSWSignature is equal to 53425355h */
  { return( USBH_MSC_PHASE_ERROR );
  }

/** Refer to USB Mass-Storage Class : BOT (www.usb.org) Section 6.7
 *
 *  (2) Hn < Di ( Host expects no data transfers,  Device intends to send data to the host)
 *  (3) Hn < Do ( Host expects no data transfers,  Device intends to receive data from the host)
 *  (7) Hi < Di ( Host expects to receive data from the device, Device intends to send data to the host)
 *  (8) Hi <> Do ( Host expects to receive data from the device, Device intends to receive data from the host)
 * (10) Ho <> Di (Host expects to send data to the device, Di Device intends to send data to the host)
 * (13) Ho < Do (Host expects to send data to the device, Device intends to receive data from the host)
 */
  if ( USBH_MSC_CSWData.field.CSWStatus != USBH_MSC_OK ) /* Check Condition 3. dCSWTag matches the dCBWTag from the corresponding CBW */
  { return( USBH_MSC_CSWData.field.CSWStatus );
  }

  return( USBH_MSC_OK );
}



/**
 * @brief  USBH_MSC_HandleBOTXfer
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 *
 */
/* BOT CSW stage */
/* NOTE: We cannot reset the BOTStallErrorCount here as it may come from the clearFeature from previous command */
static void USBH_MSC_HandleBOTreceive( byte ep )
{ USBH_MSC_BOTXferParam.pRxTxBuff = USBH_MSC_CSWData.CSWArray;
  USBH_MSC_BOTXferParam.DataLength= USBH_MSC_CSW_MAX_LENGTH;

  for( int index= USBH_MSC_CSW_LENGTH-1
     ;     index >= 0
     ;     index-- )
  { USBH_MSC_CSWData.CSWArray[ index ]= 0;
  }
//  USBH_MSC_CSWData.CSWArray[ 0 ]= 0;

  USBH_BulkReceiveData( USBH_MSC_BOTXferParam.pRxTxBuff
                      , USBH_MSC_CSW_MAX_LENGTH
                      , MSC_Machine.hcNumIn );
  USBH_MSC_BOTXferParam.BOTState= USBH_BOT_DECODE_CSW;
}

/**
 * @brief  USBH_MSC_HandleBOTXfer
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 *
 */
static void USBH_MSC_HandleDatain( byte ep )
{ { if ( remainingDataLength > MSC_Machine.MSBulkInEpSize )
    { USBH_BulkReceiveData( datapointer
                          , MSC_Machine.MSBulkInEpSize
                          , MSC_Machine.hcNumIn );

      remainingDataLength -= MSC_Machine.MSBulkInEpSize;
      datapointer         += MSC_Machine.MSBulkInEpSize;
    }

    else
    { USBH_BulkReceiveData( datapointer
                          , remainingDataLength
                          , MSC_Machine.hcNumIn );
      remainingDataLength= 0;  /* Reset this value and decode past receiving */
      USBH_MSC_BOTXferParam.BOTState= USBH_BOT_RECEIVE_CSW_STATE;
} } }

/*   Refer to USB Mass-Storage Class : BOT (www.usb.org)
 *  6.7.2 Host expects to receive data from the device
 *  3. On a STALL condition receiving data, then:
 *     The host shall accept the data received.
 *     The host shall clear the Bulk-In pipe.
 *  4. The host shall attempt to receive a CSW.
 *
 *     USBH_MSC_BOTXferParam.BOTStateBkp is used to switch to the Original
 *     state after the ClearFeature Command is issued.
 */



/**
 * @brief  USBH_MSC_HandleDataout
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 *
 */
static void USBH_MSC_HandleDataout( byte ep )
{ { mDelay( DEVICE_CHANCE_MSECS );   /* Give the device a chance */

    if ( remainingDataLength > MSC_Machine.MSBulkOutEpSize )
    { USBH_BulkSendData( datapointer
                       , MSC_Machine.MSBulkOutEpSize
                       , MSC_Machine.hcNumOut );
      datapointer= datapointer + MSC_Machine.MSBulkOutEpSize;
      remainingDataLength= remainingDataLength - MSC_Machine.MSBulkOutEpSize;
    }

    else
    { USBH_BulkSendData( datapointer
                       , remainingDataLength
                       , MSC_Machine.hcNumOut );
      remainingDataLength= 0;             /* Reset this value and keep in same state */
      USBH_MSC_HandleBOTreceive( ep );    /* Both send and receive */
} } }

/* Refer to USB Mass-Storage Class : BOT (www.usb.org)
   6.7.3 Ho - Host expects to send data to the device
  3. On a STALL condition sending data, then:
 " The host shall clear the Bulk-Out pipe.
   4. The host shall attempt to receive a CSW.

      The Above statement will do the clear the Bulk-Out pipe.
      The Below statement will help in Getting the CSW.

              USBH_MSC_BOTXferParam.BOTStateBkp is used to switch to the Original
              state after the ClearFeature Command is issued.
              */


/**
 * @brief  USBH_MSC_HandleBOTXferIn
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 *
 */
void USBH_MSC_HandleBOTXferIn( byte ep )
{ switch( USBH_MSC_BOTXferParam.BOTState )
  { case USBH_BOT_DECODE_CSW:
      USBH_MSC_DecodeCSW();             /*USBH_MSC_BOTXferParam.BOTXferStatus=*/
      USBH_MSC_Machine( ep );           /* next step */
    break;

    case USBH_BOT_RECEIVE_CSW_STATE:    /* Complete packet received */
      USBH_MSC_HandleBOTreceive( ep );  /* ACK ? */
    break;

    case USBH_BOT_DATAIN_STATE:         /* There is data to receive left*/
      USBH_MSC_HandleDatain( ep );
    break;

    case USBH_BOT_SENT_CBW:
    case USBH_BOT_DATAOUT_STATE:
    break;
} }



/**
 * @brief  USBH_MSC_HandleBOTXfer
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 */
void USBH_MSC_HandleBOTXferOut( byte ep )
{ byte xferDirection;

  { switch( USBH_MSC_BOTXferParam.BOTState )
    { case USBH_BOT_SENT_CBW:  /* Response from above */
      { xferDirection= USBH_MSC_CBWData.field.CBWFlags & USB_REQ_DIR_MASK; /* If the CBW Pkt is sent successful, then change the state */

        if ( USBH_MSC_CBWData.field.CBWTransferLength  )
        { remainingDataLength= USBH_MSC_CBWData.field.CBWTransferLength ;
          datapointer= USBH_MSC_BOTXferParam.pRxTxBuff;

          if ( xferDirection == USB_D2H )                           /* If there is Data Transfer Stage */
          { USBH_MSC_BOTXferParam.BOTState= USBH_BOT_DATAIN_STATE;  /* Data Direction is IN */
            USBH_MSC_HandleDatain( ep );
          }
          else
          { USBH_MSC_BOTXferParam.BOTState= USBH_BOT_DATAOUT_STATE; /* Data Direction is OUT */
            USBH_MSC_HandleDataout( ep );
        } }

        else                       /* No data transfer stage */
        { USBH_MSC_HandleBOTreceive( ep );
        }
      return;

      case USBH_BOT_DATAOUT_STATE:
        USBH_MSC_HandleDataout( ep );

      case USBH_BOT_DECODE_CSW:
      case USBH_BOT_DATAIN_STATE:
      case USBH_BOT_RECEIVE_CSW_STATE:
      return;
} } } }


/**
 * @brief  USBH_MSC_XferStart
 */
void USBH_MSC_XferStart( byte ep )
{ USBH_BulkSendData( USBH_MSC_CBWData.CBWArray
                   , USBH_MSC_BOT_CBW_PACKET_LENGTH
                   , MSC_Machine.hcNumOut );
  USBH_MSC_BOTXferParam.BOTState= USBH_BOT_SENT_CBW;
}




