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

#include "usb_core.h"
#include "stm32f4.h"

struct USB_OTG_CORE_HANDLE USB_OTG_Core;

/**
 * @brief  USB_OTG_CoreReset : Soft reset of the core
 * @retval schar : status
 */
static schar USB_OTG_CoreReset(  )
{ volatile dword count = 0; do    /* Wait for AHB master IDLE state. */
  { if ( ++count > 1800000 * 10 )
    { return( 0 );
  } }
  while( !STM32F4.USB.GLOBAL.GRSTCTL.AHBIDL ); /** 0x1F AHB master idle */

  mDelay( 20 );  // Must wait 10ms

  count = 0; do  /* Core Soft Reset */
  { if ( ++count > 1800000 * 10 )
    { break;
  } }
  while( STM32F4.USB.GLOBAL.GRSTCTL.CSRST );

  return( 0 );
}

/**
 * @brief  USB_OTGWritePacket : Writes a packet into the Tx FIFO associated
 *         with the EP
 * @param  src : source pointer
 * @param  ch_ep_num : end point number
 * @param  bytes : No. of bytes
 * @retval schar : status
 */
short OTGwritePacket( const void * buff
                    , byte         ChEpNum
                    , word         len )
{ if ( !USB_OTG_Core.dmaEnable )
  { dword * src= (dword *)buff;
    volatile dword * fifo= STM32F4.USB.FIFOMEM[ ChEpNum ];

    len += 3; len /= 4;  while( len-- )  /* Quantize to dword */
    { *fifo= *src++;
  } }

  return( 0 );
}


/**
 * @brief  OTGreadPacket : Reads a packet from the Rx FIFO
 * @param  dest : Destination Pointer
 * @param  bytes : No. of bytes
 * @retval None
 */
void * OTGreadPacket( void * buff
                    , word len )
{ dword * dst= (dword *)buff;
  volatile dword * fifo= STM32F4.USB.FIFOMEM[ 0 ];

  len += 3; len /= 4; while( len-- )       /* Quantize to dword */
  { *dst++ = *fifo;
  }

  return( dst );
}

/**
 * @brief  USBcoreInit
 *         Initializes the USB_OTG controller registers and prepares the core
 *         device mode or host mode operation.
 * @retval schar : status
 */
 schar USBcoreInit(  )
{
  if ( USB_OTG_Core.vbusPin & USB_ULPI_PHY )   /* External interface */
  { STM32F4.USB.GLOBAL.GCCFG.PWRDWN= 0;    /** 0x10 Power down */
    STM32F4.USB.GLOBAL.GUSBCFG.TSDPS=
    STM32F4.USB.GLOBAL.GUSBCFG.PHYSEL=
    STM32F4.USB.GLOBAL.GUSBCFG.ULPIFSLS= 0; /* FS interface  */

//    STM32F4.USB.GLOBAL.GUSBCFG.ULPIEVBUSD=UTMISEL;  UTMI interface ( for STM32F722xx )

    STM32F4.USB.GLOBAL.GUSBCFG.ULPIEVBUSD= 1;  /* External VBUS */
    STM32F4.USB.GLOBAL.GUSBCFG.ULPIEVBUSI= 0;

    USB_OTG_CoreReset();                  /* Reset after a PHY select and set Host mode */
  }
  else
  {
    STM32F4.USB.GLOBAL.GUSBCFG.PHYSEL= 1; /* FS interface (embedded Phy) */
    USB_OTG_CoreReset();                  /* Reset after a PHY select and set Host mode */
    STM32F4.USB.GLOBAL.GCCFG.PWRDWN= 1;    /** 0x10 Power down */

    if ( USB_OTG_Core.vbusPin & USB_VBUS_INT )    /* Vbus sense enabled */
    { STM32F4.USB.GLOBAL.GCCFG.VBUSASEN= 1; /** 0x12 Enable the VBUS sensing device */
      STM32F4.USB.GLOBAL.GCCFG.VBUSBSEN= 1; /** 0x12 Enable the VBUS sensing device */
      STM32F4.USB.GLOBAL.GCCFG.NOVBUSEN= 0;
    }
    else
    {

#if defined (STM32F446xx) || defined (STM32F469_479xx)
      STM32F4.USB.GLOBAL.GCCFG.VBDEN = 1;
#else
      STM32F4.USB.GLOBAL.GCCFG.VBUSASEN= 0; /** 0x12 Disable the VBUS sensing device */
      STM32F4.USB.GLOBAL.GCCFG.VBUSBSEN= 0; /** 0x12 Disable the VBUS sensing device */
#endif
      STM32F4.USB.GLOBAL.GCCFG.NOVBUSEN=  1;
/* B-peripheral session valid override enable
 */
      STM32F4.USB.GLOBAL.GOTGCTL.BVALOEN=
      STM32F4.USB.GLOBAL.GOTGCTL.BVALOVAL= 1;
  } }
//STM32F4.USB.GLOBAL.GCCFG.NOVBUSEN= 0; //// ����
///  if ( USB_OTG_Core.vbusPin & USB_VBUS_INT )  /* Vbus sense enabled *///
//  { STM32F4.USB.GLOBAL.GCCFG.SOFOUTEN= 1;     /** 0x14 SOF output enable */
//  }
//  mDelay( 20 );   // AWFULL !!!

  if ( USB_OTG_Core.dmaEnable )          /* case the HS core is working in FS mode */
  { STM32F4.USB.GLOBAL.GAHBCFG.HBURSTLEN1= 5;  /* 64 x 32-bits*/
    STM32F4.USB.GLOBAL.GAHBCFG.DMAENABLE1= 1;
  }

#if defined (STM32F446xx) || defined (STM32F469_479xx)
//  STM32F4.USB.GLOBAL.GUSBCFG.SRPCAP = 1;
#endif

  return( 0 );
}


