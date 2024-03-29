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
 * @file usb_corehost.c
 *
 * original draft from MCD Application Team
 */

#include "usb_hcd.h"
#include "usbh_core.h"
#include "stm32f4.h"

/**
 * @brief  USBHcoreInit : Initializes USB_OTG controller for host mode
 * @retval status
 */
schar USBHcoreInit( )
{ word vbusPin= USB_OTG_Core.vbusPin & 0x0FFF; /* Extract ID pin flags */

  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0; /* Disasble interrupts */

/* Configure VBUS switch
 */
  if ( vbusPin )
  { PIN_MODE( vbusPin, GPIO_OUT | GPIO_HIGH );
    PIN_PUT( vbusPin, ( USB_OTG_Core.vbusPin & USB_VBUS_INV ) ? 0 : 1 );  /*ENABLE the Power Switch by driving the Enable LOW */
    mDelay( 400 );   /* Delay is need for stabilising the Vbus Low  in Reset Condition, when Vbus=1 and Reset-button is pressed by user */
  }

  STM32F4.USB.PWRCLK.FS_PCGCCTL.atomic= 0;  /* Restart the Phy Clock */ // USB_OTGWrite_ REG32(USB_OTG->regs.PCGCCTL, 0 );

  usbHOSTinitFSLSPClkSel( HCFG_48_MHZ );  /* Initialize Host Configuration Register */

  usbHostDeInit();                            /* Clear previous state */
  USBHresetPort();

  STM32F4.USB.HOST.HCFG.FSLSS= 0;          /** 0x02 FS- and LS-only support */


/* Configure data FIFO sizes
 *
 */

  STM32F4.USB.GLOBAL.GRXFSIZ.RXFD= USBHostConfig.rxFifoSize; /* set Rx FIFO size */

  STM32F4.USB.GLOBAL.DIEPTXF[ 0 ].TXSA= USBHostConfig.rxFifoSize;
  STM32F4.USB.GLOBAL.DIEPTXF[ 0 ].TXFD= USBHostConfig.txNPSize;

  STM32F4.USB.GLOBAL.HPERTXF[ 0 ].TXSA= USBHostConfig.rxFifoSize
                                      + USBHostConfig.txNPSize;
  STM32F4.USB.GLOBAL.HPERTXF[ 0 ].TXFD= USBHostConfig.txPRSize;

  if ( (dword)&STM32F4.USB == 0x50000000 )            /* set Rx FIFO size  Full Speed */
  {
  }

  else if ( (dword)&STM32F4.USB == 0x40040000 )         /* set Rx FIFO size  High Speed */
  {
  }

  else
  { return( -1 );  /* Out of sync */
  }

/* Make sure the FIFOs are flushed.
 */
  usbOTGflushTxFifo( 0x10 );         /* all Tx FIFOs */
  usbOTGflushRxFifo();


/* Clear all pending HC Interrupts
 */
  for ( int i = 0
      ;     i < USB_OTG_Core.hostChannels
      ;     i++ )
  { STM32F4.USB.HOST.HC[ i ].INT.atomic   = 0xFFFFFFFF;
    STM32F4.USB.HOST.HC[ i ].INTMSK.atomic= 0x00000000;
  }

  USBHdriveVbus( 1 );

  usbOTGenableCommonInt( HOST_MODE );  /* Enable the common interrupts */
  return( 0 );
}

/**
 * @brief  USBHisEvenFrame
 *         This function returns the frame number for sof packet
 * @retval Frame number
 */
byte USBHisEvenFrame( )
{ return( STM32F4.USB.HOST.HFNUM.FRNUM  & 0x1 );
}

/**
 * @brief  USBHdriveVbus : set/reset vbus
 * @param  state : VBUS state
 * @retval None
 */
