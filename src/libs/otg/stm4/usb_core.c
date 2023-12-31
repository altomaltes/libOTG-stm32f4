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

USB_OTG_CORE_HANDLE USB_OTG_Core;

/**
 * @brief  USB_OTG_CoreReset : Soft reset of the core
 * @retval schar : status
 */
static schar USB_OTG_CoreReset(  )
{ dword count = 0; do    /* Wait for AHB master IDLE state. */
  { uDelay( 100 );
    if ( ++count > 400000 )
    { return( 0 );
  } }
  while( !STM32F4.USB.GLOBAL.GRSTCTL.AHBIDL ); /** 0x1F AHB master idle */

  mDelay( 200 );

  count = 0; do  /* Core Soft Reset */
  { uDelay( 100 );
    if ( ++count > 400000 )
    { break;
  } }
  while( STM32F4.USB.GLOBAL.GRSTCTL.CSRST );

  mDelay( 200 );

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
schar USB_OTG_WritePacket( void * buff
                         , byte   ch_ep_num
                         , word   len )
{ if ( !USB_OTG_Core.dmaEnable )
  { dword * src= (dword *)buff;
    volatile dword * fifo= STM32F4.USB.FIFOMEM + ch_ep_num;

    len += 3; len /= 4;  while( len-- )               /* Quantize to dword */
    { *fifo= *src++;
  } }

  return( 0 );
}


/**
 * @brief  USB_OTG_ReadPacket : Reads a packet from the Rx FIFO
 * @param  dest : Destination Pointer
 * @param  bytes : No. of bytes
 * @retval None
 */
void * USB_OTG_ReadPacket( void * buff
                         , word len )
{ dword * dst= (dword *)buff;
  volatile dword * fifo= STM32F4.USB.FIFOMEM + 0;

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
static schar USBcoreInit(  )
{ STM32F4.USB.GLOBAL.GUSBCFG.PHYSEL= 1; /* FS interface (embedded Phy) */
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
    STM32F4.USB.GLOBAL.GCCFG.NOVBUSEN=  0;
/* B-peripheral session valid override enable
 */
    STM32F4.USB.GLOBAL.GOTGCTL.BVALOEN=
    STM32F4.USB.GLOBAL.GOTGCTL.BVALOVAL= 1;
  }
//STM32F4.USB.GLOBAL.GCCFG.NOVBUSEN= 0; //// çççç
///  if ( USB_OTG_Core.vbusPin & USB_VBUS_INT )  /* Vbus sense enabled *///
//  { STM32F4.USB.GLOBAL.GCCFG.SOFOUTEN= 1;     /** 0x14 SOF output enable */
//  }

  mDelay( 20 );

  if ( USB_OTG_Core.dmaEnable )          /* case the HS core is working in FS mode */
  { STM32F4.USB.GLOBAL.GAHBCFG.HBURSTLEN1= 5;  /* 64 x 32-bits*/
    STM32F4.USB.GLOBAL.GAHBCFG.DMAENABLE1= 1;
  }

#if defined (STM32F446xx) || defined (STM32F469_479xx)
  STM32F4.USB.GLOBAL.GUSBCFG.SRPCAP = 1;
#endif

  return( 0 );
}


/**
 * @brief  OTGselectCore
 *         Initialize core registers address.
 * @retval schar : status
 */
schar OTGselectCore( dword flags )
{ USB_OTG_Core.vbusPin  = flags;
  USB_OTG_Core.dmaEnable= 0;
  USB_OTG_Core.mps      = 64; // çççç USB_OTG_FS_MAX_PACKET_SIZE;

  /* initialize device cfg following its address */
  switch( (dword)&STM32F4.USB )
  { case 0x50000000:                                 /* Full speed */
      USB_OTG_Core.speed= USB_OTG_SPEED_FULL; /* at startup the core is in FS mode */
      USB_OTG_Core.devEndpoints= 4;
      USB_OTG_Core.hostChannels= 8;

      if ( flags & USB_VBUS_INT )
      { PIN_MODE( PORTPIN( PORTA,  9 ), GPIO_IN  | GPIO_FLOAT  );  /* Vbus, left as default */
      }

      if ( USB_OTG_Core.vbusPin & USB_ID_PIN )
      { PIN_MODE( PORTPIN( PORTA, 10 ), GPIO_IN  | GPIO_PULLUP | GPIO_CRUI | AF_OTG_FS );  /* ID pin */
      }

      PIN_MODE( PORTPIN( PORTA, 11 ), GPIO_OUT | GPIO_FAIR   | GPIO_CRUI | AF_OTG_FS );   /* FS_DM   */
      PIN_MODE( PORTPIN( PORTA, 12 ), GPIO_OUT | GPIO_FAIR   | GPIO_CRUI | AF_OTG_FS );   /* FS_DP  */
      DEVICE_RESET(  RCC_OTG_FS  );
      DEVICE_ENABLE( RCC_OTG_FS  );

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

      if ( flags & USB_VBUS_INT )
      { PIN_MODE( PORTPIN( PORTB, 13 ), GPIO_IN  | GPIO_FLOAT                     );  /* Vbus, left as default */
      }

      if ( USB_OTG_Core.vbusPin & USB_ID_PIN )
      { PIN_MODE( PORTPIN( PORTB, 12 ), GPIO_IN  | GPIO_PULLUP         | AF_OTG_HS );  /* ID pin */
      }

      PIN_MODE( PORTPIN( PORTB, 14 ), GPIO_OUT | GPIO_FAIR | GPIO_HIGH | AF_OTG_HS );
      PIN_MODE( PORTPIN( PORTB, 15 ), GPIO_OUT | GPIO_FAIR | GPIO_HIGH | AF_OTG_HS );
      DEVICE_RESET(  RCC_OTG_FS );
      DEVICE_ENABLE( RCC_OTG_HS );

     /*! 74 < USB OTG HS End Point 1 Out global interrupt */
     /*! 75 < USB OTG HS End Point 1 In global interrupt  */
     /*! 76 < USB OTG HS Wakeup through EXTI interrupt    */

      IRQ_ENABLE( 77, 3 );  /*!< USB OTG HS global interrupt  */
    break;

    default: return( -1 );
  }

  DEVICE_ENABLE( RCC_SYSCFG ); // ???

  return( USBcoreInit());
}

/**
 * @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
 * @param  num : FO num
 * @retval schar : status
 */
schar USB_OTG_FlushTxFifo( dword num )
{ dword count    = 0;
  STM32F4.USB.GLOBAL.GRSTCTL.TXFFLSH = 1;
  STM32F4.USB.GLOBAL.GRSTCTL.TXFNUM  = num;

  do
  { if ( ++count > 400000 )
    { break;
  } }
  while ( STM32F4.USB.GLOBAL.GRSTCTL.TXFFLSH == 1);

  uDelay( 3 ); /* Wait for 3 PHY Clocks*/

  return( 0 );
}

/**
 * @brief  USB_OTG_FlushRxFifo : Flush a Rx FIFO
 * @retval schar : status
 */
schar USB_OTG_FlushRxFifo(  )
{ dword count = 0;

  STM32F4.USB.GLOBAL.GRSTCTL.RXFFLSH = 1;

  do
  { if (++count > 400000)
    { break;
  } }
  while ( STM32F4.USB.GLOBAL.GRSTCTL.RXFFLSH == 1);

  uDelay( 3 );   /* Wait for 3 PHY Clocks*/
  return( 0 );
}


/**
 * @brief  OTGsetCurrentMode : Set ID line
 * @param  mode :  (Host/device)
 * @retval schar : status
 */
schar OTGsetCurrentMode( byte mode )
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
 * @brief  USB_OTG_GetMode : Get current mode
 * @retval current mode
 */
dword USB_OTG_GetMode()
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
 * @brief  USBenableCommonInt
 *         Initializes the commmon interrupts, used in both device and modes
 * @retval None
 */
void USBenableCommonInt( byte mode )        /* Give OTG a chance */
{ union STM32_USB_GLOBAL$GINTSTS INTS; INTS.atomic= 0;

  if ( !USB_OTG_Core.dmaEnable )
  { INTS.RXFLVL= 1;
  }

/* Enable interrupts matching to the Device mode ONLY
 */
  switch( mode )
  { case DEVICE_MODE:
      if ( USB_OTG_Core.vbusPin & USB_VBUS_INT )    /* Vbus sense enabled */
      { INTS.SRQINT= 1;
      }

      INTS.SRQINT=
      INTS.USBRST=
      INTS.IEPINT=
      INTS.OEPINT=
      INTS.USBSUSP=         /** 0x0B USB suspend mask */
      INTS.ENUMDNE=
      INTS.INCOMPISOOUT= 1;

      INTS.WKUPINT= 1;       /** 0x1F Resume/remote wakeup detected interrupt mask */
   //   INTS.ESUSPM= 1;
    break;

    case HOST_MODE:
      INTS.HCINT  =     /** 0x19 Host channels interrupt */
      INTS.HPRTINT= 1;  /** 0x18 Host port interrupt */
    break;

    case DISABLE_MODE:
      STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xBFFFFFFF; /* Enable the interrupts in the INTMSK */
      STM32F4.USB.GLOBAL.GINTMSK.atomic= 0;
    return;

    case OTG_MODE:
//      INTS.SOF= 1;       /* To detect ID VBUSLESS role change*/
 // INTS.SRQIM=       /** 0x1E Session request/new session detected interrupt mask */
 // INTS.OTGINTM=     /** 0x02 OTG interrupt mask */
      INTS.atomic= 0xBFFFFFFF; INTS.CIDSCHG= 0; STM32F4.USB.GLOBAL.GINTSTS= INTS; /* disable previous */
      INTS.atomic= 0x00000000; INTS.CIDSCHG= 1; STM32F4.USB.GLOBAL.GINTMSK= INTS; /** 0x1C Connector ID status change mask */
    return;
  }

  INTS.SOF=
 // INTS.CIDSCHG=         /** 0x1C Connector ID status change mask, keep this one */
  INTS.INCOMPISOIN=
  INTS.INCOMPISOOUT= 1;  /** 0x14 Incomplete isochronous IN transfer */

INTS.SRQINT= 0;
  STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xBFFFFFFF; /* disable all previous */
  STM32F4.USB.GLOBAL.GINTMSK= INTS;
}

/**
  * @brief  HCDgetCurrentFrame
  *         This function returns the future frame number for sof packet
  * @retval Frame number
  *
  */
dword HCDgetCurrentFrame( word delta )
{ return( ( STM32F4.USB.HOST.HFNUM.FRNUM + delta ) & 0x3FFF ); /** 0x00 Frame number */
}



/**
  * @brief OTGgetCoreSpeed
  *
  * @param  None
  * @retval : speed
  */
schar OTGgetCoreSpeed()    { return( USB_OTG_Core.speed        ); }
schar OTGgetDevEndpoints() { return( USB_OTG_Core.devEndpoints ); }
schar OTGgetHostChannels() { return( USB_OTG_Core.hostChannels ); }
schar OTGgetDmaEnable()    { return( USB_OTG_Core.dmaEnable    ); }


