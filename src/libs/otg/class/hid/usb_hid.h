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

#ifndef _USB_HID_H_
#define _USB_HID_H_

/**\addtogroup USB_HID USB HID class
 * This module contains USB Human Interface Devices class definitions
 * \details This module based on
 * + [Device Class Definition for Human Interface Devices (HID) Version 1.11](https://www.usb.org/sites/default/files/documents/hid1_11.pdf)
 * + [LUFA - the Lightweight USB Framework for AVRs.](https://github.com/abcminiuser/lufa)
 *
 * \par Control usage types
 *                Type     | Logical min | Logical max |                     Flags | Signal | Operation
 *-------------------------|-------------|-------------|---------------------------|--------|----------------
 *[LC] Linear control      | -1          | 1           | Relative, Preferred state |   Edge | Inc / Dec value
 *                        || -Min        | Max         | Relative, Preferred state |  Level | Inc / Dec value by n
 *                        ||  Min        | Max         | Absolute, Preferred state |   N/A  | Absolute value
 *[OOC] ON/OFF control     | -1          | 1           | Relative, No preferred    |   Edge | 1 asserts ON; -1 assertsOFF
 *                        || 0           | 1           | Relative, Preferred state |   Edge | 0->1 toggles ON/OFF
 *                        || 0           | 1           | Absolute, No preferred    |  Level | 0 asserts ON; 1 asserts OFF
 *[MC] Momentary control   | 0           | 1           | Absolute, Preferred state |  Level | 0 asserts, 1 deasserts condition
 *[OSC] One shot control   | 0           | 1           | Relative, Preferred state |   Edge | 0->1 triggers an event
 *[RTC] Re-trigger control | 0           | 1           | Absolute, Preferred state |  Level | 1 trigger an event
 *
 *\par Data usage types
 *Type               | Flgs                         | Description
 *-------------------|------------------------------|------------------------
 *[SEL] Selector     | Array                        | Contained a Named Array
 *[SV] Static value  | Constant, Variable, Absolute | Read-only multiple-bit value
 *[SF] Static flag   | Constant, Variable, Absolute | Read-only single-bit value
 *[DV] Dynamic value | Data, Variable, Absolute     | Read-write multiple-bit value
 *[DF] Dynamic flag  | Data, Variable, Absolute     | Read-write single-bit value
 *
 */

/** USB HID class code
 */
#define USB_CLASS_HID             0x03    /** Interface belongs to the Human Interface device class.*/

/** USB HID subclass codes
 */
#define USB_HID_SUBCLASS_NONBOOT  0x00    /** Nonboot subclass.*/
#define USB_HID_SUBCLASS_BOOT     0x01    /** Boot subclass.   */


/** USB HID protocol codes
 */
#define USB_HID_PROTO_NONBOOT   0x00 /** Nonboot protocol.       */
#define USB_HID_PROTO_KEYBOARD  0x01 /** Keyboard boot protocol. */
#define USB_HID_PROTO_MOUSE     0x02 /** Mouse boot protocol.    */

#define USB_HID_REPORT_IN       0x01 /** Indicates that the item is an IN report type.*/
#define USB_HID_REPORT_OUT      0x02 /** Indicates that the item is an OUT report type.*/
#define USB_HID_REPORT_FEATURE  0x03 /** Indicates that the item is a FEATURE report type.*/


/** USB HID class-specified requests
 */
#define HID_GETREPORT    0x01  /** Request to get the current HID report from the device.*/
#define HID_GETIDLE      0x02  /** Request to get the current device idle count.*/
#define HID_GETPROTOCOL  0x03  /** Request to get the current HID report protocol mode.*/
#define HID_SETREPORT    0x09  /** Request to set the current HID report to the device.*/
#define HID_SETIDLE      0x0A  /** Request to set the device's idle count.*/
#define HID_SETPROTOCOL  0x0B  /** Request to set the current HID report protocol mode.*/


/** USB HID class-specified descriptor types
 */
