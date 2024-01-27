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
 * @file usbh_core.h
 *
 * original draft from MCD Application Team
 */
#ifndef __USBH_CORE_H
#define __USBH_CORE_H

#include "../usbdef.h"
#include "usb-glue.h"

enum
{ MSC_PROTOCOL= 0x50
, CBI_PROTOCOL= 0x01
};

typedef enum
{ CTRL_START = 0
, CTRL_XFRC
, CTRL_HALTED
, CTRL_NAK
, CTRL_STALL
, CTRL_XACTERR
, CTRL_BBLERR
, CTRL_DATATGLERR
, CTRL_FAIL
} CTRL_STATUS;

/* Following states are used for CtrlXferStateMachine
 */
typedef enum
{ CTRL_IDLE =0
, CTRL_SETUP
, CTRL_SETUP_WAIT
, CTRL_DATA_IN
, CTRL_DATA_OUT
, CTRL_STATUS_IN
, CTRL_STATUS_OUT
, CTRL_ERROR
, CTRL_STALLED
}
CTRL_State;


#define USBH_MAX_ERROR_COUNT  2

/* Following states are used for EnumerationState
 */
typedef enum
{ ENUM_IDLE = 0
, ENUM_GET_FULL_DEV_DESC
, ENUM_SET_ADDR
, ENUM_GET_CFG_DESC
, ENUM_GET_FULL_CFG_DESC
, ENUM_GET_MFC_STRING_DESC
, ENUM_GET_PRODUCT_STRING_DESC
, ENUM_GET_SERIALNUM_STRING_DESC
, ENUM_SET_CONFIGURATION
, ENUM_DEV_CONFIGURED

} ENUM_State;


typedef struct
{ byte bLength;
  byte bDescriptorType;
  byte bInterfaceNumber;
  byte bAlternateSetting;  /* Value used to select alternative setting */
  byte bNumEndpoints;      /* Number of Endpoints used for this interface */
  byte bInterfaceClass;    /* Class Code (Assigned by USB Org) */
  byte bInterfaceSubClass; /* Subclass Code (Assigned by USB Org) */
  byte bInterfaceProtocol; /* Protocol Code */
  byte iInterface;         /* Index of String Descriptor Describing this interface */
}
USBHinterfaceDescRec;

typedef struct
{ byte   bIface;            /* JACS, interface what belongs to */
  byte   bLength;
  byte   bDescriptorType;
  byte   bEndpointAddress; /* indicates what endpoint this descriptor is describing */
  byte   bmAttributes;     /* specifies the transfer type. */
  word  wMaxPacketSize;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
  byte   bInterval;        /* is used to specify the polling interval of certain transfers. */
}
USBHepDescRec;

/* Following states are used for RequestState
 */
typedef struct
{ byte                 devAddr; // Belongs to devices
  byte                 speed;
  USBH_DevDesc_TypeDef Dev_Desc;
  USBH_CfgDesc_TypeDef Cfg_Desc;
  USBHinterfaceDescRec Itf_Desc[ USBH_MAX_NUM_INTERFACES ];  // 2
  USBHepDescRec        Ep_Desc [ USBH_MAX_NUM_ENDPOINTS  ];  // 3
} USBHdeviceRec;

typedef short( *hostClassLink)( byte );


typedef struct
{ byte              hcNumIn;
  byte              hcNumOut;
  byte              ep0size;
  byte             *buff;
  word              length;
  byte              errorcount;
  word              timer;
  CTRL_STATUS       status;
  USB_Setup_TypeDef setup;
  CTRL_State        state;
} USBH_Ctrl_TypeDef;



typedef struct
{ ENUM_State        EnumState;    /* Enumeration state Machine */
  USBH_Ctrl_TypeDef Control;
  USBHdeviceRec     deviceProp;

  short( *handleCtrlPkg )( byte epNum ); /* Package arrive manage */
} USBH_HOST_REC;

extern union USBclassBM USBHclassSignaler;

