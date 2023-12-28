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
 * @file usbd_msc_core.h
 *
 * original draft from MCD Application Team
 */

#ifndef _USB_MSC_CORE_H_
#define _USB_MSC_CORE_H_

#define MSC_IN_EP      0x81
#define MSC_OUT_EP     0x02
#define MSC_MAX_PACKET 512
#define MSC_MEDIA_PACKET MSC_MAX_PACKET

#include "usbd_core.h"
#include "usbd_mass.h"

#define USB_MSC_CONFIG_DESC_SIZ  32

#define MSC_EPIN_SIZE  64
#define MSC_EPOUT_SIZE 64


extern USBDclassDefREC USBD_MSC_cb;

#endif
