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
#include "usbd_msc_data.h"

#include "usbd_msc.h"


/**
 * @brief  SCSIsenseCode
 *         Load the last error code in the error list
 * @param  lun: Logical unit number
 * @param  sKey: Sense Key
 * @param  ASC: Additional Sense Key
 * @retval none
 */
void SCSIsenseCode( byte lun, MSCdriverRec * drv
                  , byte sKey, byte ASC )
{ drv->scsi_sense[ drv->scsi_sense_tail ].Skey  = sKey;
  drv->scsi_sense[ drv->scsi_sense_tail ].w.ASC = ASC << 8;
  drv->scsi_sense_tail++;

  if ( drv->scsi_sense_tail == SENSE_LIST_DEEPTH )
  { drv->scsi_sense_tail= 0;
} }

/**
 * @brief  SCSI_TestUnitReady
 *         Process SCSI Test Unit Ready Command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_TestUnitReady( byte   lun
                               , byte * params
                               , MSCdriverRec * drv )
{ if ( drv->cbw.dDataLength ) /* case 9 : Hi > D0 */
  { SCSIsenseCode( drv->cbw.bLUN, drv
                 , ILLEGAL_REQUEST
                 , INVALID_CDB );
    return( -1 );
  }

  if ( drv->Ioctl( USB_IOCTL_ISREADY | lun ))
  { SCSIsenseCode( lun, drv
                 , NOT_READY
                 , MEDIUM_NOT_PRESENT );
    drv->bot_state= USBD_BOT_NO_DATA;
    return( -1 );
  }

  drv->bot_data_length = 0;
  return( 0 );
}

/**
 * @brief  SCSI_Inquiry
 *         Process Inquiry command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_Inquiry( byte lun
                         , byte         * params
                         , MSCdriverRec * drv )
{ byte* pPage;
  word len;

  if (params[1] & 0x01)/*Evpd is set*/
  { pPage = (byte *)MSC_Page00_Inquiry_Data;
    len = LENGTH_INQUIRY_PAGE00;
  }
  else
  {
    drv->Ioctl( USB_IOCTL_INQUIRY | lun, &pPage );
    len = pPage[4] + 5;

    if ( params[4] <= len )
    { len = params[4];
  } }

  drv->bot_data_length= len;

  while ( len )
  { len--;
    drv->bot_data[len]= pPage[ len ];
  }

  return( 0 );
}

