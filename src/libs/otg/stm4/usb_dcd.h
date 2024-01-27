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
void  DCDdevConnect   ();
void  DCDdevDisconnect();
dword DCD_Handle_ISR   ();

/* --------------- INT ----------------------
 */
word  handleSofDevISR   ( word frame );  /** 0x03 Start of frame */

short USBDdataOutStage    ( byte epnum );
short USBDdataInStage     ( byte epnum );
short USBDsetupStage      ( );

short USBDreset           ( );
short USBDsuspend         ( );
short USBDresume          ( );
short USBDisoINIncomplete ( );
short USBDisoOUTIncomplete( );

short USBD_DevConnected   ( );
short USBD_DevDisconnected( );

short USBD_OTG_EP1OUT_ISR_Handler();
short USBD_OTG_EP1IN_ISR_Handler ();


#endif

