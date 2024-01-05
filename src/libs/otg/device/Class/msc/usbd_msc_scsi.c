/**
  ******************************************************************************
  * @file    usbd_msc_scsi.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides all the USBD SCSI layer functions.
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

#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_data.h"

#include "usbd_msc.h"


/**
 * @brief  SCSI_TestUnitReady
 *         Process SCSI Test Unit Ready Command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_TestUnitReady( byte   lun
                               , byte * params )
{ if ( hmsc.cbw.dDataLength ) /* case 9 : Hi > D0 */
  { SCSI_SenseCode( hmsc.cbw.bLUN
                  , ILLEGAL_REQUEST
                  , INVALID_CDB );
    return( -1 );
  }

  if ( stor.IsReady( lun ))
  { SCSI_SenseCode( lun
                  , NOT_READY
                  , MEDIUM_NOT_PRESENT);
    hmsc.bot_state= USBD_BOT_NO_DATA;
    return( -1 );
  }

  hmsc.bot_data_length = 0;
  return( 0 );
}

/**
 * @brief  SCSI_Inquiry
 *         Process Inquiry command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_Inquiry( byte lun
                         , byte * params)
{ byte* pPage;
  word len;

  if (params[1] & 0x01)/*Evpd is set*/
  { pPage = (byte *)MSC_Page00_Inquiry_Data;
    len = LENGTH_INQUIRY_PAGE00;
  }
  else
  { pPage = (byte *)&stor.pInquiry[ lun * STANDARD_INQUIRY_DATA_LEN ];
    len = pPage[4] + 5;

    if ( params[4] <= len )
    { len = params[4];
  } }

  hmsc.bot_data_length= len;

  while ( len )
  { len--;
    hmsc.bot_data[len]= pPage[ len ];
  }

  return( 0 );
}

