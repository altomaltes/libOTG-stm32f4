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

#include <string.h>

#include "usbd_core.h"

DCD_DEV USB_DEV;

WEAK void usbDevEvent( word what, void * args ) {}



#define EPNUM( idx ) USB_DEV.inEp + ( ( idx & EPDIR_IN ) ? ( idx & 0xF ) : ( idx & 0xF ) + USB_OTG_MAX_TX_FIFOS ) /* Skip reserver space */

/**
 * @brief  Stall an endpoint.
 * @param epnum: endpoint address
 * @retval : status
 */
short  USBDepStall( byte epAddr )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->isStall= 1;
  ep->num    = epAddr & EPDIR_MASK;

  return( USBDsetStall( epAddr ));
}

/**
 * @brief  USBDsetupStage
 *         Handle the setup stage
 * @retval status
 */
short USBDsetupStage()
{ USB_SETUP_REQ req; USBDparseSetupRequest( &req );

  switch ( req.bmRequest & 0x1F )
  { case USB_REQ_RECIPIENT_DEVICE:    USBDstdDevReq( &req ); break;
    case USB_REQ_RECIPIENT_INTERFACE: USBDstdItfReq( &req ); break;
    case USB_REQ_RECIPIENT_ENDPOINT:  USBDstdEPReq(  &req ); break;

    default: USBDepStall( req.bmRequest & 0x80 ); break;
  }

  return( 0 );
}

/**
 * @brief  USBD_RunTestMode
 *         Launch test mode process
 * @retval status
 */
static schar  USBD_RunTestMode(  )
{ //USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCTL, SET_TEST_MODE.d32 ); ççç

  return( 0 );
}

/**
 * @brief  USBDdataOutStage
 *         Handle data out stage
 * @param  epnum: endpoint index
 * @retval status
 */
short USBDdataOutStage( byte epnum )
{ USB_OTG_EP * ep= USB_DEV.outEp + epnum;

  if ( epnum == 0 )
  {// ep= USB_DEV.outEp + 0; !!!

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
      { if ( USB_DEV.deviceStatus >= USB_OTG_CONFIGURED )
        { ep->doIt( epnum, ep->dev->doArgs );
        }
        USBDctlSendStatus();
  } } }

  else if ( USB_DEV.deviceStatus >= USB_OTG_CONFIGURED )
  { ep->doIt( epnum, ep->dev->doArgs );  // JACS
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
{ USB_OTG_EP * ep= USB_DEV.inEp + epnum;

  if ( epnum == 0 )
  { // ep= &[ 0 ];  !!!
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
        &&  ( ep->totalDataLen <  ep->ctlDataLen ))
        { USBDctlContinueSendData( NULL, 0);
          ep->ctlDataLen= 0;
          USBDepPrepareRx ( 0, NULL, 0 );   /* Start the transfer */
        }

        else
        { if ( USB_DEV.deviceStatus >= USB_OTG_CONFIGURED )
          { ep->doIt( epnum, ep->dev->doArgs );
          }
          USBDctlReceiveStatus(  );
    } } }

    if ( USB_DEV.testMode == 1 )
    { USBD_RunTestMode();
      USB_DEV.testMode= 0;
  } }

  else if ( USB_DEV.deviceStatus >= USB_OTG_CONFIGURED )
  { ep->doIt( epnum, ep->dev->doArgs );  // JACS, data arrived
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
{ USBDepOpen( EPDIR_OUT, USB_OTG_MAX_EP0_SIZE, EPTYPE_CONTROL, NULL ); /* Open EP0 OUT */
  USBDepOpen( EPDIR_IN , USB_OTG_MAX_EP0_SIZE, EPTYPE_CONTROL, NULL ); /* Open EP0 IN */

  USB_DEV.deviceStatus= USB_OTG_DEFAULT; /* Upon Reset call usr call back */
  usbDevEvent( USB_DEV_RESET, NULL );

  return( 0 );
}


/**
 * @brief  USBDsetCfg
 *        Configure device and start the interface
 * @param  cfgidx: configuration index
 * @retval status
 */
short USBDsetCfg( byte cfgIdx, byte action )
{ USBDclassDefREC ** const * ptr= USBdeviceDesc.classHandle; /* Walk all subdevices */
  DeviceListRec    * dList= USB_DEV.devList;

  while( *ptr )
  { static short (*jump)( word what, word cfgIdx, DeviceListRec * dList );     /* EP used by this device */

    jump= (***ptr).Ctrl;
    jump( action, cfgIdx, dList );
    dList++; ptr++;
  }

  usbDevEvent( USB_DEV_CONFIGURED, NULL );
  return( 0 );
}



/**
 * @brief  USBDisoINIncomplete
 *         Handle iso in incomplete event
 * @retval status
 */
short USBDisoINIncomplete()
{ return(( ***USBdeviceDesc.classHandle ).Ctrl( ACTION_ISO_INICOM
                                              , 0, 0 ));
}

/**
 * @brief  USBDisoOUTIncomplete
 *         Handle iso out incomplete event
 * @retval status
 */
short USBDisoOUTIncomplete()
{ return(( ***USBdeviceDesc.classHandle).Ctrl( ACTION_ISO_OUTICOM
                                             , 0, 0 ));
}

/**
 * @brief  USBD_DevConnected
 *         Handle device connection event
 * @retval status
 */
short USBD_DevConnected()
{ return( 0 );
}

/**
 * @brief  USBD_DevDisconnected
 *         Handle device disconnection event
 * @retval status
 */
short USBD_DevDisconnected()
{ return( 0 );
}


/**
* @brief  Configure an EP
* @param epdesc : Endpoint Descriptor
* @retval : status
*/
static short doNone( dword ep, void * a )
{ return( 0 );
}

void USBDepAction( byte epAddr
                 , short(*doIt)( dword what, void * args )
                 , void * args )
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->dev->doArgs= args ? args : ep->dev->doArgs;  /* JACS, NULL means no change */
  ep->doIt  = doIt ? doIt : doNone;
}

