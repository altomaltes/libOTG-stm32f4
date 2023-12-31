/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2021   ( altomaltes@yahoo.es )
 *                                             ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2014 JACS
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
 *     FILE: irq.c
 *     DATE: ene 2021
 *
 *  DESCRIPCION: weak default handlers
 */

/* E000E100 : NVIC : Nested Vectored Interrupt Controller
 */
#include "cortexm.h"

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   SCB : System control block                     @ 0xE000ED00 : 0x0041
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

union SCB$SHPR1
{ struct
  { dword      PRI_4 :   8; /** 0x00 Priority of system handler 4 */
    dword      PRI_5 :   8; /** 0x08 Priority of system handler 5 */
    dword      PRI_6 :   8; /** 0x10 Priority of system handler 6 */
    dword            :   8; /** 0x18 */
  };

  dword atomic;            /** atomic access */
};

union SCB$SHPR2
{ struct
  { dword            :  24; /** 0x00 */
    dword     PRI_11 :   8; /** 0x18 Priority of system handler 11 */
  };

  dword atomic;            /** atomic access */
};

union SCB$SHPR3
{ struct
  { dword            :  16; /** 0x00 */
    dword     PRI_14 :   8; /** 0x10 Priority of system handler 14 */
    dword     PRI_15 :   8; /** 0x18 Priority of system handler 15 */
  };

  dword atomic;            /** atomic access */
};