#define HID_DTYPE_HID    0x21   /** HID class HID descriptor type.       */
#define HID_DTYPE_REPORT 0x22   /** HID class HID report descriptor type.*/


/** USB HID country codes
 */
#define USB_HID_COUNTRY_NONE     0    /** Not supported */
#define USB_HID_COUNTRY_AR       1    /** Arabic */
#define USB_HID_COUNTRY_BE       2    /** Belgian */
#define USB_HID_COUNTRY_CA_BI    3    /** Canadian-Bilingual */
#define USB_HID_COUNTRY_CA_FR    4    /** Canadian-French */
#define USB_HID_COUNTRY_CZ       5    /** Czech Republic */
#define USB_HID_COUNTRY_DK       6    /** Danish */
#define USB_HID_COUNTRY_FI       7    /** Finnish */
#define USB_HID_COUNTRY_FR       8    /** French */
#define USB_HID_COUNTRY_DE       9    /** German */
#define USB_HID_COUNTRY_GR       10   /** Greek */
//#define USB_HID_COUNTRY_HEB      11   /** Hebrew (santcioned) */
#define USB_HID_COUNTRY_HU       12   /** Hungary */
#define USB_HID_COUNTRY_ISO      13   /** International (ISO) */
#define USB_HID_COUNTRY_IT       14   /** Italian */
#define USB_HID_COUNTRY_JP       15   /** Japan (Katakana) */
#define USB_HID_COUNTRY_KR       16   /** Korean */
#define USB_HID_COUNTRY_LAT      17   /** Latin American */
#define USB_HID_COUNTRY_NL       18   /** Netherlands/Dutch */
#define USB_HID_COUNTRY_NO       19   /** Norwegian */
#define USB_HID_COUNTRY_PER      20   /** Persian (Farsi) */
#define USB_HID_COUNTRY_PL       21   /** Poland */
#define USB_HID_COUNTRY_PO       22   /** Portuguese */
#define USB_HID_COUNTRY_RU       23   /** Russia */
#define USB_HID_COUNTRY_SK       24   /** Yugoslavia */
#define USB_HID_COUNTRY_TR_F     35   /** Slovakia */
#define USB_HID_COUNTRY_ES       25   /** Spanish */
#define USB_HID_COUNTRY_SE       26   /** Swedish */
#define USB_HID_COUNTRY_CH_FR    26   /** Swiss-French */
#define USB_HID_COUNTRY_CH_DE    27   /** Swiss-German */
#define USB_HID_COUNTRY_CH       29   /** Switzerland */
#define USB_HID_COUNTRY_TW       30   /** Taiwan */
#define USB_HID_COUNTRY_TR_Q     31   /** Turkish-Q */
#define USB_HID_COUNTRY_UK       32   /** United Kingdom */
#define USB_HID_COUNTRY_US       33   /** United States */
#define USB_HID_COUNTRY_YU       34   /** Yugoslavia */
#define USB_HID_COUNTRY_TR_F     35   /** Turkish-F */


/** HID Collections types
 */
#define HID_PHYSICAL_COLLECTION        0x00    /** A physical collection of items.*/
#define HID_APPLICATION_COLLECTION     0x01    /** Applies a name to a top level collection which the operating system uses to identify a device and possibly remap to a legacy API.*/
#define HID_LOGICAL_COLLECTION         0x02    /** A logical collection of items.*/
#define HID_REPORT_COLLECTION          0x03
#define HID_NARY_COLLECTION            0x04    /** A collection that encompasses an array definition, naming the array set or the field created by the array.*/
#define HID_USAGE_SWITCH_COLLECTION    0x05    /** Modifies the purpose or function of the usages (controls) that it contains. */
#define HID_USAGE_MODIFIER_COLLECTION  0x06    /** Modifies the purpose or function of the usages (controls) that contains it.*/



/** USB HID functional descriptor
 */