/**
 * @brief  SCSIreadCapacity10
 *         Process Read Capacity 10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSIreadCapacity10( byte lun
                               , byte * params
                               , MSCdriverRec * drv )
{ if ( drv->Ioctl( USB_IOCTL_GETCAP | lun
                 , &drv->scsi_blk_nbr, &drv->scsi_blk_size ))
  { SCSIsenseCode( lun, drv
                 , NOT_READY
                 , MEDIUM_NOT_PRESENT );
    return( -1 );
  }
  else
  { drv->bot_data[ 0 ] = (byte)(( drv->scsi_blk_nbr - 1) >> 24);
    drv->bot_data[ 1 ] = (byte)(( drv->scsi_blk_nbr - 1) >> 16);
    drv->bot_data[ 2 ] = (byte)(( drv->scsi_blk_nbr - 1) >>  8);
    drv->bot_data[ 3 ] = (byte)(  drv->scsi_blk_nbr - 1);

    drv->bot_data[ 4 ] = (byte)( drv->scsi_blk_size >> 24);
    drv->bot_data[ 5 ] = (byte)( drv->scsi_blk_size >> 16);
    drv->bot_data[ 6 ] = (byte)( drv->scsi_blk_size >>  8);
    drv->bot_data[ 7 ] = (byte)( drv->scsi_blk_size);

    drv->bot_data_length = 8;
    return( 0 );
} }

/**
 * @brief  SCSIreadFmtCap
 *         Process Read Format Capacity command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSIreadFmtCap( byte lun, byte * params
                           , MSCdriverRec   * drv )
{ word i;

  for( i= 0
     ; i  < 12
     ; i ++ )
  { drv->bot_data[ i ] = 0;
  }

  if ( drv->Ioctl( USB_IOCTL_GETCAP | lun
                 , &drv->scsi_blk_nbr
                 , &drv->scsi_blk_size ))
  { SCSIsenseCode( lun, drv
                 , NOT_READY
                 , MEDIUM_NOT_PRESENT );
    return( -1 );
  }
  else
  { drv->bot_data[  3 ] = 0x08;
    drv->bot_data[  4 ] = (byte)((drv->scsi_blk_nbr - 1) >> 24);
    drv->bot_data[  5 ] = (byte)((drv->scsi_blk_nbr - 1) >> 16);
    drv->bot_data[  6 ] = (byte)((drv->scsi_blk_nbr - 1) >>  8);
    drv->bot_data[  7 ] = (byte) (drv->scsi_blk_nbr - 1);

    drv->bot_data[  8 ] = 0x02;
    drv->bot_data[  9 ] = (byte)(drv->scsi_blk_size >>  16);
    drv->bot_data[ 10 ] = (byte)(drv->scsi_blk_size >>  8);
    drv->bot_data[ 11 ] = (byte)(drv->scsi_blk_size);

    drv->bot_data_length = 12;
    return( 0 );
} }

/**
 * @brief  SCSI_ModeSense6
 *         Process Mode Sense6 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_ModeSense6( byte lun, byte * params
                            , MSCdriverRec   * drv )
{ word len= drv->bot_data_length= 0x08;

  while( len )
  { len--;
    drv->bot_data[ len ]= MSC_Mode_Sense6_data[ len ];
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
static short SCSI_ModeSense10( byte lun
                             , MSCdriverRec * drv )
{ word len= drv->bot_data_length=  8;

  while( len )
  { len--;
    drv->bot_data[ len ]= MSC_Mode_Sense10_data[ len ];
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
static short SCSI_RequestSense( byte lun, byte * params
                              , MSCdriverRec * drv )
{ byte i;

  for(i=0 ; i < REQUEST_SENSE_DATA_LEN ; i++)
  { drv->bot_data[i] = 0;
  }

  drv->bot_data[0] = 0x70;
  drv->bot_data[7] = REQUEST_SENSE_DATA_LEN - 6;

  if ((drv->scsi_sense_head != drv->scsi_sense_tail))
  { drv->bot_data[2]     = drv->scsi_sense[ drv->scsi_sense_head ].Skey;
    drv->bot_data[12]    = drv->scsi_sense[ drv->scsi_sense_head ].w.b.ASCQ;
    drv->bot_data[13]    = drv->scsi_sense[ drv->scsi_sense_head ].w.b.ASC;
    drv->scsi_sense_head++;

    if (drv->scsi_sense_head == SENSE_LIST_DEEPTH)
    { drv->scsi_sense_head = 0;
  } }

  drv->bot_data_length = REQUEST_SENSE_DATA_LEN;

  if ( params[4] <= REQUEST_SENSE_DATA_LEN )
  { drv->bot_data_length = params[4];
  }
  return( 0 );
}

/**
 * @brief  SCSI_StartStopUnit
 *         Process Start Stop Unit command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_StartStopUnit( byte lun
                               , MSCdriverRec * drv )
{ drv->bot_data_length = 0;
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
                                   , MSCdriverRec * drv )
{ if ( (drv->scsi_blk_addr + drv->scsi_blk_len) > drv->scsi_blk_nbr )
  { SCSIsenseCode( lun, drv
                 , ILLEGAL_REQUEST
                 , ADDRESS_OUT_OF_RANGE );
    return -1;
  }
  return 0;
}



/**
 * @brief  SCSIprocessRead
 *         Handle Read Process
 * @param  lun: Logical unit number
 * @retval status
 */
static schar SCSIprocessRead( byte lun
                            , MSCdriverRec * hmsc )
{ dword len= MIN( hmsc->scsi_blk_len, MSC_MEDIA_PACKET );

  if  ( hmsc->Read( lun, hmsc->bot_data
                 , hmsc->scsi_blk_addr / hmsc->scsi_blk_size
                 , len                / hmsc->scsi_blk_size ) < 0 )
  { SCSIsenseCode( lun, hmsc
                 , HARDWARE_ERROR, UNRECOVERED_READ_ERROR );
    return( -1 );
  }

  USBDepTx( hmsc->epIn
          , hmsc->bot_data
          , len  );  // !!!
  hmsc->scsi_blk_addr   += len;
  hmsc->scsi_blk_len    -= len;
  hmsc->csw.dDataResidue-= len;   /* case 6 : Hi = Di */

  hmsc->bot_state= hmsc->scsi_blk_len ? USBD_BOT_DATA_IN   /* more to send */
                                    : USBD_BOT_LAST_DATA_IN;

  return( 0 );
}

