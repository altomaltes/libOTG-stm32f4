/**
  ******************************************************************************
  * @file    ./Src/usbd_storage.c
  * @author  kyChu
  * @version V1.0.0
  * @date    26-November-2017
  * @brief   Memory management layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include <string.h>
#include <stdarg.h>

#include "usbd_storage.h"
#include "FAT_TAB.h"
#include "usbdef.h"
#include "usbd_msc_scsi.h"


static byte TestBuffer[ SECTOR_SIZE ] =
  "We choose to go to the moon in this decade and do the other things,\r\n"
  "not because they are easy, but because they are hard.\r\n"
  " -- John F. Kennedy, 1962";

/* USB Mass storage Standard Inquiry Data
 */
static const schar mscInquirydata[] =   /* 36 */
{ 0x00 /* LUN 0 */
, 0x80
, 0x02
, 0x02
, ( STANDARD_INQUIRY_DATA_LEN - 5 )
, 0x00
, 0x00
, 0x00
, 'l', 'i', 'b', 'O', 'T', 'G', ' ', ' ' /* Manufacturer: 8 bytes  */
, 'a', 'l', 't', 'o', 'm', 'a', 'l', 't' /* Product     : 16 Bytes */
, 'e', 's', ' ', ' ', ' ', ' ', ' ', ' '
, '0', '.', '0', '1',                     /* Version     : 4 Bytes  */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: OK / -1: Error)
  */
static short mscRead( dword   lun
                    , void * buf1
                    , dword  blk_addr
                    , dword  blk_len )
{ dword blk_addr_offset= 0;
  dword blk_copy_number= 0;
  byte * buf= (byte *)buf1;

  if ( lun == 0 )
  { do
    { if(blk_addr == BOOT_TABLE_SECTOR_IDX)
      { blk_copy_number = 1;
        memcpy( buf + SECTOR_IDX_TO_ADDR( blk_addr_offset ) , BOOT_TABLE, BOOT_TABLE_USED_SIZE );
        memset( buf + SECTOR_IDX_TO_ADDR( blk_addr_offset ) + BOOT_TABLE_USED_SIZE, 0, BOOT_TABLE_SIZE - BOOT_TABLE_USED_SIZE);
              ( buf + SECTOR_IDX_TO_ADDR( blk_addr_offset ))[ BOOT_TABLE_SIZE - 2 ] = 0x55;
              ( buf + SECTOR_IDX_TO_ADDR( blk_addr_offset ))[ BOOT_TABLE_SIZE - 1 ] = 0xAA;
      }

      else if( blk_addr < FAT2_TABLE_SECTOR_IDX)
      { blk_copy_number = (FAT2_TABLE_SECTOR_IDX - blk_addr > blk_len) ? blk_len : FAT2_TABLE_SECTOR_IDX - blk_addr;
        memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
              , FATn_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - FAT1_TABLE_SECTOR_IDX)
              , SECTORS_CONV_BYTES(blk_copy_number));
      }

      else if( blk_addr < ROOT_TABLE_SECTOR_IDX)
      { blk_copy_number = (ROOT_TABLE_SECTOR_IDX - blk_addr > blk_len) ? blk_len : ROOT_TABLE_SECTOR_IDX - blk_addr;
        memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
              , FATn_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - FAT2_TABLE_SECTOR_IDX)
              , SECTORS_CONV_BYTES( blk_copy_number ));
      }

      else if( blk_addr < FATFS_TOTAL_SECTORS)
      { blk_copy_number = (FATFS_TOTAL_SECTORS - blk_addr > blk_len) ? blk_len : FATFS_TOTAL_SECTORS - blk_addr;
        memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
              , ROOT_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - ROOT_TABLE_SECTOR_IDX)
              , SECTORS_CONV_BYTES(blk_copy_number));
      }

      else    /* README.TXT */
      { if ( blk_addr < README_SECT_IDX + README_SECT_NUM)
        { if(blk_addr == README_SECT_IDX + README_SECT_NUM - 1)  /* END OF FILE */
          { blk_copy_number = 1;
            memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
                  , README_DATA + SECTORS_CONV_BYTES(README_SECT_NUM - 1)
                  , README_TAIL_LEN);
            memset( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset) + README_TAIL_LEN
                  , 0, SECTOR_SIZE - README_TAIL_LEN);
           }
           else
           { blk_copy_number = (README_SECT_IDX + README_SECT_NUM - blk_addr - 1 > blk_len) ? blk_len : README_SECT_IDX + README_SECT_NUM - blk_addr - 1;
             memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
                   , README_DATA + SECTOR_IDX_TO_ADDR(blk_addr - README_SECT_IDX)
                   , SECTORS_CONV_BYTES( blk_copy_number ));
         }  }

         else /* DATA IN FLASH */
         { if ( blk_addr == README_SECT_IDX + README_SECT_NUM )
           { blk_copy_number = 1;
             memcpy( buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
                   , TestBuffer, SECTORS_CONV_BYTES( blk_copy_number ));
           }
           else
           { blk_copy_number = blk_len;
           }
         } }
      blk_len         -= blk_copy_number;
      blk_addr        += blk_copy_number;
      blk_addr_offset += blk_copy_number;
    }
    while( blk_len );

    return( FATFS_OK );
  }

  return( FATFS_ERROR );
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : OK / -1 : Error)
  */
