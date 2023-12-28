/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define ATA		0
#define MMC		1
#define USB		2

/*-----------------------------------------------------------------------*/
/* Inicializes a Drive                                                    */

DSTATUS disk_initialize( BYTE drv )    /* Physical drive nmuber (0..) */
{ //return( stat &= ~STA_NOINIT );
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS diskStatusXXX(	BYTE drv	)	/* Physical drive nmuber (0..) */
{ DSTATUS stat= 0;
	 int result;

	 switch ( drv )
	 { case ATA: result= ATAdiskStatus();	return( stat );	// translate the reslut code here
   	case MMC: result= MMCdiskStatus();	return( stat );	// translate the reslut code here
   	case USB: result= USBdiskStatus();	return( stat );	// translate the reslut code here
	 }

	return( STA_NOINIT );
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT diskReadXXX( BYTE drv		    /* Physical drive nmuber (0..) */
                ,	BYTE *buff		  /* Data buffer to store read data */
                ,	DWORD sector	 /* Sector address (LBA) */
                ,	BYTE count )		/* Number of sectors to read (1..255) */
{ switch ( drv )
	 { case ATA: return( ATAdiskRead( buff, sector, count )); 		// translate the reslut code here
   	case MMC:	return( MMCdiskRead( buff, sector, count )); 		// translate the reslut code here
   	case USB:	return( USBdiskRead( buff, sector, count ));		// translate the reslut code here
	}

	return( RES_PARERR );
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT diskWriteXXX( BYTE drv			      /* Physical drive nmuber (0..) */
                 ,	const BYTE *buff	/* Data to be written */
                 ,	DWORD sector		   /* Sector address (LBA) */
                 , BYTE count	)		   /* Number of sectors to write (1..255) */
{ DRESULT res;
 	int result;

 	switch (drv)
 	{ case ATA :	    	result= ATAdiskWrite(buff, sector, count);
		// translate the reslut code here

		  return res;

 	  case MMC :		    result= MMCdiskWrite(buff, sector, count);
		// translate the reslut code here
  		return res;

	   case USB :		    result= USBdiskWrite(buff, sector, count);
		// translate the reslut code here
  		return res;
	}

	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT diskIoctlXXX( BYTE drv		   /* Physical drive nmuber (0..) */
                 ,	BYTE ctrl		  /* Control code */
                 ,	void *buff	)	/* Buffer to send/receive control data */
{ DRESULT res;
	 int result;

	switch ( drv )
	{ case ATA :
		  // pre-process here
  	 	result = ATA_diskIoctl(ctrl, buff);
	  	// post-process here
 		return res;

	   case MMC :
		// pre-process here
    		result = MMC_diskIoctl(ctrl, buff);
		// post-process here
		  return res;

	  case USB :
		// pre-process here
		result = USB_diskIoctl(ctrl, buff);
		// post-process here
  	return res;
	}

	return RES_PARERR;
}

