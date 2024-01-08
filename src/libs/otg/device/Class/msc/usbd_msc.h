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

#ifdef __cplusplus
 extern "C" {
#endif

#include  "./usbd_msc_bot.h"
#include  "./usbd_msc_scsi.h"

#include  "usbd_mass.h"

#define MSC_MAX_FS_PACKET         0x40
#define MSC_MAX_HS_PACKET123     0x200

#define BOT_GET_MAX_LUN           0xFE
#define BOT_RESET                 0xFF
#define USB_MSC_CONFIG_DESC_SIZ   32

#define MSC_EPIN_ADDR             0x81
#define MSC_EPOUT_ADDR            0x01

#define MSC_MEDIA_PACKET 0x200

typedef struct
{ byte (* Init             ) ( byte lun);
  byte (* GetCapacity      ) ( byte lun, dword *block_num, word *block_size);
  byte (* IsReady          ) ( byte lun);
  byte (* IsWriteProtected ) ( byte lun);
  byte (* Read             ) ( byte lun, byte *buf, dword blk_addr, word blk_len);
  byte (* Write            ) ( byte lun, byte *buf, dword blk_addr, word blk_len);
  byte (* GetMaxLun        ) ( void );

  byte *pInquiry;

} USBD_StorageTypeDef;

extern USBD_StorageTypeDef stor;

typedef struct
{ dword max_lun;
  dword interface;
  byte  bot_state;
  byte  bot_status;

  MScmdBlkWrapperRec cbw;
  MScmdStatWrapperRec csw;

  USBD_SCSI_SenseTypeDef scsi_sense[ SENSE_LIST_DEEPTH ];
  byte scsi_sense_head; byte scsi_sense_tail;

  word  scsi_blk_size;
  dword scsi_blk_nbr;

  dword scsi_blk_addr;
  dword scsi_blk_len;

  word  bot_data_length;
  byte  bot_data[ 1024 ];  // MSC_MEDIA_PACKET

}
USBD_MSC_BOT_HandleTypeDef;

extern USBD_MSC_BOT_HandleTypeDef hmsc;


#endif  /* __USBD_MSC_H */
