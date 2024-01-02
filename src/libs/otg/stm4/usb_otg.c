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
 * @file usb_otg.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_core.h"
#include "usbd_core.h"
#include "usb_dcd.h"
#include "usb_otg.h"

/**
  * @brief  USB_OTG_BSP_Resume
  *         Handles the USB Resume from Suspend Mode
  * @retval Status
  */
void USB_OTG_BSP_Resume(  ) /*  switch-off the clocks */
{ STM32F4.USB.PWRCLK.FS_PCGCCTL.STPPCLK = 1; /** 0x00 Stop PHY clock */
  STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK= 0; /** 0x01 Gate HCLK */
  STM32F4.USB.PWRCLK.FS_PCGCCTL.PHYSUSP = 0; /** 0x04 PHY Suspended */
  STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK= 1; /** 0x01 Gate HCLK */

  STM32F4.USB.HOST.HPRT.PENA   =    /** 0x02 Port enable                */
  STM32F4.USB.HOST.HPRT.PCDET  =    /** 0x01 Port connect detected      */
  STM32F4.USB.HOST.HPRT.PENCHNG=    /** 0x03 Port enable/disable change */
  STM32F4.USB.HOST.HPRT.POCCHNG= 0; /** 0x05 Port overcurrent change    */
  STM32F4.USB.HOST.HPRT.PSUSP  = 0; /** 0x07 Port suspend */

  STM32F4.USB.HOST.HPRT.PRST   = 1; mDelay(100);  /** 0x08 Port reset */
  STM32F4.USB.HOST.HPRT.PRST   = 0;              /** 0x08 Port reset */
}

/**
  * @brief  USB_OTG_BSP_Suspend
  *         Handles the Enter USB to Suspend Mode
  * @retval Status
  */
void USB_OTG_BSP_Suspend()
{ STM32F4.USB.HOST.HPRT.PENA   =    /** 0x02 Port enable                */
  STM32F4.USB.HOST.HPRT.PCDET  =    /** 0x01 Port connect detected      */
  STM32F4.USB.HOST.HPRT.PENCHNG=    /** 0x03 Port enable/disable change */
  STM32F4.USB.HOST.HPRT.POCCHNG= 0; /** 0x05 Port overcurrent change    */
  STM32F4.USB.HOST.HPRT.PSUSP  = 1; /** 0x07 Port suspend */

  /*  switch-off the clocks */
  STM32F4.USB.PWRCLK.FS_PCGCCTL.atomic= 0;
  STM32F4.USB.PWRCLK.FS_PCGCCTL.STPPCLK= 1; /** 0x00 Stop PHY clock */
  STM32F4.USB.PWRCLK.FS_PCGCCTL.GATEHCLK= 1; /** 0x01 Gate HCLK */
}



/**
  * @brief  handleOtgISR
  *         handles the OTG Interrupts
  * @param  None
  * @retval : status
  */
word handleOtgISR()
{ if ( STM32F4.USB.GLOBAL.GOTGINT.SEDET )  /** 0x02 Session end detected */
  { if ( USB_OTG_GetMode() == HOST_MODE )
    {
    }
    else
    {

  } }

  return( 0 );

/* ----> SRP SUCCESS or FAILURE INTERRUPT <----
 */
  if ( STM32F4.USB.GLOBAL.GOTGINT.SRSSCHG ) /** 0x08 Session request success status change */
  { if ( STM32F4.USB.GLOBAL.GOTGCTL.SRQSCS ) /** 0x00 Session request success */
    { if ( USB_OTG_GetMode() != HOST_MODE )
      {
      }

      STM32F4.USB.GLOBAL.GOTGCTL.SRQ= 1; /** 0x01 Session request */
    }
    else /* Session request failure                                          */
    { if ( USB_OTG_GetMode() != HOST_MODE )
      {
  } } }

  /* ----> HNP SUCCESS or FAILURE INTERRUPT <---- */
  if ( STM32F4.USB.GLOBAL.GOTGINT.HNSSCHG )     /** 0x09 Host negotiation success status change */
  { if ( STM32F4.USB.GLOBAL.GOTGCTL.HNGSCS ) /** 0x08 Host negotiation success */
    { if ( USB_OTG_GetMode() == HOST_MODE )             /* The core AUTOMATICALLY sets the Host mode                        */
      {
    } }

    else                                                        /* Host negotiation failure */
    {
    }

    STM32F4.USB.GLOBAL.GOTGINT.HNSSCHG= 1;     /* Ack "Host Negotiation Success Status Change" interrupt.          */
  }

/* ----> HOST NEGOTIATION DETECTED INTERRUPT <----
 */
  if ( STM32F4.USB.GLOBAL.GOTGINT.HNGDET ) /** 0x11 Host negotiation detected */
  { if ( USB_OTG_GetMode() == HOST_MODE  )    /* The core AUTOMATICALLY sets the Host mode   */
    {
    }
    else
    {
    }
    STM32F4.USB.GLOBAL.GOTGINT.HNGDET= 1;
  }

  if ( STM32F4.USB.GLOBAL.GOTGINT.ADTOCHG )  /** 0x12 A-device timeout change */
  { STM32F4.USB.GLOBAL.GOTGINT.ADTOCHG= 1;
  }

  if ( STM32F4.USB.GLOBAL.GOTGINT.DBCDNE ) /** 0x13 Debounce done */
  {// USB_OTG_ResetPort();
    STM32F4.USB.GLOBAL.GOTGINT.DBCDNE= 1;   /* Clear OTG INT */
  }

  return 1;
}


