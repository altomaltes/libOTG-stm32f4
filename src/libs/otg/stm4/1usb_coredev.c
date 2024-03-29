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
 * @file usb_core.c
 *
 * original draft from MCD Application Team
 */

#include "usb_dcd.h"


/**
 * @brief  usbDEVinitSpeed :Initializes the DevSpd field of DCFG register
 *         depending the PHY type and the enumeration speed of the device.
 * @retval : None
 */
void usbDEVinitSpeed( byte speed )
{ STM32F4.USB.DEVICE.DCFG.DSPD= speed;
}

/**
 * @brief  USBDcoreInit : Initializes the USB_OTG controller registers
 *         for device mode
 * @retval schar : status
 */
schar USBDcoreInit()
{ schar top;

  usbDEVdeInit();

  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0; /* Disasble interrupts */
  DCD_DevDisconnect();
  STM32F4.USB.DEVICE.DCTL.SDIS= 1;    /** 0x01 Soft disconnect. This places the DEVICE off */

  struct STM32_USB_GLOBAL$FIFOMEM size;
  size.TXSA= 0;                                          /* Reset the fifo mem giver*/

  STM32F4.USB.PWRCLK.FS_PCGCCTL.atomic= 0;               /* Restart the Phy Clock */
  STM32F4.USB.DEVICE.DCFG.PFIVL= DCFG_FRAME_INTERVAL_80; /* Device configuration register */

  if      ((dword)&STM32F4.USB == 0x50000000 )           /* Full spped */
  { usbDEVinitSpeed( USB_OTG_SPEED_PARAM_FULL );        /* Set Full speed phy */
    size.TXFD= RX_FIFO_FS_SIZE; top= 3;
  }

  else if ((dword)&STM32F4.USB == 0x40040000 )           /* High spped */
  { usbDEVinitSpeed( USB_OTG_SPEED_PARAM_HIGH );        /* Set High speed phy */
    size.TXFD= RX_FIFO_HS_SIZE; top= 5;
  }

  else
  { return( -1 );
  }

  STM32F4.USB.GLOBAL.GRXFSIZ.RXFD= size.TXFD; size.TXSA += size.TXFD; /* Set the RX fifo size, point to free  */
  size.TXFD >>= 1;

  STM32F4.USB.GLOBAL.GNPTXFSIZ=     size; size.TXSA += size.TXFD;  /* Non periodic transmit */
  STM32F4.USB.GLOBAL.HPTXFSIZ[ 0 ]= size; size.TXSA += size.TXFD;  /* Periodic xmit         */
  size.TXFD <<= 1;

  for( int loop= 0
     ;     loop  < top
     ;     loop ++ )
  { STM32F4.USB.GLOBAL.DIEPTXF[ loop ]= size; size.TXSA += size.TXFD;             /* Next free space */
  }


/* Flush the FIFOs
 */
  usbOTGflushTxFifo( 0x10 );  /* all Tx FIFOs */
  usbOTGflushRxFifo(      );

/* Clear all pending Device Interrupts
 */
  STM32F4.USB.DEVICE.DIEPMSK .atomic=
  STM32F4.USB.DEVICE.DOEPMSK .atomic=
  STM32F4.USB.DEVICE.DAINTMSK.atomic= 0x00000000;
  STM32F4.USB.DEVICE.DAINT   .atomic= 0xFFFFFFFF;

  union STM32_USB_DEVICE$DOIEPCTL CTL;

  for ( top = 0
      ; top < USB_OTG_Core.devEndpoints
      ; top++ )
  { CTL= STM32F4.USB.DEVICE.DIEP[ top ].CTL; CTL.atomic = 0;

    if ( CTL.EPENA  )
    { CTL.EPDIS= 1;
      CTL.SNAK = 1;
    }

    STM32F4.USB.DEVICE.DIEP[ top ].CTL= CTL;
    STM32F4.USB.DEVICE.DIEP[ top ].INT.atomic=  0xFF;
    STM32F4.USB.DEVICE.DIEP[ top ].TSIZ.atomic= 0x00;

    CTL= STM32F4.USB.DEVICE.DOEP[ top ].CTL; CTL.atomic = 0;  // DIEP ����

    if ( CTL.EPENA  )
    { CTL.EPDIS= 1;
      CTL.SNAK = 1;
    }

    STM32F4.USB.DEVICE.DOEP[ top ].CTL= CTL;
    STM32F4.USB.DEVICE.DOEP[ top ].INT.atomic=  0xFF;
    STM32F4.USB.DEVICE.DOEP[ top ].TSIZ.atomic= 0x00;
  }

  STM32F4.USB.DEVICE.DIEPMSK.TXFIFOUNM_NW= 1;

  if ( USB_OTG_Core.dmaEnable )
  { STM32F4.USB.DEVICE.DTHRCTL_NW.NISOEN_NW= 1;
    STM32F4.USB.DEVICE.DTHRCTL_NW.ISOEN_NW=  1;
    STM32F4.USB.DEVICE.DTHRCTL_NW.TXLEN_NW= 64;
    STM32F4.USB.DEVICE.DTHRCTL_NW.RXEN_NW=   1;
    STM32F4.USB.DEVICE.DTHRCTL_NW.RXLEN_NW= 64;
  }

  usbOTGenableCommonInt( DEVICE_MODE );  /* Enable the common interrupts */
  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 1; /* Enable interrupts ( global ) */
  DCD_DevConnect(); /** 0x01 Soft disconnect. This tell the host device ON */

  return( 0 );
}

/**
 * @brief  usbDEVgetDeviceSpeed
 *         Get the device speed from the device status register
 * @param  None
 * @retval status
 */