typedef struct
{ byte bLength;            /** Size of the descriptor, in bytes. */
  byte bType;              /** Type of the descriptor, set to \ref DTYPE_HID */
  word bcdHID;             /** BCD encoded version that the HID descriptor and device complies to. */
  byte bCountryCode;       /** Country code of the localized device, or zero if universal. */
  byte bNumDescriptors;    /** Total number of HID report descriptors for the interface. */
  byte bType0;             /** 1'st HID report descriptor type, set to \ref DTYPE_HID_REPORT */
  word wDescriptorLength0; /** 1'sr HID report descriptor length in bytes. */
} PACKED usbHidDescriptor;


/** USB HID functional descriptor header
*/
typedef struct
{ byte bLength;         /** Size of the descriptor, in bytes. */
  byte bType;           /** Type of the descriptor, set to \ref DTYPE_HID */
  word bcdHID;          /** BCD encoded version that the HID descriptor and device complies to. */
  byte bCountryCode;    /** Country code of the localized device, or zero if universal. */
  byte bNumDescriptors; /** Total number of HID report descriptors for the interface. */
} PACKED  usbHidDescriptor_header;

/** USB HID report descriptor
 */
typedef struct
{ byte bType;                        /** Type of HID report, set to \ref DTYPE_HID_REPORT */
  word wDescriptorLength;            /** Length of the associated HID report descriptor, in bytes. */
} PACKED usb_hid_report_descriptor ;

/** Helper macro for the multireport hid descriptor
 */
#define DECLARE_usbHidDescriptor(p)               \
struct usbHidDescriptor_##p {                     \
    byte     bLength;                            \
    byte     bType;                    \
    word    bcdHID;                            \
    byte     bCountryCode;                      \
    byte     bNumDescriptors;                   \
    struct usb_hid_report_descriptor report[p]; \
} PACKED;


/* Public Interface - May be used in end-application: */
/* Macros: */
/** HID Input, Output and Feature Report Descriptor Item Flags
*/

#define HID_IOF_CONSTANT           (1 << 0)  /** Item is a static read-only field in a report and cannot be modified.*/
#define HID_IOF_DATA               (0 << 0)  /** Item contains a modifiable device data.*/
#define HID_IOF_VARIABLE           (1 << 1)  /** Each field represents data from a physical control.*/
#define HID_IOF_ARRAY              (0 << 1)  /** Array returns an index in each field that corresponds to the pressed button.*/
#define HID_IOF_RELATIVE           (1 << 2)  /** Data is relative (indicating the change in value from the last report).*/
#define HID_IOF_ABSOLUTE           (0 << 2)  /** Data is absolute (based on a fixed origin).*/
#define HID_IOF_WRAP               (1 << 3)  /** Data “rolls over” when reaching either the extreme high or low value.*/
#define HID_IOF_NO_WRAP            (0 << 3)  /** Data not “rolls over” when reaching either the extreme high or low value. */
#define HID_IOF_NON_LINEAR         (1 << 4)  /** Noninear relationship between logical and physical units.*/
#define HID_IOF_LINEAR             (0 << 4)  /** Linear relationship between logical and physical units.*/
#define HID_IOF_NO_PREFERRED_STATE (1 << 5)  /** Control has no preferred state.*/
#define HID_IOF_PREFERRED_STATE    (0 << 5)  /** Control has a preferred state to which it will return when the user is                                                             * not physically interacting with the control. */
#define HID_IOF_NULLSTATE          (1 << 6)  /** Control has a state in which it is not sending meaningful data.*/
#define HID_IOF_NO_NULL_POSITION   (0 << 6)  /** Control has no nullstate.*/
#define HID_IOF_VOLATILE           (1 << 7)  /** Data may be changed by device itself. (Output and Feature) */
#define HID_IOF_NON_VOLATILE       (0 << 7)  /** Data can be changed by host only. (Output and Feature) */
#define HID_IOF_BUFFERED_BYTES     (1 << 8)  /** Indicates that the control emits a fixed-size stream of bytes.*/
#define HID_IOF_BITFIELD           (0 << 8)  /** Data is a fixed bitfield.*/