void USBHdriveVbus( byte state )
{ word vbusPin= USB_OTG_Core.vbusPin & 0x0FFF; /* Extract ID pin flags */

  if ( vbusPin )
  { PIN_PUT( vbusPin, ( USB_OTG_Core.vbusPin & USB_VBUS_INV )
                    ? !!state : !state   );  /*ENABLE the Power Switch by driving the Enable LOW */
    mDelay( 200 );
  }

  if (( STM32F4.USB.HOST.HPRT.PPWR == 0 ) && ( state == 1 )) /* Turn on the Host port power. */
  { //STM32F4.USB.HOST.HPRT.PENA=
    //STM32F4.USB.HOST.HPRT.PCDET=
    //STM32F4.USB.HOST.HPRT.PENCHNG=
    //STM32F4.USB.HOST.HPRT.POCCHNG= 0;

    STM32F4.USB.HOST.HPRT.PPWR= 1;
  }
  if (( STM32F4.USB.HOST.HPRT.PPWR == 1 ) && ( state == 0 ))
  { //STM32F4.USB.HOST.HPRT.PENA=
    //STM32F4.USB.HOST.HPRT.PCDET=
    //STM32F4.USB.HOST.HPRT.PENCHNG=
    //STM32F4.USB.HOST.HPRT.POCCHNG= 1;

    STM32F4.USB.HOST.HPRT.PPWR= 0;
  }

   mDelay( 200 );
}


/**
 * @brief  usbHOSTinitFSLSPClkSel : Initializes the FSLSPClkSel field of the
 *         HCFG register on the PHY type
 * @param  freq : clock frequency
 * @retval None
 */
void usbHOSTinitFSLSPClkSel( byte freq )
{ STM32F4.USB.HOST.HCFG.FSLSPCS= freq; /** 0x00 FS/LS PHY clock select */ // hcfg.b.fslspclksel = freq;
}


/**
 * @brief  USBHresetPort : Reset Host Port
 * @retval status
 * @note : (1)The application must wait at least 10 ms (+ 10 ms security)
 *   before clearing the reset bit.
 */
byte USBHresetPort( )
{ STM32F4.USB.HOST.HPRT.PENA   =    /** 0x02 Port enable                */
  STM32F4.USB.HOST.HPRT.PCDET  =    /** 0x01 Port connect detected      */
  STM32F4.USB.HOST.HPRT.PENCHNG=    /** 0x03 Port enable/disable change */
  STM32F4.USB.HOST.HPRT.POCCHNG= 0; /** 0x05 Port overcurrent change    */

  STM32F4.USB.HOST.HPRT.PRST= 1; mDelay( 200 ); /** 0x08 Port reset */
  STM32F4.USB.HOST.HPRT.PRST= 0; mDelay(  40 ); /** 0x08 Port reset */

  return( 1 );
}


/**
 * @brief  USBHgetHCdad:
 * @param  hcNum : channel number
 * @retval schar : status STM32F4.USB.HOST.HC[ hcNum ]
 */
byte USBHgetHCdad( byte hcNum )
{ return( STM32F4.USB.HOST.HC[ hcNum ].CHAR.DAD );
}


/**
 * @brief  USBHinitC : Prepares a host channel for transferring packets
 * @param  hcNum : channel number
 * @retval schar : status STM32F4.USB.HOST.HC[ hcNum ]
 */

byte USBHaddrHC( byte hcNum, byte devAddr )
{ return( STM32F4.USB.HOST.HC[ hcNum ].CHAR.DAD= devAddr );
}

byte USBHpacketHC( byte hcNum, word maxPacket )
{ return( STM32F4.USB.HOST.HC[ hcNum ].CHAR.MPSIZ= maxPacket );
}

