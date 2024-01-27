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
 * @file usbd_cdc_core.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USB_CDC_CORE_H_
#define __USB_CDC_CORE_H_

#include "usbd_core.h"
#include "usb_cdc.h"

struct cdcHandleRec
{ const USBDclassDefREC * driver;

  short ( *dataIn  )( dword epNum, struct cdcHandleRec * args );
  short ( *dataOut )( dword epNum, struct cdcHandleRec * args );
  short ( *dataCmd )( dword epNum, struct cdcHandleRec * args );
  word  ( *ctrl    )( byte req   , struct cdcHandleRec * args );

  dword altSet;
  word  cdcCmd;
  word  cdcLen;
  byte  epIn;

  byte cmdBuff[ 16 ];
  byte dataBuf[ 32 ];
};

extern const USBDclassDefREC  USBD_CDC_cb;

#endif /* __USB_CDC_CORE_H_ */
