/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *   Heavily scrapped code. See original note below
 *
 * @file host.c
 *
 */

#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "stm32.h"


/* share among boards
 */
extern word LED1;
extern word LED2;
extern word LED3;
extern word LED4;

/**
 * @brief  usbDevGotConnected
 *         Links the correct device handler
 * @retval
 */
void usbHIDkeyArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED2 ); PIN_SET( LED4 );
  }
  else
  { PIN_RST( LED2 ); PIN_RST( LED4 );
} }


void usbHIDmouseArrived( byte * report )
{ if ( report[ 2 ] )
  { PIN_SET( LED2 ); PIN_SET( LED4 );
  }
  else
  { PIN_RST( LED2 ); PIN_RST( LED4 );
} }

void usbHIDrawArrived( byte * report )
{ if ( report[ 6 ] )
  { PIN_SET( LED1 ); PIN_SET( LED3 );
  }
  else
  { PIN_RST( LED1 ); PIN_RST( LED3 );
} }



#define HID_EP 1

/**
  * @brief  hidHandleRec.
  * @param  None
  * @retval None
  */
struct hidHandleRec
{ struct USBDdriverHidRec hid;
}

hidHandle=
{{ &USBD_HID_cb
}};


/**
  * @brief  handleSofDevISR.
  * @param  None
  * @retval None
  */
short handleSofDevISR( dword frame )
{ static schar buf[ 4 ];

  if ( frame & 0x03F ) /* Every 8 ms */
  { return( 0 );
  }

  frame >>= 6;

  switch( frame & 0x7 )
  { case 0x0: buf[ 1 ]=  1; buf[ 2 ]=  0; break;
    case 0x1: buf[ 1 ]=  1; buf[ 2 ]= -1; break;
    case 0x2: buf[ 1 ]=  0; buf[ 2 ]= -1; break;
    case 0x3: buf[ 1 ]= -1; buf[ 2 ]= -1; break;
    case 0x4: buf[ 1 ]= -1; buf[ 2 ]=  0; break;
    case 0x5: buf[ 1 ]= -1; buf[ 2 ]=  1; break;
    case 0x6: buf[ 1 ]=  0; buf[ 2 ]=  1; break;
    case 0x7: buf[ 1 ]=  1; buf[ 2 ]=  1; break;
  }

  PIN_PUT( LED1, buf[ 1 ] > 0 );
  PIN_PUT( LED2, buf[ 2 ] > 0 );
  PIN_PUT( LED3, buf[ 1 ] < 0 );
  PIN_PUT( LED4, buf[ 2 ] < 0 );

  buf[0]=0;buf[1]=0;buf[2]=1;buf[3]=0;

  DeviceListRec * dList= USBDgetDevice( &hidHandle );

  return( dList ? USBDepTx( dList->epIn, buf, 4 ) : 0 );
}


void usbDevEvent( word what, void * args )
{ switch( what )
  { case USB_DEV_DISCON:
      PIN_RST( LED2 ); PIN_RST( LED4 );
      for( int loop= 0
         ;     loop < 3
         ;     loop ++ )
      { PIN_SET( LED1 ); PIN_SET( LED3 ); mDelay( 90 );
        PIN_RST( LED1 ); PIN_RST( LED3 ); mDelay( 60 );
      }
    break;

    case USB_DEV_CONFIGURED:
      PIN_RST( LED1 ); PIN_RST( LED3 );
      for( int loop= 0
         ;     loop < 3
         ;     loop ++ )
      { PIN_SET( LED2 ); PIN_SET( LED4 ); mDelay( 90 );
        PIN_RST( LED2 ); PIN_RST( LED4 ); mDelay( 60 );
      }
    break;

  }

}