/**
 * @brief  OTGselectCore
 *         Initialize core registers address.
 * @retval schar : status
 */
short OTGselectCore( dword flags )
{ USB_OTG_Core.vbusPin= flags= ( flags == USB_KEEP_PCONF )
                        ? USB_OTG_Core.vbusPin : flags ;
  USB_OTG_Core.dmaEnable= 0;

  DEVICE_ENABLE( RCC_SYSCFG ); // ???

  /* initialize device cfg following its address */
  switch( (dword)&STM32F4.USB )
  { case 0x50000000:                                 /* Full speed */
      USB_OTG_Core.speed= USB_OTG_SPEED_FULL; /* at startup the core is in FS mode */
      USB_OTG_Core.devEndpoints= 4;
      USB_OTG_Core.hostChannels= 8;

      if ( flags & USB_VBUS_INT )
      { PIN_MODE( PORTPIN( PORTA, 9 ), GPIO_IN  | GPIO_FLOAT  );  /* Vbus, left as default */
      }

      if ( USB_OTG_Core.vbusPin & USB_ID_PIN )
      { PIN_MODE( PORTPIN( PORTA, 10 ), GPIO_IN  | GPIO_PULLUP | GPIO_CRUI | AF_OTG_FS );  /* ID pin */
      }

      PIN_MODE( PORTPIN( PORTA, 11 ), GPIO_OUT | GPIO_FAIR   | GPIO_CRUI | AF_OTG_FS );   /* FS_DM   */
      PIN_MODE( PORTPIN( PORTA, 12 ), GPIO_OUT | GPIO_FAIR   | GPIO_CRUI | AF_OTG_FS );   /* FS_DP  */
      DEVICE_ENABLE( RCC_OTG_FS  );
      DEVICE_RESET(  RCC_OTG_FS  ); mDelay( 50 ); /* Wait after a device reset */

    /*! 42, < USB OTG FS Wakeup through EXTI line interrupt  */

      IRQ_ENABLE( 67, 3 );  /*!< USB OTG FS global interrupt        */
    break;

    case 0x40040000:                          /* Hight speed */
      USB_OTG_Core.speed= USB_OTG_SPEED_HIGH; /* at startup the core is in FS mode */
      USB_OTG_Core.devEndpoints= 6;
      USB_OTG_Core.hostChannels= 12;

#ifdef USB_OTG_HS_INTERNAL_dmaEnableD
     USB_OTG_Core.dmaEnable       = 1;
#endif

      if ( flags & USB_ULPI_PHY )   /* External interface */
      { PIN_MODE( PORTPIN( PORTA,  5 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* FS_DM   */
        PIN_MODE( PORTPIN( PORTA,  3 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* FS_DM   */
        PIN_MODE( PORTPIN( PORTB,  0 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D1   */
        PIN_MODE( PORTPIN( PORTB,  1 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D2   */
        PIN_MODE( PORTPIN( PORTB,  5 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D3   */
        PIN_MODE( PORTPIN( PORTB, 10 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D4   */
        PIN_MODE( PORTPIN( PORTB, 11 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D5   */
        PIN_MODE( PORTPIN( PORTB, 12 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D6   */
        PIN_MODE( PORTPIN( PORTB, 13 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* D7  */
        PIN_MODE( PORTPIN( PORTC,  0 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* STP */
        PIN_MODE( PORTPIN( PORTH,  4 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* NXT */
        PIN_MODE( PORTPIN( PORTC,  2 ), GPIO_OUT | GPIO_FAIR  | GPIO_FAST | AF_OTG_FS );  /* DIR */

        DEVICE_ENABLE( RCC_OTG_ULPI );
      }
      else
      { if ( flags & USB_VBUS_INT )
        { PIN_MODE( PORTPIN( PORTB, 13 ), GPIO_IN  | GPIO_FLOAT                     );  /* Vbus, left as default */
        }

        if ( USB_OTG_Core.vbusPin & USB_ID_PIN )
        { PIN_MODE( PORTPIN( PORTB, 12 ), GPIO_IN  | GPIO_PULLUP         | AF_OTG_HS );  /* ID pin */
        }

        PIN_MODE( PORTPIN( PORTB, 14 ), GPIO_OUT | GPIO_FAIR | GPIO_HIGH | AF_OTG_HS );
        PIN_MODE( PORTPIN( PORTB, 15 ), GPIO_OUT | GPIO_FAIR | GPIO_HIGH | AF_OTG_HS );
      }

      DEVICE_ENABLE( RCC_OTG_HS );
      DEVICE_RESET(  RCC_OTG_HS ); mDelay( 50 ); /* Wait after a device reset */

     /*! 74 < USB OTG HS End Point 1 Out global interrupt */
     /*! 75 < USB OTG HS End Point 1 In global interrupt  */
     /*! 76 < USB OTG HS Wakeup through EXTI interrupt    */

      IRQ_ENABLE( 77, 3 );  /*!< USB OTG HS global interrupt  */
    break;

    default: return( -1 );
  }


  return( USBcoreInit());
}

/**
 * @brief  usbOTGflushTxFifo : Flush a Tx FIFO
 * @param  num : FO num
 * @retval schar : status
 */
schar usbOTGflushTxFifo( dword num )
{ volatile dword count    = 0;
  STM32F4.USB.GLOBAL.GRSTCTL.TXFFLSH= 1;
  STM32F4.USB.GLOBAL.GRSTCTL.TXFNUM = num;

  do
  { if ( ++count > 1400000 * 10 )
    { break;
  } }
  while ( STM32F4.USB.GLOBAL.GRSTCTL.TXFFLSH );

  mDelay( 3 ); /* Wait for 3 PHY Clocks*/

  return( 0 );
}

/**
 * @brief  usbOTGflushRxFifo : Flush a Rx FIFO
 * @retval schar : status
 */
schar usbOTGflushRxFifo(  )
{ volatile dword count = 0;

  STM32F4.USB.GLOBAL.GRSTCTL.RXFFLSH= 1;

  do
  { if ( ++count > 1400000 * 10 )
    { break;
  } }
  while( STM32F4.USB.GLOBAL.GRSTCTL.RXFFLSH );

  mDelay( 3 );   /* Wait for 3 PHY Clocks*/
  return( 0 );
}


/**
 * @brief  OTGsetCurrentMode : Set ID line
 * @param  mode :  (Host/device)
 * @retval schar : status
 */
short OTGsetCurrentMode( byte mode )
{
  if     ( mode == HOST_MODE)
  { STM32F4.USB.GLOBAL.GUSBCFG.FDMOD= 0;
    STM32F4.USB.GLOBAL.GUSBCFG.FHMOD= 1;
  }
  else if ( mode == DEVICE_MODE)
  { STM32F4.USB.GLOBAL.GUSBCFG.FDMOD= 1;
    STM32F4.USB.GLOBAL.GUSBCFG.FHMOD= 0;
  }
  else
  { STM32F4.USB.GLOBAL.GUSBCFG.FHMOD= 0;
    STM32F4.USB.GLOBAL.GUSBCFG.FDMOD= 0;
  }

  mDelay( 50 );
  return( 0 );
}

/**
 * @brief  usbOTGgetMode : Get current mode
 * @retval current mode
 */
dword usbOTGgetMode()
{ return( STM32F4.USB.GLOBAL.GINTSTS.CMOD );
}


/**
 * @brief  handleMmisISR
 *         handles all USB Interrupts
 * @retval status
 */
void handleMmisISR()
{
}


/**
 * @brief  usbOTGenableCommonInt
 *         Initializes the commmon interrupts, used in both device and modes
 * @retval None
 */
void usbOTGenableCommonInt( byte mode )        /* Give OTG a chance */
{ union STM32_USB_GLOBAL$GINTSTS INTS; INTS.atomic= 0;

  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0; /* Disable interrupts ( global ) */

  INTS.SOF=
  INTS.OTGINT= 1;

/* Enable interrupts matching to the Device mode ONLY
 */
  switch( mode )
  { case DEVICE_MODE:
      if ( USB_OTG_Core.vbusPin & USB_VBUS_INT )    /* Vbus sense enabled */
      { INTS.SRQINT= 1;
      }

      INTS.WKUPINT=     /** 0x1F Resume/remote wakeup detected interrupt mask */
      INTS.SRQINT=
      INTS.USBRST=
      INTS.IEPINT=
      INTS.OEPINT=
      INTS.USBSUSP=      /** 0x0B USB suspend mask */
      INTS.ENUMDNE=
      INTS.INCOMPISOOUT= 1;
   //   INTS.ESUSPM= 1;
    break;

    case HOST_MODE:
      INTS.HCINT  =     /** 0x19 Host channels interrupt */
      INTS.HPRTINT= 1;  /** 0x18 Host port interrupt     */
    break;

    case DISABLE_MODE:
      STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xFFFFFFFE; /* Enable the interrupts in the INTMSK */
      STM32F4.USB.GLOBAL.GINTMSK.atomic= 0;
      STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0;    /* Disable interrupts ( global ) */
    return;

    case OTG_MODE:

//      INTS.SOF= 1;       /* To detect ID VBUSLESS role change*/
 // INTS.SRQIM=       /** 0x1E Session request/new session detected interrupt mask */
 // INTS.OTGINTM=     /** 0x02 OTG interrupt mask */
      INTS.atomic= 0xBFFFFFFF; INTS.CIDSCHG= 0; STM32F4.USB.GLOBAL.GINTSTS= INTS; /* disable previous */
      INTS.atomic= 0x00000000; INTS.CIDSCHG= 1; STM32F4.USB.GLOBAL.GINTMSK= INTS; /** 0x1C Connector ID status change mask */
//      STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xFFFFFFFE; /* disable all previous, avoid CMOD */
  //    STM32F4.USB.GLOBAL.GINTMSK= INTS;
      STM32F4.USB.GLOBAL.GAHBCFG.GINT= 1;    /* Enable interrupts ( global ) */
    return;
  }

///** 0x1C Connector ID status change mask, keep this one in OTG mode
// */
//  INTS.CIDSCHG= ( STM32F4.USB.GLOBAL.GUSBCFG.FDMOD
//                | STM32F4.USB.GLOBAL.GUSBCFG.FHMOD )
//                ? 0 : 1 ;

  if ( !USB_OTG_Core.dmaEnable )
  { INTS.RXFLVL= 1;
  }

  INTS.INCOMPISOIN=
  INTS.INCOMPISOOUT= 1;  /** 0x14 Incomplete isochronous IN transfer */

  INTS.SRQINT= 0; /** 0x1E Session request/new session detected interrupt, dual role ? */
  STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xFFFFFFFE; /* disable all previous, avoid CMOD */
  STM32F4.USB.GLOBAL.GINTMSK= INTS;
  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 1;    /* Enable interrupts ( global ) */
}

/**
  * @brief  usbOTGgetCurrentFrame
  *         This function returns the future frame number for sof packet
  * @retval Frame number
  *
  */
dword usbOTGgetCurrentFrame( word delta )
{ return( ( STM32F4.USB.HOST.HFNUM.FRNUM + delta ) & 0x3FFF ); /** 0x00 Frame number */
}


/**
  * @brief OTGgetCoreSpeed
  *
  * @param  None
  * @retval : speed
  */
short OTGgetCoreSpeed()    { return( USB_OTG_Core.speed        ); }
short OTGgetDevEndpoints() { return( USB_OTG_Core.devEndpoints ); }
short OTGgetHostChannels() { return( USB_OTG_Core.hostChannels ); }
short OTGgetDmaEnable()    { return( USB_OTG_Core.dmaEnable    ); }


