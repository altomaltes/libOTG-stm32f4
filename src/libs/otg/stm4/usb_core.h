/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                         ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 */

#ifndef __USB_CORE_H__
#define __USB_CORE_H__

#include "uc.h"
#include "usb_conf.h"
#include "usb-glue.h"
#include "stm32f4usb.h"

#define HFIR_USEC_CLOCKS 5999 // ���

#define USB_OTG_SPEED_PARAM_HIGH 0
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL 1
#define USB_OTG_SPEED_PARAM_FULL 3

#define A_HOST        1
#define A_SUSPEND     2
#define A_PERIPHERAL  3
#define B_PERIPHERAL  4
#define B_HOST        5

enum USB_OTG_SPEED
{ USB_SPEED_UNKNOWN = 0
, USB_SPEED_LOW
, USB_SPEED_FULL
, USB_SPEED_HIGH
};


/* ---------------------------------------------------------- */

#define USB_OTG_EP_TX_DIS    0x0000
#define USB_OTG_EP_TX_STALL  0x0010
#define USB_OTG_EP_TX_NAK    0x0020
#define USB_OTG_EP_TX_VALID  0x0030

#define USB_OTG_EP_RX_DIS    0x0000
#define USB_OTG_EP_RX_STALL  0x1000
#define USB_OTG_EP_RX_NAK    0x2000
#define USB_OTG_EP_RX_VALID  0x3000

struct USB_OTG_CORE_HANDLE
{ byte  hostChannels;
  byte  devEndpoints;
  byte  speed;
  byte  dmaEnable;
  word  keepAlive;
  dword vbusPin;
};

extern struct USB_OTG_CORE_HANDLE USB_OTG_Core;

schar  usbOTGflushTxFifo( dword num);
schar  usbOTGflushRxFifo( );

dword  usbOTGgetMode  ();
enum USB_OTG_SPEED usbDEVgetDeviceSpeed();

short USBDep0Activate ( );
short USBDepStartRevc ( byte epAddr );

void  usbDEVinitSpeed         ( byte speed );
void  usbDEVactiveRemoteWakeup();
void  USBDEVungateClock       ();
void  usbDEVsetEPStatus       ( byte epAddr, dword Status);
dword usbDEVgetEPStatus       ( byte epAddr );
schar usbDEVdeInit            ( void ); /* Software Init */


void handleCanReadHOST  ();  /** 0x04 RxFIFO non-empty */
void handleNptxfemptyISR();  /** 0x05 Non-periodic TxFIFO empty */
void hndIncompPerXferISR();  /** 0x15 Incomplete periodic transfer(Host mode)/Incomplete isochronous OUT transfer(Device mode) */
void handlePtxfemptyISR ();  /** 0x1A Periodic TxFIFO empty */
void handlePortISR      ();  /** 0x18 Host port interrupt */
void handleChannelISR   ();  /** 0x19 Host channels interrupt */
void handleDisconnectISR();  /** 0x1D Disconnect detected interrupt */
void handleCanReadDEV   ();  /** 0x04 RxFIFO non-empty */
void handleIsoOUTcopmISR();  /** 0x15 Incomplete isochronous OUT transfer */
void handleMmisISR      ();  /** 0x01 Mode mismatch interrupt */
void handleInEpISR      ();  /** 0x12 IN endpoint interrupt */
void handleOutEpISR     ();  /** 0x13 OUT endpoint interrupt */
void handleIsoINcompISR ();  /** 0x14 Incomplete isochronous IN transfer */
void handleUsbResetISR  ();  /** 0x0C USB reset */
void handleEnumDoneISR  ();  /** 0x0D Enumeration done */
void handleConIDStChgISR();  /** 0x1C Connector ID status change */
void handleSofFrameISR  ();  /** 0x03 Start of frame */


void   usbOTGenableCommonInt( byte mode );
byte   usbHOSTsetUrbState( byte channel, byte state );
void * USBHgetBuffer();



#endif  /* __USB_CORE_H__ */


