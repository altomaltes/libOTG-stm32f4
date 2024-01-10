/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#ifndef _TUSB_HUB_H_
#define _TUSB_HUB_H_


/*! D1...D0: Logical Power Switching Mode

 00: Ganged power switching (all ports’power at once)
 01: Individual port power switching
 1X: Reserved. Used only on 1.0 compliant hubs that implement no power switching
 D2: Identifies a Compound Device
  0: Hub is not part of a compound device.
  1: Hub is part of a compound device.

 D4...D3: Over-current Protection Mode
      00: Global Over-current Protection. The hub reports over-current as a summation of all ports’current draw, without a breakdown of individual port over-current status.
      01: Individual Port Over-current Protection. The hub reports over-current on a per-port basis. Each port has an over-current status.
      1X: No Over-current Protection. This option is allowed only for bus-powered hubs that do not implement over-current protection.

  D6...D5: TT Think TIme
       00:  TT requires at most 8 FS bit times of inter transaction gap on a full-/low-speed downstream bus.
       01:  TT requires at most 16 FS bit times.
       10:  TT requires at most 24 FS bit times.
       11:  TT requires at most 32 FS bit times.
       D7:  Port Indicators Supported
        0:  Port Indicators are not supported on its downstream facing ports and the PORT_INDICATOR request has no effect.
        1:  Port Indicators are supported on its downstream facing ports and the PORT_INDICATOR request controls the indicators. See Section 11.5.3.
 D15...D8: Reserved
 *
 */

#include "usbh_core.h"

typedef struct PACKED
{ byte  bLength          ; ///< Size of this descriptor in bytes
  byte  bDescriptorType  ; ///< ENDPOINT Descriptor Type
  byte  bEndpointAddress ; ///< The address of the endpoint on the USB device described by this descriptor. The address is encoded as follows: \n Bit 3...0: The endpoint number \n Bit 6...4: Reserved, reset to zero \n Bit 7: Direction, ignored for control endpoints 0 = OUT endpoint 1 = IN endpoint.

  struct PACKED
  { byte xfer  : 2;
    byte sync  : 2;
    byte usage : 2;
    byte       : 2;
  } bmAttributes     ; ///< This field describes the endpoint's attributes when it is configured using the bConfigurationValue. \n Bits 1..0: Transfer Type \n- 00 = Control \n- 01 = Isochronous \n- 10 = Bulk \n- 11 = Interrupt \n If not an isochronous endpoint, bits 5..2 are reserved and must be set to zero. If isochronous, they are defined as follows: \n Bits 3..2: Synchronization Type \n- 00 = No Synchronization \n- 01 = Asynchronous \n- 10 = Adaptive \n- 11 = Synchronous \n Bits 5..4: Usage Type \n- 00 = Data endpoint \n- 01 = Feedback endpoint \n- 10 = Implicit feedback Data endpoint \n- 11 = Reserved \n Refer to Chapter 5 of USB 2.0 specification for more information. \n All other bits are reserved and must be reset to zero. Reserved bits must be ignored by the host.

  struct PACKED
  { word size           : 11; ///< Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected. \n For isochronous endpoints, this value is used to reserve the bus time in the schedule, required for the per-(micro)frame data payloads. The pipe may, on an ongoing basis, actually use less bandwidth than that reserved. The device reports, if necessary, the actual bandwidth used via its normal, non-USB defined mechanisms. \n For all endpoints, bits 10..0 specify the maximum packet size (in bytes). \n For high-speed isochronous and interrupt endpoints: \n Bits 12..11 specify the number of additional transaction opportunities per microframe: \n- 00 = None (1 transaction per microframe) \n- 01 = 1 additional (2 per microframe) \n- 10 = 2 additional (3 per microframe) \n- 11 = Reserved \n Bits 15..13 are reserved and must be set to zero.
    word hs_period_mult : 2;
    word : 0;
  } wMaxPacketSize;

  byte  bInterval        ; ///< Interval for polling endpoint for data transfers. Expressed in frames or microframes depending on the device operating speed (i.e., either 1 millisecond or 125 us units). \n- For full-/high-speed isochronous endpoints, this value must be in the range from 1 to 16. The bInterval value is used as the exponent for a \f$ 2^(bInterval-1) \f$ value; e.g., a bInterval of 4 means a period of 8 (\f$ 2^(4-1) \f$). \n- For full-/low-speed interrupt endpoints, the value of this field may be from 1 to 255. \n- For high-speed interrupt endpoints, the bInterval value is used as the exponent for a \f$ 2^(bInterval-1) \f$ value; e.g., a bInterval of 4 means a period of 8 (\f$ 2^(4-1) \f$) . This value must be from 1 to 16. \n- For high-speed bulk/control OUT endpoints, the bInterval must specify the maximum NAK rate of the endpoint. A value of 0 indicates the endpoint never NAKs. Other values indicate at most 1 NAK each bInterval number of microframes. This value must be in the range from 0 to 255. \n Refer to Chapter 5 of USB 2.0 specification for more information.
} tusb_desc_endpoint_t;


