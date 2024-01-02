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
 *     FILE: uc.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

#ifndef UC_H_INCLUDED
#define UC_H_INCLUDED

#ifndef LEGACY_TYPE_NAMES
#define LEGACY_TYPE_NAMES DEFINED_ONES

  #define schar   signed char
  #define byte  unsigned char
  #define word  unsigned short
  #define dword unsigned int
  #define qword unsigned long long

  #define  NULL   ((void*)0)

  #define  SECTION( sec  ) __attribute__ ((section( sec )))
  #define       WEAK       __attribute__ ((weak          ))
  #define  INTERRUPT       __attribute__ ((interrupt     ))
  #define      NAKED       __attribute__ ((naked         ))
  #define     PACKED       __attribute__ ((packed        ))

  #define ABSOLUTE( var, addr ) asm ( ".equ  " #var ", " #addr "\n .global " #var  );
  #define COMPILER_BARRIER asm volatile("":::"memory")
  #define ISB              asm volatile ("isb 0xF":::"memory")
  #define DSB              asm volatile ("dsb 0xF":::"memory")
  #define EI()             asm volatile ("cpsie i":::"memory")
  #define DI()             asm volatile ("cpsid i":::"memory")  /* Disable interrupts */

  #define ALIGN_THIS( w ) w  __attribute__ ((aligned (4)))
  #define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#endif

#define BYTE( b) ((byte )(b))
#define WORD( b) ((word )(b))
#define DWORD(b) ((dword)(b))
#define QWORD(b) ((qword)(b))

#define SETBIT( r, b ) if ( b ) r= 1


typedef void  ( *pgmfun )( void );  // ISR no parameters managing
typedef word  ( *pgmwrd )( void );  // ISR no parameters managing
typedef   int ( *argfun )( void * );           /* Minimal callback */



typedef struct
{ argfun code;
  void * data;
} IsrGPIO;



/**
 *   Device classes
 */

enum
{     NO_DEVICE
,   GPIO_DEVICE
,    ADC_DEVICE
,    DAC_DEVICE
,    DMA_DEVICE
,    CRC_DEVICE
,    USB_DEVICE
,  TIMER_DEVICE
,   WWDG_DEVICE
,    SPI_DEVICE
,    I2C_DEVICE
,   SDIO_DEVICE
,   UART_DEVICE
,    PWR_DEVICE
, SYSCFG_DEVICE
};


#define DEV_DISABLE 0x80000000
#define DEV_ENABLE  0x80000000
#define DEV_IDRIVE  0x40000000
#define DEV_DDRIVE  0x20000000
#define DEV_UPDATE  0x08000000 /*! Update event  */
#define DEV_SWICTH  0x04000000 /*! conmutation   */
#define DEV_TRIGER  0x02000000 /*! Trigger       */
#define DEV_BREAK   0x01000000 /*! Break                */
#define DEV_XMIT    0x00800000 /*! Trasnmit              */
#define DEV_RECV    0x00400000 /*! Break                 */
#define DEV_XFER    0x00400000 /*! TX-RECV               */
#define DEV_TCOMP   0x00400000 /*! Transmission complete */


/* Modes for ports, almost shared among brands
 */
#define GPIO_OUT    ( 0x8000 )
#define GPIO_IN     ( 0x0000 )
#define GPIO_PWSAVE ( 0xC000 )
#define GPIO_OD     ( 0x4000 )   /* Open drain */
#define GPIO_ANALOG ( 0x00C0 )

#define GPIO_STRO   ( GPIO_OUT | 0x0000 ) /* Output power */
#define GPIO_FAIR   ( GPIO_OUT | 0x0100 )
#define GPIO_ILLD   ( GPIO_OUT | 0x0200 )
#define GPIO_WEAK   ( GPIO_OUT | 0x0300 )