extern volatile struct            /** 0xE000ED00 System control block */
{ struct
  { dword   Revision :   4; /** 0x00 Revision number */
    dword     PartNo :  12; /** 0x04 Part number of the processor */
    dword   Constant :   4; /** 0x10 Reads as 0xF */
    dword    Variant :   4; /** 0x14 Variant number */
    dword Implementer:   8; /** 0x18 Implementer code */
  }  CPUID;                 /** 0x00 RST: 0x410FC241 CPUID base register */

  struct
  { dword VECTACTIVE :   9; /** 0x00 Active vector */
    dword            :   2; /** 0x09 */
    dword  RETTOBASE :   1; /** 0x0B Return to base level */
    dword VECTPENDING:   7; /** 0x0C Pending vector */
    dword            :   3; /** 0x13 */
    dword ISRPENDING :   1; /** 0x16 Interrupt pending flag */
    dword            :   2; /** 0x17 */
    dword  PENDSTCLR :   1; /** 0x19 SysTick exception clear-pending bit */
    dword  PENDSTSET :   1; /** 0x1A SysTick exception set-pending bit   */
    dword  PENDSVCLR :   1; /** 0x1B PendSV clear-pending bit            */
    dword  PENDSVSET :   1; /** 0x1C PendSV set-pending bit              */
    dword            :   2; /** 0x1D                                     */
    dword NMIPENDSET :   1; /** 0x1F NMI set-pending bit.                */
  } ICSR;

  void *           VTOR;    /** 0x08 RST: 0x00000000 Vector table offset register */

  struct
  { dword    VECTRESET :   1; /** 0x00 VECTRESET */
    dword VECTCLRACTIVE:   1; /** 0x01 VECTCLRACTIVE */
    dword   SYSRESETREQ:   1; /** 0x02 SYSRESETREQ */
    dword              :   5; /** 0x03 */
    dword     PRIGROUP :   3; /** 0x08 PRIGROUP */
    dword              :   4; /** 0x0B */
    dword    ENDIANESS :   1; /** 0x0F ENDIANESS */
    dword  VECTKEYSTAT :  16; /** 0x10 Register key */
  } AIRCR;    /** 0x0C RST: 0x00000000 Application interrupt and reset control register */

  struct
  { dword            :   1; /** 0x00 */
    dword SLEEPONEXIT:   1; /** 0x01 SLEEPONEXIT */
    dword  SLEEPDEEP :   1; /** 0x02 SLEEPDEEP */
    dword            :   1; /** 0x03 */
    dword SEVEONPEND :   1; /** 0x04 Send Event on Pending bit */
    dword            :  27; /** 0x05 */
  } SCR;                    /** 0x10 RST: 0x00000000 System control register */

  struct
  { dword NONBASETHRDENA:   1; /** 0x00 Configures how the processor enters Thread mode */
    dword USERSETMPEND  :   1; /** 0x01 USERSETMPEND */
    dword               :   1; /** 0x02 */
    dword UNALIGN__TRP  :   1; /** 0x03 UNALIGN_ TRP */
    dword  DIV_0_TRP    :   1; /** 0x04 DIV_0_TRP */
    dword               :   3; /** 0x05 */
    dword  BFHFNMIGN    :   1; /** 0x08 BFHFNMIGN */
    dword   STKALIGN    :   1; /** 0x09 STKALIGN */
    dword               :  22; /** 0x0A */
  } CCR;                       /** 0x14 RST: 0x00000000 Configuration and control register */

  union SCB$SHPR1       SHPR1;    /** 0x18 RST: 0x00000000 System handler priority registers */
  union SCB$SHPR2       SHPR2;    /** 0x1C RST: 0x00000000 System handler priority registers */
  union SCB$SHPR3       SHPR3;    /** 0x20 RST: 0x00000000 System handler priority registers */

  struct
  { dword MEMFAULTACT    :   1; /** 0x00 Memory management fault exception active bit */
    dword BUSFAULTACT    :   1; /** 0x01 Bus fault exception active bit */
    dword                :   1; /** 0x02 */
    dword USGFAULTACT    :   1; /** 0x03 Usage fault exception active bit */
    dword                :   3; /** 0x04 */
    dword  SVCALLACT     :   1; /** 0x07 SVC call active bit */
    dword MONITORACT     :   1; /** 0x08 Debug monitor active bit */
    dword                :   1; /** 0x09 */
    dword  PENDSVACT     :   1; /** 0x0A PendSV exception  active bit      */
    dword SYSTICKACT     :   1; /** 0x0B SysTick exception active bit      */
    dword USGFAULTPENDED :   1; /** 0x0C Usage fault exception pending bit */
    dword MEMFAULTPENDED :   1; /** 0x0D Memory management fault exception pending bit */
    dword BUSFAULTPENDED :   1; /** 0x0E Bus fault exception pending bit */
    dword SVCALLPENDED   :   1; /** 0x0F SVC call pending bit */
    dword MEMFAULTENA    :   1; /** 0x10 Memory management fault enable bit */
    dword BUSFAULTENA    :   1; /** 0x11 Bus fault enable bit */
    dword USGFAULTENA    :   1; /** 0x12 Usage fault enable bit */
    dword                :  13; /** 0x13 */
  } SHCRS;                      /** 0x24 RST: 0x00000000 System handler control and state register */

  struct
  { dword            :   1; /** 0x00 */
    dword   IACCVIOL :   1; /** 0x01 Instruction access violation flag */
    dword            :   1; /** 0x02 */
    dword  MUNSTKERR :   1; /** 0x03 Memory manager fault on unstacking for a return from exception */
    dword    MSTKERR :   1; /** 0x04 Memory manager fault on stacking for exception entry. */
    dword    MLSPERR :   1; /** 0x05 MLSPERR */
    dword            :   1; /** 0x06 */
    dword  MMARVALID :   1; /** 0x07 Memory Management Fault Address Register (MMAR) valid flag */
    dword    IBUSERR :   1; /** 0x08 Instruction bus error */
    dword  PRECISERR :   1; /** 0x09 Precise data bus error */
    dword IMPRECISERR :   1; /** 0x0A Imprecise data bus error */
    dword   UNSTKERR :   1; /** 0x0B Bus fault on unstacking for a return from exception */
    dword     STKERR :   1; /** 0x0C Bus fault on stacking for exception entry */
    dword     LSPERR :   1; /** 0x0D Bus fault on floating-point lazy state preservation */
    dword            :   1; /** 0x0E */
    dword  BFARVALID :   1; /** 0x0F Bus Fault Address Register (BFAR) valid flag */
    dword UNDEFINSTR :   1; /** 0x10 Undefined instruction usage fault */
    dword   INVSTATE :   1; /** 0x11 Invalid state usage fault */
    dword      INVPC :   1; /** 0x12 Invalid PC load usage fault */
    dword       NOCP :   1; /** 0x13 No coprocessor usage fault. */
    dword            :   4; /** 0x14 */
    dword  UNALIGNED :   1; /** 0x18 Unaligned access usage fault */
    dword  DIVBYZERO :   1; /** 0x19 Divide by zero usage fault */
    dword            :   6; /** 0x1A */
  } CFSR_UFSR_BFSR_MMFSR;   /** 0x28 RST: 0x00000000 Configurable fault status register */

  struct
  { dword            :   1; /** 0x00 */
    dword    VECTTBL :   1; /** 0x01 Vector table hard fault */
    dword            :  28; /** 0x02 */
    dword     FORCED :   1; /** 0x1E Forced hard fault */
    dword   DEBUG_VT :   1; /** 0x1F Reserved for Debug use */
  } HFSR;                   /** 0x2C RST: 0x00000000 Hard fault status register */

  dword            gap$30[  1 ];   /** 0x30 Unused register */

  void *             MMFAR;    /** 0x34 RST: 0x00000000 Memory management fault address register */
  void *              BFAR;    /** 0x38 RST: 0x00000000 Bus fault address register */
  dword               AFSR;    /** 0x3C RST: 0x00000000 Auxiliary fault status register */

  dword          gap$40[   0 ];    /** 0x40 Unused memory */
} SCB;


