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

INTERRUPT         WWDGirqHnd();     /* Window WatchDog              */
INTERRUPT          PVDirqHnd();     /* PVD through EXTI Line detection */
INTERRUPT             irqHnd();     /* Default orphan Handler       */
INTERRUPT          FPUirqHnd();     /* FPU interrupt */
INTERRUPT          RCCirqHnd();     /* RCC global interrupt */
INTERRUPT        EXTI0irqHnd();     /* EXTI Line0 interrupt */
INTERRUPT    RTC_AlarmirqHnd();     /* RTC Alarms (A and B) through EXTI line interrupt */
INTERRUPT     RTC_WKUPirqHnd();     /* RTC Wakeup interrupt through the EXTI line */
INTERRUPT        EXTI1irqHnd();     /* EXTI Line1 interrupt */
INTERRUPT        EXTI2irqHnd();     /* EXTI Line2 interrupt */
INTERRUPT        EXTI3irqHnd();     /* EXTI Line3 interrupt */
INTERRUPT        EXTI4irqHnd();     /* EXTI Line4 interrupt */
INTERRUPT      EXTI9_5irqHnd();     /* EXTI Line[9:5] interrupts */
INTERRUPT   TAMP_STAMPirqHnd();     /* Tamper and TimeStamp interrupts through the EXTI line */
INTERRUPT    EXTI15_10irqHnd();     /* EXTI Line[15:10] interrupts */

INTERRUPT      TIM1_CCirqHnd();    /* TIM1 capture compare */
INTERRUPT   TIM1_BRK_9irqHnd();    /* TIM1 Break interrupt and TIM9 global interrupt */
INTERRUPT   TIM1_UP_10irqHnd();    /* TIM1 Update interrupt and TIM10 global interrupt */
INTERRUPT  TIM1_TRG_11irqHnd();    /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt */
INTERRUPT         TIM2irqHnd();    /* TIM2 global interrupt */
INTERRUPT         TIM3irqHnd();    /* TIM3 global interrupt */
INTERRUPT         TIM4irqHnd();    /* TIM3 global interrupt */
INTERRUPT         TIM5irqHnd();    /* TIM3 global interrupt */
INTERRUPT         SPI1irqHnd();    /* SPI1 global interrupt */
INTERRUPT         SPI4irqHnd();    /* SPI1 global interrupt */
INTERRUPT      I2C1_EVirqHnd();    /* I2C1 event interrupt */
INTERRUPT      I2C2_EVirqHnd();    /* I2C1 event interrupt */
INTERRUPT      I2C3_EVirqHnd();    /* I2C3 event interrupt */
INTERRUPT      I2C1_ERirqHnd();    /* I2C1 error interrupt */
INTERRUPT      I2C2_ERirqHnd();    /* I2C2 error interrupt */
INTERRUPT      I2C3_ERirqHnd();    /* I2C3 event interrupt */
INTERRUPT         SPI2irqHnd();    /* SPI2 global interrupt */
INTERRUPT         SPI3irqHnd();    /* SPI3 global interrupt */
INTERRUPT        FLASHirqHnd();    /* FLASH global interrupt */
INTERRUPT         SDIOirqHnd();    /* SDIO global interrupt */
INTERRUPT       OTG_FSirqHnd();    /* USB On The Go FS global interrupt */
INTERRUPT  OTG_FS_WKUPirqHnd();    /* USB OTG FS Wakeup through EXTI line */
INTERRUPT          FPUirqHnd();    /* Floating point unit interrupt */

INTERRUPT       DMA1_0irqHnd();
INTERRUPT       DMA1_1irqHnd();
INTERRUPT       DMA1_2irqHnd();
INTERRUPT       DMA1_3irqHnd();
INTERRUPT       DMA1_4irqHnd();
INTERRUPT       DMA1_5irqHnd();
INTERRUPT       DMA1_6irqHnd();
INTERRUPT       DMA1_7irqHnd();
INTERRUPT       DMA2_0irqHnd();
INTERRUPT       DMA2_1irqHnd();
INTERRUPT       DMA2_2irqHnd();
INTERRUPT       DMA2_3irqHnd();
INTERRUPT       DMA2_4irqHnd();
INTERRUPT       DMA2_5irqHnd();
INTERRUPT       DMA2_6irqHnd();
INTERRUPT       DMA2_7irqHnd();

INTERRUPT       USART1irqHnd();
INTERRUPT       USART2irqHnd();
INTERRUPT       USART3irqHnd();
INTERRUPT       USART6irqHnd();

INTERRUPT       ADCirqHnd();           /* ADC1, ADC2 and ADC3s         */

INTERRUPT       OTG_HSep1OUTirqHnd();  /*    74 OTG HS End Point 1 Out global interrupt */
INTERRUPT       OTG_HSep1INirqHnd();   /*    75 OTG HS End Point 1 In global interrupt  */
INTERRUPT       OTG_HSWakeUPirqHnd();  /*    76 OTG HS Wakeup through EXTI interrupt    */
INTERRUPT       OTG_HSirqHnd();        /*    77 OTG HS global interrupt                 */





#endif
