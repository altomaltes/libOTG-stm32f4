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
 * @file usbh_core.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_core.h"
#include "usb_hcd.h"



union USBclassBM USBHclassSignaler;

WEAK void usbHostEvent( word what, void * args ) {}   /* HostLibInitialized */

WEAK void usbHostGotConfigurationDescAvailable( USBH_CfgDesc_TypeDef * c
                                              , USBHinterfaceDescRec * i
                                              , USBHepDescRec        * p ) {}

WEAK short usbHostGotUserApplication( union USBclassBM p )
{ USBHclassSignaler.addr= USB_Host.deviceProp.devAddr == DEFAULT_DEVADDR
                        ? 1 : 1 << USB_Host.deviceProp.devAddr;
  return( 0 );
}

HCD_DEV       USB_HOST;
USBH_HOST_REC USB_Host;

/**
  * @brief  gotDevConnected
  *         USB Connect callback function from the Interrupt.
  * @param  selected device
  * @retval Status
  */
byte gotDevConnected()
{ mDelay( 100 );                      /* wait denounce delay */
  HCDresetPort();                     /* Apply a port RESET */
  usbHostEvent( USB_HOST_RESET_DEV, NULL ); /* User RESET callback*/
  return( 0 );
}

/**
  * @brief  USBHsetControlChannelAddr
  *
  * @param  selected device
  * @retval Status
  */
void USBHsetControlChannelAddr( byte devAddr, short( *handle )( byte epNum ) )
{ USBHaddrHC( USB_Host.Control.hcNumIn , devAddr );
  USBHaddrHC( USB_Host.Control.hcNumOut, devAddr );
  USB_Host.handleCtrlPkg= handle;
}


/**
  * @brief  USBH_HandleControl
  *         Handles the USB control transfer state machine
  * @retval Status
  */
void USBHxferInControl( short channel )
{ URB_STATE URB_Status= USB_HOST.URB_State[ USB_Host.Control.hcNumIn ];

  switch( USB_Host.Control.state )
  { case CTRL_DATA_IN:
      switch( URB_Status )
      { case URB_STATE_DONE:         /* check is DATA packet transferred successfully */
          USB_HOST.hc[ USB_Host.Control.hcNumOut ].toggleOut ^= 1;
          USBH_CtlSendData( USB_Host.Control.hcNumOut, 0, 0 );
          USB_Host.Control.state= CTRL_STATUS_OUT;
        break;

        case URB_STATE_STALL:         /* manage error cases*/
          USB_Host.Control.state= CTRL_STALLED;
        break;

        case URB_STATE_ERROR:
          USB_Host.Control.state= CTRL_ERROR;   /* Device error */
        break;

        case URB_STATE_NACK:                          /* Not recognized */
          USB_Host.handleCtrlPkg( channel );    /* Next step             */
        break;

        default: break;
      }
    break;

    case CTRL_STATUS_IN:
      if  ( URB_Status == URB_STATE_DONE )           /* Control transfers completed, Exit the State Mech */
      { USB_Host.Control.state= CTRL_IDLE;
        USB_Host.handleCtrlPkg( channel );     /* Next step             */
      break;                                   /* Ready for another one */
      }

      else if ( URB_Status == URB_STATE_STALL ) /* Control transfers completed, Exit the State Mech */
      { USB_Host.Control.state= CTRL_STALLED;
      }

      else if ( URB_Status == URB_STATE_ERROR )
      { USB_Host.Control.state= CTRL_ERROR;
      }

    break;

    default: break;
} }

/**
  * @brief  USBH_HandleControl
  *         Handles the USB control transfer state machine
  * @retval Status
  */
