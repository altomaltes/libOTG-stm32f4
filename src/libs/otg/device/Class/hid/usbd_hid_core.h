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
 * @file usbd_hid_core.h
 *
 * original draft from MCD Application Team
 */


#ifndef __USB_HID_CORE_H_
#define __USB_HID_CORE_H_

#include "usbd_core.h"

#define HID_IN_PACKET  4

#define USB_HID_DESC_SIZ             9
#define HID_MOUSE_REPORT_DESC_SIZE  74

#define HID_HS_BINTERVAL          0x07
#define HID_FS_BINTERVAL          0x0A

#define HID_REQ_SET_REPORT        0x09
#define HID_REQ_GET_REPORT        0x01

extern USBDclassDefREC USBD_HID_cb;

struct USBDdriverHidRec
{ const USBDclassDefREC * driver;          /* Base handler */

//  short ( *ioctl)( dword opcode, ...     );  // control function
//  short ( *read )( dword ep, void * args );  // Read function

  dword  USBD_HID_AltSet;
  dword  USBD_HID_Protocol;
  dword  USBD_HID_IdleState;

};


dword USBD_HID_GetPollingInterval();

#endif

