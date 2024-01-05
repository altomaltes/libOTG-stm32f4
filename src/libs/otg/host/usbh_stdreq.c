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
 * @file usbh_stdreq.c
 *
 * original draft from MCD Application Team
 */


#include "usbh_core.h"

ALIGN_THIS( byte  USBH_CfgDesc[ CFG_DESC_MAX_SIZE ] );


/**
 * @brief  USBH_Get_DevDesc
 *         Issue Get Device Descriptor command to the device. Once the response
 *         received, it parses the device descriptor and updates the status.
 * @param  dev_desc: Device Descriptor buffer address
 * @param  length: Length of the descriptor
 * @retval Status
 */
schar USBH_Get_DevDesc( byte devAddr, byte length )
{ if ( devAddr )                                      /* Start Machine */
  { USBHsetControlChannelAddr( 0, USBH_HandleEnum );  /* Broadcast addr */
    USB_Host.handleCtrlPkg= USBH_HandleEnum;
    USB_Host.EnumState= ENUM_IDLE;
  }

  USBH_GetDescriptor( USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD
                    , DTYPE_DEVICE << 8
                    , length );

  if ( devAddr )                 /* Start Machine */
  { USB_Host.deviceProp.devAddr= devAddr;
 //   USBHxferOutControl( 0 );
  }

  return( 0 );
}

/**
* @brief  USBH_ParseInterfaceDesc
*         This function Parses the interface descriptor
* @param  if_descriptor : Interface descriptor destination
* @param  buf: Buffer where the descriptor data is available
* @retval None
*/
static void  USBH_ParseInterfaceDesc( USBHinterfaceDescRec *if_descriptor
                                    , byte * buf )
{ if_descriptor->bLength           = buf[ 0 ];
  if_descriptor->bDescriptorType   = buf[ 1 ];
  if_descriptor->bInterfaceNumber  = buf[ 2 ];
  if_descriptor->bAlternateSetting = buf[ 3 ];
  if_descriptor->bNumEndpoints     = buf[ 4 ];
  if_descriptor->bInterfaceClass   = buf[ 5 ];
  if_descriptor->bInterfaceSubClass= buf[ 6 ];
  if_descriptor->bInterfaceProtocol= buf[ 7 ];
  if_descriptor->iInterface        = buf[ 8 ];
}

/**
 * @brief  USBH_ParseEPDesc
 *         This function Parses the endpoint descriptor
 * @param  ep_descriptor: Endpoint descriptor destination address
 * @param  buf: Buffer where the parsed descriptor stored
 * @retval None
 */
static void  USBH_ParseEPDesc( USBHepDescRec * ep_descriptor
                             , byte * buf )
{ ep_descriptor->bLength         = buf[ 0 ];
  ep_descriptor->bDescriptorType = buf[ 1 ];
  ep_descriptor->bEndpointAddress= buf[ 2 ];
  ep_descriptor->bmAttributes    = buf[ 3 ];
  ep_descriptor->wMaxPacketSize  = LE16 (buf + 4);
  ep_descriptor->bInterval       = buf[ 6 ];
}

/**
 * @brief  USBH_ParseCfgDesc
 *         This function Parses the configuration descriptor
 * @param  cfg_desc: Configuration Descriptor address
 * @param  itf_desc: Interface Descriptor address
 * @param  ep_desc: Endpoint Descriptor address
 * @param  buf: Buffer where the source descriptor is available
 * @param  length: Length of the descriptor
 * @retval None
 */
