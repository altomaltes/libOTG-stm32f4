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

#include "usbh-hub.h"

enum
{ PHASE_IDLE
, PHASE_GET_DESC
, PHASE_SET_PORT_POW
, PHASE_SET_PORT_COMPLETE
, PHASE_HUB_MACHINE
, PHASE_HUB_STATUS
, PHASE_PORT_STATUS
, PHASE_CONNECTION_EVENT
, PHASE_RESET_PORT
, PHASE_PORT_DONE
};

struct
{ byte intEp;
  byte hcInt;
  byte iface;
  byte phase;
  byte portIdx;
  byte portCount;

  struct
  { word status;
    word change;
  } alter;

  byte statusChange;  // !! may falls over buff

} HUB_Machine;



/**
 * @brief hubPort GetStatusComplete
 *
 *
 * @retval
 */
//static void hubPortGetStatusComplete ()
//{
 // hub_interface_t* p_hub = get_itf(daddr);
  //byte const port_num = (byte)tu_le16toh( xfer->setup->wIndex );

  // Connection change
    // Other changes are: L1 state
    // TODO clear change

    // prepare for next hub status
    // TODO continue with status_change, or maybe we can do it again with status
   // hubEdptStatusXfer(daddr);
//}

/**
 * @brief hubPort GetStatus
 *
 * @retval
 */
byte hubPortGetStatus( byte hub_addr, byte hub_port
                     , void* resp
                     , void * complete_cb, dword * user_data)
{ //tusb_control_request_t const request =
  //{ .bmRequestType_bit =
  //  { .recipient = (hub_port == 0) ? TUSB_REQ_RCPT_DEVICE
  //                                 : TUSB_REQ_RCPT_OTHER
  //  , .type      = TUSB_REQ_TYPE_CLASS
//    , .direction = TUSB_DIR_IN
//    }
//  , .bRequest = HUB_REQUEST_GET_STATUS
//  , .wValue   = 0
//  , .wIndex   = hub_port
//  , .wLength  = 4
//  };
  return( 0 );

}

//--------------------------------------------------------------------+
// CLASS-USBH API (don't require to verify parameters)
//--------------------------------------------------------------------+

/**
 * @brief hubClose
 *
 * @retval
 */
void hubClose( byte dev_addr )
{
}


static byte USBH_HUB_Handle( byte epAddr );


static void testForNextPort( void )
{ while( HUB_Machine.portIdx <= HUB_Machine.portCount )
  { byte bMap= 1 << HUB_Machine.portIdx;
    if ( HUB_Machine.statusChange & bMap )  // 0 -> Hub event  > 0 port event
    { HUB_Machine.statusChange &= ~bMap;
      HUB_Machine.phase= HUB_Machine.portIdx ? PHASE_PORT_STATUS
                                             : PHASE_HUB_STATUS;
      USBHsetControlChannelAddr( DEFAULT_DEVADDR, USBH_HUB_Handle );   /* Recover the HUB primary address */
      UHOSTgetClassStatus(  HUB_Machine.portIdx
                         , &HUB_Machine.alter
                         , 4 );
      return;  // abobe machine reasks
    }
    HUB_Machine.portIdx++;
  }

  HUB_Machine.phase= PHASE_HUB_MACHINE;
}

/**
 * @brief doEvent
 *        callback as response
 *
 * @retval
 */
static byte doEvent( byte what )
{ switch( what )
  { case HCD_EVENT_DEVICE_ATTACH:
      mDelay( 100 );                               /* Debounce */
      USBH_Get_DevDesc( HUB_Machine.portIdx, 8 );  /* HUB port assigns address*/
      HUB_Machine.alter.change= 0;                 /* Do not taint device enumeration */
      HUB_Machine.phase= PHASE_PORT_DONE;
    break;

    case HCD_EVENT_DEVICE_REMOVE:
      USBHfreeDeviceChannels( HUB_Machine.portIdx );
      usbHostGotDisconnected( HUB_Machine.portIdx );
    break;

    default:
      if ( HUB_Machine.alter.change & ( 1 << what ) )
      { UHOSTclearClassFeature( what + 16,  HUB_Machine.portIdx );
        HUB_Machine.alter.change &= ~( 1 << what );
        return( 1 );
    } }

  return( 0 );
}

/**
 * @brief USBH_HUB_Handle
 *        callback as response of interrupt endpoint polling
 *
 * @retval
 */
