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

/** Definitions for
 * + Multiple Line Control Model
 * + CAPI Control Model
 */

#ifndef _USB_CDC_ISDN_H_
#define _USB_CDC_ISDN_H_


/**Communications Class Subclass Codes
 */
#define USB_CDC_SUBCLASS_MCNL           0x04    /** Multi-Channel Control Model */
#define USB_CDC_SUBCLASS_CAPI           0x05    /** CAPI Control Model */


/** CDC ISDN Data Interface Class Protocol Codes
 */
#define USB_CDC_PROTO_I340              0x30    /** Physical interface protocol for ISDN BRI */
#define USB_CDC_PROTO_HDLC              0x31    /** HDLC */
#define USB_CDC_PROTO_TRANSPARENT       0x32    /** Transparent */
#define USB_CDC_PROTO_Q921M             0x50    /** Management protocol for Q.921 data link protocol */
#define USB_CDC_PROTO_Q921              0x51    /** Data link protocol for Q.931 */
#define USB_CDC_PROTO_Q921TM            0x52    /** TEI-multiplexor for Q.921 data link protocol */
#define USB_CDC_PROTO_V42BIS            0x90    /** Data compression procedures */
#define USB_CDC_PROTO_Q931              0x91    /** Euro-ISDN protocol control */
#define USB_CDC_PROTO_V120              0x92    /** V.24 rate adaptation to ISDN */
#define USB_CDC_PROTO_CAPI20            0x93    /** CAPI Commands */


/** CDC ISDN subclass specific Functional Descriptors codes
 */
#define DTYPE_CDC_TERMINAL          0x09   /** USB Terminal Functional Descriptor */
#define DTYPE_CDC_NETWORK_TERMINAL  0x0A   /** Network Channel Terminal Descriptor */
#define DTYPE_CDC_PROTOCOL_UNIT     0x0B   /** Protocol Unit Functional Descriptor */
#define DTYPE_CDC_EXTENSION_UNIT    0x0C   /** Extension Unit Functional Descriptor */
#define DTYPE_CDC_MCNL_MANAGEMENT   0x0D   /** Multi-Channel Management Functional Descriptor */
#define DTYPE_CDC_CAPI_CONTROL      0x0E   /** CAPI Control Management Functional Descriptor */


/**CDC ISDN subclass specific requests
 */
#define USB_CDC_SET_UNIT_PARAMETER      0x37    /** Used to set a Unit specific parameter. */
#define USB_CDC_GET_UNIT_PARAMETER      0x38    /** Used to retrieve a Unit specific parameter */
#define USB_CDC_CLEAR_UNIT_PARAMETER    0x39    /** Used to set a Unit specific parameter to
                                                 * its default state. */
#define USB_CDC_GET_PROFILE             0x3A    /** Returns the implemented capabilities of
                                                 * the device */


/** USB_DFU_MCNHCAP
 * Multi-Channel Management Functional Descriptor capabilities
 */
#define USB_CDC_MCHN_UNIT_NVRAM         0x01    /** Device stores Unit parameters in
                                                 * non-volatile memory. */
#define USB_CDC_MCHN_UNIT_CLR           0x02    /** Device supports the request Clear_Unit_Parameter. */
#define USB_CDC_MCHN_UNIR_SET           0x04    /** Device supports the request Set_Unit_Parameter.*/


/** USB_DFU_CAPICAP
 * CAPI Control Management Functional Descriptor capabilities
 */
#define USB_CDC_CAPI_SIMPLE             0x00    /** Device is a Simple CAPI device. */
#define USB_CDC_CAPI_INTELLIGENT        0x01    /** Device is an Intelligent CAPI device. */


/** USB Terminal Functional Descriptor
 * \details The USB Terminal Functional Descriptor provides a means to indicate a relationship
 * between a Unit and an USB Interface. It also defines parameters specific to the interface between
 * the device and the host. It can only occur within the class-specific portion of an Interface
 * descriptor.*/
typedef struct
{ byte bLength;    /** Size of this functional descriptor, in bytes. */
  byte bType;    /** CS_INTERFACE descriptor type. */
  byte bDescriptorSubType; /** USB Terminal Functional Descriptor */
  byte bEntityId;          /** Constant uniquely identifying the Terminal */
  byte bInInterfaceNo;     /** The input interface number of the associated USB interface. */
  byte bOutInterfaceNo;    /** The output interface number of the associated USB interface. */
  byte bmOptions;          /** D0: Protocol wrapper usage */
  byte bChildId0;          /** First ID of lower Terminal or Unit to which this  Terminal is connected. */
} SERIALIZE usb_cdc_terminal_desc ;
    /* ... and there can be a lot of Terminals or Units */