extern volatile struct
{ dword ISER[ 32 ];  /* E000E100 + 00  : ISER Interrupt Set Enable Register  */
  dword ICER[ 32 ];  /* E000E100 + 80  : ICER Interrupt Clear Enable Register  */
  dword ISPR[ 32 ];  /* E000E100 + 100 : ISPR Interrupt Set-Pending Register  */
  dword ICPR[ 96 ];  /* E000E100 + 180 : ICPR Interrupt Clear-Pending Register  */


/* E000E100 + 300 : IPR0 Interrupt Priority Register 0
 */
  union
  { struct
    { dword PRI_0 : 8; /*  0 priority for interrupt 0 */
      dword PRI_1 : 8; /*  8 priority for interrupt 1 */
      dword PRI_2 : 8; /* 16 priority for interrupt 2 */
      dword PRI_3 : 8; /* 24 priority for interrupt 3 */
    };

    dword atomic;            /** atomic access */

    dword asDarr[ 1 ];       /** 32 bit access */
    word  asWarr[ 2 ];       /** 16 bit access */
    byte  asBarr[ 4 ];       /**  8 bit access */
  } IPR[ 8 ];

} NVIC;


/*
 */


WEAK INTERRUPT void irqHnd()
{ while( 1 );
}

WEAK INTERRUPT void NMIHnd()            { while(1);}   /* NMI Handler                  */
WEAK INTERRUPT void HardFaultHnd()      { while(1);}   /* Hard Fault Handler           */

WEAK INTERRUPT void MemManageHnd()
{ while(1);
}   /* MPU Fault Handler            */

int a;
WEAK INTERRUPT void UsageFaultHnd()
{ while(1);
}   /* Usage Fault Handler          */

WEAK INTERRUPT void BusFaultHnd()       { while(1);}   /* Bus Fault Handler            */
WEAK INTERRUPT void SVCHnd()            { while(1);}   /* SVCall Handler               */
WEAK INTERRUPT void DebugMonHnd()       { while(1);}   /* Debug Monitor Handler        */
WEAK INTERRUPT void PendSVHnd()         { while(1);}   /* PendSV Handler               */
               void SysTickHnd( void );   /* SysTick Handler              */
