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
 * @file usbd_msc_scsi.c
 *
 * original draft from MCD Application Team
 */

#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_data.h"


SCSI_Sense_TypeDef SCSI_Sense[ SENSE_LIST_DEEPTH ];
byte   SCSI_Sense_Head;
byte   SCSI_Sense_Tail;

dword  SCSI_blk_size;
dword  SCSI_blk_nbr;

dword  SCSI_blk_addr;
dword  SCSI_blk_len;

/**
 * @brief  SCSI_TestUnitReady
 *         Process SCSI Test Unit Ready Command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
*/
static schar SCSI_TestUnitReady( byte lun, byte *params )
{
  if (MSC_BOT_cbw.dDataLength != 0) /* case 9 : Hi > D0 */
  { SCSI_SenseCode( MSC_BOT_cbw.bLUN
                  , ILLEGAL_REQUEST
                  , INVALID_CDB );
    return( -1 );
  }

  if ( USBD_STORAGE_fops->IsReady(lun) !=0 )
  { SCSI_SenseCode(lun
                  , NOT_READY
                  , MEDIUM_NOT_PRESENT );
    return( -1 );
  }

  MSC_BOT_DataLen= 0;

  return( 0 );
}

/**
* @brief  SCSI_Inquiry
*         Process Inquiry command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static schar SCSI_Inquiry( byte lun, byte * params )
{ byte * pPage;
  word   len;

  if ( params[ 1 ] & 0x01 ) /* Evpd is set */
  { pPage = (byte *)MSCpage00inquirydata;
    len = LENGTH_INQUIRY_PAGE00;
  }
  else
  { pPage= (byte *)&USBD_STORAGE_fops->pInquiry[ lun * USBD_STD_INQUIRY_LENGTH ];
    len = pPage[4] + 5;

    if ( params[4] <= len )
    { len= params[4];
  } }

  MSC_BOT_DataLen= len;

  while( len )
  { len--;
    MSC_BOT_Data[ len ]= pPage[len];
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
static schar SCSI_ReadCapacity10( byte lun, byte * params )
{
  if ( USBD_STORAGE_fops->GetCapacity( lun, &SCSI_blk_nbr, &SCSI_blk_size))
  { SCSI_SenseCode(lun, NOT_READY, MEDIUM_NOT_PRESENT);
    return( -1 );
  }
  else
  {
    MSC_BOT_Data[ 0 ] = (byte)((SCSI_blk_nbr - 1) >> 24);
    MSC_BOT_Data[ 1 ] = (byte)((SCSI_blk_nbr - 1) >> 16);
    MSC_BOT_Data[ 2 ] = (byte)((SCSI_blk_nbr - 1) >>  8);
    MSC_BOT_Data[ 3 ] = (byte)( SCSI_blk_nbr - 1);

    MSC_BOT_Data[ 4 ] = (byte)(SCSI_blk_size >>  24 );
    MSC_BOT_Data[ 5 ] = (byte)(SCSI_blk_size >>  16 );
    MSC_BOT_Data[ 6 ] = (byte)(SCSI_blk_size >>  8  );
    MSC_BOT_Data[ 7 ] = (byte)(SCSI_blk_size        );

    MSC_BOT_DataLen = 8;

    return( 0 );
} }
/**
* @brief  SCSIreadFmtCapacity
*         Process Read Format Capacity command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static schar SCSIreadFmtCapacity( byte lun, byte *params )
{ dword blk_size;
  dword blk_nbr;
  word i;

  for( i= 0
     ; i < 12
     ; i ++ )
  { MSC_BOT_Data[i] = 0;
  }

  if ( USBD_STORAGE_fops->GetCapacity( lun
                                     , &blk_nbr
                                     , &blk_size ))
  { SCSI_SenseCode( lun
                  , NOT_READY
                  , MEDIUM_NOT_PRESENT );
    return( -1 );
  }

  else
  { MSC_BOT_Data[  3 ]= 0x08;
    MSC_BOT_Data[  4 ]= (byte)((blk_nbr - 1) >> 24);
    MSC_BOT_Data[  5 ]= (byte)((blk_nbr - 1) >> 16);
    MSC_BOT_Data[  6 ]= (byte)((blk_nbr - 1) >>  8);
    MSC_BOT_Data[  7 ]= (byte) (blk_nbr - 1);

    MSC_BOT_Data[  8 ]= 0x02;
    MSC_BOT_Data[  9 ]= (byte)(blk_size >>  16);
    MSC_BOT_Data[ 10 ]= (byte)(blk_size >>  8);
    MSC_BOT_Data[ 11 ]= (byte)(blk_size);

    MSC_BOT_DataLen = 12;

    return( 0 );
} }
/**
* @brief  SCSI_ModeSense6
*         Process Mode Sense6 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static schar SCSI_ModeSense6( byte lun, byte *params )
{ word len= 8 ;
  MSC_BOT_DataLen = len;

  while( len )
  { len--;
    MSC_BOT_Data[ len ]= MSCmodeSense6Data[len];
  }
  return 0;
}

/**
* @brief  SCSI_ModeSense10
*         Process Mode Sense10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static schar SCSI_ModeSense10( byte lun, byte *params)
{ word len= 8;

  MSC_BOT_DataLen= len;

  while( len )
  { len--;
    MSC_BOT_Data[len]= MSCmodeSense10Data[ len ];
  }

  return( 0 );
}

/**
* @brief  SCSI_RequestSense
*         Process Request Sense command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/

static schar SCSI_RequestSense( byte lun, byte *params )
{ byte i;

  for( i=0
     ; i < REQUEST_SENSE_DATA_LEN
     ; i++ )
  { MSC_BOT_Data[i] = 0;
  }

  MSC_BOT_Data[0]	= 0x70;
  MSC_BOT_Data[7]	= REQUEST_SENSE_DATA_LEN - 6;

  if ((SCSI_Sense_Head != SCSI_Sense_Tail))
  { MSC_BOT_Data[ 2  ]= SCSI_Sense[ SCSI_Sense_Head ].Skey;
    MSC_BOT_Data[ 12 ]= SCSI_Sense[ SCSI_Sense_Head ].w.b.ASCQ;
    MSC_BOT_Data[ 13 ]= SCSI_Sense[ SCSI_Sense_Head ].w.b.ASC;
    SCSI_Sense_Head++;

    if (SCSI_Sense_Head == SENSE_LIST_DEEPTH)
    { SCSI_Sense_Head = 0;
  } }

  MSC_BOT_DataLen = REQUEST_SENSE_DATA_LEN;

  if (params[4] <= REQUEST_SENSE_DATA_LEN)
  {
    MSC_BOT_DataLen = params[4];
  }
  return 0;
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
{
  SCSI_Sense[ SCSI_Sense_Tail ].Skey  = sKey;
  SCSI_Sense[ SCSI_Sense_Tail ].w.ASC = ASC << 8;
  SCSI_Sense_Tail++;

  if ( SCSI_Sense_Tail == SENSE_LIST_DEEPTH )
  { SCSI_Sense_Tail = 0;
} }

/**
* @brief  SCSI_StartStopUnit
*         Process Start Stop Unit command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static schar SCSI_StartStopUnit(byte lun, byte *params)
{
  MSC_BOT_DataLen = 0;
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
                                   , word blk_nbr )
{ if ((blk_offset + blk_nbr) > SCSI_blk_nbr )
  { SCSI_SenseCode(lun, ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
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
static schar SCSI_ProcessRead (byte lun)
{
  dword len;

  len = MIN( SCSI_blk_len , MSC_MEDIA_PACKET );

  if ( USBD_STORAGE_fops->Read( lun
                              , MSC_BOT_Data
                              , SCSI_blk_addr / SCSI_blk_size
                              , len           / SCSI_blk_size ) < 0 )
  { SCSI_SenseCode(lun, HARDWARE_ERROR, UNRECOVERED_READ_ERROR);
    return -1;
  }

  USBDepTx( MSC_IN_EP, MSC_BOT_Data, len );
  SCSI_blk_addr+= len;
  SCSI_blk_len -= len;

  MSC_BOT_csw.dDataResidue -= len;   /* case 6 : Hi = Di */

  if ( SCSI_blk_len == 0 )
  { MSC_BOT_State = BOT_LAST_DATA_IN;
  }

  return( 0 );
}

