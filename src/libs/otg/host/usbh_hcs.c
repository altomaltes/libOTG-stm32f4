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
 * @file usbh_hcs.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_core.h"


/**
  * @brief  USBH_GetFreeChannel
  *         Get a free channel number for allocation to a device endpoint
  * @param  None
  * @retval idx: Free Channel number
  */
static short USBH_GetFreeChannel(  )
{ for( byte idx = 0
     ;      idx < OTGgetHostChannels()
     ;      idx ++ )
  { word rot= 1 << idx; /* bitmap selector */

    if (!( USB_HOST.used & rot )) /* Free channel */
   	{ USB_HOST.used |= rot;
   	  return( idx );
  } }

  return(-1 );
}



/**
  * @brief  UHOSTopenChannel
  *         Open a  pipe
  * @param  speed : USB device speed (Full/Low)
  * @param  epType: end point type (Bulk/int/ctl)
  * @param  mps: max pkt size
  * @retval Status
  */
byte UHOSTopenChannel( byte epNum
                     , byte epType
                     , word mps
                     , void (*action)( schar ep ))
{ byte hcNum= USBH_GetFreeChannel( );

  USB_OTG_HC * hc= USB_HOST.hc + hcNum;

  hc->xferBuff= 0;
  hc->xferLen=
  hc->xferCount= 0;
  hc->toggleIn =
  hc->toggleOut= 0;

  byte speed= USB_Host.deviceProp.speed;
  USB_HOST.sched[ hcNum ].action= action;

  switch( speed )
  { case HPRT0_PRTSPD_HIGH_SPEED: epType |= DO_PING_FLAG;   break;
    case HPRT0_PRTSPD_LOW_SPEED : epType |= LOW_SPEED_FLAG; break;
  }

  usbHOSTinitC( hcNum, epNum
           , epType
           , USB_Host.deviceProp.devAddr, mps  );   /* Hardware init */

  return( hcNum );
}

/**
  * @brief  USBHfreeChannel
  *         Free the USB host channel
  * @param  idx: Channel number to be freed
  * @retval Status
  */
byte USBHfreeChannel( byte idx )
{ word rot= 1 << idx;

  USB_HOST.used &= ~rot;
  //usbHOSThaltHC( idx ); !!! NO

  return( 0 );
}


/**
  * @brief  USBHfreeChannel
  *         Free the USB host channel
  * @param  idx: Channel number to be freed
  * @retval Status
  */
void USBHfreeDeviceChannels( byte addr )
{ for( byte idx = 0
     ;      idx < OTGgetHostChannels()
     ;      idx++ )
  { word rot= 1 << idx;

    if ( USB_HOST.used & rot )           /* Used channel */
    { if ( usbHOSTgetHCdad( idx ) == addr )  /* Belongs to this one */
     	{ usbHOSThaltHCnum( idx );
     	  USB_HOST.used &= ~rot;           /* Mark as free */
} } } }

/**
  * @brief  USBHchannelTimer
  *         Free the USB host channel
  * @param  idx: Channel number to be freed
  * @retval Status
  */
schar USBHchannelTimer( byte hcNum, word interval )
{ if ( hcNum < OTGgetHostChannels() )
  { USB_HOST.sched[ hcNum ].reload=
    USB_HOST.sched[ hcNum ].current= interval;   /* Trhow again  */

    return( 0 );
  }

  return( -1 );
}


/**
  * @brief  USBHallocateAddress
  *         Free all USB host channel
  * @retval Status
  */
byte USBHallocAddress()
{ for( byte idx = 1                           /* 0 is the initial broadcast address */
     ;      idx < sizeof( USB_HOST.devAddr )
     ;      idx ++ )
  { if ( !USB_HOST.devAddr[ idx ] )
    { return( idx );
  } }

  return( 0 );
}

/**
  * @brief  USBHdeallocAllChannel
  *         Free all USB host channel
  * @retval Status
  */
byte USBHdeallocAllChannel()
{ USB_HOST.used= 0;

  return( 0 );
}


/**
  * @brief  HCDgetXferCnt
  *
  * @retval No. of data bytes transferred
  */
dword HCDgetXferCnt( byte chNum )
{ return( USB_HOST.hc[ chNum ].xferCount );

//  return( USB_HOST.XferCnt[ ch_num ] );
}

/**
  * @brief  HCDsubmitRequest
  *         This function prepare a HC and start a transfer
  * @param  hcNum: Channel number
  * @retval status
  */
dword HCDsubmitRequest( byte   hcNum
                      , byte   pid
                      , byte * buff
                      , word   len )
{ USB_HOST.URB_State[ hcNum ]= URB_STATE_IDLE;
  USB_HOST.hc[ hcNum ].xferBuff= buff;
  USB_HOST.hc[ hcNum ].xferCount= 0;          /* start accounting */

  return( usbHOSTstartXferHC( hcNum, pid, buff, len ));
}


/**
 * @brief  handleSofFrameISR
 *         Handles the start-of-frame interrupt in host mode.
 *         schedule for channel jobs
 * @param  none
 * @retval status
 */
word handleSofHostISR( word frNum )  /** 0x03 Start of frame */
{ schar hcNum= frNum & 0xF;             /* Spread over 16 values, this chooses channel */

  if ( hcNum < OTGgetHostChannels() )
  { if ( USB_HOST.sched[ hcNum ].reload )           /* Active timer */
    { if ( --USB_HOST.sched[ hcNum ].current <= 0 ) /* time reached */
      { USB_HOST.sched[ hcNum ].action( -hcNum );   /* Fire action  */
        USB_HOST.sched[ hcNum ].current=            /* Trhow again  */
        USB_HOST.sched[ hcNum ].reload;
  } } }

  else if ( hcNum == OTGgetHostChannels() )        /* System schedule, symetric */
  {
  }
  return( 0 );
}



