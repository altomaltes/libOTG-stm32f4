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
 * @file usbh_msc_scsi.c
 *
 * original draft from MCD Application Team
 */


#include "usb_hcd.h"
#include "usb_conf.h"
#include "diskio.h"
#include "usbh_msc_core.h"

dword getFattime( void );

static volatile DSTATUS Stat= STA_NOINIT;	/* Disk status */

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS diskInitialize( byte drv		) /* Physical drive number (0) */
{ return( Stat &= ~STA_NOINIT );
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS diskStatus( byte drv	 )	/* Physical drive number (0) */
{ if ( drv ) return STA_NOINIT;		/* Supports only single drive */
  return( Stat );
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT diskRead( dword  pDrv			 /* Physical drive number (0) */
                , void * buff			 /* Pointer to the data buffer to store read data */
                , dword  sector		 /* Start sector number (LBA) */
                , dword  count )	/* Sector count (1..255) */
{ if ( count )
  { if ( !count             ) return( RES_PARERR );
    if ( Stat & STA_NOINIT  ) return( RES_NOTRDY );
    if ( Stat & STA_PROTECT ) return( RES_WRPRT  );

    count *= 512;

    word maxTry= 5;
    while( maxTry-- )                       /* Allow 5 tries     */
    { if ( USBH_MSCread10( pDrv
                          , buff            /* Not able to read */
                          , sector
                          , count  ) < 0 )
      { return( RES_ERROR );                /* IO error */
      }

      word end= usbOTGgetCurrentFrame( 5 );       /* Allow 5 ms to read */
      while( end != usbOTGgetCurrentFrame( 0 ))
      { if ( USBH_MSC_Param.xfer.MSCState == USBH_MSC_IDLE )  /* Is it free ?      */
        { return( RES_OK );   /* read achieved */
    } } }
    return( RES_NOTRDY );
  }
  return( RES_OK );
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT diskWrite( dword pDrv			     /* Physical drive number (0) */
                 , const void * buff	/* Pointer to the data to be written */
                 , dword sector	    	/* Start sector number (LBA) */
                 , dword count	)	    /* Sector count (1..255) */
{ if ( count )
  { if ( /* pDrv || */ !count     ) return( RES_PARERR );
    if ( Stat & STA_NOINIT  ) return( RES_NOTRDY );
    if ( Stat & STA_PROTECT ) return( RES_WRPRT  );

    count *= 512;

/* Make sure previous is written
 */
//    word maxTry= 5;        /* Allow 5 tries     */
//    while( maxTry-- )
//    { word end= usbOTGgetCurrentFrame( 5 );                        /* Allow 5 ms to read */
//      while( end != usbOTGgetCurrentFrame( 0 ))                    /* Timer not reached */
//      { if ( USBH_MSC_Param.xfer.MSCState == USBH_MSC_IDLE )  /* Is it free ?      */
//        { if ( USBH_MSCwrite10( NULL                           /* Write previous  */
//                               , sector
//                               , count ) < 0 )
//          { return( RES_ERROR );   /* IO error */
//
//        } break;  // !!!
//     } } //}

//    if ( maxTry )                                    /* Able to write */


    while ( USBH_MSC_Param.xfer.MSCState != USBH_MSC_IDLE ){};  /* Is it free ?      */

    { if ( USBH_MSCwrite10( pDrv
                           , buff                         /* Write previous  */
                           , sector
                           , count ) < 0 )
     { return( RES_ERROR );   /* IO error */
   } }

    return( RES_NOTRDY );
  }
  return( RES_OK );
}


#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT diskIoctl( dword  drv	  	/* Physical drive number (0) */
                 , dword  ctrl 		/* Control code */
                 , void * buff	)	/* Buffer to send/receive control data */
{ if ( !drv ) return RES_PARERR;
  if ( Stat & STA_NOINIT ) return RES_NOTRDY;

  switch ( ctrl )
  { case CTRL_SYNC :		/* Make sure that no pending write process */
    return( RES_OK );

    case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
      *(dword*)buff= USBH_MSCgetCapacity( drv );
    return( RES_OK );

    case GET_SECTOR_SIZE: *(word *)buff = 512; return( RES_OK );	/* Get R/W sector size (WORD) */
    case GET_BLOCK_SIZE : *(dword*)buff=  512; return( RES_OK ); /* Get erase block size in unit of sector (DWORD) */
  }

  return( RES_PARERR );
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
WEAK dword getFattime( void )
{ return( 0 );
}

