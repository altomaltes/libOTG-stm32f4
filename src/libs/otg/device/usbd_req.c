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
 * @file usbd_req.c
 *
 * original draft from MCD Application Team
 */

#include "usbd_core.h"

ALIGN_THIS( dword USBD_ep_status   ) = 0;
ALIGN_THIS( dword USBD_default_cfg ) = 0;
ALIGN_THIS( dword USBD_cfg_status  ) = 0;

/**
  * @brief  USBD_GetLen
  *         return the string length
  * @param  buf : pointer to the ascii string buffer
  * @retval string length
  */

static byte USBD_GetLen( const char * buf )
{ byte  len = 0;

  while ( *buf  )
  { len++; buf++;
  }

  return len;
}

/**
 * @brief  USBDstdItfReq
 *         Handle standard usb interface requests
 *
 * @param  req: usb request
 * @retval status
 */
schar  USBDstdItfReq( USB_SETUP_REQ * req )
{ switch ( USB_DEV.deviceStatus )
  { case USB_OTG_CONFIGURED:

      if ( LOBYTE( req->wIndex ) <=  1 )           // çççç USBD_ITF_MAX_NUM )
      { USBdeviceDesc.driver->Setup( req );

        if (( req->wLength == 0 ) /*&& (ret == USBD_OK) */ )
        { USBDctlSendStatus();
      } }
      else
      { USBDctlError( req );
      }
      break;

    default: USBDctlError( req );  break;
  }
  return( 0 );
}

/**
 * @brief  USBDstdEPReq
 *         Handle standard usb endpoint requests
 *
 * @param  req: usb request
 * @retval status
 */
