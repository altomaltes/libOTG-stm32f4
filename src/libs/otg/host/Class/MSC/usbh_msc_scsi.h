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
 * @file usbh_msc_scsi.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBH_MSC_SCSI_H__
#define __USBH_MSC_SCSI_H__

#include "./usbh_msc_bot.h"

typedef enum
{ USBH_MSC_OK         = 0
, USBH_MSC_FAIL       = 1
, USBH_MSC_PHASE_ERROR= 2
} USBH_MSC_Status_TypeDef;

typedef enum
{ CMD_UNINITIALIZED_STATE= 0
, CMD_SEND_STATE
, CMD_WAIT_STATUS
} CMD_STATES_TypeDef;



typedef struct
{ byte hcNumIn;  byte MSBulkInEp;  word MSBulkInEpSize;
  byte hcNumOut; byte MSBulkOutEp; word MSBulkOutEpSize;

  byte maxLun;

  HostCBWPkt_TypeDef USBH_MSC_CBWData;
  HostCSWPkt_TypeDef USBH_MSC_CSWData;

  dword MSCapacity;
  dword MSSenseKey;
  dword MSWriteProtect;  // Keep aligned
  word  MSPageLength;

  USBH_BOTXfer_TypeDef xfer;

  dword  botRemainingDataLength;
  byte * botDatapointer;

  byte dataInBuffer[  512 ];

} MassStorageParameter_TypeDef;


extern MassStorageParameter_TypeDef USBH_MSC_Param;


#define OPCODE_TEST_UNIT_READY            0X00
#define OPCODE_READ_CAPACITY10            0x25
#define OPCODE_MODE_SENSE6                0x1A
#define OPCODE_READ10                     0x28
#define OPCODE_WRITE10                    0x2A
#define OPCODE_REQUEST_SENSE              0x03

#define DESC_REQUEST_SENSE                0X00
#define ALLOCATION_LENGTH_REQUEST_SENSE   63
#define XFER_LEN_READ_CAPACITY10           8
#define XFER_LEN_MODE_SENSE6              63

#define MASK_MODE_SENSE_WRITE_PROTECT     0x80
#define MODE_SENSE_PAGE_CONTROL_FIELD     0x00
#define MODE_SENSE_PAGE_CODE              0x3F
#define DISK_WRITE_PROTECTED              0x01

short USBHtestUnitReady();
short MSCrespModeSense6(       short pDrv);
short MSrespRequestSense(      short pDrv);
short MSCaskForModeSense6(     short pDrv);
short MSCrespReadCapacity10(   short pDrv);
short MSCaskForRequestSense(   short pDrv);
short MSCaskForReadCapacity10( short pDrv );

dword USBH_MSCgetCapacity( short pDrv );
short USBH_MSCwrite10(     short pDrv, const void *, dword, dword );
short USBH_MSCread10(      short pDrv,       void *, dword, dword );

void  USBH_MSC_StateMachine();




#endif  //__USBH_MSC_SCSI_H__

