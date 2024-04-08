/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include "common.h"
#include "FileRW.h"
#include "KeyBoard.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>

#include <Pi/PiFirmwareFile.h>
#include <IndustryStandard/Bmp.h> //for bmp
#include <Library/MemoryAllocationLib.h> //AllocatePool function...
#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>

#define MyPic                      L"luobing.bmp"		//where the my bmp located 
EFI_STATUS ShowBmp (void);
EFI_STATUS
ConvertBmpToGopBlt (
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
     OUT UINTN     *PixelHeight,
     OUT UINTN     *PixelWidth
  );

extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  UINT64 flag;
  UINTN i;
  // Print(L"Hello there fellow Programmer.\n");
  // Print(L"Welcome to the world of EDK II.\n");
  Print(L"Argc=%d\n", Argc);
  for (i = 0; i < Argc;i++)
  {
    Print(L"Argv[%d]= %s\n", i,Argv[i]);
  }
    
        flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO);
	Print(L"flag=%x\n",flag);
	WaitKey();
  {
    //1 将图像数据读入缓冲器
    EFI_STATUS Status;
    UINT32 ModeIndex;
    VOID *buffBMP;   //存储图像的缓冲器
    BMP_IMAGE_HEADER imageHeader;
    EFI_FILE_PROTOCOL *file;
    UINT16 *temPtr;
    UINT16 index;

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *BltBuffer = NULL;
		UINTN                               BltSize;
		UINTN                               Height;
		UINTN                               Width;
    UINTN                               CoordinateX;
		UINTN                               CoordinateY;

    UINTN bufLength = sizeof(BMP_IMAGE_HEADER);


    //1-1 读入头部数据
    Print(L"--open %s --\n",Argv[1]);  //命令行参数传入bmp文件名称
    Status = OpenFile(&file, Argv[1], EFI_FILE_MODE_READ);
    if(Status ==EFI_SUCCESS)
    {
      Print(L"Open file: %s success!\n", Argv[1]);
      WaitKey();
      // file->Close(file);
    }

    Status=ReadFile(file,&bufLength, &imageHeader);
    if(Status ==EFI_SUCCESS)
    {
      Print(L"Read file: %s success!\n", Argv[1]);
      Print(L"BMP size=0x%x\n",imageHeader.Size);
      WaitKey();
      // file->Close(file);
    }
    //1-2 申请内存，并将bmp文件读入内存中
    buffBMP = AllocateZeroPool(imageHeader.Size);
    if(buffBMP  !=  NULL)
    {
      Print(L"Allocate memory to buffBMP success!\n");
    }
    else
    {
      Print(L"Allocate memory to buffBMP fail!\n");
    }
    SetFilePosition(file, 0); //文件最开始处;
    bufLength = (UINTN)(imageHeader.Size);
    Status=ReadFile(file,&bufLength, buffBMP);
    if(Status ==EFI_SUCCESS)
    {
      Print(L"Read file  %s to buffbmp success!\n", Argv[1]);
      temPtr = (UINT16 *)buffBMP;
      Print(L"buffBMP:\n");
      for (index = 0; index < 26; index++)
      {
          Print(L"0x%x  ", temPtr[index]);
      }
      Print(L"buffBMP end\n");  
    }

    //2 进入图形模式，并转换数据到bltbuffer;
    
		for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		{
			DisplaySpecifyModeMessage(ModeIndex);
			WaitKey();
		}
	
		SwitchGraphicsMode(TRUE);
    SetMyMode(0x3);
		SetBKG(&(gColorTable[3]));
		WaitKey();
    Status = ConvertBmpToGopBlt (buffBMP,bufLength,(VOID **) &BltBuffer,&BltSize,&Height,&Width);
     Print(L"Status of ConvertBmpToGopBlt=%d\n",Status);
     WaitKey();
     CoordinateX = (UINTN) (gGraphicsOutput->Mode->Info->HorizontalResolution / 2) - (Width / 2);
		 CoordinateY = (UINTN) (gGraphicsOutput->Mode->Info->VerticalResolution / 2) - (Height / 2);
     
    Status = gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,CoordinateX,CoordinateY,Width,Height,Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    // Print(L"CoordinateX=0x%x,CoordinateY=0x%x\n",CoordinateX,CoordinateY);
    // Print(L"Mode=0x%x,HorizontalResolution=0x%x,VerticalResolution=0x%x\n",gGraphicsOutput->Mode->Mode,gGraphicsOutput->Mode->Info->HorizontalResolution,gGraphicsOutput->Mode->Info->VerticalResolution);
    // Print(L"Width=0x%x,Height=0x%x\n",Width,Height);
    WaitKey();
    file->Close(file);
    FreePool(buffBMP);
    FreePool(BltBuffer);
  }
  {
    // //测试文件读写
    // EFI_STATUS Status;
    // EFI_FILE_PROTOCOL *file;
    // CHAR16 *filename = L"readme.txt";
    // UINTN bufLength = 20;
    // CHAR16 *bufStr = L"Hello,UEFI. I'm Robin";
    // CHAR16 bufStr1[200];

    // Print(L"--open readme.txt--\n");
    // Status = OpenFile(&file, filename, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);

    // if(Status ==EFI_SUCCESS)
    // {
    //   Print(L"Open file: %s success!\n", filename);
    //   WaitKey();
    //   // file->Close(file);
    // }
    // else
    //   Print(L"Open file: %s fail!\n", filename);

    // bufLength = StrLen(bufStr)*2+2;

    // Status = WriteFile(file, &bufLength, bufStr);
    // if(Status ==EFI_SUCCESS)
    // {
    //   Print(L"Write file: %s success!\n", filename);
    //   WaitKey();
    //   // file->Close(file);
    // }
    // else
    //   Print(L"Write file: %s fail!\n", filename);
    
    // Status = SetFilePosition(file, 0);
    // if(Status ==EFI_SUCCESS)
    // {
    //   Print(L"SetFilePosition file: %s success!\n", filename);
    //   WaitKey();
    //   // file->Close(file);
    // }
    // else
    //   Print(L"SetFilePosition file: %s fail!\n", filename);

    // bufLength = 10;
    // Status=ReadFile(file,&bufLength, bufStr1);
    //  if(Status ==EFI_SUCCESS)
    // {
    //   Print(L"Read file: %s success!\n", filename);
    //   WaitKey();
    //   // file->Close(file);
    // }
    // else
    //   Print(L"Read file: %s fail!\n", filename);
    
    // Print(L"bufLength=%d,bufStr1=%s\n", bufLength, bufStr1);

    // file->Close(file);
  }