WEAK INTERRUPT void WWDGirqHnd()        { while(1);}   /* Window WatchDog              */
WEAK INTERRUPT void PVDirqHnd()         { while(1);}   /* PVD through EXTI Line detection */
WEAK INTERRUPT void TAMP_STAMPirqHnd()  { while(1);}   /* Tamper and TimeStamps through the EXTI line */
WEAK INTERRUPT void RTC_WKUPirqHnd()    { while(1);}   /* RTC Wakeup through the EXTI line */
WEAK INTERRUPT void FLASHirqHnd()       { while(1);}   /* FLASH                        */
WEAK INTERRUPT void RCCirqHnd()         { while(1);}   /* RCC                          */
WEAK INTERRUPT void EXTI0irqHnd()       { while(1);}   /* EXTI Line0                   */
WEAK INTERRUPT void EXTI1irqHnd()       { while(1);}   /* EXTI Line1                   */
WEAK INTERRUPT void EXTI2irqHnd()       { while(1);}   /* EXTI Line2                   */
WEAK INTERRUPT void EXTI3irqHnd()       { while(1);}   /* EXTI Line3                   */
WEAK INTERRUPT void EXTI4irqHnd()       { while(1);}   /* EXTI Line4                   */
WEAK INTERRUPT void DMA1_0irqHnd()      { while(1);}   /* DMA1 Stream 0                */
WEAK INTERRUPT void DMA1_1irqHnd()      { while(1);}   /* DMA1 Stream 1                */
WEAK INTERRUPT void DMA1_2irqHnd()      { while(1);}   /* DMA1 Stream 2                */
WEAK INTERRUPT void DMA1_3irqHnd()      { while(1);}   /* DMA1 Stream 3                */
WEAK INTERRUPT void DMA1_4irqHnd()      { while(1);}   /* DMA1 Stream 4                */
WEAK INTERRUPT void DMA1_5irqHnd()      { while(1);}   /* DMA1 Stream 5                */
WEAK INTERRUPT void DMA1_6irqHnd()      { while(1);}   /* DMA1 Stream 6                */
WEAK INTERRUPT void ADCirqHnd()         { while(1);}   /* ADC1, ADC2 and ADC3s         */
WEAK INTERRUPT void EXTI9_5irqHnd()     { while(1);}   /* External Line[9:5]s          */
WEAK INTERRUPT void TIM1_BRK_9irqHnd()  { while(1);}   /* TIM1 Break and TIM9          */
WEAK INTERRUPT void TIM1_UP_10irqHnd()  { while(1);}   /* TIM1 Update and TIM10        */
WEAK INTERRUPT void TIM1_TRG_11irqHnd() { while(1);}   /* TIM1 Trigger and Commutation and TIM11 */
WEAK INTERRUPT void TIM1_CCirqHnd()     { while(1);}   /* TIM1 Capture Compare         */
WEAK INTERRUPT void TIM2irqHnd()        { while(1);}   /* TIM2                         */
WEAK INTERRUPT void TIM3irqHnd()        { while(1);}   /* TIM3                         */
WEAK INTERRUPT void TIM4irqHnd()        { while(1);}   /* TIM4                         */
WEAK INTERRUPT void I2C1_EVirqHnd()     { while(1);}   /* I2C1 Event                   */
WEAK INTERRUPT void I2C1_ERirqHnd()     { while(1);}   /* I2C1 Error                   */
WEAK INTERRUPT void I2C2_EVirqHnd()     { while(1);}   /* I2C2 Event                   */
WEAK INTERRUPT void I2C2_ERirqHnd()     { while(1);}   /* I2C2 Error                   */
WEAK INTERRUPT void SPI1irqHnd()        { while(1);}   /* SPI1                         */
WEAK INTERRUPT void SPI2irqHnd()        { while(1);}   /* SPI2                         */
WEAK INTERRUPT void USART1irqHnd()      { while(1);}   /* USART1                       */
WEAK INTERRUPT void USART2irqHnd()      { while(1);}   /* USART2                       */
WEAK INTERRUPT void USART3irqHnd()      { while(1);}   /* USART3                       */
WEAK INTERRUPT void USART4irqHnd()      { while(1);}   /* USART4                       */
WEAK INTERRUPT void USART5irqHnd()      { while(1);}   /* USART5                       */
WEAK INTERRUPT void USART6irqHnd()      { while(1);}   /* USART6                       */
WEAK INTERRUPT void EXTI15_10irqHnd()   { while(1);}   /* External Line[15:10]s        */
WEAK INTERRUPT void RTC_AlarmirqHnd()   { while(1);}   /* RTC Alarm (A and B) through EXTI Line */
WEAK INTERRUPT void OTG_FS_WKUPirqHnd() { while(1);}   /* USB OTG FS Wakeup through EXTI line */
WEAK INTERRUPT void DMA1_7irqHnd()      { while(1);}   /* DMA1 Stream7                 */
WEAK INTERRUPT void SDIOirqHnd()        { while(1);}   /* SDIO                         */
WEAK INTERRUPT void TIM5irqHnd()        { while(1);}   /* TIM5                         */
WEAK INTERRUPT void SPI3irqHnd()        { while(1);}   /* SPI3                         */
WEAK INTERRUPT void DMA2_0irqHnd()      { while(1);}   /* DMA2 Stream 0                */
WEAK INTERRUPT void DMA2_1irqHnd()      { while(1);}   /* DMA2 Stream 1                */
WEAK INTERRUPT void DMA2_2irqHnd()      { while(1);}   /* DMA2 Stream 2                */
WEAK INTERRUPT void DMA2_3irqHnd()      { while(1);}   /* DMA2 Stream 3                */
WEAK INTERRUPT void DMA2_4irqHnd()      { while(1);}   /* DMA2 Stream 4                */
//WEAK INTERRUPT void OTG_FSirqHnd()      { while(1);}   /* USB OTG FS                   */
WEAK INTERRUPT void DMA2_5irqHnd()      { while(1);}   /* DMA2 Stream 5                */
WEAK INTERRUPT void DMA2_6irqHnd()      { while(1);}   /* DMA2 Stream 6                */
WEAK INTERRUPT void DMA2_7irqHnd()      { while(1);}   /* DMA2 Stream 7                */
WEAK INTERRUPT void I2C3_EVirqHnd()     { while(1);}   /* I2C3 event                   */
WEAK INTERRUPT void I2C3_ERirqHnd()     { while(1);}   /* I2C3 error                   */
WEAK INTERRUPT void FPUirqHnd()         { while(1);}   /* FPU                          */
WEAK INTERRUPT void SPI4irqHnd()        { while(1);}   /* SPI4                         */

