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
 * \addtogroup USB_CDC_ATM USB CDC ATM subclass
 * USB CDC ATM subclass definitions
 * \details This module based on "Universal Serial Bus Communications Class Subclass Specification
 * for Asynchronous Transfer Mode Devices" Revision 1.2
 */

#ifndef USB_CDC_SUBCLASS_ATM


/**Communications Class Subclass Codes
 */
#define USB_CDC_SUBCLASS_ATM                0x07 /** ATM Networking Control Model */


/**CDC ATM subclass specific Functional Descriptors codes
 */
#define DTYPE_CDC_ATM                   0x10 /** ATM Networking Functional Descriptor */


/**CDC ATM subclass specific requests
 */
#define USB_CDC_SET_ATM_DATA_FORMAT         0x50 /** Chooses which ATM data format will be exchanged between the host and the ATM Networking  device.*/
#define USB_CDC_GET_ATM_DEVICE_STATISTICS   0x51 /** Retrieves global statistics from the ATM Networking device.*/
#define USB_CDC_SET_ATM_DEFAULT_VC          0x52 /** Pre-selects the VPI/VCI value for subsequent GET_ATM_DEVICE_STATISTICS requests.*/
#define USB_CDC_GET_ATM_VC_STATISTICS       0x53 /** Retrieves statistics from the ATM Networking device for a particular VPI/VCI.*/


/**ATM Device Statistics Feature Selector Codes
 */
#define ATM_STAT_US_CELLS_SENT              0x01 /** The number of cells that have been sent                                                  * upstream to the WAN link by the ATM layer. */
#define ATM_STAT_DS_CELLS_RECEIVED          0x02 /** The number of cells that have been received                                                  * downstream from the WAN link by the ATM layer */
#define ATM_STAT_DS_CELLS_USB_CONGESTION    0x03 /** The number of cells that have been received      * downstream from the WAN link by the ATM layer and                                                  * discarded due to congestion on the USB link.*/
#define ATM_STAT_DS_CELLS_AAL5_CRC_ERROR    0x04 /** The number of cells that have been received                                                  * downstream from the WAN link by the ATM layer and                                                  * discarded due to AAL5 CRC errors.*/
#define ATM_STAT_DS_CELLS_HEC_ERROR         0x05 /** The number of cells that have been received                                                  * downstream from the WAN link and discarded due to                                                  * HEC errors in the cell header.*/
#define ATM_STAT_DS_CELLS_HEC_ERROR_CORRT   0x06 /** The number of cells that have been received                                                  * downstream from the WAN link and have been detected                                                  * with HEC errors in the cell  header and successfully                                                  * corrected. */


/**ATM Networking Functional Descriptor
 */
typedef struct
{ byte  bLength;               /** Size of this functional descriptor, in bytes.*/
  byte  bType;                 /** CS_INTERFACE descriptor type.*/
  byte  bDescriptorSubType;    /** ATM Networking Functional Descriptor subtype.*/
  byte  iEndSystemIdentifier;  /** The string descriptor holds the End System Identifier. */
  byte  bmDataCapabilities;    /** The ATM data types the device supports.*/
  byte  bmATMDeviceStatistics; /** Indicates which optional statistics functions the  device collects. */
  word  wType2MaxSegmentSize;  /** The maximum segment size that the Type 2 device   is capable of supporting */
  word  wType3MaxSegmentSize;  /** The maximum segment size that the Type 3 device    is capable of supporting */
  word  wMaxVC;                /** The maximum number of simultaneous virtual circuits  the device is capable of supporting (Type 3 only) */
} SERIALIZE  usb_cdc_atm_desc;



#endif /* _USB_CDC_ATM_H_ */
