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
 * @file usbh_msc_bot.h
 *
 * original draft from MCD Application Team
 */
#ifndef __USBH_MSC_BOT_H__
#define __USBH_MSC_BOT_H__

typedef union _USBH_CBW_Block
{ struct __CBW
  { dword CBWSignature;
    dword CBWTag;
    dword CBWTransferLength;
    byte CBWFlags;
    byte CBWLUN;
    byte CBWLength;
    byte CBWCB[16];
  } field;

  byte CBWArray[31];
} HostCBWPkt_TypeDef;

enum MSCState
{ USBH_MSC_RESET
, USBH_MSC_GET_MAX_LUN
, USBH_MSC_REQUEST_SENSE
, USBH_MSC_TEST_UNIT_READY
, USBH_MSC_READ_CAPACITY10
, USBH_MSC_MODE_SENSE6
, USBH_MSC_DEFAULT_APPLI_STATE
, USBH_MSC_CTRL_ERROR_STATE
, USBH_MSC_UNRECOVERED_STATE

, USBH_MSC_IDLE
, USBH_MSC_READ10
, USBH_MSC_WRITE10
};


enum BOTState
{ USBH_BOT_SENT_CBW
, USBH_BOT_DATAIN_STATE
, USBH_BOT_DATAOUT_STATE
, USBH_BOT_RECEIVE_CSW_STATE
, USBH_BOT_DECODE_CSW
};



typedef struct
{ enum MSCState  MSCState;
  enum BOTState  BOTState;
  byte * pRxTxBuff;
  word   DataLength;
  byte   BOTXferErrorCount;
} USBH_BOTXfer_TypeDef;


typedef union _USBH_CSW_Block
{ struct __CSW
  { dword CSWSignature;
    dword CSWTag;
    dword CSWDataResidue;
    byte  CSWStatus;
  } field;

  byte CSWArray[ 13 ];

} HostCSWPkt_TypeDef;



#define USBH_MSC_BOT_CBW_SIGNATURE        0x43425355
#define USBH_MSC_BOT_CBW_TAG              0x20304050
#define USBH_MSC_BOT_CSW_SIGNATURE        0x53425355
#define USBH_MSC_BOT_CBW_PACKET_LENGTH    31

#define USBH_MSC_CSW_DATA_LENGTH          0x000D
#define USBH_MSC_CSW_LENGTH               13
#define USBH_MSC_CSW_MAX_LENGTH           63

/* CSW Status Definitions */
#define USBH_MSC_CSW_CMD_PASSED           0x00
#define USBH_MSC_CSW_CMD_FAILED           0x01
#define USBH_MSC_CSW_PHASE_ERROR          0x02

#define USBH_MSC_SEND_CSW_DISABLE         0
#define USBH_MSC_SEND_CSW_ENABLE          1

#define USBH_MSC_DIR_IN                   0
#define USBH_MSC_DIR_OUT                  1
#define USBH_MSC_BOTH_DIR                 2

//#define USBH_MSC_PAGE_LENGTH                 0x40
#define USBH_MSC_PAGE_LENGTH              512


#define CBW_CB_LENGTH                     16
#define CBW_LENGTH                        10
#define CBW_LENGTH_TEST_UNIT_READY         6

#define MAX_BULK_STALL_COUNT_LIMIT       0x04   /* If STALL is seen on Bulk  Endpoint continuously, this means
                                         that device and Host has phase error
                                         Hence a Reset is needed */
//extern USBH_BOTXfer_TypeDef USBH_MSC_BOTXferParam;

void USBH_MSC_Init();
void USBH_MSC_HandleBOTXfer(    short ep );
void USBH_MSC_HandleBOTXferIn(  short ep );
void USBH_MSC_HandleBOTXferOut( short ep );
void USBH_MSC_XferStart(        short ep );

schar USBH_MSC_BOT_Abort( byte direction );

#endif  //__USBH_MSC_BOT_H__