void * USBinitHOST( dword vbusPin );

void   usbHostSetXferDest( schar( *handleXferPkg )( byte epNum ) );       /* HostLibInitialized */
short  usbHostDeInit( void );
short  usbHostHandleEnum( byte ep );

enum
{ USB_HOST_DEINIT
, USB_HOST_DESC_AVAILABLE
, USB_HOST_DEV_ATTACHED
, USB_HOST_RESET_DEV
, USB_HOST_OVER_CURRENT
, USB_HOST_SPEED_DET
, USB_HOST_ADDR_ASSIGNED
, USB_HOST_ENUM_DONE
, USB_HOST_DEV_NOT_SUP
, USB_HOST_USER_APP
, USB_HOST_UNREC_ERR
, USB_HOST_SERIAL_STR
, USB_HOST_PRODUCT_STR
, USB_HOST_VENDOR_STR
};



void usbHostEvent( word what, void * args );   /* HostLibInitialized */

void usbHostGotConfigurationDescAvailable( USBH_CfgDesc_TypeDef *
                                         , USBHinterfaceDescRec *
                                         , USBHepDescRec        * );

short usbHostGotUserApplication( union USBclassBM p );

union USBclassBM
{ struct
  { word classAudio    : 1;  /** 0x01 Audio device class.              */
    word classCdc      : 1;  /** 0x02 Audio device class.              */
    word classHid      : 1;  /** 0x03  */
    word classPhysical : 1;  /** 0x05 Physical device class.           */
    word classImage    : 1;  /** 0x06 Still Imaging device class.      */
    word classPrinter  : 1;  /** 0x07 Printer device class.            */
    word classMassStor : 1;  /** 0x08 Mass Storage device class.       */
    word classHub      : 1;  /** 0x09 HUB device class.                */
    word classCsCsid   : 1;  /** 0x0B Smart Card device class.         */
    word classContSec  : 1;  /** 0x0D Content Security device class.   */
    word classVideo    : 1;  /** 0x0E Video device class.              */
    word classHealth   : 1;  /** 0x0F Personal Healthcare device class.*/
    word classAVideo   : 1;  /** 0x10 Audio/Video device class.        */
    word classBillBoard: 1;  /** 0x11 Billboard device class.          */
    word classCBridge  : 1;  /** 0x12 USB Type-C Bridge device class.  */
    word classDiag     : 1;  /** 0xDC Diagnostic device class.         */
    word classWless    : 1;  /** 0xE0 Wireless controller class.       */

    word addr;
  };

  dword atomic;
};


typedef struct USB_OTG_hc
{ byte *xferBuff;
  dword xferSzHC;
  dword xferCount;
  byte  toggleIn;
  byte  toggleOut;
}
USB_OTG_HC;

typedef struct
{ URB_STATE URB_State[ USB_OTG_MAX_TX_FIFOS ];
  USB_OTG_HC       hc[ USB_OTG_MAX_TX_FIFOS ];
  byte  devAddr      [ 8 ];                    /* free device address list ( 127 max ) */

  word used; /* Used channel bitmap */

  struct
  { short reload;                        /* Timer active     */
    short current;                       /* Timer next event */
    void ( * action )( short channel );  /* JACS, action to do on events */
  } sched[ USB_OTG_MAX_TX_FIFOS ];

  byte rxBuffer[ MAX_DATA_LENGTH ];
}
HCD_DEV;

void USBHxferOutControl( short channel );
void USBHxferInControl(  short channel );

hostClassLink USBHgetHandleAtConnect( USBHdeviceRec * ); // JACS, host class linker

void USBHsetControlChannelAddr( byte devAddr, short( *handle )( byte epNum ) );

extern HCD_DEV        USB_HOST;
extern USBH_HOST_REC  USB_Host;

void usbHIDrawArrived(   byte * );
void usbHIDkeyArrived(   byte * );
void usbHIDmouseArrived( byte * );

/* Link codlets
 */

