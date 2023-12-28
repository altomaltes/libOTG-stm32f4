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

#ifndef HID_PAGE_DESKTOP

/**  USB_HID
 *
 * Contains USB HID Usages definitions for Generic Desktop Page
 *
 * [HID Usage Tables Version 1.12](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
 */

#define HID_PAGE_DESKTOP       0x01    /** Desktop usage page.*/

/** Application Usages
 */
#define HID_DESKTOP_POINTER    0x01  /** CP Pointer control.*/
#define HID_DESKTOP_MOUSE      0x02  /** CA Mouse.*/
#define HID_DESKTOP_JOYSTICK   0x04  /** CA Joystick.*/
#define HID_DESKTOP_GAMEPAD    0x05  /** CA Gamepad.*/
#define HID_DESKTOP_KEYBOARD   0x06  /** CA Keybiard.*/
#define HID_DESKTOP_KEYPAD     0x07  /** CA Keypad.*/
#define HID_DESKTOP_MULTIAXIS  0x08  /** CA 3D input device.*/
#define HID_DESKTOP_TABLET     0x09  /** CA System controls on Tablet PCs.*/


/** Axis Usages
 */
#define HID_DESKTOP_X           0x30  /** DV Linear translation in the X direction.*/
#define HID_DESKTOP_Y           0x31  /** DV Linear translation in the Y direction.*/
#define HID_DESKTOP_Z           0x32  /** DV Linear translation in the Z direction.*/
#define HID_DESKTOP_RX          0x33  /** DV Rotation about X axis.*/
#define HID_DESKTOP_RY          0x34  /** DV Rotation about Y axis.*/
#define HID_DESKTOP_RZ          0x35  /** DV Rotation about Z axis.*/


/** Miscellaneous Controls
 */
#define HID_DESKTOP_SLIDER          0x36    /** DV Linear control for a variable value.*/
#define HID_DESKTOP_DIAL            0x37    /** DV Rotary control for a variable value.*/
#define HID_DESKTOP_WHEEL           0x38    /** DV Rotary control for a variable value.*/
#define HID_DESKTOP_HAT_SWITCH      0x39    /** DV A specialized mechanical configuration of                                             * switches generating a variable value with a null state.*/
#define HID_DESKTOP_MOTION_WAKEUP   0x3C    /** DF Enables the generation of a USB remote                                             * wakeup when the device detects motion.*/
#define HID_DESKTOP_START           0x3D    /** OOC Session start button.*/
#define HID_DESKTOP_SELECT          0x3E    /** OOC Application option select button.*/
#define HID_DESKTOP_RESOLUTION_MULT 0x48    /** DV Resolution Multiplier for a Control.*/


/** Vector Usages
 */
#define HID_DESKTOP_VX              0x40    /** DV Vector in the X direction.*/
#define HID_DESKTOP_VY              0x41    /** DV Vector in the Y direction.*/
#define HID_DESKTOP_VZ              0x42    /** DV Vector in the Z direction.*/
#define HID_DESKTOP_VBRX            0x43    /** DV Relative Vector in the X direction.*/
#define HID_DESKTOP_VBRY            0x44    /** DV Relative vector in the Y direction.*/
#define HID_DESKTOP_VBRZ            0x45    /** DV Relative vector in the Z direction.*/
#define HID_DESKTOP_VNO             0x46    /** DV A non oriented vector or value.*/


/** System Controls
 */
#define HID_DESKTOP_SYS_CONTROL     0x80    /** CA Application-level collection.*/
#define HID_DESKTOP_SYS_CONTEXT_MNU 0x84    /** OSC Evokes a context-sensitive menu.*/
#define HID_DESKTOP_SYS_MAIN_MNU    0x85    /** OSC Evokes the OS main-level selection menu.*/
#define HID_DESKTOP_SYS_APP_MNU     0x86    /** OSC Displays an application-specific menu.*/
#define HID_DESKTOP_SYS_MENU_HELP   0x87    /** OSC Displays the help menu.*/
#define HID_DESKTOP_SYS_MENU_EXIT   0x88    /** OSC Exits a menu.*/
#define HID_DESKTOP_SYS_MENU_SELECT 0x89    /** OSC Selects a menu item.*/
#define HID_DESKTOP_SYS_MENU_RIGHT  0x8A    /** RTC Menu select right.*/
#define HID_DESKTOP_SYS_MENU_LEFT   0x8B    /** RTC Menu select left.*/
#define HID_DESKTOP_SYS_MENU_UP     0x8C    /** RTC Menu select up.*/
#define HID_DESKTOP_SYS_MENU_DOWN   0x8D    /** RTC Menu select down.*/