/* Private Interface - For use in library only:
 */
#define CONCAT(x, y)            x ## y
#define CONCAT_EXPANDED(x, y)   CONCAT(x, y)

#define HID_RI_DATA_SIZE_MASK  0x03
#define HID_RI_TYPE_MASK       0x0C
#define HID_RI_TAG_MASK        0xF0
#define HID_RI_TYPE_MAIN       0x00
#define HID_RI_TYPE_GLOBAL     0x04
#define HID_RI_TYPE_LOCAL      0x08
#define HID_RI_DATA_BITS_0     0x00
#define HID_RI_DATA_BITS_8     0x01
#define HID_RI_DATA_BITS_16    0x02
#define HID_RI_DATA_BITS_32    0x03

#define HID_RI_DATA_BITS(DataBits)              CONCAT_EXPANDED(HID_RI_DATA_BITS_, DataBits)
#define _HID_RI_ENCODE_0(Data)
#define _HID_RI_ENCODE_8(Data)                  , (Data & 0xFF)
#define _HID_RI_ENCODE_16(Data)                 _HID_RI_ENCODE_8(Data)  _HID_RI_ENCODE_8(Data >> 8)
#define _HID_RI_ENCODE_32(Data)                 _HID_RI_ENCODE_16(Data) _HID_RI_ENCODE_16(Data >> 16)
#define _HID_RI_ENCODE(DataBits, ...)           CONCAT_EXPANDED(_HID_RI_ENCODE_, DataBits(__VA_ARGS__))
#define _HID_RI_ENTRY(Type, Tag, DataBits, ...) (Type | Tag | HID_RI_DATA_BITS(DataBits)) _HID_RI_ENCODE(DataBits, (__VA_ARGS__))



/** HID Report Descriptor Main items
 */

#define HID_RI_MAIN_INPUT      0x80 /** Describes information about the data provided by one or more physical controls. */
#define HID_RI_MAIN_OUTPUT     0x90 /** similar to an Input item except it describes data sent to the device—for example, LED states. */
#define HID_RI_MAIN_COLLECTION 0xA0 /** identifies a relationship between two or more data (Input, Output, or Feature.) */
#define HID_RI_MAIN_FEATURE    0xB0 /** describe device configuration information that can be sent to the device. */
#define HID_RI_MAIN_ECOLLEC    0xC0 /** closes a collection */

#define HID_RI_INPUT(     DataBits, ...) _HID_RI_ENTRY( HID_RI_TYPE_MAIN, 0x80, DataBits, __VA_ARGS__ ) /** Describes information about the data provided by one or more physical controls. */
#define HID_RI_OUTPUT(    DataBits, ...) _HID_RI_ENTRY( HID_RI_TYPE_MAIN, 0x90, DataBits, __VA_ARGS__ ) /** similar to an Input item except it describes data sent to the device—for example, LED states. */
#define HID_RI_COLLECTION(DataBits, ...) _HID_RI_ENTRY( HID_RI_TYPE_MAIN, 0xA0, DataBits, __VA_ARGS__ ) /** identifies a relationship between two or more data (Input, Output, or Feature.) */
#define HID_RI_FEATURE(   DataBits, ...) _HID_RI_ENTRY( HID_RI_TYPE_MAIN, 0xB0, DataBits, __VA_ARGS__ ) /** describe device configuration information that can be sent to the device. */
#define HID_RI_END_COLLEC(DataBits, ...) _HID_RI_ENTRY( HID_RI_TYPE_MAIN, 0xC0, DataBits, __VA_ARGS__ ) /** closes a collection */

/** HID Report Descriptor Global items */


