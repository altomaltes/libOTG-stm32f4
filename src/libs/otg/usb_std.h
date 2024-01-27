/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 * @file usb_std.h
 *
 *  See original note below:
 *
 * This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
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

#ifndef USB_CFG_ATTR_RESERVED

#include "uc.h"
#include "usb_conf.h"

/** USB device configuration definitions
 */
#define USB_CFG_ATTR_RESERVED       0x80
#define USB_CFG_ATTR_SELFPOWERED    0x40


#define __CAT(x,y) x ## y
#define CAT(x,y) __CAT(x,y)

/**\addtogroup USB_STD USB Standard
 * This module contains generic USB device framework definitions
 * \details This module based on
 * + Chapter 9 of the [Universal Serial Bus Specification Revision 2.0]
 * (https://www.usb.org/sites/default/files/usb_20_20181221.zip)
 * + [LUFA - the Lightweight USB Framework for AVRs.](https://github.com/abcminiuser/lufa)
 */

/**Utility functional macros
 */
/** Macro to encode major/minor/version number into BCD code
 * \param maj Major version
 * \param min Minor version
 * \param rev Revision
 */

#define VERSION_BCD(maj, min, rev) (((maj & 0xFF) << 8) | ((min & 0x0F) << 4) | (rev & 0x0F))
#define USB_ARRAY_DESC(...)        { .bLength = 2 + sizeof((word[]){__VA_ARGS__}), .bType = DTYPE_STRING, .wString = {__VA_ARGS__}} /** Macro to create usbStringDesc from array */
#define USB_STRING_DESC(s)         { .bLength= sizeof(CAT(u,s)), .bType  = DTYPE_STRING, .wString= {CAT(u,s)} }                     /** Macro to create usbStringDesc from string */
#define USB_CFG_POWER_MA( mA )     ((byte)(mA) >> 1)/**Macro to set мaximum power consumption field for the \ref usbConfigDesc */

#define USB_SETUP_COUNT     3		   /** The number of USB setup packets */
#define USB_SETUP_SIZE      8	   	/** The size of USB setup data */


/** USB_ENDPOINT_DEF
 *  USB endpoint bmAttributes definitions
 */

#define EPDIR_REQTYPE_MASK    0x60
#define EPADDR_MASK           0x0F	    /**  */

#define EPTYPE_DBLBUF   0x04    /** Doublebuffered endpoint (bulk endpoint only).*/

#define EPDIR_IN              0x80   /** Host-to-device endpoint direction.*/
#define EPDIR_OUT             0x00   /** Device-to-host endpoint direction.*/
#define EPDIR_MASK            0x7F   /** Device-to-host endpoint direction.*/

#define EPTYPE_CONTROL        0x00   /** Control endpoint.     */
#define EPTYPE_ISOCHRONUS     0x01   /** Isochronous endpoint. */
#define EPTYPE_BULK           0x02   /** Bbulk endpoint.       */
#define EPTYPE_INTERRUPT      0x03   /** Interrupt endpoint.   */

#define EPATTR_NO_SYNC        0x00   /** No synchronization.*/
#define EPATTR_ASYNC          0x04   /** Asynchronous endpoint.*/
#define EPATTR_ADAPTIVE       0x08   /** Adaptive endpoint.*/
#define EPATTR_SYNC           0x0C   /** Synchronous endpoint.*/
#define EPUSAGE_DATA          0x00   /** Data endpoint.*/
#define EPUSAGE_FEEDBACK      0x10   /** Feedback endpoint.*/
#define EPUSAGE_IMP_FEEDBACK  0x20   /** Implicit feedback Data endpoint.*/


/** Special string descriptor indexes
 */
#define NO_DESCRIPTOR           0x00    /** String descriptor doesn't exists in the device.*/
#define INTSERIALNO_DESCRIPTOR  0xFE    /** String descriptor is an internal serial number provided by hardware driver.*/

/** USB class definitions
 */
