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
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);

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
	EFI_STATUS      Status;
	UINT16 scanCode=0;
	UINT16 uniChar=0;
	UINT32 shiftState;
	EFI_KEY_TOGGLE_STATE toggleState;
	UINT32 count=0;
	
	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2013-2-1 11:57:51\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
	//text out test

	//测试键盘输入的例子，打印所有我需要的键盘信息
	gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"please input key:\n\r");
//	ISTATUS=&gST->ConsoleInHandle->ReadKeyStrokeEx(&gST->ConsoleInHandle,&IKeyData);
	while(scanCode!=0x17)	//ESC
	{
		Status=GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		if (EFI_ERROR (Status)) 
		{
			Print(L"Call GetKeyEx() Error!\n");
			break;
		}
		else
		{
			Print(L"NO.%08d\n",count);
			++count;
			Print(L"  ScanCode=%04x",scanCode);
			Print(L"  UnicodeChar=%04x",uniChar);
			Print(L"  ShiftState=%08x",shiftState);
			Print(L"  ToggleState=%02x",toggleState);
			Print(L"\n");
		}
	}
	return 0;
}

//Name:  GetKeyEx
//Input:
//Output:
//Descriptor:
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)
{
	EFI_STATUS                        Status;
	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  UINTN															Index;
  
  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			gBS->WaitForEvent(1,&(SimpleEx->WaitForKeyEx),&Index);
    	Status = SimpleEx->ReadKeyStrokeEx(SimpleEx,&KeyData);
    	if(!EFI_ERROR(Status))
    	{
    		*ScanCode=KeyData.Key.ScanCode;
  			*UniChar=KeyData.Key.UnicodeChar;
  			*ShiftState=KeyData.KeyState.KeyShiftState;
  			*ToggleState=KeyData.KeyState.KeyToggleState;
  			return EFI_SUCCESS;
  		}
    }
  }	 
	return Status;
}