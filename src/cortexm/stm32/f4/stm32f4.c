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
 *     FILE: STM32G031.c
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */


#include "stm32f4.h"

 /* Provides a default interrupt table
  */

SECTION( ".systemVectors" ) void * systemVectors[]=
{ WWDGirqHnd             /*  0 Window WatchDog              */
, PVDirqHnd              /*  1 PVD through EXTI Line detection */
, TAMP_STAMPirqHnd       /*  2 Tamper and TimeStamps through the EXTI line */
, RTC_WKUPirqHnd         /*  3 RTC Wakeup through the EXTI line */
, FLASHirqHnd            /*  4 FLASH                        */
, RCCirqHnd              /*  5 RCC                          */
, EXTI0irqHnd            /*  6 EXTI Line0                   */
, EXTI1irqHnd            /*  7 EXTI Line1                   */
, EXTI2irqHnd            /*  8 EXTI Line2                   */
, EXTI3irqHnd            /*  9 EXTI Line3                   */
, EXTI4irqHnd            /* 10 EXTI Line4                   */
, DMA1_0irqHnd           /* 11 DMA1 Stream 0                */
, DMA1_1irqHnd           /* 12 DMA1 Stream 1                */
, DMA1_2irqHnd           /* 13 DMA1 Stream 2                */
, DMA1_3irqHnd           /* 14 DMA1 Stream 3                */
, DMA1_4irqHnd           /* 15 DMA1 Stream 4                */
, DMA1_5irqHnd           /* 16 DMA1 Stream 5                */
, DMA1_6irqHnd           /* 17 DMA1 Stream 6                */
, ADCirqHnd              /* 18 ADC1, ADC2 and ADC3s         */
, irqHnd                 /* 19 Reserved                      */
, irqHnd              	  /* 20 Reserved                     */
, irqHnd                 /* 21 Reserved                     */
, irqHnd                 /* 22 Reserved                     */
, EXTI9_5irqHnd          /* 23 External Line[9:5]s          */
, TIM1_BRK_9irqHnd       /* 24 TIM1 Break and TIM9          */
, TIM1_UP_10irqHnd       /* 25 TIM1 Update and TIM10        */
, TIM1_TRG_11irqHnd      /* 26 TIM1 Trigger and Commutation and TIM11 */
, TIM1_CCirqHnd          /* 27 TIM1 Capture Compare         */
, TIM2irqHnd             /* 28 TIM2                         */
, TIM3irqHnd             /* 29 TIM3                         */
, TIM4irqHnd             /* 30 TIM4                         */
, I2C1_EVirqHnd          /* 31 I2C1 Event                   */
, I2C1_ERirqHnd          /* 32 I2C1 Error                   */
, I2C2_EVirqHnd          /* 33 I2C2 Event                   */
, I2C2_ERirqHnd          /* 34 I2C2 Error                   */
, SPI1irqHnd             /* 35 SPI1                         */
, SPI2irqHnd             /* 36 SPI2                         */
, USART1irqHnd           /* 37 USART1                       */
, USART2irqHnd           /* 38 USART2                       */
, irqHnd               	 /* 39 Reserved                       */
, EXTI15_10irqHnd        /* 40 External Line[15:10]s        */
, RTC_AlarmirqHnd        /* 41 RTC Alarm (A and B) through EXTI Line */
, OTG_FS_WKUPirqHnd      /* 42 USB OTG FS Wakeup through EXTI line */
, irqHnd                 /* 43 Reserved     				  */
, irqHnd                 /* 44 Reserved       			  */
, irqHnd                 /* 45 Reserved 					  */
, irqHnd                 /* 46 Reserved                     */
, DMA1_7irqHnd           /* 47 DMA1 Stream7                 */
, irqHnd                 /* 48 Reserved                     */
, SDIOirqHnd            /* 49 SDIO                         */
, TIM5irqHnd            /* 50 TIM5                         */
, SPI3irqHnd            /* 51 SPI3                         */
, irqHnd                /* 52 Reserved                     */
, irqHnd                /* 53 Reserved                     */
, irqHnd                /* 54 Reserved                     */
, irqHnd                /* 55 Reserved                     */
, DMA2_0irqHnd          /* 56 DMA2 Stream 0                */
, DMA2_1irqHnd          /* 57 DMA2 Stream 1                */
, DMA2_2irqHnd          /* 58 DMA2 Stream 2                */
, DMA2_3irqHnd          /* 59 DMA2 Stream 3                */
, DMA2_4irqHnd          /* 60 DMA2 Stream 4                */
, irqHnd                /* 61 Reserved                     */
, irqHnd              	 /* 62 Reserved                     */
, irqHnd              	 /* 63 Reserved                     */
, irqHnd             		 /* 64 Reserved                     */
, irqHnd              	 /* 65 Reserved                     */
, irqHnd              	 /* 66 Reserved                     */
, OTG_FSirqHnd          /* 67 USB OTG FS                   */
, DMA2_5irqHnd          /* 68 DMA2 Stream 5                */
, DMA2_6irqHnd          /* 69 DMA2 Stream 6                */
, DMA2_7irqHnd          /* 70 DMA2 Stream 7                */
, USART6irqHnd          /* 71 USART6                       */
, I2C3_EVirqHnd         /* 72 I2C3 event                   */
, I2C3_ERirqHnd         /* 73 I2C3 error                   */
, OTG_HSep1OUTirqHnd    /* 74 OTG HS End Point 1 Out global interrupt */
, OTG_HSep1INirqHnd     /* 75 OTG HS End Point 1 In global interrupt  */
, OTG_HSWakeUPirqHnd    /* 76 OTG HS Wakeup through EXTI interrupt    */
, OTG_HSirqHnd          /* 77 OTG HS global interrupt                 */
, irqHnd                /* 79 Reserved                     */
, irqHnd                /* 80 Reserved                     */
, FPUirqHnd             /* 81 FPU                          */
, irqHnd                /* 82 Reserved                     */
, irqHnd                /* 83 Reserved                     */
, SPI4irqHnd            /* 84 SPI4                         */
};


