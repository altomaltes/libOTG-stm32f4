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
 * @file usbd_core.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#include "../usbdef.h"
#include "usb-glue.h"

typedef struct
{ byte   num;
  byte   type;
  word   tx_fifo_num;
  dword  maxpacket;

  byte   is_in         : 1;
  byte   is_stall      : 1;
  byte   even_odd_frame: 1;

/* transaction level variables
 */
  byte *xferBuffEp;
  dword xferLen;
  dword xferCount;

  dword dmaAddr;

  word remDataLen;
  word totalDataLen;
  word ctl_data_len;
  word dataPidStart;

/* SCHED related
 */
  short reload;              /* Timer active     */
  short current;             /* Timer next event */
  void (*doIt)( word what ); // JACS, action callback
}
USB_OTG_EP;


typedef  struct
{ byte  bmRequest;
  byte  bRequest;
  word  wValue;
  word  wIndex;
  word  wLength;
} USB_SETUP_REQ;

typedef struct
{ schar ( *Ctrl  )( byte act, word idx );
  schar ( *Setup )( USB_SETUP_REQ  *req );
  byte * configDescriptor;
} USBDclassDefREC;

typedef struct
{ byte        deviceConfig;
  byte        deviceState;
  byte        deviceStatus;
  byte        testMode;
  byte        devRemoteWakeup;
  USB_OTG_EP  inEp [ USB_OTG_MAX_TX_FIFOS ];
  USB_OTG_EP  outEp[ USB_OTG_MAX_TX_FIFOS ];
  byte        setupPacket[ 8*3 ];
}
DCD_DEV;

typedef struct
{ const char * deviceDescriptor;
  const char * strings[ 6 ]; // 0

  USBDclassDefREC * driver;

#if ( USBD_LPM_ENABLED == 1 )
  const char * GetBOSDescriptor;
#endif

} USBD_DEVICE;


void * USBinitD( word flags );

schar USBD_ClrCfg( byte cfgidx );
schar USBD_SetCfg( byte cfgidx );

void  USBD_ParseSetupRequest( USB_SETUP_REQ * );

schar USBD_StdDevReq( USB_SETUP_REQ * );
schar USBD_StdItfReq( USB_SETUP_REQ * );
schar USBD_StdEPReq(  USB_SETUP_REQ * );

void  USBD_CtlError( USB_SETUP_REQ * );

byte * USBDgetString( const char * desc );

short USBDreadPacket(  byte epNum, word size );
short USBDwritePacket( byte epNum, word size );
word  USBD_GetRxCount( byte epnum );

extern DCD_DEV USB_DEV;
extern USBD_DEVICE USBdeviceDesc;

/* ---------------------------------------------------- */
#define  USB_LEN_DEV_QUALIFIER_DESC   0x0A

#define  USBD_IDX_LANGID_STR          0x00
#define  USBD_IDX_MFC_STR             0x01
#define  USBD_IDX_PRODUCT_STR         0x02
#define  USBD_IDX_SERIAL_STR          0x03
#define  USBD_IDX_CONFIG_STR          0x04
#define  USBD_IDX_INTERFACE_STR       0x05

#define USB_CONFIG_REMOTE_WAKEUP   2
#define USB_CONFIG_SELF_POWERED    1

#define USB_FEATURE_EP_HALT        0
#define USB_FEATURE_REMOTE_WAKEUP  1
#define USB_FEATURE_TEST_MODE      2


enum
{ ACTION_INIT
, ACTION_DEINIT
, ACTION_ISO_INICOM
, ACTION_ISO_OUTICOM
};

/*  Device Status
 */
#define USB_OTG_DEFAULT     1
#define USB_OTG_ADDRESSED   2
#define USB_OTG_CONFIGURED  3
#define USB_OTG_SUSPENDED   4




#define  SWAPBYTE(addr)        (((word)(*((byte *)(addr)))) + \
                               (((word)(*(((byte *)(addr)) + 1))) << 8))

#define LOBYTE(x)  ((byte)((x) & 0x00FF))
#define HIBYTE(x)  ((byte)(((x) & 0xFF00) >>8))


/* -------------------------------------------------------- */

schar USBD_CtlSendData        ( byte * pbuf, word len );
schar USBD_CtlContinueSendData( byte * pbuf, word len );
schar USBD_CtlPrepareRx       ( byte * pbuf, word len );
schar USBD_CtlContinueRx      ( byte * pbuf, word len );
schar USBD_CtlSendStatus      ( );
schar USBD_CtlReceiveStatus   ( );

dword USBDepPrepareRx( byte epnum, byte * pbuf, word buf_len );
dword USBDepOpen       ( byte epnum, word ep_mps, byte epType, void (*doIt)( word what ) );
dword USBDepClose      ( byte epnum );
dword USBDepTx         ( byte epnum, byte * pbuf, word buf_len );
dword USBDepStall      ( byte epnum );
dword USBDepClrStall   ( byte epnum );
dword USBDepFlush      ( byte epnum );



#endif /* __USBD_CORE_H */


