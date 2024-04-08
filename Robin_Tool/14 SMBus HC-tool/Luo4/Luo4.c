/** @file
  This sample application bases on HelloWorld PCD setting 
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#include "common.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Font.h"
#include "Smbus.h"

extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
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
};		//调试的时候使用
//EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
//  EFI_STATUS      Status;
//	UINT16 scanCode=0;
//	UINT16 uniChar=0;
//	UINT32 shiftState;
//	EFI_KEY_TOGGLE_STATE toggleState;
//	EFI_INPUT_KEY key={0,0};
//	UINT32 count=0;
	UINT64 flag;
	
	UINT8 rindex,userInput=0x0;
	
	flag=InintGloabalProtocols(S_TEXT_INPUT_EX|GRAPHICS_OUTPUT|PCI_ROOTBRIDGE_IO|PCI_IO|SMBUSHC);
	Print(L"flag=%x\n",flag);
	WaitKey();
	
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
//测试smbus
	//Display version 01~04

	Print(L"Sytool 1.0 -- uefi tool 2011/9/6 16:38:47 by luobing\n\n");
	Print(L"Version offset 01: 0x%02x\n",ReadSmbusByte(0xf0,0x01));
	Print(L"Version offset 02: 0x%02x\n",ReadSmbusByte(0xf0,0x02));
	Print(L"Version offset 03: 0x%02x\n",ReadSmbusByte(0xf0,0x03));
	Print(L"Version offset 04: 0x%02x\n\n",ReadSmbusByte(0xf0,0x04));
	
	
	//get register index,slave address=0xf0(sy device)
	Print(L"Please Input Register(00/10): ");
	rindex=iSimpleGetKeyIn();
	Print(L"  You input register index=0x %02x \n",rindex);
	Print(L"SlaveAddress=0x0f0,RegisterIndex=0x%02x \n",rindex);
	while(userInput!=0xff)
  {
		Print(L"Please Input cmd(0~fe,ff exit): ");
		
  	userInput=iSimpleGetKeyIn();
		Print(L"  cmd=0x%02x \n",userInput);
		
		WriteSmbusByte(0xf0,rindex,userInput);
		Print(L"   Return Value=0x %02x \n",ReadSmbusByte(0xf0,rindex));
  }
	{
		//DisplayTextMode...
		UINTN ModeIndex;
		for(ModeIndex=0;ModeIndex<(UINTN)(gST->ConOut->Mode->MaxMode);ModeIndex++)
		{
			DisplaySpecifyTextModeMessage(ModeIndex);
			WaitKey();
//			GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		}
	}
	{
		//DisplayModeInformation();
		UINT32 ModeIndex;
		for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		{
			DisplaySpecifyModeMessage(ModeIndex);
			WaitKey();
//			GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		}
	}
	{
		//图形显示测试
		SwitchGraphicsMode(TRUE);
		SetMyMode(0x3);
		WaitKey();
//		GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		rectblock(0,0,799,599-1,&(gColorTable[1])); //white
		rectangle(100,100,150,150,&(gColorTable[8]));//red
		circle(300,300,40,&(gColorTable[3]));//blue
		rectblock(450,450,500,500,&(gColorTable[6]));
		WaitKey();
//		GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);
		
	}
//	//测试键盘输入的例子，打印所有我需要的键盘信息
//	gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
//	gST->ConOut->OutputString(gST->ConOut,L"please input key(ESC to exit):\n\r");
//	while(key.ScanCode!=0x17)	//ESC
//	{
//		//Status=GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
//		Status=GetKey(&key);
//		if (EFI_ERROR (Status)) 
//		{
//			Print(L"Call GetKeyEx() Error!\n");
//			break;
//		}
//		else
//		{
//			Print(L"NO.%08d\n",count);
//			++count;
//			Print(L"  ScanCode=%04x",key.ScanCode);
//			Print(L"  UnicodeChar=%04x",key.UnicodeChar);
////			Print(L"  ShiftState=%08x",shiftState);
////			Print(L"  ToggleState=%02x",toggleState);
//			Print(L"\n");
//		}
//	}
 
  return EFI_SUCCESS;
}
