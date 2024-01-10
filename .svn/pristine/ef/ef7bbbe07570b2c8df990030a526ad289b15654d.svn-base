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
 * @file usbh_msc_core.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBH_MSC_CORE_H
#define __USBH_MSC_CORE_H

#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbd_mass.h"


typedef struct _MSC_Process /* Structure for MSC process */
{ byte hcNumIn;
  byte hcNumOut;

  word MSBulkInEpSize;
  word MSBulkOutEpSize;
  byte maxLun;

  byte buff[ 0x200 ];
}
MSC_Machine_TypeDef;


extern MSC_Machine_TypeDef MSC_Machine;

schar USBH_MSC_Machine( byte ep );


#endif



