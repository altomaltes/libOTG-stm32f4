/**
  ******************************************************************************
  * @file    usbd_msc_bot.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides all the BOT protocol core functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "./usbd_msc_bot.h"
#include "./usbd_msc.h"
#include "./usbd_msc_scsi.h"


/**
 * @brief  MSC_BOT_Init
 *         Initialize the BOT Process
 * @retval None
 */
short MSC_BOT_Init( MSCdriverRec * hmsc  )
{ hmsc->bot_state  = USBD_BOT_IDLE;
  hmsc->bot_status = USBD_BOT_STATUS_NORMAL;

  hmsc->scsi_sense_tail=
  hmsc->scsi_sense_head= 0;

//  ((USBDdriverRec *)pdev->pUserData)->Init(0);  çççç

  USBDepFlush( hmsc->epOt );
  USBDepFlush( hmsc->epIn  );

/* Prapare EP to Receive First BOT Cmd
 */
  return( USBDepPrepareRx( hmsc->epOt            /* Prapare EP to Receive First BOT Cmd */
                         , &hmsc->cbw
                         , USBD_BOT_CBW_LENGTH ));
}

/**
 * @brief  MSC_BOT_Reset
 *         Reset the BOT Machine
 * @retval  None
 */
short MSC_BOT_Reset( MSCdriverRec * hmsc  )
{ hmsc->bot_state = USBD_BOT_IDLE;
  hmsc->bot_status= USBD_BOT_STATUS_RECOVERY;

  return( USBDepPrepareRx( hmsc->epOt            /* Prapare EP to Receive First BOT Cmd */
                         , &hmsc->cbw
                         , USBD_BOT_CBW_LENGTH ));
}

/**
 * @brief  MSC_BOT_DeInit
 *         Deinitialize the BOT Machine
 * @retval None
 */
short MSC_BOT_DeInit( MSCdriverRec * hmsc  )
{ hmsc->bot_state= USBD_BOT_IDLE;
  return( 0 );
}

/**
 * @brief  MSC_BOT_SendData
 *         Send the requested data
 * @param  buf: pointer to data buffer
 * @param  len: Data Length
 * @retval None
 */
static void  MSC_BOT_SendData( MSCdriverRec * hmsc
                             , byte         * buf
                             , word           len )
{ len = MIN( hmsc->cbw.dDataLength, len );
  hmsc->csw.dDataResidue -= len;
  hmsc->csw.bStatus       = USBD_CSW_CMD_PASSED;
  hmsc->bot_state         = USBD_BOT_SEND_DATA;

  USBDepTx( hmsc->epIn, buf, len );
}

/**
 * @brief  MSC_BOT_Abort
 *         Abort the current transfer
 * @retval status
 */
static void  MSC_BOT_Abort( MSCdriverRec * hmsc  )
{ if (( hmsc->cbw.bFlags == 0)
  &&  ( hmsc->cbw.dDataLength != 0)
  &&  ( hmsc->bot_status == USBD_BOT_STATUS_NORMAL) )
  { USBDepStall( hmsc->epOt );
  }
  USBDepStall( hmsc->epIn);

  if ( hmsc->bot_status == USBD_BOT_STATUS_ERROR )
  { USBDepPrepareRx( hmsc->epOt            /* Prapare EP to Receive First BOT Cmd */
                  , (byte *)&hmsc->cbw
                  , USBD_BOT_CBW_LENGTH );
 } }


/**
 * @brief  MSC_BOT_SendCSW
 *         Send the Command Status Wrapper
 * @param  status : CSW status
 * @retval None
 */
short  MSC_BOT_SendCSW( MSCdriverRec * hmsc , dword CSW_Status )
{ hmsc->csw.dSignature= USBD_BOT_CSW_SIGNATURE;
  hmsc->csw.bStatus   = CSW_Status;
  hmsc->bot_state     = USBD_BOT_IDLE;

  USBDepTx( hmsc->epIn
          , (byte *)&hmsc->csw
          , USBD_BOT_CSW_LENGTH );

  return( USBDepPrepareRx( hmsc->epOt           /* Prepare EP to Receive next Cmd */
                         , &hmsc->cbw
                         , USBD_BOT_CBW_LENGTH ));
}


