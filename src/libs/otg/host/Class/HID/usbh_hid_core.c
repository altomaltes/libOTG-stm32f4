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
 * @file usbh_hid_core.c
 *
 * original draft from MCD Application Team
 *
 *          ===================================================================
 *                                HID Class  Description
 *          ===================================================================
 *           This module manages the MSC class V1.11 following the "Device Class Definition
 *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
 *           This driver implements the following aspects of the specification:
 *             - The Boot Interface Subclass
 *             - The Mouse and Keyboard protocols
 *
 */
#include "usb_core.h"

#include "usbh_hid_core.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

byte report[ 8 ];

ALIGN_THIS( usbHidDescriptor HID_Desc );
//ALIGN_THIS( HID_Report_TypeDef   HID_Report  );
ALIGN_THIS( HID_Machine_TypeDef  HID_Machine );

volatile byte start_toggle = 0;

//typedef struct
//{ byte report_id;
//  byte usage;
//  byte usage_page[ 32 ];
//} hidReportInfoRec;


/**
 * @brief  USBH_ParseHIDDesc
 * @brief  hidParseReportDescriptor
 * @param  None
 * @retval None
 */
short hidParseReportDescriptor( usbHidDescriptor * desc, void * io )
{ USBH_DescHeader_t * pdesc= (USBH_DescHeader_t *)io;

  word CfgDescLen = LE16( (byte *)pdesc + 2 );

  if ( CfgDescLen > USB_CONFIGURATION_DESC_SIZE )
  {
    word ptr= USB_CONFIGURATION_DESC_SIZE;

    while ( ptr < CfgDescLen )
    { pdesc= USBH_GetNextDesc((byte *)pdesc, &ptr);
      byte * buff= ( byte *)io;

      if ( pdesc->bDescriptorType == HID_DTYPE_HID )
      { desc->bLength           = buff[ 0 ];
        desc->bType             = buff[ 1 ];
        desc->bcdHID            = LE16( buff + 2 );
        desc->bCountryCode      = buff[ 4 ];
        desc->bNumDescriptors   = buff[ 5 ];
        desc->bType0            = buff[ 6 ];
        desc->wDescriptorLength0= LE16( buff + 7 );
        break;
  } } }

  return( 0 );
}


/**
 * @brief  USBH_MSC_HandleBOTXfer
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 */
void USBH_HID_HandleBOTXferOut( short ep )
{
}

void USBH_HID_HandleBOTXferIn( short ep )
{ if ( ep < 0 )
  { USBH_InterruptReceiveData( report  /* Fire the self machine */
                             , HID_Machine.length
                             , -ep );
  }
  else
  { HID_Machine.cb1( report );
} }

byte classReqStatus;


/**
 * @brief  USBH_Get_HID_ReportDescriptor
 *         Issue report Descriptor command to the device. Once the response
 *         received, parse the report descriptor and update the status.
 * @param  Length : HID Report Descriptor Length
 * @retval schar : Response for USB HID Get Report Descriptor Request
 */

/* HID report descriptor is available in pdev->host.Rx_Buffer.
  In case of USB Boot Mode devices for In report handling ,
  HID report descriptor parsing is not required.
  In case, for supporting Non-Boot Protocol devices and output reports,
  user may parse the report descriptor
*/


/**
 * @brief  USBH_Get_HID_Descriptor
 *         Issue HID Descriptor command to the device. Once the response
 *         received, parse the report descriptor and update the status.
 * @param  Length : HID Descriptor Length
 * @retval schar : Response for USB HID Get Report Descriptor Request
 */


/**
 * @brief  USBH_HID_Handle
 *         The function is responsible for handling HID Class requests
 *         for HID class.
 * @retval  schar :Response for USB Set Protocol request
 */
