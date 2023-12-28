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

#include "usb_dcd.h"
#include "usbd_cdc_if_template.h"

/**
  * @brief  TEMPLATE_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static schar TEMPLATE_Init(void)
{ return( 0 );
}

/**
  * @brief  TEMPLATE_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static schar TEMPLATE_DeInit(void)
{ return( 0 );
}


/**
  * @brief  TEMPLATE_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static schar TEMPLATE_Ctrl( byte Cmd, byte * buf, byte Len )
{ //usbCdcNotification * msg= (usbCdcNotification *)buf;

  if ( !buf )
  { return( 0 );
  }

  switch( Cmd )
  { case USB_CDC_SEND_ENCAPSULATED_CMD :  /** 0x00 Used to issue a command in the format of the supported control protocol of the Communication Class interface.*/
     Len++;
    break;

    case USB_CDC_GET_ENCAPSULATED_RESP :  /** 0x01 Used to request a response in the format of the supported control protocol of the Communication Class interface.*/
     Len++;
    break;

    case USB_CDC_SET_COMM_FEATURE      :  /** 0x02 Controls the settings for a particular communication feature  of a particular target. */
     Len++;
    break;

    case USB_CDC_GET_COMM_FEATURE      :  /** 0x03 Returns the current settings for the communication feature as selected.*/
     Len++;
    break;

    case USB_CDC_CLEAR_COMM_FEATURE    :  /** 0x04 Controls the settings for a particular communication feature of a particular target, setting the selected feature to its default state.*/
     Len++;
    break;

    case USB_CDC_SET_LINE_CODING       :  /** 0x20 Allows the host to specify typical asynchronous line-character  formatting properties.*/
    {//usbCdcLineCoding * config=(usbCdcLineCoding *)buf;

//      configUART( config->dwDTERate    /** Data terminal rate, in bits per second.*/
  //              , config->bStopBits    /** Stop bits.*/
    //            , config->bParityType  /** Parity.*/
      //          , config->bDataBits ); /** Data bits (5,6,7,8 or 16).*/
    }
   return( 0 );

    case USB_CDC_GET_LINE_CODING       :  /** 0x21 Allows the host to find out the currently configured line coding.*/
    { usbCdcLineCoding * config=(usbCdcLineCoding *)buf;

      config->dwDTERate= 4800;                 /** Data terminal rate, in bits per second.*/
      config->bStopBits= USB_CDC_1_STOP_BITS;  /** Stop bits.*/
      config->bParityType= USB_CDC_NO_PARITY;  /** Parity.*/
      config->bDataBits= 8;                    /** Data bits (5,6,7,8 or 16).*/
    }
    return( 0 );

    case USB_CDC_SET_CONTROL_LINE_STATE:  /** 0x22 Generates RS-232/V.24 style control signals.*/
     Len++;
    return( 0 );

    case USB_CDC_SEND_BREAK            :  /** 0x23 Sends special carrier modulation that */
     Len++;
    break;

    default:
    break;
  }

  return( 0 );
}

/**
 * @brief  TEMPLATE_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 *
 * @param  epNum: Received endpoint
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static schar TEMPLATE_DataTx( byte epNum )
{ return( 0 );
}

byte buff[ 64 ];

/**
 * @brief  TEMPLATE_DataRx
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
static schar TEMPLATE_DataRx( byte epNum )
{ word len= USBD_GetRxCount( epNum ); /* Get the received data buffer and update the counter */

  if ( len )
  { buff[ 1 ]= ' ';
    buff[ 2 ]= 'H';
    buff[ 3 ]= 'e';
    buff[ 5 ]= 'l';
    buff[ 6 ]= 'l';
    buff[ 7 ]= 'o';
    buff[ 8 ]= 10;
    buff[ 9 ]= 13;

    USBDepTx( epNum | 0x80 /* Prepare the available data buffer to be sent on IN endpoint */
            , buff
            , 10 );
  }

  USBDepPrepareRx( epNum
                  , buff
                  , sizeof( buff ));

  return( 0 );
}

static schar TEMPLATE_DataCt( byte epNum )
{ word len= USBD_GetRxCount( epNum ); /* Get the received data buffer and update the counter */

  if ( len )
  {
  }


  USBDepPrepareRx( epNum
                  , buff
                  , sizeof( buff ));

  return( 0 );
}


/**
  * @brief  TEMPLATE_DataRx
  */
CDC_IF_Prop_TypeDef APP_FOPS=
{ TEMPLATE_Init
, TEMPLATE_DeInit
, TEMPLATE_Ctrl
, TEMPLATE_DataTx
, TEMPLATE_DataRx
, TEMPLATE_DataCt
};
