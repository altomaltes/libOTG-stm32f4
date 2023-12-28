/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 * @file main.c
 *
 */

#include "ff.h"
#include "stm32.h"

#include "usbd_hid_core.h"
#include "usbh_msc_core.h"


#define EXTR1HEAT PORTPIN( PORTC,  7 ) // TIM3_CH2_PC7
#define EXTR2HEAT PORTPIN( PORTB,  0 ) // TIM3_CH3_PB0
#define EXTR3HEAT PORTPIN( PORTB,  1 ) // TIM3_CH4_PB0


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

  PIN_PUT( EXTR1HEAT, buf[ 1 ] > 0 );
  PIN_PUT( EXTR2HEAT, buf[ 2 ] > 0 );
  PIN_PUT( EXTR3HEAT, buf[ 1 ] < 0 );
//  PIN_PUT( LED4, buf[ 2 ] < 0 );

  USBD_HID_SendReport( buf, 4 );
}



/**
  * @brief  USBdevGotSuspended.
  * @param  None
  * @retval None
  */
word USBdevGotSuspended( byte status )
{ //PIN_RST( LED1 );
  //PIN_RST( LED2 );
  //PIN_RST( LED3 );
  //PIN_RST( LED4 );
  return( 0 );
}

/**
 * @brief  usbDevGotConnected
 *         Links the correct device handler
 * @retval
 */
void usbHIDkeyArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( EXTR1HEAT ); PIN_SET( EXTR2HEAT );
  }
  else
  { PIN_RST( EXTR1HEAT ); PIN_RST( EXTR2HEAT );
} }


/**
  * @brief  usbHostGotDisconnected.
  * @param  None
  * @retval None
  */
void usbHostGotDisconnected( byte devAddr )
{ PIN_RST( EXTR1HEAT );

  for( int loop= 0
     ;     loop < 3
     ;     loop ++ )
  { PIN_SET( EXTR2HEAT ); PIN_SET( EXTR3HEAT ); mDelay( 90 );
    PIN_RST( EXTR2HEAT ); PIN_RST( EXTR3HEAT ); mDelay( 60 );
} }

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 84000000, 25000000 );
  sysTickConfig( 8000 );
 // SYSCLK_HZ( USBDEV_HZ );

/* Demo pins ( carrousel leds )
 */
  PIN_MODE( EXTR1HEAT, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( EXTR2HEAT, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( EXTR3HEAT, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );

//  USBinitD( 0 );    /* Activate 1ms per packet callback ( handleSofDevISR ) */
 USBinitH( 0 );

  while( 1 )
  { mDelay( 100 );
    testForEvents( EXTR1HEAT, EXTR2HEAT );
} }

/**
  * @brief  Attributes for the linker
  * @param
  * @retval
  */
ABSOLUTE(    STM32F4, 0x50000000 ); // choose USB_OTG_FS

/* Linker attributes, Ram and flash size
 */
ABSOLUTE(   RAM_SIZE, 0x00018000 );
ABSOLUTE( FLASH_SIZE, 0x00080000 );