enum USB_OTG_SPEED usbDEVgetDeviceSpeed ()
{ switch ( STM32F4.USB.DEVICE.DSTS.ENUMSPD )
  { case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ: return( USB_SPEED_HIGH        );
    case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
    case DSTS_ENUMSPD_FS_PHY_48MHZ:          return( USB_SPEED_FULL );
    case DSTS_ENUMSPD_LS_PHY_6MHZ:           return( USB_SPEED_LOW );
  }

  return( USB_SPEED_FULL );
}

/**
 * @brief  enables EP0 OUT to receive SETUP packets and configures EP0
 *   for transmitting packets
 * @param  None
 * @retval schar : status
 */
schar USBDep0Activate( )
{ switch ( STM32F4.USB.DEVICE.DSTS.ENUMSPD )   /* Set the MPS of the IN EP based on the enumeration speed */
  { case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
    case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
    case DSTS_ENUMSPD_FS_PHY_48MHZ:
      STM32F4.USB.DEVICE.DIEP[ 0 ].CTL.MPSIZ= DEP0CTL_MPS_64;
    break;

    case DSTS_ENUMSPD_LS_PHY_6MHZ:
      STM32F4.USB.DEVICE.DIEP[ 0 ].CTL.MPSIZ= DEP0CTL_MPS_8;
    break;
  }

  STM32F4.USB.DEVICE.DCTL.CGINAK = 1;

  return( 0 );
}

/**
 * @brief  USBDepActivate : Activates an EP
 * @retval schar : status
 */
schar USBDepActivate( byte epAddr, byte epType, word maxPacket )
{ volatile union STM32_USB_DEVICE$DAINTMSK * daintmsk= &STM32F4.USB.DEVICE.DAINTMSK;

  byte isIN=  epAddr & 0x80; epAddr &= 0x7F;                       // is IN
  byte epNum= isIN ? epAddr : epAddr + 16; // EPNUM1 before, move to given index
//  int epNum= EPNUM1( ep );

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
  if (( epAddr == 1 ) && ( (dword)&STM32F4.USB == 0x40040000 )) // High speed
  { daintmsk= &STM32F4.USB.DEVICE.DEACHMSK_NW;
  }
#endif


/* Read DEPCTLn register
 * If the EP is already active don't change the EP Control
 * register.
 */
  if ( ! STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.USBAEP )
  { STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.MPSIZ = maxPacket;
    STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.EPTYP = epType;
    STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.TXFNUM= epAddr & 0x7F;        // ep- > tx_fifo_num;
    STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.SD0PID_SEVNFRM = 1;
    STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.USBAEP= 1;
  }

/* Enable the Interrupt for this EP
 */
  if ( isIN ) { daintmsk->IEPINT |= 1 << epAddr; }
        else  { daintmsk->OEPINT |= 1 << epAddr; }

  return( 0 );
}

/**
 * @brief  USBDepDeactivate : Deactivates an EP
 * @retval schar : status
 */
schar USBDepDeactivate( byte epAddr )
{ volatile union STM32_USB_DEVICE$DAINTMSK * daintmsk= &STM32F4.USB.DEVICE.DAINTMSK;

  byte isIN=  epAddr & 0x80; epAddr &= 0x7F;                       // is IN
  byte epNum= isIN ? epAddr : epAddr + 16; // EPNUM1 before, move to given index

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
  if (( epAddr == 1 ) &&  ( (dword)&STM32F4.USB == 0x40040000 ))
  { daintmsk= &STM32F4.USB.DEVICE.DEACHMSK_NW;
  }
#endif

/* Read DEPCTLn register
 */
  if ( isIN )    // Is IN ??
  { STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.USBAEP= 0;
    daintmsk->IEPINT &= ~( 1 << epAddr );            /* Disable the Interrupt for this EP */
  }
  else
  { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.USBAEP= 0;
    daintmsk->OEPINT &= ~( 1 << epAddr );            /* Disable the Interrupt for this EP */
  }

  return( 0 );
}

/**
 * @brief  USBDepStartXfer : Handle the setup for data xfer for an EP and
 *         starts the xfer
 * @retval schar : status
 */
schar USBDepStartXmit( byte   epAddr
                     , void * xferBuff
                     , word   xferlenEP )
{ epAddr &= 0x7F;                       // is IN, strip type info

  if  ( STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.XFRSIZ )
  { while(1);
  }

  word maxpacket= STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.MPSIZ;
  byte    epType= STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.EPTYP;

  if ( xferlenEP == 0 )               /* Zero Length Packet? */
  { STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.XFRSIZ= 0;
    STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.PKTCNT= 1;
  }

/* Program the transfer size and packet count
 * as follows: xfersize = N * maxpacket + short_packet pktcnt = N + (short_packet exist ? 1 : 0)
 */
  else
  { STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.XFRSIZ=  xferlenEP;
    STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.PKTCNT= (xferlenEP - 1 + maxpacket) / maxpacket;

    if ( epType == USB_EP_TYPE_ISOC )
    { STM32F4.USB.DEVICE.DIEP[ epAddr ].TSIZ.MULTICNT= 1;
  } }

  if ( USB_OTG_Core.dmaEnable == 1)
  { //STM32F4.USB.DEVICE.DIEP[ epAddr ].DMA= ep ->dmaAddr; ��
  }
  else
  { if ( epType != USB_EP_TYPE_ISOC )
    { if ( xferlenEP > 0 ) /* Enable the Tx FIFO Empty Interrupt for this EP */
      { STM32F4.USB.DEVICE.DIEPEMPMSK.INEPTXFEM |= 1 << epAddr;
  } } }


  if ( epType == USB_EP_TYPE_ISOC )
  { if ((( STM32F4.USB.DEVICE.DSTS.FNSOF ) & 0x1) == 0 )
    { STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.SD1PID_SODDFRM= 1;
    }
    else
    { STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.SD0PID_SEVNFRM= 1;
  } }


/* EP enable, IN data in FIFO
 */
  STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.CNAK =
  STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.EPENA= 1;

  if ( epType == USB_EP_TYPE_ISOC )
  { OTGwritePacket( xferBuff
                  , epAddr
                  , xferlenEP );
  }

  return( 0 );
}

