/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>  //gHiiString
#include <Library/MemoryAllocationLib.h> //AllocatZeroPool,FreePool

#include <Protocol/HiiFont.h>
#include "Protocol/HiiDatabase.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

//Global variables
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;
EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
//Functions declare
EFI_STATUS LocateSimpleTextInputEx(void);
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
EFI_STATUS GetKey(EFI_INPUT_KEY *key);
VOID FlushKeyBuffer(VOID);
VOID WaitKey(VOID);

EFI_STATUS LocateGraphicsOutput (void);
EFI_STATUS SwitchGraphicsMode(BOOLEAN flag);
VOID SetMyMode(UINT32 ModeNumber);
VOID putpixel(UINTN x,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);

VOID HLine(UINTN x1,UINTN x2,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID VLine(UINTN x,UINTN y1,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  rectblock(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  rectangle(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  circle(UINTN centerx,UINTN centery,UINTN radius,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);


EFI_STATUS DisplayModeInformation(void);
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber);
EFI_STATUS DisplayTextModeInformation(void);
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber);

EFI_STATUS TestLanguage(EFI_HANDLE HiiHandle);
EFI_STATUS TestString(EFI_HANDLE HiiHandle );

EFI_STATUS CreatesimpleFontPkg(EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes);

EFI_STATUS FillNarrowGLYPH(UINT8* p, 
        EFI_NARROW_GLYPH* NarrowGlyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 NrStart, CHAR16 NrCharNum);
EFI_STATUS FillWideGLYPH(UINT8* p, 
        EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 Start, CHAR16 CharNum);
EFI_STATUS FillGLYPH(UINT8* p, 
        UINT8* Glyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 Start, CHAR16 CharNum);        
EFI_STATUS CreateDMFontPkg(CHAR16* FontName, UINT16 FontWidth, UINT16 FontHeight,  
        EFI_NARROW_GLYPH* NarrowGlyph, UINT32 NrSizeInBytes, CHAR16 NrStart, CHAR16 NrCharNum,
        EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes, CHAR16 Start, CHAR16 CharNum);        

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

//SimpleFont
extern EFI_WIDE_GLYPH  gSimpleFontWideGlyphData[];
extern UINT32 gSimpleFontBytes;
static EFI_GUID gSimpleFontPackageListGuid = {0xf6643673, 0x6006, 0x3738, {0x5c, 0xcd, 0xda, 0x1a, 0xeb, 0x3b, 0x26, 0xa2}};

//Font
extern EFI_WIDE_GLYPH  gDMFontWideGlyphData[];
extern UINT32 gDMFontBytes;
extern EFI_NARROW_GLYPH  gUsStdNarrowGlyphData[]; 
extern UINT32 mNarrowFontSize;
static EFI_GUID gDMFontPackageListGuid = {0xf6645673, 0x6106, 0x3138, {0x5c, 0xcd, 0xda, 0x1a, 0xab, 0x3b, 0x26, 0xa2}};
#define WfontWidth  16
#define WfontHight  19
CONST UINT16 WideGlyphBytes = WfontHight * WfontWidth / 8;
CONST UINT16 NrGlyphBytes = 19;

/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int
main (
  IN int Argc,
  IN char **Argv
  )
{
	EFI_STATUS      Status;
//	UINT16 scanCode=0;
//	UINT16 uniChar=0;
//	UINT32 shiftState;
//	EFI_KEY_TOGGLE_STATE toggleState;
	UINT32 count=0;
	EFI_INPUT_KEY key={0,0};
	EFI_GUID   mStrPackageGuid = { 0xedd31def, 0xf262, 0xc24e, 0xa2, 0xe4, 0xde, 0xf7, 0xde, 0xcd, 0xcd, 0xee };
  EFI_HII_HANDLE    * handles = 0;		//for simpleFont of Hii
  EFI_HII_HANDLE    * handles1 = 0;   //for Font of Hii
	//首先注册字符串资源文件：将我们的字符串package加入到Hii数据库中
	//EFI_HANDLE HiiHandle = HiiAddPackages (&mStrPackageGuid ,  gImageHandle, exampleStrings, NULL);
	EFI_HANDLE HiiHandle = HiiAddPackages (&mStrPackageGuid ,  gImageHandle, Luo2Strings, NULL);
	//Add SimpleFont package to Hii database
	handles = HiiGetHiiHandles(&gSimpleFontPackageListGuid); 
	if(handles == 0)
	{
		Status = CreatesimpleFontPkg(gSimpleFontWideGlyphData, gSimpleFontBytes);
		gST->ConOut->OutputString(gST->ConOut,L"execute CreatesimpleFontPkg()  handles==0\n\r");
	}
	else
	{
		gST->ConOut->OutputString(gST->ConOut,L"execute CreatesimpleFontPkg()  handles==1\n\r");
		FreePool(handles);
	}
	
	//Add Font pacekage to Hii database
	handles1 = HiiGetHiiHandles(&gDMFontPackageListGuid); 
	if(handles1 == 0)
	{
		//extern CHAR16* FontName ;
		CHAR16 * FontName=(CHAR16*)L"LUOBING";  //随便取个名字
		Status = CreateDMFontPkg( FontName, 16, 19,  
															gUsStdNarrowGlyphData  , mNarrowFontSize, 32, (CHAR16)(mNarrowFontSize/sizeof(EFI_NARROW_GLYPH)),
                    					gSimpleFontWideGlyphData, gSimpleFontBytes, 0x4e00, (CHAR16)(gSimpleFontBytes/sizeof(EFI_WIDE_GLYPH))
                    				);
		gST->ConOut->OutputString(gST->ConOut,L"execute CreateDMFontPkg()  handles1==0\n\r");
	}
	else
	{
		gST->ConOut->OutputString(gST->ConOut,L"execute CreateDMFontPkg()  handles1==1\n\r");
		FreePool(handles1);
	}
	
	//lbdebug end
	
	
//	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Data: 2013-2-1 11:57:51\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
	//text out test
//	iStatus = gBS->AllocatePool(EfiBootServicesData,0x100,(void**)&buffer);
//	if (EFI_ERROR (iStatus)) {
//    return EFI_UNSUPPORTED;
//  }
//  SetMem(buffer,0x100,0x00);
//	j=13;
//	//swprintf (WName, (strlen (StringIdentifierName) + 1) * sizeof (WCHAR), L"%S", StringIdentifierName);
//	swprintf(buffer,99,L"Now is 20%d, the world is exist!\n",j);
//	gST->ConOut->OutputString(gST->ConOut,buffer);
//	gBS->FreePool(buffer);
	//测试键盘输入的例子，打印所有我需要的键盘信息
	gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"please input key(ESC to exit):\n\r");
//	ISTATUS=&gST->ConsoleInHandle->ReadKeyStrokeEx(&gST->ConsoleInHandle,&IKeyData);
//	//lbdebug begin
//	DisplayTextModeInformation();
	Status=LocateSimpleTextInputEx();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocateSimpleTextInputEx, Can't find protocol!\n");
	}
	else
	{
		Print(L"Call LocateSimpleTextInputEx, Find protocol!\n");
	}
	Status=LocateGraphicsOutput();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocateGraphicsOutput, Can't find graphics protocol!\n");
	}
	else
	{
		Print(L"Call LocateGraphicsOutput, Find graphics protocol!\n");
	}
