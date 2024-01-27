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
 * @file usbd_cdc_if_template.c
 *
 * original draft from MCD Application Team
 */

#include "usbd_cdc_core.h"
#include "usb_cdc.h"

/**
  * @brief  cdcCtrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static word cdcCtrl( byte ep, struct cdcHandleRec * hnd )
{ switch( hnd->cdcCmd )
  { case USB_CDC_SEND_ENCAPSULATED_CMD :  /** 0x00 Used to issue a command in the format of the supported control protocol of the Communication Class interface.*/
    break;

    case USB_CDC_GET_ENCAPSULATED_RESP :  /** 0x01 Used to request a response in the format of the supported control protocol of the Communication Class interface.*/
    break;

    case USB_CDC_SET_COMM_FEATURE      :  /** 0x02 Controls the settings for a particular communication feature  of a particular target. */
    break;

    case USB_CDC_GET_COMM_FEATURE      :  /** 0x03 Returns the current settings for the communication feature as selected.*/
    break;

    case USB_CDC_CLEAR_COMM_FEATURE    :  /** 0x04 Controls the settings for a particular communication feature of a particular target, setting the selected feature to its default state.*/
    break;

    case USB_CDC_SET_LINE_CODING       :  /** 0x20 Allows the host to specify typical asynchronous line-character  formatting properties.*/
    { volatile usbCdcLineCoding * config=(usbCdcLineCoding *)hnd->cmdBuff;

    //      configUART( config->dwDTERate    /** Data terminal rate, in bits per second.*/
    //              , config->bStopBits    /** Stop bits.*/
      //            , config->bParityType  /** Parity.*/
        //          , config->bDataBits ); /** Data bits (5,6,7,8 or 16).*/
    }
    return( 0 );

    case USB_CDC_GET_LINE_CODING       :  /** 0x21 Allows the host to find out the currently configured line coding.*/
    { usbCdcLineCoding * config=(usbCdcLineCoding *)hnd->cmdBuff;

      config->dwDTERate  = 4800;                /** Data terminal rate, in bits per second.*/
      config->bStopBits  = USB_CDC_1_STOP_BITS; /** Stop bits.*/
      config->bParityType= USB_CDC_NO_PARITY;   /** Parity.*/
      config->bDataBits  = 8;                   /** Data bits (5,6,7,8 or 16).*/
    }
    return( 0 );

    case USB_CDC_SET_CONTROL_LINE_STATE:  /** 0x22 Generates RS-232/V.24 style control signals.*/
    return( 0 );

    case USB_CDC_SEND_BREAK            :  /** 0x23 Sends special carrier modulation that */
    break;

    default:
    break;
  }

  return( 0 );
}

/**
 * @brief  cdcDataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 *
 * @param  epNum: Received endpoint
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static short cdcDataIn( dword epNum, struct cdcHandleRec *  hnd )
{ return( 0 );
}


/**
 * @brief  cdcDataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         untill exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
 */
static short cdcDataOut( dword epNum, struct cdcHandleRec * hnd )
{ word len= USBDgetRxCount( epNum ); /* Get the received data buffer and update the counter */

  if ( len )
  { hnd->dataBuf[ 1 ]= ' ';
    hnd->dataBuf[ 2 ]= 'H';
    hnd->dataBuf[ 3 ]= 'e';
    hnd->dataBuf[ 5 ]= 'l';
    hnd->dataBuf[ 6 ]= 'l';
    hnd->dataBuf[ 7 ]= 'o';
    hnd->dataBuf[ 8 ]= 10;
    hnd->dataBuf[ 9 ]= 13;

    USBDepTx( hnd->epIn     /* Prepare the available data buffer to be sent on IN endpoint */
            , hnd->dataBuf
            , 10 );
  }

  USBDepPrepareRx( epNum
                 , hnd->dataBuf
                 , sizeof( hnd->dataBuf ));

  return( 0 );
}

static short cdcDataCmd( dword epNum, struct cdcHandleRec * hnd )
{ return( 0 );
}


/**
  * @brief  cdcDataRx
  */
struct cdcHandleRec cdcHandle=
{ driver : &USBD_CDC_cb
, dataIn : cdcDataIn
, dataOut: cdcDataOut
, dataCmd: cdcDataCmd
, ctrl   : cdcCtrl
};