/**
 * @brief  SCSI_Read10
 *         Process Read10 command
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
static schar SCSI_Read10(byte lun , byte *params)
{
  if ( MSC_BOT_State == BOT_IDLE)  /* Idle */
  { if ((MSC_BOT_cbw.bmFlags & 0x80) != 0x80 ) /* case 10 : Ho <> Di */
    { SCSI_SenseCode( MSC_BOT_cbw.bLUN
                    , ILLEGAL_REQUEST
                    ,  INVALID_CDB );
      return( -1 );
    }

    if ( USBD_STORAGE_fops->IsReady( lun ))
    { SCSI_SenseCode( lun
                    , NOT_READY
                    , MEDIUM_NOT_PRESENT );
      return( -1 );
    }

    SCSI_blk_addr= (params[2] << 24) |(params[3] << 16) |(params[4] <<  8) | params[5];
    SCSI_blk_len = (params[7] <<  8) | params[8];

    if ( SCSI_CheckAddressRange(lun, SCSI_blk_addr, SCSI_blk_len) < 0)
    { return( -1 );
    }

    MSC_BOT_State  = BOT_DATA_IN;
    SCSI_blk_addr *= SCSI_blk_size;
    SCSI_blk_len  *= SCSI_blk_size;

    if ( MSC_BOT_cbw.dDataLength != SCSI_blk_len ) /* cases 4,5 : Hi <> Dn */
    { SCSI_SenseCode( MSC_BOT_cbw.bLUN
                    , ILLEGAL_REQUEST
                    , INVALID_CDB );
      return( -1 );
  } }

  MSC_BOT_DataLen= MSC_MEDIA_PACKET;

  return( SCSI_ProcessRead(lun) );
}

