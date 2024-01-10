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
 *     FILE: cortexm.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

#ifndef CORTEXM_H_INCLUDED
#define CORTEXM_H_INCLUDED

#include "../uc.h"


short IRQ_SET(     short irq );
short IRQ_CLR(     short irq );
short IRQ_DISABLE( short irq );
short IRQ_ENABLE(  short irq, short prio );

short irqEnable(    irqfun fun, short prio );
short irqDisable(   irqfun fun );
short irqEnableFun( irqfun fun, short irq, short prio );

INTERRUPT void         WWDGirqHnd();     /* Window WatchDog              */
INTERRUPT void          PVDirqHnd();     /* PVD through EXTI Line detection */
INTERRUPT void             irqHnd();     /* Default orphan Handler       */
INTERRUPT void          FPUirqHnd();     /* FPU interrupt */
INTERRUPT void          RCCirqHnd();     /* RCC global interrupt */
INTERRUPT void        EXTI0irqHnd();     /* EXTI Line0 interrupt */
INTERRUPT void    RTC_AlarmirqHnd();     /* RTC Alarms (A and B) through EXTI line interrupt */
INTERRUPT void     RTC_WKUPirqHnd();     /* RTC Wakeup interrupt through the EXTI line */
INTERRUPT void        EXTI1irqHnd();     /* EXTI Line1 interrupt */
INTERRUPT void        EXTI2irqHnd();     /* EXTI Line2 interrupt */
INTERRUPT void        EXTI3irqHnd();     /* EXTI Line3 interrupt */
INTERRUPT void        EXTI4irqHnd();     /* EXTI Line4 interrupt */
INTERRUPT void      EXTI9_5irqHnd();     /* EXTI Line[9:5] interrupts */
INTERRUPT void   TAMP_STAMPirqHnd();     /* Tamper and TimeStamp interrupts through the EXTI line */
INTERRUPT void    EXTI15_10irqHnd();     /* EXTI Line[15:10] interrupts */

INTERRUPT void      TIM1_CCirqHnd();    /* TIM1 capture compare */
INTERRUPT void   TIM1_BRK_9irqHnd();    /* TIM1 Break interrupt and TIM9 global interrupt */
INTERRUPT void   TIM1_UP_10irqHnd();    /* TIM1 Update interrupt and TIM10 global interrupt */
INTERRUPT void  TIM1_TRG_11irqHnd();    /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt */
INTERRUPT void         TIM2irqHnd();    /* TIM2 global interrupt */
INTERRUPT void         TIM3irqHnd();    /* TIM3 global interrupt */
INTERRUPT void         TIM4irqHnd();    /* TIM3 global interrupt */
INTERRUPT void         TIM5irqHnd();    /* TIM3 global interrupt */
INTERRUPT void         SPI1irqHnd();    /* SPI1 global interrupt */
INTERRUPT void         SPI4irqHnd();    /* SPI1 global interrupt */
INTERRUPT void      I2C1_EVirqHnd();    /* I2C1 event interrupt */
INTERRUPT void      I2C2_EVirqHnd();    /* I2C1 event interrupt */
INTERRUPT void      I2C3_EVirqHnd();    /* I2C3 event interrupt */
INTERRUPT void      I2C1_ERirqHnd();    /* I2C1 error interrupt */
INTERRUPT void      I2C2_ERirqHnd();    /* I2C2 error interrupt */
INTERRUPT void      I2C3_ERirqHnd();    /* I2C3 event interrupt */
INTERRUPT void         SPI2irqHnd();    /* SPI2 global interrupt */
INTERRUPT void         SPI3irqHnd();    /* SPI3 global interrupt */
INTERRUPT void        FLASHirqHnd();    /* FLASH global interrupt */
INTERRUPT void         SDIOirqHnd();    /* SDIO global interrupt */
INTERRUPT void       OTG_FSirqHnd();    /* USB On The Go FS global interrupt */
INTERRUPT void  OTG_FS_WKUPirqHnd();    /* USB OTG FS Wakeup through EXTI line */
INTERRUPT void          FPUirqHnd();    /* Floating point unit interrupt */

INTERRUPT void       DMA1_0irqHnd();
INTERRUPT void       DMA1_1irqHnd();
INTERRUPT void       DMA1_2irqHnd();
INTERRUPT void       DMA1_3irqHnd();
INTERRUPT void       DMA1_4irqHnd();
INTERRUPT void       DMA1_5irqHnd();
INTERRUPT void       DMA1_6irqHnd();
INTERRUPT void       DMA1_7irqHnd();
INTERRUPT void       DMA2_0irqHnd();
INTERRUPT void       DMA2_1irqHnd();
INTERRUPT void       DMA2_2irqHnd();
INTERRUPT void       DMA2_3irqHnd();
INTERRUPT void       DMA2_4irqHnd();
INTERRUPT void       DMA2_5irqHnd();
INTERRUPT void       DMA2_6irqHnd();
INTERRUPT void       DMA2_7irqHnd();

INTERRUPT void       USART1irqHnd();
INTERRUPT void       USART2irqHnd();
INTERRUPT void       USART3irqHnd();
INTERRUPT void       USART6irqHnd();

INTERRUPT void       ADCirqHnd();           /* ADC1, ADC2 and ADC3s         */

INTERRUPT void       OTG_HSep1OUTirqHnd();  /*    74 OTG HS End Point 1 Out global interrupt */
INTERRUPT void       OTG_HSep1INirqHnd();   /*    75 OTG HS End Point 1 In global interrupt  */
INTERRUPT void       OTG_HSWakeUPirqHnd();  /*    76 OTG HS Wakeup through EXTI interrupt    */
INTERRUPT void       OTG_HSirqHnd();        /*    77 OTG HS global interrupt                 */





#endif
