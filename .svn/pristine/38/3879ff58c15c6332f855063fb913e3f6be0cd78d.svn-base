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
 *     FILE: STM32F4.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

#ifndef STM32F4_H_INCLUDED
#define STM32F4_H_INCLUDED

#include "../stm32.h"


/**
        AF00     AF01      AF02       AF03        AF04         AF05       AF06       AF07         AF08    AF09 AF10
PA0  | -     | TIM2_CH1 | TIM5_CH1 | -        | -         | -         | -        | USART2_CTS | -         |  | -
PA1  | -     | TIM2_CH2 | TIM5_CH2 | -        | -         | -         | -        | USART2_RTS | -         |  | -
PA2  | -     | TIM2_CH3 | TIM5_CH3 | TIM9_CH1 | -         | -         | -        | USART2_TX  | -         |  | -
PA3  | -     | TIM2_CH4 | TIM5_CH4 | TIM9_CH2 | -         | -         | -        | USART2_RX  | -         |  | -
PA4  | -     | -        | -        | -        | -         | SPI1_NSS  | SPI3_NSS | USART2_CK  | -         |  | -
PA5  | -     | TIM2_CH1 | -        | -        | -         | SPI1_SCK  | -        | -          | -         |  | -
PA6  | -     | TIM1_BKIN| TIM3_CH1 | -        | -         | SPI1_MISO | -        | -          | -         |  | -
PA7  | -     | TIM1_CH1N| TIM3_CH2 | -        | -         | SPI1_MOSI | -        | -          | -         |  | -
PA8  | MCO_1 | TIM1_CH1 | -        | -        | I2C3_SCL  | -         |          | USART1_CK  | -         |  | OTG_FS_SOF
PA9  | -     | TIM1_CH2 | -        | -        | I2C3_SMBA | -         |          | USART1_TX  | -         |  | OTG_FS_VBUS
PA10 | -     | TIM1_CH3 | -        | -        | -         | -         | -        | USART1_RX  | -         |  | OTG_FS_ID
PA11 | -     | TIM1_CH4 | -        | -        | -         | -         | -        | USART1_CTS | USART6_TX |  | OTG_FS_DM
PA12 | -     | TIM1_ETR | -        | -        | -         | -         | -        | USART1_RTS | USART6_RX |  | OTG_FS_DP
PA13 | SWDIO | -        | -        | -        | -         | -         | -        | -          | -         |  | -
PA14 | SWCLK | -        | -        | -        | -         | -         | -        | -          | -         |  | -
PA15 | JTDI  | TIM2_CH1 | -        | -        | -         | SPI1_NSS  | SPI3_NSS | -          | -         |  | -

PB0  | -     | TIM1_CH2N| TIM3_CH3 | -        | -         | -         | -        | -          |
PB1  | -     | TIM1_CH3N| TIM3_CH4 | -        | -         | -         | -        | -          |
PB2  | -     | -        | -        | -        | -         | -         | -        | -          |
PB3  | SWO   | TIM2_CH2 | -        | -        | -         | SPI1_SCK  | I2S3_CK  | -          | - I2C2_SDA - - -
PB4  | JTRST | -        | TIM3_CH1 | -        | -         | SPI1_MISO | I2S3ex_SD| -          | - I2C3_SDA - - -
PB5  | -     | -        | TIM3_CH2 | -        | I2C1_SMBA | SPI1_MOSI |          | I2S3_SD    |
PB6  | -     | -        | TIM4_CH1 | -        | I2C1_SCL  | -         | -        | USART1_TX  |
PB7  | -     | -        | TIM4_CH2 | -        | I2C1_SDA  | -         | -        | USART1_RX  |
PB8  | -     | -        | TIM4_CH3 | TIM10_CH1| I2C1_SCL  | -         | -        | -          | - - - - SDIO_D4
PB9  | -     | -        | TIM4_CH4 | TIM11_CH1| I2C1_SDA  | SPI2_NSS  | -        | -          | - - - - SDIO_D5
PB10 | -     | TIM2_CH3 | -        | -        | I2C2_SCL  | SPI2_SCK  | -        | -          |
PB12 | -     | TIM1_BKIN| -        | -        | I2C2_SMBA | SPI2_NSS  | -        | -          |
PB13 | -     | TIM1_CH1N| -        | -        | -         | SPI2_SCK  | -        | -          |
PB14 | -     | TIM1_CH2N| -        | -        | -         | SPI2_MISO | I2S2ex_SD| -          |
PB15 | RTC_RN| TIM1_CH3N| -        | -        | -         | SPI2_MOSI | -        | -          |

*/

/* alternate function pins
 */