schar USBHinitC( byte hcNum
                     , byte epAddr, byte epType
                     , byte devAddr, word maxPacket )
{ volatile struct HC_STRUCT * HC= STM32F4.USB.HOST.HC + hcNum;

  byte   doPing= epType & DO_PING_FLAG;
  byte lowSpeed= epType & LOW_SPEED_FLAG;

  epType &= 0x3;

  HC->INT.atomic= 0xFFFFFFFF;

  if ( USB_OTG_Core.dmaEnable )  /* Enable channel interrupts required for this transfer. */
  { HC->INTMSK.AHBERR_NW= 1;
  }

  switch( epType )
  { case USB_EP_TYPE_CTRL:
    case USB_EP_TYPE_BULK:
      HC->INTMSK.XFRC   = 1;
      HC->INTMSK.STALL  = 1;
      HC->INTMSK.XACTERR= 1;
      HC->INTMSK.DTGLERR= 1;
      HC->INTMSK.NAK    = 1;

      if ( epAddr & EPDIR_IN )
      { HC->INTMSK.BBERR = 1;
      }
      else
      { HC->INTMSK.NYET= 1;
        if ( doPing )
        { HC->INTMSK.ACK= 1;
      } }
    break;

    case USB_EP_TYPE_INTR:
      HC->INTMSK.XFRC   = 1;
      HC->INTMSK.NAK    = 1;
      HC->INTMSK.STALL  = 1;
      HC->INTMSK.XACTERR= 1;
      HC->INTMSK.DTGLERR= 1;
      HC->INTMSK.FRMOVRR= 1;

      if ( epAddr & EPDIR_IN )       // EP is in
      { HC->INTMSK.BBERR = 1;
      }

    break;

    case USB_EP_TYPE_ISOC:
      HC->INTMSK.XFRC = 1;
      HC->INTMSK.FRMOVRR = 1;
      HC->INTMSK.ACK = 1;

      if ( epAddr & EPDIR_IN )       // EP is in
      { HC->INTMSK.XACTERR = 1;
        HC->INTMSK.BBERR = 1;
      }
      break;
  }

  STM32F4.USB.HOST.HAINTMSK.HAINTM |= 1 << hcNum;  /* Enable the top level host channel interrupt. */
  STM32F4.USB.GLOBAL.GINTMSK.HCINT= 1;           /* Make sure host channel interrupts are enabled. */

/* Program the HCCHAR register
 */
  HC->CHAR.DAD  = devAddr;
  HC->CHAR.EPNUM= epAddr &  EPDIR_MASK;
  HC->CHAR.EPDIR= epAddr >> 0x07;
  HC->CHAR.LSDEV= lowSpeed; // (USB_HOST . hc[ hcNum ].speed == HPRT0_PRTSPD_LOW_SPEED ) ? 1 : 0;
  HC->CHAR.EPTYP= epType;
  HC->CHAR.MPSIZ= maxPacket;

  if ( epType == HCCHAR_INTR )
  { HC->CHAR.ODDFRM= 1;
  }

  return( 0 );
}


/**
 * @brief  usbHOSTstartXferHC : Start transfer
 * @param  hcNum : channel number
 * @retval schar : status STM32_USB_HOST$FS_HCCHAR
 */
schar usbHOSTstartXferHC( byte hcNum, byte pid
                        , void * xferBuff, word xferlenHC )
{ volatile struct HC_STRUCT * HC= STM32F4.USB.HOST.HC + hcNum;

  union STM32_USB_HOST$HCTSIZ HSIZE; HSIZE.atomic= 0;

  word maxPacket= HC->CHAR.MPSIZ;   // JACS, recover from hardware
  word num_packets;
  word max_hc_pkt_count;
  max_hc_pkt_count = 256;

  union STM32_USB_HOST$FS_HCCHAR HCHAR;


/* Compute the expected number of packets associated to the transfer
 */
  if ( xferlenHC > 0 )
  { num_packets= ( xferlenHC
                 + maxPacket - 1 ) / maxPacket;

    if ( num_packets > max_hc_pkt_count )
    { num_packets= max_hc_pkt_count;
      xferlenHC = num_packets * maxPacket;
  } }
  else
  { num_packets = 1;
  }

  if ( HC->CHAR.EPDIR )    //  if ( USB_HOST . hc[ hcNum ].ep_ is_in )
  { xferlenHC= num_packets * maxPacket;
  }

/* Initialize the HCTSIZn register
 */
  HSIZE.XFRSIZ= xferlenHC;
  HSIZE.PKTCNT= num_packets;
  HSIZE.DPID  = pid;
  HC->TSIZ= HSIZE;

  if ( USB_OTG_Core.dmaEnable == 1 )
  { HC->DMA= (dword)xferBuff;
  }

  HCHAR= HC->CHAR;
    HCHAR.ODDFRM= USBHisEvenFrame(); /** 0x1D Odd frame       */
    HCHAR.CHENA= 1;                      /** 0x1F Channel enable  */  /* Set host channel enable */
    HCHAR.CHDIS= 0;                      /** 0x1E Channel disable */
  HC->CHAR= HCHAR;

  if ( ! USB_OTG_Core.dmaEnable ) /* Slave mode */
  { if (( HC->CHAR.EPDIR == 0 ) && ( xferlenHC >  0 ))  /* EP is out */
    { word len_words= (xferlenHC + 3 ) / 4;

      if ( len_words > STM32F4.USB.HOST.HPTXSTS.PTXFSAVL )  /* check if there is enough space in FIFO space */
      { switch( HC->CHAR.EPTYP )
        { case USB_EP_TYPE_CTRL: /* Non periodic transfer */
          case USB_EP_TYPE_BULK:
            STM32F4.USB.GLOBAL.GINTMSK.NPTXFE= 1; /** 0x05 Non-periodic TxFIFO empty mask */ /* need to process data in nptxfempty interrupt */ //  intmsk.b.nptxfempty = 1;
          break;

          case USB_EP_TYPE_INTR:         /* Periodic transfer */
          case USB_EP_TYPE_ISOC:
            STM32F4.USB.GLOBAL.GINTMSK.PTXFE= 1; /** 0x1A Periodic TxFIFO empty mask */ /* need to process data in ptxfempty interrupt */
          break;
      } }

      OTGwritePacket( xferBuff
                    , hcNum
                    , xferlenHC );
  } }

  return( 0 );
}


