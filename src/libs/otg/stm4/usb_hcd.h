/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below:
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 *
 * @file usb_hcd.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USB_HCD_H__
#define __USB_HCD_H__

#include "usb_core.h"
#include "usb_hcd.h"

dword  HCDsubmitRequest( byte hcNum, byte dataPid, byte * buff, word len );
dword  HCDgetCurrentSpeed();
dword  HCDresetPort      ();
dword  HCDgetCurrentFrame( word delta );
dword  HCDgetXferCnt     ( byte ch_num );

byte   OTGgetCoreSpeed();

schar USBstartXferHC( byte hcNum, byte pid
                     , void * xferbuff, word xferLen );


schar USBcoreInitHost  ();
void  USBhaltHC(   volatile struct HC_STRUCT * );
schar USBdoPingHC( volatile struct HC_STRUCT * );
void  USBhaltHCnum( byte idx );
byte  USB_OTG_ResetPort       ( );

byte USBgetHCdad( byte hcNum );
byte USBaddrHC(   byte hcNum, byte devAddr );
byte USBpacketHC( byte hcNum, word maxPacket );
schar USBinitHC(   byte hcNum
              ,   byte epAddr, byte epType
              ,   byte devAddr, word maxPacket );

void  USBdriveVbus      ( byte state );
void  OTGinitFSLSPClkSel( byte freq );
byte  USB_OTG_IsEvenFrame();
void  USB_OTG_StopHost   ();

word handleSofHostISR( word frame );  /** 0x03 Start of frame */
void usbHostGotDisconnected( byte devAddr );

short USBHreadPacket(  byte epNum, word size );  // JACS, split hardware
short USBHwritePacket( byte chNum, word size );

/* --------------- INT ------------------------- */
byte gotDevConnected();
byte gotDevDisconnected();
byte gotDevPortEnabled();
byte gotDevPortDisabled();



#endif
