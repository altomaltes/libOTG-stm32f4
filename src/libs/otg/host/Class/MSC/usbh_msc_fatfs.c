
#include "usb_conf.h"
#include "diskio.h"
#include "usbh_msc_core.h"


/*--------------------------------------------------------------------------

Module Private Functions and Variables

---------------------------------------------------------------------------*/
DWORD getFattime(void);

static volatile DSTATUS Stat= STA_NOINIT;	/* Disk status */

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS diskInitialize( BYTE drv		) /* Physical drive number (0) */
{ return( Stat &= ~STA_NOINIT );
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS diskStatus( BYTE drv	 )	/* Physical drive number (0) */
{ if ( drv ) return STA_NOINIT;		/* Supports only single drive */
  return( Stat );
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT diskRead( BYTE   pdrv			  /* Physical drive number (0) */
                , BYTE * buff			  /* Pointer to the data buffer to store read data */
                , DWORD  sector		 /* Start sector number (LBA) */
                , DWORD   count )		/* Sector count (1..255) */
{ if ( count )
  { if ( pdrv || !count     ) return( RES_PARERR );
    if ( Stat & STA_NOINIT  ) return( RES_NOTRDY );
    if ( Stat & STA_PROTECT ) return( RES_WRPRT  );

    count *= 512;

    if ( USBH_MSC_Read10((BYTE*)buff /* Not able to read */
                        , sector
                        , count  ) < 0 )
    { return( RES_ERROR );   /* IO error */
    }

    while( USBH_MSC_BOTXferParam.MSCState != USBH_MSC_IDLE );  /* Is it busy ? */
  }

  return( RES_OK );
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT diskWrite( BYTE pdrv			      /* Physical drive number (0) */
                 , const BYTE * buff	/* Pointer to the data to be written */
                 , DWORD sector	    	/* Start sector number (LBA) */
                 , DWORD count	)	    /* Sector count (1..255) */

{ if ( count )
  { if ( pdrv || !count     ) return( RES_PARERR );
    if ( Stat & STA_NOINIT  ) return( RES_NOTRDY );
    if ( Stat & STA_PROTECT ) return( RES_WRPRT  );

    while( USBH_MSC_BOTXferParam.MSCState != USBH_MSC_IDLE );  /* Is it previously busy ? */

    count *= 512;

    if ( USBH_MSC_Write10((BYTE*)buff /* Not able to read */
       , sector
       , count ) < 0 )
    { return( RES_ERROR );   /* IO error */
  } }

  return( RES_OK );
}

#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT diskIoctl( BYTE drv		/* Physical drive number (0) */
                  , BYTE ctrl		/* Control code */
                  , void *buff	)	/* Buffer to send/receive control data */

{ if ( !drv ) return RES_PARERR;
  if ( Stat & STA_NOINIT ) return RES_NOTRDY;

  switch ( ctrl )
  { case CTRL_SYNC :		/* Make sure that no pending write process */
    return( RES_OK );

    case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
      *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
    return( RES_OK );

    case GET_SECTOR_SIZE: *(WORD*)buff = 512; return( RES_OK );	/* Get R/W sector size (WORD) */
    case GET_BLOCK_SIZE : *(DWORD*)buff= 512; return( RES_OK ); /* Get erase block size in unit of sector (DWORD) */
  }

  return( RES_PARERR );
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
WEAK DWORD getFattime( void )
{ return 0;
}

#endif /* _USE_IOCTL != 0 */
