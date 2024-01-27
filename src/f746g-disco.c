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

word LED1= PORTPIN( PORTI, 1 );
word LED2= PORTPIN( PORTI, 1 );
word LED3= PORTPIN( PORTI, 1 );
word LED4= PORTPIN( PORTI, 1 );





extern dword STM32F4;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 42000000, 25000000 );  // 25 Mhz xtal
  sysTickConfig( 8000 );                // OS scheduler

//ABSOLUTE(    STM32F4, 0x50000000 ); // choose USB_OTG_FS
  dword ULPI= ( &STM32F4 == 0x40040000 ) ? USB_ULPI_PHY : PORTPIN( PORTD, 5 ); // This board has a microchip ULPI on high speed

/* Demo pins ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );

  USBinitDEV( ULPI );
//  USBinitHOST( ULPI | USB_ID_PIN | USB_VBUS_INT ); // PORTJ 12
 // !!!  USBinitOTG(  ULPI | USB_ID_PIN | USB_VBUS_INT ); OTG not working. CIDSCHG is only externalli cleared

  while( 1 )
  { testForEvents( LED1, LED1 );
    mDelay(10);
  }
  return( 0 );
}

/**
  * @brief  Attributes for the linker
  * @param
  * @retval
  */
//ABSOLUTE(    STM32F4, 0x50000000 ); // choose USB_OTG_FS
ABSOLUTE(    STM32F4, 0x40040000 ); // choose USB_OTG_HS
ABSOLUTE(   RAM_SIZE, 0x00010000 );
ABSOLUTE( FLASH_SIZE, 0x00100000 );


