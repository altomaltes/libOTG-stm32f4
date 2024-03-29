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
 * @file usbh_msc_scsi.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"


/**
  * @brief  USBH_MSC_TestUnitReady
  *         Issues 'Test unit ready' command to the device. Once the response
  *         received, it updates the status to upper layer.
  * @param  None
  * @retval Status
  */
short USBHtestUnitReady()
{ USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= 0;       /* No Data Transfer */
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags = EPDIR_OUT;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength= CBW_LENGTH_TEST_UNIT_READY;

  USBH_MSC_Param.xfer.pRxTxBuff = USBH_MSC_Param.USBH_MSC_CSWData.CSWArray;
  USBH_MSC_Param.xfer.DataLength= USBH_MSC_CSW_MAX_LENGTH;
  USBH_MSC_Param.xfer.MSCState  = USBH_MSC_TEST_UNIT_READY;

  for( int index= CBW_CB_LENGTH - 1
     ;     index
     ;     index-- )
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ index ]= 0x00;
  }
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_TEST_UNIT_READY;

  return( USBH_MSC_TEST_UNIT_READY );
}



/**
  * @brief  USBH_MSC_ReadCapacity10
  *         Issue the read capacity command to the device. Once the response
  *         received, it updates the status to upper layer
  * @param  None
  * @retval Status
  */
short MSCaskForReadCapacity10( short pDrv )
{ USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= XFER_LEN_READ_CAPACITY10;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags         = EPDIR_IN;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength        = CBW_LENGTH;

  int index;

  USBH_MSC_Param.xfer.pRxTxBuff= USBH_MSC_Param.dataInBuffer;

  for( index= CBW_CB_LENGTH -1
     ; index
     ; index-- )
   { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ index ]= 0x00;
   }
   USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_READ_CAPACITY10;

   return( USBH_MSC_READ_CAPACITY10 );
}

short MSCrespReadCapacity10( short pDrv )
{ (((byte*)&USBH_MSC_Param.MSCapacity   )[3])= USBH_MSC_Param.dataInBuffer[ 0 ];
  (((byte*)&USBH_MSC_Param.MSCapacity   )[2])= USBH_MSC_Param.dataInBuffer[ 1 ];
  (((byte*)&USBH_MSC_Param.MSCapacity   )[1])= USBH_MSC_Param.dataInBuffer[ 2 ];
  (((byte*)&USBH_MSC_Param.MSCapacity   )[0])= USBH_MSC_Param.dataInBuffer[ 3 ];

  (((byte*)&USBH_MSC_Param.MSPageLength )[1])= USBH_MSC_Param.dataInBuffer[ 6 ]; /*assign the page length*/
  (((byte*)&USBH_MSC_Param.MSPageLength )[0])= USBH_MSC_Param.dataInBuffer[ 7 ];

  return( USBH_MSC_READ_CAPACITY10 );
}


/**
  * @brief  USBH_MSC_ModeSense6
  *         Issue the Mode Sense6 Command to the device. This function is used
  *          for reading the WriteProtect Status of the Mass-Storage device.
  * @param  None
  * @retval Status
  */
short MSCaskForModeSense6( short pDrv )
{ USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= XFER_LEN_MODE_SENSE6;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags= EPDIR_IN;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength= CBW_LENGTH;

  USBH_MSC_Param.xfer.pRxTxBuff= USBH_MSC_Param.dataInBuffer;
  USBH_MSC_Param.xfer.MSCState= USBH_MSC_MODE_SENSE6;

  for( int index= CBW_CB_LENGTH - 1
     ;     index
     ;     index-- )
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ index ] = 0x00;
  }

  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_MODE_SENSE6;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 2 ]= MODE_SENSE_PAGE_CONTROL_FIELD | MODE_SENSE_PAGE_CODE;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 4 ]= XFER_LEN_MODE_SENSE6;

  return( USBH_MSC_MODE_SENSE6 );
}

/* Assign the Write Protect status */
/* If WriteProtect = 0, Writing is allowed
   If WriteProtect != 0, Disk is Write Protected
 */

short MSCrespModeSense6( short pDrv )
{ USBH_MSC_Param.MSWriteProtect= USBH_MSC_Param.dataInBuffer[ 2 ] & MASK_MODE_SENSE_WRITE_PROTECT
                               ? DISK_WRITE_PROTECTED : 0;
  return( USBH_MSC_MODE_SENSE6 );
}

/**
  * @brief  USBH_MSC_RequestSense
  *         Issues the Request Sense command to the device. Once the response
  *         received, it updates the status to upper layer
  * @param  None
  * @retval Status
  */
short MSCaskForRequestSense( short pDrv )
{ USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags = EPDIR_IN;  /* Prepare the CBW and relevant field*/
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength= CBW_LENGTH;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= ALLOCATION_LENGTH_REQUEST_SENSE;
  USBH_MSC_Param.xfer.pRxTxBuff = USBH_MSC_Param.dataInBuffer;

  for( int index = CBW_CB_LENGTH - 1
     ; index
     ; index--)
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[index] = 0x00;
  }

  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_REQUEST_SENSE;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 1 ]= DESC_REQUEST_SENSE;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 4 ]= ALLOCATION_LENGTH_REQUEST_SENSE;

  return( USBH_MSC_REQUEST_SENSE );
}

