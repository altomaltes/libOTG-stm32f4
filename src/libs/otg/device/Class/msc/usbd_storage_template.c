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
 * @file usbd_storage_template.c
 *
 * original draft from MCD Application Team
 */


#include "usbd_msc_mem.h"
#include "usbd_mass.h"

#define STORAGE_LUN_NBR  1

/* USB Mass storage Standard Inquiry Data
 */
const byte STORAGE_Inquirydata[] =  //36
{ 0x00                          /* LUN 0 */
, 0x80                          /* Removable */
, 0x02
, 0x02
, (USBD_STD_INQUIRY_LENGTH - 5)
, 0x00
, 0x00
, 0x00
, 'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ' /* Manufacturer : 8 bytes */
, 'D', 'I', 'S', 'K', ' ', ' ', ' ', ' ' /* Product      : 16 Bytes */
, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
, '0', '.', '0' ,'1'                     /* Version      : 4 Bytes */
};

#define BSIZE 512;

byte disk[ 16384 ];

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar STORAGE_Init (byte lun)
{ return( 0 );
}

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar STORAGE_GetCapacity( byte lun
                         , dword * block_num
                         , dword * block_size )
{ *block_num=  16384 /  BSIZE;
  *block_size=          BSIZE;

  return( 0 );
}

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar  STORAGE_IsReady( byte lun )
{ return( 0 );
}

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar  STORAGE_IsWriteProtected( byte lun )
{ return( 0 );
}

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar STORAGE_Read( byte   lun
                  , byte * buf
                  , dword  blk_addr
                  , word   blk_len )
{ blk_addr *= BSIZE;
  blk_len  *= BSIZE;

  memcpy( buf, disk + blk_addr, blk_len );

  return( 0 );
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar STORAGE_Write( byte   lun
                   , byte * buf
                   , dword  blk_addr
                   , word   blk_len )
{ blk_addr *= BSIZE;
  blk_len  *= BSIZE;

  memcpy( disk + blk_addr, buf, blk_len );
  return( 0 );
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the STORAGE card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
schar STORAGE_GetMaxLun( byte Cmd, byte * Buf, byte Len )
{ switch( Cmd )
  { case MS_GET_MAX_LUN:
      *Buf= 0; // 0STORAGE_LUN_NBR;
    return( 0 );

    case MS_RESET_MASS_STOR :
      if ( *Buf == 0 && Len == 0 )
      { MSC_BOT_Reset();
      }
      else
      { //USBD_CtlError( req );
        return( -1 );
      }
    break;

    default:
   //   USBD_CtlError( req );
    return( -1 );
  }

  return( -1 );
}


USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{ STORAGE_Init
, STORAGE_GetCapacity
, STORAGE_IsReady
, STORAGE_IsWriteProtected
, STORAGE_Read
, STORAGE_Write
, STORAGE_GetMaxLun
, STORAGE_Inquirydata
};
USBD_STORAGE_cb_TypeDef * USBD_STORAGE_fops= &USBD_MICRO_SDIO_fops;
