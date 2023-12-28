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
 * @file usbh_hid_mouse.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBH_HID_MOUSE_H
#define __USBH_HID_MOUSE_H

#include "usbh_hid_core.h"

typedef struct _HID_MOUSE_Data
{ byte x;
  byte y;
  byte z;               /* Not Supported */
  byte button;
}
HID_MOUSE_Data_TypeDef;


extern HID_cb_TypeDef HID_MOUSE_cb;
extern HID_MOUSE_Data_TypeDef	 HID_MOUSE_Data;

void  USR_MOUSE_Init (void);
void  USR_MOUSE_ProcessData( HID_MOUSE_Data_TypeDef *data );

#endif /* __USBH_HID_MOUSE_H */

