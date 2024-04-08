//Window.h:
//Code by luobing 2011/9/8 13:59:59
#ifndef WINDOW_H
#define WINDOW_H
#include "Common.h"

extern UINT32 HorRes;
extern UINT32 VerRes;
//***************************  Macro **********************************************
//#define SYGraphicsMode  0x2         //800X600
#define SY_SCREEN_WIDTH   800
#define SY_SCREEN_HEIGHT  600
//#define SY_SCREEN_WIDTH   1024//800
//#define SY_SCREEN_HEIGHT  768//600

#define DEFAULTBUTTONHEIGHT 		2*CHARHEIGHT        //2*CHARHEIGHT
#define DEFAULTCAPTIONHEIGHT   26      //amend for show logo 2005-7-13 luobing
#define DEFAULTEDITBOXHEIGHT   24      //DEFAULTEDITBOXHEIGHT=DEFAULTBUTTONHEIGHT-CHARHEIGHT/2

#define BKG_COLOR			DEEPBLUE				//background color
#define BUT_Light_Color    WHITE                   //��ť����Դ������ɫ
#define BUT_LShadow_Color 	 LIGHTBLACK              //Զ���Դ����Ӱ��ɫ
#define BUT_DShadow_Color 	 DARKBLACK               //Զ���Դ����Ӱ��ɫ
//#define BUTPRES_Light_Color   	 DARKBLACK           //��ť����Դ������ɫ
//#define BUTPRES_LShadow_Color 	 WHITE               //Զ���Դ����Ӱ��ɫ
//#define BUTPRES_DShadow_Color 	 LIGHTBLACK          //Զ���Դ����Ӱ��ɫ
	#define WND_BG_Color       LIGHTGRAY             //���屳��ɫ
//  #define WND_Caption_Color  BLACK                 //�����������ɫ
	#define WND_Caption_Color  BLUE                 //�����������ɫ
  #define WND_CapStr_Color   WHITE                 //����������ַ�����ɫ
  #define WND_WaiStr_Color   RED                   //����ѡ��ť�ַ�����ɫ
  #define WND_NeiStr_Color   GREEN                 //����ѡ��ť�ַ�����ɫ
  #define WND_Light_Color    WHITE                 //���尴ť����Դ������ɫ
  #define WND_LShadow_Color  LIGHTBLACK            //����Զ���Դ����Ӱ��ɫ
  #define WND_DShadow_Color  DARKBLACK             //����Զ���Դ����Ӱ��ɫ


#define     BLACK         0
#define     WHITE         (BLACK    +   1)
#define     LIGHTGRAY     (WHITE    + 1)
#define     DEEPBLUE      (LIGHTGRAY  + 1)
#define     DARKBLACK     (DEEPBLUE + 1)
#define     LIGHTBLACK    (DARKBLACK  + 1)
#define     YELLOW        (LIGHTBLACK + 1)
#define     GREEN         (YELLOW   + 1)
#define     RED           (GREEN    + 1)
#define     BLUE          (RED    + 1)

#define WND_FRAME_LEFT   (SY_SCREEN_WIDTH/3-SY_SCREEN_WIDTH/20)			//Frame location
#define WND_FRAME_TOP    (SY_SCREEN_HEIGHT/3-SY_SCREEN_HEIGHT/20)
#define WND_FRAME_RIGHT  (SY_SCREEN_WIDTH-WND_FRAME_LEFT+SY_SCREEN_WIDTH/40)
#define WND_FRAME_BOTTOM (SY_SCREEN_HEIGHT-WND_FRAME_TOP-SY_SCREEN_HEIGHT/20)

#define TOOLTIP_LEFT WND_FRAME_LEFT+(WND_FRAME_RIGHT-WND_FRAME_LEFT-27*CHARWIDTH)*4/9
#define TOOLTIP_TOP WND_FRAME_TOP+(WND_FRAME_BOTTOM-WND_FRAME_TOP)/5
#define BUTTON_TOOLTIP_LEFT WND_FRAME_LEFT+(WND_FRAME_RIGHT-WND_FRAME_LEFT-10*CHARWIDTH)*4/9
#define BUTTON_TOOLTIP_TOP  WND_FRAME_TOP+(WND_FRAME_BOTTOM-WND_FRAME_TOP)/2
//*************************** Function Declaration ***************************************
VOID PutLenovoFrame(VOID);
VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
       
VOID PutButton(UINTN x,UINTN y,UINT8 *str,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *str_color);
VOID PutWindow(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *wndColor,
								UINT8 *pCapStr,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *capStrColor);

#endif