static short mscWrite( dword  lun
                          , const void *buf
                          , dword blk_addr
                          , dword blk_len )
{ dword blk_addr_offset = 0;
  dword blk_wrte_number = 0;
  if ( lun == 0 )
  { do
    { if(blk_addr == BOOT_TABLE_SECTOR_IDX)
      { blk_wrte_number = 1;
        memcpy(BOOT_TABLE, buf + SECTOR_IDX_TO_ADDR(blk_addr_offset), BOOT_TABLE_USED_SIZE);
      }

      else if ( blk_addr < FAT2_TABLE_SECTOR_IDX )
      { blk_wrte_number = (FAT2_TABLE_SECTOR_IDX - blk_addr > blk_len) ? blk_len : FAT2_TABLE_SECTOR_IDX - blk_addr;
        memcpy(FATn_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - FAT1_TABLE_SECTOR_IDX), buf + SECTOR_IDX_TO_ADDR(blk_addr_offset), SECTORS_CONV_BYTES(blk_wrte_number));
      }

      else if(blk_addr < ROOT_TABLE_SECTOR_IDX)
      { blk_wrte_number = (ROOT_TABLE_SECTOR_IDX - blk_addr > blk_len) ? blk_len : ROOT_TABLE_SECTOR_IDX - blk_addr;
//        memcpy(FATn_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - FAT2_TABLE_SECTOR_IDX), buf + SECTOR_IDX_TO_ADDR(blk_addr_offset), SECTORS_CONV_BYTES(blk_wrte_number));
      }

      else if ( blk_addr < FATFS_TOTAL_SECTORS )
      { blk_wrte_number = (FATFS_TOTAL_SECTORS - blk_addr > blk_len) ? blk_len : FATFS_TOTAL_SECTORS - blk_addr;
        memcpy(ROOT_TABLE + SECTOR_IDX_TO_ADDR(blk_addr - ROOT_TABLE_SECTOR_IDX), buf + SECTOR_IDX_TO_ADDR(blk_addr_offset), SECTORS_CONV_BYTES(blk_wrte_number));
      }
      else
      { if ( blk_addr < README_SECT_IDX + README_SECT_NUM) /* README.TXT */
        { if( blk_addr == README_SECT_IDX + README_SECT_NUM - 1)  /* END OF FILE */
          { blk_wrte_number = 1;
            memcpy( README_DATA + SECTORS_CONV_BYTES(README_SECT_NUM - 1)
                  , buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
                  , README_TAIL_LEN);
           }
           else
           { blk_wrte_number= (README_SECT_IDX + README_SECT_NUM - blk_addr - 1 > blk_len)
                            ? blk_len : README_SECT_IDX + README_SECT_NUM - blk_addr - 1;
             memcpy( README_DATA + SECTOR_IDX_TO_ADDR(blk_addr - README_SECT_IDX), buf + SECTOR_IDX_TO_ADDR(blk_addr_offset), SECTORS_CONV_BYTES(blk_wrte_number));
          } }
          else
          { if ( blk_addr == README_SECT_IDX + README_SECT_NUM ) /* DATA IN FLASH */
            { blk_wrte_number = 1;
              memcpy( TestBuffer
                    , buf + SECTOR_IDX_TO_ADDR(blk_addr_offset)
                    , SECTORS_CONV_BYTES(blk_wrte_number));
             }
             else
             { blk_wrte_number = blk_len;
         } } }

      blk_len -= blk_wrte_number;
      blk_addr += blk_wrte_number;
      blk_addr_offset += blk_wrte_number;
    }
    while(blk_len);
    return FATFS_OK;
  }

  return( FATFS_ERROR );
}

/**
  * @brief  Miscellaneous control of the medium capacity.
  * @param  lun: Logical unit number + opeartion << 8
  * @retval Status (0: OK / -1: Error)
  */
static short mscIoctl( dword opcode, ... )
{ va_list ap;
  va_start( ap, opcode );

  switch( opcode & USB_IOCTL_MASK )
  { case USB_IOCTL_INIT   :
    case USB_IOCTL_ISREADY:
    case USB_IOCTL_IS_WP  :
    break;

    case USB_IOCTL_GET_LUN  :
    { unsigned char * ptr= va_arg( ap, unsigned char  * );
      *ptr= 0;
    }
    break;

    case USB_IOCTL_INQUIRY:  /// !! advance to the LUN
    { void ** ptr= va_arg( ap, void ** );
      *ptr= mscInquirydata;
    }
    break;

    case USB_IOCTL_GETCAP:
    { dword * ptr;

      ptr= va_arg( ap, dword * ); *ptr= TOTAL_SECTORS;
      ptr= va_arg( ap, dword * ); *ptr= SECTOR_SIZE;
    }
    break;
  }

  va_end( ap );
  return( 0 );
}

extern const USBDclassDefREC USBD_MSC_cb;


MSCdriverRec mscHandle=
{ driver : &USBD_MSC_cb
, Ioctl  : mscIoctl
, Read   : mscRead
, Write  : mscWrite
};

