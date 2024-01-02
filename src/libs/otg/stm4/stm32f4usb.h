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
 *     FILE: stm32f4usb.h
 *     DATE: sep 2023
 *
 *  DESCRIPCION: device peripheral tree
 */

#ifndef STM32F4USB_H_INCLUDED
#define STM32F4USB_H_INCLUDED

#include "uc.h"

typedef enum
{ USB_EP_TYPE_CTRL= 0x00
, USB_EP_TYPE_ISOC= 0x01
, USB_EP_TYPE_BULK= 0x02
, USB_EP_TYPE_INTR= 0x03
} aaaaaaaaaaaa;

typedef enum
{ DEP0CTL_MPS_64= 0
, DEP0CTL_MPS_32= 1
, DEP0CTL_MPS_16= 2
, DEP0CTL_MPS_8 = 3
} bbbbbbbbbbbbb;

typedef enum
{ DCFG_FRAME_INTERVAL_80= 0
, DCFG_FRAME_INTERVAL_85= 1
, DCFG_FRAME_INTERVAL_90= 2
, DCFG_FRAME_INTERVAL_95= 3
} ccccccccccccc;

typedef enum
{ STS_GOUT_NAK  = 1
, STS_DATA_UPDT = 2
, STS_XFER_COMP = 3
, STS_SETUP_COMP= 4
, STS_SETUP_UPDT= 6
} ddddddddddddddddd;

typedef enum
{ HCCHAR_CTRL= 0
, HCCHAR_ISOC= 1
, HCCHAR_BULK= 2
, HCCHAR_INTR= 3
} eeeeeeeeeeeeeeeee;

typedef enum
{ DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ= 0
, DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ= 1
, DSTS_ENUMSPD_LS_PHY_6MHZ          = 2
, DSTS_ENUMSPD_FS_PHY_48MHZ         = 3
} eeeeeeeeeeeeeeeeeeeee;

typedef enum
{ GRXSTS_PKTSTS_IN             = 2
, GRXSTS_PKTSTS_IN_XFER_COMP   = 3
, GRXSTS_PKTSTS_DATA_TOGGLE_ERR= 5
, GRXSTS_PKTSTS_CH_HALTED      = 7
} ffffffffffff;

