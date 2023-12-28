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
 * \addtogroup USB_CDC_WCM USB CDC WCM subclass
 * USB CDC WCM subclass definitions
 * \details Wireless Mobile Communications  Devices subclass
 * \details based on Universal Serial Bus CDC Subclass Specification for Wireless Mobile Communications Devices
 *  Revision 1.1 (Errata 1)
 * + Wireless Handset Control Model
 * + Device Management Model
 * + Mobile Direct Line Model
 * + OBEX Model
 */

#ifndef _USB_CDC_WCM_H_
#define _USB_CDC_WCM_H_

/**Communications Class Subclass Codes
 */
#define USB_CDC_SUBCLASS_WHCM           0x08    /** Wireless Handset Control Model.*/
#define USB_CDC_SUBCLASS_DMM            0x09    /** Device Management Model.*/
#define USB_CDC_SUBCLASS_MDLM           0x0A    /** Mobile Direct Line Model.*/
#define USB_CDC_SUBCLASS_OBEX           0x0B    /** OBEX Model.*/


/**Communications Class Protocol Codes
 */
#define USB_CDC_PROTO_PCCA101           0x02    /** AT Commands defined by PCCA-101.*/
#define USB_CDC_PROTO_PCCA101O          0x03    /** AT Commands defined by PCCA-101 & Annex O.*/
#define USB_CDC_PROTO_GSM               0x04    /** AT Commands defined by GSM 07.07.*/
#define USB_CDC_PROTO_3G                0x05    /** AT Commands defined by 3GPP 27.007.*/
#define USB_CDC_PROTO_CDMA              0x06    /** AT Commands defined by TIA for CDMA.*/


/**CDC WCM subclass specific Functional Descriptors codes
 */
#define DTYPE_CDC_WHCM           0x11    /** Wireless Handset Control Model Functional Descriptor.*/
#define DTYPE_CDC_MDLM           0x12    /** Mobile Direct Line Model Functional Descriptor.*/
#define DTYPE_CDC_MDLM_DETAIL    0x13    /** MDLM Detail Functional Descriptor.*/
#define DTYPE_CDC_DMM            0x14    /** Device Management Model Functional Descriptor.*/
#define DTYPE_CDC_OBEX           0x15    /** OBEX Functional Descriptor.*/
#define DTYPE_CDC_CMDSET         0x16    /** Command Set Functional Descriptor.*/
#define DTYPE_CDC_CMDSET_DETAIL  0x17    /** Command Set Detail Functional Descriptor.*/
#define DTYPE_CDC_TEL_CONRTOL    0x18    /** Telephone Control Model Functional Descriptor.*/
#define DTYPE_CDC_OBEX_SERVICE   0x19    /** OBEX Service Identifier Functional Descriptor.*/


/**CDC WCM subclass specific requests
 */



/**Wireless Handset Control Model Functional Descriptor */
typedef struct
{

    byte    bLength;         /** Size of this functional descriptor, in bytes.*/
    byte    bType;         /** CS_INTERFACE descriptor type.*/
    byte    bDescriptorSubType;      /** Wireless Handset Control Model Functional Descriptor.*/
    word   bcdVersion;              /** BCD version number for this subclass specification.*/
} SERIALIZE usb_cdc_whcm_desc ;

/**Mobile Direct Line Model Functional Descriptor
 * \details This descriptor is mandatory. It conveys the GUID that uniquely identifies the kind of
 * MDLM interface that is being provided.
 */
typedef struct
{ byte    bLength;         /** Size of this functional descriptor, in bytes.*/
    byte    bType;         /** CS_INTERFACE descriptor type.*/
    byte    bDescriptorSubType;      /** Mobile Direct Line Model Functional Descriptor.*/
    word   bcdVersion;              /** Version number for this subclass specification.*/
    byte    bGUID[16];               /** Uniquely identifies the detailed transport protocol
                                         * provided by this MDLM interface. */
} SERIALIZE  usb_cdc_mdlm_desc ;

/**Mobile Direct Line Model Detail Functional Descriptor
 * \details This descriptor is optional, and may be repeated as necessary. It conveys any additional
 * information required by the MDLM transport specification identified by the MDLM Functional
 * Descriptor.
 */
typedef struct
{ byte    bLength;      /** Size of this functional descriptor, in bytes.*/
  byte    bType;      /** CS_INTERFACE descriptor type.*/
  byte    bDescriptorSubType;   /** Mobile Direct Line Model Details Functional Descriptor.*/
  byte    bGuidDescriptorType;  /** Discriminator, interpreted according to the semantic  model specified by the GUID in the MDLM Functional Descriptor.*/
  byte    bDetailData[0];       /** Information associated with this GUID and discriminator, according to the semantic model specified by the GUID in                                         * the MDLM Functional Descriptor */
} SERIALIZE  usb_cdc_mdlm_detail_desc;

/**Device Management Functional Descriptor
 */
typedef struct
{ byte bLength;      /** Size of this functional descriptor, in bytes.*/
  byte bType;         /** CS_INTERFACE descriptor type.*/
  byte bDescriptorSubType;      /** Wireless Handset Control Model Functional Descriptor.*/
  word bcdVersion;              /** Version number for this subclass specification.*/
  word wMaxCommand;             /** The buffer size allocated in the device for data                                         * sent from the host using SEND_ENCAPSULATED_CMD. */
} SERIALIZE  usb_cdc_dmm_desc;

/**OBEX Service Identification Functional Descriptor
 * \details This optional functional descriptor indicates the mode supported by this OBEX function.
 * This corresponds to an OBEX role (client or server), a particular OBEX service, and an OBEX
 * service version.
 */
typedef struct
{ byte    bLength;         /** Size of this functional descriptor, in bytes.*/
  byte    bType;         /** CS_INTERFACE descriptor type.*/
  byte    bDescriptorSubType;      /** OBEX Service Identifier Functional Descriptor.*/
  byte    bmOBEXRole;              /** Represents the OBEX role to be played by the function.*/
  byte    bOBEXServiceUUID[16];    /** A 16 byte UUID value used to indicate the particular                                         * OBEX service associated with this function. */
  word   wOBEXServiceVersion;     /** A 16 bit value indicating the version of the OBEX                                         * service associated with this function. */
} SERIALIZE usb_cdc_obex_serv_desc;


#endif /* _USB_CDC_WCM_H_ */