schar USBH_ParseCfgDesc( USBH_CfgDesc_TypeDef * cfg_desc
                             , USBHinterfaceDescRec * itf_desc
                             , USBHepDescRec        *  ep_desc1
                             , byte * buf, word length )
{ USBHinterfaceDescRec * pif ;
  USBHinterfaceDescRec   temp_pif ;
  USBHepDescRec        * pep;

  USBH_DescHeader_t * pdesc = (USBH_DescHeader_t *)buf;
  word                ptr;
  byte                if_ix= 0;
  byte                ep_ix= 0;
  byte                epCount= 0;

  static word  prev_ep_size= 0;
  static byte      prev_itf= 0;

  pdesc= (USBH_DescHeader_t *)buf;

/* Parse configuration descriptor
 */
  cfg_desc->bLength            = *(byte *) (buf + 0);
  cfg_desc->bDescriptorType    = *(byte *) (buf + 1);
  cfg_desc->wTotalLength       = LE16 (buf + 2);
  cfg_desc->bNumInterfaces     = *(byte *)(buf + 4);
  cfg_desc->bConfigurationValue= *(byte *)(buf + 5);
  cfg_desc->iConfiguration     = *(byte *)(buf + 6);
  cfg_desc->bmAttributes       = *(byte *)(buf + 7);
  cfg_desc->bMaxPower          = *(byte *)(buf + 8);

  if ( length > USB_CONFIGURATION_DESC_SIZE )
  { if ( cfg_desc->bNumInterfaces <= USBH_MAX_NUM_INTERFACES )
    { ptr= 0;
      pif= (USBHinterfaceDescRec *)0;

      while( !0 )
      { pdesc= USBH_GetNextDesc((byte *)pdesc, &ptr);
        if ( ptr >= cfg_desc->wTotalLength )
        { break;
        }
        if ( pdesc->bDescriptorType == DTYPE_INTERFACE )
        { if_ix= *(((byte *)pdesc ) + 2);
          pif  = &itf_desc[if_ix];

          if ((*((byte *)pdesc + 3)) < 3)
          { USBH_ParseInterfaceDesc (&temp_pif, (byte *)pdesc);
            ep_ix = 0;

            if ( temp_pif.bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) /* Parse Ep descriptors relative to the current interface */
            { while ( ep_ix < temp_pif.bNumEndpoints )
              { pdesc = USBH_GetNextDesc((void* )pdesc, &ptr);

                if ( pdesc->bDescriptorType == DTYPE_ENDPOINT )
                { pep= ep_desc1 + epCount++;

                  if ( prev_itf != if_ix )
                  { prev_itf= if_ix;
                    USBH_ParseInterfaceDesc (pif, (byte *)&temp_pif);
                  }
                  else
                  { if ( prev_ep_size > LE16((byte *)pdesc + 4))
                    { break;
                    }
                    else
                    { USBH_ParseInterfaceDesc (pif, (byte *)&temp_pif);
                  } }

                  USBH_ParseEPDesc( pep, (byte *)pdesc );
                  pep->bIface= if_ix;
                  prev_ep_size= LE16((byte *)pdesc + 4);
                  ep_ix++;
            } } }

            else
            { return( -1 ); /* num endpoints exceeded */
    } } } } }

    else /* num interfaces exceeded */
    { return( -1 );
    }

    prev_ep_size= 0;
    prev_itf= 0;
  }

  return( 0 );
}

/**
 * @brief  USBH_Get_CfgDesc
 *         Issues Configuration Descriptor to the device. Once the response
 *         received, it parses the configuration descriptor and updates the
 *         status.
 * @param  cfg_desc: Configuration Descriptor address
 * @param  itf_desc: Interface Descriptor address
 * @param  ep_desc: Endpoint Descriptor address
 * @param  length: Length of the descriptor
 * @retval Status
 */
schar USBHGetCfgDesc( word length )
{ return( USBH_GetDescriptor( USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD
                            , DTYPE_CONFIGURATION << 8
                            , length ));
}

schar parseCfgDesc( word length )
{ word index = 0;

  for( ; index < length ; index ++ )     /* save Cfg descriptor for class parsing usage */
  { USBH_CfgDesc[ index ]= USB_HOST.rxBuffer[ index ];
  }

  return( USBH_ParseCfgDesc( &USB_Host.deviceProp.Cfg_Desc
                           ,  USB_Host.deviceProp.Itf_Desc
                           ,  USB_Host.deviceProp.Ep_Desc
                           ,  USB_HOST.rxBuffer
                           ,  length ));
}

schar testCfgDesc( void * buff, int len )
{// word index = 0;

 // for( ; index < length ; index ++ )     /* save Cfg descriptor for class parsing usage */
 // { USBH_CfgDesc[ index ]= USB_HOST.rxBuffer[ index ];
 // }

  return( USBH_ParseCfgDesc( &USB_Host.deviceProp.Cfg_Desc
                           ,  USB_Host.deviceProp.Itf_Desc
                           ,  USB_Host.deviceProp.Ep_Desc
                           ,  buff
                           ,  len ));
}