/**
 * @brief  MSC_BOT_CBW_Decode
 *         Decode the CBW command and set the BOT state machine accordingly
 * @retval None
 */
static void  MSC_BOT_CBW_Decode( MSCdriverRec * hmsc )
{ hmsc->csw.dTag        = hmsc->cbw.dTag;
  hmsc->csw.dDataResidue= hmsc->cbw.dDataLength;

  if  (( USBDgetRxCount( hmsc->epOt) != USBD_BOT_CBW_LENGTH )
    || ( hmsc->cbw.dSignature != USBD_BOT_CBW_SIGNATURE )
    || ( hmsc->cbw.bLUN       > 1  )
    || ( hmsc->cbw.bCmdLength < 1  )
    || ( hmsc->cbw.bCmdLength > 16 ))
  { SCSIsenseCode( hmsc->cbw.bLUN, hmsc, ILLEGAL_REQUEST, INVALID_CDB );

    hmsc->bot_status= USBD_BOT_STATUS_ERROR;
    MSC_BOT_Abort( hmsc );
  }
  else
  { if ( SCSIprocessCmd( hmsc->cbw.bLUN
                       , hmsc
                       , hmsc->cbw.bCmdData ) < 0 )
    { if ( hmsc->bot_state == USBD_BOT_NO_DATA )
      { MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_FAILED );
      }
      else
      { MSC_BOT_Abort( hmsc );
      }
      return;
    }

    switch( hmsc->bot_state )
    { case USBD_BOT_DATA_OUT:
      case USBD_BOT_DATA_IN:
      case USBD_BOT_LAST_DATA_IN:   /* Burst xfer handled internally  */
      return;
    }

    if ( hmsc->bot_data_length > 0 )             /* Send response */
    { MSC_BOT_SendData( hmsc
                      , hmsc->bot_data
                      , hmsc->bot_data_length );
    }
    else
    { MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_PASSED );    /* Give ACK */
} } }

/**
 * @brief  MSC_BOT_CplClrFeature
 *         Complete the clear feature request
 * @param  epnum: endpoint index
 * @retval None
 */
short  MSC_BOT_CplClrFeature( MSCdriverRec * hmsc, dword epnum )
{ if ( hmsc->bot_status == USBD_BOT_STATUS_ERROR )/* Bad CBW Signature */
  { USBDepStall( hmsc->epIn );
    hmsc->bot_status = USBD_BOT_STATUS_NORMAL;
  }

  else if (( epnum & EPDIR_IN )
       && ( hmsc->bot_status != USBD_BOT_STATUS_RECOVERY ))
  { MSC_BOT_SendCSW ( hmsc, USBD_CSW_CMD_FAILED);
  }

  return( 0 );
}



/**
 * @brief  MSC_BOT_DataOut
 *         Process MSC OUT data
 * @param  epnum: endpoint index
 * @retval None
 */
short MSC_BOT_DataOut( dword epnum, void * args1 )
{ MSCdriverRec * hmsc= (MSCdriverRec *)args1;

   switch( hmsc->bot_state )
  { case USBD_BOT_IDLE:
      MSC_BOT_CBW_Decode( hmsc );
    break;

    case USBD_BOT_DATA_OUT:
      if ( SCSIprocessCmd( hmsc->cbw.bLUN
                         , hmsc
                         , hmsc->cbw.bCmdData ) < 0 )
      { MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_FAILED );
      }
    break;

    default:
    break;
  }
  return( 0 );
}

/**
 * @brief  MSC_BOT_DataIn
 *         Handle BOT IN data stage
 * @param  epnum: endpoint index
 * @retval None
 */
short MSC_BOT_DataIn( dword epnum, void * args1 )
{ MSCdriverRec * hmsc= (MSCdriverRec *)args1;

  switch( hmsc->bot_state )
  { case USBD_BOT_DATA_IN:
      if ( SCSIprocessCmd( hmsc->cbw.bLUN
                         , hmsc
                         , hmsc->cbw.bCmdData ) < 0 )
      { MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_FAILED );
      }
    break;

    case USBD_BOT_DATA_OUT:
    case USBD_BOT_LAST_DATA_IN:
    case USBD_BOT_SEND_DATA:
      MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_PASSED );
    break;

    default:
    break;
  }
  return( 0 );
}