//	{
////		//DisplayTextMode...
////		UINTN ModeIndex;
////		for(ModeIndex=0;ModeIndex<(UINTN)(gST->ConOut->Mode->MaxMode);ModeIndex++)
////		{
////			DisplaySpecifyTextModeMessage(ModeIndex);
////			WaitKey();
////		}
//	}
//	{
//		//DisplayModeInformation();
//		UINT32 ModeIndex;
//		for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
//		{
//			DisplaySpecifyModeMessage(ModeIndex);
//			WaitKey();
//		}
//	}
//	{
//		//图形显示测试
//		SwitchGraphicsMode(TRUE);
//		SetMyMode(0x0);
//		WaitKey();
//		rectblock(0,0,799,599-1,&(gColorTable[1])); //white
//		rectangle(100,100,150,150,&(gColorTable[8]));//red
//		circle(300,300,40,&(gColorTable[3]));//blue
//		rectblock(450,450,500,500,&(gColorTable[6]));
//		WaitKey();
//		SetMyMode(0x3);
//		SwitchGraphicsMode(FALSE);
//		
//	}
	//lbdebug end
	//lbdebug start
	Print(L"========start test hii===========2019-6-7 12:56:21 \n");
	
	TestLanguage(HiiHandle);
	TestString(HiiHandle);
	{
		//test simpleFont
		Print((const CHAR16*)L" 您好UEFI SimpleFont,My name is 罗冰~Robin.\n");
		gST->ConOut->SetAttribute(gST->ConOut,EFI_BACKGROUND_RED|EFI_WHITE);
		gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
		Print((const CHAR16*)L" 测试彩色文字\n");
		
//		Print((const CHAR16*)L" \033[1m\033[45;33m HELLO_WORLD \033[0m\n");
	}
	{
		//test FONT
		    EFI_STATUS                             Status = 0;
   			EFI_HII_FONT_PROTOCOL                *HiiFont = 0;
   			EFI_GRAPHICS_OUTPUT_PROTOCOL         *gGraphicsOutput;
   			EFI_IMAGE_OUTPUT                     gSystemFrameBuffer;
   			EFI_IMAGE_OUTPUT*                     pSystemFrameBuffer  = &gSystemFrameBuffer;
   			const CHAR16* Str = (const CHAR16*)L" 您好UEFI Font";
   			CHAR16 R1[32];
   			EFI_FONT_DISPLAY_INFO SystemFont[4]= {{
   			    {0x00,0x00,0xFF,0x00},// BGRA ForeColor
   			    {0xFF,0xFF,0xFF,0x00},// BackColor
   			    EFI_FONT_INFO_ANY_FONT,
   			    {EFI_HII_FONT_STYLE_NORMAL, 19, L'D'}
   			}};
   			CHAR16 R2[32];
   			(void)R1;
   			(void)R2;
   			(void)Str;
   			(void)pSystemFrameBuffer;
   			(void)Status;
   			(void)HiiFont;
   			(void)gGraphicsOutput;
   			(void)gSystemFrameBuffer;
   			(void)SystemFont;
   			
   			Status = gBS->LocateProtocol (&gEfiHiiFontProtocolGuid, NULL, (VOID **) &HiiFont);
   			//Print((const CHAR16*)L"gEfiHiiFontProtocolGuid %r %p", Status, HiiFont);
   			
   			gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&gGraphicsOutput);
   			//Print((const CHAR16*)L"gEfiGraphicsOutputProtocolGuid %r  %p", Status, gGraphicsOutput);
   			gSystemFrameBuffer.Width = (UINT16) gGraphicsOutput->Mode->Info->HorizontalResolution;
   			gSystemFrameBuffer.Height = (UINT16) gGraphicsOutput->Mode->Info->VerticalResolution;
   			gSystemFrameBuffer.Image.Screen = gGraphicsOutput;
   			CopyMem(SystemFont[0].FontInfo.FontName, (const CHAR16*)L"DMTestFont", StrLen((const CHAR16*)L"DMTestFont")*2 +2);
   			Status = HiiFont->StringToImage (
   			        HiiFont,
   			        EFI_HII_IGNORE_IF_NO_GLYPH | EFI_HII_OUT_FLAG_CLIP |
   			        EFI_HII_OUT_FLAG_CLIP_CLEAN_X | EFI_HII_OUT_FLAG_CLIP_CLEAN_Y |
   			        EFI_HII_IGNORE_LINE_BREAK | EFI_HII_OUT_FLAG_TRANSPARENT | EFI_HII_DIRECT_TO_SCREEN ,
   			        (CHAR16*)Str,
   			        (const EFI_FONT_DISPLAY_INFO*)(&SystemFont[0]),
   			        &pSystemFrameBuffer,
   			        (UINTN) 100,
   			        (UINTN) 100,                
   			        0,
   			        0,
   			        0 
   			        );
	}
	Print(L"========end test hii===========2019-6-7 12:56:19 \n");
	
	while(key.ScanCode!=0x17)	//ESC
	{
		//Status=GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		Status=GetKey(&key);
		if (EFI_ERROR (Status)) 
		{
			Print(L"Call GetKeyEx() Error!\n");
			break;
		}
		else
		{
			Print(L"NO.%08d\n",count);
			++count;
			Print(L"  ScanCode=%04x",key.ScanCode);
			Print(L"  UnicodeChar=%04x",key.UnicodeChar);
//			Print(L"  ShiftState=%08x",shiftState);
//			Print(L"  ToggleState=%02x",toggleState);
			Print(L"\n");
		}
	}
	return 0;
}


