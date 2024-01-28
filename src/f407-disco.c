/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 * @file f407-disco.c
 *
 */

#include "usb_otg.h"
#include "stm32.h"

extern byte hubAddr;
extern byte mscAddr;
extern byte hidAddr;

word LED1= PORTPIN( PORTD, 12 );
word LED2= PORTPIN( PORTD, 13 );
word LED3= PORTPIN( PORTD, 14 );
word LED4= PORTPIN( PORTD, 15 );


void debug(){}
void testForEvents( dword l1, dword l2 );

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 168000000, 8000000 );  // 8 Mhz xtal
  sysTickConfig( 2000 );               // OS scheduler

/* Demo pins ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED2, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED3, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED4, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );




// USBinitDEV( USB_VBUS_INT );    /*  */
// USBinitHOST( PORTPIN( PORTC, 0 ) | USB_ID_PIN );
 USBinitOTG( PORTPIN( PORTC, 0 ) | USB_VBUS_INT | USB_ID_PIN  );

  while( 1 )
  { testForEvents( LED1, LED3 );
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


