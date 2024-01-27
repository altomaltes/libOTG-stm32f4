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
 * @file usbd_serial.c
 *
 * original draft from MCD Application Team
 */

#include <string.h>
#include <stdarg.h>

#include "usbdef.h"
#include "uc.h"


/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: OK / -1: Error)
  */
static short RS232read( dword   lun
                      , void  * buf
                      , dword   blkAddr
                      , dword   blkLen )
{ return( 0 );
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : OK / -1 : Error)
  */
static short RS232write( dword  lun
                       , const void *buf
                       , dword blk_addr
                       , dword blk_len )
{ return( 0 );
}

/**
  * @brief  Miscellaneous control of the medium capacity.
  * @param  lun: Logical unit number + opeartion << 8
  * @retval Status (0: OK / -1: Error)
  */
static short RS232ioctl( dword opcode, ... )
{ va_list ap;
  va_start( ap, opcode );

  switch( opcode & USB_IOCTL_MASK )
  { case USB_IOCTL_INIT   :
    break;

    case USB_IOCTL_INQUIRY:  /// !! advance to the LUN
    {// void ** ptr= va_arg( ap, void ** );
     // *ptr= STORAGE_Inquirydata;
    }
    break;

    case USB_IOCTL_GETCAP:
    break;
  }

  va_end( ap );
  return( 0 );
}

const USBDdriverRec rs232Handle=
{ RS232read
, RS232write
, RS232ioctl
};