void USBHxferOutControl( short channel )
{ URB_STATE URB_Status= 0;
  USB_Host.Control.status= CTRL_START;

  switch ( USB_Host.Control.state )
  { case CTRL_SETUP: /* send a SETUP packet */
    break;

    case CTRL_SETUP_WAIT:
      URB_Status= USB_HOST.URB_State[ USB_Host.Control.hcNumOut ];

      if ( URB_Status == URB_STATE_DONE )    /* case SETUP packet sent successfully */
      { int direction= ( USB_Host.Control.setup.b.bmRequestType & USB_REQ_DIR_MASK );


        if ( USB_Host.Control.setup.b.wLength.w )   /* check if there is a data stage */  //timeout= DATA_STAGE_TIMEOUT;
        { if ( direction == USB_D2H )               /* Data Direction is IN */
          { USBH_CtlReceiveData( USB_Host.Control.hcNumIn
                               , USB_Host.Control.buff
                               , USB_Host.Control.length );
            USB_Host.Control.state= CTRL_DATA_IN;
          }
          else                                      /* Data Direction is OUT */ /* Start DATA out transfer (only one DATA packet)*/
          { USB_HOST.hc[ USB_Host.Control.hcNumOut ].toggleOut = 1;
            USBH_CtlSendData( USB_Host.Control.hcNumOut
                            , USB_Host.Control.buff
                            , USB_Host.Control.length );
            USB_Host.Control.state= CTRL_DATA_OUT;
        } }

        else //timeout = NODATA_STAGE_TIMEOUT;                                           /* No DATA stage */
        { if ( direction == USB_D2H )                  /* If there is No Data Transfer Stage */
          { USB_HOST.hc[ USB_Host.Control.hcNumOut ].toggleOut ^= 1;
            USBH_CtlSendData( USB_Host.Control.hcNumOut, 0, 0 );
            USB_Host.Control.state = CTRL_STATUS_OUT;
          }
          else
          { USBH_CtlReceiveData( USB_Host.Control.hcNumIn
                               , 0, 0 ); /* Data Direction is OUT */
            USB_Host.Control.state= CTRL_STATUS_IN;
        } }

        USB_Host.Control.timer= usbOTGgetCurrentFrame( 0 ); /* Set the delay timer to enable timeout for data stage completion */
      }

      else if ( URB_Status == URB_STATE_ERROR )
      { USB_Host.Control.state = CTRL_ERROR;
        USB_Host.Control.status= CTRL_XACTERR;
      }
    break;

    case CTRL_DATA_OUT:
      URB_Status = USB_HOST.URB_State[ USB_Host.Control.hcNumOut ];

      if ( URB_Status == URB_STATE_DONE )
      { USBH_CtlReceiveData( USB_Host.Control.hcNumIn
                           , 0, 0 );            /* Data Direction is OUT */
        USB_Host.Control.state= CTRL_STATUS_IN; /* If the Setup Pkt is sent successful, then change the state */
      }

      /* handle error cases */
      else if ( URB_Status == URB_STATE_STALL ) /* In stall case, return to previous machine state*/
      { USB_Host.Control.state= CTRL_STALLED;
      }

      else if ( URB_Status == URB_STATE_NACK ) /* Nack received from device, again */
      { USB_HOST.hc[ USB_Host.Control.hcNumOut ].toggleOut = 1;
        USBH_CtlSendData( USB_Host.Control.hcNumOut
                        , USB_Host.Control.buff
                        , USB_Host.Control.length );
      }

      else if (URB_Status == URB_STATE_ERROR) /* device error */
      { USB_Host.Control.state= CTRL_ERROR;
      }
    break;

    case CTRL_STATUS_OUT: switch( USB_HOST.URB_State[ USB_Host.Control.hcNumOut ] )
    { case URB_STATE_DONE:
        USB_Host.Control.state= CTRL_IDLE;
        USB_Host.handleCtrlPkg( channel );  /* Next step             */
      break;                                /* Ready for another one */

      case URB_STATE_NACK:
        USB_HOST.hc[ USB_Host.Control.hcNumOut ].toggleOut ^= 1;
        USBH_CtlSendData( USB_Host.Control.hcNumOut, 0, 0 );
      break;

      case URB_STATE_ERROR:
        USB_Host.Control.state= CTRL_ERROR;

      case URB_STATE_IDLE:
      case URB_STATE_STALL:
      break;

      default: break;
    }
    break;

/**
 *   After a halt condition is encountered or an error is detected by the
 *     host, a control endpoint is allowed to recover by accepting the next Setup
 *     PID; i.e., recovery actions via some other pipe are not required for control
 *     endpoints. For the Default Control Pipe, a device reset will ultimately be
 *     required to clear the halt or error condition if the next Setup PID is not
 *     accepted.
 */
    case CTRL_ERROR:
      if ( ++ USB_Host.Control.errorcount <= USBH_MAX_ERROR_COUNT )
      { USB_Host.Control.state= CTRL_SETUP; /* Do the transmission again, starting from SETUP Packet */
      }
      else
      { USB_Host.Control.status= CTRL_FAIL;
      }
    break;

    default: break;
} }



void * USBHgetBuffer()
{ return( USB_HOST.rxBuffer );
}

/**
  * @brief  usbHostHandleEnum
  *         This function includes the complete enumeration process
  * @retval schar
  */
