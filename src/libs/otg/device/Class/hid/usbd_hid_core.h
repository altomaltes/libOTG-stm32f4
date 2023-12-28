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

#include "usb_hid.h"

#define USB_HID_CONFIG_DESC_SIZ     34
#define USB_HID_DESC_SIZ             9
#define HID_MOUSE_REPORT_DESC_SIZE  74

#define HID_DESCRIPTOR_TYPE       0x21
#define HID_REPORT_DESC           0x22

#define HID_HS_BINTERVAL          0x07
#define HID_FS_BINTERVAL          0x0A

#define HID_REQ_SET_REPORT        0x09
#define HID_REQ_GET_REPORT        0x01


extern USBDclassDefREC USBD_HID_cb;

schar USBD_HID_SendReport( schar * report, word len);
dword USBD_HID_GetPollingInterval ();

word HIDpoll( word frame );


#endif
