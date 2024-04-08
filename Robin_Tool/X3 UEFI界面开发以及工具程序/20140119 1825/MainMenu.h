//MainMenu.h

//Code by luobing 2011-09-09 0:19:49
#ifndef MAINMENU_H
#define MAINMENU_H

#include "Common.h"

extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];


#define STR_CNT_Color      WHITE
#define DEFAULTTIME        15

//#define MAIN_KEY_1   			0x00000031					//'1'
//#define MAIN_KEY_2   			0x00000032					//'2'
//#define MAIN_KEY_ENTER 		0x0000000d					//enter
//#define MAIN_KEY_TAB 			0x00000009					//tab
//#define MAIN_KEY_UP 			0x00000100					//up
//#define MAIN_KEY_DOWN 		0x00000200					//down
//#define MAIN_KEY_RALT_V 	0x00100076					//alt+v
//#define MAIN_KEY_LALT_V		0x00200076

#define MAIN_KEY_1   			0x00000031					//'1'
#define MAIN_KEY_2   			0x00000032					//'2'
#define MAIN_KEY_ENTER 		0x0000000d					//enter
#define MAIN_KEY_TAB 			0x00000009					//tab
#define MAIN_KEY_UP 			0x00010000					//up
#define MAIN_KEY_DOWN 		0x00020000					//down
#define MAIN_KEY_V 	0x00000076					//v

#define BUTTON_TOP_WAI   (WND_FRAME_BOTTOM - WND_FRAME_TOP)/3 + WND_FRAME_TOP - CHARWIDTH
#define BUTTON_LEFT_WAI  WND_FRAME_LEFT  + (WND_FRAME_RIGHT-WND_FRAME_LEFT)/2-8*CHARWIDTH
#define BUTTON_TOP_NEI   WND_FRAME_BOTTOM - (WND_FRAME_BOTTOM - WND_FRAME_TOP)/3 -2*CHARWIDTH 
#define BUTTON_LEFT_NEI  BUTTON_LEFT_WAI

//*************************** Function Declaration ***************************************
UINT8 DisplayMainMenu(VOID);
VOID DisplayNWSelect(UINTN x,UINTN y,UINT8 *str,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *selColor);
VOID DisplayCounterTime(UINTN x,UINTN y,UINT8 *str,UINT8 seconds,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID DisplayUserSel(UINT8 sel);
VOID main(VOID);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif