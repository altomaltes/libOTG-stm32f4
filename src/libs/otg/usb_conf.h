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

#define USB_VBUS_INV 0x100000
#define USB_ID_PIN   0x200000   /* OTG id connector */
#define USB_VBUS_INT 0x400000   /* */
#define USB_ULPI_PHY 0x800000   /* use UPLI pysical interface */

#define USB_KEEP_PCONF 0xFFFFFFFF

#define USBH_MAX_NUM_INTERFACES 2
#define USBH_MAX_NUM_ENDPOINTS  3

#define USBH_MAX_DATA_BUFFER 256

/* RAM  fifo sizes
 */
#define USB_OTG_FS_TOTAL_FIFO_SIZE ( 320  * 4 )
#define USB_OTG_HS_TOTAL_FIFO_SIZE ( 1280 * 4 )


#define USB_DEVICE_DESCRIPTOR_TYPE         0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE  0x02
#define USB_STRING_DESCRIPTOR_TYPE         0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE      0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE       0x05
#define USB_SIZ_DEVICE_DESC                  18
#define USB_SIZ_STRING_LANGID                 4


#define CFG_SCRATCH_SIZE   512


#endif //__USB_CONF__H__