/**
 * @brief  USBHhaltHC : Halt channel
 * @param  hcNum : channel number
 * @retval schar : status
 */
void USBHhaltHC( volatile struct HC_STRUCT * HC )
{ HC->CHAR.CHDIS= 1;    /** 0x1E Channel disable */

/* Check for space in the request queue to issue the halt.
 */
  if ( HC->CHAR.EPTYP == HCCHAR_CTRL
    || HC->CHAR.EPTYP == HCCHAR_BULK )
  { if ( STM32F4.USB.HOST.HPTXSTS.PTXQSAV /*  nptxsts.b.nptxqspcavail == 0 */)
    { HC->CHAR.CHENA= 0;  /** 0x1F Channel enable */
  } }
  else
  { if (  STM32F4.USB.HOST.HPTXSTS.PTXQSAV == 0 /* hptxsts.b.ptxqspcavail == 0 */ )
    { HC->CHAR.CHENA= 0;  /** 0x1F Channel enable */
  } }

  HC->CHAR.CHENA= 1;  /** 0x1F Channel enable */
}

void USBHhaltHCnum( byte hcNum )
{ USBHhaltHC( STM32F4.USB.HOST.HC + hcNum );
}

/**
 * @brief  Issue a ping token
 * @param  None
 * @retval : None
 */
schar USBHdoPingHC( volatile struct HC_STRUCT * HC )
{ union STM32_USB_HOST$HCTSIZ HSIZE; HSIZE.atomic= 0;

//  union STM32_USB_HOST$FS_HCCHAR HCHAR= STM32F4.USB.HOST.HC[ hcNum ].CHAR;

  HSIZE.DOPNG_NW= 1;
  HSIZE.PKTCNT  = 1;
  HC->TSIZ= HSIZE;

  HC->CHAR.CHENA= 1;
  HC->CHAR.CHDIS= 0;
//  HC->CHAR= HCHAR;

  return( 0 );
}


/**
 * @brief  Stop the device and clean up fifo's
 * @param  None
 * @retval : None
 */
void USBHstopHost()
{ STM32F4.USB.HOST.HAINT.atomic=    0xFFFFFFFF;
  STM32F4.USB.HOST.HAINTMSK.atomic= 0x00000000;

  for ( dword i= 0  /* Flush out any leftover queued requests. */
      ;       i < USB_OTG_Core.hostChannels
      ;       i++ )
  { STM32F4.USB.HOST.HC[ i ].CHAR.CHENA= 0;
    STM32F4.USB.HOST.HC[ i ].CHAR.CHDIS= 1;
    STM32F4.USB.HOST.HC[ i ].CHAR.EPDIR= 0;
  }

  /* Flush the FIFO */
  usbOTGflushRxFifo(  );
  usbOTGflushTxFifo( 0x10 );
}


/* --------------------------- HCD ----------------------------------------------------- */

/**
  * @brief  HCD init
  *         Initialize the HOST portion of the driver.
  * @param  base_address: OTG base address
  * @retval Status
  */
schar HCD1init( )
{ STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0; /* Disasble interrupts */
  USBHcoreInit();                  /* Host initializations */
  usbOTGenableCommonInt( HOST_MODE );    /* Enable the common interrupts */
  return( 0 );
}


/**
  * @brief  HCDgetCurrentSpeed
  *         Get Current device Speed.
  * @retval Status
  */
dword HCDgetCurrentSpeed()
{ return( STM32F4.USB.HOST.HPRT.PSPD ); /** 0x11 Port speed */
}

/**
  * @brief  HCDresetPort
  *         Issues the reset command to device
  * @retval Status
  */
