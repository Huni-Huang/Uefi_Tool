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
#include "PciRW.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
//debug function
EFI_STATUS ListPCIMessage(void);

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
//	EFI_STATUS      Status;
//	PCI_TYPE00 Pci;
	UINT64 flag;

	
	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2013-2-1 11:57:51\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
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
//调试pci
	flag=InintGloabalProtocols(S_TEXT_INPUT_EX|GRAPHICS_OUTPUT|PCI_ROOTBRIDGE_IO|PCI_IO);
	Print(L"flag=%x\n",flag);
	WaitKey();
//	{
//		//图形显示测试
//		SwitchGraphicsMode(TRUE);
//		SetMyMode(0x3);
//		WaitKey();
//		rectblock(0,0,799,599-1,&(gColorTable[1])); //white
//		rectangle(100,100,150,150,&(gColorTable[8]));//red
//		circle(300,300,40,&(gColorTable[3]));//blue
//		rectblock(450,450,500,500,&(gColorTable[6]));
//		WaitKey();
//		SetMyMode(0x0);
//		SwitchGraphicsMode(FALSE);
//	}
	
//	ListPCIMessage();
//	Status=GetSpecifyPciDevice(gPCIRootBridgeIO,&Pci,0x9999,0x8000);
//	if (EFI_ERROR (Status)) 
//	{
//		Print(L"Call GetSpecifyPciDevice, Can't find device!\n");
//	}
//	else
//	{
//		Print(L"Call GetSpecifyPciDevice, device!\n");
//		Print(L"BaseAddress0 = %x\n",Pci.Device.Bar[0]);
//		Print(L"BaseAddress2 = %x\n",Pci.Device.Bar[1]);
//		Print(L"BaseAddress2 = %x\n",Pci.Device.Bar[2]);
//		Print(L"BaseAddress3 = %x\n",Pci.Device.Bar[3]);
//		Print(L"BaseAddress4 = %x\n",Pci.Device.Bar[4]);
//		Print(L"BaseAddress5 = %x\n",Pci.Device.Bar[5]);
//		
//	}
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