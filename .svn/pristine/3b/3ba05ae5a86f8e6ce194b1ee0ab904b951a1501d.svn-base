/**
  ******************************************************************************
  * @file    usbh_cdc_core.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   This file is the CDC Layer Handlers for USB Host CDC class.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_cdc_core.h"

CDC_Requests                        CDC_ReqState;
CDC_Xfer_TypeDef                    CDC_TxParam;
CDC_Xfer_TypeDef                    CDC_RxParam;

__ALIGN_BEGIN CDC_Machine_TypeDef   CDC_Machine __ALIGN_END ;
__ALIGN_BEGIN byte               TxBuf [CDC_BUFFER_SIZE] __ALIGN_END ;
__ALIGN_BEGIN byte               RxBuf [CDC_BUFFER_SIZE] __ALIGN_END ;

CDC_Usercb_TypeDef                  UserCb;
byte                             RX_Enabled = 0;
/**
* @}
*/


/** @defgroup CDC_CORE_Private_FunctionPrototypes
* @{
*/
static void CDC_InitTxRxParam(void);
static void CDC_ReceiveData( CDC_Xfer_TypeDef *cdc_Data );

static void CDC_ProcessTransmission(  USBH_HOST * phost );
static void CDC_ProcessReception(     USBH_HOST * phost );
static USBH_Status CDC_InterfaceInit( USBH_HOST * phost );
void CDC_InterfaceDeInit  (           USBH_HOST * phost );
static USBH_Status CDC_Handle(        USBH_HOST * phost );
static USBH_Status CDC_ClassRequest(  USBH_HOST * phost );



/** @defgroup CDC_CORE_Private_Functions
* @{
*/

/**
  * @brief  CDC_InterfaceInit
  *         The function init the CDC class.
  * @param  hdev: Selected device property
  * @retval  USBH_Status :Response for USB CDC driver intialization
  */