/* Program the transfer size and packet count as follows:
 * pktcnt = N
 * xfersize = N * maxpacket
 */
schar USBDepStartXrecv( byte epNum, word xferlen, word maxPacket )
{ byte epType= STM32F4.USB.DEVICE.DIEP[ epNum & 0xFF ].CTL.EPTYP;

  if ( xferlen )
  { STM32F4.USB.DEVICE.DOEP[ epNum ].TSIZ.PKTCNT= ( xferlen + ( maxPacket - 1)) / maxPacket;
    STM32F4.USB.DEVICE.DOEP[ epNum ].TSIZ.XFRSIZ= STM32F4.USB.DEVICE.DOEP[ epNum ].TSIZ.PKTCNT * maxPacket;
  }
  else
  { STM32F4.USB.DEVICE.DOEP[ epNum ].TSIZ.PKTCNT= 1;
    STM32F4.USB.DEVICE.DOEP[ epNum ].TSIZ.XFRSIZ= maxPacket;
  }

//  if ( USB_OTG_Core.dmaEnable == 1 )
//  { STM32F4.USB.DEVICE.DOEP[ ep -> num ].DMA= ep ->dmaAddr;
// �  }

    if ( epType == USB_EP_TYPE_ISOC )
    { if ( /* �� ep -> even_odd_frame */ 0 )
      { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.SD1PID_SODDFRM= 1;
      }
      else
      { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.SD0PID_SEVNFRM = 1;
    } }

/* EP enable
 */
  STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.CNAK = 1;
  STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.EPENA= 1;

  return( 0 );
}

/**
 * @brief  USB_OTG_EP0StartXfer : Handle the setup for a data xfer for EP0 and
 *         starts the xfer
 * @retval schar : status ep -> is_in == 1
 */
schar USBDep0StartXmit( word xferlenEP )
{ if ( xferlenEP == 0 )     /* Zero Length Packet? */
  { STM32F4.USB.DEVICE.DIEP[ 0 ].TSIZ.XFRSIZ= 0;
    STM32F4.USB.DEVICE.DIEP[ 0 ].TSIZ.PKTCNT= 1;
  }
  else
  { STM32F4.USB.DEVICE.DIEP[ 0 ].TSIZ.XFRSIZ= xferlenEP;  // xferlen yet crippled
    STM32F4.USB.DEVICE.DIEP[ 0 ].TSIZ.PKTCNT= 1;
  }

  if ( USB_OTG_Core.dmaEnable == 1 )
  {//  STM32F4.USB.DEVICE.DIEP[ 0 ].DMA=  ep -> dmaAddr; ��
  }

/* EP enable, IN data in FIFO
 */
  STM32F4.USB.DEVICE.DIEP[ 0 ].CTL.CNAK = 1;
  STM32F4.USB.DEVICE.DIEP[ 0 ].CTL.EPENA= 1;

  if ( USB_OTG_Core.dmaEnable == 0 )
  { if ( xferlenEP > 0 ) /* Enable the Tx FIFO Empty Interrupt for this EP */
    { STM32F4.USB.DEVICE.DIEPEMPMSK.INEPTXFEM |= 1 << 0;  // ep -> num;
  } }

  return( 0 );
}

/* Program the transfer size and packet count as follows:
 * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
 * pktcnt = N
 */

schar USBDep0StartRecv( word maxpacket ) //  else /* OUT endpoint */
{ //if ( ep -> xfer Len  )  { ep -> xferlen= ep -> maxpacket;

  STM32F4.USB.DEVICE.DOEP[ 0 ].TSIZ.XFRSIZ= maxpacket;
  STM32F4.USB.DEVICE.DOEP[ 0 ].TSIZ.PKTCNT= 1;

//  if ( USB_OTG_Core.dmaEnable == 1 )
//  { STM32F4.USB.DEVICE.DOEP[ ep -> num ].DMA= ep -> dmaAddr;
// �  }

  /* EP enable */
  STM32F4.USB.DEVICE.DOEP[ 0 ].CTL.CNAK =
  STM32F4.USB.DEVICE.DOEP[ 0 ].CTL.EPENA= 1;

  return( 0 );
}

/**
 * @brief  USBDsetStall : Set the EP STALL
 * @retval schar : status
 */
schar USBDsetStall( byte epAddr )
{ byte epNum= epAddr & 0x7F;               /* Extract the addr part */

  if ( epAddr & 0x80 )  // IS IN
  { if ( STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.EPENA ) /* set the disable and stall bits */
    { STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.EPDIS= 1;
    }
    STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.STALL= 1;
  }
  else
  { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.STALL= 1;
  }

  return( 0 );
}

/**
 * @brief  Clear the EP STALL
 * @retval schar : status
 */
schar USBDepClearStall( byte epAddr )
{ int epNum= epAddr & 0x7F;

//  if ( ep -> is_in != 1 )
//  { ep += EPOUT_OFFSET;   /* Point to out endpoints */
//  }

  STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.STALL = 0;  /* clear the stall bits */


//  if ( ep -> type == EP_TYPE_INTR
//  ||   ep -> type == EPTYPE_BULK )
//  { STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.SD0PID_SEVNFRM = 1; /* DATA0 */
 // }

  return( 0 );
}