//===========================================Keyboard function begin====================================
//Name:LocateSimpleTextInputEx
//Input:
//Output: EFI_STATUS=EFI_SUCCESS, *SimpleTextInputEx
//				EFI_STATUS=
//Descriptor: Get the handle of SimpleTextInputEx
EFI_STATUS LocateSimpleTextInputEx(void)
{
	EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;

  
  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &gSimpleTextInputEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			return EFI_SUCCESS;
  	}
  }	 
	return Status;
}
//Name:  GetKeyEx
//Input:
//Output:
//Descriptor: 
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)
{
	EFI_STATUS                        Status;
  EFI_KEY_DATA                      KeyData;
 	UINTN															Index;
	  
	gBS->WaitForEvent(1,&(gSimpleTextInputEx->WaitForKeyEx),&Index);
	Status = gSimpleTextInputEx->ReadKeyStrokeEx(gSimpleTextInputEx,&KeyData);
	if(!EFI_ERROR(Status))
	{
 		*ScanCode=KeyData.Key.ScanCode;
		*UniChar=KeyData.Key.UnicodeChar;
		*ShiftState=KeyData.KeyState.KeyShiftState;
		*ToggleState=KeyData.KeyState.KeyToggleState;
		return EFI_SUCCESS;
	} 
	return Status;
}
//Function Name: FlushKeyBuffer
//Input: none
//Output: none
//Description: Flush all keyboard buffer
VOID FlushKeyBuffer(VOID)
{
  EFI_INPUT_KEY     Key;
 
  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
}
//Function Name: WaitKey
//Input: none
//Output: none
//Description: Wait any key press
VOID WaitKey(VOID)
{
  EFI_INPUT_KEY     Key;
  UINTN Index;

  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
  gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return;
}
//Function Name: GetKey
//Input: none
//Output: none
//Description: Get key code,return unicode code and scan code(such as F1...)
EFI_STATUS GetKey(EFI_INPUT_KEY *key)
{
	UINTN Index;
	gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return gST->ConIn->ReadKeyStroke(gST->ConIn,key);  
}
//===========================================Keyboard function end====================================

