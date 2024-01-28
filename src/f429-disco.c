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

word LED1= PORTPIN( PORTG, 13 );
word LED2= PORTPIN( PORTG, 14 );
word LED3= PORTPIN( PORTG, 13 );
word LED4= PORTPIN( PORTG, 14 );

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 168000000, 8000000 );
  sysTickConfig( 2000 );
  SYSCLK_HZ(0);

/* Demo pins  ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED2, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );

// USBinitDEV( USB_VBUS_INT );      /* Activate 1ms per packet callback ( handleSofDevISR ) */
//  USBinitHOST(  PORTPIN( PORTC, 4 ) | USB_ID_PIN  );
 USBinitOTG( PORTPIN( PORTC, 4 ) | USB_VBUS_INT | USB_ID_PIN  ); //not working

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