/**
 * @brief  usbDEVreadOutEPitr : returns Device OUT EP Interrupt register
 * @param  ep : end point number
 * @retval Device OUT EP Interrupt register
 */
dword usbDEVreadOutEPitr( byte epnum )
{ return( STM32F4.USB.DEVICE.DOEP[ epnum ].INT.atomic
        & STM32F4.USB.DEVICE.DOEPMSK.atomic );
}

/**
 * @brief  configures EPO to receive SETUP packets
 * @param  None
 * @retval : None
 */
void USBDep0OutStart(  )
{ STM32F4.USB.DEVICE.DOEP[ 0 ].TSIZ.STUPCNT= 3;
  STM32F4.USB.DEVICE.DOEP[ 0 ].TSIZ.PKTCNT = 1;
  STM32F4.USB.DEVICE.DOEP[ 0 ].TSIZ.XFRSIZ = 8 * 3;

  if (USB_OTG_Core.dmaEnable == 1 )
  { // STM32F4.USB.DEVICE.DOEP[ 0 ].DMA= (dword)&USB_DEV.setupPacket; ��

    STM32F4.USB.DEVICE.DOEP[ 0 ].CTL.EPENA = 1; /* EP enable */
    STM32F4.USB.DEVICE.DOEP[ 0 ].CTL.USBAEP= 1;  // 0x80008000

    volatile int addr= STM32F4.USB.DEVICE.DOEP[ 0 ].CTL.atomic;
    addr++;
} }

/**
 * @brief  USBDEVungateClock : active USB Core clock
 * @param  None
 * @retval : None
 */
//  if  ( STM32F4.USB.DEVICE.DSTS.SUSPSTS == 1)) /*  switch-off the clocks */
//  { STM32F4.USB.PWRCLK.FS_PCGCCTL.STPPCLK = 1;
//    STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK = 1;
//    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);  /* Request to enter Sleep mode after exit from current ISR */
 // }

void USBDEVungateClock(  )
{ if ( STM32F4.USB.DEVICE.DSTS.SUSPSTS == 1 ) /* un-gate USB Core clock */
  { STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK = 0;
    STM32F4.USB.PWRCLK.FS_PCGCCTL.STPPCLK = 0;
} }


void USBlowPower( int act )  /* un-gate USB Core clock */
{ if ( act )
  { STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK= 0;
    STM32F4.USB.PWRCLK.FS_PCGCCTL.STPPCLK = 0;
  }
  else
  {}


}


/**
* @brief  USB_OTG_RemoteWakeup : active remote wakeup signalling
* @param  None
* @retval : None
*/
void usbDEVactiveRemoteWakeup(  )
{ // if ( USB_DEV.DevRemoteWakeup )   ��
  { if ( STM32F4.USB.DEVICE.DSTS.SUSPSTS == 1 )
    { STM32F4.USB.DEVICE.DCTL.RWUSIG= 1; /* active Remote wakeup signaling */
      mDelay( 5 );
      STM32F4.USB.DEVICE.DCTL.RWUSIG= 0;
} } }

/**
 * @brief  Stop the device and clean up fifo's
 * @param  None
 * @retval : None
 */
void USBDstopDevice(  )
{ for ( dword i = 0
      ;       i < USB_OTG_Core.devEndpoints
      ;       i ++ )
  { STM32F4.USB.DEVICE.DIEP[ 0 ].INT.atomic= 0xFF;
    STM32F4.USB.DEVICE.DOEP[ 0 ].INT.atomic= 0xFF;
  }

  STM32F4.USB.DEVICE.DIEPMSK .atomic= 0x00000000;
  STM32F4.USB.DEVICE.DOEPMSK .atomic= 0x00000000;
  STM32F4.USB.DEVICE.DAINTMSK.atomic= 0x00000000;
  STM32F4.USB.DEVICE.DAINT.atomic=    0xFFFFFFFF;

/* Flush the FIFO
 */
  usbOTGflushRxFifo(      );
  usbOTGflushTxFifo( 0x10 );
}

/**
 * @brief  returns the EP Status
 *         ep : endpoint structure
 * @retval : EP status
 */
dword usbDEVgetEPStatus( byte epAddr )
{ byte isIN= epAddr & 0x80; epAddr &= 0x7F;                       // is IN

  if ( isIN )
  { if ( STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.STALL  == 1 ) { return( USB_OTG_EP_TX_STALL );}
    if ( STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.NAKSTS == 1 ) { return( USB_OTG_EP_TX_NAK   );}
                                                             { return( USB_OTG_EP_TX_VALID );}
  }
  else
  { if ( STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.STALL  == 1 ) { return( USB_OTG_EP_RX_STALL );}
    if ( STM32F4.USB.DEVICE.DIEP[ epAddr ].CTL.NAKSTS == 1 ) { return( USB_OTG_EP_RX_NAK   );}
                                                             { return( USB_OTG_EP_RX_VALID );}
} }

/**
 * @brief  Set the EP Status
 *         Status : new Status
 *         ep : EP structure
 * @retval : None
 */
