//code by luobing 2013-5-15 10:20:14
#ifndef _WINDOW_H
#define _WINDOW_H	
#include "Common.h"


//***************************  Macro **********************************************
//#define SYGraphicsMode  0x2         //800X600
#define SY_SCREEN_WIDTH   1024 
#define SY_SCREEN_HEIGHT  768

VOID InSYGraphicsMode(void);
VOID OutSYGraphicsMode(void);
VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);


#endif