WEAK INTERRUPT void OTG_HSep1OUTirqHnd(){ while(1);}   /* SPI4                         */
WEAK INTERRUPT void OTG_HSep1INirqHnd() { while(1);}   /*    75 OTG HS End Point 1 In global interrupt  */
WEAK INTERRUPT void OTG_HSWakeUPirqHnd(){ while(1);}  /*    76 OTG HS Wakeup through EXTI interrupt    */
WEAK INTERRUPT void OTG_HSirqHnd()      { while(1);}       /*    77 OTG HS global interrupt                 */


void  ldStack();  /* defined on the ld script */



void main();
extern irqfun systemVectors[];   // Begining of uc vectors
void * s= systemVectors;   // Begining of uc vectors


/** ------------------------------------------------------------------------- */
    NAKED void resetHandler()
/** ------------------------------------------------------------------------- */
{ extern dword  _sdata[];
  extern dword  _edata[];
  extern dword  _sbss[];
  extern dword  _ebss[];
  extern dword  _roend[];

  dword * dst= _sdata;
  dword * src= _roend;

  s++;

/* Copy inizialized data
 */
  while( dst < _edata )
  { *dst++= *src++;
  }

/* Zero fill the bss segment.
 */
  dst= _sbss;
  while( dst <= _ebss )
  { *dst++= 0;
  }

 /* Configure the Vector Table location add offset address */
#ifdef VECT_TAB_SRAM
  extern void * RAM_START;   SCB.VTOR= &RAM_START; /* Vector Table Relocation in Internal SRAM */
#else
  extern void * FLASH_START; SCB.VTOR= &FLASH_START; /* Vector Table Relocation in Internal FLASH */
#endif

  setupHardware();

  main();

}