/** Network Channel Terminal Functional Descriptor
 * \details The Network Channel Terminal Functional descriptor provides a means to indicate a
 * relationship between a Unit and a Network Channel. It can only occur within the class-specific
 * portion of an Interface descriptor.*/
typedef struct
{ byte     bLength;    /** Size of this functional descriptor, in bytes. */
  byte     bType;    /** CS_INTERFACE descriptor type. */
  byte     bDescriptorSubType; /** Network Channel Terminal Functional Descriptor */
  byte     bEntityId;          /** Constant uniquely identifying the Terminal */
  byte     iName;              /** Index of string descriptor, describing the name of                                     * the Network Channel Terminal. */
  byte     bChannelIndex;      /** The channel index of the associated network channel                                     * according to indexing rules below. */
  byte     bPhysicalInterface; /** Type of physical interface.
                                     *  + 0 none
                                     *  + 1 ISDN
                                     *  + 2-200 RESERVED
                                     *  + 201 -255 Vendor specific */
} SERIALIZE  usb_cdc_network_terminal_desc ;

/** Protocol Unit Functional Descriptor
 * \details A communication protocol stack is a combination of communication functions (protocols)
 * into a layered structure. Each layer in the stack presents some abstract function for the layer
 * above according to some layer-interface-standard, making it possible to replace a function with
 * another as long as it conforms to the standard. Each layer may have a set of protocol parameters,
 * defined in Appendix E, to configure it for proper operation in the actual environment and the
 * parameters may be retrieved and/or modified. The Unit state is initially reset. See Section 6.2.23
 * “SetUnitParameter”, Section 6.2.24 “GetUnitParameter”, and 6.2.25 “ClearUnitParameter” for details.
 * A Protocol Unit Functional Descriptor identifies with bEntityId a specific protocol instance of
 * bProtocol in a stack. It can only occur within the class-specific portion of an Interface descriptor.
 */
typedef struct
{ byte     bLength;    /** Size of this functional descriptor, in bytes. */
  byte     bType;    /** CS_INTERFACE descriptor type. */
  byte     bDescriptorSubType; /** Network Channel Terminal Functional Descriptor */
  byte     bEntityId;          /** Constant uniquely identifying the Unit */
  byte     bProtocol;          /** Protocol code */
} SERIALIZE usb_cdc_proto_unit_desc;

/** Extension Unit Functional Descriptor
 * \details The Extension Unit Functional Descriptor provides minimal information about the Extension
 * Unit for a generic driver at least to notice the presence of vendor-specific components within
 * the protocol stack. */
typedef struct
{ byte bLength;    /** Size of this functional descriptor, in bytes. */
  byte bType;    /** CS_INTERFACE descriptor type. */
  byte bDescriptorSubType; /** Network Channel Terminal Functional Descriptor */
  byte bEntityId;          /** Constant uniquely identifying the Unit */
  byte bExtensionCode;     /** Vendor specific code identifying the Extension Unit. */
  byte iName;              /** Index of string descriptor, describing the name of  the Extension Unit. */
  byte bChildId0;          /** First ID of lower Terminal or Unit to which this   Terminal is connected. */    /* ... and there can be a lot of Terminals or Units */
} SERIALIZE usb_cdc_ext_unit_desc ;

/**Multi-Channel Management Functional Descriptor
 * \details The Multi-Channel Management functional descriptor describes the commands supported by
 * the Communications Class interface, as defined in CDC , with the SubClass code of Multi-Channel.*/
typedef struct
{ byte     bLength;            /** Size of this functional descriptor, in bytes. */
  byte     bType;              /** CS_INTERFACE descriptor type. */
  byte     bDescriptorSubType; /** Multi-Channel Management Functional Descriptor */
  byte     bmCapabilities;     /** The capabilities that this configuration supports. */
} SERIALIZE usb_cdc_mcnl_managemnt_desc;

/**CAPI Control Management Functional Descriptor
 * \details The CAPI control management functional descriptor describes the commands supported by
 * the CAPI Control Model over the Data Class interface with the protocol code of CAPI control. */

typedef struct
{ byte  bLength;    /** Size of this functional descriptor, in bytes. */
  byte  bType;    /** CS_INTERFACE descriptor type. */
  byte  bDescriptorSubType; /** CAPI Control Management Functional Descriptor */
  byte  bmCapabilities;     /** The capabilities that this configuration supports. */
} SERIALIZE usb_cdc_capi_ctl_desc;



#endif /* _USB_CDC_ISDN_H_ */
