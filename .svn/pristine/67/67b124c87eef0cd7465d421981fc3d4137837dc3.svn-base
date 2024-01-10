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
 * @file usb_dcd.h
 *
 * original draft from MCD Application Team
 */

#ifndef __DCD_H__
#define __DCD_H__

#include "usb_core.h"

void  DCDinit          ();
void  DCDdone          ();
void  DCD_DevConnect   ();
void  DCD_DevDisconnect();
void  USBDepAction     ( byte epNum, schar (*doIt)( byte ep ) );
void  USBDepTimer      ( byte epAddr, word interval );
dword DCD_Handle_ISR   ();

/* --------------- INT ----------------------
 */
word  handleSofDevISR   ( word frame );  /** 0x03 Start of frame */
word  USBdevGotSuspended( byte status );
void  USBdevGotResumed  ( void );
word  handleSofDevISR   ( word frNum );  /** 0x03 Start of frame */

schar USBDdataOutStage    ( byte epnum );
schar USBDdataInStage     ( byte epnum );
schar USBDsetupStage      ( );

schar USBDreset           ( );
schar USBDsuspend         ( );
schar USBDresume          ( );
schar USBDisoINIncomplete ( );
schar USBDisoOUTIncomplete( );

schar USBD_DevConnected   ( );
schar USBD_DevDisconnected( );

dword USBD_OTG_EP1OUT_ISR_Handler();
dword USBD_OTG_EP1IN_ISR_Handler ();




#endif