short usbHostHandleEnum( byte ep )
{ static byte lBuffer[ 64 ];

  byte enumState;

  switch ( enumState= USB_Host.EnumState )
  { case ENUM_IDLE:                               /* Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize */
      USBH_ParseDevDesc( &USB_Host.deviceProp.Dev_Desc
                       ,  USB_HOST.rxBuffer, 8 ); /* Commands successfully sent and Response Received */

      USB_Host.Control.ep0size= USB_Host.deviceProp.Dev_Desc.bMaxPacketSize;

      USBHpacketHC( USB_Host.Control.hcNumOut /* modify control channels configuration for MaxPacket size */
                 , USB_Host.Control.ep0size );

      USBHpacketHC( USB_Host.Control.hcNumIn
                 , USB_Host.Control.ep0size );

      USBH_Get_DevDesc( 0, USB_DEVICE_DESC_SIZE );
      USB_Host.EnumState= ENUM_GET_FULL_DEV_DESC;
    break;

    case ENUM_GET_FULL_DEV_DESC:                                     /* Get FULL Device Desc  */
      USBH_ParseDevDesc( &USB_Host.deviceProp.Dev_Desc
                       ,  USB_HOST.rxBuffer, USB_DEVICE_DESC_SIZE ); /* Commands successfully sent and Response Received */
      usbHostEvent( USB_HOST_DESC_AVAILABLE, &USB_Host.deviceProp.Dev_Desc );      /* user callback for device descriptor available */

      USBH_SetAddress( USB_Host.deviceProp.devAddr );
      USB_Host.EnumState= ENUM_SET_ADDR;
    break;

    case ENUM_SET_ADDR:       /* set address */
      mDelay( 20 );
      usbHostEvent( USB_HOST_ADDR_ASSIGNED       /* user callback for device address assigned */
                  , &USB_Host.deviceProp.devAddr );

    /* modify control channels to update device address
     */
      USBHaddrHC( USB_Host.Control.hcNumIn,  USB_Host.deviceProp.devAddr );
      USBHaddrHC( USB_Host.Control.hcNumOut, USB_Host.deviceProp.devAddr );

      USBHGetCfgDesc( USB_CONFIGURATION_DESC_SIZE );
      USB_Host.EnumState= ENUM_GET_CFG_DESC;
    break;

    case ENUM_GET_CFG_DESC:       /* get standard configuration descriptor */
      parseCfgDesc( USB_CONFIGURATION_DESC_SIZE  );
      USBHGetCfgDesc( USB_Host.deviceProp.Cfg_Desc.wTotalLength );
      USB_Host.EnumState= ENUM_GET_FULL_CFG_DESC;
    break;

/**  before getting full config descriptor, check if it does not exceed
 * buffer size allocated to config descriptor USBH_MAX_DATA_BUFFER
 * in the usbh_conf.h
 */
    case ENUM_GET_FULL_CFG_DESC:      /* get FULL config descriptor (config, interface, endpoints) */
      parseCfgDesc( USB_Host.deviceProp.Cfg_Desc.wTotalLength  );

      if ( USB_Host.deviceProp.Cfg_Desc.wTotalLength <= USBH_MAX_DATA_BUFFER )
      { usbHostGotConfigurationDescAvailable( &USB_Host.deviceProp.Cfg_Desc /* User callback for configuration descriptors available */
                                            , USB_Host.deviceProp.Itf_Desc
                                            , USB_Host.deviceProp.Ep_Desc );
      }

      if ( USB_Host.deviceProp.Dev_Desc.iManufacturer || 1 )
      { USBH_Get_StringDesc( USB_Host.deviceProp.Dev_Desc.iManufacturer
                           , lBuffer
                           , 0xFF );
        USB_Host.EnumState= ENUM_GET_MFC_STRING_DESC;
        break;
      }
    usbHostEvent( USB_HOST_VENDOR_STR, "N/A" );   // Not available fallout


    case ENUM_GET_MFC_STRING_DESC: /* Check that Manufacturer String is available */
      if ( enumState == ENUM_GET_MFC_STRING_DESC )
      { USBH_ParseStringDesc( USB_HOST.rxBuffer, lBuffer, sizeof( lBuffer ));    /* Commands successfully sent and Response Received  */
        usbHostEvent( USB_HOST_VENDOR_STR, lBuffer ); /* User callback for Manufacturing string */
      }

      if ( USB_Host.deviceProp.Dev_Desc.iProduct )
      { USBH_Get_StringDesc( USB_Host.deviceProp.Dev_Desc.iProduct
                           , lBuffer
                           , 0xFF );
        USB_Host.EnumState= ENUM_GET_PRODUCT_STRING_DESC;
        break;
      }
      usbHostEvent( USB_HOST_PRODUCT_STR, "N/A");  // Not available fallout

    case ENUM_GET_PRODUCT_STRING_DESC: /* Check that Product string is available */
      if ( enumState == ENUM_GET_PRODUCT_STRING_DESC )
      { USBH_ParseStringDesc( USB_HOST.rxBuffer, lBuffer,  sizeof( lBuffer ) );    /* Commands successfully sent and Response Received  */
        usbHostEvent( USB_HOST_PRODUCT_STR, lBuffer ); /* User callback for Product string */
      }

      if ( USB_Host.deviceProp.Dev_Desc.iSerialNumber ) /* Check that Serial number string is available */
      { USBH_Get_StringDesc( USB_Host.deviceProp.Dev_Desc.iSerialNumber
                           , lBuffer
                           , 0xFF );
        USB_Host.EnumState= ENUM_GET_SERIALNUM_STRING_DESC;
        break;
      }
    usbHostEvent( USB_HOST_SERIAL_STR, "N/A" );

    case ENUM_GET_SERIALNUM_STRING_DESC:
      if ( enumState == ENUM_GET_SERIALNUM_STRING_DESC )
      { USBH_ParseStringDesc( USB_HOST.rxBuffer, lBuffer,  sizeof( lBuffer ) );    /* Commands successfully sent and Response Received  */
        usbHostEvent( USB_HOST_SERIAL_STR, lBuffer );
      }

      USBH_SetCfg( USB_Host.deviceProp.Cfg_Desc.bConfigurationValue );
      USB_Host.EnumState = ENUM_SET_CONFIGURATION;
    break;

    case ENUM_SET_CONFIGURATION:  /* set configuration  (default config) */
      USB_Host.EnumState= ENUM_DEV_CONFIGURED;
      USB_Host.handleCtrlPkg= USBHgetHandleAtConnect( &USB_Host.deviceProp );
    break;                                                /* Throw packet */

    default:  break;
  }

  return( 1 );   /* Ask for next packet */
}