//===========================================Graphic function begin====================================
//Name: LocateGraphicsOutput
//Input: 
//Output: *GraphicsOutput
//Descriptor:
EFI_STATUS LocateGraphicsOutput (void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *GraphicsOutputControllerHandles = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports GraphicsOutputProtocol
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&HandleCount,
		&GraphicsOutputControllerHandles
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			GraphicsOutputControllerHandles[HandleIndex],
			&gEfiGraphicsOutputProtocolGuid,
			(VOID**)&gGraphicsOutput);
		if (EFI_ERROR(Status))	continue;

		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: SwitchGraphicsMode(BOOLEAN flag)
//Input:
//Output:
EFI_STATUS SwitchGraphicsMode(BOOLEAN flag)
{
	EFI_STATUS                         Status;
//	if(flag)
//	{
//		Status=gST->ConOut->SetMode(gST->ConOut,EfiConsoleControlScreenGraphics);
//	}
//	else
//	{
//		Status=gST->ConOut->SetMode(gST->ConOut,EfiConsoleControlScreenText);
//	}
	if(flag)
	{
		Status=gST->ConOut->EnableCursor (gST->ConOut, FALSE);
	}
	else
	{
		Status=gST->ConOut->EnableCursor (gST->ConOut, TRUE);
	}
	return Status;
}
//Function Name:  SetMyMode
//Input: ModeNumber
//Output: Void
//Description: Set Specify mode
VOID SetMyMode(UINT32 ModeNumber)
{
   gGraphicsOutput->SetMode(gGraphicsOutput,ModeNumber);
   return;
}
VOID putpixel(UINTN x,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
//  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,x,y,1,1,0);
}
//Function Name:  HLine
//Input: x1,x2,y,color (x1<x2)
//Output: Void
//Description: Draw horizontal line
VOID HLine(UINTN x1,UINTN x2,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN minx,maxx;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  minx=(x1<x2)?x1:x2;
  maxx=(x1>x2)?x1:x2;
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,minx,y,(maxx-minx+1),1,0);  
}
//Function Name:  VLine
//Input: x,y1,y2,color(y1<y2)
//Output: Void
//Description: Draw vertical line
VOID VLine(UINTN x,UINTN y1,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN miny,maxy;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  miny=(y1<y2)?y1:y2;
  maxy=(y1>y2)?y1:y2;
  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,x,miny,1,(maxy-miny+1),0);  
}
//Function Name:  rectblock
//Input: x1,y1,x2,y2,color(y1<y2)
//Output: Void
//Description: Draw block rect
VOID  rectblock(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN minx,miny,maxx,maxy;
  
  minx=(x1<x2)?x1:x2;
  miny=(y1<y2)?y1:y2;
  maxx=(x1>x2)?x1:x2;
  maxy=(y1>y2)?y1:y2;
  
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,minx,miny,(maxx-minx+1),(maxy-miny+1),0);   
}