/**
 * @brief  USBH_Get_StringDesc
 *         Issues string Descriptor command to the device. Once the response
 *         received, it parses the string descriptor and updates the status.
 * @param  string_index: String index for the descriptor
 * @param  buff: Buffer address for the descriptor
 * @param  length: Length of the descriptor
 * @retval Status
 */
schar USBH_Get_StringDesc( byte   string_index
                               , byte * buff
                               , word   length )
{ return( USBH_GetDescriptor( USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD
                            , ( DTYPE_STRING << 8 ) | string_index
                            , length ));
}

/**
 * @brief  USBH_GetDescriptor
 *         Issues Descriptor command to the device. Once the response received,
 *         it parses the descriptor and updates the status.
 * @param  req_type: Descriptor type
 * @param  value_idx: wValue for the GetDescriptr request
 * @param  buff: Buffer to store the descriptor
 * @param  length: Length of the descriptor
 * @retval Status
 */
schar USBH_GetDescriptor( byte  req_type
                              , word value_idx
                              , word length )
{ USB_Host.Control.setup.b.bmRequestType= USB_D2H | req_type;
  USB_Host.Control.setup.b.bRequest     = USB_REQ_GET_DESCRIPTOR;
  USB_Host.Control.setup.b.wValue.w     = value_idx;

  if (( value_idx & 0xff00 ) == ( DTYPE_STRING << 8 ) )
  { USB_Host.Control.setup.b.wIndex.w = 0x0409;
  }
  else
  { USB_Host.Control.setup.b.wIndex.w = 0;
  }
  USB_Host.Control.setup.b.wLength.w= length;

  return( USBH_CtlReq( USB_HOST.rxBuffer, length ));
}




/**
 * @brief  USBH_SetAddress
 *         This command sets the address to the connected device
 * @param  DeviceAddress: Device address to assign
 * @retval Status
 */
schar USBH_SetAddress( byte DeviceAddress )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_RECIPIENT_DEVICE
                                        | USB_REQ_TYPE_STANDARD;

  USB_Host.Control.setup.b.bRequest = USB_REQ_SET_ADDRESS;
  USB_Host.Control.setup.b.wValue.w = (word)DeviceAddress;
  USB_Host.Control.setup.b.wIndex.w = 0;
  USB_Host.Control.setup.b.wLength.w= 0;

  return( USBH_CtlReq( 0, 0 ));
}

/**
 * @brief  USBH_SetCfg
 *         The command sets the configuration value to the connected device
 * @param  cfg_idx: Configuration value
 * @retval Status
 */
schar USBH_SetCfg( word cfg_idx )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_RECIPIENT_DEVICE
                                        | USB_REQ_TYPE_STANDARD;

  USB_Host.Control.setup.b.bRequest = USB_REQ_SET_CONFIGURATION;
  USB_Host.Control.setup.b.wValue.w = cfg_idx;
  USB_Host.Control.setup.b.wIndex.w = 0;
  USB_Host.Control.setup.b.wLength.w = 0;

  return( USBH_CtlReq( 0, 0 ));
}

/**
 * @brief  USBHgetInterface
 *         USBHgetInterface
 *         The command gets the Interface value to the connected device
 * @param  itf_idx: Interface value
 * @retval Status
 */
schar USBHgetInterface( byte request
                            , word value
                            , void * buf, word len )
{ USB_Host.Control.setup.b.bmRequestType= USB_D2H
                                        | USB_REQ_TYPE_CLASS
                                        | USB_REQ_RECIPIENT_INTERFACE;

  USB_Host.Control.setup.b.bRequest = request;
  USB_Host.Control.setup.b.wValue.w = value;
  USB_Host.Control.setup.b.wIndex.w = 0;
  USB_Host.Control.setup.b.wLength.w= 1;

  return( USBH_CtlReq( buf, len ));
}

/**
 * @brief  USBHsetInterface
 *
 *         The command sets the Interface value to the connected device
 * @param  itf_idx: Interface value
 * @retval Status
 */
schar USBHsendInterface( byte request
                             , word index, word value
                             , void * buff, word len )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_TYPE_CLASS
                                        | USB_REQ_RECIPIENT_INTERFACE;

  USB_Host.Control.setup.b.bRequest = request;
  USB_Host.Control.setup.b.wIndex.w = index;
  USB_Host.Control.setup.b.wValue.w = value;
  USB_Host.Control.setup.b.wLength.w= len;

  return( USBH_CtlReq( buff, len ) );
}