schar  USBDstdEPReq( USB_SETUP_REQ * req )
{ byte   ep_addr;
  //schar ret = 0;

  ep_addr= LOBYTE( req->wIndex );

  if ((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_CLASS) /* Check the class specific requests before going to standard request */
  { USBdeviceDesc.driver->Setup( req );
    return( 0 );
  }

  switch ( req->bRequest )
  { case USB_REQ_SET_FEATURE :
      switch (USB_DEV.deviceStatus)
      { case USB_OTG_ADDRESSED:
          if (( ep_addr != 0x00 )
           && ( ep_addr != 0x80 ))
          { USBDepStall( ep_addr );
      }
      break;

      case USB_OTG_CONFIGURED:
        if (req->wValue == USB_FEATURE_EP_HALT)
        { if ((ep_addr != 0x00)
           && (ep_addr != 0x80))
          { USBDepStall(  ep_addr);
        } }
        USBdeviceDesc.driver->Setup( req );
        USBDctlSendStatus();
      break;

      default: USBDctlError( req);          break;
    }
    break;

    case USB_REQ_CLEAR_FEATURE: switch (USB_DEV.deviceStatus)
    { case USB_OTG_ADDRESSED:
        if ((ep_addr != 0x00)
         && (ep_addr != 0x80))
        { USBDepStall( ep_addr );
        }
        break;

        case USB_OTG_CONFIGURED:
          if (req->wValue == USB_FEATURE_EP_HALT)
          { if ((ep_addr != 0x00)
             && (ep_addr != 0x80))
            { USBDepClrStall(  ep_addr );
              USBdeviceDesc.driver->Setup( req );
            }
            USBDctlSendStatus();
          }
        break;

        default: USBDctlError( req);     break;
      }
      break;

    case USB_REQ_GET_STATUS:
      switch ( USB_DEV.deviceStatus )
      { case USB_OTG_ADDRESSED:
          if ((ep_addr != 0x00)
           && (ep_addr != 0x80))
          { USBDepStall( ep_addr );
          }
          break;

        case USB_OTG_CONFIGURED:
          if ((ep_addr & 0x80)== 0x80)
          { if ( USB_DEV.inEp[ ep_addr & 0x7F ].is_stall)
            { USBD_ep_status = 0x0001;
            }
            else
            { USBD_ep_status = 0x0000;
          } }

          else if ((ep_addr & 0x80)== 0x00)
          { if ( USB_DEV.outEp[ ep_addr ].is_stall )
            { USBD_ep_status = 0x0001;
            }

            else
            { USBD_ep_status = 0x0000;
          } }

          else
          { /* Do Nothing */
          }

          USBDctlSendData( (byte *)&USBD_ep_status, 2 );
        break;

        default: USBDctlError( req);        break;
      }
    break;

    default: break;
  }

  return( 0 );
}

extern byte USBD_DeviceQualifierDesc[];

/**
 * @brief  usbDEVgetDescriptor
 *         Handle Get Descriptor requests
 *
 * @param  req: usb request
 * @retval status
 */
static void usbDEVgetDescriptor( USB_SETUP_REQ *req )
{ word len;
  byte * pbuf;
  len = req->wLength ;

  switch (req->wValue >> 8)
  {

#if ( USBD_LPM_ENABLED == 1 )
    case USB_DESC_TYPE_BOS:
      pbuf= pdev->pDesc->GetBOSDescriptor( pdev->dev_speed, &len );
    break;
#endif

    case DTYPE_CONFIGURATION:
      pbuf= USBdeviceDesc.driver->configDescriptor;
      len= pbuf[ 2 ];
    break;

    case DTYPE_DEVICE:
      pbuf= USBdeviceDesc.deviceDescriptor; len= *pbuf;
    break;

    case DTYPE_STRING:
      pbuf= USBDgetString( USBdeviceDesc.strings[ req->wValue & 0xFF ] );
      len= *pbuf;
    break;

    case DTYPE_QUALIFIER:
   //   if ( HCDgetCurrentSpeed() != HPRT0_PRTSPD_HIGH_SPEED )
      { USBDctlError( req);
        return;
      }

      pbuf= &USBD_DeviceQualifierDesc;
      len= *pbuf;
    break;

    case DTYPE_OTHER: // Speed config
     // if ( HCDgetCurrentSpeed() != HPRT0_PRTSPD_HIGH_SPEED )
      { USBDctlError( req );
        return;
      }

      pbuf= &USBD_DeviceQualifierDesc;
      len= *pbuf;
    break;

    default:
      USBDctlError( req);
    return;
  }

  if (( len ) && (req->wLength ))
  { len= MIN( len, req->wLength );

    USBDctlSendData( pbuf, len );
} }

/**
 * @brief  USBD_SetAddress
 *         Set device address
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetAddress( USB_SETUP_REQ * req )
{ byte dev_addr;

  if ((req->wIndex == 0) && (req->wLength == 0))
  { dev_addr= (byte)(req->wValue) & 0x7F;

    if ( USB_DEV.deviceStatus == USB_OTG_CONFIGURED )
    { USBDctlError( req );
    }
    else
    { USBDepSetAddress( dev_addr );
      USBDctlSendStatus();

      if (dev_addr != 0)
      { USB_DEV.deviceStatus= USB_OTG_ADDRESSED;
      }
      else
      { USB_DEV.deviceStatus= USB_OTG_DEFAULT;
  } } }

  else
  { USBDctlError( req );
} }

/**
 * @brief  USBD_SetConfig
 *         Handle Set device configuration request
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetConfig( USB_SETUP_REQ * req )
{ static byte  cfgidx;

  cfgidx = (byte)(req->wValue);

  if ( cfgidx > 1 )  // çççç USBD_CFG_MAX_NUM )
  { USBDctlError( req);
  }
  else
  { switch( USB_DEV.deviceStatus )
    { case USB_OTG_ADDRESSED:
        if ( cfgidx )
        { USB_DEV.deviceConfig = cfgidx;
          USB_DEV.deviceStatus = USB_OTG_CONFIGURED;
          USBD_SetCfg( cfgidx);
          USBDctlSendStatus();
        }
        else
        { USBDctlSendStatus();
        }
        break;

      case USB_OTG_CONFIGURED:
        if ( cfgidx == 0 )
        { USB_DEV.deviceStatus= USB_OTG_ADDRESSED;
          USB_DEV.deviceConfig= cfgidx;
          USBDclrCfg( cfgidx);
          USBDctlSendStatus();
        }
        else if (cfgidx != USB_DEV.deviceConfig )
        { USBDclrCfg( USB_DEV.deviceConfig);     /* Host Task handler */

          USB_DEV.deviceConfig = cfgidx; /* set new configuration */
          USBD_SetCfg( cfgidx);
          USBDctlSendStatus();
        }
        else
        { USBDctlSendStatus();
        }
      break;

      default:
        USBDctlError( req);
      break;
} } }

/**
 * @brief  USBD_GetConfig
 *         Handle Get device configuration request
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_GetConfig( USB_SETUP_REQ *req )
{ if ( req->wLength != 1 )
  { USBDctlError( req);
  }
  else
  { switch (USB_DEV.deviceStatus )
    { case USB_OTG_ADDRESSED:  USBDctlSendData((byte *)&USBD_default_cfg, 1 ); break;
      case USB_OTG_CONFIGURED: USBDctlSendData(&USB_DEV.deviceConfig,     1 ); break;
      default:                 USBDctlError   ( req );                         break;
} } }

/**
 * @brief  USBD_GetStatus
 *         Handle Get Status request
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_GetStatus( USB_SETUP_REQ *req )
{ switch ( USB_DEV.deviceStatus )
  { case USB_OTG_ADDRESSED:
    case USB_OTG_CONFIGURED:

#ifdef USBD_SELF_POWERED
      USBD_cfg_status = USB_CONFIG_SELF_POWERED;
#else
      USBD_cfg_status = 0x00;
#endif

      if ( USB_DEV.devRemoteWakeup )
      { USBD_cfg_status |= USB_CONFIG_REMOTE_WAKEUP;
      }

      USBDctlSendData ((byte *)&USBD_cfg_status, 2 );
      break;

    default: USBDctlError( req );  break;
} }


/**
 * @brief  USBD_SetFeature
 *         Handle Set device feature request
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetFeature( USB_SETUP_REQ * req )
{ if ( req->wValue == USB_FEATURE_REMOTE_WAKEUP )
  { USB_DEV.devRemoteWakeup= 1;
    USBdeviceDesc.driver->Setup( req );
    USBDctlSendStatus();
  }

  else if (( req->wValue == USB_FEATURE_TEST_MODE )
       && (( req->wIndex & 0xFF ) == 0))
  { USBDepSetTestMode( req->wIndex >> 8 );
    USB_DEV.testMode = 1;
    USBDctlSendStatus();
  }
  else
  { /* Do Nothing */
} }


/**
 * @brief  USBD_ClrFeature
 *         Handle clear device feature request
 *
 * @param  req: usb request
 * @retval status
 */
static void USBD_ClrFeature( USB_SETUP_REQ *req )
{ switch (USB_DEV.deviceStatus)
  { case USB_OTG_ADDRESSED:
    case USB_OTG_CONFIGURED:
      if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
      { USB_DEV.devRemoteWakeup= 0;
        USBdeviceDesc.driver->Setup( req );
        USBDctlSendStatus();
      }
      break;

    default: USBDctlError( req ); break;
} }

/**
 * @brief  USBDparseSetupRequest
 *         Copy buffer into setup structure
 *
 * @param  req: usb request
 * @retval None
 */

void USBDparseSetupRequest( USB_SETUP_REQ * req )
{ req->bmRequest= *(byte *)( USB_DEV.setupPacket + 0 );
  req->bRequest = *(byte *)( USB_DEV.setupPacket + 1 );
  req->wValue   = SWAPBYTE ( USB_DEV.setupPacket + 2 );
  req->wIndex   = SWAPBYTE ( USB_DEV.setupPacket + 4 );
  req->wLength  = SWAPBYTE ( USB_DEV.setupPacket + 6 );

  USB_DEV.inEp[0].ctl_data_len = req->wLength  ;
  USB_DEV.deviceState= USB_OTG_EP0_SETUP;
}

/**
 * @brief  USBDctlError
 *         Handle USB low level Error
 *
 * @param  req: usb request
 * @retval None
 */
void USBDctlError( USB_SETUP_REQ * req )
{ USBDepStall( 0x80 );
  USBDepStall( 0x00 );
  USBDep0OutStart();
}


/**
  * @brief  USBD_GetString
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted str Check the class specific requests before going to standard request  (unicode)
  * @param  len : descriptor length
  * @retval None
  */
byte * USBDgetString( const char * desc )
{ static byte strDesc[ 64 ];  // ççççUSB_MAX_STR_DESC_SIZ  ];
  byte * unicode= strDesc;
  byte idx = 0;

  unicode[ idx++ ]= USBD_GetLen( desc ) * 2 + 2;
  unicode[ idx++ ]= DTYPE_STRING;

  while (* desc )
  { unicode[ idx++ ] = *desc++;
    unicode[ idx++ ] =  0x00;
  }

  return( strDesc );
}

/**
 * @brief  USBDstdDevReq
 *         Handle standard usb device requests
 *
 * @param  req: usb request
 * @retval status
 */
schar  USBDstdDevReq( USB_SETUP_REQ  * req )
{ switch ( req->bRequest )
  { case USB_REQ_GET_STATUS:        USBD_GetStatus (     req ); break;
    case USB_REQ_GET_DESCRIPTOR:    usbDEVgetDescriptor( req ); break;
    case USB_REQ_SET_ADDRESS:       USBD_SetAddress(     req ); break;
    case USB_REQ_SET_CONFIGURATION: USBD_SetConfig (     req ); break;
    case USB_REQ_GET_CONFIGURATION: USBD_GetConfig (     req ); break;
    case USB_REQ_SET_FEATURE:       USBD_SetFeature(     req ); break;
    case USB_REQ_CLEAR_FEATURE:     USBD_ClrFeature(     req ); break;
    default:                        USBDctlError(        req ); break;
  }

  return( 0 );
}