SECTION( ".cortexVectors" ) const irqfun cortexVectors[]=
{ ldStack
, resetHandler
, NMIHnd
, HardFaultHnd
, MemManageHnd
, BusFaultHnd
, UsageFaultHnd
, irqHnd
, irqHnd
, irqHnd
, irqHnd
, SVCHnd
, DebugMonHnd
, irqHnd
, PendSVHnd
, SysTickHnd
};


/* IRQ related
 */
short IRQ_DISABLE( short irq ) { NVIC.ICER[ irq >> 0x05 ]= ((dword)0x01 << ( irq & 0x1F ));  return( 0 ); }
short IRQ_SET(     short irq ) { NVIC.ISPR[ irq >> 0x05 ]= ((dword)0x01 << ( irq & 0x1F ));  return( 0 ); }
short IRQ_CLR(     short irq ) { NVIC.ICPR[ irq >> 0x05 ]= ((dword)0x01 << ( irq & 0x1F ));  return( 0 ); }

short IRQ_ENABLE( short irq, short prio )
{ IRQ_CLR( irq );   // Clear previous

  if ( prio )
  { NVIC.IPR[ irq >> 2 ].atomic &= ~( 0x03 << ( 6 + ( irq & 0x03 )));
    NVIC.IPR[ irq >> 2 ].atomic |=  ( prio << ( 6 + ( irq & 0x03 )));
  }

  COMPILER_BARRIER;
  NVIC.ISER[ irq >> 0x05 ]= ((dword)0x01 << ( irq & 0x1F ));
  COMPILER_BARRIER;

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   short irqEnable( irqfun fun, short prio )
/* ------------------------------------------------------------------------- */
{ extern irqfun _rostart[];        // Below last uc vector

  irqfun * ptr= systemVectors;

  while( ptr < _rostart )         // Iterate system vectors
  { if ( *ptr == fun )
    { return( IRQ_ENABLE( ptr - systemVectors, prio ));
    }
    ptr++;
  }

  return( -1 );                   // Not found
}

/* ------------------------------------------------------------------------- */
   short irqDisable( irqfun fun )
/* ------------------------------------------------------------------------- */
{ extern irqfun _rostart[];        // Below last uc vector
  extern irqfun systemVectors[];   // Begining of uc vectors

  irqfun * ptr= systemVectors;

  while( ptr < _rostart )         // Iterate system vectors
  { if ( *ptr == fun )
    { return( IRQ_DISABLE( ptr - systemVectors ));
    }
    ptr++;
  }

  return( -1 );                   // Not found
}

/* ------------------------------------------------------------------------- */
   short irqEnableFun( irqfun fun                 /* Link interrupt handlers */
                     , short irq, short prio )
/* ------------------------------------------------------------------------- */
{ extern void * VECTBL_START[];                   /* RAM vector table        */

  VECTBL_START[ irq + 16 ]= fun;

  return( IRQ_ENABLE( irq, prio ));
}



extern volatile struct
{ struct
  { dword ENBL:  1;  /*! Enable counter   */
    dword INTE:  1;  /*! Enable interrupt */
    dword CSRC:  1;  /*! clock source 0 -> AHB/8 1 -> AHB  */
    dword     : 13;  /*! reserved         */
    dword CBIT:  1;  /*! count active     */
    dword     : 15;  /*! reserved         */
  } CTRL;            /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */

  dword LOAD;    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  dword VALUE;   /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */

  struct
  { dword VALUE: 24;
    dword      :  6;
    dword SKEW :  1;
    dword REF  :  1;
  } CALIB;   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */

} STK;


/* systick related
 */

volatile static qword upTime;
volatile static  word  uSecs;

/* ········································································· */
   WEAK void osSysTick( dword upTime )
/* ········································································· */
{ // Placeholder
}


/* ------------------------------------------------------------------------- */
   dword uDelay( word usecs )  // 24 bit usec delay
/* ------------------------------------------------------------------------- */
{ int ticks  = ( STK.LOAD + 1 ); /* Calc number of pits */
      ticks *= usecs;
      ticks /= uSecs;
      ticks= STK.VALUE - ticks;

  if ( ticks > 0 )      /* Not rolling over */
  { while( STK.VALUE > ticks );
  }
  else                           /* Rolling over */
  { ticks= STK.LOAD + ticks;
    while( STK.VALUE < ticks );
  }

  return( 0 );
}


/* ------------------------------------------------------------------------- */
   dword mDelay( dword msecs )
/* ------------------------------------------------------------------------- */
{ msecs *= 1000;
  msecs /= uSecs; /* Convert to cycles */

  while( msecs )
  { if ( STK.CTRL.CBIT )  // Returns 1 if timer counted to 0 since last time this was read.
    { msecs--;
  } }

//  while( msecs-- )
//  { while( STK.VALUE > ( STK.LOAD >> 1 ));       /* Wait for a tournement */
//    while( STK.VALUE < ( STK.LOAD >> 1 ));       /* Wait for a tournement */
//  }

  return( 0 );
}

/* ------------------------------------------------------------------------- */
   INTERRUPT void SysTickHnd( void )
/* ------------------------------------------------------------------------- */
{ upTime += uSecs;

  osSysTick( upTime );
}

/* ------------------------------------------------------------------------- */
   void sysTickConfig( dword ticks )
/* ------------------------------------------------------------------------- */
{ STK.CTRL.CSRC= 0;  // AHB, resolution / 8

  uSecs= 1000000 / ticks;        /* usecs per step */
  ticks= ( SYSCLK_HZ( 0 ) >> 3 ) /* Main clock     */
       / ticks;

  STK.LOAD= ticks - 1 ; /* Reload register */
  STK.VALUE= 0;
  STK.CTRL.ENBL=
  STK.CTRL.INTE= 1;

//  SYS_PRIORITY( systickIRQ, 0xFF );            /* Priority for Systick Interrupt */
}


///* ------------------------------------------------------------------------- */
//   void sysDelay( dword usecs )
///* ------------------------------------------------------------------------- */
//{ dword load= STK.VALUE;
//}


ABSOLUTE(      NVIC, 0xE000E100 );  /* Nested Vectored Interrupt Controller */
ABSOLUTE(       FPU, 0xE000EF34 );  /* Floting point unit */
ABSOLUTE(       MPU, 0xE000ED90 );  /* Memory protection unit */
ABSOLUTE(       STK, 0xE000E010 );  /* SysTick timer */
ABSOLUTE(       SCB, 0xE000ED00 );  /* System control block */
ABSOLUTE( NVIC_STIR, 0xE000EF00 );  /* Nested vectored interrupt controller */
ABSOLUTE( FPU_CPACR, 0xE000ED88 );  /* Floating point unit CPACR */
ABSOLUTE(  CB_ACTRL, 0xE000E008 );  /* System control block ACTLR */




