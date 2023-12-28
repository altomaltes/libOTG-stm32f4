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

#include "usbh_hid_core.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

byte report[ 8 ];

ALIGN_THIS( usbHidDescriptor HID_Desc );
//ALIGN_THIS( HID_Report_TypeDef   HID_Report  );
ALIGN_THIS( HID_Machine_TypeDef  HID_Machine );

volatile byte start_toggle = 0;

typedef struct
{ byte report_id;
  byte usage;
  byte usage_page[ 32 ];

} hidReportInfoRec;


/**
 * @brief  hidParseReportDescriptor
 * @param  None
 * @retval None
 */
byte hidParseReportDescriptor( hidReportInfoRec * report_info_arr
                             , byte arr_count
                             , byte const * desc_report, word desc_len )
{

  union PACKED // Report Item 6.2.2.2 USB HID 1.11
  { byte byte8;

    struct PACKED
    { byte size : 2;
      byte type : 2;
      byte tag  : 4;
    };
  } header;

//  tu_memclr( report_info_arr, arr_count*sizeof(tuh_hid_report_info_t));

  byte report_num = 0;
  hidReportInfoRec * info= report_info_arr;

  // current parsed report count & size from descriptor
//  byte ri_report_count = 0;
//  byte ri_report_size = 0;

  byte ri_collection_depth = 0;

  while( desc_len && report_num < arr_count)
  {
    header.byte8 = *desc_report++;
    desc_len--;

    byte const tag  = header.tag;
    byte const type = header.type;
    byte const size = header.size;

    byte const data8 = desc_report[ 0 ];

 /*   TU_LOG(3, "tag = %d, type = %d, size = %d, data = ", tag, type, size);
   for( word i = 0
       ;      i < size
       ;      i++ )
    { TU_LOG(3, "%02X ", desc_report[i]);
    }

    TU_LOG(3, "\r\n");
*/

    switch( type )
    { case HID_RI_TYPE_MAIN: switch( tag )
      { case HID_RI_MAIN_INPUT:   break;
        case HID_RI_MAIN_OUTPUT:  break;
        case HID_RI_MAIN_FEATURE: break;

        case HID_RI_MAIN_COLLECTION:
          ri_collection_depth++;
        break;

        case HID_RI_MAIN_ECOLLEC:
          ri_collection_depth--;

          if (ri_collection_depth == 0)
          { info++;
            report_num++;
          }
          break;

        default: break;
      }
      break;

      case HID_RI_TYPE_GLOBAL: switch(tag)
      { case HID_RI_GLOBAL_USAGE_PAGE:
            // only take in account the "usage page" before REPORT ID
          if ( ri_collection_depth == 0 )
          { memcpy( &info->usage_page, desc_report, size);
          }
        break;

        case HID_RI_GLOBAL_LOGICAL_MINIMUM   : break;
        case HID_RI_GLOBAL_LOGICAL_MAXIMUM   : break;
        case HID_RI_GLOBAL_PHYSICAL_MINIMUM  : break;
        case HID_RI_GLOBAL_PHYSICAL_MAXIMUM  : break;

        case HID_RI_GLOBAL_REPORT_ID:
          info->report_id= data8;
        break;

        case HID_RI_GLOBAL_REPORT_SIZE:
//            ri_report_size = data8;
        break;

        case HID_RI_GLOBAL_REPORT_COUNT:
//            ri_report_count = data8;
        break;

        case HID_RI_GLOBAL_UNIT_EXPONENT : break;
        case HID_RI_GLOBAL_UNIT          : break;
        case HID_RI_GLOBAL_PUSH          : break;
        case HID_RI_GLOBAL_POP           : break;

        default: break;
      }
      break;

      case HID_RI_TYPE_LOCAL: switch(tag)
      { case HID_RI_LOCAL_USAGE:
          // only take in account the "usage" before starting REPORT ID
          if ( ri_collection_depth == 0 )
          { info->usage = data8;
          }
        break;

        case HID_RI_LOCAL_USAGE_MINIMUM: break;
        case HID_RI_LOCAL_USAGE_MAXIMUM: break;
        case HID_RI_LOCAL_DESIGR_INDEX : break;
        case HID_RI_LOCAL_DESIGR_MIN   : break;
        case HID_RI_LOCAL_DESIGR_MAX   : break;
        case HID_RI_LOCAL_STR_INDEX    : break;
        case HID_RI_LOCAL_STR_MINIMUM  : break;
        case HID_RI_LOCAL_STR_MAXIMUM  : break;
        case HID_RI_LOCAL_DELIMITER    : break;
        default: break;
      }
    break;

    default: break;   // error
  }

  desc_report += size;
  desc_len    -= size;
  }
/*
  for( byte i = 0
     ;      i < report_num
     ; i++ )
  { info = report_info_arr+i;
    TU_LOG_DRV("%u: id = %u, usage_page = %u, usage = %u\r\n", i, info->report_id, info->usage_page, info->usage);
  }
*/
  return( report_num );
}