#define HID_RI_GLOBAL_USAGE_PAGE       0x00 /** Unsigned integer specifying the current Usage Page. */
#define HID_RI_GLOBAL_LOGICAL_MINIMUM  0x10 /** Defines a minimum value that a variable or array item will report. */
#define HID_RI_GLOBAL_LOGICAL_MAXIMUM  0x20 /** Defines a maximum value that a variable or array item will report. */
#define HID_RI_GLOBAL_PHYSICAL_MINIMUM 0x30 /** Defines a minimum value for the physical extent of a variable item */
#define HID_RI_GLOBAL_PHYSICAL_MAXIMUM 0x40 /** Defines a maximum value for the physical extent of a variable item */
#define HID_RI_GLOBAL_UNIT_EXPONENT    0x50
#define HID_RI_GLOBAL_UNIT             0x60 /** Encoded unit value \see \ref HID_UNITS_ENCODE */
#define HID_RI_GLOBAL_REPORT_SIZE      0x70 /** Unsigned integer specifying the size of the report fields in bits. */
#define HID_RI_GLOBAL_REPORT_ID        0x80 /** Unsigned value that specifies the Report ID. */
#define HID_RI_GLOBAL_REPORT_COUNT     0x90 /** Unsigned integer specifying the number of data fields for the item */
#define HID_RI_GLOBAL_PUSH             0xA0 /** Places a copy of the global item state table on the stack. */
#define HID_RI_GLOBAL_POP              0xB0 /** Replaces the item state table with the top structure from the stack. */

#define HID_RI_USAGE_PAGE(DataBits, ...)       _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x00, DataBits, __VA_ARGS__)/** Unsigned integer specifying the current Usage Page. */
#define HID_RI_LOGICAL_MINIMUM(DataBits, ...)  _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x10, DataBits, __VA_ARGS__)/** Defines a minimum value that a variable or array item will report. */
#define HID_RI_LOGICAL_MAXIMUM(DataBits, ...)  _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x20, DataBits, __VA_ARGS__)/** Defines a maximum value that a variable or array item will report. */
#define HID_RI_PHYSICAL_MINIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x30, DataBits, __VA_ARGS__)/** Defines a minimum value for the physical extent of a variable item */
#define HID_RI_PHYSICAL_MAXIMUM(DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x40, DataBits, __VA_ARGS__)/** Defines a maximum value for the physical extent of a variable item */

/** Value of the unit exponent in base 10.
 * \note The USB-HID specification is unclear about Unit exponent usage. Practically it's limited by 1 nibble.
*/

#define HID_RI_UNIT_EXPONENT( DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x50, DataBits, __VA_ARGS__)
#define HID_RI_UNIT(          DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x60, DataBits, __VA_ARGS__)/** Encoded unit value \see \ref HID_UNITS_ENCODE */
#define HID_RI_REPORT_SIZE(   DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x70, DataBits, __VA_ARGS__)/** Unsigned integer specifying the size of the report fields in bits. */
#define HID_RI_REPORT_ID(     DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x80, DataBits, __VA_ARGS__)/** Unsigned value that specifies the Report ID. */
#define HID_RI_REPORT_COUNT(  DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0x90, DataBits, __VA_ARGS__)/** Unsigned integer specifying the number of data fields for the item */
#define HID_RI_PUSH(          DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0xA0, DataBits, __VA_ARGS__)/** Places a copy of the global item state table on the stack. */
#define HID_RI_POP(           DataBits, ...) _HID_RI_ENTRY(HID_RI_TYPE_GLOBAL, 0xB0, DataBits, __VA_ARGS__)/** Replaces the item state table with the top structure from the stack. */

/** HID Report Descriptor Local items */