#define GPIO_SLOW   ( GPIO_OUT | 0x0000 ) /* Output speed */
#define GPIO_CRUI   ( GPIO_OUT | 0x0400 )
#define GPIO_HIGH   ( GPIO_OUT | 0x0800 )
#define GPIO_FAST   ( GPIO_OUT | 0x0C00 )

#define GPIO_FLOAT  ( GPIO_IN  | 0x0000 ) /* Input mode  */
#define GPIO_PULLUP ( GPIO_IN  | 0x1000 )
#define GPIO_PULLDN ( GPIO_IN  | 0x2000 )
#define GPIO_INOUT  ( GPIO_IN  | 0x3000 )

#define PORTPIN( port, pin       ) ( ( (port) << 8 ) | ( (pin) & 0xFF ) )
#define PINDEF(  port, pin, mode ) ( ( (port) << 8 ) | ( (pin) & 0xFF ) | ( mode << 16 ) )

enum
{ PORTA =  0
, PORTB =  1
, PORTC =  2
, PORTD =  3
, PORTE =  4
, PORTF =  5
, PORTG =  6
, PORTH =  7
, PORTI =  8
, PORTJ =  9
, PORTK = 10
};

enum
{ COUNT_NULL   = 0x00   /* does nothing                */
, COUNT_UP     = 0x01   /* counts from 0, then resets */
, COUNT_DOWN   = 0x02   /* counts to 0, then reloads  */
, COUNT_BOTH   = 0x03   /* from 0 and back to 0       */

, COUNT_TRG_CLK=  0x04  /* internal clock    */
, COUNT_TRG_EXT=  0x05  /* direct extren     */
, COUNT_TRG_PRE=  0x06  /* prescaled extern  */
, COUNT_TRG_INT=  0x07  /* internal event    */

};


typedef void  (*irqfun   )( void );                                  /* Minimal callback     */
typedef dword (*xmitFunc )( void * args, dword idx );                                  /* Minimal callback     */


static inline void BIT1SET( volatile dword * addr, dword MASK, dword VALUE )
{ VALUE &=          MASK;
  MASK   = *addr & ~MASK;

  *addr= MASK | VALUE;
}

/* 0 .. 15 cells
 */
static inline void BIT2SET( volatile dword * addr, word idx, word VALUE )
{ word off= idx & 0x0F; off <<= 1; idx >>= 4;  /* xlate to matrix ( 16 per cell )*/

  BIT1SET( addr + idx
        , (dword)0x03  << off
        , (dword)VALUE << off );         /* Point to row    */
}

/* 0 .. 7 cells
 */
static inline void BIT4SET( volatile dword * addr, word idx, word VALUE )
{ word off= idx & 0x07; off <<= 2; idx >>= 3;  /* xlate to matrix ( 8 per cell ) */

  BIT1SET( addr + idx
        , (dword)0x0F  << off
        , (dword)VALUE << off );         /* Point to row    */
}

/* 0 .. 3 cells
 */
static inline void BIT8SET( volatile dword * addr, word idx, word VALUE )
{ word off= idx & 0x03; off <<= 3; idx >>= 2;  /* xlate to matrix ( 4 per cel )*/

  BIT1SET( addr + idx
        , (dword)0xFF  << off
        , (dword)VALUE << off );         /* Point to row    */
}



/*
 * UART related
 */

#define NAN 0x80000000

/* UART related
 */
#define UART_STOP_0    0x00000000UL
#define UART_STOP_1    0x40000000UL
#define UART_STOP_2    0x80000000UL
#define UART_STOP_3    0xC0000000UL
#define UART_9_BITS    0x20000000UL
#define UART_RTSE      0x10000000UL  /*  8 RTS enable */
#define UART_CTSE      0x08000000UL  /*  9 CTS enable */
#define UART_PARODD    0x04000000UL
#define UART_PAREVEN   0x02000000UL

