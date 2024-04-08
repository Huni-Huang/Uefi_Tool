//Font.h:
//Code by luobing 2013-5-16 15:47:30
#ifndef _FONT_H
#define _FONT_H

#include "Common.h"

//*************************** Font Struct and Macro **************************************
#define   CHARWIDTH   8				//Width of ascii code
#define   CHARHEIGHT  16			//Height of ascii code
#define   HZWIDTH     16			//Width of chinese word
#define   HZHEIGHT    16			//height of chinese word

struct  hzk16_typ{         /*  Chinese mode struct  */
  UINT16 code;
  UINT16 array[16];
};

struct  ascii_typ{        /* ASCII code struct */
  UINT8 code;
  UINT16 array[16];
};
//*************************** Function Declaration ***************************************
VOID  putASCII(UINTN x,UINTN y,UINT8 code,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color,UINT16 mag);
VOID  putHZ16(UINTN x,UINTN y,UINT16 code,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color,UINT16 mag);
VOID  putStr(UINTN x,UINTN y,UINT8 *s,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color,UINT16 mag);


#endif