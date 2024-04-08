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
#include "Common.h"
#include "FileRW.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>


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
  // {
  //   //测试文件读写
  //   EFI_STATUS Status;
  //   EFI_FILE_PROTOCOL *file;
  //   CHAR16 *filename = L"readme.txt";
  //   UINTN bufLength = 20;
  //   CHAR16 *bufStr = L"Hello,UEFI. I'm Robin";
  //   CHAR16 bufStr1[200];

  //   Print(L"--open readme.txt--\n");
  //   Status = OpenFile(&file, filename, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);

  //   if(Status ==EFI_SUCCESS)
  //   {
  //     Print(L"Open file: %s success!\n", filename);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   else
  //     Print(L"Open file: %s fail!\n", filename);

  //   bufLength = StrLen(bufStr)*2+2;

  //   Status = WriteFile(file, &bufLength, bufStr);
  //   if(Status ==EFI_SUCCESS)
  //   {
  //     Print(L"Write file: %s success!\n", filename);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   else
  //     Print(L"Write file: %s fail!\n", filename);
    
  //   Status = SetFilePosition(file, 0);
  //   if(Status ==EFI_SUCCESS)
  //   {
  //     Print(L"SetFilePosition file: %s success!\n", filename);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   else
  //     Print(L"SetFilePosition file: %s fail!\n", filename);

  //   bufLength = 10;
  //   Status=ReadFile(file,&bufLength, bufStr1);
  //    if(Status ==EFI_SUCCESS)
  //   {
  //     Print(L"Read file: %s success!\n", filename);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   else
  //     Print(L"Read file: %s fail!\n", filename);
    
  //   Print(L"bufLength=%d,bufStr1=%s\n", bufLength, bufStr1);

  //   file->Close(file);
  // }

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