void usbDEVsetEPStatus( byte epAddr
                        , dword Status )
{ byte isIN=  epAddr & 0x80;
  byte epNum= epAddr & 0x7F;

  if ( isIN )                   /* Process for IN endpoint */
  { if ( Status == USB_OTG_EP_TX_STALL )
    { USBDsetStall( epAddr );   // JACS
      return;
    }

    else if ( Status == USB_OTG_EP_TX_NAK )
      STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.SNAK = 1;

    else if ( Status == USB_OTG_EP_TX_VALID )
    { if ( STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.STALL == 1 )
      { /// �� ep1->even_odd_frame= 0;
        USBDepClearStall( epAddr );
        return;
      }

      STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.CNAK  = 1;
      STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.USBAEP= 1;
      STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.EPENA = 1;
    }

    else if ( Status == USB_OTG_EP_TX_DIS )
    { STM32F4.USB.DEVICE.DIEP[ epNum ].CTL.USBAEP = 0;
  } }

  else /* Process for OUT endpoint */
  { if ( Status == USB_OTG_EP_RX_STALL )
    { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.STALL = 1;
    }
    else if ( Status == USB_OTG_EP_RX_NAK )
    { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.SNAK = 1;
    }

    else if ( Status == USB_OTG_EP_RX_VALID )
    { if ( STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.STALL == 1 )
      { // � ep1->even_odd_frame = 0;
        USBDepClearStall( epAddr );
        return;
      }

      STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.CNAK  = 1;
      STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.USBAEP= 1;
      STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.EPENA = 1;
    }
    else if ( Status == USB_OTG_EP_RX_DIS )
    { STM32F4.USB.DEVICE.DOEP[ epNum ].CTL.USBAEP = 0;
} } }

/* ---------------------------- DCD ------------------------------------------- */

/**
 * @brief  This Function set USB device address
 * @param address: new device address
 * @retval : status
 */
void  USBDepSetAddress( byte address )
{ STM32F4.USB.DEVICE.DCFG.DAD= address; /** 0x04 Device address */
}

/**
 * @brief  This Function set USB device address
 * @param address: new device address
 * @retval : status
 */
void  USBDepSetTestMode( byte mode )
{ switch ( mode )
  { case  1: STM32F4.USB.DEVICE.DCTL.TCTL= 1; break;  /* TEST_J */
    case  2: STM32F4.USB.DEVICE.DCTL.TCTL= 2; break;  /* TEST_K */
    case  3: STM32F4.USB.DEVICE.DCTL.TCTL= 3; break;  /* TEST_SE0_NAK */
    case  4: STM32F4.USB.DEVICE.DCTL.TCTL= 4; break;  /* TEST_PACKET */
    case  5: STM32F4.USB.DEVICE.DCTL.TCTL= 5; break;  /* TEST_FORCE_ENABLE */
    default: STM32F4.USB.DEVICE.DCTL.TCTL= 1; break;
} }

/**
 * @brief  Connect device (enable internal pull-up)
 * @retval : None
 */
void DCD_DevConnect(  )
{ STM32F4.USB.DEVICE.DCTL.SDIS= 0; /** 0x01 Soft disconnect *//* Connect device */
  mDelay( 3 );
}


/**
 * @brief  Disconnect device (disable internal pull-up)
 * @retval : None
 */
void  DCD_DevDisconnect ( )
{ STM32F4.USB.DEVICE.DCTL.SDIS= 1; /** 0x01 Soft disconnect */
  mDelay( 3 );
}


/**
 * @brief  This Function flushes the FIFOs.
 * @param epnum: endpoint address
 * @retval : status
 */
dword USBDepFlush( byte epnum )
{ if ( epnum & 0x80 )  /* is IN */
  { usbOTGflushTxFifo( epnum & 0x7F );
  }
  else
  { usbOTGflushRxFifo();
  }

  return( 0 );
}


/* ------------------------------------------------------- INT --------------------- */

word t[5];

/**
 * @brief  DCDwriteEmptyTxFifo
 *         check FIFO for the next packet to be loaded
 * @retval status
 */
static short DCDwriteEmptyTxFifo( byte epNum )
{ int tot;
  if (( tot= USBDgetTxCount( epNum ))) /* Is there bytes to send */
  { if ( tot>300 )
    { tot= 512;

      t[0]= STM32F4.USB.DEVICE.DIEP[ 0 ].DTXFSTS.INEPTFSAV;
      t[1]= STM32F4.USB.DEVICE.DIEP[ 1 ].DTXFSTS.INEPTFSAV;
      t[2]= STM32F4.USB.DEVICE.DIEP[ 2 ].DTXFSTS.INEPTFSAV;
      t[3]= STM32F4.USB.DEVICE.DIEP[ 3 ].DTXFSTS.INEPTFSAV;
      t[4]= STM32F4.USB.DEVICE.DIEP[ 4 ].DTXFSTS.INEPTFSAV;
    }

    while( USBDwritePacket( epNum
                          , STM32F4.USB.DEVICE.DIEP[ epNum ].DTXFSTS.INEPTFSAV << 2 )); /* FIFO space */
  }
  else
  { STM32F4.USB.DEVICE.DIEPEMPMSK.INEPTXFEM &=  ~( 0x1 << epNum );  /** shut down TxFIFO empty mask */
  }

  return( 1 );
}

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
 * @brief  USBD_OTG_EP1IN_ISR_Handler
 *         handles all USB Interrupts
 * @retval status
 */
dword USBD_OTG_EP1IN_ISR_Handler(  )
{ union STM32_USB_DEVICE$DIEPINT diepint;
  dword fifoemptymsk, msk, emp;

  msk= STM32F4.USB.DEVICE.DINEP1MSK_NW.atomic;
  emp= STM32F4.USB.DEVICE.DIEPEMPMSK.atomic;

  msk |= ((emp >> 1 ) & 0x1) << 7;
  diepint.atomic= STM32F4.USB.DEVICE.DIEP[ 1 ].INT.atomic & msk;

  if ( diepint.XFRC )
  { fifoemptymsk= 0x1 << 1;

    STM32F4.USB.DEVICE.DIEPEMPMSK.atomic &= ~fifoemptymsk;
    STM32F4.USB.DEVICE.DIEP[ 1 ].INT.XFRC= 1;  /* TX COMPLETE */

    USBDdataInStage( 1 );
  }

  if ( diepint.AHBERR_NW ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.AHBERR_NW= 1; }
  if ( diepint.EPDISD    ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.EPDISD   = 1; }
  if ( diepint.TOC       ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.TOC      = 1; }
  if ( diepint.ITTXFE    ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.ITTXFE   = 1; }
  if ( diepint.INEPNM    ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.INEPNM   = 1; }
  if ( diepint.INEPNE    ) { STM32F4.USB.DEVICE.DIEP[ 1 ].INT.INEPNE   = 1; }
  if ( diepint.TXFE      ) { DCDwriteEmptyTxFifo( 1 );
                             STM32F4.USB.DEVICE.DIEP[ 1 ].INT.TXFE     = 1; }
  return( 1 );
}

#endif

/**
 * @brief  DCD_ReadDevInEP
 *         Reads ep flags
 * @retval status
 */
static dword DCD_ReadDevInEP( byte epnum )
{ dword msk= STM32F4.USB.DEVICE.DIEPMSK.atomic;
  dword emp= STM32F4.USB.DEVICE.DIEPEMPMSK.INEPTXFEM;

  msk |= ((emp >> epnum) & 0x1 ) << 7;

  return( STM32F4.USB.DEVICE.DIEP[ epnum ].INT.atomic & msk );
}


/**
 * @brief  DCD_HandleUsbReset_ISR
 *         This interrupt occurs when a USB Reset is detected
 * @retval status
 */
void handleUsbResetISR(  )
{ dword i;

 // STM32F4.USB.DEVICE.DCTL.RWUSIG= 1; /* Clear the Remote Wake-up Signaling !!! why anulate this */

  usbOTGflushTxFifo( 0 );  /* Flush the Tx FIFO */

  for ( i = 0
      ; i < USB_OTG_Core.devEndpoints
      ; i++ )
  { STM32F4.USB.DEVICE.DIEP[ i ].INT.atomic= 0xFF;
    STM32F4.USB.DEVICE.DOEP[ i ].INT.atomic= 0xFF;
  }

  STM32F4.USB.DEVICE.DAINT.atomic= 0xFFFFFFFF;

  STM32F4.USB.DEVICE.DAINTMSK.IEPINT   = 1;
  STM32F4.USB.DEVICE.DAINTMSK.OEPINT   = 1;
  STM32F4.USB.DEVICE.DOEPMSK.STUPM     = 1;
  STM32F4.USB.DEVICE.DOEPMSK.XFRCM     = 1;
  STM32F4.USB.DEVICE.DOEPMSK.AHBERRM_NW= 1;
  STM32F4.USB.DEVICE.DOEPMSK.EPDISDM   = 1;

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOUTEP1MSK, doepmsk.atomic );
#endif

  STM32F4.USB.DEVICE.DIEPMSK.TOCM=
  STM32F4.USB.DEVICE.DIEPMSK.XFRCM=
  STM32F4.USB.DEVICE.DIEPMSK.EPDISDM=
  STM32F4.USB.DEVICE.DIEPMSK.INEPNMM=
  STM32F4.USB.DEVICE.DIEPMSK.AHBERRM_NW= 1;

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DINEP1MSK, diepmsk.atomic );
#endif

  STM32F4.USB.DEVICE.DCFG.DAD= 0;        /* Reset Device Address */

  USBDep0OutStart();                /* setup EP0 to receive SETUP packets */
  STM32F4.USB.GLOBAL.GINTSTS.USBRST= 1;  /* Clear interrupt */

  USBDreset();                           /*Reset internal state machine */
}


/**
 * @brief  handleEnumDoneISR
 *         Read the device status register and set the device speed
 * @retval status
 *  Set USB turn-around time based on device speed and PHY interface.
 */