//another function
//Function Name:  rectangle
//Input: x1,y1,x2,y2,color_number: Pointer to ColorTable(y1<y2)
//Output: Void
//Description: Draw Rectangle
VOID  rectangle(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN left,top,right,bottom,i;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;

  /* Location */
  left=(x1<x2)?x1:x2;
  right=(x1>x2)?x1:x2;
  top=(y1<y2)?y1:y2;
  bottom=(y1>y2)?y1:y2;
//  right=(right<maxX)?right:maxX;
//  bottom=(bottom<maxY)?bottom:maxY;       /* Cut off the graphic which out of screen */
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    
  for(i=left;i<right;i++)putpixel(i,top,color);
  for(i=top;i<bottom;i++)putpixel(right,i,color);
  for(i=right;i>left;i--)putpixel(i,bottom,color);
  for(i=bottom;i>top;i--)putpixel(left,i,color);
}
//Function Name:  circle
//Input: centerx,centery,radius,color
//Output: Void
//Description: Draw Rectangle
VOID  circle(UINTN centerx,UINTN centery,UINTN radius,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  signed int x,y,d;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if(!radius)return;
  y=(signed int)(radius);
  d=3-((signed int)(radius)<<1);
  
  
  /*see foxdisk3.01 by luobing*/
  putpixel(centerx,centery+y,color);        /*0,-1*/
  putpixel(centerx,centery-y,color);        /*0,1*/
  putpixel(centerx+y,centery,color);
  putpixel(centerx-y,centery,color);
  
  for(x=1;x<y;x++)
  {
    putpixel(centerx+x,centery+y,color);
    putpixel(centerx+x,centery-y,color);
    putpixel(centerx-x,centery-y,color);
    putpixel(centerx-x,centery+y,color);
    putpixel(centerx+y,centery+x,color);
    putpixel(centerx+y,centery-x,color);
    putpixel(centerx-y,centery-x,color);
    putpixel(centerx-y,centery+x,color);
    if(d<0)d+=((x<<2)+6);           
    else d+=(((x-(y--))<<2)+10);        
  }
  /* x=y */
  putpixel(centerx+y,centery+y,color);
  putpixel(centerx+y,centery-y,color);
  putpixel(centerx-y,centery-y,color);
  putpixel(centerx-y,centery+y,color);
}   
//Function Name: SetBKG
//Input: color
//Output: None
//Description: set background
//VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
//{
//  rectblock(0,0,SY_SCREEN_WIDTH-1,SY_SCREEN_HEIGHT-1,color);
//}
//===========================================Graphic function end======================================
//================Debug function================
//Name: DisplayModeInformation
//Input:
//Output:
EFI_STATUS DisplayModeInformation(void)
{
	Print(L"Print Current Mode Message:\n");
	Print(L"::MaxMode=%02x\n",gGraphicsOutput->Mode->MaxMode);
	Print(L"::Current Mode=%02x\n",gGraphicsOutput->Mode->Mode);
	Print(L"::SizeOfInfo=%x\n",gGraphicsOutput->Mode->SizeOfInfo);
	Print(L"::Info->Version=%x\n",gGraphicsOutput->Mode->Info->Version);
	Print(L"::Info->HorizontalResolution=%x\n",gGraphicsOutput->Mode->Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%x\n",gGraphicsOutput->Mode->Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%x\n",gGraphicsOutput->Mode->Info->PixelsPerScanLine);
	
	return EFI_SUCCESS;
}
//Name: DisplaySpecifyModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber)
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *Info;
	UINTN                                  SizeOfInfo;
	EFI_STATUS                         Status;
	
	Status=gGraphicsOutput->QueryMode(gGraphicsOutput,ModeNumber,&SizeOfInfo,&Info);
	Print(L"Print Mode %d Message:\n",ModeNumber);
	Print(L"::Info->Version=%d\n",Info->Version);
	Print(L"::Info->HorizontalResolution=%d\n",Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%d\n",Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%d\n",Info->PixelsPerScanLine);
	
	return Status;
}
//Name: DisplayTextModeInformation(void)
//Input:
//Output:
EFI_STATUS DisplayTextModeInformation(void)
{
	Print(L"Print SimPleTextMode Message:\n");
	Print(L"::MaxMode=%d\n",gST->ConOut->Mode->MaxMode);
	Print(L"::Current Mode=%d\n",gST->ConOut->Mode->Mode);
	Print(L"::Attribute=%d\n",gST->ConOut->Mode->Attribute);
	Print(L"::CursorColumn=%d\n",gST->ConOut->Mode->CursorColumn);
	Print(L"::CursorRow=%d\n",gST->ConOut->Mode->CursorRow);
	Print(L"::CursorVisible=%d\n",gST->ConOut->Mode->CursorVisible);
	
	return EFI_SUCCESS;	
}

//Name: DisplaySpecifyTextModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber)
{
	EFI_STATUS                         Status;
	UINTN Columns;
	UINTN Rows;
	
	Status=gST->ConOut->QueryMode(gST->ConOut,ModeNumber,&Columns,&Rows);
	Print(L"Print Text Mode %d Message:\n",ModeNumber);
	Print(L"::Column=%d\n",Columns);
	Print(L"::Row=%d\n",Rows);
	
	return Status;
}

//============================== Hii Function begin==============================================
EFI_STATUS TestLanguage(EFI_HANDLE HiiHandle)
{
	EFI_STATUS Status = 0;
	CHAR8 buf[256];
	CHAR16 buf16[256];
	UINTN LangSize = 256;
	UINTN i = 0;

	Status = gHiiString -> GetLanguages(
			 gHiiString,
			 HiiHandle,
			 buf,
			 &LangSize);
	for(i =0; i< LangSize; i++){
		buf16[i] = buf[i];
	}
	buf16[LangSize] = 0;
	Print(L"Support Language: %s\n", buf16);
	return Status;
}

