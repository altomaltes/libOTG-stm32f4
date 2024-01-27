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
 *     FILE: STM32F401.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

ENTRY( resetHandler ) /* Entry Point */

/*  uc devices
 */

/*   ADC_Common = 0x40012300;  /* ADC common registers */
/*         ADC1 = 0x40012000;  /* Analog-to-digital converter */
/*          CRC = 0x40023000;  /* Cryptographic processor */
/*          DBG = 0xE0042000;  /* Debug support */
/*/         EXTI = 0x40013C00;  /* External interrupt/event controller */
/*/        FLASH = 0x40023C00;  /* FLASH */
/*/         IWDG = 0x40003000;  /* Independent watchdog */
/*/OTG_FS_DEVICE = 0x50000800;  /* USB on the go full speed */
/*/OTG_FS_GLOBAL = 0x50000000;  /* USB on the go full speed */
/*/OTG_FS_HOST   = 0x50000400;  /* USB on the go full speed */
/*/OTG_FS_PWRCLK = 0x50000E00;  /* USB on the go full speed */
/*/          PWR = 0x40007000;  /* Power control */
/*/          RCC = 0x40023800;  /* Reset and clock control */
/*/          RTC = 0x40002800;  /* Real-time clock */
/*/         SDIO = 0x40012C00;  /* Secure digital input/output interface */
/*/       SYSCFG = 0x40013800;  /* System configuration controller */

/*/        TIM2 = 0x40000000;  /* 0 General purpose timers */
/*/        TIM3 = 0x40000400;  /* 1 General purpose timers */
/*/        TIM4 = 0x40000800;  /* 2 unknown */
/*/        TIM5 = 0x40000C00;  /* 3 General-purpose-timers */

/*/        TIM1 = 0x40010000;  /* 40 Advanced-timers */
/*/        TIM8 = 0x40010400;  /* 41 unknown */
/*        TIM9 = 0x40014000;  /* 50 General purpose timers */
/*       TIM10 = 0x40014400;  /* 51 General-purpose-timers */
/*       TIM11 = 0x40014800;  /* 52 General-purpose-timers */

/*      USART1 = 0x40011000;  /* Universal synchronous asynchronous receiver transmitter */
/*      USART2 = 0x40004400;  /* unknown */
/*      USART6 = 0x40011400;  /* unknown */

/*        WWDG = 0x40002C00;  /* Window watchdog */
/*       DMA2 = 0x40026400;  /* DMA controller */
/*        DMA1 = 0x40026000;  /* unknown */
/*       GPIOH = 0x40021C00;  /* General-purpose I/Os */
/*       GPIOE = 0x40021000;  /* unknown */
/*       GPIOD = 0x40020C00;  /* unknown */
/*       GPIOC = 0x40020800;  /* unknown */
/*       GPIOB = 0x40020400;  /* General-purpose I/Os */
/*       GPIOA = 0x40020000;  /* General-purpose I/Os */
/*        I2C3 = 0x40005C00;  /* Inter-integrated circuit */
/*        I2C2 = 0x40005800;  /* unknown */
/*        I2C1 = 0x40005400;  /* unknown */
/*     I2S2ext = 0x40003400;  /* Serial peripheral interface */
/*     I2S3ext = 0x40004000;  /* unknown */
/*        SPI1 = 0x40013000;  /* unknown */
/*        SPI2 = 0x40003800;  /* unknown */
/*        SPI3 = 0x40003C00;  /* unknown */
/*        SPI4 = 0x40013400;  /* unknown */
/*        NVIC = 0xE000E100;  /* Nested Vectored Interrupt Controller */
/*         FPU = 0xE000EF34;  /* Floting point unit */
/*        MPU = 0xE000ED90;  /* Memory protection unit */
/*         STK = 0xE000E010;  /* SysTick timer */
/*         SCB = 0xE000ED00;  /* System control block */
/*   NVIC_STIR = 0xE000EF00;  /* Nested vectored interrupt controller */
/*   FPU_CPACR = 0xE000ED88;  /* Floating point unit CPACR */
/*   SCB_ACTRL = 0xE000E008;  /* System control block ACTLR */

