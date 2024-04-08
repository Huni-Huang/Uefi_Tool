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
#include "Pictures.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>

#include <Pi/PiFirmwareFile.h>


#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>

#define MyPic                      L"luobing.bmp"		//where the my bmp located 
// EFI_STATUS ShowBmp (void);


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
    VOID *buffBMP;   //存储图像的缓冲器
    BMP_IMAGE_HEADER imageHeader;
    EFI_FILE_PROTOCOL *file;
    UINT16 *temPtr;
    UINT16 index;

    
		
    

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
    
		SwitchGraphicsMode(TRUE);
    SetMyMode(0x3);
		SetBKG(&(gColorTable[3]));
    // {
    //   UINTN                               CoordinateX;
		//   UINTN                               CoordinateY;
    //   EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *BltBuffer = NULL;
    //   UINTN                               BltSize;
		//   UINTN                               Height;
		//   UINTN                               Width;
    //   WaitKey();
    //   Status = ConvertBmpToGopBlt (buffBMP,bufLength,(VOID **) &BltBuffer,&BltSize,&Height,&Width);
    //   Print(L"Status of ConvertBmpToGopBlt=%d\n",Status);
    //   WaitKey();
    //   CoordinateX = (UINTN) (gGraphicsOutput->Mode->Info->HorizontalResolution / 2) - (Width / 2);
    //   CoordinateY = (UINTN) (gGraphicsOutput->Mode->Info->VerticalResolution / 2) - (Height / 2);   
    //   Status = gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,CoordinateX,CoordinateY,Width,Height,Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
      
    //   WaitKey();
    // // file->Close(file);
    //   FreePool(buffBMP);
    //   FreePool(BltBuffer);
    // }
    SetBKG(&(gColorTable[3]));
    WaitKey();
    //3 使用putpixel画图(会很慢)
    {
      UINTN i, j;
      UINTN maxX, maxY;
      EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
      UINT8 bgr[3];

      maxX = imageHeader.PixelWidth-1;
      maxY = imageHeader.PixelHeight-1;
      SetFilePosition(file, imageHeader.ImageOffset); //文件最开始处;
      
      for(i=0;i<=maxY;i++)
		    for(j=0;j<=maxX;j++)
        {
         // bufLength = (UINTN)(sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));  //EFI_GRAPHICS_OUTPUT_BLT_PIXEL刚好和BMP_COLOR_MAP同样
          bufLength = 3;
          ReadFile(file,&bufLength, bgr);
          color.Blue = bgr[0];
          color.Green = bgr[1];
          color.Red = bgr[2];
          color.Reserved = 0;
          putpixel(j,maxY-i,&color);
        }

      WaitKey(); 
    }
    file->Close(file);
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
		// //图形显示测试
		
		// //DisplayModeInformation();
		// UINT32 ModeIndex;
		// for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		// {
		// 	DisplaySpecifyModeMessage(ModeIndex);
		// 	WaitKey();
		// }
	
		// SwitchGraphicsMode(TRUE);
		// SetBKG(&(gColorTable[3]));
		// WaitKey();
		// //rectblock(0,0,799,599-1,&(gColorTable[1])); //white
		// rectangle(100,100,150,150,&(gColorTable[8]));//red
		// circle(300,300,40,&(gColorTable[1]));//blue
		// rectblock(450,450,500,500,&(gColorTable[6]));
		// WaitKey();
		// // ShowBmp();
		// WaitKey();
		// SetMyMode(0x0);
		// SwitchGraphicsMode(FALSE);
	}
  return(0);
}


// //Name: ShowBmp
// //Input: 
// //Output: 
// //Descriptor:
// EFI_STATUS ShowBmp (void)
// {
// 	EFI_STATUS                         Status;
// 	UINTN                              HandleNo = 0;
// 	UINTN                              HandleSum = 0;
// 	EFI_HANDLE                         *SimpleFileSystemHandles = NULL;
// 	EFI_DEVICE_PATH_PROTOCOL           *DevicePath = NULL;
// 	//get the handles which supports SimpleFileSystemProtocol
// 	Status = gBS->LocateHandleBuffer(
// 		ByProtocol,
// 		&gEfiSimpleFileSystemProtocolGuid,
// 		NULL,
// 		&HandleSum,
// 		&SimpleFileSystemHandles
// 		);
// 	for (HandleNo = 0; HandleNo < HandleSum; HandleNo++)
// 	{
// 		//FileDevicePath(...) is in DevicePath.h
// 		DevicePath = FileDevicePath(SimpleFileSystemHandles[HandleNo],MyPic);
// 		if(DevicePath != NULL)
// 		{
// 			UINTN                               ImageSize = 0;
// 			UINT32                              AuthenticationStatus;
// 			VOID                                *ImageData = NULL;
// 			EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *BltBuffer = NULL;
// 			UINTN                               BltSize;
// 			UINTN                               Height;
// 			UINTN                               Width;
// 			UINTN                               CoordinateX;
// 			UINTN                               CoordinateY;
// 			//GetFileBufferByFilePath(...) is in DxeServicesLib.h
// 			ImageData = GetFileBufferByFilePath(0,DevicePath,&ImageSize,&AuthenticationStatus);
// 			//change ImageData to BltBuffer,which would be dealt by GraphicOutputProtocol 
// 			//ConvertBmpToGopBlt(...) is in BdsConsole.c
// 			Status = ConvertBmpToGopBlt (ImageData,ImageSize,(VOID **) &BltBuffer,&BltSize,&Height,&Width);
// 			CoordinateX = (UINTN) (gGraphicsOutput->Mode->Info->HorizontalResolution / 2) - (Width / 2);
// 			CoordinateY = (UINTN) (gGraphicsOutput->Mode->Info->VerticalResolution / 2) - (Height / 2);
// 			//Output the image to the srceen
// 			Status = gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,CoordinateX,CoordinateY,Width,Height,Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
// 			gBS->FreePool(ImageData);
// 			gBS->FreePool(BltBuffer);
// 		}
// 	}
// 	gBS->FreePool(DevicePath);
// 	return EFI_SUCCESS;
// }