#define HID_RI_LOCAL_USAGE         0x00 /** Represents a suggested usage for the item or collection. */
#define HID_RI_LOCAL_USAGE_MINIMUM 0x10 /** Defines the starting usage associated with an array or bitmap */
#define HID_RI_LOCAL_USAGE_MAXIMUM 0x20 /** Defines the ending usage associated with an array or bitmap. */
#define HID_RI_LOCAL_DESIGR_INDEX  0x30 /** Determines the body part used for a control. */
#define HID_RI_LOCAL_DESIGR_MIN    0x40 /** Defines the index of the starting designator associated with an array or bitmap. */
#define HID_RI_LOCAL_DESIGR_MAX    0x50 /** Defines the index of the ending designator associated with an array or bitmap. */
#define HID_RI_LOCAL_STR_INDEX     0x70 /** String index for a String descriptor; allows a string to be associated with a particular item or control. */
#define HID_RI_LOCAL_STR_MINIMUM   0x80 /** Specifies the first string index when assigning a group of sequential strings to controls in an array or bitmap. */
#define HID_RI_LOCAL_STR_MAXIMUM   0x90 /** Specifies the last string index when assigning a group of sequential strings to controls in an array or bitmap. */
#define HID_RI_LOCAL_DELIMITER     0xA0 /** Defines the beginning or end of a set of local items (1 = open set, 0 = close set). */

#define HID_RI_USAGE(DataBits        , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x00, DataBits, __VA_ARGS__) /** Represents a suggested usage for the item or collection. */
#define HID_RI_USAGE_MINIMUM(DataBits, ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x10, DataBits, __VA_ARGS__) /** Defines the starting usage associated with an array or bitmap */
#define HID_RI_USAGE_MAXIMUM(DataBits, ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x20, DataBits, __VA_ARGS__) /** Defines the ending usage associated with an array or bitmap. */
#define HID_RI_DESIGR_INDEX(DataBits , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x30, DataBits, __VA_ARGS__) /** Determines the body part used for a control. */
#define HID_RI_DESIGR_MIN(DataBits   , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x40, DataBits, __VA_ARGS__) /** Defines the index of the starting designator associated with an array or bitmap. */
#define HID_RI_DESIGR_MAX(DataBits   , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x50, DataBits, __VA_ARGS__) /** Defines the index of the ending designator associated with an array or bitmap. */
#define HID_RI_STR_INDEX(DataBits    , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x70, DataBits, __VA_ARGS__) /** String index for a String descriptor; allows a string to be associated with a particular item or control. */
#define HID_RI_STR_MINIMUM(DataBits  , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x80, DataBits, __VA_ARGS__) /** Specifies the first string index when assigning a group of sequential strings to controls in an array or bitmap. */
#define HID_RI_STR_MAXIMUM(DataBits  , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0x90, DataBits, __VA_ARGS__) /** Specifies the last string index when assigning a group of sequential strings to controls in an array or bitmap. */
#define HID_RI_DELIMITER(DataBits    , ... ) _HID_RI_ENTRY( HID_RI_TYPE_LOCAL , 0xA0, DataBits, __VA_ARGS__) /** Defines the beginning or end of a set of local items (1 = open set, 0 = close set). */

/** HID Report Descriptor for most common cases
 */
