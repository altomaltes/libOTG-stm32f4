/**
  ******************************************************************************
  * @file    usbd_msc_bot.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   Header for the usbd_msc_bot.c file
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
#ifndef __USBD_MSC_BOT_H
#define __USBD_MSC_BOT_H


#include "usbd_core.h"


enum
{ USBD_BOT_IDLE= 0      /* Idle state */
, USBD_BOT_DATA_OUT     /* Data Out state */
, USBD_BOT_DATA_IN      /* Data In state */
, USBD_BOT_LAST_DATA_IN /* Last Data In Last */
, USBD_BOT_SEND_DATA    /* Send Immediate data */
, USBD_BOT_NO_DATA      /* No data Stage */
};

#define USBD_BOT_CBW_SIGNATURE  0x43425355
#define USBD_BOT_CSW_SIGNATURE  0x53425355
#define USBD_BOT_CBW_LENGTH     31
#define USBD_BOT_CSW_LENGTH     13
#define USBD_BOT_MAX_DATA       256

/* CSW Status Definitions
 */
#define USBD_CSW_CMD_PASSED   0x00
#define USBD_CSW_CMD_FAILED   0x01
#define USBD_CSW_PHASE_ERROR  0x02

/* BOT Status */
#define USBD_BOT_STATUS_NORMAL             0
#define USBD_BOT_STATUS_RECOVERY           1
#define USBD_BOT_STATUS_ERROR              2


#define USBD_DIR_IN                        0
#define USBD_DIR_OUT                       1
#define USBD_BOTH_DIR                      2


void MSC_BOT_Init  ( void );
void MSC_BOT_Reset ( void );
void MSC_BOT_DeInit( void );
void MSC_BOT_DataIn( byte epnum );

void MSC_BOT_DataOut      ( byte epnum );
void MSC_BOT_SendCSW      ( byte CSW_Status );
void MSC_BOT_CplClrFeature( byte epnum );

#endif /* __USBD_MSC_BOT_H */
