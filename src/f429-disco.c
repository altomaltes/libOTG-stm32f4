/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * @file f429-disco.c
 *
 */

#include "usbh_core.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "stm32.h"

#define LED1 PORTPIN( PORTG, 13 )
#define LED2 PORTPIN( PORTG, 14 )

word HIDpoll( word frame )
{ static signed char buf[]={0,0,0,0};

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

  return( USBD_HID_SendReport( buf, 4 ));
}



word  USBdevGotSuspended( byte status ){ return( 0 );}
void  usbDevGotConnected(){}
void  usbHostGotEnumerationDone(){}

/**
  * @brief  usbHostGotDisconnected.
  * @param  None
  * @retval None
  */
void usbHostGotDisconnected( byte devAddr )
{ for( int loop= 0
     ;     loop < 3
     ;     loop ++ )
  { PIN_RST( LED2 ); PIN_SET( LED1 ); mDelay( 60 );
    PIN_SET( LED2 ); PIN_RST( LED1 ); mDelay( 90 );
} }

int cnt;
void osSysTick1( dword upTime )
{ PIN_TGL( LED2 );
}

/**
 * @brief  usbDevGotConnected
 *         Links the correct device handler
 * @retval
 */
void usbHIDkeyArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED1 ); PIN_SET( LED2 );
  }
  else
  { PIN_RST( LED1 ); PIN_RST( LED2 );
} }

void usbHIDmouseArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED1 ); PIN_SET( LED2 );
  }
  else
  { PIN_RST( LED1 ); PIN_RST( LED2 );
} }

void usbHIDrawArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED1 ); PIN_SET( LED2 );
  }
  else
  { PIN_RST( LED1 ); PIN_RST( LED2 );
} }

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 42000000, 8000000 );
  sysTickConfig( 8000 );

/* Demo pins  ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED2, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );


 //USBinitD( USB_VBUS_INT );      /* Activate 1ms per packet callback ( handleSofDevISR ) */
  USBinitH(   PORTPIN( PORTC, 4 ) | USB_ID_PIN  );
 // USBinitOTG( PORTPIN( PORTC, 4 ) | USB_VBUS_INT | USB_ID_PIN  );


   while( 1 )
   { testForEvents( LED1, LED2 );
     mDelay(10);
} }

/**
  * @brief  Attributes for the linker
  */
ABSOLUTE(    STM32F4, 0x40040000 ); // choose USB_OTG_HS

/** Linker attributes, Ram and flash size
  */
ABSOLUTE(   RAM_SIZE, 0x00020000 );
ABSOLUTE( FLASH_SIZE, 0x00200000 );