//void USBDepTimer( byte epAddr
//                 , word interval )
//{ USB_OTG_EP * ep= EPNUM( epAddr );
//
//  ep->reload=            /* Timer active     */
//  ep->current= interval; /* Timer next event */
//}

short USBDepOpen( byte epAddr
                , word epMps
                , byte epType
                , short(*doIt)( dword what, void * args ))
{ USB_OTG_EP * ep= EPNUM( epAddr );

  ep->num= epAddr & EPDIR_MASK;

  ep->maxpacket= epMps;
  ep->type= epType;

  if ( epAddr & EPDIR_IN )
  { ep->txFifoNum= ep->num; /* Assign a Tx FIFO */
  }

  if ( epType == EPTYPE_BULK ) /* Set initial data PID. */
  { ep->dataPidStart= 0;
  }

  USBDepAction(   epAddr, doIt  , NULL  );
  USBDepActivate( epAddr, epType, epMps );

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
    ep->txFifoNum = i;

    ep->type      = EPTYPE_CONTROL; /* Control until ep is activated */
    ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
    ep->xferBuffEp= 0;
    ep->xferLen   =
    ep->xferCount = 0;

/* Init ep structure OUT
 */
    ep = USB_DEV.outEp + i;
    ep->num  = i;
    ep->txFifoNum= i;

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

  ep->num   = epAddr & EPDIR_MASK;
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
                     , void * pbuf
                     , word   bufLen )
{ USB_OTG_EP * ep= EPNUM( epAddr );

/* setup and start the Xfer
 */
  ep->xferBuffEp= pbuf;
  ep->xferLen   = bufLen;
  ep->xferCount = 0;
  ep->num       = epAddr & EPDIR_MASK;

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

  ep->isStall= 0;
  ep->num    = epAddr & EPDIR_MASK;

  USBDepClearStall( epAddr );
  return( 0 );
}

DeviceListRec * USBDgetDevice( void * addr )
{ DeviceListRec * dList= USB_DEV.devList;

  while( dList->doArgs )
  { if ( dList->doArgs == addr )
    { return( dList );
    }
    dList++;
  }

  return( NULL );
}


/**
 * @brief  walkConfig
 * @retval status
 *
 * This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
//#define DTYPE_CDC_UNION            0x06    /** Union Functional Descriptor.             */

