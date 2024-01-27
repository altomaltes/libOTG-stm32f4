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

/* per device endpoints
 */
typedef struct
{ struct USBDclassDefREC * doArgs; /* user scratch           */
  byte epIn;                       /* First free in endpoint  */
  byte epOt;                       /* First free out endpoint */
} DeviceListRec;                   /* four devices combo      */

typedef struct
{ byte   num;
  byte   type;
  word   txFifoNum;
  dword  maxpacket;

  byte   isStall     : 1;
  byte   evenOddFrame: 1;

/* transaction level variables
 */
  byte *xferBuffEp;
  dword xferLen;
  dword xferCount;

  word remDataLen;
  word totalDataLen;
  word ctlDataLen;
  word dataPidStart;

/* JACS, SCHED related
 */
  short reload;                  /* Timer active     */
  short current;                 /* Timer next event */
  short (*doIt)( dword what      /* action callback  */
               , void * args );
  DeviceListRec * dev;           /* Args for above   */
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
{ short ( *Ctrl  )( word act, word conf, DeviceListRec * );
  short ( *Setup )( USB_SETUP_REQ * req , DeviceListRec * );
  const byte * configDescriptor;
} USBDclassDefREC;


typedef struct
{ dword USBD_ep_status  ; /* Must be dword aligned for DMA transfers */
  dword USBD_default_cfg;
  dword USBD_cfg_status ;

  byte       deviceConfig;
  byte       deviceState;
  byte       deviceStatus;
  byte       testMode;
  byte       devRemoteWakeup;
  USB_OTG_EP inEp [ USB_OTG_MAX_TX_FIFOS ];
  USB_OTG_EP outEp[ USB_OTG_MAX_TX_FIFOS ];
  byte       setupPacket[ 8*3 ];

  DeviceListRec devList0;
  DeviceListRec devList[ 4 ];
  byte         devIface[ 8 ]; /* Devices associated to interfaces */
}
DCD_DEV;


#define MAX_NUM_EP 16

typedef struct
{ USBDclassDefREC * hnd;
} USBDclassInstance;

typedef struct
{ const usbDeviceDesc * deviceDescriptor;
  const char * strings[ 6 ]; // 0
  const word epSizes[ 8 ];

#if ( USBD_LPM_ENABLED == 1 )
  const char * GetBOSDescriptor;
#endif

  USBDclassDefREC ** classHandle[];

} USBD_DEVICE;


void * USBinitDEV( dword flags );

void USBDepAction( byte epAddr
                 , short(*doIt)( dword what, void * args )
                 , void * args );

short USBDsetCfg( byte cfgidx, byte action );

void  USBDparseSetupRequest( USB_SETUP_REQ * );

short  USBDstdDevReq( USB_SETUP_REQ * );
short  USBDstdItfReq( USB_SETUP_REQ * );
short  USBDstdEPReq(  USB_SETUP_REQ * );
void   USBDctlError(  USB_SETUP_REQ * );
byte * USBDgetString( const char * desc );

int USBDreadPacket(  byte epNum, word size );
int USBDwritePacket( byte epNum, word size );
int USBDgetTxCount(  byte epNum );            /* Available size */
int USBDgetRxCount(  byte epnum );

extern DCD_DEV USB_DEV;
extern const USBD_DEVICE USBdeviceDesc;

DeviceListRec * USBDgetDevice( void * addr );

enum
{ USB_DEV_DEINIT
, USB_DEV_RESET
, USB_DEV_CONN
, USB_DEV_DISCON
, USB_DEV_CONFIGURED
, USB_DEV_SUSPEND
, USB_DEV_RESUMED
};


void usbDevEvent( word what, void * args );   /* HostLibInitialized */


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
enum
{ USB_OTG_NULL
, USB_OTG_DEFAULT
, USB_OTG_SUSPENDED

, USB_OTG_ADDRESSED
, USB_OTG_CONFIGURED
, USB_OTG_RACING
};


/* -------------------------------------------------------- */

short USBDctlSendData        ( const void * pbuf, word len );
short USBDctlContinueSendData( const void * pbuf, word len );
short USBDctlPrepareRx       ( byte * pbuf, word len );
short USBDctlContinueRx      ( byte * pbuf, word len );
short USBDctlSendStatus      ( );
short USBDctlReceiveStatus   ( );

short USBDepPrepareRx( byte epnum, void * pbuf, word buf_len );
short USBDepOpen     ( byte epnum, word ep_mps, byte epType, short(*doIt)( dword what, void * args ));
short USBDepClose    ( byte epnum );
short USBDepTx       ( byte epnum, const void * pbuf, word buf_len );
short USBDepStall    ( byte epnum );
short USBDepClrStall ( byte epnum );
short USBDepFlush    ( byte epnum );


byte * parseDeviceConfig( byte * dst );

#endif /* __USBD_CORE_H */