schar USBHsetInterface( byte request
                      , word index, word value )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_TYPE_CLASS
                                        | USB_REQ_RECIPIENT_INTERFACE;

  USB_Host.Control.setup.b.bRequest = request;
  USB_Host.Control.setup.b.wIndex.w = index;
  USB_Host.Control.setup.b.wValue.w = value;
  USB_Host.Control.setup.b.wLength.w= 0;

  return( USBH_CtlReq( 0, 0 ) );
}



/**
  * @brief  USBH_SetDeviceFeature
  *         The command sets the device features (remote wakeup feature,..)
  * @param  itf_idx
  * @retval Status
  */
schar USBH_SetDeviceFeature( byte FeatureSelector, word wIndex )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_RECIPIENT_DEVICE
                                        | USB_REQ_TYPE_STANDARD;

  USB_Host.Control.setup.b.bRequest = USB_REQ_SET_FEATURE;
  USB_Host.Control.setup.b.wValue.w = FeatureSelector;
  USB_Host.Control.setup.b.wIndex.w = wIndex;
  USB_Host.Control.setup.b.wLength.w= 0;

  return( USBH_CtlReq( 0, 0 ));
}

/**
 * @brief USBH_getClassFeature
 *
 * @retval
 */
byte UHOSTgetClassStatus( word wIndex, void * buf, word len  )
{ USB_Host.Control.setup.b.bmRequestType= USB_D2H             /// IN
                                        | USB_REQ_TYPE_CLASS
                                        | ( wIndex ? USB_REQ_RECIPIENT_OTHER
                                                   : USB_REQ_RECIPIENT_DEVICE );

  USB_Host.Control.setup.b.bRequest = USB_REQ_GET_STATUS;
  USB_Host.Control.setup.b.wValue.w = 0;
  USB_Host.Control.setup.b.wIndex.w = wIndex;
  USB_Host.Control.setup.b.wLength.w= 4;

  return( USBH_CtlReq( buf, 8 ));
}

/**
 * @brief USBH_SetClassFeature
 *
 * @retval
 */
byte UHOSTsetClassFeature( byte FeatureSelector, word wIndex)
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D           /// OUT
                                        | USB_REQ_TYPE_CLASS
                                        | ( wIndex ? USB_REQ_RECIPIENT_OTHER
                                                   : USB_REQ_RECIPIENT_DEVICE );

  USB_Host.Control.setup.b.bRequest = USB_REQ_SET_FEATURE;
  USB_Host.Control.setup.b.wValue.w = FeatureSelector;
  USB_Host.Control.setup.b.wIndex.w = wIndex;
  USB_Host.Control.setup.b.wLength.w= 0;

  return( USBH_CtlReq( 0, 0 ));
}

byte UHOSTclearClassFeature( byte FeatureSelector, word wIndex )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D             /// OUT
                                        | USB_REQ_TYPE_CLASS
                                        | ( wIndex ? USB_REQ_RECIPIENT_OTHER
                                                   : USB_REQ_RECIPIENT_DEVICE );

  USB_Host.Control.setup.b.bRequest = USB_REQ_CLEAR_FEATURE;
  USB_Host.Control.setup.b.wValue.w = FeatureSelector;
  USB_Host.Control.setup.b.wIndex.w = wIndex;
  USB_Host.Control.setup.b.wLength.w= 0;

  return( USBH_CtlReq( 0, 0 ));
}


/**
 * @brief  USBH_ClearDeviceFeature
 *         The command sets the device features (remote wakeup feature,..)
 * @param  itf_idx
 * @retval Status
 */
schar USBH_ClearDeviceFeature( byte FeatureSelector, word wIndex )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_RECIPIENT_DEVICE
                                        | USB_REQ_TYPE_STANDARD;

  USB_Host.Control.setup.b.bRequest = USB_REQ_CLEAR_FEATURE;
  USB_Host.Control.setup.b.wValue.w = FeatureSelector;
  USB_Host.Control.setup.b.wIndex.w = wIndex;
  USB_Host.Control.setup.b.wLength.w = 0;

  return( USBH_CtlReq( 0, 0 ));
}

/**
 * @brief  USBH_ClrFeature
 *         This request is used to clear or disable a specific feature.
 * @param  ep_num: endpoint number
 * @param  hcNum: Host channel number
 * @retval Status
 */
