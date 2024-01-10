/**
  ******************************************************************************
  * @file    usbh_cdc_funct.c
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
#include "usbh_cdc_funct.h"


/** @defgroup CDC_CORE_Private_Variables
* @{
*/
CDC_InterfaceDesc_Typedef         CDC_Desc;
CDC_LineCodingTypeDef             CDC_GetLineCode;
CDC_LineCodingTypeDef             CDC_SetLineCode;

extern CDC_Requests               CDC_ReqState;
/**
  * @brief  This request allows the host to find out the currently
  *         configured line coding.
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */
USBH_Status CDC_GETLineCoding( USBH_HOST * phost)
{
  phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

  phost->Control.setup.b.bRequest = CDC_GET_LINE_CODING;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bMasterInterface; /*At to be checked*/
  phost->Control.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;

  return USBH_CtlReq( CDC_GetLineCode.Array, LINE_CODING_STRUCTURE_SIZE);
}


/**
  * @brief  This request allows the host to specify typical asynchronous
  *         line-character formatting properties
  *         This request applies to asynchronous byte stream data class interfaces
  *         and endpoints
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */
USBH_Status CDC_SETLineCoding( USBH_HOST * phost )
{ phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |  USB_REQ_RECIPIENT_INTERFACE;

  phost->Control.setup.b.bRequest = CDC_SET_LINE_CODING;
  phost->Control.setup.b.wValue.w = 0;
  phost->Control.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bMasterInterface;
  phost->Control.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;

  return USBH_CtlReq( CDC_SetLineCode.Array , LINE_CODING_STRUCTURE_SIZE );
}

/**
  * @brief  This request generates RS-232/V.24 style control signals.
  * @retval USBH_Status : USB ctl xfer status
  */
USBH_Status CDC_SETControlLineState( USBH_HOST *phost)
{ phost->Control.setup.b.bmRequestType= USB_H2D | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_INTERFACE;

  phost->Control.setup.b.bRequest = CDC_SET_CONTROL_LINE_STATE;
  /*Control Signal Bitmap Values for SetControlLineState*/
  phost->Control.setup.b.wValue.w= CDC_DEACTIVATE_CARRIER_SIGNAL_RTS | CDC_DEACTIVATE_SIGNAL_DTR;
  phost->Control.setup.b.wIndex.w= CDC_Desc.CDC_UnionFuncDesc.bMasterInterface;

  /*Length feild is zero*/
  phost->Control.setup.b.wLength.w = 0;

  return USBH_CtlReq(  0 , 0 );
}

/**
  * @brief  This function prepares the state before issuing the class specific commands
  * @param  None
  * @retval None
  */
void CDC_ChangeStateToIssueSetConfig( USBH_HOST *phost)
{ phost->gStateBkp= phost->gState  ;
  phost->gState= HOST_CLASS_REQUEST;
  CDC_ReqState= CDC_SET_LINE_CODING_RQUEST;
}

/**
  * @brief  This function prepares the state before issuing the class specific commands
  * @param  None
  * @retval None
  */
void CDC_IssueGetConfig( USBH_HOST * phost)
{ phost->gStateBkp= phost->gState ;
  phost->gState= HOST_CLASS_REQUEST;
  CDC_ReqState= CDC_GET_LINE_CODING_RQUEST;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
