/**
  ******************************************************************************
  * @file    usbd_audio.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the Audio core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                AUDIO Class  Description
  *          ===================================================================
 *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Standard AC Interface Descriptor management
  *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *             - 1 Audio Streaming Endpoint
  *             - 1 Audio Terminal Input (1 channel)
  *             - Audio Class-Specific AC Interfaces
  *             - Audio Class-Specific AS Interfaces
  *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
  *             - Audio Feature Unit (limited to Mute control)
  *             - Audio Synchronization type: Asynchronous
  *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
  *          The current audio class version supports the following audio features:
  *             - Pulse Coded Modulation (PCM) format
  *             - sampling rate: 48KHz.
  *             - Bit resolution: 16
  *             - Number of channels: 2
  *             - No volume control
  *             - Mute/Unmute capability
  *             - Asynchronous Endpoints
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"


#define AUDIO_SAMPLE_FREQ(frq)      (byte)(frq), (byte)((frq >> 8)), (byte)((frq >> 16))
#define AUDIO_PACKET_SZE(frq)          (byte)(((frq * 2 * 2)/1000) & 0xFF), \
                                       (byte)((((frq * 2 * 2)/1000) >> 8) & 0xFF)


/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN static byte USBD_AUDIO_CfgDesc[USB_AUDIO_CONFIG_DESC_SIZ] __ALIGN_END =
{
/* Configuration 1
 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  LOBYTE(USB_AUDIO_CONFIG_DESC_SIZ),    /* wTotalLength  109 bytes*/
  HIBYTE(USB_AUDIO_CONFIG_DESC_SIZ),
  0x02,                                 /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
  0xC0,                                 /* bmAttributes  BUS Powred*/
  0x32,                                 /* bMaxPower = 100 mA*/
  /* 09 byte*/

/* USB Speaker Standard interface descriptor
 */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  0x00,                                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x00,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

/* USB Speaker Class-specific AC Interface Descriptor
 */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
  0x00,          /* 1.00 */             /* bcdADC */
  0x01,
  0x27,                                 /* wTotalLength = 39*/
  0x00,
  0x01,                                 /* bInCollection */
  0x01,                                 /* baInterfaceNr */
  /* 09 byte*/

/* USB Speaker Input Terminal Descriptor
 */
  AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
  0x01,                                 /* bTerminalID */
  0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
  0x01,
  0x00,                                 /* bAssocTerminal */
  0x01,                                 /* bNrChannels */
  0x00,                                 /* wChannelConfig 0x0000  Mono */
  0x00,
  0x00,                                 /* iChannelNames */
  0x00,                                 /* iTerminal */
  /* 12 byte*/

/* USB Speaker Audio Feature Unit Descriptor
 */
  0x09,                                 /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
  AUDIO_OUT_STREAMING_CTRL,             /* bUnitID */
  0x01,                                 /* bSourceID */
  0x01,                                 /* bControlSize */
  AUDIO_CONTROL_MUTE,// |AUDIO_CONTROL_VOLUME, /* bmaControls(0) */
  0,                                    /* bmaControls(1) */
  0x00,                                 /* iTerminal */
  /* 09 byte*/

/* USB Speaker Output Terminal Descriptor
 */
  0x09,      /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
  0x03,                                 /* bTerminalID */
  0x01,                                 /* wTerminalType  0x0301*/
  0x03,
  0x00,                                 /* bAssocTerminal */
  0x02,                                 /* bSourceID */
  0x00,                                 /* iTerminal */
  /* 09 byte*/

/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith
 */
  /* Interface 1, Alternate Setting 0                                             */
  AUDIO_INTERFACE_DESC_SIZE,  /* bLength */
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType */
  0x01,                                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x00,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

/* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational
 */
  /* Interface 1, Alternate Setting 1                                           */
  AUDIO_INTERFACE_DESC_SIZE,  /* bLength */
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType */
  0x01,                                 /* bInterfaceNumber */
  0x01,                                 /* bAlternateSetting */
  0x01,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

/* USB Speaker Audio Streaming Interface Descriptor
 */
  AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
  0x01,                                 /* bTerminalLink */
  0x01,                                 /* bDelay */
  0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
  0x00,
  /* 07 byte*/

/* USB Speaker Audio Type III Format Interface Descriptor
 */
  0x0B,                                 /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
  AUDIO_FORMAT_TYPE_III,                /* bFormatType */
  0x02,                                 /* bNrChannels */
  0x02,                                 /* bSubFrameSize :  2 Bytes per frame (16bits) */
  16,                                   /* bBitResolution (16-bits per sample) */
  0x01,                                 /* bSamFreqType only one frequency supported */
  AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ),         /* Audio sampling frequency coded on 3 bytes */
  /* 11 byte*/