#define TIM3_CH1_PB4  2 | 0x80
#define TIM3_CH2_PB5  2 | 0x80
#define TIM3_CH3_PB0  2 | 0x80
#define TIM3_CH4_PB1  2 | 0x80

#define TIM3_CH1_PC6  2 | 0x80
#define TIM3_CH2_PC7  2 | 0x80
#define TIM3_CH3_PC8  2 | 0x80
#define TIM3_CH4_PC9  2 | 0x80

#define UART1_TX_PA9  7 | 0x80
#define UART1_RX_PA10 7 | 0x80

#define RCC_GPIOA   ( 0x0000 | 0x00 ) /** IO port A reset, enable, sleep */
#define RCC_GPIOB   ( 0x0000 | 0x01 ) /** IO port B reset, enable, sleep */
#define RCC_GPIOC   ( 0x0000 | 0x02 ) /** IO port C reset, enable, sleep */
#define RCC_GPIOD   ( 0x0000 | 0x03 ) /** IO port D reset, enable, sleep */
#define RCC_GPIOE   ( 0x0000 | 0x04 ) /** IO port E reset, enable, sleep */
#define RCC_GPIOH   ( 0x0000 | 0x07 ) /** IO port H reset, enable, sleep */
#define RCC_GPIOI   ( 0x0000 | 0x08 ) /** IO port H reset, enable, sleep */
#define RCC_GPIOJ   ( 0x0000 | 0x09 ) /** IO port H reset, enable, sleep */
#define RCC_GPIOK   ( 0x0000 | 0x0A ) /** IO port H reset, enable, sleep */

#define RCC_CRC     ( 0x0000 | 0x0C ) /** CRC reset       */
#define RCC_DMA1    ( 0x0000 | 0x15 ) /** DMA2 reset      */
#define RCC_DMA2    ( 0x0000 | 0x16 ) /** DMA2 reset      */

#define RCC_ETH_MAC ( 0x0000 | 0x10 ) /**        */
#define RCC_ETH_TX  ( 0x0000 | 0x1A ) /**        */
#define RCC_ETH_RX  ( 0x0000 | 0x1B ) /**        */
#define RCC_ETH_PTP ( 0x0000 | 0x1C ) /**        */

#define RCC_OTG_HS   ( 0x0000 | 0x1D ) /**      */
#define RCC_OTG_ULPI ( 0x0000 | 0x1E ) /**       */

#define RCC_OTG_FS ( 0x0100 | 0x07 ) /** USB OTG FS module reset, enable, sleep */

#define RCC_TIM2   ( 0x0400 | 0x00 ) /** TIM2 reset, enable, sleep */
#define RCC_TIM3   ( 0x0400 | 0x01 ) /** TIM3 reset, enable, sleep */
#define RCC_TIM4   ( 0x0400 | 0x02 ) /** TIM4 reset, enable, sleep */
#define RCC_TIM5   ( 0x0400 | 0x03 ) /** TIM5 reset, enable, sleep */
#define RCC_WWDG   ( 0x0400 | 0x0B ) /** Window watchdog reset, enable, sleep */
#define RCC_SPI2   ( 0x0400 | 0x0E ) /** SPI 2 reset, enable, sleep */
#define RCC_SPI3   ( 0x0400 | 0x0F ) /** SPI 3 reset, enable, sleep */
#define RCC_USART2 ( 0x0400 | 0x11 ) /** USART 2 reset, enable, sleep */
#define RCC_I2C1   ( 0x0400 | 0x15 ) /** I2C 1 reset, enable, sleep */
#define RCC_I2C2   ( 0x0400 | 0x16 ) /** I2C 2 reset, enable, sleep */
#define RCC_I2C3   ( 0x0400 | 0x17 ) /** I2C3 reset, enable, sleep */
#define RCC_PWR    ( 0x0400 | 0x1C ) /** Power interface reset, enable, sleep */

#define RCC_TIM1   ( 0x0500 | 0x00 ) /** TIM1 reset, enable, sleep */
#define RCC_USART1 ( 0x0500 | 0x04 ) /** USART1 reset, enable, sleep */
#define RCC_USART6 ( 0x0500 | 0x05 ) /** USART6 reset, enable, sleep */
#define RCC_ADC    ( 0x0500 | 0x08 ) /** ADC interface reset (common to all ADCs) */
#define RCC_SDIO   ( 0x0500 | 0x0B ) /** SDIO reset, enable, sleep */
#define RCC_SPI1   ( 0x0500 | 0x0C ) /** SPI 1 reset, enable, sleep */
#define RCC_SYSCFG ( 0x0500 | 0x0E ) /** System configuration controller reset, enable, sleep */
#define RCC_TIM9   ( 0x0500 | 0x10 ) /** TIM9 reset, enable, sleep */
#define RCC_TIM10  ( 0x0500 | 0x11 ) /** TIM10 reset, enable, sleep */
#define RCC_TIM11  ( 0x0500 | 0x12 ) /** TIM11 reset, enable, sleep */