/*
 *  Before starting to drive a USB reset, the application waits for the OTG
 * interrupt triggered by the denounce done bit (DBCDNE bit in OTG_GOTGINT),
 * which indicates that the bus is stable again after the electrical denounce
 * caused by the attachment of a pull-up resistor on DP (FS) or DM (LS).
 */


dword HCDresetPort(  )
{ USBHresetPort();
  return( 0 );
}


/* ----------------------------------------------- INT --------------------------------------------- */

typedef enum
{ HC_ZERO = 0
, HC_IDLE
, HC_XFRC
, HC_HALTED
, HC_NYET
, HC_STALL
, HC_XACTERR
, HC_BBLERR
, HC_DATATGLERR

, HC_NAK
} HC_STATUS;

#define MAX_NACK 30

static byte HC_Status[ 12 ];  // USB_OTG_MAX_TX_FIFOS

/**
 * @brief  handleHcnOutISR
 *         Handles interrupt for a specific Host Channel
 *
 * @param  hcNum: Channel number
 * @retval status
 */
static word handleHcnOutISR( volatile struct HC_STRUCT * HC, word status )
{ volatile union STM32_USB_HOST$HCINT HCINT=
  { .atomic= HC->INT.atomic & HC->INTMSK.atomic
  };
  HC->INT= HCINT; /* Confirm to free to new ones */

  if ( HCINT.AHBERR_NW )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
  }

  if ( HCINT.ACK )
  {    /* Clear interrupt */ //    CLEAR_HC_INT( HCINT, AHBERR_NW );
  }

  if ( HCINT.FRMOVRR )    /** 0x09 Frame overrun */
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBHhaltHC( HC );
  }

  if ( HCINT.XFRC )        /** 0x00 Transfer completed */
  { HC->INTMSK.CHHLT= 1;   /** 0x01 Channel halted mask */
    USBHhaltHC( HC );

    status= HC_XFRC;
  }

  if ( HCINT.STALL )
  { HC->INTMSK.CHHLT= 1;   /** 0x01 Channel halted mask */
    USBHhaltHC(  HC );
    status= HC_STALL;
  }

  if ( HCINT.NAK )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */

    if ( !USB_OTG_Core.dmaEnable )
    { USBHhaltHC( HC );
    }

    status= HC_NAK;
  }

  if ( HCINT.XACTERR )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBHhaltHC( HC );
    status= HC_XACTERR;
  }

  if ( HCINT.NYET )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    if ( ! USB_OTG_Core.dmaEnable )
    { USBHhaltHC( HC );
    }

    status= HC_NYET;
  }

  if ( HCINT.DTGLERR )  /** 0x0A Data toggle error */
  { HC->INTMSK.CHHLT= 1;    /** 0x01 Channel halted mask */
    USBHhaltHC( HC );

    status= HC_DATATGLERR;
  }

  if ( HCINT.CHHLT )         /** 0x01 Channel halted */
  { HC->INTMSK.CHHLT= 0;

    switch( status )
    { case HC_NAK    : status |= URB_STATE_NACK  << 8; break;
      case HC_STALL  : status |= URB_STATE_STALL << 8; break;
      case HC_XACTERR: status |= URB_STATE_ERROR << 8; break;

      case HC_NYET:
        if ( HC->INTMSK.ACK )          // do_ping
        { USBHdoPingHC( HC );
        }
        status |= URB_STATE_NACK << 8;
      break;

      case HC_XFRC:
        status |= URB_STATE_DONE << 8;
        if ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK )
        { status |= URB_TOGGLE_OUT << 8; // USBHtoggle( num, 0 ); // USB_HOST.hc[ num ].toggleOut ^= 1;
        }
      break;
  } }

  return( status );
}

/**
 * @brief  handleHcnInISR
 *         Handles interrupt for a specific Host Channel
 * @param  hcNum: Channel number
 * @retval status
 */
