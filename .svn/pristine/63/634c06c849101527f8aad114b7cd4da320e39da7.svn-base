/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                         ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 * This program is free software; you can redisgibute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is disgibuted in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *     FILE: STM32G031.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

#ifndef STM32_H_INCLUDED
#define STM32_H_INCLUDED

#include "cortexm.h"

#undef HSI_VALUE // !!!
#undef HSE_VALUE // !!!

#define HSI_VALUE 16  /* Hight speed internal clock ( MHZ )*/


short DEVICE_RESET(   dword device );
short DEVICE_ENABLE(  dword device );
short DEVICE_DISABLE( dword device );
short DEVICE_STATUS(  dword device );
short SLEEP_ENABLE(   dword device );
short SLEEP_DISABLE(  dword device );
short SLEEP_STATUS(   dword device );


/* GPIO functions
 */

short PIN_MODE( word idx, word mode );
short PORT_MODE( int idx, word mode, word pins );



short CONFIG_UART( dword mangle, dword baud, dword mode );
short ENABLE_UART( dword  uart                          );

dword nullXmit( void * args, int idx );

void fireUart1();
void fireUart2();
void fireUart3();
void fireUart4();
void fireUart5();
void fireUart6();

void setDebugUART1();
void setDebugUART2();
void setDebugUART6();


/* TIMERS
 */
short CONFIG_TIMER( dword  tim, dword period );
short CONFIG_TCHAN( dword  tim, dword work   );
short ENABLE_TIMER( dword  tim               );


/*
 */

extern volatile dword GPIO_BSRR[];  /* ports are exactly 2^8 dwords separated. Exploit this */
extern volatile dword GPIO_ODR[];  /* ports are exactly 2^8 dwords separated. Exploit this */
extern volatile dword GPIO_IDR[];  /* ports are exactly 2^8 dwords separated. Exploit this */

#define PIN_SET( pin )  GPIO_BSRR[ (pin) & 0xFF00 ]  = 0x00001 << (( pin ) & 0xFF )
#define PIN_RST( pin )  GPIO_BSRR[ (pin) & 0xFF00 ]  = 0x10000 << (( pin ) & 0xFF )
#define PIN_TGL( pin )   GPIO_ODR[ (pin) & 0xFF00 ] ^= 0x00001 << (( pin ) & 0xFF )
#define PIN_GET( pin ) ( GPIO_IDR[ (pin) & 0xFF00 ] &  0x00001 << (( pin ) & 0xFF ))

#define PIN_PUT( pin, val ) GPIO_BSRR[( pin ) & 0xFF00 ]= ( val ? 0x00001 : 0x10000 ) << (( pin ) & 0xFF )

enum
{ SYSDEV_HZ= 0
, USBDEV_HZ
};


dword SYSCLK_HZ( byte what );


/* Timer related
 */
struct TIM$SR
{ dword    UIF :   1; /** 0x00 Update interrupt flag */
  dword  CC1IF :   1; /** 0x01 Capture/compare 1 interrupt flag */
  dword  CC2IF :   1; /** 0x02 Capture/Compare 2 interrupt flag */
  dword  CC3IF :   1; /** 0x03 Capture/Compare 3 interrupt flag */
  dword  CC4IF :   1; /** 0x04 Capture/Compare 4 interrupt flag */
  dword  COMIF :   1; /** 0x05 COM interrupt flag */
  dword    TIF :   1; /** 0x06 Trigger interrupt flag */
  dword    BIF :   1; /** 0x07 Break interrupt flag */
  dword        :   1; /** 0x08 */
  dword  CC1OF :   1; /** 0x09 Capture/Compare 1 overcapture flag */
  dword  CC2OF :   1; /** 0x0A Capture/compare 2 overcapture flag */
  dword  CC3OF :   1; /** 0x0B Capture/Compare 3 overcapture flag */
  dword  CC4OF :   1; /** 0x0C Capture/Compare 4 overcapture flag */
  dword        :  19; /** 0x0D */
};

extern struct TIM$SR TIM2_SR;   /* 0 General purpose timers */
extern struct TIM$SR TIM3_SR;   /* 1 General purpose timers */
extern struct TIM$SR TIM4_SR;
extern struct TIM$SR TIM5_SR;   /* 3 General-purpose-timers */
extern struct TIM$SR TIM1_SR;   /* 40 Advanced-timers */
extern struct TIM$SR TIM8_SR;
extern struct TIM$SR TIM9_SR;   /* 50 General purpose timers */
extern struct TIM$SR TIM10_SR;  /* 51 General-purpose-timers */
extern struct TIM$SR TIM11_SR;  /* 52 General-purpose-timers */

#define TIM_CHANNEL( t, c ) (( t & 0xFF ) | (( c - 1 ) << 16 ))




#endif  /* STM32G031_H_INCLUDED */
