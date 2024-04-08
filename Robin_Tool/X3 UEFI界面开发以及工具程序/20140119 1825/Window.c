//Windows.c: Draw Window and button
//Code by luobing 2013-11-27 14:10:29

#include "Graphic.h"
#include "Font.h"
#include "Logo.h"
#include "Window.h"

extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYLogoColorTable[]; //defined in logo.c
extern UINT8 gSYLogoPicture[]; //defined in logo.c
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
//String of lenovo caption:ÁªÏëË«Íø¸ôÀë¼ÆËã»ú
//UINT8 str_lenovo_cap[]={0xc1,0xaa,0xcf,0xeb,0xcb,0xab,0xcd,0xf8,0xb8,0xf4,0xc0,0xeb,0xbc,0xc6,0xcb,0xe3,0xbb,0xfa,0x00};
//ÉñÒ×Ë«Íø¸ôÀë¿¨ V7.0(for UEFI)
UINT8 str_senypc_cap[]={0xc9,0xf1,0xd2,0xd7,0xcb,0xab,0xcd,0xf8,0xb8,0xf4,0xc0,0xeb,0xbf,0xa8,0x20,0x56,0x37,0x2e,0x30,0x28,0x66,0x6f,0x72,0x20,0x55,0x45,0x46,0x49,0x29,0x00};
//========================= Frame function =========================================
//Function Name: PutLenovoFrame
//Input: color
//Output: None
//Description: set background
VOID PutLenovoFrame(VOID)
{
//	 PutWindow(WND_FRAME_LEFT,WND_FRAME_TOP,WND_FRAME_RIGHT,WND_FRAME_BOTTOM,&(gColorTable[WND_BG_Color]),str_lenovo_cap,&(gColorTable[WND_CapStr_Color]));
	 PutWindow(WND_FRAME_LEFT,WND_FRAME_TOP,WND_FRAME_RIGHT,WND_FRAME_BOTTOM,&(gColorTable[WND_BG_Color]),str_senypc_cap,&(gColorTable[WND_CapStr_Color]));
}

//Function Name: SetBKG
//Input: color
//Output: None
//Description: set background
VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  rectblock(0,0,SY_SCREEN_WIDTH-1,SY_SCREEN_HEIGHT-1,color);
}

//Function Name: PutButton
//Input: x,y,str=display str,str_color
//Output: None
//Description: 
VOID PutButton(UINTN x,UINTN y,UINT8 *str,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *str_color)
{
	UINTN length,x1,y1;

	
	for(length=0;length<255;length++)		//max str =255
	{
		if(str[length]==0x00)
			break;
	}
	//length<<=3;					//width of ascii is 8
	x1=x+((length+4)<<3)-1;
	y1=y+DEFAULTBUTTONHEIGHT-1;
	
	HLine(x,x1,y,&(gColorTable[BUT_Light_Color]));
	VLine(x,y,y1,&(gColorTable[BUT_Light_Color]));
	HLine(x+1,x1-1,y1-1,&(gColorTable[BUT_LShadow_Color]));
	HLine(x,x1,y1,&(gColorTable[BUT_DShadow_Color]));
	VLine(x1-1,y+1,y1-1,&(gColorTable[BUT_LShadow_Color]));
	VLine(x1,y,y1,&(gColorTable[BUT_DShadow_Color]));
	putStr(x+((x1-x-(length<<3))>>1),y+((y1-y-CHARWIDTH*2)>>1),str,str_color,1);
}

//Function Name: PutWindow
//Input: x,y,str=display str,str_color
//Output: None
//Description: 
VOID	PutWindow(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *wndColor,
								UINT8 *pCapStr,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *capStrColor)
{

	rectblock(x1,y1,x2,y2,wndColor);

	HLine(x1,x2,y1,&(gColorTable[WND_Light_Color]));
	VLine(x1,y1,y2,&(gColorTable[WND_Light_Color]));
	HLine(x1+1,x2-1,y2-1,&(gColorTable[WND_LShadow_Color]));
	HLine(x1,x2,y2,&(gColorTable[WND_DShadow_Color]));
	VLine(x2-1,y1+1,y2-1,&(gColorTable[WND_LShadow_Color]));
	VLine(x2,y1,y2,&(gColorTable[WND_DShadow_Color]));
	rectblock(x1+4,y1 +3,x2-4,y1+DEFAULTCAPTIONHEIGHT+2-1,&(gColorTable[WND_Caption_Color]));
	
	ShowLogo(x1+4+2,y1+3,LOGO_WIDTH,LOGO_HEIGHT,gSYLogoColorTable,gSYLogoPicture,LOGO_MASK_COLOR);
	putStr(x1+8+2+LOGO_WIDTH,y1+6,pCapStr,capStrColor,1);
}

