//code by luobing 2019-6-7 21:52:13

#include "HiiFont.h"

extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
extern EFI_HII_FONT_PROTOCOL *gHiiFont;
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


//============================== Hii Function begin==============================================
//ugly codes...
//必须先注册汉字simplefont库，才能显示汉字
EFI_STATUS TestStrPackage(void)
{
	EFI_STATUS Status = 0;
	EFI_GUID   mStrPackageGuid = { 0xedd31def, 0xf262, 0xc24e, 0xa2, 0xe4, 0xde, 0xf7, 0xde, 0xcd, 0xcd, 0xee };
	EFI_HANDLE HiiHandle = HiiAddPackages (&mStrPackageGuid ,  gImageHandle, Luo3Strings, NULL);
	Status=TestLanguage(HiiHandle);
	Status=TestString(HiiHandle);
	return Status;
}
//ugly codes...
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
//ugly codes...
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
//Name:
//Input:
//Output:
//Description:
EFI_STATUS LoadSimpleFont(void)
{
	EFI_STATUS Status=0;
	EFI_HII_HANDLE    * handles = 0;		//for simpleFont of Hii
	handles = HiiGetHiiHandles(&gSimpleFontPackageListGuid); 
	if(handles == 0)
	{
		Status = CreatesimpleFontPkg(gSimpleFontWideGlyphData, gSimpleFontBytes);
		//gST->ConOut->OutputString(gST->ConOut,L"execute CreatesimpleFontPkg()  handles==0\n\r");
		//return Status;
	}
	else
	{
		//gST->ConOut->OutputString(gST->ConOut,L"execute CreatesimpleFontPkg()  handles==1\n\r");
		FreePool(handles);
		return EFI_LOAD_ERROR;			//refer to EfiError.h
	}
	
	return Status;
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


//-----------------------------------Hii Font-----------------------
//Font functions
//Name:
//Input:
//Output:
//Description:
EFI_STATUS LoadFont(void)
{
	EFI_STATUS Status=0;
	EFI_HII_HANDLE    * handles1 = 0;		//for Font of Hii
	handles1 = HiiGetHiiHandles(&gDMFontPackageListGuid); 
	if(handles1 == 0)
	{
		//extern CHAR16* FontName ;
		CHAR16 * FontName=(CHAR16*)L"LUOBING";  //随便取个名字
		Status = CreateDMFontPkg( FontName, 16, 19,  
															gUsStdNarrowGlyphData  , mNarrowFontSize, 32, (CHAR16)(mNarrowFontSize/sizeof(EFI_NARROW_GLYPH)),
                    					gSimpleFontWideGlyphData, gSimpleFontBytes, 0x4e00, (CHAR16)(gSimpleFontBytes/sizeof(EFI_WIDE_GLYPH))
                    				);
		//gST->ConOut->OutputString(gST->ConOut,L"execute CreateDMFontPkg()  handles1==0\n\r");
	}
	else
	{
		//gST->ConOut->OutputString(gST->ConOut,L"execute CreateDMFontPkg()  handles1==1\n\r");
		FreePool(handles1);
		return EFI_LOAD_ERROR;			//refer to EfiError.h
	}
	
	return Status;
}
//Name:putHiiFontStr
//Input:x,y,String
//Output:
//Description:字体颜色、大小等，函数没有提供控制，只允许指定位置，显示相应的字符串
//			typedef struct _EFI_FONT_DISPLAY_INFO {
//							EFI_GRAPHICS_OUTPUT_BLT_PIXEL ForegroundColor;
//							EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackgroundColor;
//							EFI_FONT_INFO_MASK FontInfoMask;
//							EFI_FONT_INFO FontInfo
//			} EFI_FONT_DISPLAY_INFO; 
//			typedef struct {
//							EFI_HII_FONT_STYLE FontStyle;
//							UINT16 FontSize;
//							CHAR16 FontName[…];
//			} EFI_FONT_INFO;

//		typedef struct _EFI_IMAGE_OUTPUT {
//						UINT16 Width;
//						UINT16 Height;
//						union {
//									EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Bitmap;
//									EFI_GRAPHICS_OUTPUT_PROTOCOL *Screen;
//						} Image;
//		} EFI_IMAGE_OUTPUT;

EFI_STATUS putHiiFontStr(UINTN x,UINTN y,EFI_STRING String,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *fontForegroundColor,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *fontBackgroundColor)
{
	//gHiiFont and gGraphicsOutput has been loaded
	 EFI_STATUS                             Status = 0;
	 EFI_IMAGE_OUTPUT                     gSystemFrameBuffer;
	 EFI_IMAGE_OUTPUT*                     pSystemFrameBuffer  = &gSystemFrameBuffer;
	 EFI_FONT_DISPLAY_INFO  *fontDisplayInfo;
//	 EFI_FONT_DISPLAY_INFO  fontDisplayInfo[4]={{
//	 				{0x00,0x22,0x00,0x00},// BGRA ForeColor
//        {0x00,0x00,0xFF,0x00},// BackColor
//        EFI_FONT_INFO_ANY_FONT,
//        {EFI_HII_FONT_STYLE_NORMAL, 19, L'D'}
//        }};
//     CHAR16 FontName[…]这个内存怎么申请啊？
	 fontDisplayInfo = (EFI_FONT_DISPLAY_INFO  *)AllocateZeroPool (sizeof (EFI_FONT_DISPLAY_INFO)+StrLen((const CHAR16*)L"LUOBING")*2 +2); 
	 fontDisplayInfo->ForegroundColor=*fontForegroundColor;
	 fontDisplayInfo->BackgroundColor=*fontBackgroundColor;
	 fontDisplayInfo->FontInfoMask=EFI_FONT_INFO_ANY_FONT;
	 fontDisplayInfo->FontInfo.FontStyle=EFI_HII_FONT_STYLE_NORMAL;
	 fontDisplayInfo->FontInfo.FontSize=19;
	 CopyMem(fontDisplayInfo->FontInfo.FontName, (const CHAR16*)L"LUOBING", StrLen((const CHAR16*)L"LUOBING")*2 +2);
	 
//	 EFI_FONT_DISPLAY_INFO SystemFont[4]= {{
//        {0x00,0x22,0x00,0x00},// BGRA ForeColor
//        {0x00,0x00,0xFF,0x00},// BackColor
//        EFI_FONT_INFO_ANY_FONT,
//        {EFI_HII_FONT_STYLE_NORMAL, 19, L'D'}
//    }};
    
	 	gSystemFrameBuffer.Width = (UINT16) gGraphicsOutput->Mode->Info->HorizontalResolution;
		gSystemFrameBuffer.Height = (UINT16) gGraphicsOutput->Mode->Info->VerticalResolution;
    gSystemFrameBuffer.Image.Screen = gGraphicsOutput;
//    CopyMem(SystemFont[0].FontInfo.FontName, (const CHAR16*)L"DMTestFont", StrLen((const CHAR16*)L"DMTestFont")*2 +2);
    
    Status = gHiiFont->StringToImage (
            gHiiFont,
            EFI_HII_IGNORE_IF_NO_GLYPH | EFI_HII_OUT_FLAG_CLIP |
            EFI_HII_OUT_FLAG_CLIP_CLEAN_X | EFI_HII_OUT_FLAG_CLIP_CLEAN_Y |   
            EFI_HII_IGNORE_LINE_BREAK |  EFI_HII_DIRECT_TO_SCREEN , 
            //EFI_HII_IGNORE_LINE_BREAK | EFI_HII_OUT_FLAG_TRANSPARENT | EFI_HII_DIRECT_TO_SCREEN ,
            String,
            //(const EFI_FONT_DISPLAY_INFO*)(&SystemFont[0]),
            //(EFI_FONT_DISPLAY_INFO*)(&fontDisplayInfo[0]),
            fontDisplayInfo,
            &pSystemFrameBuffer,
            (UINTN) x,
            (UINTN) y,                
            0,
            0,
            0 
            );
    FreePool(fontDisplayInfo);   
		return Status;
}


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


