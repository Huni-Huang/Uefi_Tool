//luobng 2019-10-01 22:01:04 


// #include "Fontdb.c"
#include "Font.h"

//SimpleFont
extern EFI_WIDE_GLYPH  gSimpleFontWideGlyphData[];
extern UINT32 gSimpleFontBytes;
static EFI_GUID gSimpleFontPackageListGuid = {0xf6643673, 0x6006, 0x3738, {0x5c, 0xcd, 0xda, 0x1a, 0xeb, 0x3b, 0x26, 0xa2}};

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
            return EFI_NOT_FOUND;  //check file:EfiError.h 
        }
    }
    FreePool (Package); 
    return EFI_SUCCESS;
}

//Name:LoadSimpleFont
//Input:
//Output: 
//Descriptor: load simple font, after that, we can print chinese string in uefi shell
EFI_STATUS LoadMySimpleFont()
{
    EFI_STATUS Status;
    EFI_HII_HANDLE    * handles = 0;

    handles = HiiGetHiiHandles(&gSimpleFontPackageListGuid); 
    if(handles == 0)
        Status = CreatesimpleFontPkg(gSimpleFontWideGlyphData, gSimpleFontBytes);
    else
        FreePool(handles);

    return Status;
}

//Name:UnLoadSimpleFont
//Input:
//Output: 
//Descriptor: unload simple font handle
EFI_STATUS UnLoadMySimpleFont()
{
    EFI_STATUS Status = 0;
    EFI_HII_HANDLE    * handles = 0;
    UINT32 i = 0;
    handles = HiiGetHiiHandles(&gSimpleFontPackageListGuid); 
    if(handles == 0){
        return EFI_LOAD_ERROR;
    }
    while(handles[i] != 0){
        HiiRemovePackages( handles[i]); 
        i++;
    }
    FreePool(handles);

    return Status;
}