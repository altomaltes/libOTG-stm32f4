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
 * @file usbd_msc_data.c
 *
 * original draft from MCD Application Team
 */


#include "usbd_msc_core.h"
#include "usbd_msc_data.h"

typedef struct
{ byte data_len;
  byte medium_type;
  byte reserved : 7; byte write_protected : 1;
  byte block_descriptor_len;
} scsi_mode_sense6_resp_t;

typedef struct PACKED
{ byte peripheral_device_type     : 5;
  byte peripheral_qualifier       : 3; // 0

  byte                            : 7; // 1
  byte is_removable               : 1;

  byte version;                         // 2

  byte response_data_format       : 4;  // 3
  byte hierarchical_support       : 1;
  byte normal_aca                 : 1;
  byte                            : 2;

  byte additional_length;               // 4

  byte protect                    : 1;  // 5
  byte                            : 2;
  byte third_party_copy           : 1;
  byte target_port_group_support  : 2;
  byte access_control_coordinator : 1;
  byte scc_support                : 1;

  byte addr16                     : 1; // 6
  byte                            : 3;
  byte multi_port                 : 1;
  byte                            : 1; // vendor specific
  byte enclosure_service          : 1;
  byte                            : 1;

  byte                            : 1; // 7 vendor specific
  byte cmd_que                    : 1;
  byte                            : 2;
  byte sync                       : 1;
  byte wbus16                     : 1;
  byte                            : 2;

  byte vendor_id[  8  ]; ///< 8 bytes of ASCII data identifying the vendor of the product.
  byte product_id[ 16 ]; ///< 16 bytes of ASCII data defined by the vendor.
  byte product_rev[ 4 ]; ///< 4 bytes of ASCII data defined by the vendor.
} scsi_inquiry_resp_t;


/* USB Mass storage Page 0 Inquiry Data
 */
const byte  MSC_Page00_Inquiry_Data[]=  //7
{ 0x00  /* Qualifier 3, device type 5 */
, 0x01                        /* Is removable RMB 1, reserverd 6*/
,	0x00                        /* AERC, obsolete,NORM ACA, HISUP, Response format */
,	(LENGTH_INQUIRY_PAGE00 - 4) /* Aditional length - 4 */
,	0x00                         /* SCCS, reserved */
,	0x80    /* BQE.ENCSERV,VS,MULTIP,*/
,	0x83    /* RELADR ,,, CMDQE VS */
};

/* USB Mass storage sense 6  Data
 */
const byte  MSC_Mode_Sense6_data[] = /* From USB stick */
{ 0x0B
,	0x00
,	0x00    /* 0x80 -> write protect */
,	0x08
};

/* USB Mass storage sense 10  Data
*/
const byte  MSC_Mode_Sense10_data[] =
{ 0x00,	0x06,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00 };