/* JACS, sophisticated "weak". Chooses handler based on initializer, so dead code can be removed, extendable to two cores
OTG_HSirqHnd = USBinitOTG  ? USBIrqHandlerOTG
             : USBinitDEV  ? USBIrqHandlerDEV
             : USBinitHOST ? USBIrqHandlerHOST
             : irqHnd ;

OTG_FSirqHnd = USBinitOTG  ? USBIrqHandlerOTG
             : USBinitDEV  ? USBIrqHandlerDEV
             : USBinitHOST ? USBIrqHandlerHOST
             : irqHnd ;

 */


HEAP_GAP  = DEFINED(  __HEAP_SIZE ) ? __HEAP_SIZE  : 0x0200;      /* required amount of heap  */
STACK_GAP = DEFINED( __STACK_SIZE ) ? __STACK_SIZE : 0x0500;      /* required amount of stack */

FLASH_START = 0x08000000;
RAM_START   = 0x20000000;
CCRAM_START = 0x10000000;


/* Specify the memory areas
 */
MEMORY
{ FLASH  (rx) : ORIGIN = FLASH_START, LENGTH = FLASH_SIZE  /*  Main FLASH memory  */
    RAM (rwx) : ORIGIN =   RAM_START, LENGTH = RAM_SIZE    /*  Main RAM memory  */
 CCMRAM (rwx) : ORIGIN = CCRAM_START, LENGTH =   64K       /*  Core Coupled RAM Memory  */
}


/* Highest address of the user mode stack */
/* Generate a link error if heap and stack don't fit into RAM */

SECTIONS                          /* Define output sections */
{ .rodata :
  { KEEP ( *(.cortexVectors*) )   /* Common cortex CPU vectors */
           *(.systemVectors*)     /* Architecture dependent vectors */
    . = ALIGN(4); _rostart = .;
    *(.gnu.linkonce.d*)
     *(.rodata)                   /* .rodata sections (constants, strings, etc.)  */
     *(.rodata*)                  /* .rodata* sections (constants, strings, etc.) */
     *(.gnu.linkonce.d*)
    . = ALIGN(4); _roend   = .;
  } > FLASH

  .data :                        /* Initialized data sections goes into RAM, load LMA copy after code */
  { . = ALIGN(32);  _sdata = .;  /* create a global symbol at data start */
    *(.gnu.linkonce.r*)
    *(.data)                     /* .data sections */
    *(.data*)                    /* .data* sections */
    *(.gnu.linkonce.r*)
    . = ALIGN(4);  _edata = .;   /* define a global symbol at data end */
  } > RAM AT > FLASH

  .ARM.extab :
  { *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  .ARM :
  { __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > FLASH

  .preinit_array :
  { PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  { PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  { PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH


/* The program code and other data goes into FLASH
 */
  .text :   /* Startup code */
  { . = ALIGN(4); _stext = . ;
    *(.text)          /* .text sections (code) */
    *(.text*)         /* .text* sections (code) */
    *(.glue_7)        /* glue arm to thumb code */
    *(.glue_7t)       /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4); _etext = .;   /* define a global symbols at end of code */
  } > FLASH

  .bss (NOLOAD) :               /* Uninitialized data section  */
  { . = ALIGN(4); _sbss = .;    /* define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4); _ebss = .;    /* define a global symbol at bss end */
  } > RAM

  ._user_heap_stack (NOLOAD):   /* User_heap_stack section, used to check that there is enough RAM left */
  { . = ALIGN(8);
    PROVIDE (  end = . );
    PROVIDE ( _end = . );
    . = . + HEAP_GAP;
    . = . + STACK_GAP;
    . = ALIGN(8);
  } > RAM


  /DISCARD/ : /* Remove information from the standard libraries */
  { libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 :
  { *(.ARM.attributes)
} }

ldStack = ORIGIN( RAM ) + LENGTH( RAM );    /* end of RAM */