EFI_STATUS TestString(EFI_HANDLE HiiHandle )
{
	EFI_STATUS Status = 0;
	//CHAR8* BestLanguage = "en-US";
	CHAR8* BestLanguage = "zh-Hans";
	EFI_STRING TempString = NULL;
	UINTN StringSize = 0;
	Status = gHiiString -> GetString (
			 gHiiString,
                         BestLanguage,
                         HiiHandle,
                         STRING_TOKEN (STR_LANGUAGE_SELECT),
                         TempString,
                         &StringSize,
                         NULL
                         );
//gBS->AllocatePool(EfiBootServicesData,0x100,(void**)&buffer);
	if(Status == EFI_BUFFER_TOO_SMALL){
		Status = gBS->AllocatePool (EfiBootServicesData,StringSize,(void**)&TempString);
		if(EFI_ERROR(Status))
			return 0;
		Status = gHiiString -> GetString (
			 gHiiString,
                         BestLanguage,
                         HiiHandle,
                         STRING_TOKEN (STR_LANGUAGE_SELECT),
                         TempString,
                         &StringSize,
                         NULL
                         );
		Print(L"%s\n", TempString);
		gBS->FreePool(TempString);

	}else{
		Print(L"GetString %r\n", Status);
	}
	return 0;
}

// Add 4 bytes to the header for entire length for HiiAddPackages use only. 
// 
// +--------------------------------+ <-- Package 
// | | 
// | packageLen(4 bytes) | 
// | | 
// |--------------------------------| <-- simpleFont 
// | | 
// |EFI_HII_SIMPLE_FONT_PACKAGE_HDR | 
// | | 
// |--------------------------------| <-- Location 
// | | 
// | EFI_NARROW_GLYPH       NarrowGlyphs[];
// | EFI_WIDE_GLYPH         WideGlyphs[];
// | | 
// +--------------------------------+ 


// 0 |   0       |   1       |   2       |   3       |
// 4 |            Package Length                     | Package Length
// 8 |               Length              | Type: 07  | EFI_HII_PACKAGE_HEADER         
// 12|  NumberOfNarrowGlyphs |  NumberOfWideGlyphs   |
// 16|  [EFI_NARROW_GLYPH[] | EFI_WIDE_GLYPH[]]      |
 
//simpleFont function
EFI_STATUS CreatesimpleFontPkg(EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes)
{
    EFI_STATUS Status; 
    EFI_HII_SIMPLE_FONT_PACKAGE_HDR *simpleFont; 
    UINT8 *Package; 

    // 
    // Locate HII Database Protocol 
    // 
    EFI_HII_DATABASE_PROTOCOL *HiiDatabase = 0; 
    Status = gBS->LocateProtocol ( 
            &gEfiHiiDatabaseProtocolGuid, 
            NULL, 
            (VOID **) &HiiDatabase 
            ); 
    if(EFI_ERROR(Status)){
    }

    {
        UINT32    packageLen = sizeof (EFI_HII_SIMPLE_FONT_PACKAGE_HDR) + SizeInBytes + 4; 
        Package = (UINT8*)AllocateZeroPool (packageLen); 

        WriteUnaligned32((UINT32 *) Package,packageLen); 
        simpleFont = (EFI_HII_SIMPLE_FONT_PACKAGE_HDR *) (Package + 4); 
        simpleFont->Header.Length = (UINT32) (packageLen - 4); 
        simpleFont->Header.Type = EFI_HII_PACKAGE_SIMPLE_FONTS; 
        simpleFont->NumberOfNarrowGlyphs = 0;
        simpleFont->NumberOfWideGlyphs = (UINT16) (SizeInBytes / sizeof (EFI_WIDE_GLYPH)); 
    }
    {
        UINT8 * Location = (UINT8 *) (&simpleFont->NumberOfWideGlyphs + 1); 
        CopyMem (Location, WideGlyph, SizeInBytes); 
    }

    // 
    // Add this simplified font package to a package list then install it. 
    // 
    {
        EFI_HII_HANDLE  simpleFontHiiHandle = HiiAddPackages ( 
                &gSimpleFontPackageListGuid,
                NULL, 
                Package, 
                NULL 
                ); 
        if(simpleFontHiiHandle == NULL)
        {
            return EFI_NOT_FOUND;  //查看了EfiError.h,选择这个返回值
        }
    }
    FreePool (Package); 
    return EFI_SUCCESS;
}