void handleEnumDoneISR(  )
{ USBDep0Activate(  );

  dword hclk= 16000000; //  hclk= RCC_Clocks.HCLK_Frequency;

  if ( usbDEVgetDeviceSpeed() == USB_SPEED_HIGH )   /* Full or High speed */
  { USB_OTG_Core.speed= USB_OTG_SPEED_HIGH;
    STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 9;
  }
  else
  { USB_OTG_Core.speed    = USB_OTG_SPEED_FULL;
//    STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 5;

    if      ((hclk >= 15000000) && (hclk < 16000000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xE; } /* hclk Clock Range between 15-16 MHz */
    else if ((hclk >= 16000000) && (hclk < 17100000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xD; } /* hclk Clock Range between 16-17.1 MHz */
    else if ((hclk >= 17100000) && (hclk < 18400000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xC; } /* hclk Clock Range between 17-18.4 MHz */
    else if ((hclk >= 18400000) && (hclk < 20000000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xB; } /* hclk Clock Range between 18.4-20 MHz */
    else if ((hclk >= 20000000) && (hclk < 21800000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xA; } /* hclk Clock Range between 20-21.8 MHz */
    else if ((hclk >= 21800000) && (hclk < 24000000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0x9; } /* hclk Clock Range between 21.8-24 MHz */
    else if ((hclk >= 24000000) && (hclk < 26600000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0x8; } /* hclk Clock Range between 24-26.6 MHz */
    else if ((hclk >= 26600000) && (hclk < 30000000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0x7; } /* hclk Clock Range between 26.6-30 MHz */
    else if ((hclk >= 30000000) && (hclk < 34300000)) { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0x6; } /* hclk Clock Range between 30-34.3 MHz */
    else   /* if(hclk >= 34300000) */                 { STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0x5; } /* hclk Clock Range between 34.3-168 MHz */
} }

/**
 * @brief  handleIsoOUTcopmISR
 *         handle the ISO OUT incomplete interrupt
 * @retval status
 */
void handleIsoOUTcopmISR()
{ USBDisoOUTIncomplete ();
}

/**
 * @brief  DCD_IsoINIncomplete_ISR
 *         handle the ISO IN incomplete interrupt
 * @retval status
 */
void handleIsoINcompISR()
{ USBDisoINIncomplete (  );
}



void handleOutEpISR()  /** 0x13 OUT endpoint interrupt */
{ dword ep_intr;
  union STM32_USB_DEVICE$DOEPINT DOEPINT; DOEPINT.atomic= 0;
  dword epnum = 0;

  ep_intr= STM32F4.USB.DEVICE.DAINT.OEPINT    /* Read in the device interrupt bits */
         & STM32F4.USB.DEVICE.DAINTMSK.OEPINT;

  while ( ep_intr )
  { if ( ep_intr & 0x1 )
    { DOEPINT.atomic= usbDEVreadOutEPitr( epnum );

      if ( epnum & 0x7 )
      { if ( DOEPINT.atomic )
        { ep_intr &= 0;
      } }

      if ( DOEPINT.XFRC )                 /* Transfer complete */
      { STM32F4.USB.DEVICE.DOEP[ epnum ].INT.XFRC= 1; /* Clear the bit in DOEPINTn for this interrupt */

//        if ( USB_OTG_Core.dmaEnable == 1 )
  //      { STM32F4.USB.DEVICE.DOEP[ epnum ].TSIZ.XFRSIZ; /*ToDo : handle more than one single MPS size packet */
//
  //        USB_DEV.outEp[ epnum ].xferCount= USB_DEV.outEp[ epnum ].maxpacket - deptsiz.XFRSIZ;
    // ��    }

        USBDdataOutStage( epnum ); /* Inform upper layer: data ready */ /* RX COMPLETE */

//        if ( USB_OTG_Core.dmaEnable == 1 )
  //      { if (( epnum == 0 ) && (USB_DEV.deviceState == USB_OTG_EP0_STATUS_OUT))
    //      { USBDep0OutStart(); /* prepare to rx more setup packets */
      // �� } }

      }

      if ( DOEPINT.EPDISD    ) { STM32F4.USB.DEVICE.DOEP[ epnum ].INT.EPDISD= 1   ; } /* Endpoint disable  *//* Clear the bit in DOEPINTn for this interrupt */
      if ( DOEPINT.AHBERR_NW ) { STM32F4.USB.DEVICE.DOEP[ epnum ].INT.AHBERR_NW= 1; } /* AHB Error */
      if ( DOEPINT.STUP )                                                 /* Setup Phase Done (control EPs) */
      { USBDsetupStage();                                                /* inform the upper layer that a setup packet is available */
        STM32F4.USB.DEVICE.DOEP[ epnum ].INT.STUP= 1;  /* SETUP COMPLETE */
    } }

    epnum++; ep_intr >>= 1;
} }

void handleInEpISR()                            /** 0x12 IN endpoint interrupt */
{ union STM32_USB_DEVICE$DIEPINT DIEPINT;

  dword epIntr= STM32F4.USB.DEVICE.DAINT.IEPINT
               & STM32F4.USB.DEVICE.DAINTMSK.IEPINT;
  dword epnum= 0;

  while ( epIntr )
  { if ( epIntr & 0x1 )                        /* In ITR */
    { DIEPINT.atomic= DCD_ReadDevInEP( epnum ); /* Get In ITR status */

      if ( DIEPINT.TXFE  )                      /* Token received, start transaction  */
      { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.TXFE= 1;
        DCDwriteEmptyTxFifo( epnum );
      }

      if ( DIEPINT.XFRC ) /** 0x00 Transfer completed */
      { STM32F4.USB.DEVICE.DIEPEMPMSK.INEPTXFEM &= ~( 0x1 << epnum );
        STM32F4.USB.DEVICE.DIEP[ epnum ].INT.XFRC= 1;
        USBDdataInStage( epnum );           /* TX COMPLETE */

   //     if ( USB_OTG_Core.dmaEnable == 1 )
     //   { if (( epnum == 0 )
       //   && ( USB_DEV.deviceState == USB_OTG_EP0_STATUS_IN ))
         // { USBDep0OutStart();         /* prepare to rx more setup packets */
//��      } }

       }

      if ( DIEPINT.AHBERR_NW ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.AHBERR_NW= 1; }
      if ( DIEPINT.TOC       ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.TOC      = 1; }
      if ( DIEPINT.ITTXFE    ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.ITTXFE   = 1; }
      if ( DIEPINT.INEPNM    ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.INEPNM   = 1; }
      if ( DIEPINT.INEPNE    ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.INEPNE   = 1; }
      if ( DIEPINT.EPDISD    ) { STM32F4.USB.DEVICE.DIEP[ epnum ].INT.EPDISD   = 1; }
    }

    epnum++; epIntr >>= 1;
} }


/**
 * @brief  handleCanReadDEV
 *
 * @param  none
 * @retval status
 */
void handleCanReadDEV()  /** 0x04 RxFIFO non-empty */
{ STM32F4.USB.GLOBAL.GINTMSK.RXFLVL= 0;   /* Disable the Rx Status Queue Level interrupt */

  union STM32_USB_GLOBAL$GRXSTSR status= STM32F4.USB.GLOBAL.GRXSTSP;

  switch( status.PKTSTS )
  { case STS_DATA_UPDT:  USBDreadPacket( status.EPNUM, status.BCNT ); break;
    case STS_SETUP_UPDT: USBDreadPacket( status.EPNUM, 0xFFFF      ); break;  /* Copy the setup packet received in FIFO into the setup buffer in RAM */

    case STS_GOUT_NAK:   break;
    case STS_XFER_COMP:  break;
    case STS_SETUP_COMP: break;
    default:  break;
  }

  STM32F4.USB.GLOBAL.GINTMSK.RXFLVL= 1;   /* Enable the Rx Status Queue Level interrupt */
}



/**
 * @brief  USBD_OTG_EP1OUT_ISR_Handler
 *         handles all USB Interrupts
 * @retval status
 */
//dword USBD_OTG_EP1OUT_ISR_Handler()
////{ union STM32_USB_DEVICE$DOEPINT DOEPINT;
//
//  DOEPINT.atomic  = STM32F4.USB.DEVICE.DOEP[ 1 ].INT.atomic;
//  DOEPINT.atomic &= STM32F4.USB.DEVICE.DOUTEP1MSK_NW.atomic;
//
//  if ( DOEPINT.XFRC )                 /* Transfer complete */
///  { CLEAR_ OUT_EP_INTR( 1, XFRC );     /* Clear the bit in DOEPINTn for this interrupt */
 //   if (USB_OTG_Core.dmaEnable == 1)
//    { USB_DEV.outEp[ 1 ].xferCount= USB_DEV.outEp[ 1 ].maxpacket
 //                                   - STM32F4.USB.DEVICE.DOEP[ 1 ].TSIZ.XFRSIZ;
 //   }
 //   USBDdataOutStage( 1 );
 // }
//
 // if ( DOEPINT.EPDISD )              /* Endpoint disable  */
 // { CLEAR_OUT_ EP_INTR( 1, EPDISD );  /* Clear the bit in DOEPINTn for this interrupt */
 // }
//
 // if ( DOEPINT.AHBERR_NW ) /* AHB Error */
 // { CLEAR_OUT_ EP_INTR( 1, AHBERR_NW );
 // }
//
 // return( 1 );
//}




/**
  * @brief  handleSessnReqISR
  *           Initiating the Session Request Protocol
  * @param  None
  * @retval : status
  */
WEAK void usbDevGotConnected();

static word handleSessnReqISR()
{ if ( STM32F4.USB.GLOBAL.GOTGCTL.BSVLD )     /** 0x13 B-session valid */
  { usbDevGotConnected();
  }
  return( 0 );
}

/**
  * @brief  handleOtgISR
  *         handles the OTG Interrupts
  * @param  None
  * @retval : status
  */
WEAK void USBdevGotDisconnected( void )
{
}
static word handleOtgISR()
{ if ( STM32F4.USB.GLOBAL.GOTGINT.SEDET )  /** 0x02 Session end detected */
  { USBdevGotDisconnected();
  }
  STM32F4.USB.GLOBAL.GOTGINT.SEDET= 1;   /* Clear interrupt */

  return( 0 );
}

/**
 * @brief  USBIrqHandlerDEV
 *         This function handles all USB Interrupts
 * @param  core: Selected device
 * @retval status
 */
INTERRUPT void USBIrqHandlerDEV( /* dword core */ )
{ union STM32_USB_GLOBAL$GINTSTS INTS;

  if ( INTS.atomic= ( STM32F4.USB.GLOBAL.GINTSTS.atomic
                    & STM32F4.USB.GLOBAL.GINTMSK.atomic ))
  { STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xBFFFFFFE; /* Clear stored interrupts */
  /* Device events
   */
    if ( INTS.MMIS        ) { handleMmisISR      (); } /** 0x01 Mode mismatch interrupt                 */
    if ( INTS.IEPINT      ) { handleInEpISR      (); } /** 0x12 IN endpoint interrupt                   */
    if ( INTS.OEPINT      ) { handleOutEpISR     (); } /** 0x13 OUT endpoint interrupt                  */
    if ( INTS.WKUPINT     ) { USBdevGotResumed   (); } /** 0x1F Resume/remote wakeup detected interrupt */
    if ( INTS.SRQINT      ) { handleSessnReqISR  (); } /** 0x02 OTG interrupt, cable disconnect         */
    if ( INTS.OTGINT      ) { handleOtgISR       (); } /** 0x02 OTG interrupt, cable disconnect         */
    if ( INTS.INCOMPISOIN ) { handleIsoINcompISR (); } /** 0x14 Incomplete isochronous IN transfer      */
    if ( INTS.USBRST      ) { handleUsbResetISR  (); } /** 0x0C USB reset                               */
    if ( INTS.ENUMDNE     ) { handleEnumDoneISR  (); } /** 0x0D Enumeration done                        */
    if ( INTS.RXFLVL      ) { handleCanReadDEV   (); } /** 0x04 RxFIFO non-empty                        */
    if ( INTS.INCOMPISOOUT) { handleIsoOUTcopmISR(); } /** 0x15 Incomplete isochronous OUT transfer(Device mode) */

    if ( INTS.USBSUSP     )
    { USBdevGotSuspended( STM32F4.USB.DEVICE.DSTS.SUSPSTS ); /* Upon Resume call usr call back */
    }

    if ( INTS.SOF         )                            /** 0x03 Start of frame */
    { handleSofDevISR( STM32F4.USB.HOST.HFNUM.FRNUM );
} } }



