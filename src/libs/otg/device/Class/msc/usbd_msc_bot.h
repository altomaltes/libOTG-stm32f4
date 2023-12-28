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
 * @file usbd_msc_bot.h
 *
 * original draft from MCD Application Team
 */

#include "usbd_core.h"

#ifndef __USBD_MSC_BOT_H
#define __USBD_MSC_BOT_H

#define BOT_IDLE          0       /* Idle state */
#define BOT_DATA_OUT      1       /* Data Out state */
#define BOT_DATA_IN       2       /* Data In state */
#define BOT_LAST_DATA_IN  3       /* Last Data In Last */
#define BOT_SEND_DATA     4       /* Send Immediate data */

#define BOT_CBW_SIGNATURE 0x43425355
#define BOT_CSW_SIGNATURE 0x53425355
#define BOT_CBW_LENGTH            31
#define BOT_CSW_LENGTH            13

/* CSW Status Definitions
 */
#define CSW_CMD_PASSED  0x00
#define CSW_CMD_FAILED  0x01
#define CSW_PHASE_ERROR 0x02

/* BOT Status
 */
#define BOT_STATE_NORMAL   0
#define BOT_STATE_RECOVERY 1
#define BOT_STATE_ERROR    2


#define DIR_IN             0
#define DIR_OUT            1
#define BOTH_DIR           2

typedef struct PACKED
{ dword dSignature;
  dword dTag;
  dword dDataLength;
  byte  bmFlags;
  byte  bLUN;
  byte  bCBLength;
  byte  CB[16];
}
MSC_BOT_CBW_TypeDef;


typedef struct PACKED
{ dword dSignature;
  dword dTag;
  dword dDataResidue;
  byte  bStatus;
}
MSC_BOT_CSW_TypeDef;


extern byte                MSC_BOT_Data[];
extern word                MSC_BOT_DataLen;
extern byte                MSC_BOT_State;
extern byte                MSC_BOT_BurstMode;
extern MSC_BOT_CBW_TypeDef MSC_BOT_cbw;
extern MSC_BOT_CSW_TypeDef MSC_BOT_csw;


void MSC_BOT_Init   ();
void MSC_BOT_Reset  ();
void MSC_BOT_DeInit ();
schar MSC_BOT_DataIn ( byte epnum );
schar MSC_BOT_DataOut( byte epnum );

void MSC_BOT_SendCSW (       byte CSW_Status );
void MSC_BOT_CplClrFeature( byte epnum);

#endif