/* Endpoint 1 - Standard Descriptor
 */
  9,    /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  AUDIO_OUT_EP,                         /* bEndpointAddress 1 out endpoint*/
  USBD_EP_TYPE_ISOC,                    /* bmAttributes */
  AUDIO_PACKET_SZE(USBD_AUDIO_FREQ),    /* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  0x01,                                 /* bInterval */
  0x00,                                 /* bRefresh */
  0x00,                                 /* bSynchAddress */
  /* 09 byte*/

/* Endpoint - Audio Streaming Descriptor
 */
  AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
  AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
  AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
  0x00,                                 /* bmAttributes */
  0x00,                                 /* bLockDelayUnits */
  0x00,                                 /* wLockDelay */
  0x00,
  /* 07 byte*/
} ;

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static byte USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END=
{ USB_LEN_DEV_QUALIFIER_DESC
, USB_DESC_TYPE_DEVICE_QUALIFIER
, 0x00
, 0x02
, 0x00
, 0x00
, 0x00
, 0x40
, 0x01
, 0x00
};

/**
  * @brief  USBD_AUDIO_Init
  *         Initialize the AUDIO interface
  * @param  cfgidx: Configuration index
  * @retval status
  */
static byte  USBD_AUDIO_Init( byte cfgidx )
{ USBD_AUDIO_HandleTypeDef   *haudio;

  /* Open EP OUT */
  USBD_LL_OpenEP(AUDIO_OUT_EP,
                 USBD_EP_TYPE_ISOC,
                 AUDIO_OUT_PACKET);

  /* Allocate Audio structure */
  pdev->pClassData = USBD_malloc(sizeof (USBD_AUDIO_HandleTypeDef));

  if( pdev->pClassData == NULL )
  {
    return USBD_FAIL;
  }
  else
  { haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
    haudio->alt_setting = 0;
    haudio->offset = AUDIO_OFFSET_UNKNOWN;
    haudio->wr_ptr = 0;
    haudio->rd_ptr = 0;
    haudio->rd_enable = 0;

    /* Initialize the Audio output Hardware layer */
    if (((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->Init( USBD_AUDIO_FREQ
                                                        , AUDIO_DEFAULT_VOLUME
                                                        , 0) != USBD_OK)
    { return USBD_FAIL;
    }

    /* Prepare Out endpoint to receive 1st packet */
    USBD_LL_PrepareReceive( AUDIO_OUT_EP
                          , haudio->buffer
                          , AUDIO_OUT_PACKET );
  }

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Init
  *         DeInitialize the AUDIO layer
  * @param  cfgidx: Configuration index
  * @retval status
  */
static byte  USBD_AUDIO_DeInit( byte cfgidx )
{


  USBD_LL_CloseEP( AUDIO_OUT_EP);   /* Open EP OUT */

  /* DeInit  physical Interface components */
  if(pdev->pClassData != NULL)
  {
   ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->DeInit(0);
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  req: usb requests
  * @retval status
  */
static byte  USBD_AUDIO_Setup( USBD_SetupReqTypedef *req )
{ USBD_AUDIO_HandleTypeDef   *haudio;
  word len;
  byte *pbuf;
  byte ret = USBD_OK;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  { case USB_REQ_TYPE_CLASS : switch (req->bRequest)
    { case AUDIO_REQ_GET_CUR: AUDIO_REQ_GetCurrent( req); break;
      case AUDIO_REQ_SET_CUR: AUDIO_REQ_SetCurrent( req); break;
      default:      USBD_CtlError ( req);      ret = USBD_FAIL;
    }
    break;

    case USB_REQ_TYPE_STANDARD: switch (req->bRequest)
    { case USB_REQ_GET_DESCRIPTOR:
        if( (req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
        { pbuf = USBD_AUDIO_CfgDesc + 18;
          len = MIN(USB_AUDIO_DESC_SIZ , req->wLength);

          USBD_CtlSendData ( pbuf,   len);
        }
      break;

      case USB_REQ_GET_INTERFACE :
        USBD_CtlSendData ((byte *)&(haudio->alt_setting),
                          1);
      break;

      case USB_REQ_SET_INTERFACE :
        if ((byte)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
        { haudio->alt_setting = (byte)(req->wValue);
        }
        else   /* Call the error management function (command will be nacked */
        { USBD_CtlError ( req);
        }
      break;

      default:
        USBD_CtlError ( req);
        ret = USBD_FAIL;
  } }

  return ret;
}


/**
  * @brief  USBD_AUDIO_GetCfgDesc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static byte  *USBD_AUDIO_GetCfgDesc (word *length)
{ *length = sizeof (USBD_AUDIO_CfgDesc);
  return USBD_AUDIO_CfgDesc;
}

/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  epnum: endpoint index
  * @retval status
  */
static byte  USBD_AUDIO_DataIn( byte epnum )
{
  return USBD_OK; /* Only OUT data are processed */
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @retval status
  */
static byte  USBD_AUDIO_EP0_RxReady()
{ USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  if (haudio->control.cmd == AUDIO_REQ_SET_CUR) /* In this driver, to simplify code, only SET_CUR request is managed */
  { if (haudio->control.unit == AUDIO_OUT_STREAMING_CTRL)
    {
     ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->MuteCtl(haudio->control.data[0]);
      haudio->control.cmd = 0;
      haudio->control.len = 0;
  } }

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @retval status
  */
static byte  USBD_AUDIO_EP0_TxReady()
{ return USBD_OK;   /* Only OUT control data are processed */
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @retval status
  */
static byte  USBD_AUDIO_SOF()
{ return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @retval status
  */
void  USBD_AUDIO_Sync(, AUDIO_OffsetTypeDef offset)
{
  schar shift = 0;
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  haudio->offset =  offset;


  if(haudio->rd_enable == 1)
  {
    haudio->rd_ptr += AUDIO_TOTAL_BUF_SIZE/2;

    if (haudio->rd_ptr == AUDIO_TOTAL_BUF_SIZE)
    {

      haudio->rd_ptr = 0;     /* roll back */
  } }

  if(haudio->rd_ptr > haudio->wr_ptr)
  {
    if((haudio->rd_ptr - haudio->wr_ptr) < AUDIO_OUT_PACKET)
    {
      shift = -4;
    }
    else if((haudio->rd_ptr - haudio->wr_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = 4;
    }

  }
  else
  {
    if((haudio->wr_ptr - haudio->rd_ptr) < AUDIO_OUT_PACKET)
    {
      shift = 4;
    }
    else if((haudio->wr_ptr - haudio->rd_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = -4;
    }
  }

  if(haudio->offset == AUDIO_OFFSET_FULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                         AUDIO_TOTAL_BUF_SIZE/2 - shift,
                                                         AUDIO_CMD_PLAY);
      haudio->offset = AUDIO_OFFSET_NONE;
  }
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  epnum: endpoint index
  * @retval status
  */
static byte  USBD_AUDIO_IsoINIncomplete( byte epnum )
{ return USBD_OK;
}


/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  epnum: endpoint index
  * @retval status
  */
static byte USBD_AUDIO_IsoOutIncomplete( byte epnum )
{ return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  epnum: endpoint index
  * @retval status
  */
static byte  USBD_AUDIO_DataOut( byte epnum )
{ USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  if (epnum == AUDIO_OUT_EP)    /* Increment the Buffer pointer or roll it back when all buffers are full */
  { haudio->wr_ptr += AUDIO_OUT_PACKET;

    if (haudio->wr_ptr == AUDIO_TOTAL_BUF_SIZE) /* All buffers are full: roll back */
    { haudio->wr_ptr = 0;

      if(haudio->offset == AUDIO_OFFSET_UNKNOWN)
      {
        ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                             AUDIO_TOTAL_BUF_SIZE/2,
                                                             AUDIO_CMD_START);
          haudio->offset = AUDIO_OFFSET_NONE;
    } }

    if( haudio->rd_enable == 0 )
    { if (haudio->wr_ptr == (AUDIO_TOTAL_BUF_SIZE / 2))
      { haudio->rd_enable = 1;
    } }

    /* Prepare Out endpoint to receive next audio packet */
    USBD_LL_PrepareReceive(AUDIO_OUT_EP,
                           &haudio->buffer[haudio->wr_ptr],
                           AUDIO_OUT_PACKET);

  }

  return USBD_OK;
}

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_GetCurrent( USBD_SetupReqTypedef *req )
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  memset(haudio->control.data, 0, 64);
  /* Send the current mute state */
  USBD_CtlSendData (haudio->control.data,
                    req->wLength);
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_SetCurrent( USBD_SetupReqTypedef *req)
{ USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;

  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    USBD_CtlPrepareRx (haudio->control.data,
                       req->wLength);

    haudio->control.cmd = AUDIO_REQ_SET_CUR;     /* Set the request value */
    haudio->control.len = req->wLength;          /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex);  /* Set the request target unit */
} }


/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static byte  *USBD_AUDIO_GetDeviceQualifierDesc (word *length)
{
  *length = sizeof (USBD_AUDIO_DeviceQualifierDesc);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
* @brief  USBD_AUDIO_RegisterInterface
* @param  fops: Audio interface callback
* @retval status
*/
byte  USBD_AUDIO_RegisterInterface(   USBD_AUDIO_ItfTypeDef *fops)
{
  if(fops != NULL)
  {
    pdev->pUserData= fops;
  }
  return 0;
}



USBD_ClassTypeDef  USBD_AUDIO =
{ USBD_AUDIO_Init
, USBD_AUDIO_DeInit
, USBD_AUDIO_Setup
, USBD_AUDIO_EP0_TxReady
, USBD_AUDIO_EP0_RxReady
, USBD_AUDIO_DataIn
, USBD_AUDIO_DataOut
, USBD_AUDIO_SOF
, USBD_AUDIO_IsoINIncomplete
, USBD_AUDIO_IsoOutIncomplete
, USBD_AUDIO_GetCfgDesc
, USBD_AUDIO_GetCfgDesc
, USBD_AUDIO_GetCfgDesc
, USBD_AUDIO_GetDeviceQualifierDesc
};

