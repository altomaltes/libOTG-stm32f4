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
 * @file usbh_ioreq.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_core.h"

enum
{ HC_PID_DATA0= 0
, HC_PID_DATA2= 1
, HC_PID_DATA1= 2
, HC_PID_SETUP= 3
} HOST_DEFINES;



/**
  * @brief  USBH_CtlReq
  *         USBH_CtlReq sends a control request and provide the status after
  *            completion of the request
  * @param  req:    Setup Request Structure
  * @param  buff:   data buffer address to store the response
  * @param  length: length of the response
  * @retval Status
  */
schar USBH_CtlReq( byte * buff
                       , word  length )
{ USB_Host.Control.buff  = buff;
  USB_Host.Control.length= length;
  USB_Host.Control.state = CTRL_SETUP;

  HCDsubmitRequest( USB_Host.Control.hcNumOut
                  , HC_PID_SETUP
                  , USB_Host.Control.setup.d8, USBH_SETUP_PKT_SIZE );

  USB_Host.Control.state= CTRL_SETUP_WAIT;

  return( 0 );
}

/**
  * @brief  USBH_CtlSendData
  *         Sends a data Packet to the Device
  * @param  buff: Buffer pointer from which the Data will be sent to Device
  * @param  length: Length of the data to be sent
  * @param  hcNum: Host channel Number
  * @retval Status
  */
schar USBH_CtlSendData( byte   hcNum
                            , byte * buff
                            , word   length )
{ if ( length == 0 ) /* For Status OUT stage, Length==0, Status Out PID = 1 */
  { USB_HOST.hc[ hcNum ].toggleOut = 1;
  }

  return( HCDsubmitRequest( hcNum
                          , USB_HOST.hc[ hcNum ].toggleOut ? HC_PID_DATA1 /* Put the PID 1 */
                                                           : HC_PID_DATA0 /* Put the PID 0 */
                          , buff, length ));
}


/**
  * @brief  USBH_CtlReceiveData
  *         Receives the Device Response to the Setup Packet
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_CtlReceiveData( byte   hcNum
                               , byte * buff
                               , word   length )
{ return( HCDsubmitRequest( hcNum, HC_PID_DATA1, buff, length ));
}


/**
  * @brief  USBH_BulkSendData
  *         Sends the Bulk Packet to the device
  * @param  buff: Buffer pointer from which the Data will be sent to Device
  * @param  length: Length of the data to be sent
  * @param  hcNum: Host channel Number
  * @retval Status
  */
schar USBH_BulkSendData( byte * buff
                             , word len
                             , byte hcNum )
{ return( HCDsubmitRequest( hcNum, USB_HOST.hc[ hcNum ].toggleOut ? HC_PID_DATA1 /* Put the PID 1 */
                                                                  : HC_PID_DATA0 /* Put the PID 0 */
                          , buff, len ));
}


/**
  * @brief  USBH_BulkReceiveData
  *         Receives IN bulk packet from device
  * @param  buff: Buffer pointer in which the received data packet to be copied
  * @param  length: Length of the data to be received
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_BulkReceiveData( byte *buff
                                , word len
                                , byte hcNum )
{ return( HCDsubmitRequest( hcNum, USB_HOST.hc[ hcNum ].toggleIn ? HC_PID_DATA1 /* Put the PID 1 */
                                                                 : HC_PID_DATA0 /* Put the PID 0 */
                          , buff, len ));
}


/**
  * @brief  USBH_InterruptReceiveData
  *         Receives the Device Response to the Interrupt IN token
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_InterruptReceiveData( byte * buff
                                     , byte   len
                                     , byte   hcNum )
{ HCDsubmitRequest( hcNum, USB_HOST.hc[ hcNum ].toggleIn ? HC_PID_DATA1 /* Put the PID 1 */
                                                         : HC_PID_DATA0 /* Put the PID 0 */
                  , buff, len );
  USB_HOST.hc[ hcNum ].toggleIn ^= 1;                                   /* toggle DATA PID */
  return( 0 );
}


/**
  * @brief  USBH_InterruptSendData
  *         Sends the data on Interrupt OUT Endpoint
  * @param  buff: Buffer pointer from where the data needs to be copied
  * @param  length: Length of the data to be sent
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_InterruptSendData( byte * buff   // !!!! toggleOut ???
                                  , byte len
                                  , byte hcNum )
{ HCDsubmitRequest( hcNum, USB_HOST.hc[ hcNum ].toggleIn ? HC_PID_DATA1 /* Put the PID 1 */
                                                         : HC_PID_DATA0 /* Put the PID 0 */
                   , buff, len );
  USB_HOST.hc[ hcNum ].toggleIn ^= 1;   /* toggle DATA PID */
  return( 0 );
}



/**
  * @brief  USBH_IsocReceiveData
  *         Receives the Device Response to the Isochronous IN token
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_IsocReceiveData( byte * buff
                                , dword length
                                , byte hcNum )
{ return( HCDsubmitRequest( hcNum, HC_PID_DATA0, buff, length ));
}


/**
  * @brief  USBH_IsocSendData
  *         Sends the data on Isochronous OUT Endpoint
  * @param  buff: Buffer pointer from where the data needs to be copied
  * @param  length: Length of the data to be sent
  * @param  hcNum: Host channel Number
  * @retval Status.
  */
schar USBH_IsocSendData( byte * buff
                             , dword length
                             , byte hcNum )
{ return( HCDsubmitRequest( hcNum, HC_PID_DATA0, buff, length ));
}


/**
 * @brief  USBDreadPacket (JACS)
 *
 * @retval status
 */
short USBHreadPacket( byte chNum, word size )
{ if ( size )
  { USB_OTG_ReadPacket( USB_HOST.hc[ chNum ].xferBuff
                      , size );
    USB_HOST.hc[ chNum ].xferBuff += size;         /* manage multiple Xfer */
    USB_HOST.hc[ chNum ].xferCount+= size;
    USB_HOST.XferCnt[ chNum ]= USB_HOST.hc[ chNum ].xferCount;
  }

  return( size );
}


/**
 * @brief  USBDreadPacket (JACS)
 *
 * @retval status
 */
short USBHwritePacket( byte chNum, word size )
{ if ( size )
  { USB_OTG_WritePacket( USB_HOST.hc[ chNum ].xferBuff
                       , chNum
                       , size );
    USB_HOST.hc[ chNum ].xferBuff  += size;
    USB_HOST.hc[ chNum ].xferLen   -= size;
    USB_HOST.hc[ chNum ].xferCount += size;
  }

  return( USB_HOST.hc[ chNum ].xferLen );
}





