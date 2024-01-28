/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * This software component was licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: <http://www.st.com/SLA0044>
 *
 * @file usbd_desc.c
 *
 * original draft from MCD Application Team
 */

#include "usbd_core.h"
#include "usb_std.h"

#include "usbd_storage.h"

#define USBD_VID  (0x03EB)

#define USBdeviceDescCdc USBdeviceDesc  /** select suitable test */

#if defined(  USBdeviceDescMsc )
  #define USBD_PID  (0x204D)

#elif defined( USBdeviceDescCdc )
  #define USBD_PID  (0x204A)

#elif defined( USBdeviceDescCdcCombo )
  #define USBD_PID  (0x204E)

#elif defined( USBdeviceDescMscCombo )
  #define USBD_PID  (0x204C)

#elif defined( USBdeviceDescHid )
  #define USBD_PID  (0x204B)

#else
  #error Must define a test
#endif


extern USBDclassInstance hidHandle;
extern USBDclassInstance cdcHandle;
extern USBDclassInstance mscHandle;
extern USBDclassInstance mscHandle2;

/* USB Standard Device Descriptor
 */
const usbDeviceDesc USBD_DeviceDesc=
{ .bLength= sizeof( USBD_DeviceDesc )       /** Size of the descriptor, in bytes.*/
, .bType             = DTYPE_DEVICE         /** \ref DTYPE_DEVICE Device descriptor.*/
, .bcdUSB            = VERSION_BCD( 1,1,0 ) /** BCD of the supported USB specification.*/
, .bDeviceClass      = USB_CLASS_IAD        /** USB device class.*/
, .bDeviceSubClass   = USB_SUBCLASS_IAD     /** USB device subclass.*/
, .bDeviceProtocol   = USB_PROTO_IAD        /** USB device protocol.*/
, .bMaxPacketSize0   = USB_OTG_MAX_EP0_SIZE /** Size of the control endpoint's bank in bytes.*/
, .idVendor          = USBD_VID             /** Vendor ID for the USB product.*/
, .idProduct         = USBD_PID             /** Unique product ID for the USB product.*/
, .bcdDevice         = VERSION_BCD( 0,0,1 ) /** Product release (version) number.*/
, .iManufacturer     = USBD_IDX_MFC_STR     /** String index for the manufacturer's name.*/
, .iProduct          = USBD_IDX_PRODUCT_STR /** String index for the product name/details.*/
, .iSerialNumber     = USBD_IDX_SERIAL_STR  /** String index for the product serial number.*/
, .bNumConfigurations= 1                    /** Total number of configurations supported by the device.*/
};

const USBD_DEVICE USBdeviceDescHid=
{ &USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik joystick"    // 2 USBD_CONFIGURATION_FS_STRING
 ,  "30201037011D"        // 3 USBD_PRODUCT_FS_STRING
 ,  "HID Config"          // 4
 ,  "HID Interface" }    // 5 USBD_INTERFACE_FS_STRING
, { 0x80                 // GRXFSIZ, RX fifo size
  , 0x10                  // GNPTXFSIZ, TX EPOUNT fifo size
  , 0x10 }    // EPIN 1 .. 3 fifo size

, { &hidHandle
  , NULL }};

const USBD_DEVICE USBdeviceDescCdcCombo=
{ &USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik Serial CDC combo"    // 2 USBD_CONFIGURATION_FS_STRING
 ,  "30201007011D"        // 3 USBD_PRODUCT_FS_STRING
 ,  "Serial CDC - HID Config"          // 4
 ,  "Serial CDC - HID Interface" }    // 5 USBD_INTERFACE_FS_STRING
, { 0x80                 // GRXFSIZ, RX fifo size
  , 0x10                  // GNPTXFSIZ, TX EPOUNT fifo size
  , 0x20, 0x20, 0x20 }    // EPIN 1 .. 3 fifo size

, { &hidHandle
  , &cdcHandle
  , NULL }};

const USBD_DEVICE USBdeviceDescCdc=
{ &USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik Serial CDC combo"    // 2 USBD_CONFIGURATION_FS_STRING
 ,  "30201007011D"        // 3 USBD_PRODUCT_FS_STRING
 ,  "Serial CDC - HID Config"          // 4
 ,  "Serial CDC - HID Interface" }    // 5 USBD_INTERFACE_FS_STRING
, { 0x80                 // GRXFSIZ, RX fifo size
  , 0x10                  // GNPTXFSIZ, TX EPOUNT fifo size
  , 0x20, 0x20 }    // EPIN 1 .. 3 fifo size

, { &cdcHandle
  , NULL }};

const USBD_DEVICE USBdeviceDescMsc=
{ &USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik Disk"     // 2 USBD_CONFIGURATION_FS_STRING
 ,  "B101001011E"         // 3 USBD_PUSBdeviceDescRODUCT_FS_STRING
 ,  "MSC Config"          // 4
 ,  "MSC Interface" }     // 5 USBD_INTERFACE_FS_STRING
, { 0x80                 // GRXFSIZ, RX fifo size
  , 0x10                  // GNPTXFSIZ, TX EPOUNT fifo size
  , 0x40  }    // EPIN 1 .. 3 fifo size
,{ &mscHandle
 , NULL }};

const USBD_DEVICE USBdeviceDescMscCombo=
{ &USBD_DeviceDesc
, { "\0x4\0x9"              // 0 USBD_LANGID_STRING
  ,  "STMicroelectronics"   // 1 USBD_MANUFACTURER_STRING
  ,  "Kinetik disk"         // 2 USBD_CONFIGURATION_FS_STRING
 ,  "30000002411D"        // 3 USBD_PRODUCT_FS_STRING
  ,  "Mass + HID Config"          // 4
  ,  "Mass + HID Interface" }     // 5 USBD_INTERFACE_FS_STRING
, { 0x80                    // GRXFSIZ, RX fifo size
  , 0x10                    // GNPTXFSIZ, TX fifo size
  , 0x20, 0x10, 0x00 }      // EPIN 1 and EPIN 2 fifo size
,{ &mscHandle
,  &hidHandle
, NULL }};


