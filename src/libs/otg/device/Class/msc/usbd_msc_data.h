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
 * @file usbd_msc_data.h
 *
 * original draft from MCD Application Team
 */

#ifndef _USBD_MSC_DATA_H_
#define _USBD_MSC_DATA_H_


#define MODE_SENSE6_LEN		       	 8
#define MODE_SENSE10_LEN       		 8
#define LENGTH_INQUIRY_PAGE00		   7
#define LENGTH_FORMAT_CAPACITIES	20

extern const byte MSC_Page00_Inquiry_Data[];
extern const byte MSC_Mode_Sense6_data[];
extern const byte MSC_Mode_Sense10_data[] ;

#endif

