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

//#include "usbd_msc_core.h"
#include "usbd_hid_core.h"
//#include "usbd_cdc_core.h"

//#define USBD_VID  0x0483
//#define USBD_PID  0x3752

#define USBD_VID                      0x0483
#define USBD_PID                      0x5720

#define MSC_MAX_FS_PACKET 64

/* USB Standard Device Descriptor
 */
ALIGN_THIS( byte USBD_DeviceQualifierDesc[ USB_LEN_DEV_QUALIFIER_DESC ] ) =
{ USB_LEN_DEV_QUALIFIER_DESC
, DTYPE_QUALIFIER
, 0x00, 0x02, 0x00, 0x00
, 0x00
, MSC_MAX_FS_PACKET, 0x01, 0x00
};


/* USB Standard Device Descriptor
 */
ALIGN_THIS( static const char USBD_DeviceDesc[ USB_SIZ_DEVICE_DESC ] ) =
{ 0x12                                    /* bLength         */
, USB_DEVICE_DESCRIPTOR_TYPE              /* bDescriptorType */
, 0x00, 0x02                              /* bcdUSB          */

//, USB_CLASS_PER_INTERFACE   /* Defined on interface level */
//, USB_CLASS_CDC // 0x00                                    /* bDeviceClass    */
, 0//USB_CLASS_MASS_STOR
, 0//USB_SUBCLASS_NONE
, 0x00

                                 /* bDeviceProtocol */
, USB_OTG_MAX_EP0_SIZE                    /* bMaxPacketSize   */
, LOBYTE( USBD_VID ), HIBYTE( USBD_VID )  /* idVendor         */
, LOBYTE( USBD_PID ), HIBYTE( USBD_PID )  /* idVendor*/
, 0x00, 0x02                              /* bcdDevice rel. 2.00*/
, USBD_IDX_MFC_STR                        /* Index of manufacturer  string */
, USBD_IDX_PRODUCT_STR                    /* Index of product string       */
, USBD_IDX_SERIAL_STR                     /* Index of serial number string */
, 1  // USBD_CFG_MAX_NUM                        /* bNumConfigurations*/
}; /* USB_DeviceDescriptor */


extern USBDclassDefREC USBD_MSC_cb;


/*

USBD_DEVICE USBdeviceDescCdc=
//USBD_DEVICE USBdeviceDesc=
{ USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik Serial CDC"    // 2 USBD_CONFIGURATION_FS_STRING
 ,  "A0000000011G"        // 3 USBD_PRODUCT_FS_STRING
 ,  "Serial CDC Config"          // 4
 ,  "Serial CDC Interface" }    // 5 USBD_INTERFACE_FS_STRING
 , &USBD_CDC_cb };

USBD_DEVICE USBdeviceDesc=
{ USBD_DeviceDesc
, { "\0x4\0x9"              // 0 USBD_LANGID_STRING
  ,  "STMicroelectronics"   // 1 USBD_MANUFACTURER_STRING
  ,  "Kinetik disk"         // 2 USBD_CONFIGURATION_FS_STRING
  ,  "DEF0A000013E"         // 3 USBD_PRODUCT_FS_STRING
  ,  "Mass Config"          // 4
  ,  "Mass Interface" }     // 5 USBD_INTERFACE_FS_STRING
, &USBD_MSC_cb };


*/


USBD_DEVICE USBdeviceDesc=
{ USBD_DeviceDesc
,{ "\0x4\0x9"             // 0 USBD_LANGID_STRING
 ,  "STMicroelectronics"  // 1 USBD_MANUFACTURER_STRING
 ,  "Kinetik Joystck"    // 2 USBD_CONFIGURATION_FS_STRING
 ,  "B000000011E"        // 3 USBD_PUSBdeviceDescRODUCT_FS_STRING
 ,  "HID Config"          // 4
 ,  "HID Interface" }    // 5 USBD_INTERFACE_FS_STRING
 , &USBD_HID_cb };

