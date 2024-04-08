/** @file
    A basic application displaying an image that moves via keyboard input.
    
    Copyright (c) 2013, Tim Lewis
    This program and the accompanying materials are licensed and made available 
    under the terms and conditions of the BSD License which accompanies this 
    distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR 
    IMPLIED.
**/


// #include  <stdio.h>
// #include "protocol/graphicsoutput.h"
// #include <Stretcher.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Uefi.h>
#include <IndustryStandard/Bmp.h>

#include "Library/BaseLib.h"
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>


UINTN MaxHeight;

//GOP for ALL images
EFI_GRAPHICS_OUTPUT_PROTOCOL *mGraphicsOutput;

//Background picture image info
UINTN mBgWidth;
UINTN mBgHeight;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mBgBlt;

//Middle picture image info
UINTN mMidWidth;
UINTN mMidHeight;                         //varies
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mMidBlt;   

//Foreground picture image info
UINTN mFrontWidth;
UINTN mFrontHeight;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mFrontBlt;

//define the GUIDs of bg, mid, and front

// STATIC
//   EFI_GUID
//   gBgImageFileGuid = {0xde7022e0, 0xe879, 0x46ee, {0x85, 0x73, 0xa9, 0x8a, 0x24, 0x36, 0xa2, 0xba}};

// STATIC 
//   EFI_GUID
//   gMidImageFileGuid = {0x4a6ee4c8, 0x89cc, 0x4d6c, {0x84, 0xd1, 0x2b, 0x55, 0xc1, 0x41, 0x86, 0x3C}};

// STATIC
//   EFI_GUID
//   gFrontImageFileGuid = {0xd72fe152, 0xe3f6, 0x4ca1, {0x92, 0x58, 0x5b, 0xc4, 0x11, 0x6e, 0x61, 0x26}};
EFI_STATUS ConvertBmpToGopBlt (VOID*, UINTN, VOID**, UINTN*, UINTN*, UINTN*);

EFI_STATUS CopyBitmapToBitmap (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*, UINTN, UINTN, EFI_GRAPHICS_OUTPUT_BLT_PIXEL*, UINTN, UINTN, UINTN, UINTN);

EFI_STATUS RotateUp ();
EFI_STATUS RotateDown();
BOOLEAN InitGop(VOID);
EFI_STATUS LoadFile(
  IN CONST CHAR8    *FilePath,
  OUT VOID          **File,
  OUT UINT32        *FileSize
);
BOOLEAN
LoadBitmapFile(
  IN CONST CHAR8    *BmpFilePath,
  OUT VOID          **Blt,
  OUT UINTN         *Height,
  OUT UINTN         *Width
);
// EFI_STATUS SetUpImages (CONST EFI_GUID*, CONST EFI_GUID*, CONST EFI_GUID*);
EFI_STATUS SetUpImages (  
  IN CONST CHAR8    *BgLogoFilePath,
  IN CONST CHAR8    *MidLogoFilePath,
  IN CONST CHAR8    *FrontLogoFilePath);