#define USB_CLASS_AUDIO       0x01    /** Audio device class.              */
#define USB_CLASS_CDC         0x02    /** Audio device class.              */
#define USB_CLASS_HID         0x03    /** */
#define USB_CLASS_PHYSICAL    0x05    /** Physical device class.           */
#define USB_CLASS_STILL_IMAGE 0x06    /** Still Imaging device class.      */
#define USB_CLASS_PRINTER     0x07    /** Printer device class.            */
#define USB_CLASS_MASS_STOR   0x08    /** Mass Storage device class.       */
#define USB_CLASS_HUB         0x09    /** HUB device class.                */
#define USB_CLASS_CSCID       0x0B    /** Smart Card device class.         */
#define USB_CLASS_CONTENT_SEC 0x0D    /** Content Security device class.   */
#define USB_CLASS_VIDEO       0x0E    /** Video device class.              */
#define USB_CLASS_HEALTHCARE  0x0F    /** Personal Healthcare device class.*/
#define USB_CLASS_AV          0x10    /** Audio/Video device class.        */
#define USB_CLASS_BILLBOARD   0x11    /** Billboard device class.          */
#define USB_CLASS_CBRIDGE     0x12    /** USB Type-C Bridge device class.  */
#define USB_CLASS_DIAGNOSTIC  0xDC    /** Diagnostic device class.         */
#define USB_CLASS_WIRELESS    0xE0    /** Wireless controller class.       */

#define USB_CLASS_PER_INTERFACE 0x00    /** Class defined on interface level.*/
#define USB_CLASS_IAD           0xEF    /** Class defined on interface association level.*/
#define USB_CLASS_APP_SPEC      0xFE    /** Application Specific class.*/
#define USB_CLASS_VENDOR        0xFF    /** Vendor specific class.*/
#define USB_CLASS_MISC          0xEF    /** Miscellanious device class.*/


#define USB_SUBCLASS_NONE       0x00    /** No subclass defined.*/
#define USB_SUBCLASS_IAD        0x02    /** Subclass defined on interface association level.*/
#define USB_SUBCLASS_VENDOR     0xFF    /** Vendor specific subclass.*/

#define USB_SUBCLASS_CDC        0x00


#define USB_PROTO_NONE      0x00  /** No protocol defined.*/
#define USB_PROTO_IAD       0x01  /** Protocol defined on interface association level.*/
#define USB_PROTO_VENDOR    0xFF  /** Vendor specific protocol.*/

/** USB Standard descriptor types
 */
#define DTYPE_DEVICE         0x01  /** Device descriptor.                  */
#define DTYPE_CONFIGURATION  0x02  /** Configuration descriptor.           */
#define DTYPE_STRING         0x03  /** String descriptor.                  */
#define DTYPE_INTERFACE      0x04  /** Interface descriptor.               */
#define DTYPE_ENDPOINT       0x05  /** Endpoint  descriptor.               */
#define DTYPE_QUALIFIER      0x06  /** Qualifier descriptor.               */
#define DTYPE_OTHER          0x07  /** Descriptor is of other type.        */
#define DTYPE_INTERFACEPOWER 0x08  /** Interface power descriptor.         */
#define DTYPE_OTG            0x09  /** OTG descriptor.                     */
#define DTYPE_DEBUG          0x0A  /** Debug descriptor.                   */
#define DTYPE_INTERFASEASSOC 0x0B  /** Interface association descriptor.   */
#define DTYPE_CS_INTERFACE   0x24  /** Class specific interface descriptor.*/
#define DTYPE_CS_ENDPOINT    0x25  /** Class specific endpoint descriptor. */


/** USB Standard requests
 */
#define USB_GET_STATUS      0x00    /** Returns status for the specified recipient.*/
#define USB_CLEAR_FEATURE   0x01    /** Used to clear or disable a specific feature.*/
#define USB_SET_FEATURE     0x03    /** Used to set or enable a specific feature.*/
#define USB_SET_ADDRESS     0x05    /** Sets the device address for all future device  accesses.*/
#define USB_GET_DESCRIPTOR  0x06    /** Returns the specified descriptor if the descriptor exists.*/
#define USB_SET_DESCRIPTOR  0x07    /** This request is optional and may be used to update existing descriptors or new descriptors may be added.*/
#define USB_GET_CONFIG      0x08    /** Returns the current device configuration value.*/
#define USB_SET_CONFIG      0x09    /** Sets the device configuration.*/
#define USB_GET_INTERFACE   0x0A    /** Returns the selected alternate setting for                                             * the specified interface.*/
#define USB_SET_INTERFACE   0x0B    /** Allows the host to select an alternate setting                                             * for the specified interface.*/
#define USB_SYNCH_FRAME     0x0C    /** Used to set and then report an endpoint's synchronization frame.*/