/**
 * @brief  SCSIread10
 *         Process Read10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSIread10( byte lun, byte * params
                       , MSCdriverRec * drv )
{ if ( drv->bot_state == USBD_BOT_IDLE )  /* Idle */
  { if ((drv->cbw.bFlags & 0x80) != 0x80)     /* case 10 : Ho <> Di */
    { SCSIsenseCode( drv->cbw.bLUN, drv
                   , ILLEGAL_REQUEST
                   , INVALID_CDB );
      return( -1 );
    }

    if ( drv->Ioctl( USB_IOCTL_ISREADY | lun ) )
    { SCSIsenseCode( lun, drv
                   , NOT_READY
                   , MEDIUM_NOT_PRESENT );
      return( -1 );
    }

    drv->scsi_blk_addr= (params[2] << 24)
                      | (params[3] << 16)
                      | (params[4] <<  8)
                      |  params[5];

    drv->scsi_blk_len =  (params[7] <<  8)
                      |   params[8] ;

    if ( SCSI_CheckAddressRange( lun
                               , drv ) < 0 )
    { return( -1 );
    }

    drv->scsi_blk_addr *= drv->scsi_blk_size;
    drv->scsi_blk_len  *= drv->scsi_blk_size;

/* cases 4,5 : Hi <> Dn
 */
    if ( drv->cbw.dDataLength != drv->scsi_blk_len )
    { SCSIsenseCode( drv->cbw.bLUN, drv
                   , ILLEGAL_REQUEST
                   , INVALID_CDB );
      return -1;
  } }

  drv->bot_data_length= MSC_MEDIA_PACKET;

  return( SCSIprocessRead( lun, drv ) );
}

/**
 * @brief  SCSI_ProcessWrite
 *         Handle Write Process
 * @param  lun: Logical unit number
 * @retval status
 */

static short SCSI_ProcessWrite( byte lun
                              , MSCdriverRec * hmsc )
{ dword len= MIN( hmsc->scsi_blk_len, MSC_MEDIA_PACKET );

  if ( hmsc->Write( lun
                 , hmsc->bot_data
                 , hmsc->scsi_blk_addr / hmsc->scsi_blk_size
                 , len                / hmsc->scsi_blk_size ) < 0 )
  { SCSIsenseCode( lun, hmsc
                 , HARDWARE_ERROR
                 , WRITE_FAULT );
    return( -1 );
  }

  hmsc->scsi_blk_addr   += len;
  hmsc->scsi_blk_len    -= len;
  hmsc->csw.dDataResidue-= len; /* case 12 : Ho = Do */

  if ( hmsc->scsi_blk_len  )
  { USBDepPrepareRx( hmsc->epOt    /* Prepare EP to Receive next packet */
                   , hmsc->bot_data
                   , MIN( hmsc->scsi_blk_len, MSC_MEDIA_PACKET ));
  }
  else
  { MSC_BOT_SendCSW( hmsc, USBD_CSW_CMD_PASSED );
  }

  return( 0 );
}

