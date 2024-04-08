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
//2013-05-15 1100

#include "common.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"
#include "HiiFont.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
//debug function
EFI_STATUS ListPCIMessage(void);

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
//	EFI_STATUS      Status;
//	PCI_TYPE00 Pci;
	UINT64 flag;
	

//  EFI_HII_HANDLE    * handles1 = 0;   //for Font of Hii
  
  flag=InintGloabalProtocols(S_TEXT_INPUT_EX|GRAPHICS_OUTPUT|PCI_ROOTBRIDGE_IO|PCI_IO|HII_FONT);
	Print(L"flag=%x\n",flag);
	WaitKey();
	
  {
  	//1 使用字符串资源的例子
  	LoadSimpleFont();
  	TestStrPackage();
		//2 test simpleFont 之前已经安装了simpleFont package，可以直接打印(Text模式下)
		Print((const CHAR16*)L" 您好UEFI SimpleFont,My name is 罗冰~Robin.\n");
		gST->ConOut->SetAttribute(gST->ConOut,EFI_BACKGROUND_LIGHTGRAY|EFI_YELLOW);  //UEFI SPEC 2.8 Page451
		gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
		Print((const CHAR16*)L" 测试彩色文字\n");
  }
	

	{
		//图形显示测试
		
		//DisplayModeInformation();
		UINT32 ModeIndex;
		const CHAR16* TestStr=(const CHAR16 *)L"君不见黄河之水天上来，奔流到海不复回。";
		const CHAR16* TestStr1=(const CHAR16 *)L"君不见高堂明镜悲白发，朝如青丝暮成雪。";
		
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
		
		putHiiFontStr(120,120,(CHAR16*)TestStr,&(gColorTable[1]),&(gColorTable[8]));
		putHiiFontStr(120,150,(CHAR16*)TestStr1,&(gColorTable[4]),&(gColorTable[5]));
		WaitKey();
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);
	}
	
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