/**
 * @brief  SCSI_ReadCapacity10
 *         Process Read Capacity 10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_ReadCapacity10( byte lun
                                , byte *params)
{ if ( stor.GetCapacity(lun, &hmsc.scsi_blk_nbr, &hmsc.scsi_blk_size) != 0)
  { SCSI_SenseCode( lun
                    , NOT_READY
                    , MEDIUM_NOT_PRESENT );
    return( -1 );
  }
  else
  { hmsc.bot_data[ 0 ] = (byte)((hmsc.scsi_blk_nbr - 1) >> 24);
    hmsc.bot_data[ 1 ] = (byte)((hmsc.scsi_blk_nbr - 1) >> 16);
    hmsc.bot_data[ 2 ] = (byte)((hmsc.scsi_blk_nbr - 1) >>  8);
    hmsc.bot_data[ 3 ] = (byte)( hmsc.scsi_blk_nbr - 1);

    hmsc.bot_data[ 4 ] = (byte)(hmsc.scsi_blk_size >>  24);
    hmsc.bot_data[ 5 ] = (byte)(hmsc.scsi_blk_size >>  16);
    hmsc.bot_data[ 6 ] = (byte)(hmsc.scsi_blk_size >>  8);
    hmsc.bot_data[ 7 ] = (byte)(hmsc.scsi_blk_size);

    hmsc.bot_data_length = 8;
    return( 0 );
} }

/**
 * @brief  SCSIreadFmtCap
 *         Process Read Format Capacity command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSIreadFmtCap( byte lun, byte *params)
{ word blk_size;
  dword blk_nbr;
  word i;

  for( i= 0
     ; i  < 12
     ; i ++ )
  { hmsc.bot_data[ i ] = 0;
  }

  if ( stor.GetCapacity(lun, &blk_nbr, &blk_size) )
  { SCSI_SenseCode( lun
                  , NOT_READY
                  , MEDIUM_NOT_PRESENT );
    return( -1 );
  }
  else
  { hmsc.bot_data[  3 ] = 0x08;
    hmsc.bot_data[  4 ] = (byte)((blk_nbr - 1) >> 24);
    hmsc.bot_data[  5 ] = (byte)((blk_nbr - 1) >> 16);
    hmsc.bot_data[  6 ] = (byte)((blk_nbr - 1) >>  8);
    hmsc.bot_data[  7 ] = (byte) (blk_nbr - 1);

    hmsc.bot_data[  8 ] = 0x02;
    hmsc.bot_data[  9 ] = (byte)(blk_size >>  16);
    hmsc.bot_data[ 10 ] = (byte)(blk_size >>  8);
    hmsc.bot_data[ 11 ] = (byte)(blk_size);

    hmsc.bot_data_length = 12;
    return( 0 );
} }

/**
 * @brief  SCSI_ModeSense6
 *         Process Mode Sense6 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_ModeSense6( byte lun, byte * params)
{ word len= hmsc.bot_data_length= 0x08;

  while( len )
  { len--;
    hmsc.bot_data[ len ]= MSC_Mode_Sense6_data[ len ];
  }
  return( 8 );
}

/**
 * @brief  SCSI_ModeSense10
 *         Process Mode Sense10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_ModeSense10( byte lun, byte *params)
{ word len= hmsc.bot_data_length=  8;

  while( len )
  { len--;
    hmsc.bot_data[ len ]= MSC_Mode_Sense10_data[ len ];
  }
  return( 8 );
}

/**
 * @brief  SCSI_RequestSense
 *         Process Request Sense command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_RequestSense ( byte lun, byte *params )
{ byte i;

  for(i=0 ; i < REQUEST_SENSE_DATA_LEN ; i++)
  { hmsc.bot_data[i] = 0;
  }

  hmsc.bot_data[0] = 0x70;
  hmsc.bot_data[7] = REQUEST_SENSE_DATA_LEN - 6;

  if((hmsc.scsi_sense_head != hmsc.scsi_sense_tail))
  { hmsc.bot_data[2]     = hmsc.scsi_sense[hmsc.scsi_sense_head].Skey;
    hmsc.bot_data[12]    = hmsc.scsi_sense[hmsc.scsi_sense_head].w.b.ASCQ;
    hmsc.bot_data[13]    = hmsc.scsi_sense[hmsc.scsi_sense_head].w.b.ASC;
    hmsc.scsi_sense_head++;

    if (hmsc.scsi_sense_head == SENSE_LIST_DEEPTH)
    { hmsc.scsi_sense_head = 0;
  } }

  hmsc.bot_data_length = REQUEST_SENSE_DATA_LEN;

  if ( params[4] <= REQUEST_SENSE_DATA_LEN )
  { hmsc.bot_data_length = params[4];
  }
  return( 0 );
}

/**
 * @brief  SCSI_SenseCode
 *         Load the last error code in the error list
 * @param  lun: Logical unit number
 * @param  sKey: Sense Key
 * @param  ASC: Additional Sense Key
 * @retval none
 */
void SCSI_SenseCode( byte lun, byte sKey, byte ASC )
{ hmsc.scsi_sense[ hmsc.scsi_sense_tail ].Skey  = sKey;
  hmsc.scsi_sense[ hmsc.scsi_sense_tail ].w.ASC = ASC << 8;
  hmsc.scsi_sense_tail++;

  if ( hmsc.scsi_sense_tail == SENSE_LIST_DEEPTH )
  { hmsc.scsi_sense_tail = 0;
} }


/**
 * @brief  SCSI_StartStopUnit
 *         Process Start Stop Unit command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_StartStopUnit(  byte lun, byte *params )
{ hmsc.bot_data_length = 0;
  return 0;
}

/**
 * @brief  SCSI_CheckAddressRange
 *         Check address range
 * @param  lun: Logical unit number
 * @param  blk_offset: first block address
 * @param  blk_nbr: number of block to be processed
 * @retval status
 */
static schar SCSI_CheckAddressRange( byte lun
                                     , dword blk_offset
                                     , word blk_nbr)
{ if ((blk_offset + blk_nbr) > hmsc.scsi_blk_nbr )
  { SCSI_SenseCode(lun,
                   ILLEGAL_REQUEST,
                   ADDRESS_OUT_OF_RANGE);
    return -1;
  }
  return 0;
}

/**
 * @brief  SCSI_ProcessRead
 *         Handle Read Process
 * @param  lun: Logical unit number
 * @retval status
 */
