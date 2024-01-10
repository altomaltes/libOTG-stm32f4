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
 * @file usbd_core.c
 *
 * original draft from MCD Application Team
 */

#include "usbd_core.h"

DCD_DEV USB_DEV;

#define EPNUM( idx ) USB_DEV.inEp + ( ( idx & 0x80 ) ? ( idx & 0xF ) : ( idx & 0xF ) + USB_OTG_MAX_TX_FIFOS ) /* Skip reserver space */

WEAK void USBdevGotReset       ( void )        {}
WEAK void USBdevGotConfigured  ( void )        {}
WEAK word USBdevGotSuspended   ( byte status ) { return( 0 ); }
WEAK void USBdevGotResumed     ( void )        {}

/**
 * @brief  Stall an endpoint.
 * @param epnum: endpoint address
 * @retval : status
 */
short  USBDepStall( byte epAddr )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->is_stall= 1;
  ep->num     =   epAddr & 0x7F;
//  ep->is_in   = ((epAddr & 0x80) == 0x80);

  return( USBDsetStall( epAddr ));
}

/**
 * @brief  USBDsetupStage
 *         Handle the setup stage
 * @retval status
 */
schar USBDsetupStage()
{ USB_SETUP_REQ req;

  USBDparseSetupRequest( &req );

  switch ( req.bmRequest & 0x1F )
  { case USB_REQ_RECIPIENT_DEVICE:    USBDstdDevReq( &req ); break;
    case USB_REQ_RECIPIENT_INTERFACE: USBDstdItfReq( &req ); break;
    case USB_REQ_RECIPIENT_ENDPOINT:  USBDstdEPReq(  &req ); break;

    default: USBDepStall( req.bmRequest & 0x80);    break;
  }

  return( 0 );
}

/**
 * @brief  USBD_RunTestMode
 *         Launch test mode process
 * @retval status
 */
static schar  USBD_RunTestMode(  )
{ //USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCTL, SET_TEST_MODE.d32 ); ���

  return( 0 );
}

/**
 * @brief  USBDdataOutStage
 *         Handle data out stage
 * @param  epnum: endpoint index
 * @retval status
 */
schar USBDdataOutStage( byte epnum )
{ USB_OTG_EP * ep;

  if ( epnum == 0 )
  { ep= &USB_DEV.outEp[ 0 ];

    if ( USB_DEV.deviceState == USB_OTG_EP0_DATA_OUT )
    { if ( ep->remDataLen > ep->maxpacket )
      { ep->remDataLen -= ep->maxpacket;

        if ( OTGgetDmaEnable() )
        { ep->xferBuffEp += ep->maxpacket; /* in slave mode this, is handled by the RxSTSQLvl ISR */
        }

        USBDctlContinueRx( ep->xferBuffEp
                          , MIN( ep->remDataLen
                               , ep->maxpacket ));
      }

      else
      { if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
        { ep->doIt( 0xFFFF );
        }
        USBDctlSendStatus();
  } } }

  else if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
  { USB_DEV.outEp[ epnum ].doIt( 0xFFFF );  // JACS
  }

  else
  { /* Do Nothing */
  }

  return( 0 );
}

/**
* @brief  USBDdataInStage
*         Handle data in stage
* @param  epnum: endpoint index
* @retval status
*/
schar USBDdataInStage( byte epnum )
{ USB_OTG_EP * ep;

  if ( epnum == 0 )
  { ep= &USB_DEV.inEp[ 0 ];
    if ( USB_DEV.deviceState == USB_OTG_EP0_DATA_IN )
    { if ( ep->remDataLen > ep->maxpacket )
      { ep->remDataLen -=  ep->maxpacket;

        if ( OTGgetDmaEnable() )
        { ep->xferBuffEp += ep->maxpacket; /* in slave mode this, is handled by the TxFifoEmpty ISR */
        }

        USBDctlContinueSendData( ep->xferBuffEp
                                , ep->remDataLen );
        USBDepPrepareRx( 0, NULL, 0 );
      }

      else                                  /* last packet is MPS multiple, so send ZLP packet */
      { if (( ep->totalDataLen %  ep->maxpacket == 0 )
        &&  ( ep->totalDataLen >= ep->maxpacket )
        &&  ( ep->totalDataLen <  ep->ctl_data_len ))
        { USBDctlContinueSendData( NULL, 0);
          ep->ctl_data_len= 0;
          USBDepPrepareRx ( 0, NULL, 0 );   /* Start the transfer */
        }

        else
        { if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
          { ep->doIt( 0xFFFF );
          }
          USBDctlReceiveStatus(  );
    } } }

    if ( USB_DEV.testMode == 1 )
    { USBD_RunTestMode();
      USB_DEV.testMode= 0;
  } }

  else if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
  { USB_DEV.inEp[ epnum ].doIt( 0xFFFF );  // JACS, data arrived
  }

  else
  {
  }

  return( 0 );
}


