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
 * @file usbh_hid_core.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBH_HID_CORE_H
#define __USBH_HID_CORE_H

#include "usbh_core.h"
#include "usb_hid.h"


#define HID_MIN_POLL 10

/* States for HID State Machine */
typedef enum
{ HID_IDLE= 0
, HID_SEND_DATA
, HID_BUSY
, HID_GET_DATA
, HID_SYNC
, HID_POLL
, HID_ERROR
}
HID_State;

typedef enum
{ HID_REQ_VOID = 0
, HID_REQ_GET_REPORT_DESC
, HID_REQ_GET_HID_DESC
, HID_REQ_SET_IDLE
, HID_REQ_SET_PROTOCOL
, HID_REQ_SET_REPORT
, HID_REQ_CLEAR
, HID_REQ_FIREUP
, HID_REQ_IDLE
}
HID_CtlState;

typedef struct HID_cb
{ void  ( *Init   ) (void);
  void  ( *Decode ) (byte *data);
} HID_cb_TypeDef;

typedef  struct
{ byte   ReportID;
  byte   ReportType;
  word  UsagePage;
  dword  Usage[2];
  dword  NbrUsage;
  dword  UsageMin;
  dword  UsageMax;
  int    LogMin;
  int    LogMax;
  int    PhyMin;
  int    PhyMax;
  int    UnitExp;
  dword  Unit;
  dword  ReportSize;
  dword  ReportCnt;
  dword  Flag;
  dword  PhyUsage;
  dword  AppUsage;
  dword  LogUsage;
}
HID_Report_TypeDef;

/* Structure for HID process */
typedef struct
{ byte             buff[ 64 ];
  byte             hcNumIn;
  byte             hcNumOut;
  HID_State        state;
  byte             HIDIntOutEp;
  byte             HIDIntInEp;
  HID_CtlState     ctl_state;
  word             length;
  byte             ep_addr;
  word             poll;
//  volatile word    timer;
  void (*cb)( byte * report);
}
HID_Machine_TypeDef;

schar USBH_Set_Report( byte reportType
                     , byte reportId
                     , byte reportLen
                     , byte* reportBuff );


#endif /* __USBH_HID_CORE_H */