#define  TIMER2   0  /* General purpose timers */
#define  TIMER3   1  /* General purpose timers */
#define  TIMER4   2  /* unknown */
#define  TIMER5   3  /* General-purpose-timers */

#define  TIMER1  40  /* Advanced-timers */
#define  TIMER8  41  /* unknown */
#define  TIMER9  50  /* General purpose timers */
#define  TIMER10 51  /* General-purpose-timers */
#define  TIMER11 52  /* General-purpose-timers */



/*
 * USART related
 */
union USART$SR
{ struct
  { dword         PE :   1; /** 0x00 Parity error */
    dword         FE :   1; /** 0x01 Framing error */
    dword         NF :   1; /** 0x02 Noise detected flag */
    dword        ORE :   1; /** 0x03 Overrun error */
    dword       IDLE :   1; /** 0x04 IDLE line detected */
    dword       RXNE :   1; /** 0x05 Read data register not empty */
    dword         TC :   1; /** 0x06 Transmission complete */
    dword        TXE :   1; /** 0x07 Transmit data register empty */
    dword        LBD :   1; /** 0x08 LIN break detection flag */
    dword        CTS :   1; /** 0x09 CTS flag */
    dword            :  22; /** 0x0A */
  };

  dword atomic;            /** atomic access */
};

union USART$DR
{ struct
  { dword         DR :   9; /** 0x00 Data value */
    dword            :  23; /** 0x09 */
  };

  dword atomic;            /** atomic access */
};

union USART$BRR
{ struct
  { dword DIV_Fraction :   4; /** 0x00 fraction of USARTDIV */
    dword DIV_Mantissa :  12; /** 0x04 mantissa of USARTDIV */
    dword              :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */
};

union USART$CR1
{ struct
  { dword        SBK :   1; /** 0x00 Send break */
    dword        RWU :   1; /** 0x01 Receiver wakeup */
    dword         RE :   1; /** 0x02 Receiver enable */
    dword         TE :   1; /** 0x03 Transmitter enable */

    dword     IDLEIE :   1; /** 0x04 IDLE interrupt enable */
    dword     RXNEIE :   1; /** 0x05 RXNE interrupt enable */
    dword       TCIE :   1; /** 0x06 Transmission complete interrupt enable */
    dword      TXEIE :   1; /** 0x07 TXE interrupt enable */
    dword       PEIE :   1; /** 0x08 PE interrupt enable */

    dword         PS :   1; /** 0x09 Parity selection      */
    dword        PCE :   1; /** 0x0A Parity control enable */
    dword       WAKE :   1; /** 0x0B Wakeup method         */
    dword          M :   1; /** 0x0C Word length           */
    dword         UE :   1; /** 0x0D USART enable          */
    dword            :   1; /** 0x0E */
    dword      OVER8 :   1; /** 0x0F Oversampling mode */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */
};

union USART$CR2
{ struct
  { dword        ADD :   4; /** 0x00 Address of the USART node */
    dword            :   1; /** 0x04 */
    dword       LBDL :   1; /** 0x05 lin break detection length */
    dword      LBDIE :   1; /** 0x06 LIN break detection interrupt enable */
    dword            :   1; /** 0x07 */
    dword       LBCL :   1; /** 0x08 Last bit clock pulse */
    dword       CPHA :   1; /** 0x09 Clock phase */
    dword       CPOL :   1; /** 0x0A Clock polarity */
    dword      CLKEN :   1; /** 0x0B Clock enable */
    dword       STOP :   2; /** 0x0C STOP bits */
    dword      LINEN :   1; /** 0x0E LIN mode enable */
    dword            :  17; /** 0x0F                     */
  };

  dword atomic;            /** atomic access */
};

union USART$CR3
{ struct
  { dword        EIE :   1; /** 0x00 Error interrupt enable */
    dword       IREN :   1; /** 0x01 IrDA mode enable       */
    dword       IRLP :   1; /** 0x02 IrDA low-power         */
    dword      HDSEL :   1; /** 0x03 Half-duplex selection  */
    dword       NACK :   1; /** 0x04 Smartcard NACK enable  */
    dword       SCEN :   1; /** 0x05 Smartcard mode enable  */
    dword       DMAR :   1; /** 0x06 DMA enable receiver    */
    dword       DMAT :   1; /** 0x07 DMA enable transmitter */
    dword       RTSE :   1; /** 0x08 RTS enable             */
    dword       CTSE :   1; /** 0x09 CTS enable             */
    dword      CTSIE :   1; /** 0x0A CTS interrupt enable   */
    dword     ONEBIT :   1; /** 0x0B One sample bit method enable */
    dword            :  20; /** 0x0C */
  };