schar USBH_ClrFeature( byte ep_num, byte hcNum )
{ USB_Host.Control.setup.b.bmRequestType= USB_H2D
                                        | USB_REQ_RECIPIENT_ENDPOINT
                                        | USB_REQ_TYPE_STANDARD;

  USB_Host.Control.setup.b.bRequest = USB_REQ_CLEAR_FEATURE;
  USB_Host.Control.setup.b.wValue.w = FEATURE_SELECTOR_ENDPOINT;
  USB_Host.Control.setup.b.wIndex.w = ep_num;
  USB_Host.Control.setup.b.wLength.w = 0;

  if ((ep_num & USB_REQ_DIR_MASK ) == USB_D2H) /* EP Type is IN */
  { USB_HOST.hc[ hcNum ].toggleIn= 0;
  }
  else /* EP Type is IN */
  { USB_HOST.hc[ hcNum ].toggleOut= 0;
  }

  return( USBH_CtlReq( 0 , 0 ));
}

/**
* @brief  USBH_ParseDevDesc
*         This function Parses the device descriptor
* @param  dev_desc: device_descriptor destination address
* @param  buf: Buffer where the source descriptor is available
* @param  length: Length of the descriptor
* @retval None
*/
void  USBH_ParseDevDesc( USBH_DevDesc_TypeDef * dev_desc
                       , byte * buf , word length )
{ dev_desc->bLength        = *(byte  *) (buf +  0);
  dev_desc->bDescriptorType= *(byte  *) (buf +  1);
  dev_desc->bcdUSB         = LE16 (buf +  2);
  dev_desc->bDeviceClass   = *(byte  *) (buf +  4);
  dev_desc->bDeviceSubClass= *(byte  *) (buf +  5);
  dev_desc->bDeviceProtocol= *(byte  *) (buf +  6);
  dev_desc->bMaxPacketSize = *(byte  *) (buf +  7);

/* For 1st time after device connection, Host may issue only 8 bytes for
   Device Descriptor Length
 */

  if ( length > 8 )
  { dev_desc->idVendor           = LE16(     buf +  8 );
    dev_desc->idProduct          = LE16(     buf + 10 );
    dev_desc->bcdDevice          = LE16(     buf + 12 );
    dev_desc->iManufacturer      = *(byte *)(buf + 14);
    dev_desc->iProduct           = *(byte *)(buf + 15);
    dev_desc->iSerialNumber      = *(byte *)(buf + 16);
    dev_desc->bNumConfigurations = *(byte *)(buf + 17);
} }


/**
* @brief  USBH_ParseStringDesc
*         This function Parses the string descriptor
* @param  psrc: Source pointer containing the descriptor data
* @param  pdest: Destination address pointer
* @param  length: Length of the descriptor
* @retval None
*/
void USBH_ParseStringDesc( byte * psrc
                         , byte * pdest
                         , word length)
{ word strlength;
  word idx;

  /* The UNICODE string descriptor is not NULL-terminated. The string length is
  computed by subtracting two from the value of the first byte of the descriptor.
  */

  /* Check which is lower size, the Size of string or the length of bytes read
  from the device */

  if ( psrc[1] == DTYPE_STRING ) /* Make sure the Descriptor is String Type */
  { strlength = ( ( (psrc[0]-2) <= length) ? (psrc[0]-2) :length);  /* psrc[0] contains Size of Descriptor, subtract 2 to get the length of string */
    psrc += 2; /* Adjust the offset ignoring the String Len and Descriptor type */

    for( idx = 0 /* Copy Only the string and ignore the UNICODE ID, hence add the src */
       ; idx < strlength
       ; idx+=2 )
    { *pdest=  psrc[ idx ];
       pdest++;
    }
    *pdest = 0; /* mark end of string */
} }

/**
* @brief  USBH_GetNextDesc
*         This function return the next descriptor header
* @param  buf: Buffer where the cfg descriptor is available
* @param  ptr: data pointer inside the cfg descriptor
* @retval next header
*/
USBH_DescHeader_t   * USBH_GetNextDesc( byte * pbuf, word * ptr )
{ USBH_DescHeader_t * hdr= (USBH_DescHeader_t *)pbuf;

 *ptr+= hdr->bLength;
 pbuf+= hdr->bLength;

  return(( USBH_DescHeader_t * )pbuf );
}


