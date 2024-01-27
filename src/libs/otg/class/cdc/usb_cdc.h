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

#ifndef USB_CLASS_CDC_DATA


/** USB CDC Class codes
 */
// #define USB_CLASS_CDC        0x02    /** Communicational Device class */
#define USB_CLASS_CDC_DATA   0x0A    /** Data Interface class */


/** USB CDC subclass codes
 */
#define USB_CDC_SUBCLASS_ACM      0x02    /** Abstract Control Model */

/**Communications Class Protocol Codes
 */
#define USB_CDC_PROTO_NONE        0x00    /** No class specific protocol required */
#define USB_CDC_PROTO_V25TER      0x01    /** AT Commands: V.250 etc.*/
#define USB_CDC_PROTO_AT       0x01    /* opencm3 */

/** Data Interface Class Protocol Codes
 */
#define USB_CDC_PROTO_NTB          0x01    /** Network Transfer Block.*/
#define USB_CDC_PROTO_HOST         0xFD    /** Host based driver. This protocol code should only be used in messages between host and device to identify the host driver portion of a protocol stack.*/
#define USB_CDC_PROTO_CDCSPEC      0xFE    /** CDC specified. The protocol(s) are described using a Protocol Unit Functional Descriptors on Communication Class Interface.*/

/**USB CDC class-specified functional descriptors
 */
#define DTYPE_CDC_HEADER           0x00    /** Header Functional Descriptor.*/
#define DTYPE_CDC_CALL_MANAGEMENT  0x01    /** Call Management Functional Descriptor.*/
#define DTYPE_CDC_ACM              0x02    /** Abstract Control Management Functional Descriptor.*/
#define DTYPE_CDC_UNION            0x06    /** Union Functional Descriptor.             */
#define DTYPE_CDC_COUNTRY          0x07    /** Country Selection Functional Descriptor. */


/** USB CDC class-specific requests
 */
#define USB_CDC_SEND_ENCAPSULATED_CMD  0x00 /** Used to issue a command in the format of the supported control protocol of the Communication Class interface.*/
#define USB_CDC_GET_ENCAPSULATED_RESP  0x01 /** Used to request a response in the format of the supported control protocol of the Communication Class interface.*/
#define USB_CDC_SET_COMM_FEATURE       0x02 /** Controls the settings for a particular communication feature  of a particular target.*/
#define USB_CDC_GET_COMM_FEATURE       0x03 /** Returns the current settings for the communication feature as selected.*/
#define USB_CDC_CLEAR_COMM_FEATURE     0x04 /** Controls the settings for a particular communication feature of a particular target, setting the selected feature to its default state.*/
#define USB_CDC_SET_LINE_CODING        0x20 /** Allows the host to specify typical asynchronous line-character  formatting properties.*/
#define USB_CDC_GET_LINE_CODING        0x21 /** Allows the host to find out the currently configured line coding.*/
#define USB_CDC_SET_CONTROL_LINE_STATE 0x22 /** Generates RS-232/V.24 style control signals.*/
#define USB_CDC_SEND_BREAK             0x23 /** Sends special carrier modulation that generates an RS-232 style break.*/


/** Generic CDC specific notifications
 */
#define USB_CDC_NTF_NETWORK_CONNECTION  0x00 /** Allows the device to notify the host about network connection status.*/
#define USB_CDC_NTF_RESPONSE_AVAILABLE  0x01 /** Allows the device to notify the host that a response is available.*/
#define USB_CDC_NTF_SERIAL_STATE        0x20 /** Sends asynchronous notification of UART status.*/
#define USB_CDC_NTF_SPEED_CHANGE        0x2A /** Allows the device to inform the host-networking driver that a change in either the uplink or the */

/** USB_CDC_ACMGMNTCAP
 * USB CDC Abstract Control Management capabilities
 */
#define USB_CDC_COMM_FEATURE 0x01  /** Supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, Get_Comm_Feature.*/
#define USB_CDC_CAP_LINE     0x02  /** Supports the request combination of Set_Line_Coding, Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State.*/
#define USB_CDC_CAP_BRK      0x04  /** Supports the request Send_Break.*/
#define USB_CDC_CAP_NOTIFY   0x08  /** Supports notification Network_Connection.*/


/** USB_CDC_CALLMGMTCAP
 * USB CDC Call Management capabilities
 */
#define USB_CDC_CALL_MGMT_CAP_CALL_MGMT 0x01    /** Device handles call management itself.*/
#define USB_CDC_CALL_MGMT_CAP_DATA_INTF 0x02    /** Device can send/receive call management information over a Data Class interface.*/


/** USB_CDC_LINECODE
 * Line coding structire bit fields
 */
#define USB_CDC_1_STOP_BITS    0x00    /** 1 stop bit.*/
#define USB_CDC_1_5_STOP_BITS  0x01    /** 1.5 stop bits.*/
#define USB_CDC_2_STOP_BITS    0x02    /** 2 stop bits.*/
#define USB_CDC_NO_PARITY      0x00    /** NO parity bit.*/
#define USB_CDC_ODD_PARITY     0x01    /** ODD parity bit.*/
#define USB_CDC_EVEN_PARITY    0x02    /** EVEN parity bit.*/
#define USB_CDC_MARK_PARITY    0x03    /** patity is MARK.*/
#define USB_CDC_SPACE_PARITY   0x04    /** patity is SPACE.*/


/** SERIAL_STATE notification data values
 */