  dword atomic;            /** atomic access */
};

union USART$GTPR
{ struct
  { dword        PSC :   8; /** 0x00 Prescaler value */
    dword         GT :   8; /** 0x08 Guard time value */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */
};

struct  USART_STRUCT       /** 0x40011000 Universal synchronous asynchronous receiver transmitter */
{ union USART$SR      SR;  /** 0x00 RST: 0x00C00000 Status register */
  union USART$DR      DR;  /** 0x04 RST: 0x00000000 Data register */
  union USART$BRR    BRR;  /** 0x08 RST: 0x00000000 Baud rate register */
  union USART$CR1    CR1;  /** 0x0C RST: 0x00000000 Control register 1 */
  union USART$CR2    CR2;  /** 0x10 RST: 0x00000000 Control register 2 */
  union USART$CR3    CR3;  /** 0x14 RST: 0x00000000 Control register 3 */
  union USART$GTPR  GTPR;  /** 0x18 RST: 0x00000000 Guard time and prescaler register */

   dword          gap$1C[ 249 ];  /** 0x1C Unused memory */
};

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   FLASH : FLASH                                    @ 0x40023C00 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
struct       /** 0x40023C00 FLASH */
{ struct
  { dword    LATENCY :   3; /** 0x00 Latency */
    dword            :   5; /** 0x03 */
    dword     PRFTEN :   1; /** 0x08 Prefetch enable */
    dword       ICEN :   1; /** 0x09 Instruction cache enable */
    dword       DCEN :   1; /** 0x0A Data cache enable */
    dword      ICRST :   1; /** 0x0B Instruction cache reset */
    dword      DCRST :   1; /** 0x0C Data cache reset */
    dword            :  19; /** 0x0D */
  } ACR;                    /** 0x00 RST: 0x00000000 Flash access control register */

  dword      KEYR;       /** 0x04 RST: 0x00000000 Flash key register */
  dword      OPTKEYR;    /** 0x08 RST: 0x00000000 Flash option key register */

  struct
  { dword        EOP :   1; /** 0x00 End of operation */
    dword      OPERR :   1; /** 0x01 Operation error */
    dword            :   2; /** 0x02 */
    dword     WRPERR :   1; /** 0x04 Write protection error */
    dword     PGAERR :   1; /** 0x05 Programming alignment error */
    dword     PGPERR :   1; /** 0x06 Programming parallelism error */
    dword     PGSERR :   1; /** 0x07 Programming sequence error */
    dword            :   8; /** 0x08 */
    dword        BSY :   1; /** 0x10 Busy */
    dword            :  15; /** 0x11 */
  } SR;    /** 0x0C RST: 0x00000000 Status register */

  struct
  { dword         PG :   1; /** 0x00 Programming */
    dword        SER :   1; /** 0x01 Sector Erase */
    dword        MER :   1; /** 0x02 Mass Erase */
    dword        SNB :   4; /** 0x03 Sector number */
    dword            :   1; /** 0x07 */
    dword      PSIZE :   2; /** 0x08 Program size */
    dword            :   6; /** 0x0A */
    dword       STRT :   1; /** 0x10 Start */
    dword            :   7; /** 0x11 */
    dword      EOPIE :   1; /** 0x18 End of operation interrupt enable */
    dword      ERRIE :   1; /** 0x19 Error interrupt enable */
    dword            :   5; /** 0x1A */
    dword       LOCK :   1; /** 0x1F Lock */
  } CR;                     /** 0x10 RST: 0x7FFFFFFF Control register */

  struct
  { dword    OPTLOCK :   1; /** 0x00 Option lock */
    dword    OPTSTRT :   1; /** 0x01 Option start */
    dword    BOR_LEV :   2; /** 0x02 BOR reset Level */
    dword            :   1; /** 0x04 */
    dword     WDG_SW :   1; /** 0x05 WDG_SW User option bytes */
    dword  nRST_STOP :   1; /** 0x06 nRST_STOP User option bytes */
    dword nRST_STDBY :   1; /** 0x07 nRST_STDBY User option bytes */
    dword        RDP :   8; /** 0x08 Read protect */
    dword       nWRP :  12; /** 0x10 Not write protect */
    dword            :   4; /** 0x1C */
  } OPTCR;                  /** 0x14 RST: 0x00000014 Flash option control register */

   dword          gap$18[ 250 ];    /** 0x18 Unused memory */
} FLASH;

#define AF_OTG_FS  0x0A | 0x80
#define AF_OTG_HS  0x0C | 0x80


#endif  /* STM32G031_H_INCLUDED */
