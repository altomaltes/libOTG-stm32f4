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
 * @file usbd_msc_bot.c
 *
 * original draft from MCD Application Team
 */

#include "usb_dcd.h"
#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc_mem.h"

word MSC_BOT_DataLen;
byte MSC_BOT_State;
byte MSC_BOT_Status;

ALIGN_THIS( byte MSC_BOT_Data[MSC_MEDIA_PACKET] );
ALIGN_THIS( MSC_BOT_CBW_TypeDef MSC_BOT_cbw     );
ALIGN_THIS( MSC_BOT_CSW_TypeDef MSC_BOT_csw     );

/**
 * @brief  MSC_BOT_Init
 *         Initialize the BOT Process
 * @retval None
 */
void MSC_BOT_Init()
{ MSC_BOT_State = BOT_IDLE;
  MSC_BOT_Status= BOT_STATE_NORMAL;
  USBD_STORAGE_fops->Init( 0 );

  USBDepFlush( MSC_OUT_EP );
  USBDepFlush( MSC_IN_EP  );

  USBDepPrepareRx( MSC_OUT_EP            /* Prapare EP to Receive First BOT Cmd */
                  , (byte *)&MSC_BOT_cbw
                  , BOT_CBW_LENGTH );
}

/**
 * @brief  MSC_BOT_Reset
 *         Reset the BOT Machine
 * @retval  None
 */
void MSC_BOT_Reset( )
{ MSC_BOT_State = BOT_IDLE;
  MSC_BOT_Status= BOT_STATE_RECOVERY;

  USBDepPrepareRx( MSC_OUT_EP   /* Prapare EP to Receive First BOT Cmd */
                  , (byte *)&MSC_BOT_cbw
                  , BOT_CBW_LENGTH );
}

/**
 * @brief  MSC_BOT_DeInit
 *         Uninitialize the BOT Machine
 * @retval None
 */
void MSC_BOT_DeInit ()
{ MSC_BOT_State= BOT_IDLE;
}

/**
 * @brief  MSC_BOT_SendData
 *         Send the requested data
 * @param  buf: pointer to data buffer
 * @param  len: Data Length
 * @retval None
 */
static void  MSC_BOT_SendData( byte* buf
                             , word len )
{ len = MIN (MSC_BOT_cbw.dDataLength, len);
  MSC_BOT_csw.dDataResidue -= len;
  MSC_BOT_csw.bStatus = CSW_CMD_PASSED;
  MSC_BOT_State = BOT_SEND_DATA;

  USBDepTx( MSC_IN_EP, buf, len );
}

/**
 * @brief  MSC_BOT_Abort
 *         Abort the current transfer
 * @retval status
 */
static void  MSC_BOT_Abort()
{
  if (( MSC_BOT_cbw.bmFlags == 0)
  &&  ( MSC_BOT_cbw.dDataLength != 0)
  &&  ( MSC_BOT_Status == BOT_STATE_NORMAL ))
  { USBDepStall( MSC_OUT_EP );
  }
  USBDepStall( MSC_IN_EP);

  if( MSC_BOT_Status == BOT_STATE_ERROR )
  { USBDepPrepareRx( MSC_OUT_EP
                    , (byte *)&MSC_BOT_cbw
                    ,  BOT_CBW_LENGTH);
} }

/**
 * @brief  MSC_BOT_CBW_Decode
 *         Decode the CBW command and set the BOT state machine accordingtly
 * @retval None
 */
