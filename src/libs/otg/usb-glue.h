/**
 *     AUTHOR: Jose Angel Caso Sanchez, 2023 ( altomaltes@yahoo.es )
 *                                           ( altomaltes@gmail.com )
 *
 *     Copyright (C) 2004, 2024 JACS
 *
 *
 * @file usbh-glue.h
 *
 */

#ifndef  USB_GLUE_H
#define  USB_GLUE_H
// ???
#define DEFAULT_DEVADDR 127
#define USB_OTG_HS_MAX_PACKET_SIZE 512
#define USB_OTG_FS_MAX_PACKET_SIZE  64

#define USB_OTG_SPEED_HIGH  0
#define USB_OTG_SPEED_FULL  1


#define HPRT0_PRTSPD_HIGH_SPEED  0
#define HPRT0_PRTSPD_FULL_SPEED  1
#define HPRT0_PRTSPD_LOW_SPEED   2

#define DO_PING_FLAG    0x40
#define LOW_SPEED_FLAG  0x20

typedef enum
{ URB_STATE_VOID = 0
, URB_STATE_IDLE
, URB_STATE_DONE
, URB_STATE_NACK
, URB_STATE_ERROR
, URB_STATE_STALL

, URB_TOGGLE_OUT= 0x80 /* Needs toogle action */
, URB_TOGGLE_IN = 0x80 /* Needs toogle action */

} URB_STATE;



enum
{ DEVICE_MODE=   0
, HOST_MODE
, OTG_MODE
, DISABLE_MODE
};

void USBIrqHandlerDEV( );
void USBIrqHandlerHOST();
void USBIrqHandlerOTG( );

/* usb-coredev.c
 */
void  USBDepSetAddress ( byte epNum );
void  USBDepSetTestMode( byte mode  );

void  USB_OTG_EP0_OutStart();
schar USB_OTG_EPActivate  ( byte epAddr, byte epType, word maxPacket );
schar OTGEPdeactivate( byte epAddr );

schar OTGEPsetStall       ( byte epAddr );
schar OTGselectCore( dword flags );
schar OTGcoreInitDev( );
schar OTGsetCurrentMode( byte mode );

void   USB_OTG_StopDevice          ();
schar  USB_OTG_EP0StartRecv( word maxpacket );
schar  USB_OTG_EPClearStall( byte epAddr );
schar  USB_OTG_EPStartXrecv( byte, word, word );
schar  USB_OTG_EP0StartXmit( word xferLen   );
schar  USB_OTG_EPStartXmit ( byte epAddr, void * xferBuff, word xferLen );
void * USB_OTG_ReadPacket( void * dest, word len );
schar  USB_OTG_WritePacket( void * src, byte ch_ep_num, word len );




schar OTGgetDmaEnable();
schar OTGgetDevEndpoints(); // USB_OTG_Core.devEndpoints
schar OTGgetHostChannels();







#endif