static short USBH_HID_Handle( byte ep )
{ switch ( HID_Machine.ctl_state )   /* Switch HID state machine */
  { case HID_REQ_VOID:
    case HID_REQ_GET_HID_DESC:
      USBH_GetDescriptor( USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD
                        , HID_DTYPE_HID << 8
                        , sizeof( usbHidDescriptor ));
      HID_Machine.ctl_state= HID_REQ_GET_REPORT_DESC;
  break;

  case HID_REQ_GET_REPORT_DESC:     /* Get Report Desc, not used at this moment */
    hidParseReportDescriptor( &HID_Desc, USBHgetBuffer() );
    USBH_GetDescriptor( USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD
                      , HID_DTYPE_REPORT << 8
                      , HID_Desc.wDescriptorLength0 );
    HID_Machine.ctl_state= HID_REQ_SET_IDLE;
  break;

    case HID_REQ_SET_IDLE:     /* set Idle, place to decode above ask for */
    { USBHsetInterface( HID_SETIDLE, 0, 0 );  // (duration << 8 ) | reportId
      HID_Machine.ctl_state= HID_REQ_SET_PROTOCOL;
    }
    break;

    case HID_REQ_SET_PROTOCOL:     /* set protocol */
      USBHsetInterface( HID_SETPROTOCOL
                      , 0
                      , 1 ); /* 0 Boot Protocol 1 Report Protocol */
      HID_Machine.ctl_state= HID_REQ_CLEAR;
      HID_Machine.ctl_state= HID_REQ_FIREUP;
    break;

    case HID_REQ_FIREUP:                       /* set protocol */
      USBHclassSignaler.classHid= 1;
      usbHostGotUserApplication( USBHclassSignaler );  /* | 0x80 << MSC_Machine.hcNumIn | 0x80 << MSC_Machine.hcNumOut  );*/

      USBHchannelTimer( HID_Machine.hcNumIn, 10 );
      HID_Machine.ctl_state= HID_REQ_IDLE;
      HID_Machine.state= HID_GET_DATA;         /* Change state to issue next IN token */
    break;

    default: break;
  }

  return( 0 );
}



/**
 * @brief  USBH_Set_Report
 *         Issues Set Report
 * @param  reportType  : Report type to be sent
 * @param  reportID    : Targeted report ID for Set Report request
 * @param  reportLen   : Length of data report to be send
 * @param  reportBuff  : Report Buffer
 * @retval USBH_Status : Response for USB Set Idle request
 */
schar USBH_Set_Report( byte reportType
                           , byte reportId
                           , byte reportLen
                           , byte * reportBuff )
{ /*Control.setup.b.bmRequestType= USB_H2D
                               | USB_REQ_RECIPIENT_INTERFACE
                               | USB_REQ_TYPE_CLASS;

  Control.setup.b.bRequest = USB_HID_SET_REPORT;
  Control.setup.b.wValue.w = (reportType << 8 ) | reportId;
  Control.setup.b.wIndex.w = 0;
  Control.setup.b.wLength.w = reportLen;

  return( USBH_CtlReq( reportBuff , reportLen ));*/
  return( 0 );
}




/**
  * @brief  USBHhidInterfaceInit
  *         Interface initialization for HID class.
  * @retval
  */
hostClassLink USBHhidInterfaceInit( USBHdeviceRec * dev, void * protocolHandler )
{ byte maxEP;
  USBHinterfaceDescRec * epIface;

  HID_Machine.cb1 = protocolHandler;

  for( epIface= dev->Itf_Desc
     ; dev->Itf_Desc - epIface < USBH_MAX_NUM_INTERFACES
     ; epIface ++ )
  { if ( epIface->bInterfaceSubClass == USB_HID_SUBCLASS_BOOT )
    { USBHepDescRec        * epList=  dev->Ep_Desc;

      HID_Machine.state    = HID_IDLE;
      HID_Machine.ctl_state= HID_REQ_IDLE;
      HID_Machine.length   = epList[ 0 ].wMaxPacketSize;
      HID_Machine.poll     = epList[ 0 ].bInterval ;

      if ( HID_Machine.poll  < HID_MIN_POLL )
      { HID_Machine.poll = HID_MIN_POLL;
      }


/* Check fo available number of endpoints */
/* Find the number of EPs in the Interface Descriptor */
/* Choose the lower number in order not to overrun the buffer allocated */
      maxEP=( epIface->bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS  )
           ? epIface->bNumEndpoints
           : USBH_MAX_NUM_ENDPOINTS;


/* Decode endpoint IN and OUT address from interface descriptor */

      for ( int num= 0
          ;     num < maxEP
          ;     num++ )
      { if ( epList[ num ].bEndpointAddress & EPDIR_IN )  // IN ep
        { HID_Machine.HIDIntInEp= epList[ num ].bEndpointAddress;
          HID_Machine.hcNumIn=
            UHOSTopenChannel( epList[ num ].bEndpointAddress /* Open channel for IN endpoint */
                            , EPTYPE_INTERRUPT
                            , HID_Machine.length
                            , USBH_HID_HandleBOTXferIn );
        }
        else   // OUT ep
        { HID_Machine.hcNumOut=
            UHOSTopenChannel( epList[ num ].bEndpointAddress   /* Open channel for OUT endpoint */
                            , EPTYPE_INTERRUPT
                            , HID_Machine.length
                            , USBH_HID_HandleBOTXferOut );
      }  }

      start_toggle= 0;
      HID_Machine.ctl_state= HID_REQ_VOID;
      USBH_HID_Handle( 0 );       /* Fire up enginw */

      return( USBH_HID_Handle );
  } }

  return( 0 );
}