typedef enum
{ HCFG_30_60_MHZ= 0
, HCFG_48_MHZ   = 1
, HCFG_6_MHZ    = 2
} ggggggggg;


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   STM32_USB_DEVICE : USB on the go full speed                 @ 0x50000800 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
union STM32_USB_DEVICE$DCFG
{ volatile struct
  { dword       DSPD :   2; /** 0x00 Device speed */
    dword   NZLSOHSK :   1; /** 0x02 Non-zero-length status OUT handshake */
    dword            :   1; /** 0x03 */
    dword        DAD :   7; /** 0x04 Device address */
    dword      PFIVL :   2; /** 0x0B Periodic frame interval */
    dword            :  19; /** 0x0D */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DCTL
{ volatile struct
  { dword     RWUSIG :   1; /** 0x00 Remote wakeup signaling */
    dword       SDIS :   1; /** 0x01 Soft disconnect */
    dword     GINSTS :   1; /** 0x02 Global IN NAK status */
    dword     GONSTS :   1; /** 0x03 Global OUT NAK status */
    dword       TCTL :   3; /** 0x04 Test control */
    dword     SGINAK :   1; /** 0x07 Set global IN NAK */
    dword     CGINAK :   1; /** 0x08 Clear global IN NAK */
    dword     SGONAK :   1; /** 0x09 Set global OUT NAK */
    dword     CGONAK :   1; /** 0x0A Clear global OUT NAK */
    dword   POPRGDNE :   1; /** 0x0B Power-on programming done */
    dword            :  20; /** 0x0C */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DSTS
{ volatile struct
  { dword    SUSPSTS :   1; /** 0x00 Suspend status */
    dword    ENUMSPD :   2; /** 0x01 Enumerated speed */
    dword       EERR :   1; /** 0x03 Erratic error */
    dword            :   4; /** 0x04 */
    dword      FNSOF :  14; /** 0x08 Frame number of the received SOF */
    dword            :  10; /** 0x16 */
  };

  volatile dword atomic;            /** atomic access */
};


union STM32_USB_DEVICE$DIEPINT
{ volatile struct
  { dword       XFRC :   1; /** 0x00 XFRC */
    dword     EPDISD :   1; /** 0x01 EPDISD */
    dword  AHBERR_NW :   1; /** 0x02 */
    dword        TOC :   1; /** 0x03 TOC */
    dword     ITTXFE :   1; /** 0x04 ITTXFE */
    dword     INEPNM :   1; /** 0x05 */
    dword     INEPNE :   1; /** 0x06 INEPNE */
    dword       TXFE :   1; /** 0x07 TXFE */
    dword TXFIFOUN_NW:   1;
    dword            :  23; /** 0x08 */
  };

  volatile dword atomic;            /** atomic access */
};


union STM32_USB_DEVICE$DIEPMSK
{ volatile struct
  { dword      XFRCM :   1; /** 0x00 Transfer completed interrupt mask */
    dword    EPDISDM :   1; /** 0x01 Endpoint disabled interrupt mask */
    dword AHBERRM_NW :   1; /** 0x02 */
    dword       TOCM :   1; /** 0x03 Timeout condition mask (Non-isochronous endpoints) */
    dword    ITTXFEM :   1; /** 0x04 IN token received when TxFIFO empty mask */
    dword    INEPNMM :   1; /** 0x05 IN token received with EP mismatch mask */
    dword    INEPNEM :   1; /** 0x06 IN endpoint NAK effective mask */
    dword      TXFEM :   1; /** 0x07 TXFE */
    dword TXFIFOUNM_NW:  1;
    dword            :  23; /** 0x08 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DOEPINT
{ volatile struct
  { dword       XFRC:   1; /** 0x00 XFRC    */
    dword     EPDISD:   1; /** 0x01 EPDISD  */
    dword  AHBERR_NW:   1; /** 0x02         */
    dword       STUP:   1; /** 0x03 STUP    */
    dword    OTEPDIS:   1; /** 0x04 OTEPDIS */
    dword           :   1; /** 0x05         */
    dword    B2BSTUP:   1; /** 0x06 B2BSTUP */
    dword           :  25; /** 0x07         */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DOEPMSK
{ volatile struct
  { dword      XFRCM :   1; /** 0x00 Transfer completed interrupt mask */
    dword    EPDISDM :   1; /** 0x01 Endpoint disabled interrupt mask */
    dword AHBERRM_NW :   1; /** 0x02 */
    dword      STUPM :   1; /** 0x03 SETUP phase done mask */
    dword     OTEPDM :   1; /** 0x04 OUT token received when endpoint disabled mask */
    dword            :  27; /** 0x05 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DAINT
{ volatile struct
  { dword     IEPINT :  16; /** 0x00 IN endpoint interrupt bits */
    dword     OEPINT :  16; /** 0x10 OUT endpoint interrupt bits */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DAINTMSK
{ volatile struct
  { dword     IEPINT :  16; /** 0x00 IN EP interrupt mask bits */
    dword     OEPINT :  16; /** 0x10 OUT endpoint interrupt bits */
  };

  volatile dword atomic;    /** atomic access */
};

union STM32_USB_DEVICE$DVBUSDIS
{ volatile struct
  { dword     VBUSDT :  16; /** 0x00 Device VBUS discharge time */
    dword            :  16; /** 0x10 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DVBUSPULSE
{ volatile struct
  { dword     DVBUSP :  12; /** 0x00 Device VBUS pulsing time */
    dword            :  20; /** 0x0C */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DTHRCTL
{ volatile struct
  { dword NISOEN_NW: 1;
    dword ISOEN_NW : 1;
    dword TXLEN_NW : 9;
    dword R11_15_NW: 5;
    dword RXEN_NW  : 1;
    dword RXLEN_NW : 9;
    dword R26_31_NW: 6;
  };

  volatile dword atomic;            /** atomic access */
};




union STM32_USB_DEVICE$DIEPEMPMSK
{ volatile struct
  { dword  INEPTXFEM :  16; /** 0x00 IN EP Tx FIFO empty interrupt mask bits */
    dword            :  16; /** 0x10 */
  };
};


union STM32_USB_DEVICE$DOIEPCTL
{ volatile struct
  { dword          MPSIZ :  11; /** 0x00 MPSIZ */
    dword                :   4; /** 0x0B */
    dword         USBAEP :   1; /** 0x0F USBAEP */
    dword     EONUM_DPID :   1; /** 0x10 EONUM/DPID */
    dword         NAKSTS :   1; /** 0x11 NAKSTS */
    dword          EPTYP :   2; /** 0x12 EPTYP */
    dword       snp      :   1; /** 0x14 */
    dword          STALL :   1; /** 0x15 Stall */
    dword         TXFNUM :   4; /** 0x16 TXFNUM */
    dword           CNAK :   1; /** 0x1A CNAK */
    dword           SNAK :   1; /** 0x1B SNAK */
    dword SD0PID_SEVNFRM :   1; /** 0x1C SD0PID/SEVNFRM */
    dword SD1PID_SODDFRM :   1; /** 0x1D SODDFRM/SD1PID */
    dword          EPDIS :   1; /** 0x1E EPDIS */
    dword          EPENA :   1; /** 0x1F EPENA */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DIEPTSIZ
{ volatile struct
  { dword   XFRSIZ:  19; /** 0x00 Transfer size ( 7 used )*/
    dword   PKTCNT:  10; /** 0x13 Packet count  */
    dword MULTICNT:   2; /** 0x29 ISO multicout  */
    dword         :   1; /**                    */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DOEPTSIZ
{ volatile struct
  { dword     XFRSIZ :  19; /** 0x00 Transfer size ( 7 used )*/
    dword     PKTCNT :   2; /** 0x13 Packet count */
    dword            :   9; /** 0x14 */
    dword    STUPCNT :   2; /** 0x1D SETUP packet count */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_DEVICE$DTXFSTS
{ volatile struct
  { dword  INEPTFSAV :  16; /** 0x00 IN endpoint TxFIFO space available */
    dword            :  16; /** 0x10 */
  };

  volatile dword atomic;            /** atomic access */
};

struct STM32_USB_DEVICE_ST                               /** 0x50000800 USB on the go full speed */
{ volatile union STM32_USB_DEVICE$DCFG      DCFG;        /** 0x00 RST: 0x02200000 OTG_FS device configuration register (OTG_DCFG) */
  volatile union STM32_USB_DEVICE$DCTL      DCTL;        /** 0x04 RST: 0x00000000 OTG_FS device control register (OTG_DCTL) */
  volatile union STM32_USB_DEVICE$DSTS      DSTS;        /** 0x08 RST: 0x00000010 OTG_FS device status register (OTG_DSTS) */
   dword            gap$0C[  1 ];                        /** 0x0C Unused register */
  volatile union STM32_USB_DEVICE$DIEPMSK   DIEPMSK;     /** 0x10 RST: 0x00000000 OTG_FS device IN endpoint common interrupt mask register (OTG_DIEPMSK) */
  volatile union STM32_USB_DEVICE$DOEPMSK   DOEPMSK;     /** 0x14 RST: 0x00000000 OTG_FS device OUT endpoint common interrupt mask register (OTG_DOEPMSK) */

  volatile union STM32_USB_DEVICE$DAINT     DAINT;       /** 0x18 RST: 0x00000000 OTG_FS device all endpoints interrupt register (OTG_DAINT) */
  volatile union STM32_USB_DEVICE$DAINTMSK  DAINTMSK;    /** 0x1C RST: 0x00000000 OTG_FS all endpoints interrupt mask register (OTG_DAINTMSK) */
   dword            gap$20[  2 ];                        /** 0x20 Unused register */

  volatile union STM32_USB_DEVICE$DVBUSDIS   DVBUSDIS;    /** 0x28 RST: 0x000017D7 OTG_FS device VBUS discharge time register */
  volatile union STM32_USB_DEVICE$DVBUSPULSE DVBUSPULSE;  /** 0x2C RST: 0x000005B8 OTG_FS device VBUS pulsing time register */
  volatile union STM32_USB_DEVICE$DTHRCTL    DTHRCTL_NW;  /** 0x30 Unused register -> i2c control */
  volatile union STM32_USB_DEVICE$DIEPEMPMSK DIEPEMPMSK;  /** 0x34 RST: 0x00000000 OTG_FS device IN endpoint FIFO empty interrupt mask register */

  volatile union STM32_USB_DEVICE$DAINT      DEACHINT_NW; /** 0x38 RST: 0x00000000 OTG_FS device all endpoints interrupt register (OTG_DAINT) */
  volatile union STM32_USB_DEVICE$DAINT      DEACHMSK_NW; /** 0x3C RST: 0x00000000 OTG_FS device all endpoints interrupt register (OTG_DAINT) */

   dword Reserved40[ 1 ];                                  /** 0x40       */
   volatile union STM32_USB_DEVICE$DAINT DINEP1MSK_NW;  /** 0x44       */
   dword  Reserved44[15];                                  /** 0x48 87Ch  */
   volatile union STM32_USB_DEVICE$DAINT DOUTEP1MSK_NW; /** 0x84       */

   dword            gap$88[ 30 ];                       /** 0x88 120->78h Unused register */

  volatile struct
  { volatile union STM32_USB_DEVICE$DOIEPCTL    CTL; dword gap$104;  /** 0x100 RST: 0x00000000 OTG_FS device control IN endpoint 0 control register (OTG_FS_DIEPCTL0) */
    volatile union STM32_USB_DEVICE$DIEPINT     INT; dword gap$10C;  /** 0x108 RST: 0x00000080 device endpoint-x interrupt register */
    volatile union STM32_USB_DEVICE$DIEPTSIZ   TSIZ;                 /** 0x110 RST: 0x00000000 device endpoint-0 transfer size register */
    volatile                           dword    DMA;                 /** 0x114 DMA ADDR */
    volatile union STM32_USB_DEVICE$DTXFSTS DTXFSTS; dword gap$11C;  /** 0x118 RST: 0x00000000 OTG_FS device IN endpoint transmit FIFO status register */
  } DIEP[ 8 ];

  volatile struct
  { volatile union STM32_USB_DEVICE$DOIEPCTL    CTL; dword gap$104;  /** 0x100 RST: 0x00000000 OTG_FS device control IN endpoint 0 control register (OTG_FS_DIEPCTL0) */
    volatile union STM32_USB_DEVICE$DIEPINT     INT; dword gap$10C;  /** 0x108 RST: 0x00000080 device endpoint-x interrupt register */
    volatile union STM32_USB_DEVICE$DIEPTSIZ   TSIZ;                 /** 0x110 RST: 0x00000000 device endpoint-0 transfer size register */
    volatile                        dword    DMA;                    /** 0x114 DMA ADDR */
    volatile union STM32_USB_DEVICE$DTXFSTS DTXFSTS; dword gap$11C;  /** 0x118 RST: 0x00000000 OTG_FS device IN endpoint transmit FIFO status register */
  } DPP[ 8 ];

  volatile struct
  { volatile union STM32_USB_DEVICE$DOIEPCTL  CTL; dword gap$304;    /** 0x300 RST: 0x00008000 device endpoint-0 control register */
    volatile union STM32_USB_DEVICE$DOEPINT  INT;  dword gap$30C;    /** 0x308 RST: 0x00000080 device endpoint-0 interrupt register */
    volatile union STM32_USB_DEVICE$DOEPTSIZ TSIZ;                   /** 0x310 RST: 0x00000000 device OUT endpoint-0 transfer size register */
    volatile                      dword    DMA; dword gap$318[2];    /** 0x114 DMA ADDR */
  } DOEP[ 8 ];
};

#define EPOUT_OFFSET 16

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   STM32_USB_GLOBAL : USB on the go full speed                 @ 0x50000000 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
union STM32_USB_GLOBAL$GOTGCTL
{ volatile struct
  { dword     SRQSCS :   1; /** 0x00 Session request success */
    dword        SRQ :   1; /** 0x01 Session request */
    dword   VBVALOEN :   1; /**     0x02 VBUS valid override enable */
    dword  VBVALOVAL :   1; /**     0x03 VBUS valid override value */
    dword    AVALOEN :   1; /**     0x04 A-peripheral session valid override enable */
    dword   AVALOVAL :   1; /**     0x05 A-peripheral session valid override value */
    dword    BVALOEN :   1; /**     0x06 B-peripheral session valid override enable */
    dword   BVALOVAL :   1; /**     0x07 B-peripheral session valid override value */
    dword     HNGSCS :   1; /** 0x08 Host negotiation success */
    dword      HNPRQ :   1; /** 0x09 HNP request */
    dword    HSHNPEN :   1; /** 0x0A Host set HNP enable */
    dword     DHNPEN :   1; /** 0x0B Device HNP enabled */
    dword      EHEN7 :   1; /** 0x0C Embedded host enable */
    dword            :   3; /** 0x0D */
    dword     CIDSTS :   1; /** 0x10 Connector ID status */
    dword       DBCT :   1; /** 0x11 Long/short debounce time */
    dword      ASVLD :   1; /** 0x12 A-session valid */
    dword      BSVLD :   1; /** 0x13 B-session valid */
    dword     OTGVER :   1; /**     0x14 OTG version */
    dword            :  11; /** 0x15 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GOTGINT
{ volatile struct
  { dword            :   2; /** 0x00 */
    dword      SEDET :   1; /** 0x02 Session end detected */
    dword            :   5; /** 0x03 */
    dword    SRSSCHG :   1; /** 0x08 Session request success status change */
    dword    HNSSCHG :   1; /** 0x09 Host negotiation success status change */
    dword            :   7; /** 0x0A */
    dword     HNGDET :   1; /** 0x11 Host negotiation detected */
    dword    ADTOCHG :   1; /** 0x12 A-device timeout change */
    dword     DBCDNE :   1; /** 0x13 Debounce done */
    dword            :  12; /** 0x14 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GAHBCFG
{ volatile struct
  { dword       GINT :  1; /** 0x00 Global interrupt mask */
    dword  HBURSTLEN1 :  4;
    dword  DMAENABLE1 :  1;
    dword        Res :  1;
    dword    TXFELVL :  1; /** 0x07 TxFIFO empty level */
    dword   PTXFELVL :  1; /** 0x08 Periodic TxFIFO empty level */
    dword            : 23; /** 0x09 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GUSBCFG
{ volatile struct
  { dword      TOCAL :   3; /** 0x00 FS timeout calibration */
    dword            :   3; /** 0x03 */
    dword     PHYSEL :   1; /** 0x06 USB 2.0 high-speed ULPI PHY or USB 1.1 full-speed serial transceiver select */
    dword            :   1; /** 0x07 */
    dword     SRPCAP :   1; /** 0x08 SRP-capable */
    dword     HNPCAP :   1; /** 0x09 HNP-capable */
    dword       TRDT :   4; /** 0x0A USB turnaround time */
    dword            :   1; /** 0x0E */
    dword    PHYLPCS :   1; /** 0x0F PHY Low-power clock select */
    dword            :   1; /** 0x10 */
    dword   ULPIFSLS :   1; /** 0x11 ULPI FS/LS select */
    dword     ULPIAR :   1; /** 0x12 ULPI Auto-resume */
    dword    ULPICSM :   1; /** 0x13 ULPI Clock SuspendM */
    dword ULPIEVBUSD :   1; /** 0x14 ULPI External VBUS Drive */
    dword ULPIEVBUSI :   1; /** 0x15 ULPI external VBUS indicator */
    dword      TSDPS :   1; /** 0x16 TermSel DLine pulsing selection */
    dword       PCCI :   1; /** 0x17 Indicator complement */
    dword       PTCI :   1; /** 0x18 Indicator pass through */
    dword    ULPIIPD :   1; /** 0x19 ULPI interface protect disable */
    dword            :   3; /** 0x1A */
    dword      FHMOD :   1; /** 0x1D Forced host mode */
    dword      FDMOD :   1; /** 0x1E Forced peripheral mode */
    dword            :   1; /** 0x1F */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GRSTCTL
{ volatile struct
  { dword      CSRST :   1; /** 0x00 Core soft reset */
    dword      HSRST :   1; /** 0x01 HCLK soft reset */
    dword      FCRST :   1; /** 0x02 Host frame counter reset */
    dword            :   1; /** 0x03 */
    dword    RXFFLSH :   1; /** 0x04 RxFIFO flush */
    dword    TXFFLSH :   1; /** 0x05 TxFIFO flush */
    dword     TXFNUM :   5; /** 0x06 TxFIFO number */
    dword            :  20; /** 0x0B */
    dword     AHBIDL :   1; /** 0x1F AHB master idle */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GINTSTS
{ volatile struct
  { dword       CMOD  :   1; /** 0x00 Current mode of operation */
    dword       MMIS  :   1; /** 0x01 Mode mismatch interrupt */
    dword     OTGINT  :   1; /** 0x02 OTG interrupt */
    dword        SOF  :   1; /** 0x03 Start of frame */
    dword     RXFLVL  :   1; /** 0x04 RxFIFO non-empty */
    dword     NPTXFE  :   1; /** 0x05 Non-periodic TxFIFO empty */
    dword   GINAKEFF  :   1; /** 0x06 Global IN non-periodic NAK effective */
    dword GOUTNAKEFF  :   1; /** 0x07 Global OUT NAK effective */
    dword             :   2; /** 0x08 */
    dword      ESUSP  :   1; /** 0x0A Early suspend */
    dword    USBSUSP  :   1; /** 0x0B USB suspend */
    dword     USBRST  :   1; /** 0x0C USB reset */
    dword    ENUMDNE  :   1; /** 0x0D Enumeration done */
    dword    ISOODRP  :   1; /** 0x0E Isochronous OUT packet dropped interrupt */
    dword       EOPF  :   1; /** 0x0F End of periodic frame interrupt */
    dword             :   2; /** 0x10 */
    dword      IEPINT :  1; /** 0x12 IN endpoint interrupt */
    dword      OEPINT :  1; /** 0x13 OUT endpoint interrupt */
    dword INCOMPISOIN :  1; /** 0x14 Incomplete isochronous IN transfer */
    dword INCOMPISOOUT:  1; /** 0x15 Incomplete periodic transfer(Host mode)/Incomplete isochronous OUT transfer(Device mode) */
    dword             :   2; /** 0x16 */
    dword    HPRTINT  :   1; /** 0x18 Host port interrupt */
    dword      HCINT  :   1; /** 0x19 Host channels interrupt */
    dword      PTXFE  :   1; /** 0x1A Periodic TxFIFO empty */
    dword             :   1; /** 0x1B */
    dword     CIDSCHG :   1; /** 0x1C Connector ID status change                     */
    dword     DISCINT :   1; /** 0x1D Disconnect detected interrupt                  */
    dword      SRQINT :   1; /** 0x1E Session request/new session detected interrupt */
    dword     WKUPINT :   1; /** 0x1F Resume/remote wakeup detected interrupt        */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$GRXSTSR
{ volatile struct
  { dword      EPNUM :   4; /** 0x00 Endpoint number */
    dword       BCNT :  11; /** 0x04 Byte count      */
    dword       DPID :   2; /** 0x0F Data PID        */
    dword     PKTSTS :   4; /** 0x11 Packet status   */
    dword     FRMNUM :   4; /** 0x15 Frame number    */
    dword            :   7; /** 0x19 */
  };

  volatile dword atomic;    /** atomic access */
};

struct STM32_USB_GLOBAL$GRXFSIZ
{ dword  RXFD :  16; /** 0x00 RxFIFO depth */
  dword       :  16; /** 0x10              */
};

union STM32_USB_GLOBAL$GI2CCTL
{ volatile struct
  { dword RWDATA_NW    : 8;
    dword REGADDR_NW   : 8;
    dword ADDR_NW      : 7;
    dword I2CEN_NW     : 1;
    dword ACK_NW       : 1;
    dword I2CSUSPCTL_NW: 1;
    dword I2CDEVADDR_NW: 2;
    dword DATSE0_NW    : 1;
    dword Reserved     : 1;
    dword RW_NW        : 1;
    dword BSUDNE_NW    : 1;
  };

  volatile dword atomic;            /** atomic access */
};

//union STM32_USB_GLOBAL$GNPTXFSIZ_DEV
//{ volatile struct
//  { dword     TX0FSA :  16; /** 0x00 Endpoint 0 transmit RAM start address */
//    dword      TX0FD :  16; /** 0x10 Endpoint 0 TxFIFO depth */
//  };
//
//  volatile dword atomic;            /** atomic access */
//};

union STM32_USB_GLOBAL$GNPTXSTS
{ volatile struct
  { dword   NPTXFSAV :  16; /** 0x00 Non-periodic TxFIFO space available */
    dword   NPTQXSAV :   8; /** 0x10 Non-periodic transmit request queue space available */
//    dword   NPTXQTOP :   7; /** 0x18 Top of the non-periodic transmit request queue */

    dword TERMINATE_NW: 1;
    dword TOKEN_NW    : 2;
    dword CHNUM_NW    : 4;

    dword             : 1; /** 0x1F */
  };

  volatile dword atomic;            /** atomic access */
};


//   dword      DCDET :   1; /** 0x00 Data contact detection (DCD) status */
//    dword       PDET :   1; /** 0x01 Primary detection (PD) status */
//    dword       SDET :   1; /** 0x02 Secondary detection (SD) status *///
//  dword     PS2DET :   1; /** 0x03 DM pull-up detection status */
//    dword            :  12; /** 0x04 *///
//    dword     PWRDWN :   1; /** 0x10 Power down */
//    dword      BCDEN :   1; /** 0x11 Battery charging detector (BCD) enable */
//    dword      DCDEN :   1; /** 0x12 Data contact detection (DCD) mode enable */
//    dword       PDEN :   1; /** 0x13 Primary detection (PD) mode enable */
//    dword       SDEN :   1; /** 0x14 Secondary detection (SD) mode enable */
//    dword      VBDEN :   1; /** 0x15 USB VBUS detection enable */

union STM32_USB_GLOBAL$GCCFG
{ volatile struct
  { dword            :  16; /** 0x00 */
    dword     PWRDWN :   1; /** 0x10 Power down */
    dword       BCDEN :   1; /** 0x11 Battery charging detector (BCD) enable */
    dword   VBUSASEN :   1; /** DCDEN 0x12 Enable the VBUS sensing device */
    dword   VBUSBSEN :   1; /** PDEN 0x13 Enable the VBUS sensing device */
    dword   SOFOUTEN :   1; /** SDEN 0x14 SOF output enable */
    dword   NOVBUSEN :   1; // VBDEN :   1; /** 0x15 USB VBUS detection enable */
    dword            :  10; /** 0x15 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_GLOBAL$CID
{ volatile struct
  { dword PRODUCT_ID :  32; /** 0x00 Product ID field */
  };

  volatile dword atomic;    /** atomic access */
};

struct STM32_USB_GLOBAL$FIFOMEM
{ word TXSA; /** 0x00 IN endpoint FIFO2 transmit RAM start address */
  word TXFD; /** 0x10 IN endpoint TxFIFO depth */
};

struct STM32_USB_GLOBAL_ST                          /** 0x50000000 USB on the go full speed */
{ volatile union STM32_USB_GLOBAL$GOTGCTL GOTGCTL;  /** 0x00 RST: 0x00000800 OTG_FS control and status register (OTG_GOTGCTL) */
  volatile union STM32_USB_GLOBAL$GOTGINT GOTGINT;  /** 0x04 RST: 0x00000000 OTG_FS interrupt register (OTG_GOTGINT) */
  volatile union STM32_USB_GLOBAL$GAHBCFG GAHBCFG;  /** 0x08 RST: 0x00000000 OTG_FS AHB configuration register (OTG_GAHBCFG) */
  volatile union STM32_USB_GLOBAL$GUSBCFG GUSBCFG;  /** 0x0C RST: 0x00000A00 OTG_FS USB configuration register (OTG_GUSBCFG) */
  volatile union STM32_USB_GLOBAL$GRSTCTL GRSTCTL;  /** 0x10 RST: 0x20000000 OTG_FS reset register (OTG_FS_GRSTCTL) */

  volatile union STM32_USB_GLOBAL$GINTSTS GINTSTS;  /** 0x14 RST: 0x04000020 OTG_FS core interrupt register (OTG_FS_GINTSTS) */
  volatile union STM32_USB_GLOBAL$GINTSTS GINTMSK;  /** 0x18 RST: 0x00000000 OTG_FS interrupt mask register (OTG_FS_GINTMSK) */

  volatile union STM32_USB_GLOBAL$GRXSTSR GRXSTSR; /** 0x1C RST: 0x00000000 OTG_FS Receive status debug read(Host mode) */
  volatile union STM32_USB_GLOBAL$GRXSTSR GRXSTSP; /** 0x20 RST: 0x00000000 OTG_FS Receive status debug read(Host mode) */
  volatile struct STM32_USB_GLOBAL$GRXFSIZ GRXFSIZ; /** 0x24 RST: 0x00000200 OTG_FS Receive FIFO size register (OTG_GRXFSIZ) */

  volatile struct STM32_USB_GLOBAL$FIFOMEM GNPTXFSIZ;  /** 0x28 RST: 0x00000200 OTG_FS non-periodic transmit FIFO size register (Host mode) */
                                                       /** 0x28 RST: 0x00000200 OTG_FS non-periodic transmit FIFO size register (Device mode) */

  volatile union STM32_USB_GLOBAL$GNPTXSTS GNPTXSTS;   /** 0x2C RST: 0x00080200 OTG_FS non-periodic transmit FIFO/queue status register (OTG_GNPTXSTS) */
  volatile union STM32_USB_GLOBAL$GI2CCTL  GI2CCTL_NW; /** 0x30 Unused register -> i2c control */
  dword            gap$30[  1 ];                       /** 0x34  PHY Vendor Control Register        034h */

  volatile union STM32_USB_GLOBAL$GCCFG GCCFG;           /** 0x38 RST: 0x00000000 OTG_FS general core configuration register (OTG_GCCFG) */
  volatile union STM32_USB_GLOBAL$CID     CID;           /** 0x3C RST: 0x00001000 core ID register */
   dword            gap$40[ 48 ];                        /** 0x40 Unused register */

  volatile struct STM32_USB_GLOBAL$FIFOMEM HPTXFSIZ[ 1 ]; /** 0x100 RST: 0x02000600 OTG_FS Host periodic transmit FIFO size register (OTG_HPTXFSIZ) */
  volatile struct STM32_USB_GLOBAL$FIFOMEM DIEPTXF[  3 ]; /** 0x104 RST: 0x02000400 OTG_FS device IN endpoint transmit FIFO size register (OTG_DIEPTXF2) */

   dword          gap$110[ 188 ];    /** 0x110 Unused memory */
};


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   STM32_USB_HOST : USB on the go full speed                 @ 0x50000400 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
union STM32_USB_HOST$HCFG
{ volatile struct
  { dword    FSLSPCS :   2; /** 0x00 FS/LS PHY clock select */
    dword      FSLSS :   1; /** 0x02 FS- and LS-only support */
    dword            :  29; /** 0x03 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$HFIR
{ volatile struct
  { dword      FRIVL :  16; /** 0x00 Frame interval */
    dword            :  16; /** 0x10 */
  };

  volatile dword atomic;    /** atomic access */
};

union STM32_USB_HOST$FS_HFNUM
{ volatile struct
  { dword      FRNUM :  16; /** 0x00 Frame number */
    dword      FTREM :  16; /** 0x10 Frame time remaining */
  };

  volatile dword atomic;     /** atomic access */
};

union STM32_USB_HOST$HPTXSTS
{ volatile struct
  { dword PTXFSAVL:  16; /** 0x00 Periodic transmit data FIFO space available */
    dword PTXQSAV :   8; /** 0x10 Periodic transmit request queue space available */
    dword PTXQTOP :   8; /** 0x18 Top of the periodic transmit request queue */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$HAINT
{ volatile struct
  { dword      HAINT :  16; /** 0x00 Channel interrupts */
    dword            :  16; /** 0x10 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$HAINTMSK
{ volatile struct
  { dword     HAINTM :  16; /** 0x00 Channel interrupt mask */
    dword            :  16; /** 0x10 */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$HPRT
{ volatile struct
  { dword      PCSTS :   1; /** 0x00 Port connect status        */
    dword      PCDET :   1; /** 0x01 Port connect detected      */
    dword       PENA :   1; /** 0x02 Port enable                */
    dword    PENCHNG :   1; /** 0x03 Port enable/disable change */
    dword       POCA :   1; /** 0x04 Port overcurrent active    */
    dword    POCCHNG :   1; /** 0x05 Port overcurrent change    */
    dword       PRES :   1; /** 0x06 Port resume                */
    dword      PSUSP :   1; /** 0x07 Port suspend               */
    dword       PRST :   1; /** 0x08 Port reset                 */
    dword            :   1; /** 0x09                            */
    dword      PLSTS :   2; /** 0x0A Port line status           */
    dword       PPWR :   1; /** 0x0C Port power                 */
    dword      PTCTL :   4; /** 0x0D Port test control          */
    dword       PSPD :   2; /** 0x11 Port speed                 */
    dword            :  13; /** 0x13                            */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$FS_HCCHAR
{ volatile struct
  { dword      MPSIZ :  11; /** 0x00 Maximum packet size */
    dword      EPNUM :   4; /** 0x0B Endpoint number     */
    dword      EPDIR :   1; /** 0x0F Endpoint direction  */
    dword            :   1; /** 0x10                     */
    dword      LSDEV :   1; /** 0x11 Low-speed device    */
    dword      EPTYP :   2; /** 0x12 Endpoint type       */
    dword       MCNT :   2; /** 0x14 Multicount          */
    dword        DAD :   7; /** 0x16 Device address      */
    dword     ODDFRM :   1; /** 0x1D Odd frame           */
    dword      CHDIS :   1; /** 0x1E Channel disable     */
    dword      CHENA :   1; /** 0x1F Channel enable      */
  };

  volatile dword atomic;            /** atomic access */
};

union STM32_USB_HOST$HCINT
{ volatile struct
  { dword       XFRC :  1; /** 0x00 Transfer completed */
    dword      CHHLT :  1; /** 0x01 Channel halted */
    dword  AHBERR_NW :  1; /** 0x02 */
    dword      STALL :  1; /** 0x03 STALL response received interrupt */
    dword        NAK :  1; /** 0x04 NAK response received interrupt */
    dword        ACK :  1; /** 0x05 ACK response received/transmitted interrupt */
    dword       NYET :  1; /** 0x06 */
    dword    XACTERR :  1; /** 0x07 Transaction error */
    dword      BBERR :  1; /** 0x08 Babble error */
    dword    FRMOVRR :  1; /** 0x09 Frame overrun */
    dword    DTGLERR :  1; /** 0x0A Data toggle error */
    dword            : 21; /** 0x0B */
  };

  volatile dword atomic;            /** atomic access */
};



union STM32_USB_HOST$HCTSIZ
{ volatile struct
  { dword     XFRSIZ :  19; /** 0x00 Transfer size */
    dword     PKTCNT :  10; /** 0x13 Packet count */
    dword       DPID :   2; /** 0x1D Data PID */
    dword   DOPNG_NW :   1; /** 0x1F */
  };

  volatile dword atomic;            /** atomic access */
};


struct HC_STRUCT
{ volatile union STM32_USB_HOST$FS_HCCHAR CHAR; /** 0x100 RST: 0x00000000 OTG_FS host channel-0 characteristics register (OTG_FS_HCCHAR0) */
   dword                                  SPLT; /** 0x104 Unused register */
  volatile union STM32_USB_HOST$HCINT      INT; /** 0x108 RST: 0x00000000 OTG_FS host channel-0 interrupt register (OTG_HCINT0) */
  volatile union STM32_USB_HOST$HCINT   INTMSK; /** 0x10C RST: 0x00000000 OTG_FS host channel-0 mask register (OTG_HCINTMSK0) */
  volatile union STM32_USB_HOST$HCTSIZ    TSIZ; /** 0x110 RST: 0x00000000 OTG_FS host channel-0 transfer size register */
   dword                                   DMA; /** 0x104 Unused register */
   dword                         gap$114[  2 ]; /** 0x114 Unused register */
};

#define MAX_ENDPOINT_CHANNELS 8

struct STM32_USB_HOST_ST                           /** 0x50000400 USB on the go full speed */
{ volatile union STM32_USB_HOST$HCFG     HCFG;     /** 0x00 RST: 0x00000000 OTG_FS host configuration register (OTG_HCFG) */
  volatile union STM32_USB_HOST$HFIR     HFIR;     /** 0x04 RST: 0x0000EA60 OTG_FS Host frame interval register */
  volatile union STM32_USB_HOST$FS_HFNUM HFNUM;    /** 0x08 RST: 0x00003FFF OTG_FS host frame number/frame time remaining register (OTG_FS_HFNUM) */
   dword                 gap$0C[  1 ];             /** 0x0C Unused register */
  volatile union STM32_USB_HOST$HPTXSTS  HPTXSTS;  /** 0x10 RST: 0x00080100 STM32_USB_HOST periodic transmit FIFO/queue status register (OTG_HPTXSTS) */
  volatile union STM32_USB_HOST$HAINT    HAINT;    /** 0x14 RST: 0x00000000 OTG_FS Host all channels interrupt register */
  volatile union STM32_USB_HOST$HAINTMSK HAINTMSK; /** 0x18 RST: 0x00000000 OTG_FS host all channels interrupt mask register */
   dword            gap$1C[  9 ];                  /** 0x1C Unused register */
  volatile union STM32_USB_HOST$HPRT   HPRT      ; /** 0x40 RST: 0x00000000 OTG_FS host port control and status register (OTG_FS_HPRT) */
   dword            gap$44[ 47 ];                  /** 0x44 Unused register */

  volatile struct HC_STRUCT HC[ MAX_ENDPOINT_CHANNELS ];  /** Endpoint channels */

   dword          gap$1F4[ 128 ];    /** 0x1F4 Unused memory */
};

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   OTG_FS_PWRCLK : USB on the go full speed                 @ 0x50000E00 : 0x0400
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
union OTG_FS_PWRCLK$FS_PCGCCTL
{ volatile struct
  { dword    STPPCLK :   1; /** 0x00 Stop PHY clock */
    dword   GATEHCLK :   1; /** 0x01 Gate HCLK */
    dword            :   2; /** 0x02 */
    dword    PHYSUSP :   1; /** 0x04 PHY Suspended */
    dword            :  27; /** 0x05 */
  };

  volatile dword atomic;            /** atomic access */
};

struct STM32_USB_PWRCLK_ST            /** 0x50000E00 USB on the go full speed */
{ volatile union OTG_FS_PWRCLK$FS_PCGCCTL FS_PCGCCTL; /** 0x00 RST: 0x00000000 OTG_FS power and clock gating control register */

   dword          gap$04[ 127 ];                      /** 0x04 Unused memory */
};


struct STM32_USB_ST
{ struct STM32_USB_GLOBAL_ST GLOBAL;  /** 0x50000000 USB on the go full speed */
  struct STM32_USB_HOST_ST   HOST;    /** 0x50000400 USB on the go */
  struct STM32_USB_DEVICE_ST DEVICE;  /** 0x50000800 USB on the go full speed */
  dword  RESERVED_GAP[ 0x80 ];
  struct STM32_USB_PWRCLK_ST PWRCLK;  /** 0x50000E00 USB on the go full speed */
  dword  FIFOMEM[ 0x08 ][ 0x400 ];    /** 0x50001000 USB on the go full speed */
};


volatile struct
{ struct STM32_USB_ST USB;
}
extern STM32F4;



#endif