byte * parseDeviceConfig( byte * dst )
{ USBDclassDefREC ** const * ptr= USBdeviceDesc.classHandle;

  usbConfigDesc * header= (usbConfigDesc *)dst; dst += sizeof( *header );

  header->bLength       = 0x09;       /** Size of the descriptor, in bytes.*/
  header->bType= DTYPE_CONFIGURATION; /** Configuration descriptor.*/
  header->wTotalLength  = 0x09;       /** Size of the configuration descriptor header, and all sub descriptors inside the configuration. */
  header->bNumInterfaces= 0x00;       /** Total number of interfaces in the configuration.*/
  header->bConfigIndex  = 0x01;       /** Configuration index of the current configuration.*/
  header->iConfiguration= 0x00;       /** Index of a string descriptor describing the configuration.*/
  header->bmbmAttributes= 0xC0;       /** Mask of USB_CONFIG_ATTR_ masks. On all devices, this should include USB_CONFIG_ATTR_RESERVED at a minimum. */
  header->bMaxPower     = 0x32;       /** Maximum power consumption of the device. USB_CFG_POWER_MA() macro.*/

  byte epIn= 0x81;
  byte epOt= 0x01;
  DeviceListRec * dList= USB_DEV.devList;
  USB_DEV.inEp->dev=
  USB_DEV.outEp->dev= &USB_DEV.devList0;    /* phantom */

  for(  ptr= USBdeviceDesc.classHandle
     ; *ptr
     ;  ptr++, dList++  )                                /* Walk all subdevices */
  { const byte * src= (**ptr)->configDescriptor;
    byte firstIface= header->bNumInterfaces;

    dList->epIn= epIn;
    dList->epOt= epOt;
    dList->doArgs= *ptr;

    byte inSt= ( epIn & EPDIR_MASK ) - 1;
    byte otSt= ( epOt & EPDIR_MASK ) - 1;

    while( *src )   /* Iterate items list */
    { usbHeaderDescriptor * desc= (usbHeaderDescriptor *)src; /* "C" pointer arithmetic does not allow arbitrary increase  */
      usbHeaderDescriptor * calc= (usbHeaderDescriptor *)dst; /* "C" pointer arithmetic does not allow arbitrary increase  */
      memcpy( dst, src, desc->bLength );   /** Unmodofied by default */

      switch( desc->bType )
     { case DTYPE_CONFIGURATION:    /* skip provided header */
          src+=  desc->bLength;   /** point to next */
        continue;

        case DTYPE_ENDPOINT:
          if ( desc->iIdx & EPDIR_IN )        /** New IN endpoint     */
          { calc->iIdx+= inSt; epIn++;        /** Bias the current EP */
            USB_DEV.inEp[ calc->iIdx & EPDIR_MASK ]
                                .dev= dList;  /** Link the bunndle    */

          }
          else                                       /** New OUT endpoint     */
          { calc->iIdx += otSt; epOt++;              /** Bias the current EP  */
            USB_DEV.outEp[ calc->iIdx & EPDIR_MASK ]
                                .dev= dList;         /** Link the bunndle     */
          }
        break;

        case DTYPE_INTERFASEASSOC:
          dst[ 2 ] += firstIface;  /** Bias the current master */
        break;

        case DTYPE_INTERFACE:                 // Add interface
          calc->iIdx= header->bNumInterfaces++;                          /** Bias the current Iface */
          USB_DEV.devIface[ calc->iIdx ]= ptr-USBdeviceDesc.classHandle; /** Associate witch device */
        break;

        case DTYPE_CS_INTERFACE:
          if ( dst[ 2 ] == 0x06 )    /** DTYPE_CDC_UNION -> Patch union interfaces  */
          { dst[ 3 ] += firstIface;  /** Bias the current master */
            dst[ 4 ] += firstIface;  /** Bias the current slave */
          }
        break;
      }

      header->wTotalLength+= desc->bLength;
      src                 += desc->bLength;   /** point to next ( last one ) */
      dst                 += desc->bLength;   /** point to next */
  } }

		return( dst );
}




