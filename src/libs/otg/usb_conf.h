/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below:
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 *
 * @file usb_conf.h
 *
 * original draft from MCD Application Team
 */

#ifndef __USB_CONF__H__
#define __USB_CONF__H__


/*******************************************************************************
*                      FIFO Size Configuration in Device mode
*
*  (i) Receive data FIFO size = RAM for setup packets +
*                   OUT endpoint control information +
*                   data OUT packets + miscellaneous
*      Space = ONE 32-bits words
*     --> RAM for setup packets = 10 spaces
*        (n is the nbr of CTRL EPs the device core supports)
*     --> OUT EP CTRL info      = 1 space
*        (one space for status information written to the FIFO along with each
*        received packet)
*     --> data OUT packets      = (Largest Packet Size / 4) + 1 spaces
*        (MINIMUM to receive packets)
*     --> OR data OUT packets  = at least 2*(Largest Packet Size / 4) + 1 spaces
*        (if high-bandwidth EP is enabled or multiple isochronous EPs)
*     --> miscellaneous = 1 space per OUT EP
*        (one space for transfer complete status information also pushed to the
*        FIFO with each endpoint's last packet)
*
*  (ii)MINIMUM RAM space required for each IN EP Tx FIFO = MAX packet size for
*       that particular IN EP. More space allocated in the IN EP Tx FIFO results
*       in a better performance on the USB and can hide latencies on the AHB.
*
*  (iii) TXn min size = 16 words. (n  : Transmit FIFO index)
*   (iv) When a TxFIFO is not used, the Configuration should be as follows:
*       case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txm can use the space allocated for Txn.
*       case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txn should be configured with the minimum space of 16 words
*  (v) The FIFO is used optimally when used TxFIFOs are allocated in the top
*       of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
*******************************************************************************/

#define   MAX_DATA_LENGTH    0x200

#define USB_VBUS_INV 0x08000
#define USB_ID_PIN   0x80000   /* OTG id connector */
#define USB_VBUS_INT 0x40000   /* */
#define USB_ULPI_PHY 0x20000   /* use UPLI pysical interface */
#define USB_VBUS_EMB 0x10000

#define RX_FIFO_FS_SIZE 128
#define RX_FIFO_HS_SIZE 512

#define TX0_FIFO_FS_SIZE   64
#define TX0_FIFO_HS_SIZE  256

#define TXN_FIFO_FS_SIZE   128
#define TXN_FIFO_HS_SIZE   128

#define TXH_NP_FS_FIFOSIZ  96
#define TXH_NP_HS_FIFOSIZ 256

#define TXH_P_FS_FIFOSIZ   96
#define TXH_P_HS_FIFOSIZ  256

//      USB_OTG_Core.TotalFifoSize= 320; /* in 32-bits */
//      USB_OTG_Core.TotalFifoSize= 1280;        /* in 32-bits */
//#define USBD_ITF_MAX_NUM      1

#define USB_MAX_STR_DESC_SIZ 64

#define USBH_MAX_NUM_INTERFACES 2
#define USBH_MAX_NUM_ENDPOINTS  3

#define USBH_MAX_DATA_BUFFER 256


#ifdef USE_USB_OTG_FS
  #define USBH_MSC_MPS_SIZE 0x40
#else
  #define USBH_MSC_MPS_SIZE 0x200
#endif

#define HID_IN_EP   0x81
#define HID_IN_PACKET  4

#define USB_DEVICE_DESCRIPTOR_TYPE         0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE  0x02
#define USB_STRING_DESCRIPTOR_TYPE         0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE      0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE       0x05
#define USB_SIZ_DEVICE_DESC                  18
#define USB_SIZ_STRING_LANGID                 4





#endif //__USB_CONF__H__


