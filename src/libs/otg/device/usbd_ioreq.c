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
short USBDepTx( byte   epAddr
              , const void * pbuf
              , word   bufLen )
{ static volatile USB_OTG_EP * ep;

  ep= &USB_DEV.inEp[ ep->num= epAddr & EPDIR_MASK ];

/* Setup and start the Transfer
 */
  ep->xferBuffEp= (byte*)pbuf;
//  ep->dmaAddr   = (dword)pbuf;
  ep->xferCount = 0;
  ep->xferLen   = bufLen;

  if ( ep->num == 0 )
  { if ( ep->xferLen > ep->maxpacket )
    { ep->xferLen= ep->maxpacket;
    }
    USBDep0StartXmit( bufLen );
  }
  else
  { USBDepStartXmit( epAddr
                   , pbuf
                   , bufLen );
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
short USBDctlSendData( const void * pbuf, word len )
{ USB_DEV.inEp[ 0 ].totalDataLen=
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
short USBDctlPrepareRx( byte * pbuf, word len )
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
short  USBDctlContinueSendData( const void * pbuf, word len )
{ USBDepTx( 0, pbuf, len );

  return( 0 );
}

/**
* @brief  USBDctlContinueRx
*         continue receive data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be received
 * @retval status
 */
short USBDctlContinueRx( byte * pbuf, word len)
{ USBDepPrepareRx( 0, pbuf, len );

  return( 0 );
}

/**
 * @brief  USBDctlSendStatus
 *         send zero length packet on the ctl pipe
 * @retval status
 */
short  USBDctlSendStatus(  )
{ USB_DEV.deviceState= USB_OTG_EP0_STATUS_IN;
  USBDepTx( 0, NULL, 0 );
  USBDep0OutStart();

  return( 0 );
}

/**
 * @brief  USBDctlReceiveStatus
 *         receive zero length packet on the ctl pipe
 * @retval status
 */
short USBDctlReceiveStatus(  )
{ USB_DEV.deviceState= USB_OTG_EP0_STATUS_OUT;
  USBDepPrepareRx( 0, NULL, 0 );
  USBDep0OutStart();

  return( 0 );
}


/**
 * @brief  USBDreadPacket (JACS)
 *
 * @retval status
 */
int USBDreadPacket( byte epNum, word size )
{ if ( size )
  { USB_OTG_EP * ep= &USB_DEV.outEp[ epNum ];

    if ( size == 0xFFFF ) /* asked for setupPacket */
    { ep->xferBuffEp= USB_DEV.setupPacket;
      ep->xferCount= 0; size= 8;
    }

    OTGreadPacket( ep->xferBuffEp, size );
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
int USBDgetRxCount( byte epnum )
{ return( USB_DEV.outEp[ epnum ].xferCount );
}

/**
 * @brief  USBHwritePacket (JACS)
 *
 * @retval status
 */
int USBDgetTxCount( byte epNum )          /* Available size */
{ USB_OTG_EP * ep= USB_DEV.inEp + epNum;

  int left= ep->xferLen - ep->xferCount;

  if ( left > ep->maxpacket )
  { left= ep->maxpacket;
  }

  return( left );
}

int USBDwritePacket( byte epNum, word avail )   /* Available size */
{ USB_OTG_EP * ep= USB_DEV.inEp + epNum;

  if ( ep->xferLen )                           /* something to xmit */
  { if ( ep->xferCount < ep->xferLen )
    { int left= ep->xferLen - ep->xferCount;

      if ( left > ep->maxpacket )
      { left= ep->maxpacket;
      }

      int l32b= (left + 3) / 4;

      if ( l32b <= avail )              /* 32 bit granulatity */   /* short ( last ) packet */
      { OTGwritePacket( ep->xferBuffEp
                       , epNum
                       , left );

        ep->xferBuffEp+= left;
        ep->xferCount += left;
        return( ep->xferLen - ep->xferCount );
  } } }

  return( 0 );
}