static word handleHcnInISR( volatile struct HC_STRUCT * HC, word status )
{ union STM32_USB_HOST$HCINT HCINT=
  { .atomic= HC->INT.atomic & HC->INTMSK.atomic
  };
  HC->INT= HCINT; /* Confirm to free to new ones */

  if ( HCINT.AHBERR_NW )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
  }

  if ( HCINT.ACK )
  {  // CLEAR_HC_INT( hcreg,ack);
  }

  if ( HCINT.STALL )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    status= HC_STALL;
        /* Clear the NAK Condition */
      /* Clear the STALL Condition */
    HCINT.NAK= 0;    /* NOTE: When there is a 'stall', reset also nak,  else, the   will be overwritten by 'nak' in code below */
    USBHhaltHC( HC );
  }

  if ( HCINT.DTGLERR )       /** 0x0A Data toggle error */
  { HC->INTMSK.CHHLT= 1;     /** 0x01 Channel halted mask */
    USBHhaltHC( HC );
    status = HC_DATATGLERR;
  }

  if ( HCINT.FRMOVRR )
  { HC->INTMSK.CHHLT= 1;      /** 0x01 Channel halted mask */
    USBHhaltHC( HC );
  }

  if ( HCINT.XFRC )          /** 0x00 Transfer completed */
  { //if (USB_OTG_Core.dmaEnable == 1 )
    //{ USB_HOST.XferCnt[ num ]=  USB_HOST.hc[ num ].xfer Len
    //                         -  HC->TSIZ.XFRSIZ;
   // ��  }

    status= HC_XFRC;


    if (( HC->CHAR.EPTYP == USB_EP_TYPE_CTRL )
    ||  ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK ))
    { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
      USBHhaltHC( HC );

      status |= URB_TOGGLE_IN << 8; // USBHtoggle( num, 1 ); // ].toggleIn ^= 1;
    }

    else if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR )
    { HC->CHAR.ODDFRM= 1;
      status |= URB_STATE_DONE << 8;
  } }

  if ( HCINT.CHHLT ) /** 0x01 Channel halted */
  { HC->INTMSK.CHHLT= 0;

    switch( status )
    { case HC_XFRC:       status |=  URB_STATE_DONE  << 8; break;
      case HC_STALL:      status |=  URB_STATE_STALL << 8; break;
      case HC_XACTERR:
      case HC_DATATGLERR: status |=  URB_STATE_ERROR << 8; break;
      default: if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR )
      { status |= URB_TOGGLE_IN << 8;  // USBHtoggle( num, 1 ); // USB_HOST.hc[ num ].toggleIn ^= 1;
  } } }

  if ( HCINT.XACTERR )  /** 0x07 Transaction error */
  { HC->INTMSK.CHHLT= 1;    /** 0x01 Channel halted mask */
    status = HC_XACTERR;
    USBHhaltHC( HC );
  }

  if ( HCINT.NAK )
  { if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR)
    { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
      if ( !USB_OTG_Core.dmaEnable )
      { USBHhaltHC( HC );
    } }

    status= HC_NAK | ( URB_STATE_NACK << 8 );

    if  (( HC->CHAR.EPTYP == USB_EP_TYPE_CTRL ) /* re-activate the channel  */
    ||   ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK ))
    { HC->CHAR.CHENA= 1;
      HC->CHAR.CHDIS= 0;
  } }

  return( status );
}

/**
 * @brief  handleChannelISR
 *         This function indicates that one or more host channels has a pending
 * @retval status
 */
void handleChannelISR( )
{ /* Clear appropriate bits in HCINTn to clear the interrupt bit in
  * GINTSTS */

  word haint= STM32F4.USB.HOST.HAINT.HAINT;
  volatile struct HC_STRUCT * HC= STM32F4.USB.HOST.HC;
  word idx= 0;

  while( haint )      /* Process all bits */
  { if ( haint & 1 )  /* live */
    { word status= HC->CHAR.EPDIR
                 ? handleHcnInISR(  HC, HC_Status[ idx ] )
                 : handleHcnOutISR( HC, HC_Status[ idx ] );

      HC_Status[ idx ]= status & 0xFF; status >>= 8;
      usbHOSTsetUrbState( idx, status );
    }

    haint >>= 1; HC++; idx++;
} }

/**
 * @brief  handleDisconnectISR
 *         Handles disconnect event.
 * @retval status
 */
void handleDisconnectISR()
{ usbHostGotDisconnected( DEFAULT_DEVADDR );
  usbHostDeInit();                              /* Host de-initializations */
}

/**
 * @brief  handleNptxfemptyISR
 *         Handles non periodic tx fifo empty.
 * @retval status
 */