/**
 * @brief  SCSI_Write10
 *         Process Write10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_Write10( byte lun, byte * params
                         , MSCdriverRec   * hmsc )
{ if ( hmsc->bot_state == USBD_BOT_IDLE )     /* Idle */
  { if ( hmsc->cbw.bFlags & 0x80 )           /* case 8 : Hi <> Do */
    { SCSIsenseCode( hmsc->cbw.bLUN, hmsc
                   , ILLEGAL_REQUEST
                   , INVALID_CDB );
      return( -1 );
    }

/* Check whether Media is ready
 */
    if ( hmsc->Ioctl( USB_IOCTL_ISREADY | lun ) )
    { SCSIsenseCode( lun, hmsc
                   , NOT_READY
                   , MEDIUM_NOT_PRESENT );
      return( -1 );
    }

/* Check If media is write-protected
 */
    if ( hmsc->Ioctl( USB_IOCTL_IS_WP | lun ))
    { SCSIsenseCode( lun, hmsc
                   , NOT_READY
                   , WRITE_PROTECTED );
      return( -1 );
    }


    hmsc->scsi_blk_addr = ( params[2] << 24 )
                       | ( params[3] << 16 )
                       | ( params[4] <<  8 )
                       |   params[5];

    hmsc->scsi_blk_len = (params[7] <<  8)
                      |  params[8];

/* check if LBA address is in the right range
 */
    if ( SCSI_CheckAddressRange( lun, hmsc ) < 0 )
    { return( -1 ); /* error */
    }

    hmsc->scsi_blk_addr *= hmsc->scsi_blk_size;
    hmsc->scsi_blk_len  *= hmsc->scsi_blk_size;


    if ( hmsc->cbw.dDataLength != hmsc->scsi_blk_len ) /* cases 3,11,13 : Hn,Ho <> D0 */
    { SCSIsenseCode( hmsc->cbw.bLUN, hmsc
                   , ILLEGAL_REQUEST
                   , INVALID_CDB);
      return( -1 );
    }

/* Prepare EP to receive first data packet
 */
    hmsc->bot_state= USBD_BOT_DATA_OUT;
    USBDepPrepareRx( hmsc->epOt
                   , hmsc->bot_data
                   , MIN( hmsc->scsi_blk_len, MSC_MEDIA_PACKET ));
  }
  else /* Write Process ongoing */
  { return( SCSI_ProcessWrite( lun, hmsc ));
  }

  return( 0 );
}


/**
 * @brief  SCSI_Verify10
 *         Process Verify10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static short SCSI_Verify10( byte  lun
                          , byte * params
                          , MSCdriverRec  * drv )
{ if ((params[1]& 0x02) == 0x02)
  { SCSIsenseCode( lun, drv
                 , ILLEGAL_REQUEST
                 , INVALID_FIELED_IN_COMMAND );
    return( -1 ); /* Error, Verify Mode Not supported*/
  }

  if ( SCSI_CheckAddressRange( lun, drv ) < 0 )
  { return( -1 ); /* error */
  }

  drv->bot_data_length= 0;
  return 0;
}

/**
 * @brief  SCSIprocessCmd
 *         Process SCSI commands
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
short SCSIprocessCmd( byte   lun
                    , MSCdriverRec * hnd
                    , byte * params )
{ switch ( params[0] )
  { case SCSI_INQUIRY:         return( SCSI_Inquiry      ( lun, params, hnd ));
    case SCSI_READ_FMT_CAP:    return( SCSIreadFmtCap    ( lun, params, hnd ));
    case SCSI_READ_CAPACITY10: return( SCSIreadCapacity10( lun, params, hnd ));
    case SCSI_READ10:          return( SCSIread10        ( lun, params, hnd ));
    case SCSI_TEST_UNIT_READY: return( SCSI_TestUnitReady( lun, params, hnd ));
    case SCSI_WRITE10:         return( SCSI_Write10      ( lun, params, hnd ));
    case SCSI_REQUEST_SENSE:   return( SCSI_RequestSense ( lun, params, hnd ));
    case SCSI_START_STOP_UNIT: return( SCSI_StartStopUnit( lun,         hnd ));
    case SCSI_ALLOW_REMOVAL:   return( SCSI_StartStopUnit( lun,         hnd ));
    case SCSI_MODE_SENSE6:     return( SCSI_ModeSense6   ( lun, params, hnd ));
    case SCSI_MODE_SENSE10:    return( SCSI_ModeSense10  ( lun,         hnd ));
    case SCSI_VERIFY10:        return( SCSI_Verify10     ( lun, params, hnd ));

    default: SCSIsenseCode( lun, hnd
                          , ILLEGAL_REQUEST, INVALID_CDB );  return -1;
   break;
} }

/**
 * @brief  SCSIgetMaxLun
 *         Load the last error code in the error list
 * @param  lun: Logical unit number
 * @param  sKey: Sense Key
 * @param  ASC: Additional Sense Key
 * @retval none
 */
dword SCSIgetMaxLun( MSCdriverRec * drv )
{ return( drv->Ioctl( USB_IOCTL_GET_LUN, &drv->max_lun ));
}

