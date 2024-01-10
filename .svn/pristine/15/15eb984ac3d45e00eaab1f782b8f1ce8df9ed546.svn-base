/**
  ******************************************************************************
  * @file    usb_hcd_int.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   Host driver interrupt subroutines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4usb.h"
#include "usb_hcd_int.h"

//#pragma GCC optimize ("O0")

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

static byte HC_Status[ USB_OTG_MAX_TX_FIFOS ];

/**
 * @brief  handleHcnOutISR
 *         Handles interrupt for a specific Host Channel
 *
 * @param  hcNum: Channel number
 * @retval status
 */
static word handleHcnOutISR( struct HC_STRUCT * HC, word status )
{ union STM32_USB_HOST$HCINT HCINT=
  { .atomic= HC->INT.atomic & HC->INTMSK.atomic
  };
  HC->INT= HCINT; /* Ack signals, so new ones can be detected */

  if ( HCINT.AHBERR_NW )
  { HC->INTMSK.CHHLT= 1;   /** 0x01 Channel halted mask */
  }

  if ( HCINT.ACK )
  {
  }

  if ( HCINT.FRMOVRR )   /** 0x09 Frame overrun */
  { HC->INTMSK.CHHLT= 1; /** 0x01 Channel halted mask */
    USBhaltHC( HC );
  }

  if ( HCINT.XFRC )      /** 0x00 Transfer completed */
  { HC->INTMSK.CHHLT= 1; /** 0x01 Channel halted mask */
    USBhaltHC( HC );
    status= HC_XFRC;
  }

  if ( HCINT.STALL )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBhaltHC( HC );
    status= HC_STALL;
  }

  if ( HCINT.NAK )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */

    if ( !USB_OTG_Core.dmaEnable )
    { USBhaltHC( HC );
    }
    status= HC_NAK;
  }

  if ( HCINT.XACTERR )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBhaltHC( HC );
    status= HC_XACTERR;
  }

  if ( HCINT.NYET )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    if ( ! USB_OTG_Core.dmaEnable )
    { USBhaltHC( HC );
    }
    status= HC_NYET;
  }

  if ( HCINT.DTGLERR )    /** 0x0A Data toggle error */
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBhaltHC( HC );
    status= HC_DATATGLERR;
  }

  if ( HCINT.CHHLT )       /** 0x01 Channel halted */
  { HC->INTMSK.CHHLT= 0;

    switch( status )
    { case HC_XFRC:
//        USBHsetUrb State( num, URB_DONE );

        if ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK )
        { status |= URB_TOGGLE_OUT << 8;             // USBHtog gle( num, 0 ); // USB_HOST.hc[ num ].toggleOut ^= 1;
        }
        status |= URB_DONE << 8;
      break;

      case HC_NYET:
        if ( HC->INTMSK.ACK )          // do_ping
        { USB_OTG_HC_DoPing( HC );
        }

      case HC_NAK    : status |= URB_NACK  << 8; break;
      case HC_STALL  : status |= URB_STALL << 8; break;
      case HC_XACTERR: status |= URB_ERROR << 8; break;
   } }

  return( status );
}

/**
 * @brief  handleHcnInISR
 *         Handles interrupt for a specific Host Channel
 * @param  hcNum: Channel number
 * @retval status
 */