/** Feature Selector values for Set Feature and Clear Feature standard control requests directed to the device, interface
 *  and endpoint recipients.
 */
#define FEAT_SEL_EP_HALT       0x00  /** Clear Feature or Set Feature commands. endpoint its stall condition changed.				                                        */
#define FEAT_SEL_DEV_IRREM_WUP 0x01  /** Device level Remote Wakeup enable set or clear. remote wakeup support in their descriptors to selectively disable or	enable remote wakeup.*/
#define FEAT_SEL_TESTMODE      0x02 /** Test Mode features, used to test the USB controller. check for incorrect operation.			                                            */


/** USB Feature selector
 */
#define USB_FEAT_ENDPOINT_HALT  0x00    /** Halt endpoint.*/
#define USB_FEAT_REMOTE_WKUP    0x01
#define USB_FEAT_TEST_MODE      0x02
#define USB_FEAT_DEBUG_MODE     0x06

/* bmRequestType D4..0 Recipient
 */
#define  USB_REQ_RECIPIENT_DEVICE    0x00
#define  USB_REQ_RECIPIENT_INTERFACE 0x01
#define  USB_REQ_RECIPIENT_ENDPOINT  0x02
#define  USB_REQ_RECIPIENT_OTHER     0x03

/* bmRequestType D6..5 Type
 */
#define  USB_REQ_TYPE_STANDARD    0x00
#define  USB_REQ_TYPE_CLASS       0x20
#define  USB_REQ_TYPE_VENDOR      0x40
#define  USB_REQ_TYPE_RESERVED    0x60
#define  USB_REQ_TYPE_MASK        0x60


/** USB Test mode Selectors
 */
#define USB_TEST_J              0x01    /** Test J.*/
#define USB_TEST_K              0x02    /** Test K.*/
#define USB_TEST_SE0_NAK        0x03    /** Test SE0 NAK.*/
#define USB_TEST_PACKET         0x04    /** Test Pcaket.*/
#define USB_TEST_FORCE_ENABLE   0x05    /** Test Force Enable.*/


/** USB_STD_LANGID USB standard LANGID codes
 */