#define HID_INPUT( data )           HID_RI_INPUT(      8, data)        /**  HID_RI_INPUT */
#define HID_OUTPUT( data )          HID_RI_OUTPUT(     8, data)        /**  HID_RI_OUTPUT */
#define HID_COLLECTION(data)        HID_RI_COLLECTION( 8, data)        /**  HID_RI_COLLECTION */
#define HID_FEATURE(data)           HID_RI_FEATURE(    8, data)        /**  HID_RI_FEATURE */
#define HID_END_COLLECTION          HID_RI_END_COLLEC(0)               /**  HID_RI_END_COLLECTION */
#define HID_USAGE_PAGE(data)        HID_RI_USAGE_PAGE(8, data)         /** Range [0; 0xFF];  HID_RI_USAGE_PAGE */
#define HID_LOGICAL_MINIMUM(data)   HID_RI_LOGICAL_MINIMUM(8, data)    /** Range [-127; 127];  HID_RI_LOGICAL_MINIMUM */
#define HID_LOGICAL_MAXIMUM(data)   HID_RI_LOGICAL_MAXIMUM(8, data)    /** Range [-127; 127];  HID_RI_LOGICAL_MAXIMUM */
#define HID_PHYSICAL_MINIMUM(data)  HID_RI_PHYSICAL_MINIMUM(16, data)  /** Range [-32767; 32767];  HID_RI_PHYSICAL_MINIMUM */
#define HID_PHYSICAL_MAXIMUM(data)  HID_RI_PHYSICAL_MAXIMUM(16, data)  /** Range [-32767; 32767];  HID_RI_PHYSICAL_MAXIMUM */
#define HID_UNIT_EXPONENT(data)     HID_RI_UNIT_EXPONENT(8, data)      /** Range [-127; 127];  HID_RI_UNIT_EXPONENT */
#define HID_UNIT(bits, data)        HID_RI_UNIT(bits, data)            /**  HID_RI_UNIT */
#define HID_REPORT_SIZE(data)       HID_RI_REPORT_SIZE(8, data)        /** Range [0; 0xFF];  HID_RI_REPORT_SIZE */
#define HID_REPORT_ID(data)         HID_RI_REPORT_ID(8, data)          /** Range [0; 0xFF];  HID_RI_REPORT_ID */
#define HID_REPORT_COUNT(data)      HID_RI_REPORT_COUNT(8, data)       /** Range [0; 0xFF];  HID_RI_REPORT_COUNT */
#define HID_PUSH                    HID_RI_PUSH( 0 )                   /**  HID_RI_PUSH */
#define HID_POP                     HID_RI_POP( 0 )                    /**  HID_RI_POP */
#define HID_USAGE(data)             HID_RI_USAGE(         8, data )    /** Range [0; 0xFF];  HID_RI_USAGE */
#define HID_USAGE_MINIMUM(data)     HID_RI_USAGE_MINIMUM( 8, data )    /** Range [0; 0xFF];  HID_RI_USAGE_MINIMUM */
#define HID_USAGE_MAXIMUM(data)     HID_RI_USAGE_MAXIMUM( 8, data )    /** Range [0; 0xFF];  HID_RI_USAGE_MAXIMUM */


/** Macros for the units encoding
 *  HID_UNITS_ENCODE
 */
#define HID_UNIT_NONE               0x00                /** No system. */
#define HID_UNIT_CGS_LINEAR         0x01                /** Centimeter-Gram-Second metric linear system. \note USB-HID defines this as SI linear.*/
#define HID_UNIT_CGS_ROTATION       0x02                /** Centimeter-Gram-Second metric rotation system. \note USB-HID defines this as SI rotation.*/
#define HID_UNIT_IMPERIAL_LINEAR    0x03                /** Imperial linear system.*/
#define HID_UNIT_IMPERIAL_ROTATION  0x04                /** Imperial rotation system.*/
#define HID_UNIT_LENGTH(exp)       ((exp & 0x0F) << 4)  /** Length, position, distance unit. cm (CGS), inch (Imperial) */
#define HID_UNIT_ANGLE(exp)        ((exp & 0x0F) << 4)  /** Angle unit. radians(CGS), degree (Imperial) */
#define HID_UNIT_MASS(exp)         ((exp & 0x0F) << 8)  /** Mass unit. gram (CGS), slug (Imperial) */
#define HID_UNIT_TIME(exp)         ((exp & 0x0F) << 12) /** Time unit. secound. */
#define HID_UNIT_TEMPERATURE(exp)  ((exp & 0x0F) << 16) /** Temperature unit. Kelvin (CGS), Fahrenheit (Imperial) */
#define HID_UNIT_CURRENT(exp)      ((exp & 0x0F) << 20) /** Current unit. Ampere */
#define HID_UNIT_LUMINOUS(exp)     ((exp & 0x0F) << 24) /** Luminous intensity unit. Candela */


/**Macros for the some SI named units */

/** SI length. m = 1E2 * cm */
#define HID_UNIT_METER(exp) \
    HID_UNIT(8, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1)), \
    HID_UNIT_EXPONENT(exp + 2)

/** SI mass. kg = 1E3 * g
 */