volatile extern struct
{ struct                    /* 0x00 */
  { dword      HSION :   1; /** 0x00 Internal high-speed clock enable */
    dword     HSIRDY :   1; /** 0x01 Internal high-speed clock ready flag */
    dword            :   1; /** 0x02 */
    dword    HSITRIM :   5; /** 0x03 Internal high-speed clock trimming */
    dword     HSICAL :   8; /** 0x08 Internal high-speed clock calibration */
    dword      HSEON :   1; /** 0x10 HSE clock enable */
    dword     HSERDY :   1; /** 0x11 HSE clock ready flag */
    dword     HSEBYP :   1; /** 0x12 HSE clock bypass */
    dword      CSSON :   1; /** 0x13 Clock security system enable */
    dword            :   4; /** 0x14 */
    dword      PLLON :   1; /** 0x18 Main PLL (PLL) enable */
    dword     PLLRDY :   1; /** 0x19 Main PLL (PLL) clock ready flag */
    dword   PLLI2SON :   1; /** 0x1A PLLI2S enable */
    dword  PLLI2SRDY :   1; /** 0x1B PLLI2S clock ready flag */
    dword            :   4; /** 0x1C */
  } CR;

  struct                  /* 0x04 */
  { dword      PLLM :  6; /** 0x00 Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock */
    dword      PLLN :  9; /** 0x06 Main PLL (PLL) multiplication factor for VCO */
    dword           :  1; /** 0x0F */
    dword      PLLP :  2; /** 0x10 Main PLL (PLL) division factor for main system clock */
    dword           :  4; /** 0x12 */
    dword     PLLSRC:  1; /** 0x16 Main PLL(PLL) and audio PLL (PLLI2S) entry clock source */
    dword           :  1; /** 0x17 */
    dword      PLLQ0:  4; /** 0x18 Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks */
    dword           :  4; /** 0x1C */
  } PLLCFRG;

  struct                    /* 0x08 */
  { dword         SW : 2; /** 0x00 System clock switch */
    dword        SWS : 2; /** 0x02 System clock switch status */
    dword       HPRE : 4; /** 0x04 AHB prescaler */
    dword            : 2; /** 0x08 */
    dword      PPRE1 : 3; /** 0x0A APB Low speed prescaler (APB1) */
    dword      PPRE2 : 3; /** 0x0D APB high-speed prescaler (APB2) */
    dword     RTCPRE : 5; /** 0x10 HSE division factor for RTC clock */
    dword       MCO1 : 2; /** 0x15 Microcontroller clock output 1 */
    dword     I2SSRC : 1; /** 0x17 I2S clock selection */
    dword    MCO1PRE : 3; /** 0x18 MCO1 prescaler */
    dword    MCO2PRE : 3; /** 0x1B MCO2 prescaler */
    dword       MCO2 : 2; /** 0x1E Microcontroller clock output 2 */
  } CFGR;

  dword         CIR;        /** 0x0C RST: 0x00000000 clock interrupt register */
  dword        RSTR[ 8 ];   /** 0x10 RST: AHB1 peripheral reset register */
  dword         ENR[ 8 ];   /** 0x30 RST: AHB1 peripheral clock register */
  dword       LPENR[ 8 ];   /** 0x50 RST: AHB1 peripheral clock enable in low power mode register */


} RCC;


#define RCC_IOPRSTR   9
#define RCC_IOPENR   13
#define RCC_IOPSMENR 17

/* ------------------------------------------------------------------------- */
   short DEVICE_RESET( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  RCC.RSTR[ device >> 8 ] |=  ( 1 << device & 0xFF );
  RCC.RSTR[ device >> 8 ] &= ~( 1 << device & 0xFF );

  return( 0 );
}


