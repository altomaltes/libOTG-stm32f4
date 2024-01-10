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
dword  usbOTGgetCurrentFrame( word delta );
dword  HCDgetXferCnt     ( byte ch_num );

byte   OTGgetCoreSpeed();

schar usbHOSTstartXferHC( byte hcNum, byte pid
                        , void * xferbuff, word xferLen );

schar USBHcoreInit ();
void  USBHhaltHC   (   volatile struct HC_STRUCT * );
schar USBHdoPingHC ( volatile struct HC_STRUCT * );
void  USBHhaltHCnum( byte idx );
byte  USBHresetPort( );
byte  USBHgetHCdad ( byte hcNum );
byte  USBHaddrHC   ( byte hcNum, byte devAddr );
byte  USBHpacketHC ( byte hcNum, word maxPacket );
schar USBHinitC    ( byte hcNum
                   , byte epAddr, byte epType
                   , byte devAddr, word maxPacket );

void  USBHdriveVbus      ( byte state );
void  usbHOSTinitFSLSPClkSel( byte freq );
byte  USBHisEvenFrame();
void  USBHstopHost   ();

word handleSofHostISR( word frame );  /** 0x03 Start of frame */
void usbHostGotDisconnected( byte devAddr );

short USBHreadPacket(  byte epNum, word size );  // JACS, split hardware
short USBHwritePacket( byte chNum, word size );

byte gotDevConnected();
byte gotDevDisconnected();
byte gotDevPortEnabled();
byte gotDevPortDisabled();


#endif