/**
  * @brief  handleSessnReqISR
  *           Initiating the Session Request Protocol
  * @param  None
  * @retval : status
  */
static word handleSessnReqISR()
{
  if  (( USB_OTG_GetMode() != HOST_MODE  )
   && ( STM32F4.USB.GLOBAL.GOTGCTL.BSVLD ))   /** 0x13 B-session valid */
  {
  }

  else if (STM32F4.USB.GLOBAL.GOTGCTL.ASVLD ) /** 0x12 A-session valid */
  {
  }

  return( 1 );
}


/**
  * @brief  USB_OTG_InitiateSRP
  *         Initiate an srp session
  * @param  None
  * @retval : None
  */
void USB_OTG_InitiateSRP()
{ if ( STM32F4.USB.GLOBAL.GOTGCTL.SRQ ) /** 0x01 Session request */
  { return;                               /* SRP in progress */
  }

  STM32F4.USB.GLOBAL.GOTGCTL.SRQ= 1;    /** 0x01 Session request */
  STM32F4.USB.GLOBAL.GUSBCFG.SRPCAP= 1; /** 0x08 SRP-capable */
}


/**
  * @brief  USB_OTG_InitiateHNP
  *         Initiate HNP
  * @param  None
  * @retval : None
  */
void USB_OTG_InitiateHNP( byte state, byte mode )
{ if ( mode )                /* Device mode */
  { if ( state )
    { STM32F4.USB.GLOBAL.GOTGCTL.DHNPEN= 1; /* B-Dev has been enabled to perform HNP         */
      STM32F4.USB.GLOBAL.GOTGCTL.HNPRQ = 1; /* Initiate an HNP req. to the connected USB host*/
  } }

  else if ( state )            /* Host mode */
  { STM32F4.USB.GLOBAL.GOTGCTL.HSHNPEN = 1;  /* A-Dev has enabled B-device for HNP       */
    STM32F4.USB.HOST.HPRT.PSUSP= 1;     /* The core clear this bit when disconnect interrupt generated (GINTSTS.DisconnInt = '1') */
  }

  STM32F4.USB.GLOBAL.GUSBCFG.HNPCAP= 1; /** 0x08 SRP-capable */
}



//static void OTGenableInts()
//{ //STM32F4.USB.GLOBAL.GOTGINT.SEDET=     /** 0x02 Session end detected */
 // STM32F4.USB.GLOBAL.GOTGINT.SRSSCHG=   /** 0x08 Session request success status change */
 // STM32F4.USB.GLOBAL.GOTGINT.HNSSCHG=   /** 0x09 Host negotiation success status change */
 // STM32F4.USB.GLOBAL.GOTGINT.HNGDET=    /** 0x11 Host negotiation detected */
 // STM32F4.USB.GLOBAL.GOTGINT.ADTOCHG=   /** 0x12 A-device timeout change */
 // STM32F4.USB.GLOBAL.GOTGINT.DBCDNE= 1; /** 0x13 Debounce done */

 // STM32F4.USB.GLOBAL.GINTMSK.SRQIM=       /** 0x1E Session request/new session detected interrupt mask */
 // STM32F4.USB.GLOBAL.GINTMSK.OTGINTM=     /** 0x02 OTG interrupt mask */
 // STM32F4.USB.GLOBAL.GINTMSK.CIDSCHGM= 1; /** 0x1C Connector ID status change mask */

/* initialize OTG features
 */

//  STM32F4.USB.GLOBAL.GCCFG.VBUSASEN= 0; /** 0x12 Disable the VBUS sensing device */
//  STM32F4.USB.GLOBAL.GCCFG.SOFOUTEN= 1; /** 0x14 SOF output enable */

 // STM32F4.USB.GLOBAL.GUSBCFG.TOCAL= 2;
 // STM32F4.USB.GLOBAL.GUSBCFG.TRDT= 0xF;
 // STM32F4.USB.GLOBAL.GUSBCFG.HNPCAP= 1; /** 0x08 SRP-capable */
//}




/**
  * @brief  USB_OTG_GetCurrentState
  *         Return current OTG State
  * @param  None
  * @retval : None
  */
byte USB_OTG_GetCurrentState ()
{ return( STM32F4.USB.GLOBAL.GOTGCTL.CIDSTS ? B_PERIPHERAL : A_HOST );
}