/**
 * @brief  USBDreset
 *         Handle Reset event
 * @retval status
 */
schar USBDreset()
{ USBDepOpen( 0x00, USB_OTG_MAX_EP0_SIZE, EPTYPE_CONTROL, NULL ); /* Open EP0 OUT */
  USBDepOpen( 0x80, USB_OTG_MAX_EP0_SIZE, EPTYPE_CONTROL, NULL ); /* Open EP0 IN */

  USB_DEV.deviceStatus= USB_OTG_DEFAULT; /* Upon Reset call usr call back */
  USBdevGotReset( );

  return( 0 );
}


/**
 * @brief  USBD_SetCfg
 *        Configure device and start the interface
 * @param  cfgidx: configuration index
 * @retval status
 */
schar USBD_SetCfg( byte cfgidx )
{ USBdeviceDesc.driver->Ctrl( ACTION_INIT, cfgidx );

  USBdevGotConfigured();  /* Upon set config call usr call back */

  return( 0 );
}

/**
 * @brief  USBDclrCfg
 *         Clear current configuration
 * @param  cfgidx: configuration index
 * @retval status: USBD_Status
 */
schar USBDclrCfg( byte cfgidx )
{ USBdeviceDesc.driver->Ctrl( ACTION_DEINIT, cfgidx );

  return( 0 );
}

/**
 * @brief  USBDisoINIncomplete
 *         Handle iso in incomplete event
 * @retval status
 */
schar USBDisoINIncomplete()
{ return( USBdeviceDesc.driver->Ctrl( ACTION_ISO_INICOM, 0 ));
}

/**
 * @brief  USBDisoOUTIncomplete
 *         Handle iso out incomplete event
 * @retval status
 */
schar USBDisoOUTIncomplete()
{ USBdeviceDesc.driver->Ctrl( ACTION_ISO_OUTICOM, 0 );

  return( 0 );
}

/**
 * @brief  USBD_DevConnected
 *         Handle device connection event
 * @retval status
 */
schar USBD_DevConnected()
{ return( 0 );
}

/**
 * @brief  USBD_DevDisconnected
 *         Handle device disconnection event
 * @retval status
 */
schar USBD_DevDisconnected()
{ return( 0 );
}


/**
 * @brief  USBinitDEV
 *         Initializes the device stack and load the class driver
 * @retval None
 */
void * USBinitDEV( dword flags )
{ OTGselectCore( flags );
  OTGsetCurrentMode( DEVICE_MODE );
  USBDcoreInit( USBdeviceDesc.epSizes );             /* set USB OTG core params */
//  OTGsetCurrentMode( DEVICE_MODE );

  return( &USBIrqHandlerDEV );  /* Be sure is linked */
}

/**
* @brief  Configure an EP
* @param epdesc : Endpoint Descriptor
* @retval : status
*/
static short doNone( dword ep )
{ return( 0 );
}

void USBDepAction( byte epAddr
                 , short(*doIt)( dword what ) )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->doIt= doIt ? doIt : doNone;
}

void USBDepTimer( byte epAddr
                 , word interval )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->reload=            /* Timer active     */
  ep->current= interval; /* Timer next event */
}


short USBDepOpen( byte epAddr
                , word epMps
                , byte epType
                , short(*doIt)( dword what ) )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->num= epAddr & 0x7F;

 // ep->is_in= ( epAddr & 0x80 ) != 0;
  ep->maxpacket= epMps;
  ep->type= epType;

  if ( epAddr & 0x80 )
  { ep->tx_fifo_num= ep->num; /* Assign a Tx FIFO */
  }

  if ( epType == EPTYPE_BULK ) /* Set initial data PID. */
  { ep->dataPidStart= 0;
  }

  USBDepAction( epAddr, doIt );

  USBDepActivate( epAddr, epType, epMps  );
  return( 0 );
}


