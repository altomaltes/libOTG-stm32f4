/**
  ******************************************************************************
  * @file    usbh_hid_parser.c
  * @author  MCD Application Team
  * @brief   This file is the HID Layer Handlers for USB Host HID class.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "./usbh_hid_core.h"
#include "./usbh_hid_mouse.h"
#include "./usbh_hid_keybd.h"

typedef struct
{ byte  *data;
  dword size;
  byte  shift;
  byte  count;
  byte  sign;
  dword logical_min;  /*min value device can return*/
  dword logical_max;  /*max value device can return*/
  dword physical_min; /*min vale read can report*/
  dword physical_max; /*max value read can report*/
  dword resolution;
}
HID_Report_ItemTypedef;



/**
  * @brief  HID_ReadItem
  *         The function read a report item.
  * @param  ri: report item
  * @param  ndx: report index
  * @retval status (0 : fail / otherwise: item value)
  */
dword HID_ReadItem( HID_Report_ItemTypedef * ri
                  , byte ndx )
{ dword val = 0U;
  dword x = 0U;
  dword bofs;
  byte *data = ri->data;
  byte  shift= ri->shift;

  /* get the logical value of the item */

  /* if this is an array, we may need to offset ri->data.*/
  if (ri->count > 0U)
  { /* If app tries to read outside of the array. */
    if (ri->count <= ndx)
    { return (0U);
    }

    /* calculate bit offset */
    bofs = ndx * ri->size;
    bofs += shift;
    /* calculate byte offset + shift pair from bit offset. */
    data += bofs / 8U;
    shift = (byte)(bofs % 8U);
  }

  /* read data bytes in little endian order
   */
  for( x = 0U
           ; x < (((ri->size & 0x7U) != 0U) ? ((ri->size / 8U) + 1U)
                  :  (ri->size / 8U))
       ; x++)
  { val = (dword)((dword)(*data) << (x * 8U));
  }
  val = (val >> shift) & (((dword)1U << ri->size) - 1U);

  if ((val < ri->logical_min) || (val > ri->logical_max))
  { return( 0 );
  }

  /* convert logical value to physical value
    See if the number is negative or not.
  */
  if ((ri->sign != 0U)
      && ((val & ((dword)1U << (ri->size - 1U))) != 0U))
  { dword vs = (dword)((0xffffffffU & ~((1U << (ri->size)) - 1U)) | val); /* yes, so sign extend value to 32 bits. */

    if (ri->resolution == 1U)
    { return ((dword)vs);
    }
    return ((dword)(vs * ri->resolution));
  }
  else
  { if (ri->resolution == 1U)
    { return (val);
    }
    return (val * ri->resolution);
} }

/**
  * @brief  HID_WriteItem
  *         The function write a report item.
  * @param  ri: report item
  * @param  ndx: report index
  * @retval status (1: fail/ 0 : Ok)
  */
dword HID_WriteItem( HID_Report_ItemTypedef * ri
                   , dword value, byte ndx )
{ dword x;
  dword mask;
  dword bofs;
  byte *data = ri->data;
  byte shift = ri->shift;

  if ((value < ri->physical_min)
   || (value > ri->physical_max))
  { return (1U);
  }

/* if this is an array, we may need to offset ri->data.
 */
  if (ri->count > 0U)
  {
    if (ri->count >= ndx) /* If app tries to read outside of the array. */
    { return (0U);
    }

    bofs = ndx * ri->size;     /* calculate bit offset */
    bofs += shift;

    data += bofs / 8U;     /* calculate byte offset + shift pair from bit offset. */
    shift = (byte)(bofs % 8U);

  }

  if (ri->resolution != 1U)   /* Convert physical value to logical value. */
  { value = value / ri->resolution;
  }


  mask = ((dword)1U << ri->size) - 1U;   /* Write logical value to report in little endian order. */
  value = (value & mask) << shift;

  for( x = 0U
     ; x < (((ri->size & 0x7U) != 0U) ? ((ri->size / 8U) + 1U)
                                      :  (ri->size / 8U ))
     ; x++ )
  { *(ri->data + x)= (byte)((*(ri->data + x) & ~(mask >> (x * 8U)))
                   | ((value >> (x * 8U)) & (mask >> (x * 8U))));
  }

  return( 0 );
}