/**
  * @brief  gotDevPortEnabled
  *         USB Port Enable function.
  * @param  selected device
  * @retval Status
  */
byte gotDevPortEnabled()
{ mDelay( 50 );
  usbHostEvent( USB_HOST_DEV_ATTACHED, NULL );

  USB_Host.deviceProp.speed= HCDgetCurrentSpeed(); /* Host is Now ready to start the Enumeration */
  usbHostEvent( USB_HOST_SPEED_DET, &USB_Host.deviceProp.speed );

  USB_Host.Control.hcNumIn =
    UHOSTopenChannel( EPDIR_IN  /* Open Control pipes */
                    , EPTYPE_CONTROL, USB_Host.Control.ep0size
                    , USBHxferInControl );

  USB_Host.Control.hcNumOut=
    UHOSTopenChannel( EPDIR_OUT /* Open Control pipes */
                    , EPTYPE_CONTROL, USB_Host.Control.ep0size
                    , USBHxferOutControl );
  USBH_Get_DevDesc( DEFAULT_DEVADDR, 8 );

  return( 0 );
}

/**
  * @brief  USBH_PortDisabled
  *         USB Port Disable function.
  * @param  selected device
  * @retval Status
*/
byte gotDevPortDisabled()
{ return( 0 );
}

void usbHostSetXferDest( schar ( *handleXferPkg )( byte epNum ) )       /* HostLibInitialized */
{
}

/**
 * @brief  usbHostDeInit
 *         Re-Initialize Host
 * @param  None
 * @retval status
 */
short usbHostDeInit( void ) /* Software Init */
{ USB_Host.EnumState       = ENUM_IDLE;
  USB_Host.Control.state   = CTRL_SETUP;
  USB_Host.Control.ep0size = USB_OTG_MAX_EP0_SIZE;
  USB_Host.deviceProp.speed= HPRT0_PRTSPD_FULL_SPEED;
  USB_Host.handleCtrlPkg   = usbHostHandleEnum; /** Reset to First step */

  for ( int i = 0
      ;     i < OTGgetHostChannels()
      ;     i ++ )
  { USBHfreeChannel( i );
    USB_HOST.URB_State[ i ]= URB_STATE_VOID;
  }

/* Make sure the FIFOs are flushed.
 */
  usbOTGflushTxFifo( 0x10 );         /* all Tx FIFOs */
  usbOTGflushRxFifo();

  return( 0 );
}


/**
 * @brief  usbHOSTsetUrbState
 */
byte usbHOSTsetUrbState( byte num, byte state )
{ if ( state & URB_TOGGLE_OUT )
  { USB_HOST.hc[ num ].toggleOut ^= 1;
  }

  if ( state & URB_TOGGLE_IN )
  { USB_HOST.hc[ num ].toggleIn ^= 1;
  }

  state &= 0x0F;  /* Strip toggle flags */

  if ( state )    /* Additional action */
  { switch( USB_HOST.URB_State[ num ]= state )
    { case URB_STATE_NACK: /*  USB_HOST.sched[ num ].action( num ); */  break;
      case URB_STATE_DONE:  USB_HOST.sched[ num ].action( num );        break;
      default: break;
  } }


  return( state );
}