/* ------------------------------------------------------------------------- */
   short DEVICE_ENABLE( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  RCC.ENR[ device >> 8  ] |= ( 1 << ( device & 0x1F ));

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short DEVICE_DISABLE( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  RCC.ENR[ device >> 8  ] &= ~( 1 << ( device & 0x1F  ));

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short DEVICE_STATUS( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  return(( RCC.ENR[ device >> 8  ] | ( 1 << (device & 0x1F ))) ? 1 : 0 );
}

/* ------------------------------------------------------------------------- */
   short SLEEP_ENABLE( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  RCC.LPENR[ device >> 8  ] |= ( 1 << ( device & 0x1F) );

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short SLEEP_DISABLE( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  RCC.LPENR[ device >> 8  ] &= ~( 1 << (device & 0x1F ) );

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short SLEEP_STATUS( dword device )
/* ------------------------------------------------------------------------- */
{ device &= 0xFFFF;

  return(( RCC.LPENR[ device >> 8  ]  | ( 1 << (device & 0x1F) ) ) ? 1 : 0 );
}



/* GPIO related
 */

#define MODE_INP  0x0
#define MODE_OUT  0x1
#define MODE_ALT  0x2
#define MODE_ANA  0x3

#define OUT_PP    0x0
#define OUT_DRAIN 0x1

#define CLIP_LOW  0x0
#define CLIP_FAIR 0x1
#define CLIP_LOW1 0x2
#define CLIP_HIGH 0x3

#define PULL_NO   0x0
#define PULL_UP   0x1
#define PULL_DN   0x2
#define PULL_RES  0x3

volatile extern struct           /** 0x50000000 General-purpose I/Os */
{ dword MODER        ;  /** 0x00 RST: 0x7FFFFFFF GPIO port mode register */
  dword OTYPER       ;  /** 0x04 RST: 0x00000000 GPIO port output type register */
  dword OSPEEDR      ;  /** 0x08 RST: 0x0C000000 GPIO port output speed register */
  dword PUPDR        ;  /** 0x0C RST: 0x24000000 GPIO port pull-up/pull-down register */
  dword IDR          ;  /** 0x10 RST: 0x00000000 GPIO port input data register */
  dword ODR          ;  /** 0x14 RST: 0x00000000 GPIO port output data register */
  dword BSRR         ;  /** 0x18 RST: 0x00000000 GPIO port bit set/reset register */
  dword LCKR         ;  /** 0x1C RST: 0x00000000 GPIO port configuration lock register */
  dword AFRL         ;  /** 0x20 RST: 0x00000000 GPIO alternate function low register */
  dword AFRH         ;  /** 0x24 RST: 0x00000000 GPIO alternate function high register */
  dword BRR          ;  /** 0x28 RST: 0x00000000 port bit reset register */
  dword gap$2C[ 245 ];  /** 0x2C Unused memory */
} GPIOA[];

/* Must be accessed as inline macros
 */

ABSOLUTE( GPIOA    , 0x40020000 + 0x0000 );
ABSOLUTE( GPIO_IDR , 0x40020000 + 0x0010 );
ABSOLUTE( GPIO_ODR , 0x40020000 + 0x0014 );
ABSOLUTE( GPIO_BSRR, 0x40020000 + 0x0018 );
ABSOLUTE( RCC      , 0x40020000 + 0x3800 );

/* ------------------------------------------------------------------------- */
   word getMode( word mode )
/* ------------------------------------------------------------------------- */
{ if ( mode & 0x80 )                                  // Alternate, strip others
  { return( mode & 0x40 ? MODE_ANA : MODE_ALT );
  }

  return( mode & GPIO_OUT ? MODE_OUT : MODE_INP );   // Output mandatory
}

/* ------------------------------------------------------------------------- */
   word getType( word mode )
/* ------------------------------------------------------------------------- */
{ return( mode & GPIO_OD ? OUT_DRAIN : OUT_PP  );
}

/* ------------------------------------------------------------------------- */
   word getClip( word mode )
/* ------------------------------------------------------------------------- */
{ switch( mode & GPIO_FAST )
  { case GPIO_SLOW: return( CLIP_LOW  );
    case GPIO_CRUI: return( CLIP_FAIR );
    case GPIO_HIGH: return( CLIP_HIGH );
    case GPIO_FAST: return( CLIP_HIGH );
  }
  return( 0 );
}

/* ------------------------------------------------------------------------- */
   word getPull( word mode )
/* ------------------------------------------------------------------------- */
{ switch( mode & GPIO_INOUT )
  { case GPIO_FLOAT : return( PULL_NO  );
    case GPIO_PULLUP: return( PULL_UP  );
    case GPIO_PULLDN: return( PULL_DN  );
    case GPIO_INOUT : return( PULL_RES );
  }
  return( 0 );
}

/* ------------------------------------------------------------------------- */
   int gpioEnable( int idx )
/* ------------------------------------------------------------------------- */
{ idx &= 0x0F;

  DEVICE_ENABLE( idx + RCC_GPIOA );
  return( idx );
}

/* ------------------------------------------------------------------------- */
   short PORT_MODE( int idx, word mode, word pins )
/* ------------------------------------------------------------------------- */
{ word MODE= getMode( mode );
  word TYPE= getType( mode );
  word CLIP= getClip( mode );
  word PULL= getPull( mode );
  word ALTR= mode & 0x0F;

  mode= gpioEnable( idx );

  for( idx= 0
     ; pins
     ; idx ++  )
  { if ( pins & 1 )
    { BIT2SET( &GPIOA[ mode ].MODER  , idx, MODE );
      BIT1SET( &GPIOA[ mode ].OTYPER , idx, TYPE );
      BIT2SET( &GPIOA[ mode ].OSPEEDR, idx, CLIP );
      BIT2SET( &GPIOA[ mode ].PUPDR  , idx, PULL );
      BIT4SET( &GPIOA[ mode ].AFRL   , idx, ALTR );
    }
    pins >>= 1;
  }

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short PIN_MODE( word idx, word mode )
/* ------------------------------------------------------------------------- */
{ word MODE= getMode( mode );
  word TYPE= getType( mode );
  word CLIP= getClip( mode );
  word PULL= getPull( mode );
  word ALTR= mode & 0x0F;

  mode= gpioEnable( idx >> 8 );   /* Extract port */
  idx &= 0xFF;                    /* Extract pin  */

  BIT2SET( &GPIOA[ mode ].MODER  , idx, MODE );
  BIT1SET( &GPIOA[ mode ].OTYPER , idx, TYPE );
  BIT2SET( &GPIOA[ mode ].OSPEEDR, idx, CLIP );
  BIT2SET( &GPIOA[ mode ].PUPDR  , idx, PULL );
  BIT4SET( &GPIOA[ mode ].AFRL   , idx, ALTR );

  return( 0 );
}


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   PWR : Power control                            @ 0x40007000 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
union PWR$CR
{ struct
  { dword       LPDS :   1; /** 0x00 Low-power deep sleep */
    dword       PDDS :   1; /** 0x01 Power down deepsleep */
    dword       CWUF :   1; /** 0x02 Clear wakeup flag */
    dword       CSBF :   1; /** 0x03 Clear standby flag */
    dword       PVDE :   1; /** 0x04 Power voltage detector enable */
    dword        PLS :   3; /** 0x05 PVD level selection */
    dword        DBP :   1; /** 0x08 Disable backup domain write protection */
    dword       FPDS :   1; /** 0x09 Flash power down in Stop mode */
    dword            :   3; /** 0x0A */
    dword     ADCDC1 :   1; /** 0x0D ADCDC1 */
    dword        VOS :   2; /** 0x0E Regulator voltage scaling output selection */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */
};

union PWR$CSR
{ struct
  { dword        WUF :   1; /** 0x00 Wakeup flag */
    dword        SBF :   1; /** 0x01 Standby flag */
    dword       PVDO :   1; /** 0x02 PVD output */
    dword        BRR :   1; /** 0x03 Backup regulator ready */
    dword            :   4; /** 0x04 */
    dword       EWUP :   1; /** 0x08 Enable WKUP pin */
    dword        BRE :   1; /** 0x09 Backup regulator enable */
    dword            :   4; /** 0x0A */
    dword     VOSRDY :   1; /** 0x0E Regulator voltage scaling output selection ready bit */
    dword            :  17; /** 0x0F */
  };

  dword atomic;            /** atomic access */
};

struct                 /** 0x40007000 Power control */
{ union PWR$CR  CR;    /** 0x00 RST: 0x00000000 power control register */
  union PWR$CSR CSR;   /** 0x04 RST: 0x00000000 power control/status register */

   dword          gap$08[ 254 ];    /** 0x08 Unused memory */
} PWR;

/*            PLL (clocked by HSE) used as System clock source                */

#define  SWS_HSI 0     /*!< HSI oscillator used as system clock */
#define  SWS_HSE 1     /*!< HSE oscillator used as system clock */
#define  SWS_PLL 2     /*!< PLL/PLLP used as system clock       */

#define HSE_STARTUP_TIMEOUT 0x05000   /*!< Time out for HSE start up */

static dword XTAL= 25;

/** ------------------------------------------------------------------------- */
    dword SET_SYSCLK_HZ( dword hz, dword xtal )
/** ------------------------------------------------------------------------- */
{ volatile dword StartUpCounter= 0;

  RCC.CR.HSEON= 1;  /* Enable HSE */

  do /* Wait till HSE is ready and if Time out is reached exit */
  { StartUpCounter++;
  } while( !RCC.CR.HSERDY && ( StartUpCounter < HSE_STARTUP_TIMEOUT ));

  if ( RCC.CR.HSERDY ) /* Enable high performance mode, System frequency up to 168 MHz */
  { DEVICE_ENABLE( RCC_PWR );
    PWR.CR.VOS= 1;               /** 0x0E Regulator voltage scaling output selection */

    RCC.CFGR.HPRE = 1;  /* HCLK = SYSCLK / 1*/
    RCC.CFGR.PPRE2= 2;  /** 0x0D APB high-speed prescaler (APB2) */
    RCC.CFGR.PPRE1= 4;  /** 0x0A APB Low speed prescaler  (APB1) */

/* Configure the main PLL
 */

    hz= ( 336000000 >> 1 ) / hz; hz --; /* xlate */

    RCC.PLLCFRG.PLLSRC= 1;      /* XTAL */
    RCC.PLLCFRG.PLLM  = XTAL= xtal / 1000000; /* same as 25 Mhz XTAL */ /** 0x00 Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock */
    RCC.PLLCFRG.PLLN  = 336; /* Fixed, 1MHZ abobe   */

    RCC.PLLCFRG.PLLP  =  hz; //( (4) / 2 ) - 1 + 2;  /* 84 / 48 -> 7/4 */ /** 0x10 Main PLL (PLL) division factor for main system clock */
    RCC.PLLCFRG.PLLQ0 =  7       ; /** 0x18 Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks */

    RCC.CR.PLLON= 1; while( !RCC.CR.PLLRDY );  /* Enable the main PLL, Wait till the main PLL is ready */

/* Configure Flash prefetch, Instruction cache, Data cache and wait state
 */
 //   FLASH.ACR.PRFTEN = 1;
 //   FLASH.ACR.ICEN   = 1;
  //  FLASH.ACR.DCEN   = 1;       // wait states
 //   FLASH.ACR.LATENCY= 2;       // wait states
 //   FLASH.ACR.ICEN= 1; FLASH.ACR.DCEN= 1; FLASH.ACR_LATENCY= 5;

    RCC.CFGR.SW= SWS_PLL;              /* Select the main PLL as system clock source */
    while ( RCC.CFGR.SWS != SWS_PLL ); /* Wait till the main PLL is used as system clock source */
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }

/******************************************************************************/
/*                        I2S clock configuration                             */
/******************************************************************************/

//  RCC->CFGR &= ~RCC_CFGR_I2SSRC;  /* PLLI2S clock used as I2S clock source */
 // RCC->PLLI2S CFGR = (PLLI2S_N << 6) | (PLLI2S_R << 28);   /* Configure PLLI2S */

 // DEVICE_ENABLE( RCC_I2S );   /* Enable PLLI2S */

//  while((RCC->CR & RCC_CR_PLLI2SRDY) == 0)   /* Wait till PLLI2S is ready */
//  {
//  }

  return( 0 );
}




/** ------------------------------------------------------------------------- */
   dword SYSCLK_HZ( byte clk )
/** ------------------------------------------------------------------------- */
{ static const byte AHBPrescTable[] =
  { 0, 0, 0, 0
  , 0, 0, 0, 0
  , 1, 2, 3, 4
  , 6, 7, 8, 9 };

  dword SystemCoreClock= RCC.CFGR.SWS;
  dword UsbCoreClock= 0;

  switch ( RCC.CFGR.SWS )
  { case SWS_HSI: SystemCoreClock= HSI_VALUE * 1000000; break; /* HSI used as system clock source */
    case SWS_HSE: SystemCoreClock= XTAL      * 1000000; break; /* HSE used as system clock source */

    case SWS_PLL:                  /* PLL used as system clock source */
      if ( RCC.PLLCFRG.PLLSRC )    /* HSE used as PLL clock source */
      { SystemCoreClock= XTAL     ;
      }
      else                       /* HSI used as PLL clock source */
      { SystemCoreClock= HSI_VALUE;
      }

      SystemCoreClock *= 1000;              /* fixed point */
      SystemCoreClock *=   RCC.PLLCFRG.PLLN;
      SystemCoreClock /= RCC.PLLCFRG.PLLM;
      SystemCoreClock *= 1000;

      UsbCoreClock= SystemCoreClock / RCC.PLLCFRG.PLLQ0;
      SystemCoreClock /= ( RCC.PLLCFRG.PLLP + 1 ) * 2;
    break;

    default: SystemCoreClock= 0;
  }

  SystemCoreClock >>= AHBPrescTable[ RCC.CFGR.HPRE >> 4 ];

  switch( clk )
  { case SYSDEV_HZ: return( SystemCoreClock );
    case USBDEV_HZ: return(    UsbCoreClock );
  }

  return( 0 );
}

/** ---------------------------------------------------------------------------------- */

union TIM$CR1
{ struct
  { dword        CEN :   1; /** 0x00 Counter enable */
    dword       UDIS :   1; /** 0x01 Update disable */
    dword        URS :   1; /** 0x02 Update request source */
    dword        OPM :   1; /** 0x03 One-pulse mode */
    dword        DIR :   1; /** 0x04 Direction */
    dword        CMS :   2; /** 0x05 Center-aligned mode selection */
    dword       ARPE :   1; /** 0x07 Auto-reload preload enable */
    dword        CKD :   2; /** 0x08 Clock division */
    dword            :  22; /** 0x0A */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$CR2
{ struct
  { dword       CCPC :   1; /** 0x00 Capture/compare preloaded control */
    dword            :   1; /** 0x01 */
    dword       CCUS :   1; /** 0x02 Capture/compare control update selection */
    dword       CCDS :   1; /** 0x03 Capture/compare DMA selection */
    dword        MMS :   3; /** 0x04 Master mode selection */
    dword       TI1S :   1; /** 0x07 TI1 selection */
    dword       OIS1 :   1; /** 0x08 Output Idle state 1 */
    dword      OIS1N :   1; /** 0x09 Output Idle state 1 */
    dword       OIS2 :   1; /** 0x0A Output Idle state 2 */
    dword      OIS2N :   1; /** 0x0B Output Idle state 2 */
    dword       OIS3 :   1; /** 0x0C Output Idle state 3 */
    dword      OIS3N :   1; /** 0x0D Output Idle state 3 */
    dword       OIS4 :   1; /** 0x0E Output Idle state 4 */
    dword            :  17; /** 0x0F */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$SMCR
{ struct
  { dword        SMS :   3; /** 0x00 Slave mode selection */
    dword            :   1; /** 0x03 */
    dword         TS :   3; /** 0x04 Trigger selection */
    dword        MSM :   1; /** 0x07 Master/Slave mode */
    dword        ETF :   4; /** 0x08 External trigger filter */
    dword       ETPS :   2; /** 0x0C External trigger prescaler */
    dword        ECE :   1; /** 0x0E External clock enable */
    dword        ETP :   1; /** 0x0F External trigger polarity */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

struct TIM$DIER
{ dword        UIE :   1; /** 0x00 Update interrupt enable */
  dword      CC1IE :   1; /** 0x01 Capture/Compare 1 interrupt enable */
  dword      CC2IE :   1; /** 0x02 Capture/Compare 2 interrupt enable */
  dword      CC3IE :   1; /** 0x03 Capture/Compare 3 interrupt enable */
  dword      CC4IE :   1; /** 0x04 Capture/Compare 4 interrupt enable */
  dword      COMIE :   1; /** 0x05 COM interrupt enable */
  dword        TIE :   1; /** 0x06 Trigger interrupt enable */
  dword        BIE :   1; /** 0x07 Break interrupt enable */
  dword        UDE :   1; /** 0x08 Update DMA request enable */
  dword      CC1DE :   1; /** 0x09 Capture/Compare 1 DMA request enable */
  dword      CC2DE :   1; /** 0x0A Capture/Compare 2 DMA request enable */
  dword      CC3DE :   1; /** 0x0B Capture/Compare 3 DMA request enable */
  dword      CC4DE :   1; /** 0x0C Capture/Compare 4 DMA request enable */
  dword      COMDE :   1; /** 0x0D COM DMA request enable */
  dword        TDE :   1; /** 0x0E Trigger DMA request enable */
  dword            :  17; /** 0x0F */
};

struct TIM$EGR
{ dword         UG :   1; /** 0x00 Update generation */
  dword       CC1G :   1; /** 0x01 Capture/compare 1 generation */
  dword       CC2G :   1; /** 0x02 Capture/compare 2 generation */
  dword       CC3G :   1; /** 0x03 Capture/compare 3 generation */
  dword       CC4G :   1; /** 0x04 Capture/compare 4 generation */
  dword       COMG :   1; /** 0x05 Capture/Compare control update generation */
  dword         TG :   1; /** 0x06 Trigger generation */
  dword         BG :   1; /** 0x07 Break generation */
  dword            :  24; /** 0x08 */
};

struct TIM$CCMR_OUT
{ byte       CCS :   2; /** 0x00 Capture/Compare 1 selection */
  byte      OCFE :   1; /** 0x02 Output Compare 1 fast enable */
  byte      OCPE :   1; /** 0x03 Output Compare 1 preload enable */
  byte       OCM :   3; /** 0x04 Output Compare 1 mode */
  byte      OCCE :   1; /** 0x07 Output Compare 1 clear enable */
};

struct TIM$CCMR_IN
{ byte       CCS :   2; /** 0x00 Capture/Compare 1 selection */
  byte      ICPCS:   2; /** 0x02 Input capture 1 prescaler */
  byte       ICF :   4; /** 0x04 Input capture 1 filter */
};


struct TIM$CCER
{ dword       CC1E :   1; /** 0x00 Capture/Compare 1 output enable */
  dword       CC1P :   1; /** 0x01 Capture/Compare 1 output Polarity */
  dword      CC1NE :   1; /** 0x02 Capture/Compare 1 complementary output enable */
  dword      CC1NP :   1; /** 0x03 Capture/Compare 1 output Polarity */
  dword       CC2E :   1; /** 0x04 Capture/Compare 2 output enable */
  dword       CC2P :   1; /** 0x05 Capture/Compare 2 output Polarity */
  dword      CC2NE :   1; /** 0x06 Capture/Compare 2 complementary output enable */
  dword      CC2NP :   1; /** 0x07 Capture/Compare 2 output Polarity */
  dword       CC3E :   1; /** 0x08 Capture/Compare 3 output enable */
  dword       CC3P :   1; /** 0x09 Capture/Compare 3 output Polarity */
  dword      CC3NE :   1; /** 0x0A Capture/Compare 3 complementary output enable */
  dword      CC3NP :   1; /** 0x0B Capture/Compare 3 output Polarity */
  dword       CC4E :   1; /** 0x0C Capture/Compare 4 output enable */
  dword       CC4P :   1; /** 0x0D Capture/Compare 3 output Polarity */
  dword            :  18; /** 0x0E */
};

union TIM$CNT
{ struct
  { dword        CNT :  16; /** 0x00 counter value */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$PSC
{ struct
  { dword        PSC :  16; /** 0x00 Prescaler value */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$ARR
{ struct
  { dword        ARR :  32; /** 0x00 Auto-reload value */
//    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$CCR
{ struct
  { dword  CCR:  16; /** 0x00 Capture/Compare 1 value */
    dword     :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$DCR
{ struct
  { dword        DBA :   5; /** 0x00 DMA base address */
    dword            :   3; /** 0x05 */
    dword        DBL :   5; /** 0x08 DMA burst length */
    dword            :  19; /** 0x0D */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$DMAR
{ struct
  { dword       DMAB :  16; /** 0x00 DMA register for burst accesses */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$RCR
{ struct
  { dword        REP :   8; /** 0x00 Repetition counter value */
    dword            :  24; /** 0x08 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

union TIM$BDTR
{ struct
  { dword        DTG :   8; /** 0x00 Dead-time generator setup */
    dword       LOCK :   2; /** 0x08 Lock configuration */
    dword       OSSI :   1; /** 0x0A Off-state selection for Idle mode */
    dword       OSSR :   1; /** 0x0B Off-state selection for Run mode */
    dword        BKE :   1; /** 0x0C Break enable */
    dword        BKP :   1; /** 0x0D Break polarity */
    dword        AOE :   1; /** 0x0E Automatic output enable */
    dword        MOE :   1; /** 0x0F Main output enable */
    dword            :  16; /** 0x10 */
  };

  dword atomic;            /** atomic access */

  dword asDarr[ 1 ];       /** 32 bit access */
  word  asWarr[ 2 ];       /** 16 bit access */
  byte  asBarr[ 4 ];       /**  8 bit access */
};

struct  TIM_STRUCT        /** 0x40010000 Advanced-timers */
{ volatile union  TIM$CR1    CR1;  /** 0x00 RST: 0x00000000 control register 1 */
  volatile union  TIM$CR2    CR2;  /** 0x04 RST: 0x00000000 control register 2 */
  volatile union  TIM$SMCR  SMCR;  /** 0x08 RST: 0x00000000 slave mode control register */
  volatile struct TIM$DIER  DIER;  /** 0x0C RST: 0x00000000 DMA/Interrupt enable register */
  volatile struct TIM$SR      SR;  /** 0x10 RST: 0x00000000 status register */
  volatile struct TIM$EGR    EGR;  /** 0x14 RST: 0x00000000 event generation register */

  volatile union                     /* shared register */
  { struct TIM$CCMR_IN  IN;    /** 0x18 .. 0X0C RST: 0x00000000 capture/compare mode register 1 (input mode) */
    struct TIM$CCMR_OUT OUT;   /** 0x18 .. 0X0C RST: 0x00000000 capture/compare mode register 1 (output mode) */
  } CCMR[ 8 ];

  volatile struct TIM$CCER  CCER;      /** 0x20 RST: 0x00000000 capture/compare enable register */
  volatile union TIM$CNT    CNT;      /** 0x24 RST: 0x00000000 counter */
  volatile union TIM$PSC    PSC;      /** 0x28 RST: 0x00000000 prescaler */
  volatile union TIM$ARR    ARR;      /** 0x2C RST: 0x00000000 auto-reload register */
  volatile union TIM$RCR    RCR;      /** 0x30 RST: 0x00000000 repetition counter register */
  volatile union TIM$CCR    CCR[ 4 ]; /** 0x34 RST: 0x00000000 capture/compare register 1 */
  volatile union TIM$BDTR  BDTR;      /** 0x44 RST: 0x00000000 break and dead-time register */
  volatile union TIM$DCR    DCR;      /** 0x48 RST: 0x00000000 DMA control register */
  volatile union TIM$DMAR  DMAR;      /** 0x4C RST: 0x00000000 DMA address for full transfer */

  dword          gap$50[ 236 ];    /** 0x50 Unused memory */
};

#define TIMADV 7
#define TIM32B 3
#define TIM16B 1
#define TIMNUL 0


extern struct TIM_STRUCT TIM1;   /* 40 Advanced-timers        */
extern struct TIM_STRUCT TIM2;   /* 0 General purpose timers  */
extern struct TIM_STRUCT TIM3;   /* 1 General purpose timers  */
extern struct TIM_STRUCT TIM4;   /* 2 unknown                 */
extern struct TIM_STRUCT TIM5;   /* 3 General-purpose-timers  */
extern struct TIM_STRUCT TIM8;   /* 41 unknown                */
extern struct TIM_STRUCT TIM9;   /* 50 General purpose timers */
extern struct TIM_STRUCT TIM10;  /* 51 General-purpose-timers */
extern struct TIM_STRUCT TIM11;  /* 52 General-purpose-timers */

static struct
{ struct TIM_STRUCT * idx;
  int    flags;
  int    RCC;
  void * irqFun;
} TIMERS[]=
{{      0, TIMNUL, 0 }
,{  &TIM1, TIMADV, RCC_TIM1 , 0                 } /* 1 Advanced-timers */
,{  &TIM2, TIM32B, RCC_TIM2 , TIM2irqHnd        } /* 2 General purpose timers */
,{  &TIM3, TIM32B, RCC_TIM3 , TIM3irqHnd        } /* 3 General purpose timers */
,{  &TIM4, TIM32B, RCC_TIM4 , TIM4irqHnd        } /* 4 unknown */
,{  &TIM5, TIM32B, RCC_TIM5 , TIM5irqHnd        } /* 5 General-purpose-timers */

,{      0, TIMNUL, 0        , 0                 } /* 6 */
,{      0, TIMNUL, 0        , 0                 } /* 7 */
,{      0, TIMNUL, 0        , 0                 } /* 8 */

,{  &TIM9, TIM16B, RCC_TIM9 , TIM1_BRK_9irqHnd  } /* 50 General purpose timers */
,{ &TIM10, TIM16B, RCC_TIM10, TIM1_UP_10irqHnd  } /* 51 General-purpose-timers */
,{ &TIM11, TIM16B, RCC_TIM11, TIM1_TRG_11irqHnd } /* 52 General-purpose-timers */
};


/* Interrupt handlers needs this register
 */

ABSOLUTE( TIM2_SR , 0x40000000 + 0x10 );  /* 0 General purpose timers */
ABSOLUTE( TIM3_SR , 0x40000400 + 0x10 );  /* 1 General purpose timers */
ABSOLUTE( TIM4_SR , 0x40000800 + 0x10 );
ABSOLUTE( TIM5_SR , 0x40000C00 + 0x10 );  /* 3 General-purpose-timers */
ABSOLUTE( TIM1_SR , 0x40010000 + 0x10 );  /* 40 Advanced-timers */
ABSOLUTE( TIM8_SR , 0x40010400 + 0x10 );
ABSOLUTE( TIM9_SR , 0x40014000 + 0x10 );  /* 50 General purpose timers */
ABSOLUTE( TIM10_SR, 0x40014400 + 0x10 );  /* 51 General-purpose-timers */
ABSOLUTE( TIM11_SR, 0x40014800 + 0x10 );  /* 52 General-purpose-timers */


/* ------------------------------------------------------------------------- */
   short CAPCOM_TIMER( dword mangle, dword period  )
/* ------------------------------------------------------------------------- */
{ volatile struct TIM_STRUCT * TIM= TIMERS[ mangle & 0xFF  ].idx;

  if ( TIM )
  { TIM->CCR[ 0 ].CCR= 0; /** 0x00 16 Capture/Compare 1 value */
    TIM->CCR[ 1 ].CCR= 0; /** 0x00 16 Capture/Compare 1 value */
    TIM->CCR[ 2 ].CCR= 0; /** 0x00 16 Capture/Compare 1 value */
    TIM->CCR[ 3 ].CCR= 0; /** 0x00 16 Capture/Compare 1 value */

    TIM->CR2.CCPC  =  0; /** 0x00 Capture/compare preloaded control */
    TIM->CR2.CCUS  =  0; /** 0x02 Capture/compare control update selection */
    TIM->CR2.CCDS  =  0; /** 0x03 Capture/compare DMA selection */
    TIM->CR2.MMS   =  0; /** 0x04 3 Master mode selection */
    TIM->CR2.TI1S  =  0; /** 0x07 TI1 selection 1 > XOR combination */
    TIM->CR2.OIS1  =  0; /** 0x08 Output Idle state 1 */
    TIM->CR2.OIS1N =  0; /** 0x09 Output Idle state 1 */
    TIM->CR2.OIS2  =  0; /** 0x0A Output Idle state 2 */
    TIM->CR2.OIS2N =  0; /** 0x0B Output Idle state 2 */
    TIM->CR2.OIS3  =  0; /** 0x0C Output Idle state 3 */
    TIM->CR2.OIS3N =  0; /** 0x0D Output Idle state 3 */
    TIM->CR2.OIS4  =  0; /** 0x0E Output Idle state 4 */

    TIM->EGR.CC1G  =  0; /** 0x01 Capture/compare 1 generation */
    TIM->EGR.CC2G  =  0; /** 0x02 Capture/compare 2 generation */
    TIM->EGR.CC3G  =  0; /** 0x03 Capture/compare 3 generation */
    TIM->EGR.CC4G  =  0; /** 0x04 Capture/compare 4 generation */
    TIM->EGR.COMG  =  0; /** 0x05 Capture/Compare control update generation */

    return( 0 );
  }

  return( -1 );
}

/* ------------------------------------------------------------------------- */
   short DMA_TIMER( dword mangle, dword period  )
/* ------------------------------------------------------------------------- */
{ volatile struct TIM_STRUCT * TIM= TIMERS[ mangle & 0xFF  ].idx;

  if ( TIM )
  { TIM->DMAR.DMAB = 0; /** 0x00 16 DMA register for burst accesses */
    TIM->DCR.DBA   = 0; /** 0x00 5 DMA base address */
    TIM->DCR.DBL   = 0; /** 0x08 5 DMA burst length */


    return( 0 );
  }

  return( -1 );
}

/* ------------------------------------------------------------------------- */
   short TRIGGER_TIMER( dword mangle, dword period  )
/* ------------------------------------------------------------------------- */
{ volatile struct TIM_STRUCT * TIM= TIMERS[ mangle & 0xFF  ].idx;

  if ( TIM )
  { TIM->SMCR.SMS  =  0; /** 0x00 Slave mode selection       */
    TIM->SMCR.TS   =  0; /** 0x04 Trigger selection          */
    TIM->SMCR.MSM  =  0; /** 0x07 Master/Slave mode          */
    TIM->SMCR.ETF  =  0; /** 0x08 External trigger filter    */
    TIM->SMCR.ETPS =  0; /** 0x0C External trigger prescaler */
    TIM->SMCR.ECE  =  0; /** 0x0E External clock enable */
    TIM->SMCR.ETP  =  0; /** 0x0F External trigger polarity */

    TIM->EGR.TG    =  0; /** 0x06 Trigger generation */
    TIM->EGR.BG    =  0; /** 0x07 Break generation */

    TIM->BDTR.BKE  = 0; /** 0x0C Break enable               */
    TIM->BDTR.BKP  = 0; /** 0x0D Break polarity             */

    return( 0 );
  }

  return( -1 );
}

/* ------------------------------------------------------------------------- */
   short CONFIG_TIMER( dword mangle, dword period  )
/* ------------------------------------------------------------------------- */
{ volatile struct TIM_STRUCT * TIM= TIMERS[ mangle & 0xFF  ].idx;

  if ( TIM )
  { DEVICE_RESET(  TIMERS[ mangle & 0xFF ].RCC );
    DEVICE_ENABLE( TIMERS[ mangle & 0xFF ].RCC );

    TIM->PSC.PSC   = 1000;  /** 0x00 16 Prescaler value */
    TIM->ARR.ARR   = period; /** 0x00 16 Auto-reload value */

//    TIM->RCR.REP   = 0; /** 0x00 8 Repetition counter value */
//    TIM->CNT.CNT   = 0; /** 0x00 16 counter value */

    TIM->BDTR.LOCK = 0; /** 0x08 2 Lock configuration */
    TIM->BDTR.OSSI = 0; /** 0x0A Off-state selection for Idle mode */
    TIM->BDTR.OSSR = 0; /** 0x0B Off-state selection for Run mode */
    TIM->BDTR.AOE  = 0; /** 0x0E Automatic output enable    */
    TIM->BDTR.MOE  = 0; /** 0x0F Main output enable         */

    TIM->CR1.UDIS  =  0; /** 0x01 Update disable */
    TIM->CR1.OPM   =  0; /** 0x03 One-pulse mode */
    TIM->CR1.DIR   =  0; /** 0x04 Direction    up, down */
    TIM->CR1.CMS   =  0; /** 0x05 Center-aligned mode selection  DIR, up, down, both */
    TIM->CR1.CKD   =  0; /** 0x08 Clock division      1,2,4,res   (digital filter)  */

    TIM->CR1.ARPE  = 1; /** 0x07 Auto-reload preload enable 1-> Buffered */
    TIM->CR1.URS   = 1; //! 8. Set interrupt source

//	TIM11->SR &= ~( TIM_SR_CC1IF | TIM_SR_UIF );				// Clear the interrupts

    TIM->EGR.UG      = 1;	//! 9. Load the register data to the hardware (preload-->shadow)
    return( period );
  }

  return( -1 );
}

#define TIM_OCM_FROZN 0
#define TIM_OCM_MATCH 1
#define TIM_OCM_NOMAT 2
#define TIM_OCM_TOGGL 3
#define TIM_OCM_FLOW  4
#define TIM_OCM_FHIGH 5
#define TIM_OCM_PWM   6
#define TIM_OCM_PWM2  7

/* ------------------------------------------------------------------------- */
   short CONFIG_TCHAN( dword  tim, dword work   )
/* ------------------------------------------------------------------------- */
{ volatile struct TIM_STRUCT * TIM= TIMERS[ tim & 0xFF  ].idx;

  if ( TIM )
  { tim >>= 16;          /* Point to channel */

   	TIM->CCR[ tim ].CCR= work;		//! 4. Set the timing parameters,	50% duty cycle

    switch( tim )               /* Extract channel */
    { case 0:
        tim= 0;
     	  TIM->CCER.CC1P=  0;	  //! 7. Configure signal polarity
	       TIM->CCER.CC1E=  1;	  //! Enable the CC output
     //  	TIM->DIER.CC1IE= 1;   //! 10. Configure and enable interrupt
      break;

      case 1:
        tim= 1;
     	  TIM->CCER.CC2P=  0;	  //! 7. Configure signal polarity
	       TIM->CCER.CC2E=  1;	  //! Enable the CC output
    //   	TIM->DIER.CC2IE= 1;   //! 10. Configure and enable interrupt
      break;

      case 2:
        tim= 4;
     	  TIM->CCER.CC3P=  0;	  //! 7. Configure signal polarity
        TIM->CCER.CC3E=  1;	  //! Enable the CC output
    //   	TIM->DIER.CC3IE= 1;   //! 10. Configure and enable interrupt
      break;

      case 3:
        tim= 5;
     	  TIM->CCER.CC4P=  0;	  //! 7. Configure signal polarity
	       TIM->CCER.CC4E=  1;	  //! Enable the CC output
    //   	TIM->DIER.CC4IE= 1;   //! 10. Configure and enable interrupt
      break;

      default: return( -2 );
    }

    TIM->CCMR[ tim ].OUT.OCM = TIM_OCM_PWM;	 //! 5. Set PWM mode
   	TIM->CCMR[ tim ].OUT.OCPE= 1;	           //! 5. Enable output compare preload register

    return( work );
  }

  return( -1 );
}




/* ------------------------------------------------------------------------- */
   short ENABLE_TIMER( dword state )
/* ------------------------------------------------------------------------- */
{ dword idx= state & 0xFF;

  volatile struct TIM_STRUCT * TIM= TIMERS[ idx ].idx;  //! 2. Extract the index

  if ( state & DEV_ENABLE   )
  { TIM->CR1.CEN= 1;                                    /*! Device enable */

    if ( state & DEV_UPDATE ) { TIM->DIER.UIE  = 1; } /*! Update event  */
    if ( state & DEV_SWICTH ) { TIM->DIER.COMIE= 1; } /*! conmutation   */
    if ( state & DEV_TRIGER ) { TIM->DIER.TIE  = 1; } /*! Trigger       */
    if ( state & DEV_BREAK  ) { TIM->DIER.BIE  = 1; } /*! Break         */
  }
  else
  { TIM->CR1.CEN= 0;                                    /*! Device disable */

    if ( state & DEV_UPDATE ) { TIM->DIER.UIE  = 0; } /*! Update event  */
    if ( state & DEV_SWICTH ) { TIM->DIER.COMIE= 0; } /*! conmutation   */
    if ( state & DEV_TRIGER ) { TIM->DIER.TIE  = 0; } /*! Trigger       */
    if ( state & DEV_BREAK  ) { TIM->DIER.BIE  = 0; } /*! Break         */
  }

  if ( state & DEV_IDRIVE )
  { TIM->DIER.CC1IE= TIM->CCER.CC1E; /** 0x01 Capture/Compare 1 interrupt enable */
    TIM->DIER.CC2IE= TIM->CCER.CC2E; /** 0x02 Capture/Compare 2 interrupt enable */
    TIM->DIER.CC3IE= TIM->CCER.CC3E; /** 0x03 Capture/Compare 3 interrupt enable */
    TIM->DIER.CC4IE= TIM->CCER.CC4E; /** 0x04 Capture/Compare 4 interrupt enable */

    irqEnable( TIMERS[ idx ].irqFun, 0 );
  }

  if ( state & DEV_DDRIVE )
  { TIM->DIER.CC1DE= TIM->CCER.CC1E; /** 0x01 Capture/Compare 1 DMA enable */
    TIM->DIER.CC2DE= TIM->CCER.CC2E; /** 0x02 Capture/Compare 2 DMA enable */
    TIM->DIER.CC3DE= TIM->CCER.CC3E; /** 0x03 Capture/Compare 3 DMA enable */
    TIM->DIER.CC4DE= TIM->CCER.CC4E; /** 0x04 Capture/Compare 4 DMA enable */
  }

 	return( 0 );
}


/** ------------------------------------------------------------------------- */
     void setupHardware()
/** ------------------------------------------------------------------------- */
{
    /* Select regulator voltage output Scale 1 mode */
 // DEVICE_ENABLE( RCC_PWR );

 // PWR.CR.VOS= 1;

//#if defined(STM32F401xx)
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
 // FLASH.ACR.PRFTEN = 1;
 // FLASH.ACR.ICEN   = 1;
 // FLASH.ACR.DCEN   = 1;       // wait states
 // FLASH.ACR.LATENCY= 2;       // wait states

//#endif /* STM32F401xx */

}

ABSOLUTE(  TIM2, 0x40000000 );  /* 0 General purpose timers */
ABSOLUTE(  TIM3, 0x40000400 );  /* 1 General purpose timers */
ABSOLUTE(  TIM4, 0x40000800 );  /* 2 unknown */
ABSOLUTE(  TIM5, 0x40000C00 );  /* 3 General-purpose-timers */

ABSOLUTE(  TIM1, 0x40010000 );  /* 40 Advanced-timers */
ABSOLUTE(  TIM8, 0x40010400 );  /* 41 unknown */
ABSOLUTE(  TIM9, 0x40014000 );  /* 50 General purpose timers */
ABSOLUTE( TIM10, 0x40014400 );  /* 51 General-purpose-timers */
ABSOLUTE( TIM11, 0x40014800 );  /* 52 General-purpose-timers */

