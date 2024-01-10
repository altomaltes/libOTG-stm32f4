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

#define CDC_OUT_EP 0x01
#define CDC_IN_EP  0x81
#define CDC_CMD_EP 0x82

#define CDC_DATA_MAX_PACKET_SIZE 0X40
#define APP_RX_DATA_SIZE         80

#define USB_CDC_CONFIG_DESC_SIZ   (67)

#define STANDARD_ENDPOINT_DESC_SIZE 0x09
#define CDC_CMD_PACKET_SIZE           32


typedef struct
{
  short ( *pIf_Init   )( void );
  short ( *pIf_DeInit )( void );
  short ( *pIf_Ctrl   )( dword Cmd, byte * Buf, byte Len );
  short ( *pIf_DataTx )( dword epNum );
  short ( *pIf_DataRx )( dword epNum );
  short ( *pIf_DataCt )( dword epNum );
}
CDC_IF_Prop_TypeDef;
extern USBDclassDefREC  USBD_CDC_cb;

#endif /* __USB_CDC_CORE_H_ */