static byte USBH_HUB_Handle( byte epAddr )
{ switch( HUB_Machine.phase )
  { case PHASE_GET_DESC:
    { descriptor_hub_desc_t * desc= (descriptor_hub_desc_t *)usbHOSTgetBuffer();

      HUB_Machine.portIdx= 1;
      HUB_Machine.portCount= desc->bNbrPorts;
      HUB_Machine.phase= PHASE_SET_PORT_POW;

      USBHclassSignaler.classHub= 1;
      usbHostGotUserApplication( USBHclassSignaler );   /* USB_CLASS_HUB addr mutes to 0 Fixed dev address*/
    }

    case PHASE_SET_PORT_POW:
      UHOSTsetClassFeature( HUB_FEATURE_PORT_POWER
                          , HUB_Machine.portIdx );
      if ( HUB_Machine.portIdx++ == HUB_Machine.portCount  ) /* Last one */
      { HUB_Machine.phase= PHASE_SET_PORT_COMPLETE;
      }
    break;

    case PHASE_SET_PORT_COMPLETE:
      USBHchannelTimer( HUB_Machine.hcInt, 10 );
      HUB_Machine.phase= PHASE_HUB_MACHINE;
    break;

    case PHASE_HUB_STATUS:
      if ( HUB_Machine.alter.change      & ( 1 << HUB_FEATURE_HUB_LOCAL_POWER_CHANGE ))
      { UHOSTclearClassFeature( HUB_FEATURE_HUB_LOCAL_POWER_CHANGE, 0 );
      }

      else if (  HUB_Machine.alter.change & ( 1 << HUB_FEATURE_HUB_OVER_CURRENT_CHANGE ))
      { UHOSTclearClassFeature( HUB_FEATURE_HUB_OVER_CURRENT_CHANGE, 0 );
      }

      else
      { testForNextPort();
      }
    break;

    case PHASE_PORT_STATUS: /* Clear any other first*/
      if ( doEvent( HUB_FEATURE_PORT_ENABLE       )) break;
      if ( doEvent( HUB_FEATURE_PORT_SUSPEND      )) break;
      if ( doEvent( HUB_FEATURE_PORT_OVER_CURRENT )) break;
      if ( doEvent( HUB_FEATURE_PORT_CONNECTION   )) { HUB_Machine.phase= PHASE_CONNECTION_EVENT; break; }
      if ( doEvent( HUB_FEATURE_PORT_RESET        )) { HUB_Machine.phase= PHASE_RESET_PORT;       break; }

      HUB_Machine.portIdx++;  /* Processed endpoint */
      testForNextPort();      /* No more events over this port */
    break;

    case PHASE_CONNECTION_EVENT:
      if ( HUB_Machine.alter.status & ( 1 << HUB_FEATURE_PORT_CONNECTION ))  /* Test for connection */
      { UHOSTsetClassFeature( HUB_FEATURE_PORT_RESET, HUB_Machine.portIdx );
        HUB_Machine.phase= PHASE_PORT_STATUS;
      }
      else /* Disconnection */
      { doEvent( HCD_EVENT_DEVICE_REMOVE );
      }
    break;

    case PHASE_RESET_PORT:
      doEvent( HCD_EVENT_DEVICE_ATTACH ); // Fallout, continuo with ports
    break;
    /* Fallout */

    case PHASE_HUB_MACHINE:      /* Test for device changes */
    break;

    case PHASE_PORT_DONE:
    break;

  }

  return( 0 );
}


/**
 * @brief  USBH_HUB_HandleBOTXferOut
 *
 * @param  None
 * @retval None
 */
static void USBH_HUB_HandleBOTXferOut( schar channel )
{ if ( channel > 0 )          /* Response */
  { HUB_Machine.portIdx= 0;   /* Start with the hub */
    testForNextPort();
  }

  else                        /* poll reached */
  { USBH_InterruptReceiveData( &HUB_Machine.statusChange  /* Fire the self machine */
                             , 1, -channel );

} }


/**
  * @brief  USBHhubInterfaceInit
  *         Interface initialization for HUB class.
  * @retval USBH_Status : Status of class request handled.
  */
hostClassLink USBHhubInterfaceInit( USBHdeviceRec * dev, void * handler )
{ USBHepDescRec * epList= dev->Ep_Desc;

  if ( dev->Itf_Desc->bInterfaceProtocol <= 1  )  // hub driver does not support multiple TT yet
  { if (( epList->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE )
    &&  ( epList->bmAttributes    == EPTYPE_INTERRUPT            ))
    {  HUB_Machine.iface= 0;
       HUB_Machine.intEp= epList->bEndpointAddress;

       HUB_Machine.hcInt=
         UHOSTopenChannel( HUB_Machine.intEp
                         , EPTYPE_INTERRUPT, epList->wMaxPacketSize                 // HUB_Machine.MSBulkOutEpSize
                         , USBH_HUB_HandleBOTXferOut );

       USBH_GetDescriptor( USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_DEVICE
                         , 0
                         , sizeof( descriptor_hub_desc_t ));
       HUB_Machine.phase= PHASE_GET_DESC;

      return( USBH_HUB_Handle );
  } }

  return( 0 );
}


