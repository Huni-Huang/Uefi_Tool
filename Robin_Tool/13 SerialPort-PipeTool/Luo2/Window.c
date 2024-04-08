//code by luobing 2013-5-15 10:19:35

#include "Window.h"
#include "Graphic.h"

UINT32 SYGraphicsMode;
UINT32 OldGraphicsMode;
UINT32 SYPixPerScanLine;

//============================== Global color table ================================
EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[]={
  0,    0,    0,    0,        /*  BLACK */\
  255,  255,  255,  0,        /*  WHITE */\
  200,  208,  212,  0,        /*  LIGHT GRAY  */\
  167,  95, 18, 0,        /*  DEEP BLUE */\
  64, 64, 64, 0,        /*  DARK BLACK  */\
  128,  128,  128,  0,        /*  LIGHT BLACK */\
  0,    255,  255,  0,        /*  YELLOW  */\
  0,    153,  0,    0,        /*  GREEN */\
  0,    0,    153,  0,        /*  RED   */\
  192,  0,    0,    0,        /* BLUE   */  
};

//Function Name: SetBKG
//Input: color
//Output: None
//Description: set background
VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  rectblock(0,0,SY_SCREEN_WIDTH-1,SY_SCREEN_HEIGHT-1,color);
}
//Function Name: InSYGraphicsMode
//Input
//Output:
//Description: set mode
VOID InSYGraphicsMode(void)
{
	GetGraphicModeNumber(SY_SCREEN_WIDTH,SY_SCREEN_HEIGHT,&SYGraphicsMode,&OldGraphicsMode,&SYPixPerScanLine);
	SetMyMode(SYGraphicsMode);
}
//Function Name: OutSYGraphicsMode
//Input
//Output:
//Description: set old mode
VOID OutSYGraphicsMode(void)
{
	//GetGraphicModeNumber(SY_SCREEN_WIDTH,SY_SCREEN_HEIGHT,&SYGraphicsMode,&OldGraphicsMode,&SYPixPerScanLine);
	SetMyMode(OldGraphicsMode);
}