#define USB_LANGID_AFR       0x0436   /** Afrikaans */
#define USB_LANGID_SQI       0x041c   /** Albanian */
#define USB_LANGID_ARA_SA    0x0401   /** Arabic (Saudi Arabia) */
#define USB_LANGID_ARA_IQ    0x0801   /** Arabic (Iraq) */
#define USB_LANGID_ARA_EG    0x0c01   /** Arabic (Egypt) */
#define USB_LANGID_ARA_LY    0x1001   /** Arabic (Libya) */
#define USB_LANGID_ARA_DZ    0x1401   /** Arabic (Algeria) */
#define USB_LANGID_ARA_MA    0x1801   /** Arabic (Morocco) */
#define USB_LANGID_ARA_TN    0x1c01   /** Arabic (Tunisia) */
#define USB_LANGID_ARA_OM    0x2001   /** Arabic (Oman) */
#define USB_LANGID_ARA_YE    0x2401   /** Arabic (Yemen) */
#define USB_LANGID_ARA_SY    0x2801   /** Arabic (Syria) */
#define USB_LANGID_ARA_JO    0x2c01   /** Arabic (Jordan) */
#define USB_LANGID_ARA_LB    0x3001   /** Arabic (Lebanon) */
#define USB_LANGID_ARA_KW    0x3401   /** Arabic (Kuwait) */
#define USB_LANGID_ARA_AE    0x3801   /** Arabic (U.A.E.) */
#define USB_LANGID_ARA_BH    0x3c01   /** Arabic (Bahrain) */
#define USB_LANGID_ARA_QA    0x4001   /** Arabic (Qatar) */
#define USB_LANGID_HYE       0x042b   /** Armenian */
#define USB_LANGID_ASM       0x044d   /** Assamese */
#define USB_LANGID_AZE_LAT   0x042c   /** Azeri (Latin) */
#define USB_LANGID_AZE_CYR   0x082c   /** Azeri (Cyrillic) */
#define USB_LANGID_EUS       0x042d   /** Basque */
#define USB_LANGID_BEL       0x0423   /** Belarussian */
#define USB_LANGID_BEN       0x0445   /** Bengali */
#define USB_LANGID_BUL       0x0402   /** Bulgarian */
#define USB_LANGID_MYA       0x0455   /** Burmese */
#define USB_LANGID_CAT       0x0403   /** Catalan */
#define USB_LANGID_ZHO_TW    0x0404   /** Chinese (Taiwan) */
#define USB_LANGID_ZHO_CN    0x0804   /** Chinese (PRC) */
#define USB_LANGID_ZHO_HK    0x0c04   /** Chinese (Hong Kong SAR, PRC) */
#define USB_LANGID_ZHO_SG    0x1004   /** Chinese (Singapore) */
#define USB_LANGID_ZHO_MO    0x1404   /** Chinese (Macau SAR) */
#define USB_LANGID_HRV       0x041a   /** Croatian */
#define USB_LANGID_CZE       0x0405   /** Czech */
#define USB_LANGID_DAN       0x0406   /** Danish */
#define USB_LANGID_NLD_NL    0x0413   /** Dutch (Netherlands) */
#define USB_LANGID_NLD_BE    0x0813   /** Dutch (Belgium) */
#define USB_LANGID_ENG_US    0x0409   /** English (United States) */
#define USB_LANGID_ENG_UK    0x0809   /** English (United Kingdom) */
#define USB_LANGID_ENG_AU    0x0c09   /** English (Australian) */
#define USB_LANGID_ENG_CA    0x1009   /** English (Canadian) */
#define USB_LANGID_ENG_NZ    0x1409   /** English (New Zealand) */
#define USB_LANGID_ENG_IE    0x1809   /** English (Ireland) */
#define USB_LANGID_ENG_ZA    0x1c09   /** English (South Africa) */
#define USB_LANGID_ENG_JM    0x2009   /** English (Jamaica) */
#define USB_LANGID_ENG_CAR   0x2409   /** English (Caribbean) */
#define USB_LANGID_ENG_BZ    0x2809   /** English (Belize) */
#define USB_LANGID_ENG_TH    0x2c09   /** English (Trinidad) */
#define USB_LANGID_ENG_ZW    0x3009   /** English (Zimbabwe) */
#define USB_LANGID_ENG_PH    0x3409   /** English (Philippines) */
#define USB_LANGID_EST       0x0425   /** Estonian */
#define USB_LANGID_FAO       0x0438   /** Faeroese */
#define USB_LANGID_FAS       0x0429   /** Farsi */
#define USB_LANGID_FIN       0x040b   /** Finnish */
#define USB_LANGID_FRA       0x040c   /** French (Standard) */
#define USB_LANGID_FRA_BE    0x080c   /** French (Belgian) */
#define USB_LANGID_FRA_CA    0x0c0c   /** French (Canadian) */
#define USB_LANGID_FRA_SZ    0x100c   /** French (Switzerland) */
#define USB_LANGID_FRA_LU    0x140c   /** French (Luxembourg) */
#define USB_LANGID_FRA_MC    0x180c   /** French (Monaco) */
#define USB_LANGID_KAT       0x0437   /** Georgian */
#define USB_LANGID_DEU       0x0407   /** German (Standard) */
#define USB_LANGID_DEU_SZ    0x0807   /** German (Switzerland) */
#define USB_LANGID_DEU_AT    0x0c07   /** German (Austria) */
#define USB_LANGID_DEU_LU    0x1007   /** German (Luxembourg) */
#define USB_LANGID_DEU_LI    0x1407   /** German (Liechtenstein) */
#define USB_LANGID_ELL       0x0408   /** Greek */
#define USB_LANGID_GUJ       0x0447   /** Gujarati */
#define USB_LANGID_HEB       0x040d   /** Hebrew */
#define USB_LANGID_HIN       0x0439   /** Hindi */
#define USB_LANGID_HUN       0x040e   /** Hungarian */
#define USB_LANGID_ISL       0x040f   /** Icelandic */
#define USB_LANGID_IND       0x0421   /** Indonesian */
#define USB_LANGID_ITA       0x0410   /** Italian (Standard) */
#define USB_LANGID_ITA_SZ    0x0810   /** Italian (Switzerland) */
#define USB_LANGID_JPN       0x0411   /** Japanese */
#define USB_LANGID_KAN       0x044b   /** Kannada */
#define USB_LANGID_KAS       0x0860   /** Kashmiri (India) */
#define USB_LANGID_KAZ       0x043f   /** Kazakh */
#define USB_LANGID_KOK       0x0457   /** Konkani */
#define USB_LANGID_KOR       0x0412   /** Korean */
#define USB_LANGID_KOR_JOH   0x0812   /** Korean (Johab) */
#define USB_LANGID_LAV       0x0426   /** Latvian */
#define USB_LANGID_LIT       0x0427   /** Lithuanian */
#define USB_LANGID_LIT_CLS   0x0827   /** Lithuanian (Classic) */
#define USB_LANGID_MKD       0x042f   /** Macedonian */
#define USB_LANGID_MSA       0x043e   /** Malay (Malaysian) */
#define USB_LANGID_MSA_BN    0x083e   /** Malay (Brunei Darussalam) */
#define USB_LANGID_MAL       0x044c   /** Malayalam */
#define USB_LANGID_MNI       0x0458   /** Manipuri */
#define USB_LANGID_MAR       0x044e   /** Marathi */
#define USB_LANGID_NEP              0x0861   /** Nepali (India) */
#define USB_LANGID_NOB              0x0414   /** Norwegian (Bokmal) */
#define USB_LANGID_NNO              0x0814   /** Norwegian (Nynorsk) */
#define USB_LANGID_ORI              0x0448   /** Oriya */
#define USB_LANGID_POL              0x0415   /** Polish */
#define USB_LANGID_POR_BR           0x0416   /** Portuguese (Brazil) */
#define USB_LANGID_POR              0x0816   /** Portuguese (Standard) */
#define USB_LANGID_PAN              0x0446   /** Punjabi */
#define USB_LANGID_RON              0x0418   /** Romanian */
#define USB_LANGID_RUS              0x0419   /** Russian */
#define USB_LANGID_SAN              0x044f   /** Sanskrit */
#define USB_LANGID_SRB_CYR          0x0c1a   /** Serbian (Cyrillic) */
#define USB_LANGID_SRB_LAT          0x081a   /** Serbian (Latin) */
#define USB_LANGID_SND              0x0459   /** Sindhi */
#define USB_LANGID_SLK              0x041b   /** Slovak */
#define USB_LANGID_SLV              0x0424   /** Slovenian */
#define USB_LANGID_SPA              0x040a   /** Spanish (Traditional Sort) */
#define USB_LANGID_SPA_MX           0x080a   /** Spanish (Mexican) */
#define USB_LANGID_SPA_MDN          0x0c0a   /** Spanish (Modern Sort) */
#define USB_LANGID_SPA_GT           0x100a   /** Spanish (Guatemala) */
#define USB_LANGID_SPA_CR           0x140a   /** Spanish (Costa Rica) */
#define USB_LANGID_SPA_PA           0x180a   /** Spanish (Panama) */
#define USB_LANGID_SPA_DO           0x1c0a   /** Spanish (Dominican Republic) */
#define USB_LANGID_SPA_VE           0x200a   /** Spanish (Venezuela) */
#define USB_LANGID_SPA_CO           0x240a   /** Spanish (Colombia) */
#define USB_LANGID_SPA_PE           0x280a   /** Spanish (Peru) */
#define USB_LANGID_SPA_AR           0x2c0a   /** Spanish (Argentina) */
#define USB_LANGID_SPA_EC           0x300a   /** Spanish (Ecuador) */
#define USB_LANGID_SPA_CL           0x340a   /** Spanish (Chile) */
#define USB_LANGID_SPA_UY           0x380a   /** Spanish (Uruguay) */
#define USB_LANGID_SPA_PY           0x3c0a   /** Spanish (Paraguay) */
#define USB_LANGID_SPA_BO           0x400a   /** Spanish (Bolivia) */
#define USB_LANGID_SPA_SV           0x440a   /** Spanish (El Salvador) */
#define USB_LANGID_SPA_HN           0x480a   /** Spanish (Honduras) */
#define USB_LANGID_SPA_NI           0x4c0a   /** Spanish (Nicaragua) */
#define USB_LANGID_SPA_PR           0x500a   /** Spanish (Puerto Rico) */
#define USB_LANGID_NSO              0x0430   /** Sutu, Sotho. */
#define USB_LANGID_SWA              0x0441   /** Swahili (Kenya) */
#define USB_LANGID_SWE              0x041d   /** Swedish */
#define USB_LANGID_SWE_FI           0x081d   /** Swedish (Finland) */
#define USB_LANGID_TAM              0x0449   /** Tamil */
#define USB_LANGID_TAT              0x0444   /** Tatar (Tatarstan) */
#define USB_LANGID_TEL              0x044a   /** Telugu */
#define USB_LANGID_THA              0x041e   /** Thai */
#define USB_LANGID_TUR              0x041f   /** Turkish */
#define USB_LANGIG_UKR              0x0422   /** Ukrainian */
#define USB_LANGID_URD_PK           0x0420   /** Urdu (Pakistan) */
#define USB_LANGID_URD_IN           0x0820   /** Urdu (India) */
#define USB_LANGID_UZB_LAT    0x0443   /** Uzbek (Latin) */
#define USB_LANGID_UZB_CYR    0x0843   /** Uzbek (Cyrillic) */
#define USB_LANGID_VIE        0x042a   /** Vietnamese */