static schar SCSI_ProcessRead( byte lun )
{ dword len= MIN( hmsc.scsi_blk_len, MSC_MEDIA_PACKET );

  if  ( stor.Read( lun, hmsc.bot_data
                 , hmsc.scsi_blk_addr / hmsc.scsi_blk_size
                 , len                / hmsc.scsi_blk_size ) < 0 )
  { SCSI_SenseCode( lun, HARDWARE_ERROR, UNRECOVERED_READ_ERROR );
    return( -1 );
  }

  USBDepTx( MSC_EPIN_ADDR
          , hmsc.bot_data
          , len );
  hmsc.scsi_blk_addr   += len;
  hmsc.scsi_blk_len    -= len;
  hmsc.csw.dDataResidue-= len;   /* case 6 : Hi = Di */
//  hmsc.bot_state         = USBD_BOT_SEND_DATA;

  hmsc.bot_state= hmsc.scsi_blk_len ? USBD_BOT_DATA_IN   /* more to send */
                                    : USBD_BOT_LAST_DATA_IN;
  return( 0 );
}

/**
 * @brief  SCSI_Read10
 *         Process Read10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_Read10( byte lun, byte * params )
{ if ( hmsc.bot_state == USBD_BOT_IDLE )  /* Idle */
  { if ((hmsc.cbw.bmFlags & 0x80) != 0x80)     /* case 10 : Ho <> Di */
    { SCSI_SenseCode( hmsc.cbw.bLUN
                    , ILLEGAL_REQUEST
                    , INVALID_CDB );
      return( -1 );
    }

    if ( stor.IsReady( lun ))
    { SCSI_SenseCode( lun
                    , NOT_READY
                    , MEDIUM_NOT_PRESENT);
      return( -1 );
    }

    hmsc.scsi_blk_addr= (params[2] << 24)
                      | (params[3] << 16)
                      | (params[4] <<  8)
                      |  params[5];

    hmsc.scsi_blk_len =  (params[7] <<  8)
                      |   params[8];

    if ( SCSI_CheckAddressRange( lun
                               , hmsc.scsi_blk_addr
                               , hmsc.scsi_blk_len ) < 0 )
    { return( -1 );
    }

    hmsc.scsi_blk_addr *= hmsc.scsi_blk_size;
    hmsc.scsi_blk_len  *= hmsc.scsi_blk_size;

/* cases 4,5 : Hi <> Dn
 */
    if ( hmsc.cbw.dDataLength != hmsc.scsi_blk_len )
    { SCSI_SenseCode( hmsc.cbw.bLUN
                    , ILLEGAL_REQUEST
                    , INVALID_CDB );
      return -1;
  } }

  hmsc.bot_data_length= MSC_MEDIA_PACKET;

  return( SCSI_ProcessRead( lun ) );
}

/**
 * @brief  SCSI_ProcessWrite
 *         Handle Write Process
 * @param  lun: Logical unit number
 * @retval status
 */

static schar SCSI_ProcessWrite( byte lun )
{ dword len;

  len = MIN(hmsc.scsi_blk_len, MSC_MEDIA_PACKET);

  if ( stor.Write( lun
                 , hmsc.bot_data
                 , hmsc.scsi_blk_addr / hmsc.scsi_blk_size
                 , len                / hmsc.scsi_blk_size ) < 0 )
  { SCSI_SenseCode( lun
                  , HARDWARE_ERROR
                  , WRITE_FAULT );
    return -1;
  }


  hmsc.scsi_blk_addr  += len;
  hmsc.scsi_blk_len   -= len;

  hmsc.csw.dDataResidue -= len; /* case 12 : Ho = Do */

  if ( hmsc.scsi_blk_len == 0 )
  { MSC_BOT_SendCSW( USBD_CSW_CMD_PASSED );
  }
  else
  { USBDepPrepareRx( MSC_EPOUT_ADDR    /* Prepare EP to Receive next packet */
                   , hmsc.bot_data
                   , MIN( hmsc.scsi_blk_len, MSC_MEDIA_PACKET ));
  }

  return 0;
}