void handleNptxfemptyISR()
{ word chNum= STM32F4.USB.GLOBAL.GNPTXSTS.CHNUM_NW;
  word chLen= USBHwritePacket( chNum, 0 );
  word lenWords= (chLen + 3 ) / 4;

  while (( STM32F4.USB.GLOBAL.GNPTXSTS.NPTXFSAV > lenWords) && ( chLen ))  /** 0x00 Non-periodic TxFIFO space available */
  { word len= STM32F4.USB.GLOBAL.GNPTXSTS.NPTXFSAV * 4;

    if ( len > chLen ) /* Last packet */
    { len= chLen;
      STM32F4.USB.GLOBAL.GINTMSK.NPTXFE= 1;  /** 0x05 Non-periodic TxFIFO empty mask */
    }

    lenWords= ( chLen + 3 ) / 4;
    chLen= USBHwritePacket( chNum, len );
} }

/**
 * @brief  handlePtxfemptyISR
 *         Handles periodic tx fifo empty
 * @retval status
 */
void handlePtxfemptyISR()
{ word chNum= STM32F4.USB.GLOBAL.GNPTXSTS.CHNUM_NW;
  word chLen= USBHwritePacket( chNum, 0 );

  word lenWords= (chLen + 3) / 4;

  while (( STM32F4.USB.HOST.HPTXSTS.PTXFSAVL > lenWords) && ( chLen ))  /** 0x00 Non-periodic TxFIFO space available */
  { word len= STM32F4.USB.HOST.HPTXSTS.PTXFSAVL * 4;

    if ( len > chLen ) /* Last packet */
    { len= chLen;
      STM32F4.USB.GLOBAL.GINTMSK.PTXFE= 1;  /** 0x05 Non-periodic TxFIFO empty mask */
    }

    lenWords= ( chLen + 3) / 4;
    chLen= USBHwritePacket( chNum, len );
} }

/**
 * @brief  handlePortISR
 *         This function determines which interrupt conditions have occurred
 * @retval status
 */
void handlePortISR( )
{ dword retval = 0;

  union STM32_USB_HOST$HPRT TOWRITE= STM32F4.USB.HOST.HPRT;

/* Clear the interrupt bits in GINTSTS */
  TOWRITE.PENA   = 0; /** 0x02 Port enable */;
  TOWRITE.PCDET  = 0; /** 0x01 Port connect detected */
  TOWRITE.PENCHNG= 0; /** 0x03 Port enable/disable change */
  TOWRITE.POCCHNG= 0; /** 0x04 Port overcurrent active */

  if ( STM32F4.USB.HOST.HPRT.PCDET )  /* Port Connect Detected */
  { TOWRITE.PCDET= 1;                 /** 0x01 Port connect detected */
    gotDevConnected();

    for( int i= 0
       ;     i < USB_OTG_Core.hostChannels
       ;     i++ )
    { HC_Status[ i ]= HC_ZERO;
    }

    retval |= 1;
  }

  if ( STM32F4.USB.HOST.HPRT.PENCHNG )   /* Port Enable Changed */
  { TOWRITE.PENCHNG = 1;                                       /** 0x03 Port enable/disable change */

    if ( STM32F4.USB.HOST.HPRT.PENA == 1 )                      /** 0x02 Port enable */
    { if (( STM32F4.USB.HOST.HPRT.PSPD == HPRT0_PRTSPD_LOW_SPEED  )
      ||  ( STM32F4.USB.HOST.HPRT.PSPD == HPRT0_PRTSPD_FULL_SPEED ))
      { if ( STM32F4.USB.HOST.HPRT.PSPD == HPRT0_PRTSPD_LOW_SPEED )
        { STM32F4.USB.HOST.HFIR.FRIVL= HFIR_USEC_CLOCKS;        /** 0x00 Frame interval, JACS, 1us step over 6Mhz (5999) */

          if ( STM32F4.USB.HOST.HCFG.FSLSPCS != HCFG_6_MHZ )    /** 0x00 FS/LS PHY clock select */
          { usbHOSTinitFSLSPClkSel( HCFG_6_MHZ );
      } } }

      gotDevPortEnabled();
      STM32F4.USB.GLOBAL.GINTMSK.DISCINT= 1;                   /** 0x1D Disconnect detected interrupt mask */
    }
    else
    { gotDevPortDisabled();
  } }

/* Overcurrent Change Interrupt
 */
  if ( STM32F4.USB.HOST.HPRT.POCCHNG ) /** 0x05 Port overcurrent change */
  { TOWRITE.POCCHNG = 1;
    retval |= 1;
  }

  STM32F4.USB.HOST.HPRT= TOWRITE;   /* Clear Port Interrupts */
}

