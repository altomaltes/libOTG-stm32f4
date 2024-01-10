/* This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
 *
 * Copyright ©2016 Dmitry Filimonchuk <dmitrystu[at]gmail[dot]com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**\ingroup USB_CDC
 * \addtogroup USB_CDC_ECM USB CDC ECM subclass
 * USB CDC ECM subclass definitions
 * \details This module based on "Universal Serial Bus Communications Class Subclass Specification for
 * Ethernet Control Model Devices Revision 1.2"
 */

#ifndef USB_CDC_SUBCLASS_ETH

/**Communications Class Subclass Codes
 */
#define USB_CDC_SUBCLASS_ETH                0x06 /** Ethernet Networking Control Model */


/**CDC ECM subclass specific Functional Descriptors codes
 */
#define DTYPE_CDC_ETHERNET              0x0F /** Ethernet Networking Functional Descriptor*/


/**CDC ECM subclass specific requests
 */
#define USB_CDC_SET_ETH_MULTICAST_FILTERS  0x40 /**  */
#define USB_CDC_SET_ETH_PM_PATTERN_FILTER  0x41 /**  */
#define USB_CDC_GET_ETH_PM_PATTERN_FILTER  0x42 /**  */
#define USB_CDC_SET_ETH_PACKET_FILTER      0x43 /** Sets device filter for running a network                                                  * analyzer application on the host machine.*/
#define USB_CDC_GET_ETH_STATISTIC          0x44 /** Retrieves Ethernet device statistics such                                                  * as frames transmitted, frames received, and bad                                                  * frames received.*/


/** Ethernet Statistics Capabilities
 */

#define USB_ETH_XMIT_OK                (1<< 0) /** Frames transmitted without errors.*/
#define USB_ETH_RCV_OK                 (1<< 1) /** Frames received without errors.*/
#define USB_ETH_XMIT_ERROR             (1<< 2) /** Frames not transmitted, or transmitted                                                     * with errors.*/
#define USB_ETH_RCV_ERROR              (1<< 3) /** Frames received with errors that are                                                     * not delivered to the USB host. */
#define USB_ETH_RCV_NO_BUFFER          (1<< 4) /** Frame missed, no buffers */
#define USB_ETH_DIRECTED_BYTES_XMIT    (1<< 5) /** Directed bytes transmitted without errors */
#define USB_ETH_DIRECTED_FRAMES_XMIT   (1<< 6) /** Directed frames transmitted without errors */
#define USB_ETH_MULTICAST_BYTES_XMIT   (1<< 7) /** Multicast bytes transmitted without errors */
#define USB_ETH_MULTICAST_FRAMES_XMIT  (1<< 8) /** Multicast frames transmitted without errors */
#define USB_ETH_BROADCAST_BYTES_XMIT   (1<< 9) /** Broadcast bytes transmitted without errors */
#define USB_ETH_BROADCAST_FRAMES_XMIT  (1<<10) /** Broadcast frames transmitted without errors */
#define USB_ETH_DIRECTED_BYTES_RCV     (1<<11) /** Directed bytes received without errors */
#define USB_ETH_DIRECTED_FRAMES_RCV    (1<<12) /** Directed frames received without errors */
#define USB_ETH_MULTICAST_BYTES_RCV    (1<<13) /** Multicast bytes received without errors */
#define USB_ETH_MULTICAST_FRAMES_RCV   (1<<14) /** Multicast frames received without errors */
#define USB_ETH_BROADCAST_BYTES_RCV    (1<<15) /** Broadcast bytes received without errors */
#define USB_ETH_BROADCAST_FRAMES_RCV   (1<<16) /** Broadcast frames received without errors */
#define USB_ETH_RCV_CRC_ERROR          (1<<17) /** Frames received with circular redundancy check (CRC) or frame check sequence (FCS) error */
#define USB_ETH_TRANSMIT_QUEUE_LENGTH  (1<<18) /** Length of transmit queue */
#define USB_ETH_RCV_ERROR_ALIGNMENT    (1<<19) /** Frames received with alignment error */
#define USB_ETH_XMIT_ONE_COLLISION     (1<<20) /** Frames transmitted with one collision */
#define USB_ETH_XMIT_MORE_COLLISIONS   (1<<21) /** Frames transmitted with more than one collision */
#define USB_ETH_XMIT_DEFERRED          (1<<22) /** Frames transmitted after deferral */
#define USB_ETH_XMIT_MAX_COLLISIONS    (1<<23) /** Frames not transmitted due to collisions */
#define USB_ETH_RCV_OVERRUN            (1<<24) /** Frames not received due to overrun */
#define USB_ETH_XMIT_UNDERRUN          (1<<25) /** Frames not transmitted due to underrun */
#define USB_ETH_XMIT_HEARTBEAT_FAILURE (1<<26) /** Frames transmitted with heartbeat failure */
#define USB_ETH_XMIT_TIMES_CRS_LOST    (1<<27) /** Times carrier sense signal lost during transmission */
#define USB_ETH_XMIT_LATE_COLLISIONS   (1<<28) /** Late collisions detected */



/**Ethernet Networking Functional Descriptor
 * \details describes the operational modes supported by the
 * Communications Class interface, as defined in Section 3.4, with the SubClass code of Ethernet
 * Networking Control. It can only occur within the class-specific portion of an Interface descriptor.
 */
typedef struct
{ byte     bLength;              /** Size of this functional descriptor, in bytes.*/
  byte     bType;                /** CS_INTERFACE descriptor type.*/
  byte     bDescriptorSubType;   /** Ethernet Networking Functional Descriptor.*/
  byte     iMACAddress;          /** Index of string descriptor that holds the 48bit Ethernet MAC.*/
  dword    bmEthernetStatistics; /** Indicates which Ethernet statistics functions the device collects. */
  word    wMaxSegmentSize;       /** The maximum segment size that the Ethernet device is capable of supporting. */
  word    wNumberMCFilters;      /** Contains the number of multicast filters that can be configured by the host. */
  byte     bNumberPowerFilters;  /** Contains the number of pattern filters that are available for causing wake-up of the host. */
} SERIALIZE  usbCdcEtherDesc;





#endif /* _USB_CDC_ECM_H_ */