{
		//图形显示测试
		
		//DisplayModeInformation();
		UINT32 ModeIndex;
		for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		{
			DisplaySpecifyModeMessage(ModeIndex);
			WaitKey();
		}
	
		SwitchGraphicsMode(TRUE);
		SetBKG(&(gColorTable[3]));
		WaitKey();
		//rectblock(0,0,799,599-1,&(gColorTable[1])); //white
		rectangle(100,100,150,150,&(gColorTable[8]));//red
		circle(300,300,40,&(gColorTable[1]));//blue
		rectblock(450,450,500,500,&(gColorTable[6]));
		WaitKey();
		// ShowBmp();
		WaitKey();
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);
	}
  return(0);
}


//Name: ShowBmp
//Input: 
//Output: 
//Descriptor:
EFI_STATUS ShowBmp (void)
{
	EFI_STATUS                         Status;
	UINTN                              HandleNo = 0;
	UINTN                              HandleSum = 0;
	EFI_HANDLE                         *SimpleFileSystemHandles = NULL;
	EFI_DEVICE_PATH_PROTOCOL           *DevicePath = NULL;
	//get the handles which supports SimpleFileSystemProtocol
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiSimpleFileSystemProtocolGuid,
		NULL,
		&HandleSum,
		&SimpleFileSystemHandles
		);
	for (HandleNo = 0; HandleNo < HandleSum; HandleNo++)
	{
		//FileDevicePath(...) is in DevicePath.h
		DevicePath = FileDevicePath(SimpleFileSystemHandles[HandleNo],MyPic);
		if(DevicePath != NULL)
		{
			UINTN                               ImageSize = 0;
			UINT32                              AuthenticationStatus;
			VOID                                *ImageData = NULL;
			EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *BltBuffer = NULL;
			UINTN                               BltSize;
			UINTN                               Height;
			UINTN                               Width;
			UINTN                               CoordinateX;
			UINTN                               CoordinateY;
			//GetFileBufferByFilePath(...) is in DxeServicesLib.h
			ImageData = GetFileBufferByFilePath(0,DevicePath,&ImageSize,&AuthenticationStatus);
			//change ImageData to BltBuffer,which would be dealt by GraphicOutputProtocol 
			//ConvertBmpToGopBlt(...) is in BdsConsole.c
			Status = ConvertBmpToGopBlt (ImageData,ImageSize,(VOID **) &BltBuffer,&BltSize,&Height,&Width);
			CoordinateX = (UINTN) (gGraphicsOutput->Mode->Info->HorizontalResolution / 2) - (Width / 2);
			CoordinateY = (UINTN) (gGraphicsOutput->Mode->Info->VerticalResolution / 2) - (Height / 2);
			//Output the image to the srceen
			Status = gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,CoordinateX,CoordinateY,Width,Height,Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
			gBS->FreePool(ImageData);
			gBS->FreePool(BltBuffer);
		}
	}
	gBS->FreePool(DevicePath);
	return EFI_SUCCESS;
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
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
     OUT UINTN     *PixelHeight,
     OUT UINTN     *PixelWidth
  )
{
  UINT8                         *Image;
  UINT8                         *ImageHeader;
  BMP_IMAGE_HEADER              *BmpHeader;
  BMP_COLOR_MAP                 *BmpColorMap;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINT64                        BltBufferSize;
  UINTN                         Index;
  UINTN                         Height;
  UINTN                         Width;
  UINTN                         ImageIndex;
  UINT32                        DataSizePerLine;
  BOOLEAN                       IsAllocated;
  UINT32                        ColorMapNum;

  if (sizeof (BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

  BmpHeader = (BMP_IMAGE_HEADER *) BmpImage;

  // {
  //   //2019-06-14 15:50:43 luobing lbdebug
  //   UINT16 * temPtr;
  //   UINTN index;
  //   temPtr = (UINT16 *)BmpHeader;
  //     Print(L"ConvertBmpToGopBlt BmpHeader:\n");
  //     for (index = 0; index < 26; index++)
  //     {
  //         Print(L"0x%x  ", temPtr[index]);
  //     }
      
  //     Print(L"ConvertBmpToGopBlt BmpHeader end\n");
  //     Print(L"BmpHeader->PixelHeight=0x%x,BmpHeader->PixelWidth=0x%x\n", BmpHeader->PixelHeight, BmpHeader->PixelWidth);
  // }

  if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M') {
    return EFI_UNSUPPORTED;
  }
  //  Print(L"lbdebug 01\n");
  //
  // Doesn't support compress.
  //
  if (BmpHeader->CompressionType != 0) {
    return EFI_UNSUPPORTED;
  }
//  Print(L"lbdebug 02\n");
  //
  // Only support BITMAPINFOHEADER format.
  // BITMAPFILEHEADER + BITMAPINFOHEADER = BMP_IMAGE_HEADER
  //
  if (BmpHeader->HeaderSize != sizeof (BMP_IMAGE_HEADER) - OFFSET_OF(BMP_IMAGE_HEADER, HeaderSize)) {
    return EFI_UNSUPPORTED;
  }
//  Print(L"lbdebug 03\n");
  //
  // The data size in each line must be 4 byte alignment.
  //
  DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
  BltBufferSize = MultU64x32 (DataSizePerLine, BmpHeader->PixelHeight);
  if (BltBufferSize > (UINT32) ~0) {
    return EFI_INVALID_PARAMETER;
  }
//  Print(L"lbdebug 04\n");
//  Print(L"BmpHeader->Size=0x%x,BmpImageSize=0x%x,BmpHeader->ImageOffset=0x%x,BmpHeader->PixelHeight * DataSizePerLine=0x%x,DataSizePerLine=0x%x\n",\
//        BmpHeader->Size, BmpImageSize, BmpHeader->ImageOffset, BmpHeader->PixelHeight * DataSizePerLine, DataSizePerLine);

//  if ((BmpHeader->Size != BmpImageSize) ||
//      (BmpHeader->Size < BmpHeader->ImageOffset) ||
//      (BmpHeader->Size - BmpHeader->ImageOffset != BmpHeader->PixelHeight * DataSizePerLine)) //此处计算发现错误,是我的图出现问题了
//  {
//    return EFI_INVALID_PARAMETER;
//   }
if ((BmpHeader->Size != BmpImageSize) ||
     (BmpHeader->Size < BmpHeader->ImageOffset) )
 {
   return EFI_INVALID_PARAMETER;
  }

  // Print(L"lbdebug end\n");
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
    *GopBlt     = AllocatePool (*GopBltSize);
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

  // //luobing add 2019-06-14 15:34:46 lbdebug
  // Print(L"ConvertBmpToGopBlt *PixelWidth=0x%x,*PixelHeight=0x%x\n", *PixelWidth, *PixelHeight);
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
        // Convert 4-bit (16 colors) BMP Palette to 24-bit color
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
          FreePool (*GopBlt);
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