/** @brief Standard USB Control Request
	 *
	 *  Type define for a standard USB control request.
	 *
	 *  @see The USB 2.0 specification for more information on standard control requests.
	 */
typedef struct
{	byte bReqType;  /** Type of the request.                    */
 	byte bRequest;  /** Request command code.                    */
 	word wValue;    /** wValue parameter of the request.         */
 	word wIndex;    /** wIndex parameter of the request.         */
 	word wLength;   /** Length of the data to transfer in bytes. */
} PACKED UsbRequestHdrRec ;

/** common USB descriptor header
 */
typedef struct
{ byte bLength;  /** Size of the descriptor, in bytes. */
  byte bType;    /** Type of the descriptor. */
  byte iIdx;
} PACKED usbHeaderDescriptor;

/**Represents a USB device descriptor
 * \details A device descriptor describes general information about a USB device. It includes
 * information that applies globally to the device and all of the device’s configurations. A USB
 * device has only one device descriptor. A high-speed capable device that has different device
 * information for full-speed and high-speed must also  have a \ref usb_qualifier_descriptor.
 */
typedef struct
{ byte bLength;            /** Size of the descriptor, in bytes.*/
  byte bType;              /** \ref DTYPE_DEVICE Device descriptor.*/
  word bcdUSB;             /** BCD of the supported USB specification.*/
  byte bDeviceClass;       /** USB device class.*/
  byte bDeviceSubClass;    /** USB device subclass.*/
  byte bDeviceProtocol;    /** USB device protocol.*/
  byte bMaxPacketSize0;    /** Size of the control endpoint's bank in bytes.*/
  word idVendor;           /** Vendor ID for the USB product.*/
  word idProduct;          /** Unique product ID for the USB product.*/
  word bcdDevice;          /** Product release (version) number.*/
  byte iManufacturer;      /** String index for the manufacturer's name.*/
  byte iProduct;           /** String index for the product name/details.*/
  byte iSerialNumber;      /** String index for the product serial number.*/
  byte bNumConfigurations; /** Total number of configurations supported by the device.*/
} PACKED  usbDeviceDesc;

