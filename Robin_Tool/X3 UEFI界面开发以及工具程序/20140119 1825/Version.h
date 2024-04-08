//Version.h
//Code by luobing 2011/9/8 17:51:32
#ifndef VERSION_H
#define VERSION_H  
   
#include "Common.h"
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];

#define SY_MainBoard         06
#define SY_MainVer           01
#define SY_SubVer            01
#define SY_OEMID             04

//*************************** Function Declaration ***************************************
BOOLEAN SYVersionMenu(VOID);
VOID DisplayVersion(VOID);
BOOLEAN DisplayCardVersion(VOID);

#endif