hostClassLink USBHhubInterfaceInit( USBHdeviceRec *, void * handler );
hostClassLink USBHmscInterfaceInit( USBHdeviceRec *, void * handler );
hostClassLink USBHhidInterfaceInit( USBHdeviceRec *, void * handler );



/* ---------------------------------------------------------- */

/*Standard Feature Selector for clear feature command
 */
#define FEATURE_SELECTOR_ENDPOINT 0X00
#define FEATURE_SELECTOR_DEVICE   0X01



schar USBH_GetDescriptor( byte req_type
                              , word value_idx
                              , word bufLength );

schar USBH_Get_DevDesc( byte devAddr, byte length );
void  USBH_ParseDevDesc( USBH_DevDesc_TypeDef * dev_desc
                       , byte *buf , word length );

schar USBH_Get_StringDesc(byte string_index,
                                byte *buff,
                                word length);

schar USBH_SetCfg(           word configuration_value);
schar USBH_Get_CfgDesc(      word length);
schar USBH_SetAddress(       byte DeviceAddress);
schar USBH_ClrFeature(       byte ep_num, byte hcNum);
schar USBH_Issue_ClrFeature( byte ep_num);

schar USBHgetInterface( byte request, word value, void * buf, word len );

schar USBHsetInterface( byte request, word index, word value );
schar USBHsendInterface( byte request, word index, word value, void * buf, word len );

USBH_DescHeader_t * USBH_GetNextDesc( byte * pbuf
                                    , word * ptr );

schar USBH_SetDeviceFeature(   byte FeatureSelector, word wIndex );
schar USBH_ClearDeviceFeature( byte FeatureSelector, word wIndex );
schar USBHGetCfgDesc( word length );
schar parseCfgDesc(   word length );

byte UHOSTsetClassFeature(   byte FeatureSelector, word wIndex );
byte UHOSTclearClassFeature( byte FeatureSelector, word wIndex );
byte UHOSTgetClassStatus(    word wIndex, void * buff, word len );


void USBH_ParseStringDesc (byte* psrc, byte* pdest, word length );



/* --------------------------------------------------------------- */

#define USBH_SETUP_PKT_SIZE   8
#define USBH_MAX_PACKET_SIZE  0x40

schar USBH_CtlSendData   (      byte hcNum, byte * buff, word length );
schar USBH_CtlReceiveData(      byte hcNum, byte * buff, word length );

schar USBH_BulkReceiveData(      byte * buff, word length, byte hcNum );
schar USBH_BulkSendData (        byte * buff, word length, byte hcNum );
schar USBH_InterruptReceiveData( byte * buff, byte length, byte hcNum );
schar USBH_InterruptSendData(    byte * buff, byte length, byte hcNum );
schar USBH_CtlReq(               byte * buff, word length );
schar USBH_IsocReceiveData(      byte * buff, dword length, byte hcNum );
schar USBH_IsocSendData(         byte * buff, dword length, byte hcNum );

/* --------------------------------------------------------------- */



byte  USBHallocAddress();
void  USBHfreeDeviceChannels( byte addr );
byte  USBHfreeChannel ( byte hcNum  );
schar USBHchannelTimer( byte hcNum, word interval );

byte USBHdeallocAllChannel( );

byte UHOSTopenChannel( byte epNum
                     , byte epType
                     , word mps
                     , void (*action)( short ep ) );

struct USBHostConfigDef
{ word rxFifoSize;
  word txNPSize;
  word txPRSize;
};

extern struct USBHostConfigDef USBHostConfig;

/* --------------------------------------------------------------- */

#define  LE16( addr ) (((word)(*((byte *)(addr))))  + (((word)(*(((byte *)(addr)) + 1))) << 8))


/* bmRequestType :D7 Data Phase Transfer Direction
 */
#define  USB_REQ_DIR_MASK  0x80
#define  USB_H2D           0x00
#define  USB_D2H           0x80

#define USB_DEVICE_DESC_SIZE        18
#define USB_CONFIGURATION_DESC_SIZE  9

#endif


