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

#ifndef HID_PAGE_SIMULATION


/**\ingroup USB_HID
 * \addtogroup USB_HID_USAGES_SIMUL HID Usage Tables for Simulation
 * Contains USB HID Usages definitions for Simulation Controls Page
 * \details This module based on
 * + [HID Usage Tables Version 1.12](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
 */

#define HID_PAGE_SIMULATION             0x02    /** Sumulation usage page.*/
#define HID_SIMUL_SPORTS                0x08    /** CA Genetic sports simulation device.*/

/** Flight Simulation Devices
 */
#define HID_SIMUL_FLIGHT                0x01    /** CA Airplane simulation device.*/
#define HID_SIMUL_SPACESHIP             0x04    /** CA Spaceship simulation device.*/
#define HID_SIMUL_AIRPLANE              0x09    /** CA Airplane simulation device.*/
#define HID_SIMUL_HELICOPTER            0x0A    /** CA Helicopter simulation device.*/
#define HID_SIMUL_ALIERON               0xB0    /** DV Aileron control.*/
#define HID_SIMUL_ALIERIN_TRIM          0xB1    /** DV Aileron fine adjustment.*/
#define HID_SIMUL_ANTI_TORQUE           0xB2    /** DV Rudder pedals.*/
#define HID_SIMUL_AUTOPILOT_ENABLE      0xB3    /** OOC Autopilot switch.*/
#define HID_SIMUL_CHAFF_RELEASE         0xB4    /** OCS Chaff Release control.*/
#define HID_SIMUL_COLLECTIVE_CONTROL    0xB5    /** DV Vertical acceleration lift confrol.*/
#define HID_SIMUL_CYCLIC_CONTROL        0x22    /** CP Helicopter cyclic control.*/
#define HID_SIMUL_CYCLIC_TRIM           0x23    /** CP Cyclic fine adjustments.*/
#define HID_SIMUL_DRIVE_BRAKE           0xB6    /** DV Air brake control.*/
#define HID_SIMUL_ELECTR_COUNTERMEAS    0xB7    /** OOC Enables electronic countermeasures.*/
#define HID_SIMUL_ELEVATOR              0xB8    /** DV Elevator control.*/
#define HID_SIMUL_ELEVATOR_TRIM         0xB9    /** DV Elevator fine adjustment.*/
#define HID_SIMUL_FLIGHT_COMM           0xBC    /** OOC Flight Communications switch.*/
#define HID_SIMUL_FLARE_RELEASE         0xBD    /** OCS Flare release button.*/
#define HID_SIMUL_FLIGHT_CONTROL_STICK  0x20    /** CA Pitch and Roll control.*/
#define HID_SIMUL_FLIGHT_STICK          0x21    /** CA Pitch and Roll control for games.*/
#define HID_SIMUL_LANDING_GEAR          0xBE    /** OOC Landing gear control.*/
#define HID_SIMUL_RUDDER                0xBA    /** DV Rudder control.*/
#define HID_SIMUL_TOE_BRAKE             0xBF    /** DV Toe Brake control.*/
#define HID_SIMUL_THROTTLE              0xBB    /** DV Trottle control.*/
#define HID_SIMUL_TRIGGER               0xC0    /** MC Firearm trigger control.*/
#define HID_SIMUL_WEAPONS_ARM           0xC1    /** OOC Enables weapons system.*/
#define HID_SIMUL_WEAPONS_SELECT        0xC2    /** OSC Select weapon.*/
#define HID_SIMUL_WING_FLAPS            0xC3    /** DV wing flap control.*/
#define HID_SIMUL_FLIGHT_YOKE           0x24    /** CA A Flight Yoke controls.*/


/** Automobile Simulation Devices
 */
#define HID_SIMUL_AUTOMOBILE            0x02    /** CA Automobile or truck simulation device.*/
#define HID_SIMUL_ACCELERATOR           0xC4    /** DV Accelerator control.*/
#define HID_SIMUL_BRAKE                 0xC5    /** DV Brake control.*/
#define HID_SIMUL_CLUTCH                0xC6    /** DV Clutch control.*/
#define HID_SIMUL_SHIFTER               0xC7    /** DV Shifting gears control.*/
#define HID_SIMUL_STEERING              0xC8    /** DV Steering wheel control.*/


/** Tank Simulation Devices
 */
#define HID_SIMUL_TANK                  0x03    /** CA Treaded vehicle simulation device.*/
#define HID_SIMUL_TRACK_CONTROL         0x25    /** CP Direction and velocity controls.*/
#define HID_SIMUL_TURRET_DIRECTION      0xC9    /** DV Turret control right-left.*/
#define HID_SIMUL_BARREL_ELEVATION      0xCA    /** DV Gun elevation control.*/


/** Maritime Simulation Devices
 */
#define HID_SIMUL_SUBMARINE             0x05    /** CA Submarine control.*/
#define HID_SIMUL_SAILING               0x06    /** CA Sailing simulatiion control.*/
#define HID_SIMUL_DIVE_PLANE            0xCB    /** DV Dive plane control*/
#define HID_SIMUL_BALLAST               0xCC    /** DV Ballast control.*/


/** Two-wheeled Simulation Devices
 */
#define HID_SIMUL_MOTOCYCLE             0x07    /** CA Motocycle simulation device.*/
#define HID_SIMUL_BICYCLE               0x0C    /** CA Bycicle simulation device*/
#define HID_SIMUL_BICYCLE_CRANK         0xCD    /** DV Bycicle crank control.*/
#define HID_SIMUL_HANDLE_BARS           0xCE    /** DV Steering control.*/
#define HID_SIMUL_FRONT_BRAKE           0xCF    /** DV Front brake control.*/
#define HID_SIMUL_REAR_BRAKE            0xD0    /** DV Rear brake control.*/


/** Miscellaneous Simulation Devices
 */
#define HID_SIMUL_MAGIC_CARPET          0x0B    /** CA Magic carpet simulation device.*/


#endif