#define HID_UNIT_KG(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1), \
    HID_UNIT_EXPONENT(exp + 3)

/** SI time. 1s
 */
#define HID_UNIT_SECOND(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(1), \
    HID_UNIT_EXPONENT(exp + 0)

/** SI velocity. m/s = 1E2 * cm/s
 */
#define HID_UNIT_MPS(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-1)), \
    HID_UNIT_EXPONENT(exp + 2)

/** SI acceleration. m/s^2 = 1E2 * cm/s^2
 */
#define HID_UNIT_MPS2(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-2)), \
    HID_UNIT_EXPONENT(exp + 2)

/** SI force. N = 1E5 * g * cm / s^2
 */
#define HID_UNIT_NEWTON(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(1) | HID_UNIT_TIME(-2)), \
    HID_UNIT_EXPONENT(exp + 5)
/** SI energy. J = 1E7 * g * cm^2 / s^2 */
#define HID_UNIT_JOULE(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-2)), \
    HID_UNIT_EXPONENT(exp + 7)

/** SI power. W = 1E7 * g * cm^2 / s^3
 */
#define HID_UNIT_WATT(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) | HID_UNIT_TIME(-3)), \
    HID_UNIT_EXPONENT(exp + 7)

/** SI pressure. Pa = 1E1 * g / (cm * s^2)
 */

#define HID_UNIT_PASCAL(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(-1) | HID_UNIT_TIME(-2)), \
    HID_UNIT_EXPONENT(exp + 1)

/** SI frequency. Hz = 1 / s
 */
#define HID_UNIT_HERTZ(exp) \
    HID_UNIT(16, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(-1)), \
    HID_UNIT_EXPONENT(exp + 0)

/** SI current. A
 */
#define HID_UNIT_AMPERE(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_CURRENT(1)), \
    HID_UNIT_EXPONENT(exp)

/** SI voltage. V = W / A  = 1E7 * g * cm^2 / (s^3 * A)
 */
#define HID_UNIT_VOLT(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) |  HID_UNIT_TIME(-3) | HID_UNIT_CURRENT(-1)), \
    HID_UNIT_EXPONENT(exp + 7)

/** SI resistance. Ohm = 1E7 * g * cm^2 / (s^3 * A^2)
 */
#define HID_UNIT_OHM(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) |  HID_UNIT_TIME(-3) | HID_UNIT_CURRENT(-2)), \
    HID_UNIT_EXPONENT(exp + 7)

/** SI inductance. H = 1E7 * g * cm^2 / (s^2 * A^2)
 */
#define HID_UNIT_HENRY(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_LENGTH(2) |  HID_UNIT_TIME(-2) | HID_UNIT_CURRENT(-2)), \
    HID_UNIT_EXPONENT(exp + 7)

/** SI  capacitance. F = 1E-7 * s^4 * A^2 / (cm^2 * g)
 */
#define HID_UNIT_FARAD(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(-1) | HID_UNIT_LENGTH(-2) |  HID_UNIT_TIME(4) | HID_UNIT_CURRENT(2)), \
    HID_UNIT_EXPONENT(exp - 7)

/** SI  electric charge. C = s * A
 */
#define HID_UNIT_COULOMB(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_TIME(1) | HID_UNIT_CURRENT(1)), \
    HID_UNIT_EXPONENT(exp)

/** SI  magnetic flux density. T = 1E3 * g / (s^2 * A)
 */
#define HID_UNIT_TESLA(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_MASS(1) | HID_UNIT_TIME(-2) | HID_UNIT_CURRENT(-1)), \
    HID_UNIT_EXPONENT(exp)

/** SI  illuminance. lx = 1E4 * cd / cm^2
 */
#define HID_UNIT_LUX(exp) \
    HID_UNIT(32, HID_UNIT_CGS_LINEAR | HID_UNIT_LENGTH(-1) | HID_UNIT_LUMINOUS(1)), \
    HID_UNIT_EXPONENT(exp)



#endif