static word handleHcnInISR( struct HC_STRUCT * HC, word status )
{ union STM32_USB_HOST$HCINT HCINT=
  { .atomic= HC->INT.atomic & HC->INTMSK.atomic
  };
  HC->INT= HCINT; /* Ack signals, so new ones can be detected */

  if ( HCINT.AHBERR_NW )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
  }

  if ( HCINT.ACK )
  {
  }

  if ( HCINT.STALL )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    status= HC_STALL;
    HCINT.NAK= 0;    /* NOTE: When there is a 'stall', reset also nak,  else, the   will be overwritten by 'nak' in code below */
    USBhaltHC( HC );
  }

  if ( HCINT.DTGLERR )                     /** 0x0A Data toggle error */
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBhaltHC( HC );
    status = HC_DATATGLERR;
  }

  if ( HCINT.FRMOVRR )
  { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
    USBhaltHC( HC );
  }

  if ( HCINT.XFRC )                        /** 0x00 Transfer completed */
  { //if (USB_OTG_Core.dmaEnable == 1 )
    //{ USB_ HOST.XferCnt[ num ]=  USB_ HOST.hc[ num ].xferLen
    //                         -  HC->TSIZ.XFRSIZ;
   // çç  }

    status= HC_XFRC;

    if (( HC->CHAR.EPTYP == USB_EP_TYPE_CTRL )
    ||  ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK ))
    { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
      USBhaltHC( HC );
      HCINT.NAK= 0;
      status |= URB_TOGGLE_IN << 8;   // USBHtog gle( num, 1 ); // ].toggleIn ^= 1;
    }

    else if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR )
    { HC->CHAR.ODDFRM= 1;
      status |= URB_DONE << 8;   // USBHsetUrb State( num, URB_DONE );
  } }

  if ( HCINT.CHHLT ) /** 0x01 Channel halted */
  { HC->INTMSK.CHHLT= 0;

    switch( status )
    { case HC_XFRC:    status |= URB_DONE  << 8; //USBH setUrb State( num, URB_DONE );
      case HC_STALL:   status |= URB_STALL << 8; // USBH setUrb State( num, URB_STALL );
      case HC_DATATGLERR:
      case HC_XACTERR: status |= URB_ERROR << 8; // USBH setUrb State( num, URB_ERROR );

      default: if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR )
    { status |= URB_TOGGLE_IN << 8;                           //USBHtog gle( num, 1 ); // USB_HOST.hc[ num ].toggleIn ^= 1;
  } } }

  if ( HCINT.XACTERR )  /** 0x07 Transaction error */
  { HC->INTMSK.CHHLT= 1;    /** 0x01 Channel halted mask */
    status = HC_XACTERR;
    USBhaltHC( HC );
  }

  if ( HCINT.NAK )
  { if ( HC->CHAR.EPTYP == USB_EP_TYPE_INTR)
    { HC->INTMSK.CHHLT= 1;  /** 0x01 Channel halted mask */
      if ( !USB_OTG_Core.dmaEnable )
      { USBhaltHC( HC );
    } }

    status= HC_NAK;

    if  (( HC->CHAR.EPTYP == USB_EP_TYPE_CTRL ) /* re-activate the channel  */
    ||   ( HC->CHAR.EPTYP == USB_EP_TYPE_BULK ))
    { HC->CHAR.CHENA= 1;
      HC->CHAR.CHDIS= 0;
    }

    { status |= URB_NACK << 8; // USBHsetUrb State( num, URB_NACK );
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

  for ( int i = 0
      ;     i < USB_OTG_Core.hostChannels
      ;     i ++ )
  { if ( haint & (1 << i) )                          /* live */
    { struct HC_STRUCT * HC= STM32F4.USB.HOST.HC + i;

      word stat= HC->CHAR.EPDIR
               ? handleHcnInISR(  HC, HC_Status[ i ] )
               : handleHcnOutISR( HC, HC_Status[ i ] );

      HC_Status[ i ]= stat & 0xFF; stat >>= 8;
      USBHsetUrbState( i, stat );
} } }

/**
 * @brief  handleDisconnectISR
 *         Handles disconnect event.
 * @retval status
 */
WEAK byte gotDevDisconnected ()
{ return( 0 );
}


void handleDisconnectISR()
{ //STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0;        /* Disasble interrupts */
  usbHostGotDisconnected( DEFAULT_DEVADDR );
  USBHdeInit();                              /* Host de-initializations */

//  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 1;        /* Disasble interrupts */
}

/**
 * @brief  handleNptxfemptyISR
 *         Handles non periodic tx fifo empty.
 * @retval status
 */
void handleNptxfemptyISR()
{ word chNum= STM32F4.USB.GLOBAL.GNPTXSTS.CHNUM_NW;
  word chLen= USBHwritePacket( chNum, 0 );
  word lenWords= (chLen + 3) / 4;

  while (( STM32F4.USB.GLOBAL.GNPTXSTS.NPTXFSAV > lenWords) && ( chLen ))  /** 0x00 Non-periodic TxFIFO space available */
  { word len= STM32F4.USB.GLOBAL.GNPTXSTS.NPTXFSAV * 4;

    if ( len > chLen ) /* Last packet */
    { len= chLen;
      STM32F4.USB.GLOBAL.GINTMSK.NPTXFE= 1;  /** 0x05 Non-periodic TxFIFO empty mask */
    }

    lenWords= ( chLen + 3) / 4;
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
          { USB_OTG_InitFSLSPClkSel( HCFG_6_MHZ );
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
INTERRUPT void USBIrqHandlerHOST( /* dword core */)
{ union STM32_USB_GLOBAL$GINTSTS INTS;

  if (( INTS.atomic= STM32F4.USB.GLOBAL.GINTSTS.atomic
                   & STM32F4.USB.GLOBAL.GINTMSK.atomic ))
  { STM32F4.USB.GLOBAL.GINTSTS= INTS; /* Clear stored interrupts */

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