typedef struct PACKED
{ byte  bLength        ; ///< Size of descriptor
  byte  bDescriptorType; ///< Other_speed_Configuration Type
  byte  bNbrPorts      ;

  struct PACKED
  { word pSwitch : 2;
    word compound: 1;
    word oCurrent: 2;
    word tTime   : 2;
    word indicat : 1;
    word reserved: 8;
  } wHubCharacteristics;

  byte  bPwrOn2PwrGood    ;
  byte  bHubContrCurrent  ;
  byte  DeviceRemovable   ; // bitmap each bit for a port (from bit1)
  byte  PortPwrCtrlMask   ; // just for compatibility, should be 0xff
} descriptor_hub_desc_t;

enum
{ HUB_REQUEST_GET_STATUS      = 0
, HUB_REQUEST_CLEAR_FEATURE   = 1
, HUB_REQUEST_SET_FEATURE     = 3

, HUB_REQUEST_GET_DESCRIPTOR  = 6
, HUB_REQUEST_SET_DESCRIPTOR  = 7
, HUB_REQUEST_CLEAR_TT_BUFFER = 8
, HUB_REQUEST_RESET_TT        = 9
, HUB_REQUEST_GET_TT_STATE    = 10
, HUB_REQUEST_STOP_TT         = 11
};


/* data in response of HUB_REQUEST_GET_STATUS, wIndex = 0 (hub)
 */
typedef struct
{ union
  { struct PACKED
    { word local_power_source : 1;
      word over_current       : 1;
      word                    : 14;
    };

    word value;
  } status, change;
} hub_status_response_t;



enum
{ HUB_FEATURE_PORT_CONNECTION  =  0
, HUB_FEATURE_PORT_ENABLE      =  1
, HUB_FEATURE_PORT_SUSPEND     =  2
, HUB_FEATURE_PORT_OVER_CURRENT=  3
, HUB_FEATURE_PORT_RESET       =  4

, HUB_FEATURE_PORT_POWER       =  8
, HUB_FEATURE_PORT_LOW_SPEED   =  9
, HUB_FEATURE_PORT_HIGH_SPEED  = 10
, HUB_FEATURE_PORT_TEST_MODE   = 11
, HUB_FEATURE_PORT_INDICATOR   = 12

/*
, HUB_FEATURE_PORT_CONNECTION_CHANGE   = 16
, HUB_FEATURE_PORT_ENABLE_CHANGE       = 17
, HUB_FEATURE_PORT_SUSPEND_CHANGE      = 18
, HUB_FEATURE_PORT_OVER_CURRENT_CHANGE = 19
, HUB_FEATURE_PORT_RESET_CHANGE        = 20
, HUB_FEATURE_PORT_TEST                = 21
, HUB_FEATURE_PORT_INDICATOR           = 22
*/

, HUB_FEATURE_HUB_LOCAL_POWER_CHANGE = 0
, HUB_FEATURE_HUB_OVER_CURRENT_CHANGE


, HCD_EVENT_DEVICE_REMOVE = 120
, HCD_EVENT_DEVICE_ATTACH

};




#endif /* _TUSB_HUB_H_ */