/** Power Controls
 */
#define HID_DESKTOP_SYS_PWR_DOWN    0x81    /** OSC Power down control.*/
#define HID_DESKTOP_SYS_SLEEP       0x82    /** OSC Sleep control.*/
#define HID_DESKTOP_SYS_WAKEUP      0x83    /** OSC Wakeup control.*/
#define HID_DESKTOP_SYS_RST_COLD    0x8E    /** OSC Cold restart control.*/
#define HID_DESKTOP_SYS_RST_WARM    0x8F    /** OSC Warm restart control.*/
#define HID_DESKTOP_SYS_DOCK        0xA0    /** OSC Prepare for docking.*/
#define HID_DESKTOP_SYS_UNDOCK      0xA1    /** OSC Prepare for undocking. */
#define HID_DESKTOP_SYS_SETUP       0xA2    /** OSC Enter to BIOS-level setup.*/
#define HID_DESKTOP_SYS_SPKR_MUTE   0xA7    /** OSC Mute system speakers.*/
#define HID_DESKTOP_SYS_HIBERNATE   0xA8    /** OSC System hibernate control.*/


/** Buffered Bytes
 */
#define HID_DESKTOP_COUNTEDBUF      0x3A    /** CL Used with buffered byte data to indicate                                             * the number of valid bytes in the buffered-byte field.*/
#define HID_DESKTOP_BYTECOUNT       0x3B    /** DV Defines a report field that indicates the                                             * number of meaningful data bytes in an associated                                             * buffered-byte field.*/


/** Direction Pads
 */
#define HID_DESKTOP_DPAD_UP         0x90    /** OOC Top of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_DOWN       0x91    /** OOC Bottom of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_RIGHT      0x92    /** OOC Right side of a Direction Pad is pressed.*/
#define HID_DESKTOP_DPAD_LEFT       0x93    /** OOC Left side of a Direction Pad is pressed.*/


/** Feature Notifications
 */
#define HID_DESKTOP_FEATURE_NOTIFY  0x47    /** DV This usage is declared in an Input report and is used as a notification to the host that the contents of a specific Feature report has changed.*/


/** Software Flow Control
 */
#define HID_DESKTOP_SYS_BREAK       0xA3    /** OSC System break control.*/
#define HID_DESKTOP_SYS_DBG_BREAK   0xA4    /** OSC System debugger break control.*/
#define HID_DESKTOP_APP_BREAK       0xA5    /** OSC Application break control.*/
#define HID_DESKTOP_APP_DBG_BREAK   0xA6    /** OSC Application debugger break control.*/


/** System Display Control
 */
#define HID_DESKTOP_SYS_DISP_INVERT 0xB0    /** OSC Set display to render in inverted colors.*/
#define HID_DESKTOP_SYS_DISP_INT    0xB1    /** OSC Set the captive display as the primary display.*/
#define HID_DESKTOP_SYS_DISP_EXT    0xB2    /** OSC Set the external display as the primary display.*/
#define HID_DESKTOP_SYS_DISP_BOTH   0xB3    /** OSC Use both internal and external displays as primary diaplay.*/
#define HID_DESKTOP_SYS_DISP_DUAL   0xB4    /** OSC Use both internal and external displays as primary and secondary diaplays.*/
#define HID_DESKTOP_SYS_DISP_TGL    0xB5    /** OSC Toggles internal/external/both displays.*/
#define HID_DESKTOP_SYS_DISP_SWAP   0xB6    /** OSC Swap primary/secondary displays.*/
#define HID_DESKTOP_SYS_DISP_AUTO   0xB7    /** OCS Toggles LCD autoscale.*/


#endif