/**  USB device qualifier descriptor
 * The device_qualifier descriptor describes information about a high-speed capable device
 * that would change if the device were operating at the other speed. For example, if the device is
 * currently operating at full-speed, the device qualifier returns information about how it would
 * operate at high-speed and vice-versa.
 */
typedef struct
{ byte  bLength;            /** Size of the descriptor, in bytes.*/
  byte  bType;              /** Qualifier descriptor.*/
  word  bcdUSB;             /** BCD of the supported USB specification.*/
  byte  bDeviceClass;       /** USB device class.*/
  byte  bDeviceSubClass;    /** USB device subclass.*/
  byte  bDeviceProtocol;    /** USB device protocol.*/
  byte  bMaxPacketSize0;    /** Size of the control endpoint's bank in bytes.*/
  byte  bNumConfigurations; /** Total number of configurations supported by the device.*/
  byte  bReserved;          /** Reserved for future use, must be 0.*/
} PACKED usbQualifierDescriptor;

/**USB device configuration descriptor
 * \details The configuration descriptor describes information about a specific device configuration.
 * The descriptor contains a bConfigurationValue field with a value that, when used as a parameter
 * to the SetConfiguration() request, causes the device to assume the described configuration.
 */
typedef struct
{ byte  bLength;          /** Size of the descriptor, in bytes.*/
  byte  bType;            /** Configuration descriptor.*/
  word wTotalLength;      /** Size of the configuration descriptor header, and all sub descriptors inside the configuration. */
  byte  bNumInterfaces;   /** Total number of interfaces in the configuration.*/
  byte  bConfigIndex;     /** Configuration index of the current configuration.*/
  byte  iConfiguration;   /** Index of a string descriptor describing the configuration.*/
  byte  bmbmAttributes;   /** Configuration bmAttributes. Comprised of a mask of \c USB_CONFIG_ATTR_ masks. On all devices, this should include USB_CONFIG_ATTR_RESERVED at a minimum. */
  byte  bMaxPower;        /** Maximum power consumption of the device. USB_CFG_POWER_MA() macro.*/
} PACKED usbConfigDesc;