/**
 * @brief  handleCanReadHOST
 *         Handles the Rx Status Queue Level Interrupt
 * @retval status
 */
void handleCanReadHOST( )
{ STM32F4.USB.GLOBAL.GINTMSK.RXFLVL= 0;  /** =1  0x04 Receive FIFO non-empty mask */ /* Disable the Rx Status Queue Level interrupt */

  union STM32_USB_GLOBAL$GRXSTSR GXSTS= STM32F4.USB.GLOBAL.GRXSTSP;  /** !!! Destructive read */

  byte chNum= GXSTS.EPNUM;
  word chCnt= GXSTS.BCNT;

  switch( GXSTS.PKTSTS )
  { case GRXSTS_PKTSTS_IN:
      USBHreadPacket( chNum, chCnt );   /* Read the data into the host buffer. */

      if ( STM32F4.USB.HOST.HC[ chNum ].TSIZ.PKTCNT > 0 ) /* re-activate the channel when more packets are expected */
      { STM32F4.USB.HOST.HC[ chNum ].CHAR.CHENA= 1; /** 0x1F Channel enable */
        STM32F4.USB.HOST.HC[ chNum ].CHAR.CHDIS= 0; /** 0x1E Channel disable */
      }
    break;

    case GRXSTS_PKTSTS_IN_XFER_COMP:
    case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:
    case GRXSTS_PKTSTS_CH_HALTED:
    default: break;
  }

  STM32F4.USB.GLOBAL.GINTMSK.RXFLVL= 1;  /** 0x04 Receive FIFO non-empty mask */  /* Enable the Rx Status Queue Level interrupt */
}

/**
 * @brief  USB_OTG_USBH_handle_IncompletePeriodicXfer_ISR
 *         Handles the incomplete Periodic transfer Interrupt
 * @retval status
 */
void hndIncompPerXferISR(  )
{ STM32F4.USB.HOST.HC[ 0 ].CHAR.CHENA= 1; /** 0x1F Channel enable */
  STM32F4.USB.HOST.HC[ 0 ].CHAR.CHDIS= 1; /** 0x1E Channel disable */
}

/**
 * @brief  USBIrqHandlerHOST
 *         This function handles all USB Host Interrupts
 * @retval status
 */
static void USBIrqHandlerHOST( dword core )
{ union STM32_USB_GLOBAL$GINTSTS INTS= STM32F4.USB.GLOBAL.GINTSTS; /* Read atomically */

  STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xFFFFFFFE; /* Clear stored interrupts, allow new ones */

  if (( INTS.atomic &= STM32F4.USB.GLOBAL.GINTMSK.atomic ))
  {

  /* Host events
   */
    if ( INTS.DISCINT ) { handleDisconnectISR(); }  /** 0x1D Disconnect detected interrupt */
    if ( INTS.NPTXFE  ) { handleNptxfemptyISR(); }  /** 0x05 Non-periodic TxFIFO empty     */
    if ( INTS.PTXFE   ) { handlePtxfemptyISR (); }  /** 0x1A Periodic TxFIFO empty         */
    if ( INTS.HPRTINT ) { handlePortISR      (); }  /** 0x18 Host port interrupt           */
    if ( INTS.HCINT   ) { handleChannelISR   (); }  /** 0x19 Host channels interrupt       */

    if ( INTS.RXFLVL       ) handleCanReadHOST  ();     /** 0x04 RxFIFO non-empty */
    if ( INTS.INCOMPISOOUT ) hndIncompPerXferISR();     /** 0x15 Incomplete periodic transfer(Host mode) */
    if ( INTS.SOF          )                            /** 0x03 Start of frame */
    { handleSofHostISR( STM32F4.USB.HOST.HFNUM.FRNUM );
} } }

/**
 * @brief  USBinitHOST
 *         Host hardware and stack initializations
 * @retval None
 */
void * USBinitHOST( dword vbusPin )
{ USBirqHnd= USBIrqHandlerHOST;

  OTGselectCore( vbusPin );
  OTGsetCurrentMode( HOST_MODE ); /* No OTG, force Host Mode */
  USBHcoreInit();                 /* Start the USB OTG core  */

  return( &USBIrqHandlerHOST );   /* Be sure is linked */
}



