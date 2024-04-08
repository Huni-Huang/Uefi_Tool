/**
  Handle Graphical Output

  Copyright (C) 2017 Tim Lewis. 
  This program and the accompanying materials are licensed and made available 
  under the terms and conditions of the BSD License which accompanies this 
  distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <stdlib.h>
#include <stdio.h>

#include <Uefi.h>

#include <IndustryStandard/Bmp.h>

#include "Library/BaseLib.h"
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>


EFI_GRAPHICS_OUTPUT_PROTOCOL *mGraphicsOutput;

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
ConvertBmpToGopBlt(
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

  if (sizeof(BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

  BmpHeader = (BMP_IMAGE_HEADER *)BmpImage;

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
  if (BmpHeader->HeaderSize != sizeof(BMP_IMAGE_HEADER) - OFFSET_OF(BMP_IMAGE_HEADER, HeaderSize)) {
    return EFI_UNSUPPORTED;
  }

  //
  // The data size in each line must be 4 byte alignment.
  //
  DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
  BltBufferSize = MultU64x32(DataSizePerLine, BmpHeader->PixelHeight);
  if (BltBufferSize > (UINT32)~0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BmpHeader->Size != BmpImageSize) ||
    (BmpHeader->Size < BmpHeader->ImageOffset)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Calculate Color Map offset in the image.
  //
  Image = BmpImage;
  BmpColorMap = (BMP_COLOR_MAP *)(Image + sizeof(BMP_IMAGE_HEADER));
  if (BmpHeader->ImageOffset < sizeof(BMP_IMAGE_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BmpHeader->ImageOffset > sizeof(BMP_IMAGE_HEADER)) {
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
    if (BmpHeader->ImageOffset - sizeof(BMP_IMAGE_HEADER) != sizeof(BMP_COLOR_MAP) * ColorMapNum) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Calculate graphics image data address in the image
  //
  Image = ((UINT8 *)BmpImage) + BmpHeader->ImageOffset;
  ImageHeader = Image;

  //
  // Calculate the BltBuffer needed size.
  //
  BltBufferSize = MultU64x32((UINT64)BmpHeader->PixelWidth, BmpHeader->PixelHeight);
  //
  // Ensure the BltBufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) doesn't overflow
  //
  if (BltBufferSize > DivU64x32((UINTN)~0, sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    return EFI_UNSUPPORTED;
  }
  BltBufferSize = MultU64x32(BltBufferSize, sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  IsAllocated = FALSE;
  if (*GopBlt == NULL) {
    //
    // GopBlt is not allocated by caller.
    //
    *GopBltSize = (UINTN)BltBufferSize;
    *GopBlt = malloc(*GopBltSize);
    IsAllocated = TRUE;
    if (*GopBlt == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    //
    // GopBlt has been allocated by caller.
    //
    if (*GopBltSize < (UINTN)BltBufferSize) {
      *GopBltSize = (UINTN)BltBufferSize;
      return EFI_BUFFER_TOO_SMALL;
    }
  }

  *PixelWidth = BmpHeader->PixelWidth;
  *PixelHeight = BmpHeader->PixelHeight;

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
            Blt->Red = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Red;
            Blt->Green = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Green;
            Blt->Blue = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Blue;
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
          Index = (*Image) >> 4;
          Blt->Red = BmpColorMap[Index].Red;
          Blt->Green = BmpColorMap[Index].Green;
          Blt->Blue = BmpColorMap[Index].Blue;
          if (Width < (BmpHeader->PixelWidth - 1)) {
            Blt++;
            Width++;
            Index = (*Image) & 0x0f;
            Blt->Red = BmpColorMap[Index].Red;
            Blt->Green = BmpColorMap[Index].Green;
            Blt->Blue = BmpColorMap[Index].Blue;
          }
          break;

        case 8:
          //
          // Convert 8-bit (256 colors) BMP Palette to 24-bit color
          //
          Blt->Red = BmpColorMap[*Image].Red;
          Blt->Green = BmpColorMap[*Image].Green;
          Blt->Blue = BmpColorMap[*Image].Blue;
          break;

        case 24:
          //
          // It is 24-bit BMP.
          //
          Blt->Blue = *Image++;
          Blt->Green = *Image++;
          Blt->Red = *Image;
          break;

        default:
          //
          // Other bit format BMP is not supported.
          //
          if (IsAllocated) {
            free(*GopBlt);
            *GopBlt = NULL;
          }
          return EFI_UNSUPPORTED;
          break;
      };

    }

    ImageIndex = (UINTN)(Image - ImageHeader);
    if ((ImageIndex % 4) != 0) {
      //
      // Bmp Image starts each row on a 32-bit boundary!
      //
      Image = Image + (4 - (ImageIndex % 4));
    }
  }

  return EFI_SUCCESS;
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

/**
Copies a Bitmap buffer onto given coordinates of another buffer
If the source image would go beyond the edge of the destination buffer, the
edges of the source image are instead clipped

  @param  Dest        Destination buffer
  @param  DestWidth   Width of destination buffer
  @param  DestHeight  Height of destination buffer
  @param  Src         Source buffer
  @param  SrcWidth    Width of source buffer
  @param  SrcHeight   Height of source buffer
  @param  OffsetX     number of pixels in the X-direction the source image
                      should be offset
  @param  OffsetY     number of pixels in the Y-direction the source image
                      should be offset

@retval

**/
EFI_STATUS
CopyBitmapToBitmap(
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Dest,
  IN UINTN          DestWidth,
  IN UINTN          DestHeight,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Src,
  IN UINTN          SrcWidth,
  IN UINTN          SrcHeight,
  IN UINTN          OffsetX,
  IN UINTN          OffsetY
  )
{
  UINTN             CurrentX;
  UINTN             CurrentY;
  UINTN             SourceX;
  UINTN             SourceY;

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

      if (!(Current.Red == 0 && Current.Green == 0 && Current.Blue == 0)) {
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
