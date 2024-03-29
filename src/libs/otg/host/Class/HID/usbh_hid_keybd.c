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
 * @file usbh_hid_keybd.c
 *
 * original draft from MCD Application Team
 */

#include "usbh_hid_keybd.h"


static void  KEYBRD_Init (void);
static void  KEYBRD_Decode(byte *data);


static  const  byte  HID_KEYBRD_Codes[]=
{   0,   0,   0,   0,  31,   50,   48,   33
,  19,  34,  35,  36,  24,   37,   38,   39       /* 0x00 - 0x0F */
,  52,   51, 25,  26,  17,   20,   32,   21
,  23,  49,  18,  47,  22,   46,    2,    3       /* 0x10 - 0x1F */
,   4,   5,   6,   7,   8,    9,   10,   11
,  43, 110,  15,  16,  61,   12,   13,   27       /* 0x20 - 0x2F */
,  28,  29,  42,  40,  41,    1,   53,   54
,  55,  30, 112, 113, 114,  115,  116,  117       /* 0x30 - 0x3F */
, 118, 119, 120, 121, 122,  123,  124,  125
, 126,  75,  80,  85,  76,   81,   86,   89       /* 0x40 - 0x4F */
,  79,  84,  83,  90,  95,  100,  105,  106
, 108,  93,  98, 103,  92,   97,  102,   91       /* 0x50 - 0x5F */
,  96, 101,  99, 104,  45,  129,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0x60 - 0x6F */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0x70 - 0x7F */
,   0,   0,   0,   0,   0,  107,    0,   56
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0x80 - 0x8F */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0x90 - 0x9F */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0xA0 - 0xAF */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0xB0 - 0xBF */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0xC0 - 0xCF */
,   0,   0,   0,   0,   0,    0,    0,    0
,   0,   0,   0,   0,   0,    0,    0,    0       /* 0xD0 - 0xDF */
,  58,  44,  60, 127,  64,   57,   62,  128        /* 0xE0 - 0xE7 */
};

#ifdef QWERTY_KEYBOARD
static  const  byte  HID_KEYBRD_Key[]=
{ '\0',  '`',  '1',  '2',  '3',  '4',  '5',  '6'
,  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\r'
,  '\t',  'q',  'w',  'e',  'r',  't',  'y',  'u'
,  'i',  'o',  'p',  '[',  ']',  '\\'
,  '\0',  'a',  's',  'd',  'f',  'g',  'h',  'j'
,  'k',  'l',  ';',  '\'', '\0', '\n'
,  '\0',  '\0', 'z',  'x',  'c',  'v',  'b',  'n'
,  'm',  ',',  '.',  '/',  '\0', '\0'
,  '\0',  '\0', '\0', ' ',  '\0', '\0', '\0', '\0'
,  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
,  '\0',  '\0', '\0', '\0', '\0', '\r', '\0', '\0'
,  '\0', '\0', '\0', '\0', '\0', '\0'
,  '\0',  '\0', '7',  '4',  '1'
,  '\0',  '/',  '8',  '5',  '2'
,  '0',   '*',  '9',  '6',  '3'
,  '.',   '-',  '+',  '\0', '\n', '\0', '\0', '\0', '\0', '\0', '\0'
,  '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
,  '\0', '\0', '\0', '\0'
};

static  const  byte  HID_KEYBRD_ShiftKey[] =
{ '\0',  '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')'
  ,  '_',  '+', '\0', '\0', '\0',  'Q',  'W',  'E',  'R',  'T',  'Y',  'U'
  ,  'I',  'O',  'P',  '{',  '}',  '|',  '\0', 'A',  'S',  'D',  'F',  'G'
  ,  'H',  'J',  'K',  'L',  ':',  '"',  '\0', '\n', '\0', '\0', 'Z',  'X'
  ,  'C',  'V',  'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0',  '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0',    '\0', '\0', '\0', '\0', '\0'
  , '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};

#else

