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
//所有包含的目录到mdepkg下去找

#include "common.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Font.h"
#include "Window.h"
#include "PciRW.h"
#include "SerialPort.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#include <IndustryStandard/Bmp.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SimpleFileSystem.h>

#define MyPic                      L"MyLogo.bmp"		//where the my bmp located 

extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
//debug function
EFI_STATUS ListPCIMessage(void);
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
	UINT64 flag;

	
//	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Data: 2013-2-1 11:57:51\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
//	//测试键盘输入的例子，打印所有我需要的键盘信息
//	gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"please input key(ESC to exit):\n\r");
//调试pci
	flag=InintGloabalProtocols(S_TEXT_INPUT_EX|GRAPHICS_OUTPUT|PCI_ROOTBRIDGE_IO|PCI_IO);
	Print(L"flag=%x\n",flag);
//	WaitKey();

	{
		//串口测试
		EFI_STATUS Status;
		UINT8 lbtest[]="luobing,test...";
		UINT8 readData[1024];
		UINTN i,readLength=4;
    UINT32 controlBits;
    Status =		LocateSerialIO();  //gSeriaoIO定位到第一个串口
		readData[0]=0;
		
		DisplayAllSerialMode();
		if (EFI_ERROR (Status)) 
		{
			Print(L"Call LocateSerialIO, Can't find device!\n");
		}
		// WaitKey();
		Status=SendDataToSerial(sizeof(lbtest)/sizeof(UINT8),lbtest);
		WaitKey();
		Status=SendDataToSerial(sizeof(lbtest)/sizeof(UINT8),lbtest);
		WaitKey();

   

		while(readData[0]!='Q')
		{
      Delayms(2);
      Status = GetSerialControlBits(&controlBits);
      if (!EFI_ERROR (Status)) 
      {
        if ((controlBits & EFI_SERIAL_INPUT_BUFFER_EMPTY) != 0)
        {
          Status = EFI_NOT_READY;
        } 
        else 
        {
          // Status = EFI_SUCCESS;
          //获取数据
          if ((controlBits & EFI_SERIAL_DATA_SET_READY) != EFI_SERIAL_DATA_SET_READY)
            Delayms(2);

          readLength = 1000;
          SetMem(readData, 1024, 0);
          Status=GetDataFromSerial(&readLength,readData);
          Print(L"First char:0x%x(hex), %c(ascii)\n",readData[0],readData[0]);
          Print(L"All data: ");
          for (i = 0; i < readLength; i++)
            Print(L"%c", readData[i]);
          Print(L"\n");
        }
      } 
      // Delayms(100);
      // GetSerialControlBits(&controlBits);
      // if(!(controlBits & EFI_SERIAL_INPUT_BUFFER_EMPTY))
      // {
      //   Status=GetDataFromSerial(&readLength,readData);
      //   Print(L"0x%x: %c\n",readData[0],readData[0]);
      // }
      // Status=GetDataFromSerial(&readLength,readData);
			
			// if(readData[0]!=0)
			// {
			// 	Print(L"0x%x: %c\n",readData[0],readData[0]);
			// 	readLength=4;
			// 	readData[0]=0;
			// }
      
    }
    ResetSerialPort();
    Delayms(1000);
    Print(L"Exit Serial IO sample...!");
	}		
	return 0;
}



//Debug function=============================================================
//Name: ListPCIMessage(void)
//Input:
//Output:
EFI_STATUS ListPCIMessage(void)
{
	EFI_STATUS                         Status=EFI_SUCCESS;
	PCI_TYPE00 Pci;
	UINT16 i,j,k,count=0;
	
	Print(L"NO.  Bus. Dev. Func. | Vendor. Device. ClassCode\n");
	for(k=0;k<128;k++)//应该没有这么多总线吧
		for(i=0;i<=PCI_MAX_DEVICE;i++)
			for(j=0;j<=PCI_MAX_FUNC;j++)
			{
				Status = PciDevicePresent(gPCIRootBridgeIO,&Pci,(UINT8)k,(UINT8)i,(UINT8)j);
				if (Status == EFI_SUCCESS)		//Find device
				{
					++count;
					if((count%16) ==0)	WaitKey();		//lbdebug
					Print(L" %d    %x    %x    %x",count,(UINT8)k,(UINT8)i,(UINT8)j);
					Print(L"       %x       %x        %x",Pci.Hdr.VendorId,Pci.Hdr.DeviceId,Pci.Hdr.ClassCode[0]);
					Print(L"\n");
				}
			}
	return EFI_SUCCESS;
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