//Font functions
EFI_STATUS FillNarrowGLYPH(UINT8* p, 
        EFI_NARROW_GLYPH* NarrowGlyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 NrStart, CHAR16 NrCharNum)
{
    UINT8* BitmapData = NULL; 
    UINTN Length = 0;
    EFI_HII_GLYPH_INFO Cell = {8, 19, 0, 0, (INT16)8};
		EFI_HII_GIBT_GLYPHS_BLOCK * GlyphsBlock;  //2019-6-7 11:59:05 luobing ammend
		
		
    // SKIP
    if(Next != NrStart){
        EFI_HII_GIBT_SKIP2_BLOCK* FontSkip2Block = (EFI_HII_GIBT_SKIP2_BLOCK*)p;
        FontSkip2Block ->Header.BlockType = (UINT8)EFI_HII_SIBT_SKIP2;
        FontSkip2Block ->SkipCount = NrStart - Next ;

        p = (UINT8*)(FontSkip2Block+1);
        Length += sizeof(EFI_HII_GIBT_SKIP2_BLOCK);
    }

//    EFI_HII_GIBT_GLYPHS_BLOCK * GlyphsBlock = (EFI_HII_GIBT_GLYPHS_BLOCK *)(p);
    GlyphsBlock = (EFI_HII_GIBT_GLYPHS_BLOCK *)(p);
    GlyphsBlock -> Header.BlockType = (UINT8)EFI_HII_GIBT_GLYPHS;
    GlyphsBlock -> Cell = Cell;
    GlyphsBlock -> Count = NrCharNum;
    BitmapData = GlyphsBlock->BitmapData;
    {
        UINT16 i = 0, j = 0; 
        for(i=0;i < NrCharNum; i++){
            for(j=0;j < 19; j++){
                BitmapData[j] = NarrowGlyph[i].GlyphCol1[j];
            }
            BitmapData += 19;
        }
    }
    Length += ( sizeof(EFI_HII_GIBT_GLYPHS_BLOCK ) + 19 * NrCharNum -1); 
    return (EFI_STATUS)Length;
}

EFI_STATUS FillWideGLYPH(UINT8* p, 
        EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 Start, CHAR16 CharNum)
{
    UINT8* BitmapData = NULL; 
    UINTN Length = 0;
	  EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK* GlyphsDefaultBlock; //2019-6-7 11:58:55 luobing ammend
	  
    // SKIP
    if(Next != Start){
        EFI_HII_GIBT_SKIP2_BLOCK* FontSkip2Block = (EFI_HII_GIBT_SKIP2_BLOCK*)p;
        FontSkip2Block ->Header.BlockType = (UINT8)EFI_HII_SIBT_SKIP2;
        FontSkip2Block ->SkipCount = Start - Next ;

        p = (UINT8*)(FontSkip2Block+1);
        Length += sizeof(EFI_HII_GIBT_SKIP2_BLOCK);
    }

//    EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK* GlyphsDefaultBlock = (EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK*)(p);
    GlyphsDefaultBlock = (EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK*)(p);
    GlyphsDefaultBlock -> Header.BlockType = (UINT8)EFI_HII_GIBT_GLYPHS_DEFAULT;
    GlyphsDefaultBlock -> Count = CharNum;
    BitmapData = GlyphsDefaultBlock->BitmapData;
    {
        UINT16 i = 0, j = 0; 
        for(i=0;i < CharNum; i++){
            for(j=0;j < WideGlyphBytes; j++){
                BitmapData[j] = WideGlyph[i].GlyphCol1[(j%2)*19 + j/2];
            }
            BitmapData += WideGlyphBytes;
        }
    }
    Length += ( sizeof(EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK) + WideGlyphBytes * CharNum -1); 
    return (EFI_STATUS)Length;
}

EFI_STATUS FillGLYPH(UINT8* p, 
        UINT8* Glyph, UINT32 SizeInBytes, CHAR16 Next, CHAR16 Start, CHAR16 CharNum)
{
    UINT8* BitmapData = NULL; 
    UINTN Length = 0;
		EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK* GlyphsDefaultBlock;	//2019-6-7 11:59:45 luobing ammend
    // SKIP
    if(Next != Start){
        EFI_HII_GIBT_SKIP2_BLOCK* FontSkip2Block = (EFI_HII_GIBT_SKIP2_BLOCK*)p;
        FontSkip2Block ->Header.BlockType = (UINT8)EFI_HII_SIBT_SKIP2;
        FontSkip2Block ->SkipCount = Start - Next ;

        p = (UINT8*)(FontSkip2Block+1);
        Length += sizeof(EFI_HII_GIBT_SKIP2_BLOCK);
    }

    // 
//    EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK* GlyphsDefaultBlock = (EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK*)(p);
    GlyphsDefaultBlock = (EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK*)(p);
    GlyphsDefaultBlock -> Header.BlockType = (UINT8)EFI_HII_GIBT_GLYPHS_DEFAULT;
    GlyphsDefaultBlock -> Count = CharNum;
    BitmapData = GlyphsDefaultBlock->BitmapData;
    CopyMem (BitmapData, Glyph, SizeInBytes);
    Length += ( sizeof(EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK) + SizeInBytes -1); 
    return (EFI_STATUS)Length;
}

