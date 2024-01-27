/**
  ******************************************************************************
  * @file    usbd_msc.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   Header for the usbd_msc.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MSC_H
#define __USBD_MSC_H

#include  "usb-glue.h"
#include  "usbd_core.h"
#include  "usbd_mass.h"

#define MSC_MAX_FS_PACKET      0x40
#define MSC_MAX_HS_PACKET123  0x200

#define BOT_GET_MAX_LUN        0xFE
#define BOT_RESET               0xFF

#define MSC_MEDIA_PACKET 0x200
#define SENSE_LIST_DEEPTH          4


struct  PACKED USBD_SCSI_SenseTypeDef
{ char Skey;
  union
  { struct _ASCs
    { char ASC;
      char ASCQ;
    } b;
    unsigned int	ASC;
    char *pData;
  } w;
};


typedef struct
{ const USBDclassDefREC * driver;

  short (* Ioctl ) ( dword lun, ... ); /* Must be first */
  short (* Read  ) ( dword lun,       void * buf, dword blk_addr, dword blk_len );
  short (* Write ) ( dword lun, const void * buf, dword blk_addr, dword blk_len );

  byte epIn;
  byte epOt;

  dword interface;
  byte  max_lun;
  byte  bot_state;
  byte  bot_status;
  byte  alingn;

  MScmdBlkWrapperRec  cbw;
  MScmdStatWrapperRec csw;

  struct USBD_SCSI_SenseTypeDef scsi_sense[ SENSE_LIST_DEEPTH ];
  byte scsi_sense_head; byte scsi_sense_tail;

  dword scsi_blk_size;
  dword scsi_blk_nbr;

  dword scsi_blk_addr;
  dword scsi_blk_len;

  word  bot_data_length;
  byte  bot_data[ MSC_MEDIA_PACKET ];  // MSC_MEDIA_PACKET

} MSCdriverRec;

#endif  /* __USBD_MSC_H */
