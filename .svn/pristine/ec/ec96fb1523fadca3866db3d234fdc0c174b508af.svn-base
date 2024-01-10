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
 * @file usbh_hid_keybd.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBH_HID_KEYBD_H
#define __USBH_HID_KEYBD_H

#include "usb_conf.h"
#include "usbh_hid_core.h"


#define QWERTY_KEYBOARD
//#define AZERTY_KEYBOARD

#define KBD_LEFT_CTRL    0x01
#define KBD_LEFT_SHIFT   0x02
#define KBD_LEFT_ALT     0x04
#define KBD_LEFT_GUI     0x08
#define KBD_RIGHT_CTRL   0x10
#define KBD_RIGHT_SHIFT  0x20
#define KBD_RIGHT_ALT    0x40
#define KBD_RIGHT_GUI    0x80

#define  KBR_MAX_NBR_PRESSED                            6


extern HID_cb_TypeDef HID_KEYBRD_cb;
void  USR_KEYBRD_Init (void);
void  USR_KEYBRD_ProcessData (byte pbuf);

#endif /* __USBH_HID_KEYBD_H */