/**
 * @brief  SCSI_ProcessWrite
 *         Handle Write Process
 * @param  lun: Logical unit number
 * @retval status
 */
static schar SCSI_ProcessWrite( byte lun )
{ dword len;

  len = MIN(SCSI_blk_len , MSC_MEDIA_PACKET);

  if( USBD_STORAGE_fops->Write( lun ,
                              MSC_BOT_Data,
                              SCSI_blk_addr / SCSI_blk_size,
                              len / SCSI_blk_size) < 0)
  { SCSI_SenseCode(lun, HARDWARE_ERROR, WRITE_FAULT);
    return -1;
  }

  SCSI_blk_addr+= len;
  SCSI_blk_len -= len;

  /* case 12 : Ho = Do */
  MSC_BOT_csw.dDataResidue -= len;

  if ( SCSI_blk_len == 0 )
  { MSC_BOT_SendCSW ( CSW_CMD_PASSED);
  }
  else /* Prapare EP to Receive next packet */
  { USBDepPrepareRx( MSC_OUT_EP
                    , MSC_BOT_Data
                    , MIN (SCSI_blk_len, MSC_MEDIA_PACKET));
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
static schar SCSI_Write10( byte lun , byte * params )
{
  if ( MSC_BOT_State == BOT_IDLE ) /* Idle */
  {

/* case 8 : Hi <> Do
 */
    if (( MSC_BOT_cbw.bmFlags & 0x80) == 0x80 )
    { SCSI_SenseCode( MSC_BOT_cbw.bLUN                    , ILLEGAL_REQUEST                    , INVALID_CDB );
      return( -1 );
    }

/* Check whether Media is ready
 */
    if ( USBD_STORAGE_fops->IsReady( lun ))
    { SCSI_SenseCode( lun                    , NOT_READY                    , MEDIUM_NOT_PRESENT );
      return( -1 );
    }

/* Check If media is write-protected
 */
    if ( USBD_STORAGE_fops->IsWriteProtected( lun ))
    { SCSI_SenseCode( lun
                    , NOT_READY
                    , WRITE_PROTECTED );
      return( -1 );
    }


    SCSI_blk_addr= (params[2] << 24) | (params[3] << 16) | (params[4] <<  8) | params[5];
    SCSI_blk_len = (params[7] <<  8) |  params[8];

/* check if LBA address is in the right range
 */
    if( SCSI_CheckAddressRange(lun, SCSI_blk_addr, SCSI_blk_len) < 0)
    { return( -1 ); /* error */
    }

    SCSI_blk_addr *= SCSI_blk_size;
    SCSI_blk_len  *= SCSI_blk_size;

/* cases 3,11,13 : Hn,Ho <> D0
 */
    if (MSC_BOT_cbw.dDataLength != SCSI_blk_len)
    { SCSI_SenseCode( MSC_BOT_cbw.bLUN
                    , ILLEGAL_REQUEST
                    , INVALID_CDB );
      return( -1 );
    }

/* Prepare EP to receive first data packet
 */
    MSC_BOT_State = BOT_DATA_OUT;
    USBDepPrepareRx( MSC_OUT_EP
                    , MSC_BOT_Data
                    , MIN (SCSI_blk_len, MSC_MEDIA_PACKET ));
  }
  else /* Write Process ongoing */
  { return( SCSI_ProcessWrite( lun ));
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

static schar SCSI_Verify10(byte lun , byte *params)
{ if ((params[1]& 0x02) == 0x02)
  { SCSI_SenseCode (lun, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
    return( -1 ); /* Error, Verify Mode Not supported*/
  }

  SCSI_blk_addr= (params[2] << 24) | (params[3] << 16) |  (params[4] <<  8) |   params[5];
  SCSI_blk_len = (params[7] <<  8) |  params[8];

  if( SCSI_CheckAddressRange(lun, SCSI_blk_addr, SCSI_blk_len) < 0 )
  { return( -1 ); /* error */
  }
  MSC_BOT_DataLen = 0;
  return( 0 );
}

/**
 * @brief  SCSI_ProcessCmd
 *         Process SCSI commands
 * @param  lun: Logical unit number
 * @param  params: Command parameters
 * @retval status
 */
schar SCSI_ProcessCmd( byte lun
                     , byte * params )
{ switch (params[0])
  { case SCSI_TEST_UNIT_READY:     break; //   return( SCSI_TestUnitReady(  lun, params ));
    case SCSI_REQUEST_SENSE:       break; //   return( SCSI_RequestSense (  lun, params ));
    case SCSI_INQUIRY:               return( SCSI_Inquiry(        lun, params ));
    case SCSI_START_STOP_UNIT:       break; // return( SCSI_StartStopUnit(  lun, params ));
    case SCSI_ALLOW_REMOVAL:  break; // return( SCSI_StartStopUnit(  lun, params ));
    case SCSI_MODE_SENSE6:           break; // return( SCSI_ModeSense6 (    lun, params ));
    case SCSI_MODE_SENSE10:          break; // return( SCSI_ModeSense10 (   lun, params ));
    case SCSI_READ_FMT_CAP:   return( SCSIreadFmtCapacity( lun, params ));
    case SCSI_READ_CAPACITY10:        return( SCSI_ReadCapacity10( lun, params ));

    case SCSI_READ10: return( SCSI_Read10(         lun, params ));

    case SCSI_WRITE10:               break; // return( SCSI_Write10(        lun, params ));
    case SCSI_VERIFY10:              break; // return( SCSI_Verify10(       lun, params ));

    default:
      SCSI_SenseCode( lun, ILLEGAL_REQUEST, INVALID_CDB );
    return( -1 );
  }

  switch ( params[0] )
  { case SCSI_TEST_UNIT_READY:  return( SCSI_TestUnitReady(  lun, params ));
    case SCSI_REQUEST_SENSE:    return( SCSI_RequestSense (  lun, params ));
    case SCSI_START_STOP_UNIT:  return( SCSI_StartStopUnit(  lun, params ));
    case SCSI_ALLOW_REMOVAL:    return( SCSI_StartStopUnit(  lun, params ));
    case SCSI_MODE_SENSE6:      return( SCSI_ModeSense6 (    lun, params ));
    case SCSI_MODE_SENSE10:     return( SCSI_ModeSense10 (   lun, params ));
    case SCSI_READ_FMT_CAP:     return( SCSIreadFmtCapacity( lun, params ));
    case SCSI_READ_CAPACITY10:  return( SCSI_ReadCapacity10( lun, params ));
    case SCSI_WRITE10:          return( SCSI_Write10(        lun, params ));
    case SCSI_VERIFY10:         return( SCSI_Verify10(       lun, params ));
  }

  return( 0 );
}


