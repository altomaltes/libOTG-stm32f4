/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * @file host.c
 *
 */

#include "usbh_core.h"
#include "usb_hid.h"
#include "ff.h"
#include "stm32.h"

extern word LED1;
extern word LED2;
extern word LED3;
extern word LED4;


byte hubAddr= 0;
byte mscAddr= 0;
byte hidAddr= 0;

/**
 * @brief  usbHostGotDisconnected
 *         Links the correct device handler
 * @retval
 */
void usbHostGotDisconnected( byte devAddr )
{
  if ( devAddr == mscAddr )
  { for( int loop= 0
       ;     loop < 3
       ;     loop ++ )
    { PIN_SET( LED2 ); PIN_SET( LED4 ); mDelay( 90 );
      PIN_RST( LED2 ); PIN_RST( LED4 ); mDelay( 60 );
  } }

  if ( devAddr == hubAddr )
  { for( int loop= 0
       ;     loop < 3
       ;     loop ++ )
    { PIN_SET( LED1 ); PIN_SET( LED3 ); mDelay( 90 );
      PIN_RST( LED1 ); PIN_RST( LED3 ); mDelay( 60 );
} } }



/**
  * @brief  usbHostGotUserApplication.
  * @param  None
  * @retval None
  */
short testForEvents( dword led1, dword led2  )
{ if ( USBHclassSignaler.classHub )
  { USBHclassSignaler.classHub= 0;

    for( int loop= 0
       ;     loop < 3
       ;     loop ++ )
    { PIN_RST( led1 ); PIN_SET( led2 ); mDelay( 60 );
      PIN_SET( led1 ); PIN_RST( led2 ); mDelay( 90 );
   } }

  if ( USBHclassSignaler.classHid )
  { USBHclassSignaler.classHid= 0;

    for( int loop= 0
       ;     loop < 2
       ;     loop ++ )
    { PIN_RST( led2 ); mDelay( 60 );
      PIN_SET( led2 ); mDelay( 90 );
  } }

  if ( USBHclassSignaler.classMassStor )
  { USBHclassSignaler.classMassStor= 0;

    for( int loop= 0
       ;     loop < 3
       ;     loop ++ )
    { PIN_RST( led1 ); mDelay( 60 );
      PIN_SET( led1 ); mDelay( 90 );
    }

    FILINFO fno;
    DIR     dir;
    FATFS fatfs;

    if ( fMount( 0, &fatfs )) /* Initialises the File System*/
    { return(-1);
    }

   /* FRESULT res=*/ fOpendir( &dir, "0:/" );

    while( !fReaddir( &dir, &fno ) )
    { if ( ! *fno.fname )
      { break;
      }
      PIN_SET( led2 ); mDelay( 30 );
      PIN_RST( led2 ); mDelay( 40 );
  } }

  return( 0 );
}


/**
 * @brief  USBHgetHandleAtConnect
 *         Links the correct device handler
 * @retval
 */
hostClassLink USBHgetHandleAtConnect( USBHdeviceRec * dev )
{ void * handler= 0;

  switch( dev->Itf_Desc->bInterfaceClass )
  { case USB_CLASS_HUB:       /** 0x09  Hub                 */
      hubAddr= dev->devAddr;
    return( USBHhubInterfaceInit( dev, handler ));

    case USB_CLASS_MASS_STOR:
       mscAddr= dev->devAddr;
    return( USBHmscInterfaceInit( dev, handler ));

    case USB_CLASS_HID:     /** 0x03  HID                 */
      switch( dev->Itf_Desc->bInterfaceProtocol )  /*Decode Bootclass Protocol: Mouse or Keyboard*/
      { case USB_HID_PROTO_NONBOOT : handler= usbHIDrawArrived;   break;
        case USB_HID_PROTO_KEYBOARD: handler= usbHIDkeyArrived;   break;
        case USB_HID_PROTO_MOUSE   : handler= usbHIDmouseArrived; break;
      }
      hidAddr= dev->devAddr;
    return( USBHhidInterfaceInit( dev, handler ));

    case USB_CLASS_AUDIO  : /** 0x01  Audio               */
    case USB_CLASS_CDC    : /** 0x02  Communications and CDC Control */
    case USB_CLASS_PRINTER: /** 0x07  Printer             */
    case USB_CLASS_VIDEO  : /** 0x0E  Video               */
    case USB_CLASS_AV     : /** 0x10  Audio/Video Devices */
    default               : break;
  }

  return( 0 );
}


/**
 * @brief  USBHostConfigDef
 *         Links the correct device handler
 * @retval
 */
struct USBHostConfigDef USBHostConfig=
{ .rxFifoSize= 192  // #define RX_FIFO_HS_SIZE 512
, .txNPSize  =  96  // #define TXH_NP_HS_FIFOSIZ 256
, .txPRSize  =  96  //#define TXH_P_HS_FIFOSIZ  256
};