short MSrespRequestSense( short pDrv )
{ (((byte*)&USBH_MSC_Param.MSSenseKey )[3])= USBH_MSC_Param.dataInBuffer[ 0 ];
  (((byte*)&USBH_MSC_Param.MSSenseKey )[2])= USBH_MSC_Param.dataInBuffer[ 1 ];
  (((byte*)&USBH_MSC_Param.MSSenseKey )[1])= USBH_MSC_Param.dataInBuffer[ 2 ];
  (((byte*)&USBH_MSC_Param.MSSenseKey )[0])= USBH_MSC_Param.dataInBuffer[ 3 ];

  return( USBH_MSC_REQUEST_SENSE );
};


dword USBH_MSCgetCapacity( short pDrv )
{ return( USBH_MSC_Param.MSCapacity );
}

/**
  * @brief  USBH_MSCwrite10
  *         Issue the write command to the device. Once the response received,
  *         it updates the status to upper layer
  * @param  dataBuffer : DataBuffer contains the data to write
  * @param  address : Address to which the data will be written
  * @param  nbOfbytes : NbOfbytes to be written
  * @retval Status
  */
short USBH_MSCwrite10( short  drv          /* Phisical drive */
                      , const void * dataBuffer
                      , dword address
                      , dword nbOfbytes )
{ switch( USBH_MSC_Param.xfer.MSCState )  /* Allow sync and no sync access */
  { case USBH_MSC_IDLE   : break;                    /* Free to read */
    case USBH_MSC_WRITE10: return( USBH_MSC_IDLE );  /* Progressing  */
    default              : return(            -1 );  /* Dead engine  */
  }

  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= nbOfbytes;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags         = EPDIR_OUT;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength        = CBW_LENGTH;
  USBH_MSC_Param.xfer.pRxTxBuff         = (byte*)dataBuffer;

  for( byte index= CBW_CB_LENGTH - 1
     ;      index
     ;      index-- )
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ index ]= 0x00;
  }

  if ( address )                                                /* Not a repeat of last try */
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_WRITE10;
    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 2 ]= (((byte*)&address)[3]);   /*logical block address*/
    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 3 ]= (((byte*)&address)[2]);
    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 4 ]= (((byte*)&address)[1]);
    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 5 ]= (((byte*)&address)[0]);

    word nbOfPages = nbOfbytes/ USBH_MSC_PAGE_LENGTH;   /*USBH_MSC_PAGE_LENGTH = 512*/

    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 7 ] = (((byte *)&nbOfPages)[1]) ;       /*Transfer length */
    USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 8 ] = (((byte *)&nbOfPages)[0]) ;
  }

  USBH_MSC_XferStart( 0 );

  return( USBH_MSC_Param.xfer.MSCState= USBH_MSC_WRITE10 ); /*status;*/
}

/**
  * @brief  USBH_MSCread10
  *         Issue the read command to the device. Once the response received,
  *         it updates the status to upper layer ( ASYNC )
  * @param  dataBuffer : DataBuffer will contain the data to be read
  * @param  address : Address from which the data will be read
  * @param  nbOfbytes : NbOfbytes to be read
  * @retval Status
  */
short USBH_MSCread10( short  drv          /* Phisical drive */
                     , void * dataBuffer
                     , dword  address
                     , dword  nbOfbytes )
{ switch( USBH_MSC_Param.xfer.MSCState )  /* Allow sync and no sync access */
  { case USBH_MSC_IDLE   : break;           /* Free to read */
    case USBH_MSC_READ10 : return( -2 );   // Previous read   return( USBH_MSC_IDLE );  /* Progressing  */
    default              : return( -1 );    /* Dead engine  */
  }

  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWTransferLength= nbOfbytes;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWFlags         = EPDIR_IN;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWLength        = CBW_LENGTH;

  USBH_MSC_Param.xfer.pRxTxBuff= dataBuffer;

  for( int index= CBW_CB_LENGTH - 1
     ;     index
     ;     index-- )
  { USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ index ]= 0x00;
  }
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 0 ]= OPCODE_READ10;

/* logical block address
 */
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 2 ]= (((byte*)&address)[3]);
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 3 ]= (((byte*)&address)[2]);
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 4 ]= (((byte*)&address)[1]);
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 5 ]= (((byte*)&address)[0]);

  word nbOfPages= nbOfbytes / USBH_MSC_PAGE_LENGTH;   /*USBH_MSC_PAGE_LENGTH = 512*/

/* Transfer length
 */
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 7 ]= (((byte *)&nbOfPages)[1]) ;
  USBH_MSC_Param.USBH_MSC_CBWData.field.CBWCB[ 8 ]= (((byte *)&nbOfPages)[0]) ;

  USBH_MSC_XferStart( 0 );

  return( USBH_MSC_Param.xfer.MSCState= USBH_MSC_READ10 ); /*status;*/
}