/**
  * @brief  USBinitOTG
  *         Return current OTG State
  * @param  None
  * @retval : None
  */
void * USBinitOTG( dword vbusPin )
{ OTGselectCore( vbusPin );
  OTGsetCurrentMode( OTG_MODE );

  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 0; /* Enable interrupts ( global ) */
  USBenableCommonInt( OTG_MODE );
  STM32F4.USB.GLOBAL.GAHBCFG.GINT= 1; /* Enable interrupts ( global ) */

  return( &USBIrqHandlerOTG );     /* Be sure is linked */
}



/**
 * @brief  handleConIDStChgISR
 *         handles the Connector ID Status Change Interrupt
 * @param  None
 * @retval : status
 */
void handleConIDStChgISR()
{ if ( STM32F4.USB.GLOBAL.GOTGCTL.CIDSTS ) /** 0x10 Connector ID status */
  { USBdriveVbus( 0 );   /* Only has sense over OTG */
    OTGcoreInitDev();    /* B-Device connector (Device Mode) */
  }
  else
  { USBcoreInitHost();
} }


/**
 * @brief  USBIrqHandlerOTG
 *         This function handles all USB Interrupts
 * @retval status
 */
INTERRUPT void USBIrqHandlerOTG( /*dword core */ )
{ union STM32_USB_GLOBAL$GINTSTS INTS;

  if (( INTS.atomic= STM32F4.USB.GLOBAL.GINTSTS.atomic
                   & STM32F4.USB.GLOBAL.GINTMSK.atomic ))
  { STM32F4.USB.GLOBAL.GINTSTS.atomic= 0xFFFFFFFF; /* Clear stored interrupts */

    if ( INTS.CIDSCHG )            /* Less probable, but void any other */
    { handleConIDStChgISR();   /** 0x1C Connector ID status change */
      return;
    }

  /* Host events
   */
    if ( INTS.NPTXFE  ) { handleNptxfemptyISR(); } /** 0x05 Non-periodic TxFIFO empty     */
    if ( INTS.PTXFE   ) { handlePtxfemptyISR (); } /** 0x1A Periodic TxFIFO empty         */
    if ( INTS.HPRTINT ) { handlePortISR      (); } /** 0x18 Host port interrupt           */
    if ( INTS.HCINT   ) { handleChannelISR   (); } /** 0x19 Host channels interrupt       */
    if ( INTS.DISCINT )
     { handleDisconnectISR(); } /** 0x1D Disconnect detected interrupt */

  /* Device events
   */
    if ( INTS.MMIS        ) { handleMmisISR      (); } /** 0x01 Mode mismatch interrupt */
    if ( INTS.IEPINT      ) { handleInEpISR      (); } /** 0x12 IN endpoint interrupt */
    if ( INTS.OEPINT      ) { handleOutEpISR     (); } /** 0x13 OUT endpoint interrupt */
    if ( INTS.INCOMPISOIN ) { handleIsoINcompISR (); } /** 0x14 Incomplete isochronous IN transfer */
    if ( INTS.USBRST      ) { handleUsbResetISR  (); } /** 0x0C USB reset */
    if ( INTS.ENUMDNE     ) { handleEnumDoneISR  (); } /** 0x0D Enumeration done */

//    if ( INTS.WKUPINT     ) { USBdevGotResumed   (); } /** 0x1F Resume/remote wakeup detected interrupt */
//    if ( INTS.USBSUSP     )
//   { USBdevGotSuspended( STM32F4.USB.DEVICE.DSTS.SUSPSTS ); /* Upon Resume call usr call back */
//    }

 /* Shared mode ints
  */

    if ( STM32F4.USB.GLOBAL.GINTSTS.CMOD )            /* Host mode */
    { if ( INTS.RXFLVL       ) handleCanReadHOST  ();     /** 0x04 RxFIFO non-empty */
      if ( INTS.INCOMPISOOUT ) hndIncompPerXferISR();     /** 0x15 Incomplete periodic transfer(Host mode) */
      if ( INTS.SOF          )                            /** 0x03 Start of frame */
      { handleSofHostISR( STM32F4.USB.HOST.HFNUM.FRNUM );
    } }

    else                                               /* Device mode */
    { if ( INTS.RXFLVL       )

        handleCanReadDEV   ();    /** 0x04 RxFIFO non-empty */
      if ( INTS.INCOMPISOOUT ) handleIsoOUTcopmISR();    /** 0x15 Incomplete isochronous OUT transfer(Device mode) */
      if ( INTS.SOF          )                           /** 0x03 Start of frame */
      { handleSofDevISR( STM32F4.USB.HOST.HFNUM.FRNUM );
    } }


/* OTG events, less probable
 */
    if ( INTS.OTGINT  )
     { handleOtgISR       (); } /** 0x02 OTG interrupt */
    if ( INTS.SRQINT  ) { handleSessnReqISR  (); } /** 0x1E Session request/new session detected interrupt */
} }


