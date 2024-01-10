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
 * @file usbd_msc_mem.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBD_MEM_H
#define __USBD_MEM_H

#include "usbd_core.h"

#define USBD_STD_INQUIRY_LENGTH		36


typedef struct _USBD_STORAGE
{ schar ( * Init             ) ( byte lun );
  schar ( * GetCapacity      ) ( byte lun, dword *block_num, dword *block_size);
  schar ( * IsReady          ) ( byte lun );
  schar ( * IsWriteProtected ) ( byte lun );
  schar ( * Read             ) ( byte lun, byte *buf, dword blk_addr, word blk_len );
  schar ( * Write            ) ( byte lun, byte *buf, dword blk_addr, word blk_len );
  schar ( * GetMaxLun        ) ( byte Cmd, byte * Buf, byte Len );

  byte   * pInquiry;

} USBD_STORAGE_cb_TypeDef;

extern USBD_STORAGE_cb_TypeDef * USBD_STORAGE_fops;

#endif /* __USBD_MEM_H */