/**
  * @brief  usbDEVdeInit
  *         Re-Initialize Device
  * @param  None
  * @retval status
  */
schar usbDEVdeInit( void ) /* Software Init */
{ USB_DEV.deviceStatus= USB_OTG_DEFAULT;

/* Init ep structure
 */
  for ( int i = 0
      ;     i < OTGgetDevEndpoints()
      ;     i ++ )
  { USB_OTG_EP * ep= USB_DEV.inEp + i;

    ep->num = i;   /* Init ep structure */
    ep->tx_fifo_num = i;

    ep->type      = EPTYPE_CONTROL; /* Control until ep is activated */
    ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
    ep->xferBuffEp= 0;
    ep->xferLen   =
    ep->xferCount = 0;

/* Init ep structure OUT
 */
    ep = USB_DEV.outEp + i;
    ep->num  = i;
    ep->tx_fifo_num= i;

    ep->type      = EPTYPE_CONTROL;              /* Control until ep is activated */
    ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
    ep->xferBuffEp= 0;
    ep->xferLen   =
    ep->xferCount = 0;
  }

  return( 0 );
}


/**
 * @brief  called when an EP is disabled
 * @param epAddr: endpoint address
 * @retval : status
 */
short USBDepClose( byte epAddr )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->num   = epAddr & 0x7F;
  //ep->is_in = (0x80 & epAddr) != 0;
  USBDepDeactivate( epAddr );

  return( 0 );
}


/**
 * @brief
 * @retval : status
 */
void DCDdone()
{ USB_DEV.deviceStatus= 1;
  USBDstopDevice();
}

/**
 * @brief  USBDepPrepareRx
 * @param epAddr: endpoint address
 * @param pbuf: pointer to Rx buffer
 * @param buf_len: data length
 * @retval : status
 */
short USBDepPrepareRx( byte   epAddr
                     , byte * pbuf
                     , word   bufLen )
{ USB_OTG_EP * ep= EPNUM( epAddr );

/* setup and start the Xfer
 */
  ep->xferBuffEp= pbuf;
  ep->xferLen   = bufLen;
  ep->xferCount = 0;
  ep->num       = epAddr & 0x7F;

//  if ( OTGgetDmaEnable() )    // hardware later
//  { ep->dmaAddr= (dword)pbuf;
//  }

  if ( ep->num  )
  { USBDepStartXrecv( ep->num, ep->xferLen, ep->maxpacket );
  }
  else
  { USBDep0StartRecv(          ep->xferLen= ep->maxpacket );
  }

  return 0;
}

/**
 * @brief  Clear stall condition on endpoints.
 * @param epnum: endpoint address
 * @retval : status
 */
short  USBDepClrStall( byte epAddr )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->is_stall= 0;
  ep->num     = epAddr & 0x7F;
//  ep->is_in   = ((epAddr & 0x80) == 0x80);

  USBDepClearStall( epAddr );
  return( 0 );
}


/**
 * @brief  handleSofDevISR
 *         Handles the start-of-frame interrupt in host mode.
 *         schedule for channel jobs
 * @param  none
 * @retval status
 */
word handleSofDevISR( word frNum )  /** 0x03 Start of frame */
{ if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
  { schar epNum= frNum & 0x0F;      /* Spread over 15 values, 24 of wich used */
    frNum >>= 4;                    /* A step over 16 ms */

    if ( epNum < OTGgetDevEndpoints() )
    { USB_OTG_EP * ep= USB_DEV.inEp + epNum;

      if ( ep->reload )             /* Active timer */
      { if ( -- ep->current <= 0 )  /* time reached */
        { ep->doIt( frNum );        /* Fire action  1/32 ms */
          ep->current= ep->reload;  /* Trhow again  */
      } }

      ep= USB_DEV.inEp + epNum;
      if ( ep->reload )             /* Active timer */
      { if ( -- ep->current <= 0 )  /* time reached */
        { ep->doIt( frNum );        /* Fire action  1/32 ms */
          ep->current= ep->reload;  /* Trhow again  */
    } } }

    else if ( epNum == ( USB_OTG_MAX_TX_FIFOS * 2 ) )        /* System schedule, symetric */
    {
  } }

  return( 0 );
}


