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


word LED1= PORTPIN( PORTC,  7 ); // TIM3_CH2_PC7
word LED2= PORTPIN( PORTB,  0 ); // TIM3_CH3_PB0
word LED3= PORTPIN( PORTB,  1 ); // TIM3_CH4_PB0

word LED4= PORTPIN( PORTB,  0 ); // TIM3_CH4_PB0


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{ SET_SYSCLK_HZ( 84000000, 25000000 );
  sysTickConfig( 8000 );

/* Demo pins ( carrousel leds )
 */
  PIN_MODE( LED1, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED2, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );
  PIN_MODE( LED3, GPIO_OUT | GPIO_FAIR | GPIO_HIGH );

  USBinitDEV( 0 );    /* As device*/
// USBinitHOST( 0 );

  while( 1 )
  { mDelay( 100 );
    testForEvents( LED1, LED2 );
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