static USBH_Status CDC_InterfaceInit (void *phost)
{ USBH_HOST *pphost = phost;
  USBH_Status status = USBH_OK ;

  /* Communication Interface */
  if((pphost->device_prop.Itf_Desc[0].bInterfaceClass  == COMMUNICATION_DEVICE_CLASS_CODE)&& \
      (pphost->device_prop.Itf_Desc[0].bInterfaceSubClass  == ABSTRACT_CONTROL_MODEL) && \
      (pphost->device_prop.Itf_Desc[0].bInterfaceProtocol == COMMON_AT_COMMAND))
  { /*Collect the notification endpoint address and length*/
    CDC_Machine.CDC_CommItf.ep_addr = pphost->device_prop.Ep_Desc[0][0].bEndpointAddress;
    CDC_Machine.CDC_CommItf.length  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;

    if(pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80)
    { CDC_Machine.CDC_CommItf.notificationEp =\
                                              (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
    }
    /*Allocate the length for host channel number in*/
    CDC_Machine.CDC_CommItf.hc_num_in = USBH_Alloc_Channel( CDC_Machine.CDC_CommItf.notificationEp );

    /* Open channel for IN endpoint */
    USBH_Open_Channel  (CDC_Machine.CDC_CommItf.hc_num_in,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_INTR,
                        CDC_Machine.CDC_CommItf.length);
  }
  else
  { pphost->usr_cb->DeviceNotSupported();
  }


  /* Data Interface */
  if (( pphost->device_prop.Itf_Desc[1].bInterfaceClass    == DATA_INTERFACE_CLASS_CODE)
      &&  ( pphost->device_prop.Itf_Desc[1].bInterfaceSubClass == RESERVED)
      &&  ( pphost->device_prop.Itf_Desc[1].bInterfaceProtocol == NO_CLASS_SPECIFIC_PROTOCOL_CODE))
  { CDC_Machine.CDC_DataItf.ep_addr = pphost->device_prop.Ep_Desc[1][0].bEndpointAddress; /*Collect the class specific endpoint address and length*/
    CDC_Machine.CDC_DataItf.length  = pphost->device_prop.Ep_Desc[1][0].wMaxPacketSize;

    if(pphost->device_prop.Ep_Desc[1][0].bEndpointAddress & 0x80)
    { CDC_Machine.CDC_DataItf.cdcInEp = (pphost->device_prop.Ep_Desc[1][0].bEndpointAddress);
    }
    else
    { CDC_Machine.CDC_DataItf.cdcOutEp = (pphost->device_prop.Ep_Desc[1][0].bEndpointAddress);
    }

    if(pphost->device_prop.Ep_Desc[1][1].bEndpointAddress & 0x80)
    { CDC_Machine.CDC_DataItf.cdcInEp = (pphost->device_prop.Ep_Desc[1][1].bEndpointAddress);
    }
    else
    { CDC_Machine.CDC_DataItf.cdcOutEp = (pphost->device_prop.Ep_Desc[1][1].bEndpointAddress);
    }

    /*Allocate the length for host channel number out*/
    CDC_Machine.CDC_DataItf.hc_num_out = USBH_Alloc_Channel( CDC_Machine.CDC_DataItf.cdcOutEp);
    /*Allocate the length for host channel number in*/
    CDC_Machine.CDC_DataItf.hc_num_in = USBH_Alloc_Channel( CDC_Machine.CDC_DataItf.cdcInEp);

    /* Open channel for OUT endpoint */
    USBH_Open_Channel  (CDC_Machine.CDC_DataItf.hc_num_out,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        CDC_Machine.CDC_DataItf.length);
    /* Open channel for IN endpoint */
    USBH_Open_Channel  (CDC_Machine.CDC_DataItf.hc_num_in,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        CDC_Machine.CDC_DataItf.length);

    /*Initilise the Tx/Rx Params*/
    CDC_InitTxRxParam();


    /*Initialize the class specific request with "GET_LINE_CODING"*/
    CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;
  }
  else
  { pphost->usr_cb->DeviceNotSupported();
  }
  return status;

}



/**
* @brief  CDC_InterfaceDeInit
*         The function DeInit the Host Channels used for the CDC class.
* @param  hdev: Selected device property
* @retval None
*/
void CDC_InterfaceDeInit (void *phost)
{ if ( CDC_Machine.CDC_CommItf.hc_num_in)
  { USB_OTG_HC_Halt( CDC_Machine.CDC_CommItf.hc_num_in);
    USBH_Free_Channel( CDC_Machine.CDC_CommItf.hc_num_in);
    CDC_Machine.CDC_CommItf.hc_num_in = 0;     /* Reset the Channel as Free */
  }

  if ( CDC_Machine.CDC_DataItf.hc_num_out)
  { USB_OTG_HC_Halt( CDC_Machine.CDC_DataItf.hc_num_out);
    USBH_Free_Channel( CDC_Machine.CDC_DataItf.hc_num_out);
    CDC_Machine.CDC_DataItf.hc_num_out = 0;     /* Reset the Channel as Free */
  }

  if ( CDC_Machine.CDC_DataItf.hc_num_in)
  { USB_OTG_HC_Halt( CDC_Machine.CDC_DataItf.hc_num_in);
    USBH_Free_Channel(CDC_Machine.CDC_DataItf.hc_num_in);
    CDC_Machine.CDC_DataItf.hc_num_in = 0;     /* Reset the Channel as Free */
  }
}

/**
  * @brief  CDC_ClassRequest
  *         The function is responsible for handling CDC Class requests
  *         for CDC class.
  * @param  hdev: Selected device property
  * @retval  USBH_Status :Response for USB Set Protocol request
  */
static USBH_Status CDC_ClassRequest(void *phost)
{ USBH_HOST *pphost = phost;

  USBH_Status status         = USBH_BUSY;
  USBH_Status ClassReqStatus = USBH_BUSY;

  switch(CDC_ReqState)
  {

    case CDC_GET_LINE_CODING_RQUEST:
      /*Issue the get line coding request*/
      ClassReqStatus = CDC_GETLineCoding( phost);
      if( ClassReqStatus == USBH_OK )
      { /*Change the state */
        CDC_ReqState = CDC_SET_CONTROL_LINE_STATE_REQUEST;
      }
      break;

    case CDC_SET_LINE_CODING_RQUEST:

      /*Issue the set line coding request*/
      ClassReqStatus = CDC_SETLineCoding( phost );
      if( ClassReqStatus == USBH_OK )
      {

        CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;  /*Change the state */
      }
      if(ClassReqStatus == USBH_NOT_SUPPORTED )
      {

        CDC_ReqState = CDC_ERROR_STATE;       /* a Clear Feature should be issued here */
      }
      break;

    case CDC_SET_CONTROL_LINE_STATE_REQUEST: /*Issue the set control line coding */

      ClassReqStatus = CDC_SETControlLineState(phost);
      if ( ClassReqStatus == USBH_OK )
      { CDC_ReqState = CDC_SET_CONTROL_LINE_STATE_REQUEST;      /*Change the state */
        CDC_RxParam.CDCState = CDC_IDLE;       /*Also set the state of receive CDCRxParam to IDLE*/

        status = USBH_OK; /*This return from class specific routinues request*/
      }
      break;

    case CDC_ERROR_STATE:

      ClassReqStatus = USBH_ClrFeature( 0x00, pphost->Control.hc_num_out);

      if(ClassReqStatus == USBH_OK )
      { /*Change the state to waiting*/
        CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;
      }
      break;
  }

  return status;
}


/**
  * @brief  CDC_Handle
  *         The function is for managing state machine for CDC data transfers
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */
static USBH_Status CDC_Handle( void   * phost )
{ USBH_Status status = USBH_OK;
  USBH_HOST *pphost = phost;

  pphost->usr_cb->UserApplication();   /* Call Application process */
  CDC_ProcessTransmission( pphost);   /*Handle the transmission */
  CDC_ProcessReception( pphost);   /*Always send in packet to device*/

  return status;
}


/**
  * @brief  The function is responsible for sending data to the device
  * @retval None
  */
void CDC_ProcessTransmission( USBH_HOST *phost )
{ static uint32_t len ;
  URB_STATE URB_StatusTx = URB_IDLE;

  URB_StatusTx= HCD_GetURB_State( CDC_Machine.CDC_DataItf.hc_num_out);

  switch(CDC_TxParam.CDCState)
  { case CDC_IDLE:
      break;

    case CDC_SEND_DATA:

      if(( URB_StatusTx == URB_DONE ) || (URB_StatusTx == URB_IDLE))
      { /*Check the data length is more then the CDC_Machine.CDC_DataItf.CDC_DataItf.length */
        if(CDC_TxParam.DataLength > CDC_Machine.CDC_DataItf.length)
        {

          len = CDC_Machine.CDC_DataItf.length ;
          /*Send the data */
          USBH_BulkSendData (CDC_TxParam.pRxTxBuff,
                             len,
                             CDC_Machine.CDC_DataItf.hc_num_out);
        }
        else
        { len = CDC_TxParam.DataLength ;
          /*Send the remaining data */
          USBH_BulkSendData (CDC_TxParam.pRxTxBuff,
                             len,
                             CDC_Machine.CDC_DataItf.hc_num_out);

        }
        CDC_TxParam.CDCState = CDC_DATA_SENT;

      }

      break;

    case CDC_DATA_SENT:
      /*Check the status done for transmssion*/
      if(URB_StatusTx == URB_DONE )
      { /*Point to next chunc of data*/
        CDC_TxParam.pRxTxBuff += len ;

        /*decrease the data length*/
        CDC_TxParam.DataLength -= len;

        if(CDC_TxParam.DataLength == 0)
        { CDC_TxParam.CDCState = CDC_IDLE;
        }
        else
        { CDC_TxParam.CDCState = CDC_SEND_DATA;
        }
      }
      else
        if( URB_StatusTx == URB_NOTREADY )
        { /*Send the same data */
          USBH_BulkSendData ((CDC_TxParam.pRxTxBuff),
                             len,
                             CDC_Machine.CDC_DataItf.hc_num_out);
        }

      break;

    case CDC_READ_DATA:      break;
    case CDC_BUSY:      break;
    case CDC_GET_DATA:      break;
    case CDC_POLL:      break;
    case CDC_CTRL_STATE:      break;

  }
}
/**
  * @brief  This function responsible for reception of data from the device
  * @retval None
  */
static void CDC_ProcessReception( USBH_HOST * phost )
{

  if(RX_Enabled == 1)
  { URB_STATE URB_StatusRx =   HCD_GetURB_State( CDC_Machine.CDC_DataItf.hc_num_in);

    switch(CDC_RxParam.CDCState)
    {

      case CDC_IDLE:


        /*check the received length lesser then the remaining space available in the
        buffer */
        if(CDC_RxParam.DataLength < (CDC_RxParam.BufferLen - CDC_Machine.CDC_DataItf.length))
        { /*Receive the data */
          USBH_BulkReceiveData(CDC_RxParam.pFillBuff,
                               CDC_Machine.CDC_DataItf.length,
                               CDC_Machine.CDC_DataItf.hc_num_in);

          /*change the cdc state to USBH_CDC_GET_DATA*/
          CDC_RxParam.CDCState = CDC_GET_DATA;
        }
        break;

      case CDC_GET_DATA:
        /*Check the last state of the device is URB_DONE */
        if(URB_StatusRx == URB_DONE)
        { /* Move the pointer as well as datalength */
          CDC_RxParam.DataLength += pdev->host.hc[CDC_Machine.CDC_DataItf.hc_num_in].xfer_count ;
          CDC_RxParam.pFillBuff += pdev->host.hc[CDC_Machine.CDC_DataItf.hc_num_in].xfer_count ;


          /* Process the recived data */
          CDC_ReceiveData(&CDC_RxParam);

          /*change the state od the CDC state*/
          CDC_RxParam.CDCState = CDC_IDLE;

        }
        break;

      case CDC_READ_DATA:

        break;

      case CDC_BUSY:

        break;

      case CDC_SEND_DATA:

        break;

      case CDC_DATA_SENT:

        break;

      case CDC_POLL:

        break;

      case CDC_CTRL_STATE:

        break;
    }
  }
}

/**
  * @brief  Initialize the transmit and receive buffer and its parameter
  * @param  None
  * @retval None
  */
static void CDC_InitTxRxParam(void)
{ /*Initialize the Transmit buffer and its parameter*/
  CDC_TxParam.CDCState = CDC_IDLE;
  CDC_TxParam.DataLength = 0;
  CDC_TxParam.pRxTxBuff = TxBuf;

  /*Initialize the Receive buffer and its parameter*/
  CDC_RxParam.CDCState = CDC_IDLE;
  CDC_RxParam.DataLength = 0;
  CDC_RxParam.pFillBuff = RxBuf;
  CDC_RxParam.pEmptyBuff = RxBuf;
  CDC_RxParam.BufferLen = sizeof(RxBuf);
}

/**
  * @brief  This is a call back function from cdc core layer to redirect the
  *         received data on the user out put system
  * @param  cdc_Data: type of USBH_CDCXfer_TypeDef
  * @retval None
  */
static void CDC_ReceiveData(CDC_Xfer_TypeDef *cdc_Data)
{ byte *ptr;

  if(cdc_Data->pEmptyBuff < cdc_Data->pFillBuff)
  { ptr = cdc_Data->pFillBuff;
    *ptr = 0x00;

    /* redirect the received data on the user out put system */
    UserCb.Receive(cdc_Data->pEmptyBuff);

    cdc_Data->pFillBuff  = cdc_Data->pEmptyBuff ;
    cdc_Data->DataLength = 0;    /*Reset the data length to zero*/
  }
}

/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file
  * @retval the filestate will be returned
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_SendData(byte *data, uint16_t length)
{

  if(CDC_TxParam.CDCState == CDC_IDLE)
  { CDC_TxParam.pRxTxBuff = data;
    CDC_TxParam.DataLength = length;
    CDC_TxParam.CDCState = CDC_SEND_DATA;
  }
}

/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file
  * @retval the filestate will be returned
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_StartReception()
{ RX_Enabled = 1;
}

/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file
  * @retval the filestate will be returned
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_StopReception( )
{ RX_Enabled = 0;
  USB_OTG_HC_Halt(   CDC_Machine.CDC_DataItf.hc_num_in);
  USBH_Free_Channel( CDC_Machine.CDC_DataItf.hc_num_in);
}

USBH_Class_cb_TypeDef CDC_cb=
{ CDC_InterfaceInit
  , CDC_InterfaceDeInit
  , CDC_ClassRequest
  , CDC_Handle
};


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