EFI_STATUS CreateDMFontPkg(CHAR16* FontName, UINT16 FontWidth, UINT16 FontHeight,  
        EFI_NARROW_GLYPH* NarrowGlyph, UINT32 NrSizeInBytes, CHAR16 NrStart, CHAR16 NrCharNum,
        EFI_WIDE_GLYPH* WideGlyph, UINT32 SizeInBytes, CHAR16 Start, CHAR16 CharNum)
{
    EFI_HII_FONT_PACKAGE_HDR *FontPkgHeader; //Font 专用的数据结构
    UINT32 PackageLength; 
    UINT8 *Package; 
    UINTN BlockLength = 0;
    UINT8 *pCurrent = 0; 
    CHAR16 NextChar = 0;
    EFI_GLYPH_GIBT_END_BLOCK* FontEndBlock;		//2019-6-7 12:01:37 luobing ammend
	  EFI_HII_GLYPH_INFO Cell;
//    EFI_HII_GLYPH_INFO Cell = {FontWidth, FontHeight, 10, 10, (INT16)FontWidth};  //2019-6-7 12:02:37 luobing: build error

//    UINT16 FontNameLen = StrLen(FontName);
		UINT16 FontNameLen = (UINT16)StrLen(FontName);
    //init Cell
    Cell.Width=FontWidth;
    Cell.Height=FontHeight;
    Cell.OffsetX=10;
    Cell.OffsetY=10;
    Cell.AdvanceX=(INT16)FontWidth;
    
    PackageLength = 4 + sizeof (EFI_HII_FONT_PACKAGE_HDR) + (FontNameLen*2 /*Max Length of Font Name*/) + 
        sizeof(EFI_HII_GIBT_SKIP2_BLOCK) + 
        sizeof(EFI_HII_GIBT_GLYPHS_DEFAULT_BLOCK) -1 + CharNum * WideGlyphBytes + 
        sizeof(EFI_HII_GIBT_SKIP2_BLOCK) + 
        sizeof(EFI_HII_GIBT_GLYPHS_BLOCK) -1 + NrCharNum * NrGlyphBytes  +
        sizeof(EFI_GLYPH_GIBT_END_BLOCK) ;
    Package = (UINT8*)AllocateZeroPool (PackageLength); 
    //ASSERT (Package != NULL); 

    // Header
    WriteUnaligned32((UINT32 *) Package,PackageLength); 
    FontPkgHeader = (EFI_HII_FONT_PACKAGE_HDR *) (Package + 4); 
    FontPkgHeader->Header.Length = (UINT32) (PackageLength - 4); 
    FontPkgHeader->Header.Type = EFI_HII_PACKAGE_FONTS; 
    FontPkgHeader->HdrSize = sizeof(EFI_HII_FONT_PACKAGE_HDR) + (FontNameLen)*2;
    FontPkgHeader->GlyphBlockOffset = sizeof(EFI_HII_FONT_PACKAGE_HDR) + (FontNameLen*2); 
    FontPkgHeader->Cell = Cell;
    FontPkgHeader->FontStyle = EFI_HII_FONT_STYLE_NORMAL;
    CopyMem((FontPkgHeader->FontFamily), FontName, StrLen(FontName)*2 +2);


    pCurrent = (UINT8 *) (Package + 4 + FontPkgHeader->GlyphBlockOffset); 

    //CHAR 0...255
    BlockLength = FillNarrowGLYPH(pCurrent, NarrowGlyph, NrSizeInBytes, 1, NrStart, NrCharNum);
    pCurrent += BlockLength;
    NextChar = NrStart + NrCharNum;
    // EFI_HII_GIBT_GLYPHS_DEFAULT
    BlockLength = FillWideGLYPH(pCurrent, WideGlyph, SizeInBytes, NextChar, Start, CharNum);
    pCurrent += BlockLength;
    // END BLOCK
//    EFI_GLYPH_GIBT_END_BLOCK* FontEndBlock = (EFI_GLYPH_GIBT_END_BLOCK*)(pCurrent);
    FontEndBlock = (EFI_GLYPH_GIBT_END_BLOCK*)(pCurrent);
    
    FontEndBlock->Header.BlockType = (UINT8)EFI_HII_GIBT_END;

    // 
    // Add this simplified font package to a package list then install it. 
    // 
    {
        EFI_HANDLE gFontHiiHandle = HiiAddPackages ( 
                &gDMFontPackageListGuid ,
                NULL, 
                Package, 
                NULL 
                ); 
        (void)gFontHiiHandle;
        //ASSERT (gFontHiiHandle != NULL); 
    }
    FreePool (Package); 
    return EFI_SUCCESS;
}
//============================== Hii Function end==============================================