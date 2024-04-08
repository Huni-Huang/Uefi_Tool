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

//Add for UTF-8 Chinese char robin 2020-0522 1229
//FONT
typedef struct struct_lattice
{
	UINT32			utf8_code;
	UINT8			width;
	UINT8 *	pixel_gray_array;
} LATTICE;
typedef struct struct_font_info
{
	UINT8	height;
	UINT32	count;
	LATTICE*		lattice_array;
} FONT_INFO;

extern FONT_INFO KaiTi_33B;

LATTICE *get_lattice(FONT_INFO *font, UINT32 utf8_code);
VOID draw_lattice(UINTN x, UINTN y, UINT8 width, UINT8 height, UINT8 *p_data, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *font_color);
UINT8 draw_single_char(UINT32 utf8_code, UINTN x, UINTN y, FONT_INFO *font, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *font_color);
VOID draw_string(UINT8 *str, UINTN x, UINTN y, FONT_INFO *font, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *font_color);
#endif