#define USB_CDC_STATE_RX_CARRIER 0x0001 /** State of receiver carrier detection mechanism. This signal corresponds to V.24 signal 109                                                * and RS-232 DCD.*/
#define USB_CDC_STATE_TX_CARRIER 0x0002 /** State of transmission carrier.This signal corresponds to V.24 signal 106 and RS-232 DSR.*/
#define USB_CDC_STATE_BREAK      0x0004 /** State of break detection mechanism of the device.*/
#define USB_CDC_STATE_RING       0x0008 /** State of ring signal detection of the device.*/
#define USB_CDC_STATE_FRAMING    0x0010 /** A framing error has occurred.*/
#define USB_CDC_STATE_PARITY     0x0020 /** A parity error has occurred.*/
#define USB_CDC_STATE_OVERRUN    0x0040 /** Received data has been discarded due to overrun in the device.*/


/** Header Functional Descriptor
 * &details Header Functional Descriptor marks the beginning of the concatenated set of functional
 * descriptors for the interface.
 */
typedef struct
{ byte  bLength;             /** Size of this descriptor in bytes.*/
  byte  bType;               /** CS_INTERFACE descriptor type.*/
  byte  bDescriptorSubType;  /** Header functional descriptor subtype.*/
  word  bcdCDC;              /** USB CDC Specification release number in BCD.*/
} PACKED  usbCdcHeaderDesc;

/**   Union Functional Descriptor
 * The Union functional descriptor describes the relationship between a group of interfaces
 * that can be considered to form a functional unit. It can only occur within the class-specific
 * portion of an Interface descriptor. One of the interfaces in the group is designated as a master
 * or controlling interface for the group, and certain class-specific messages can be sent to this
 * interface to act upon the group as a whole.
 */
typedef struct
{ byte  bLength;            /** Size of this functional descriptor, in bytes.*/
  byte  bType;              /** CS_INTERFACE descriptor type.*/
  byte  bDescriptorSubType; /** Union Functional Descriptor.*/
  byte  bMasterInterface0;  /** The interface number of the CDC interface designated as the master or controlling interface for the union.*/
  byte  bSlaveInterface0;   /** Interface number of first slave or associated interface in the union.*/    /* ... and there could be other slave interfaces */
} PACKED  usbCdcUnionDesc;

/**   Country Selection Functional Descriptor
 * The Country Selection functional descriptor identifies the countries in which the
 * communication device is qualified to operate. The parameters of the network connection often vary
 * from one country to another, especially in Europe. Also legal requirements impose certain
 * restrictions on devices because of different regulations by the governing body of the network to
 * which the device must adhere. This descriptor can only occur within the class-specific portion of
 * an Interface descriptor and should only be provided to a master Communication Class interface of
 * a union. The country codes used in the Country Selection Functional Descriptor are not the same
 * as the country codes used in dialing international telephone calls. Implementers should refer to
 * the ISO 3166 specification for more information.
 */
typedef struct
{ byte bLength;     /** Size of this functional descriptor, in bytes.*/
  byte bType;               /** CS_INTERFACE descriptor type.*/
  byte bDescriptorSubType;  /** Country Selection Functional Descriptor.*/
  byte iCountryCodeRelDate; /** Index of a string giving the release date for the implemented ISO 3166 Country Codes. */
  byte wCountyCode0;        /** Country code in hexadecimal format. As defined in ISO 3166, release date as specified in iCountryCodeRelDate for the first supported country. */    /* ... and there can be a lot of country codes */
} PACKED  usbCdcCountryDesc;

/** Call Management Functional Descriptor.
 *   The Call Management functional descriptor describes the processing of calls for the
 * Communication Class interface. It can only occur within the class-specific portion of an Interface
 * descriptor.
 */
typedef struct
{ byte bLength;             /** Size of this functional descriptor, in bytes.*/
  byte bType;               /** CS_INTERFACE descriptor type.*/
  byte bDescriptorSubType;  /** Call Management functional descriptor subtype.*/
  byte bmCapabilities;      /** The call management capabilities that this configuration supports.*/
  byte bDataInterface;      /** Interface number of Data Class interface optionally used for call management.*/
}  PACKED usbCdcCallMgmtDesc;

/**Abstract Control Management Functional Descriptor
 * \details The Abstract Control Management functional descriptor describes the commands supported
 * by the Communication Class interface, as defined in Section 3.6.2, with the SubClass code of
 * Abstract Control Model. It can only occur within the class-specific portion of an Interface
 * descriptor.
 */
typedef struct
{ byte     bLength;            /** Size of this functional descriptor, in bytes.*/
  byte     bType;              /** CS_INTERFACE descriptor type.*/
  byte     bDescriptorSubType; /** Abstract Control Management functional descriptor subtype.*/
  byte     bmCapabilities;     /** The capabilities that this configuration supports.*/
} PACKED  usbCdcAcmDesc;

/** Notification structure from CDC
 */
typedef struct
{ byte    bmRequestType;      /** This bitmapped field identifies the characteristics of the specific request.*/
  byte    bNotificationType;  /** Notification type.*/
  word    wValue;             /** Notification value.*/
  word    wIndex;             /** Interface.*/
  word    wLength;            /** Data payload length in bytes.*/
  byte    Data[];             /** Data payload.*/
} PACKED  usbCdcNotification;

/** Line Coding Structure
 */
typedef struct
{ dword    dwDTERate;        /** Data terminal rate, in bits per second.*/
  byte     bStopBits;        /** Stop bits.*/
  byte     bParityType;      /** Parity.*/
  byte     bDataBits;        /** Data bits (5,6,7,8 or 16).*/
} PACKED usbCdcLineCoding;


#endif /* _USB_CDC_H_ */
