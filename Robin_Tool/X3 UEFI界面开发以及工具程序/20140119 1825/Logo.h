//Logo.h:
//Code by luobing 2013-11-27 14:14:25
#ifndef LOGO_H
#define LOGO_H

#include "Common.h"

//*************************** Logo Macro **************************************
#define LOGO_WIDTH 24					//lenovo logo widht
#define LOGO_HEIGHT 24					//lenovo logo height
#define LOGO_MASK_COLOR 0xfc				//lenovo logo mask color

//*************************** Function Declaration ***************************************
VOID ShowLogo(UINTN x,UINTN y,UINTN Logo_Width,UINTN Logo_Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *LogoColorTable,
              UINT8 *LogoPicture,
              UINT8 MaskColor);
      
        
#endif