/**
 * @brief  SCSI_Write10
 *         Process Write10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_Write10( byte lun, byte *params )
{ if (hmsc.bot_state == USBD_BOT_IDLE) /* Idle */
  { if ( (hmsc.cbw.bmFlags & 0x80) == 0x80 )    /* case 8 : Hi <> Do */
    { SCSI_SenseCode( hmsc.cbw.bLUN
                    , ILLEGAL_REQUEST
                    , INVALID_CDB );
      return -1;
    }

/* Check whether Media is ready
 */
    if ( stor.IsReady(lun) )
    { SCSI_SenseCode( lun
                    , NOT_READY
                    , MEDIUM_NOT_PRESENT );
      return -1;
    }

/* Check If media is write-protected
 */
    if(stor.IsWriteProtected(lun) )
    { SCSI_SenseCode(lun,
                     NOT_READY,
                     WRITE_PROTECTED);
      return -1;
    }


    hmsc.scsi_blk_addr = (params[2] << 24)
                       | (params[3] << 16)
                       | (params[4] <<  8)
                       | params[5];

    hmsc.scsi_blk_len = (params[7] <<  8)
                      |  params[8];

/* check if LBA address is in the right range
 */
    if ( SCSI_CheckAddressRange( lun
                               , hmsc.scsi_blk_addr
                               , hmsc.scsi_blk_len) < 0)
    { return -1; /* error */
    }

    hmsc.scsi_blk_addr *= hmsc.scsi_blk_size;
    hmsc.scsi_blk_len  *= hmsc.scsi_blk_size;

    /* cases 3,11,13 : Hn,Ho <> D0 */
    if (hmsc.cbw.dDataLength != hmsc.scsi_blk_len)
    { SCSI_SenseCode(hmsc.cbw.bLUN,
                     ILLEGAL_REQUEST,
                     INVALID_CDB);
      return -1;
    }

/* Prepare EP to receive first data packet
 */
    hmsc.bot_state= USBD_BOT_DATA_OUT;
    USBDepPrepareRx( MSC_EPOUT_ADDR
                   , hmsc.bot_data
                   , MIN (hmsc.scsi_blk_len, MSC_MEDIA_PACKET));
  }
  else /* Write Process ongoing */
  { return SCSI_ProcessWrite( lun);
  }
  return 0;
}


/**
 * @brief  SCSI_Verify10
 *         Process Verify10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_Verify10( byte lun
                          , byte *params )
{ if ((params[1]& 0x02) == 0x02)
  { SCSI_SenseCode( lun
                    , ILLEGAL_REQUEST
                    , INVALID_FIELED_IN_COMMAND );
    return -1; /* Error, Verify Mode Not supported*/
  }

  if ( SCSI_CheckAddressRange( lun
                             , hmsc.scsi_blk_addr
                             , hmsc.scsi_blk_len) < 0 )
  { return -1; /* error */
  }
  hmsc.bot_data_length = 0;
  return 0;
}

/**
 * @brief  SCSI_ProcessCmd
 *         Process SCSI commands
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
 void x();

schar SCSI_ProcessCmd( byte lun
                     , byte * params )
{
  switch ( params[0] )
  { case SCSI_INQUIRY:         return( SCSI_Inquiry       ( lun, params ));
    case SCSI_READ_FMT_CAP:    return( SCSIreadFmtCap     ( lun, params ));
    case SCSI_READ_CAPACITY10: return( SCSI_ReadCapacity10( lun, params ));
    case SCSI_READ10:          return( SCSI_Read10        ( lun, params ));

    case SCSI_TEST_UNIT_READY: x(); return( SCSI_TestUnitReady ( lun, params ));
    case SCSI_REQUEST_SENSE:   x(); return( SCSI_RequestSense  ( lun, params ));
    case SCSI_START_STOP_UNIT: x(); return( SCSI_StartStopUnit ( lun, params ));
    case SCSI_ALLOW_REMOVAL:   x(); return( SCSI_StartStopUnit ( lun, params ));

    case SCSI_MODE_SENSE6:
       return( SCSI_ModeSense6    ( lun, params ));

    case SCSI_MODE_SENSE10:    x(); return( SCSI_ModeSense10   ( lun, params ));
    case SCSI_WRITE10:         x(); return( SCSI_Write10       ( lun, params ));
    case SCSI_VERIFY10:        x(); return( SCSI_Verify10      ( lun, params ));

    default:      SCSI_SenseCode( lun, ILLEGAL_REQUEST, INVALID_CDB );  return -1;
   break;
} }

int a; void x()
{ a++;
}