static void  MSC_BOT_CBW_Decode()
{
  MSC_BOT_csw.dTag = MSC_BOT_cbw.dTag;
  MSC_BOT_csw.dDataResidue = MSC_BOT_cbw.dDataLength;

  if (( USBDgetRxCount( MSC_OUT_EP ) != BOT_CBW_LENGTH )
  ||  ( MSC_BOT_cbw.dSignature != BOT_CBW_SIGNATURE )
  ||  ( MSC_BOT_cbw.bLUN      > 1 )
  ||  ( MSC_BOT_cbw.bCBLength < 1 )
  ||  ( MSC_BOT_cbw.bCBLength > 16 ))
  { SCSI_SenseCode( MSC_BOT_cbw.bLUN
                  , ILLEGAL_REQUEST
                  , INVALID_CDB );
    MSC_BOT_Status = BOT_STATE_ERROR;
    MSC_BOT_Abort();
  }
  else
  { if ( SCSI_ProcessCmd( MSC_BOT_cbw.bLUN
                        , MSC_BOT_cbw.CB ) < 0 )
    { MSC_BOT_Abort();
    }

    else if (( MSC_BOT_State != BOT_DATA_IN      )
         &&  ( MSC_BOT_State != BOT_DATA_OUT     )
         &&  ( MSC_BOT_State != BOT_LAST_DATA_IN )) /*Burst xfer handled internally*/
    { if ( MSC_BOT_DataLen > 0 )
      { MSC_BOT_SendData( MSC_BOT_Data
                        , MSC_BOT_DataLen );
      }
      else if (MSC_BOT_DataLen == 0)
      { MSC_BOT_SendCSW( CSW_CMD_PASSED );
} } } }

/**
 * @brief  MSC_BOT_DataIn
 *         Handle BOT IN data stage
 * @param  epnum: endpoint index
 * @retval None
 */
schar MSC_BOT_DataIn( byte epnum )
{ switch ( MSC_BOT_State )
  { case BOT_DATA_IN:
      if ( SCSI_ProcessCmd( MSC_BOT_cbw.bLUN
                          , MSC_BOT_cbw.CB ) < 0 )
      { MSC_BOT_SendCSW ( CSW_CMD_FAILED);
      }
    break;

    case BOT_SEND_DATA:
    case BOT_LAST_DATA_IN:
      MSC_BOT_SendCSW( CSW_CMD_PASSED );
    break;
  }
  return( 0 );
}

/**
 * @brief  MSC_BOT_DataOut
 *         Proccess MSC OUT data
 * @param  epnum: endpoint index
 * @retval None
 */
schar MSC_BOT_DataOut( byte epnum )
{ switch( MSC_BOT_State )
  { case BOT_IDLE:
      MSC_BOT_CBW_Decode();
    break;

    case BOT_DATA_OUT:
      if ( SCSI_ProcessCmd( MSC_BOT_cbw.bLUN
                          , MSC_BOT_cbw.CB ) < 0 )
      { MSC_BOT_SendCSW ( CSW_CMD_FAILED);
      }
    break;
  }
  return( 0 );
}

/**
 * @brief  MSC_BOT_SendCSW
 *         Send the Command Status Wrapper
 * @param  status : CSW status
 * @retval None
 */
void  MSC_BOT_SendCSW( byte CSW_Status )
{ MSC_BOT_csw.dSignature = BOT_CSW_SIGNATURE;
  MSC_BOT_csw.bStatus = CSW_Status;
  MSC_BOT_State = BOT_IDLE;

  USBDepTx( MSC_IN_EP
           , (byte *)&MSC_BOT_csw
           , BOT_CSW_LENGTH );

  /* Prepare EP to Receive next Cmd */
  USBDepPrepareRx( MSC_OUT_EP
                  , (byte *)&MSC_BOT_cbw
                  , BOT_CBW_LENGTH );

}

/**
 * @brief  MSC_BOT_CplClrFeature
 *         Complete the clear feature request
 * @param  epnum: endpoint index
 * @retval None
 */

void  MSC_BOT_CplClrFeature ( byte epnum )
{ if ( MSC_BOT_Status == BOT_STATE_ERROR )/* Bad CBW Signature */
  { USBDepStall( MSC_IN_EP);
    MSC_BOT_Status = BOT_STATE_NORMAL;
  }

  else if(((epnum & 0x80) == 0x80)
       && ( MSC_BOT_Status != BOT_STATE_RECOVERY))
  { MSC_BOT_SendCSW ( CSW_CMD_FAILED);
} }