/**USB interface descriptor
 * \details The interface descriptor describes a specific interface within a configuration. A
 *configuration provides one or more interfaces, each with zero or more endpoint descriptors
 * describing a unique set of endpoints within the configuration.
 */
typedef struct
{ byte bLength;             /** Size of the descriptor, in bytes.*/
  byte bType;               /** Interface descriptor.*/
  byte bInterfaceNumber;    /** Index of the interface in the current configuration.*/
  byte bAlternateSetting;   /** Alternate setting for the interface number.*/
  byte bNumEndpoints;       /** Total number of endpoints in the interface.*/
  byte bInterfaceClass;     /** Interface class ID.*/
  byte bInterfaceSubClass;  /** Interface subclass ID.*/
  byte bInterfaceProtocol;  /** Interface protocol ID. */
  byte iInterface;          /** Index of the string descriptor describing the interface. */
} PACKED  usbInterfaceDesc;

/**USB interface association descriptor
 * USB interface association descriptor (IAD) allows the device to group interfaces that
 * belong to a function.
 */
typedef struct
{ byte bLength;            /** Size of the descriptor, in bytes.*/
  byte bType;              /** IAD descriptor */
  byte bFirstInterface;    /** Index of the first associated interface. */
  byte bInterfaceCount;    /** Total number of associated interfaces. */
  byte bFunctionClass;     /** Function class ID. */
  byte bFunctionSubClass;  /** Function subclass ID. */
  byte bFunctionProtocol;  /** Function protocol ID. */
  byte iFunction;          /** Index of the string descriptor describing the interface association. */
} PACKED  usbIadDesc;

/**USB endpoint descriptor
 * \details This descriptor contains the information required by the host to determine the bandwidth
 * requirements of each endpoint.
 */
typedef struct
{ byte  bLength;           /** Size of the descriptor, in bytes. */
  byte  bType;             /** Endpoint descriptor.*/
  byte  bEndpointAddress;  /** Logical address of the endpoint within the device for the current configuration, including direction mask. */
  byte  bmAttributes;      /** Endpoint bmAttributes, \ref USB_ENDPOINT_DEF. */
  word wMaxPacketSize;     /** Size of the endpoint bank, in bytes. This indicates the maximum packet size that the endpoint can receive at a time. */
  byte  bInterval;         /** Polling interval in milliseconds for the endpoint ifit is an INTERRUPT or ISOCHRONOUS type.*/
} PACKED  usbEndpointDesc;

/**USB string descriptor
 * \details String descriptors are referenced by their one-based index number. A string descriptor
 * contains one or more not NULL-terminated Unicode strings.
 * \note String descriptors are optional. if a device does not support string descriptors, all
 * references to string descriptors within device, configuration, and interface descriptors must be
 * reset to zero.
 */

//#   SERAIL
typedef struct
{ byte  bLength;    /** Size of the descriptor, in bytes.*/
  byte  bType;      /** String descriptor type.*/
  word wString[];   /** String data, as unicode characters or array of * \ref USB_STD_LANGID codes. */
} __attribute__((packed, aligned(2)))   usbStringDesc;

/**  USB debug descriptor
 * This descriptor is used to describe certain characteristics of the device that the host
 * debug port driver needs to know to communicate with the device. Specifically, the debug descriptor
 * lists the addresses of the endpoints that comprise the Debug Pipe. The endpoints are identified by
 * endpoint number.
 */
typedef struct
{ byte  bLength;            /** Size of the descriptor, in bytes.*/
  byte  bType;              /** Debug descriptor type.*/

  byte  bDebugInEndpoint;   /** Endpoint number of the Debug Data IN endpoint.*/
  byte  bDebugOutEndpoint;  /** Endpoint number of the Debug Data OUTendpoint.*/
} PACKED  usbDebugDescriptor;


#endif //_USB_STD_H_
