//2019-06-11 09:23:48 luobing

#ifndef _FONT_H
#define _FONT_H 
#include "Common.h"
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>  //gHiiString
#include <Library/MemoryAllocationLib.h> //AllocatZeroPool,FreePool
#include <Protocol/HiiFont.h>
#include "Protocol/HiiDatabase.h"

EFI_STATUS LoadMySimpleFont();
EFI_STATUS UnLoadMySimpleFont();
EFI_STATUS CreatesimpleFontPkg(EFI_WIDE_GLYPH *WideGlyph, UINT32 SizeInBytes);

#endif