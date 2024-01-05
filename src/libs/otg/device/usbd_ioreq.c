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
 * @file usbd_ioreq.c
 *
 * original draft from MCD Application Team
 */


#include "usbd_core.h"

#pragma GCC optimize ("O0")

/**
 * @brief  Transmit data over USB
 * @param epAddr: endpoint address
 * @param pbuf: pointer to Tx buffer
 * @param buf_len: data length
 * @retval : status
 */
dword USBDepTx( byte   epAddr
              , byte * pbuf
              , word   buf_len )
{ static volatile USB_OTG_EP * ep;

  ep= &USB_DEV.inEp[ epAddr & 0x7F ];

/* Setup and start the Transfer
 */
  ep->is_in     = 1;
  ep->num       = epAddr & 0x7F;
  ep->xferBuffEp= pbuf;
  ep->dmaAddr   = (dword)pbuf;
  ep->xferCount = 0;
  ep->xferLen   = buf_len;

  if ( ep->num == 0 )
  { if ( ep->xferLen > ep->maxpacket )
    { ep->xferLen= ep->maxpacket;
    }
    USB_OTG_EP0StartXmit( ep->xferLen );
  }
  else
  { USB_OTG_EPStartXmit( epAddr
                       , ep->xferBuffEp
                       , ep->xferLen );
  }

  return( 0 );
}

/**
 * @brief  USBDctlSendData
 *         send data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be sent
 * @retval status
 */
schar USBDctlSendData( byte * pbuf, word len )
{ USB_DEV.inEp[ 0 ].totalDataLen= len;
  USB_DEV.inEp[ 0 ].remDataLen  = len;
  USB_DEV.deviceState= USB_OTG_EP0_DATA_IN;

  USBDepTx( 0, pbuf, len);

  return( 0 );
}

/**
 * @brief  USBDctlPrepareRx
 *         receive data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be received
 * @retval status
 */
schar USBDctlPrepareRx( byte * pbuf, word len )
{ USB_DEV.outEp[ 0 ].totalDataLen= len;
  USB_DEV.outEp[ 0 ].remDataLen  = len;
  USB_DEV.deviceState= USB_OTG_EP0_DATA_OUT;

  USBDepPrepareRx( 0, pbuf, len );

  return( 0 );
}

/**
 * @brief  USBDctlContinueSendData
 *         continue sending data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be sent
 * @retval status
 */
schar  USBDctlContinueSendData( byte * pbuf, word len )
{ USBDepTx( 0, pbuf, len);

  return( 0 );
}

/**
* @brief  USBDctlContinueRx
*         continue receive data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be received
 * @retval status
 */
schar USBDctlContinueRx( byte * pbuf, word len)
{ USBDepPrepareRx( 0, pbuf, len );

  return( 0 );
}

/**
 * @brief  USBDctlSendStatus
 *         send zero length packet on the ctl pipe
 * @retval status
 */
schar  USBDctlSendStatus(  )
{ USB_DEV.deviceState = USB_OTG_EP0_STATUS_IN;
  USBDepTx (0,  NULL, 0 );
  USB_OTG_EP0_OutStart();

  return( 0 );
}

/**
 * @brief  USBDctlReceiveStatus
 *         receive zero length packet on the ctl pipe
 * @retval status
 */
schar USBDctlReceiveStatus(  )
{ USB_DEV.deviceState= USB_OTG_EP0_STATUS_OUT;
  USBDepPrepareRx( 0, NULL, 0 );

  USB_OTG_EP0_OutStart();

  return( 0 );
}


/**
 * @brief  USBDreadPacket (JACS)
 *
 * @retval status
 */
short USBDreadPacket( byte epNum, word size )
{ if ( size )
  { USB_OTG_EP * ep= &USB_DEV.outEp[ epNum ];

    if ( size == 0xFFFF ) /* asked for setupPacket */
    { ep->xferBuffEp= USB_DEV.setupPacket;
      ep->xferCount= 0; size= 8;
    }

    USB_OTG_ReadPacket( ep->xferBuffEp, size );
    ep->xferBuffEp += size;
    ep->xferCount  += size;
  }

  return( size );
}

/**
 * @brief  USBDgetRxCount
 *         returns the received data length
 *         epnum: endpoint index
 * @retval Rx Data blength
 */
word  USBDgetRxCount( byte epnum )
{ return( USB_DEV.outEp[ epnum ].xferCount );
}


/**
 * @brief  USBHwritePacket (JACS)
 *
 * @retval status
 */
short USBDwritePacket( byte epNum, word size )
{ USB_OTG_EP * ep= USB_DEV.inEp + epNum;

  if ( size )
  { word left= ep->xferLen - ep->xferCount;

    if ( size > left )
    { size= left;
    }

    if ( size > ep->maxpacket )
    { size= ep->maxpacket;
    }

     usbOTGwritePacket( ep->xferBuffEp
                      , epNum
                      , size );
     ep->xferBuffEp+= size;
     ep->xferCount += size;
  }

  return( ep->xferLen - ep->xferCount );
}