static  const  byte  HID_KEYBRD_Key[] =
{ '\0',  '`',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0'
,  '-',  '=',  '\0', '\r', '\t',  'a',  'z',  'e',  'r',  't',  'y',  'u'
,  'i',  'o',  'p',  '[',  ']', '\\', '\0',  'q',  's',  'd',  'f',  'g'
,  'h',  'j',  'k',  'l',  'm',  '\0', '\0', '\n', '\0',  '\0', 'w',  'x'
,  'c',  'v',  'b',  'n',  ',',  ';',  ':',  '!',  '\0', '\0', '\0',  '\0'
, '\0', ' ',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0',  '\0', '\0', '\0', '\0', '\r', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0',  '\0', '7',  '4',  '1','\0',  '/'
,  '8',  '5',  '2', '0',   '*',  '9',  '6',  '3', '.',   '-',  '+',  '\0'
,  '\n', '\0', '\0', '\0', '\0', '\0', '\0','\0',  '\0', '\0', '\0', '\0', '\0'
,  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};

static  const  byte  HID_KEYBRD_ShiftKey[] =
{ '\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_'
, '+',  '\0', '\0', '\0', 'A',  'Z',  'E',  'R',  'T',  'Y',  'U',  'I',  'O'
, 'P',  '{',  '}',  '*', '\0', 'Q',  'S',  'D',  'F',  'G',  'H',  'J',  'K'
, 'L',  'M',  '%',  '\0', '\n', '\0', '\0', 'W',  'X',  'C',  'V',  'B',  'N'
, '?',  '.',  '/',  '\0',  '\0', '\0','\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
, '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};
#endif


/**
* @brief  KEYBRD_Init.
*         Initialize the keyboard function.
* @param  None
* @retval None
*/
static void  KEYBRD_Init (void)
{ USR_KEYBRD_Init(); /* Call User Init*/
}

/**
* @brief  KEYBRD_ProcessData.
*         The function is to decode the pressed keys.
* @param  pbuf : Pointer to the HID IN report data buffer
* @retval None
*/

static void KEYBRD_Decode( byte * pbuf )
{ static  byte shift;
  static  byte keys[      KBR_MAX_NBR_PRESSED ];
  static  byte keys_new[  KBR_MAX_NBR_PRESSED ];
  static  byte keys_last[ KBR_MAX_NBR_PRESSED ];
  static  byte key_newest;
  static  byte nbr_keys;
  static  byte nbr_keys_new;
  static  byte  nbr_keys_last;
  byte   ix;
  byte   jx;
  byte   error;
  byte   output;

  nbr_keys      = 0;
  nbr_keys_new  = 0;
  nbr_keys_last = 0;
  key_newest    = 0x00;

  /* Check if Shift key is pressed */
  if ((  pbuf[ 0 ] == KBD_LEFT_SHIFT  )
     || (pbuf[ 0 ] == KBD_RIGHT_SHIFT ))
  { shift = TRUE;
  }
  else
  { shift = FALSE;
  }

  error = FALSE;

/* Check for the value of pressed key
 */
  for ( ix = 2
      ; ix < 2 + KBR_MAX_NBR_PRESSED
      ; ix++ )
  { if (( pbuf[ ix ] == 0x01 )
    ||  ( pbuf[ ix ] == 0x02 )
    ||  ( pbuf[ ix ] == 0x03 ))
    { error = TRUE;
  } }

  if ( error == TRUE )
  { return;
  }

  nbr_keys     = 0;
  nbr_keys_new = 0;

  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++)
  { if (pbuf[ix] != 0)
    { keys[nbr_keys] = pbuf[ix];
      nbr_keys++;
      for (jx = 0; jx < nbr_keys_last; jx++)
      { if (pbuf[ix] == keys_last[jx])
        { break;
      } }

      if (jx == nbr_keys_last)
      { keys_new[nbr_keys_new] = pbuf[ix];
        nbr_keys_new++;
  } } }

  if (nbr_keys_new == 1)
  { key_newest = keys_new[0];

    if (shift == TRUE)
    { output =  HID_KEYBRD_ShiftKey[HID_KEYBRD_Codes[key_newest]];
    }
    else
    { output =  HID_KEYBRD_Key[HID_KEYBRD_Codes[key_newest]];
    }

    USR_KEYBRD_ProcessData(output); /* call user process handle */
  }
  else
  { key_newest = 0x00;
  }

  nbr_keys_last  = nbr_keys;

  for( ix = 0
     ; ix < KBR_MAX_NBR_PRESSED
     ; ix++ )
  { keys_last[ix] = keys[ix];
} }



HID_cb_TypeDef HID_KEYBRD_cb=
{ KEYBRD_Init
, KEYBRD_Decode
};