EFI_STATUS DisplayImageStack();
EFI_STATUS StretchImage ();
/***
  Displays an image and allows it to move through keyboard input.

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
  

  if (EFI_ERROR (SetUpImages("bg.bmp", "mid.bmp", "front.bmp"))) {
    return 1;
  }
  
    puts("Use UP and DOWN arrow keys to change temperature and <End> to quit.");

  gST->ConOut->Reset(gST->ConOut, FALSE);
  gST->ConOut->EnableCursor (gST->ConOut, FALSE);
  if (EFI_ERROR (StretchImage())) {
    return 1;
  }
   
  return 0;
}

/**
  Convert a *.BMP graphics image to a GOP blt buffer. If a NULL Blt buffer
  is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
  buffer is passed in it will be used if it is big enough.

  @param  BmpImage      Pointer to BMP file
  @param  BmpImageSize  Number of bytes in BmpImage
  @param  GopBlt        Buffer containing GOP version of BmpImage.
  @param  GopBltSize    Size of GopBlt in bytes.
  @param  PixelHeight   Height of GopBlt/BmpImage in pixels
  @param  PixelWidth    Width of GopBlt/BmpImage in pixels

  @retval EFI_SUCCESS           GopBlt and GopBltSize are returned.
  @retval EFI_UNSUPPORTED       BmpImage is not a valid *.BMP image
  @retval EFI_BUFFER_TOO_SMALL  The passed in GopBlt buffer is not big enough.
                                GopBltSize will contain the required size.
  @retval EFI_OUT_OF_RESOURCES  No enough buffer to allocate.

**/
EFI_STATUS
ConvertBmpToGopBlt (
  IN VOID *BmpImage,
  IN UINTN BmpImageSize,
  IN OUT VOID **GopBlt,
  IN OUT UINTN *GopBltSize,
     OUT UINTN *PixelHeight,
     OUT UINTN *PixelWidth
  )
{
  UINT8 *Image;
  UINT8 *ImageHeader;
  BMP_IMAGE_HEADER *BmpHeader;
  BMP_COLOR_MAP *BmpColorMap;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINT64 BltBufferSize;
  UINTN Index;
  UINTN Height;
  UINTN Width;
  UINTN ImageIndex;
  UINT32 DataSizePerLine;
  BOOLEAN IsAllocated;
  UINT32 ColorMapNum;

  if (sizeof (BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

  BmpHeader = (BMP_IMAGE_HEADER *) BmpImage;

  if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M') {
    return EFI_UNSUPPORTED;
  }

  //
  // Doesn't support compress.
  //
  if (BmpHeader->CompressionType != 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Only support BITMAPINFOHEADER format.
  // BITMAPFILEHEADER + BITMAPINFOHEADER = BMP_IMAGE_HEADER
  //
  if (BmpHeader->HeaderSize != sizeof (BMP_IMAGE_HEADER) - OFFSET_OF(BMP_IMAGE_HEADER, HeaderSize)) {
    return EFI_UNSUPPORTED;
  }

  //
  // The data size in each line must be 4 byte alignment.
  //
  DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
  BltBufferSize = MultU64x32 (DataSizePerLine, BmpHeader->PixelHeight);
  if (BltBufferSize > (UINT32) ~0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BmpHeader->Size != BmpImageSize) || 
      (BmpHeader->Size < BmpHeader->ImageOffset) ||
      (BmpHeader->Size - BmpHeader->ImageOffset !=  BmpHeader->PixelHeight * DataSizePerLine)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Calculate Color Map offset in the image.
  //
  Image       = BmpImage;
  BmpColorMap = (BMP_COLOR_MAP *) (Image + sizeof (BMP_IMAGE_HEADER));
  if (BmpHeader->ImageOffset < sizeof (BMP_IMAGE_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BmpHeader->ImageOffset > sizeof (BMP_IMAGE_HEADER)) {
    switch (BmpHeader->BitPerPixel) {
      case 1:
        ColorMapNum = 2;
        break;
      case 4:
        ColorMapNum = 16;
        break;
      case 8:
        ColorMapNum = 256;
        break;
      default:
        ColorMapNum = 0;
        break;
      }
    if (BmpHeader->ImageOffset - sizeof (BMP_IMAGE_HEADER) != sizeof (BMP_COLOR_MAP) * ColorMapNum) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Calculate graphics image data address in the image
  //
  Image         = ((UINT8 *) BmpImage) + BmpHeader->ImageOffset;
  ImageHeader   = Image;

  //
  // Calculate the BltBuffer needed size.
  //
  BltBufferSize = MultU64x32 ((UINT64) BmpHeader->PixelWidth, BmpHeader->PixelHeight);
  //
  // Ensure the BltBufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) doesn't overflow
  //
  if (BltBufferSize > DivU64x32 ((UINTN) ~0, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    return EFI_UNSUPPORTED;
  }
  BltBufferSize = MultU64x32 (BltBufferSize, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  IsAllocated   = FALSE;
  if (*GopBlt == NULL) {
    //
    // GopBlt is not allocated by caller.
    //
    *GopBltSize = (UINTN) BltBufferSize;
    *GopBlt     = malloc (*GopBltSize);
    IsAllocated = TRUE;
    if (*GopBlt == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    //
    // GopBlt has been allocated by caller.
    //
    if (*GopBltSize < (UINTN) BltBufferSize) {
      *GopBltSize = (UINTN) BltBufferSize;
      return EFI_BUFFER_TOO_SMALL;
    }
  }

  *PixelWidth   = BmpHeader->PixelWidth;
  *PixelHeight  = BmpHeader->PixelHeight;

  //
  // Convert image from BMP to Blt buffer format
  //
  BltBuffer = *GopBlt;
  for (Height = 0; Height < BmpHeader->PixelHeight; Height++) {
    Blt = &BltBuffer[(BmpHeader->PixelHeight - Height - 1) * BmpHeader->PixelWidth];
    for (Width = 0; Width < BmpHeader->PixelWidth; Width++, Image++, Blt++) {
      switch (BmpHeader->BitPerPixel) {
        case 1:
          //
          // Convert 1-bit (2 colors) BMP to 24-bit color
          //
          for (Index = 0; Index < 8 && Width < BmpHeader->PixelWidth; Index++) {
            Blt->Red    = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Red;
            Blt->Green  = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Green;
            Blt->Blue   = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Blue;
            Blt++;
            Width++;
          }
  
          Blt--;
          Width--;
          break;
  
        case 4:
          //
          //   Convert 4-bit (16 colors) BMP Palette to 24-bit color
          //
          Index       = (*Image) >> 4;
          Blt->Red    = BmpColorMap[Index].Red;
          Blt->Green  = BmpColorMap[Index].Green;
          Blt->Blue   = BmpColorMap[Index].Blue;
          if (Width < (BmpHeader->PixelWidth - 1)) {
            Blt++;
            Width++;
            Index       = (*Image) & 0x0f;
            Blt->Red    = BmpColorMap[Index].Red;
            Blt->Green  = BmpColorMap[Index].Green;
            Blt->Blue   = BmpColorMap[Index].Blue;
          }
          break;
  
        case 8:
          //
          // Convert 8-bit (256 colors) BMP Palette to 24-bit color
          //
          Blt->Red    = BmpColorMap[*Image].Red;
          Blt->Green  = BmpColorMap[*Image].Green;
          Blt->Blue   = BmpColorMap[*Image].Blue;
          break;

        case 24:
          //
          // It is 24-bit BMP.
          //
          Blt->Blue   = *Image++;
          Blt->Green  = *Image++;
          Blt->Red    = *Image;
          break;

        default:
          //
          // Other bit format BMP is not supported.
          //
          if (IsAllocated) {
            free (*GopBlt);
            *GopBlt = NULL;
          }
          return EFI_UNSUPPORTED;
          break;
      };

    }

    ImageIndex = (UINTN) (Image - ImageHeader);
    if ((ImageIndex % 4) != 0) {
      //
      // Bmp Image starts each row on a 32-bit boundary!
      //
      Image = Image + (4 - (ImageIndex % 4));
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS CopyBitmapToBitmap (
             IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Dest, 
             IN UINTN DestWidth, 
             IN UINTN DestHeight, 
             IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Src, 
             IN UINTN SrcWidth,
             IN UINTN SrcHeight,
             IN UINTN OffsetX,
             IN UINTN OffsetY
             ) 
{
  UINTN CurrentX;
  UINTN CurrentY;
  UINTN SourceX;
  UINTN SourceY;

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Current;
  Current.Red = 0;
  Current.Green = 0;
  Current.Blue = 0;

  SourceX = 0;
  SourceY = 0;
  CurrentX = OffsetX;
  CurrentY = OffsetY;

  if (CurrentX > DestWidth || CurrentY > DestHeight) {
    return EFI_INVALID_PARAMETER;
  }

  //copy image
  //if source would go over edge of destination buffer, then clip edges

  while (SourceY < SrcHeight && CurrentY < DestHeight) {
    
    while (SourceX < SrcWidth && CurrentX < DestWidth) {
      Current = Src[(SourceY * SrcWidth) + SourceX];

      if (! (Current.Red == 0 && Current.Green == 0 && Current.Blue == 0)) {
        Dest[(CurrentY * DestWidth) + CurrentX] = Src[(SourceY * SrcWidth) + SourceX];
      }
        SourceX++;
        CurrentX++;
    }
    SourceX = 0;
    CurrentX = OffsetX;

    SourceY++;
    CurrentY++;

  }

  return EFI_SUCCESS;

}


EFI_STATUS RotateUp ()
{
  if (mMidHeight >= MaxHeight) {
    return EFI_SUCCESS;   
  } else {
    mMidHeight ++;
    return EFI_SUCCESS;
  }
}

EFI_STATUS RotateDown ()
{
  if (mMidHeight == 0) {
    return EFI_SUCCESS;
  } else {
    mMidHeight --;
    return EFI_SUCCESS;
  }
}
// Load a file into memory.
EFI_STATUS
LoadFile(
  IN CONST CHAR8    *FilePath,
  OUT VOID          **File,
  OUT UINT32        *FileSize
)
{
  FILE *f = fopen(FilePath, "rb");
  if (f == NULL) {
    return EFI_NOT_FOUND;
  }

  fseek(f, 0, SEEK_END);
  *FileSize = ftell(f);
  fseek(f, 0, SEEK_SET);  //same as rewind(f);

  *File = malloc(*FileSize);
  if (*File == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (fread(*File, *FileSize, 1, f) < 1) {
    return EFI_DEVICE_ERROR;
  }
  fclose(f);
  return EFI_SUCCESS;
}

// Load a bitmap file into memory and then convert it into a HII image.
BOOLEAN
LoadBitmapFile(
  IN CONST CHAR8    *BmpFilePath,
  OUT VOID          **Blt,
  OUT UINTN         *Height,
  OUT UINTN         *Width
)
{
  EFI_STATUS        Status;
  UINT8*            ImageData;
  UINT32            ImageSize;
  UINTN             BltSize;

  Status = LoadFile(BmpFilePath, (VOID **)&ImageData, &ImageSize);
  if (EFI_ERROR(Status)) {             //print error messages
    return FALSE;
  }

  *Blt = NULL;
  BltSize = 0;
  Status = ConvertBmpToGopBlt(
            ImageData,
            ImageSize,
            (VOID **)Blt,
            &BltSize,
            Height,
            Width
            );
  if (EFI_ERROR(Status)) {             //print error message
    return FALSE;
  }

  free(ImageData);
  return TRUE;
}
BOOLEAN
InitGop(VOID)
{
  EFI_STATUS        Status;
  UINTN             Handle;
  UINTN             HandleCount;
  EFI_HANDLE        *Handles;
  EFI_DEVICE_PATH_PROTOCOL *Dpp;

  Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &HandleCount, &Handles);
  if (EFI_ERROR(Status)) {
    Print(L"graphics not supported.\n");
    return FALSE;
  }

  mGraphicsOutput = NULL;
  for (Handle = 0; Handle < HandleCount; Handle++) {
    Status = gBS->HandleProtocol(Handles[Handle], &gEfiDevicePathProtocolGuid, (VOID **)&Dpp);
    if (EFI_ERROR(Status)) {
      Status = gBS->HandleProtocol(Handles[Handle], &gEfiGraphicsOutputProtocolGuid, (VOID **)&mGraphicsOutput);
      if (!EFI_ERROR(Status)) {
        break;
      }
    }
  }
  FreePool(Handles);
  if (mGraphicsOutput == NULL) {
    Print(L"graphics console not found.\n");
    return FALSE;
  }

  return TRUE;
}

//function that sets up all data for all images (everything but Blt())
//call this function before calling StretchImage()
// EFI_STATUS SetUpImages (
//              IN CONST EFI_GUID *BgLogoFile, 
//              IN CONST EFI_GUID *MidLogoFile, 
//              IN CONST EFI_GUID *FrontLogoFile
//              )   
// {
//   EFI_STATUS Status;
//   UINT8* ImageData;
//   UINTN ImageSize;
//   UINTN BltSize;
//   //no coordinate X or Y...determined in DisplayImage section

//   gBS->LocateProtocol (&gEfiHiiImageProtocolGuid, NULL, &mHiiImage);

//   Status = gBS->HandleProtocol (gST->ConsoleOutHandle,  &gEfiGraphicsOutputProtocolGuid, (VOID**) &mGraphicsOutput);
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   //BG image
//   Status = GetSectionFromAnyFv (BgLogoFile, EFI_SECTION_RAW, 0, (VOID**) &ImageData, &ImageSize);
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   Status = ConvertBmpToGopBlt (
//              ImageData,
//              ImageSize,
//              (VOID **) &mBgBlt,
//              &BltSize,
//              &mBgHeight,
//              &mBgWidth
//              );
//   //now Bg BltBuffer, Height, and Width are set
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   //Mid Image
//   Status = GetSectionFromAnyFv (MidLogoFile, EFI_SECTION_RAW, 0, (VOID **) &ImageData, &ImageSize);
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   Status = ConvertBmpToGopBlt (
//              ImageData,
//              ImageSize,
//              (VOID **) &mMidBlt,
//              &BltSize,
//              &mMidHeight,
//              &mMidWidth
//              );
//   //now Mid BltBuffer, Height, and Width are set
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   MaxHeight = mMidHeight;

//   //Front Image
//   Status = GetSectionFromAnyFv (FrontLogoFile, EFI_SECTION_RAW, 0, (VOID **) &ImageData, &ImageSize);
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }

//   Status = ConvertBmpToGopBlt (
//              ImageData,
//              ImageSize,
//              (VOID **) &mFrontBlt,
//              &BltSize,
//              &mFrontHeight,
//              &mFrontWidth
//              );
//   //now Front BltBuffer, Height, and Width are set
//   if (EFI_ERROR (Status)) {
//     return EFI_UNSUPPORTED;
//   }
  
//   return EFI_SUCCESS;
// }
/**
  Sets all the values associated with images, including all Blt Buffers, height 
  and width.

  @retval EFI_SUCCESS

**/
EFI_STATUS 
SetUpImages (  
  IN CONST CHAR8    *BgLogoFilePath,
  IN CONST CHAR8    *MidLogoFilePath,
  IN CONST CHAR8    *FrontLogoFilePath)
{
  // Initialize the graphics support.
  if (!InitGop()) {
    return EFI_UNSUPPORTED;
  }

  // Load the background bitmap.
  if (!LoadBitmapFile(BgLogoFilePath, &mBgBlt, &mBgHeight, &mBgWidth)) {
    return EFI_UNSUPPORTED;
  }

  
  if (!LoadBitmapFile(MidLogoFilePath, &mMidBlt, &mMidHeight, &mMidWidth)) {
    return EFI_UNSUPPORTED;
  }
  MaxHeight = mMidHeight;
 
  if (!LoadBitmapFile(FrontLogoFilePath, &mFrontBlt, &mFrontHeight, &mFrontWidth)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
//new display image function
EFI_STATUS DisplayImageStack ()   
{
  UINTN CoordinateX;
  UINTN CoordinateY;
    
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  
  Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) malloc(mBgWidth * mBgHeight * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  memset (Blt, 0, mBgWidth * mBgHeight * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  CopyBitmapToBitmap (Blt, mBgWidth, mBgHeight, mBgBlt, mBgWidth, mBgHeight, 0, 0);

  CoordinateX = (mBgWidth - mMidWidth) / 2;
  CoordinateY = 15 + (MaxHeight - mMidHeight);
  CopyBitmapToBitmap (Blt, mBgWidth, mBgHeight, mMidBlt, mMidWidth, mMidHeight, CoordinateX, CoordinateY);

  CoordinateX = (mBgWidth - mFrontWidth) / 2;
  CoordinateY = 15;
  CopyBitmapToBitmap (Blt, mBgWidth, mBgHeight, mFrontBlt, mFrontWidth, mFrontHeight, CoordinateX, CoordinateY);

  // CoordinateX = (mGraphicsOutput->Mode->Info->HorizontalResolution / 2) - (mBgWidth / 2);
  // CoordinateY = (mGraphicsOutput->Mode->Info->VerticalResolution / 2) - (mBgHeight / 2);
  CoordinateX = 10;
  CoordinateY =10;

  mGraphicsOutput->Blt (
                    mGraphicsOutput,
                    Blt,
                    EfiBltBufferToVideo,
                    0,
                    0,
                    CoordinateX,
                    CoordinateY,
                    mBgWidth,
                    mBgHeight,
                    mBgWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                    );
               
  return EFI_SUCCESS;

}


//displays initial stacked image
//input loop
  //re-draw stack
EFI_STATUS StretchImage () 
{
  EFI_INPUT_KEY Key;
  UINTN index;
  
  //clear screen
  gST->ConOut->Reset (gST->ConOut, FALSE);

  DisplayImageStack();
  
  //input loop
  do {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &index);
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    if (Key.ScanCode != SCAN_NULL) {
      if (Key.ScanCode == SCAN_UP) {
        // Print(L"UP");
        RotateUp ();
      } else if (Key.ScanCode == SCAN_DOWN) {
        // Print(L"Down");
        RotateDown();
      }
      else if (Key.ScanCode == SCAN_ESC)
        break;
    }

    DisplayImageStack();

  } while (Key.ScanCode != SCAN_END);

  //clear screen after app exits
  gST->ConOut->Reset (gST->ConOut, FALSE); 

  return EFI_SUCCESS;
}