#define UART_BAUDMASK  0x01FFFFFFUL
#define UART_ABRMOD0   0x00800000UL
#define UART_ABRMOD1   0x00400000UL
#define UART_ABREN     0x00020000UL  /* 20 Auto baud rate enable */
#define UART_IREN      0x00010000UL  /*  1 Ir mode enable */
#define UART_IRLP      0x00008000UL  /*  2 Ir low-power */
#define UART_HDSEL     0x00004000UL  /*  3 Half-duplex selection */
#define UART_OVER8     0x00002000UL  /*  15 Oversampling mode  */
#define UART_RTOEN     0x00001000UL  /* 23 Receiver timeout enable */
#define UART_ONEBIT    0x00000800UL  /* 11 One sample bit method enable */
#define UART_OVRDIS    0x00000200UL  /* 12 Overrun Disable */
#define UART_ADDM7     0x00000200UL  /*  9º bit,  7-bit Address Detection/4-bit Address Detection */
#define UART_ADDR      0x000000FFUL  /* ADD4_7 ADD0_3 : 4;  24 Address of the USART node */

#define DMA_ENABLE_RX  0x80000000UL   /*  6 DMA enable receiver     */
#define DMA_ENABLE_TX  0x40000000UL   /*  7 DMA enable transmitter  */
#define DMA_DIS_ONERR  0x20000000UL   /* 13 DMA Disable on Reception Error */
#define CLOCK_PHASE    0x10000000UL   /*  0 Clock phase */
#define CLOCK_POL      0x08000000UL   /*  1 Clock polarity */
#define CLOCK_ENABLE   0x04000000UL   /*  1 Clock polarity */
#define DRIVER_ENABLE  0x02000000UL   /*  1 Clock polarity */
#define DRIVER_POL     0x01000000UL   /*  1 Clock polarity */
#define DRIVER_MODE    0x00800000UL   /*  1 Clock polarity */
#define COMM_1WIRE     0x00400000UL   /*  1 Half duplex */
#define COMM_SWAPBIT   0x00200000UL   /*  1 LSB first */
#define COMM_SWAPDATA  0x00100000UL   /*  1 LSB first */
#define COMM_SWAPPINS  0x00080000UL   /* 15 Swap TX/RX pins */
#define COMM_RXINV     0x00040000UL   /* 16 RX pin active level inversion */
#define COMM_TXINV     0x00020000UL   /* 17 TX pin active level inversion */

#define DMA_ENABLE ( DMA_ENABLE_RX | DMA_ENABLE_TX )


#define XFER_FE   0x40000000
#define XFER_NF   0x20000000
#define XFER_ORE  0x10000000
#define XFER_PE   0x08000000
#define XFER_CTS  0x04000000
#define XFER_IDLE 0x02000000
#define XFER_LBD  0x01000000
#define XFER_TC   0x00800000  /* Transmission complete */

extern void * uart1Args;
extern void * uart2Args;
extern void * uart6Args;


dword uart1Ctl(  void *, dword flags );
dword uart1Xmit( void *, dword idx   );
dword uart1Recv( void *, dword idx, dword data );

dword uart2Ctl(  void *, dword flags );
dword uart2Xmit( void *, dword idx   );
dword uart2Recv( void *, dword idx, dword data );

dword uart6Ctl(  void *, dword flags );
dword uart6Xmit( void *, dword idx   );
dword uart6Recv( void *, dword idx, dword data );



/* RTOS related
 */
void   mDelay( const dword msecs  );  // Delay in miliseconds
void   uDelay( const  word usecs  );  // 24 bit usec delay
int udelayGet( );                     // 24 bit usec delay
int udelayTop( const int usecs );          // 24 bit usec delay

void      osSysTick( const dword upTime ); // OS scheduler
void  sysTickConfig( const dword ticks  ); // RTOS initializer
void  setupHardware( void               );
dword     SYSCLK_HZ( const byte clk     );
dword SET_SYSCLK_HZ( dword hz, dword xtal );







#endif
