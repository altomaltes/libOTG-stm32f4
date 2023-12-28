/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 * @file f407-disco.c
 *
 */

#include "usbh_core.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "stm32.h"

extern byte hubAddr;
extern byte mscAddr;
extern byte hidAddr;



#define LED1 PORTPIN( PORTD, 12 )
#define LED2 PORTPIN( PORTD, 13 )
#define LED3 PORTPIN( PORTD, 14 )
#define LED4 PORTPIN( PORTD, 15 )

word HIDpoll( word frame )
{ static schar buf[]={0,0,0,0};

  frame <<= 2;

  switch( frame & 0x70 )
  { case 0x00: buf[ 1 ]=  1; buf[ 2 ]=  0; break;
    case 0x10: buf[ 1 ]=  1; buf[ 2 ]= -1; break;
    case 0x20: buf[ 1 ]=  0; buf[ 2 ]= -1; break;
    case 0x30: buf[ 1 ]= -1; buf[ 2 ]= -1; break;
    case 0x40: buf[ 1 ]= -1; buf[ 2 ]=  0; break;
    case 0x50: buf[ 1 ]= -1; buf[ 2 ]=  1; break;
    case 0x60: buf[ 1 ]=  0; buf[ 2 ]=  1; break;
    case 0x70: buf[ 1 ]=  1; buf[ 2 ]=  1; break;
  }

  PIN_PUT( LED1, buf[ 1 ] > 0 );
  PIN_PUT( LED2, buf[ 2 ] > 0 );
  PIN_PUT( LED3, buf[ 1 ] < 0 );
  PIN_PUT( LED4, buf[ 2 ] < 0 );

  return( USBD_HID_SendReport( buf, 4 ));
}


/**
 * @brief  usbDevGotConnected
 *         Links the correct device handler
 * @retval
 */
void usbHIDkeyArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED2 ); PIN_SET( LED4 );
  }
  else
  { PIN_RST( LED2 ); PIN_RST( LED4 );
} }


/**
 * @brief  usbDevGotConnected
 *         Links the correct device handler
 * @retval
 */
void usbDevGotConnected()
{ for( int loop= 0
     ;     loop < 3
     ;     loop ++ )
  { PIN_SET( LED2 ); PIN_SET( LED4 ); mDelay( 90 );
    PIN_RST( LED2 ); PIN_RST( LED4 ); mDelay( 70 );
} }

/**
 * @brief  usbDevGotDisconnected
 *         Links the correct device handler
 * @retval
 */
void usbDevGotDisconnected(  )
{ for( int loop= 0
     ;     loop < 3
     ;     loop ++ )
  { PIN_SET( LED1 ); PIN_SET( LED3 ); mDelay( 80 );
    PIN_RST( LED1 ); PIN_RST( LED3 ); mDelay( 40 );
} }


/**
 * @brief  USBdevGotSuspended
 *         Links the correct device handler
 * @retval
 */
word USBdevGotSuspended( byte status )
{ if ( status )
  { usbDevGotDisconnected();
  }
  else
  { usbDevGotConnected();
  }
  return( 0 );
}


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
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 48000000, 8000000 );  // 8 Mhz xtal
  sysTickConfig( 8000 );               // OS scheduler

/* Demo pins ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED2, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED3, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED4, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );


 //USBinitD( USB_VBUS_INT );    /* Activate 1ms per packet callback ( handleSofDevISR ) */
 USBinitH( PORTPIN( PORTC, 0 ) | USB_ID_PIN );
//  USBinitOTG( PORTPIN( PORTC, 0 ) | USB_VBUS_INT | USB_ID_PIN  );

  while( 1 )
  { testForEvents( LED1, LED2 );
    mDelay(10);
  }
  return( 0 );
}

/**
  * @brief  Attributes for the linker
  * @param
  * @retval
  */

ABSOLUTE(    STM32F4, 0x50000000 ); // choose USB_OTG_FS

/* Linker attributes, Ram and flash size
 */
ABSOLUTE(   RAM_SIZE, 0x00020000 );
ABSOLUTE( FLASH_SIZE, 0x00100000 );