/**
 * @brief  USBH_MSC_HandleBOTXfer
 *         This function manages the different states of BOT transfer and
 *         updates the status to upper layer.
 * @param  None
 * @retval None
 */
void USBH_HID_HandleBOTXferOut( byte ep )
{
}

void USBH_HID_HandleBOTXferIn( schar ep )
{ if ( ep < 0 )
  { USBH_InterruptReceiveData( report  /* Fire the self machine */
                             , 8
                             , -ep );
  }
  else
  { HID_Machine.cb( report );
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
 * @brief  USBH_ParseHIDDesc
 *         This function Parse the HID descriptor
 * @param  buf: Buffer where the source descriptor is available
 * @retval None
 */
static void  USBH_ParseHIDDesc( usbHidDescriptor * desc )
{ byte * buf= (byte*)USBHgetBuffer();

  desc->bLength           = buf[ 0 ];
  desc->bType             = buf[ 1 ];
  desc->bcdHID            = LE16  (buf + 2);
  desc->bCountryCode      = buf[ 4 ];
  desc->bNumDescriptors   = buf[ 5 ];
  desc->bType0            = buf[ 6 ];
  desc->wDescriptorLength0= LE16  (buf + 7);
}


/**
 * @brief  USBH_HID_Handle
 *         The function is responsible for handling HID Class requests
 *         for HID class.
 * @retval  schar :Response for USB Set Protocol request
 */
static schar USBH_HID_Handle( byte ep )
{ switch ( HID_Machine.ctl_state )   /* Switch HID state machine */
  { case HID_REQ_VOID:
    case HID_REQ_GET_HID_DESC:
      USBH_GetDescriptor( USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD
                        , HID_DTYPE_HID << 8
                        , sizeof( usbHidDescriptor ));
      HID_Machine.ctl_state= HID_REQ_GET_REPORT_DESC;
  break;

  case HID_REQ_GET_REPORT_DESC:     /* Get Report Desc */
    USBH_ParseHIDDesc( &HID_Desc );
    USBH_GetDescriptor( USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD
                      , HID_DTYPE_REPORT << 8
                      , HID_Desc.wDescriptorLength0 );
    HID_Machine.ctl_state= HID_REQ_SET_IDLE;
  break;

    case HID_REQ_SET_IDLE:     /* set Idle */
    {// hidReportInfoRec info[ 2 ];
      //hidParseReportDescriptor( info, 2
        //                      , (byte*)USBHgetBuffer(), HID_Desc.wDescriptorLength0 );

      USBHsetInterface( HID_SETIDLE, 0,  0 );  // (duration << 8 ) | reportId
      HID_Machine.ctl_state= HID_REQ_SET_PROTOCOL;
    }
    break;

    case HID_REQ_SET_PROTOCOL:     /* set protocol */
      USBHsetInterface( HID_SETPROTOCOL
                      , 0
                      , 1 ); /* 0 Boot Protocol 1 Report Protocol */
      HID_Machine.ctl_state= HID_REQ_CLEAR;
    break;

    case HID_REQ_CLEAR:                       /* set protocol */
      USBH_ClrFeature( HID_Machine.ep_addr     /* Issue Clear Feature on interrupt IN endpoint */
                     , HID_Machine.hcNumIn );
    { //static byte leds= 0x03;

  //    USBHsendInterface( HID_SETREPORT
    //                   , 0  // word index
      //                 , 20 // USB_HID_REPORT_OUT << 8  // word value
        //               , &leds, 1 );
    }
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
  byte num =0;

  HID_Machine.cb        = protocolHandler;
  USBHepDescRec * epList= dev->Ep_Desc;

  if ( dev->Itf_Desc->bInterfaceSubClass == USB_HID_SUBCLASS_BOOT )
  { HID_Machine.state    = HID_IDLE;
    HID_Machine.ctl_state= HID_REQ_IDLE;
    HID_Machine.ep_addr  = epList[ 0 ].bEndpointAddress;
    HID_Machine.length   = epList[ 0 ].wMaxPacketSize;
    HID_Machine.poll     = epList[ 0 ].bInterval ;

    if ( HID_Machine.poll  < HID_MIN_POLL )
    { HID_Machine.poll = HID_MIN_POLL;
    }


/* Check fo available number of endpoints */
/* Find the number of EPs in the Interface Descriptor */
/* Choose the lower number in order not to overrun the buffer allocated */
    maxEP= ( ( dev->Itf_Desc->bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS)
         ?  dev->Itf_Desc->bNumEndpoints
         :  USBH_MAX_NUM_ENDPOINTS );


/* Decode endpoint IN and OUT address from interface descriptor */

    for ( num= 0
        ; num < maxEP
        ; num++ )
    { if ( epList[ num ].bEndpointAddress & 0x80 )  // IN ep
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
  }

  return( 0 );
}

