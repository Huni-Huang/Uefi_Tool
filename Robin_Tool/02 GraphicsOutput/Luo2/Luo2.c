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

//Global variables
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInputEx;
EFI_GRAPHICS_OUTPUT_PROTOCOL       *GraphicsOutput;
//Functions declare
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
EFI_STATUS LocateGraphicsOutput (void);
EFI_STATUS LocateSimpleTextInputEx(void);

EFI_STATUS DisplayModeInformation(void);
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber);
EFI_STATUS DisplayTextModeInformation(void);
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber);
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
//	ISTATUS=&gST->ConsoleInHandle->ReadKeyStrokeEx(&gST->ConsoleInHandle,&IKeyData);
	//lbdebug begin
	DisplayTextModeInformation();
	Status=LocateSimpleTextInputEx();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocateSimpleTextInputEx, Can't find protocol!\n");
	}
	else
	{
		Print(L"Call LocateSimpleTextInputEx, Find protocol!\n");
	}
	Status=LocateGraphicsOutput();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocateGraphicsOutput, Can't find graphics protocol!\n");
	}
	else
	{
		Print(L"Call LocateGraphicsOutput, Find graphics protocol!\n");
	}
	{
		//DisplayTextMode...
		UINTN ModeIndex;
		for(ModeIndex=0;ModeIndex<(UINTN)(gST->ConOut->Mode->MaxMode);ModeIndex++)
		{
			DisplaySpecifyTextModeMessage(ModeIndex);
			GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		}
	}
	{
		//DisplayModeInformation();
		UINT32 ModeIndex;
		for(ModeIndex=0;ModeIndex<GraphicsOutput->Mode->MaxMode;ModeIndex++)
		{
			DisplaySpecifyModeMessage(ModeIndex);
			GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
		}
	}
	
	//lbdebug end
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
  EFI_KEY_DATA                      KeyData;
 	UINTN															Index;
	  
	gBS->WaitForEvent(1,&(SimpleTextInputEx->WaitForKeyEx),&Index);
	Status = SimpleTextInputEx->ReadKeyStrokeEx(SimpleTextInputEx,&KeyData);
	if(!EFI_ERROR(Status))
	{
 		*ScanCode=KeyData.Key.ScanCode;
		*UniChar=KeyData.Key.UnicodeChar;
		*ShiftState=KeyData.KeyState.KeyShiftState;
		*ToggleState=KeyData.KeyState.KeyToggleState;
		return EFI_SUCCESS;
	} 
	return Status;
}
//Name:LocateSimpleTextInputEx
//Input:
//Output: EFI_STATUS=EFI_SUCCESS, *SimpleTextInputEx
//				EFI_STATUS=
//Descriptor: Get the handle of SimpleTextInputEx
EFI_STATUS LocateSimpleTextInputEx(void)
{
	EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;

  
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
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleTextInputEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			return EFI_SUCCESS;
  	}
  }	 
	return Status;
}
//Name: LocateGraphicsOutput
//Input: 
//Output: *GraphicsOutput
//Descriptor:
EFI_STATUS LocateGraphicsOutput (void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *GraphicsOutputControllerHandles = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports GraphicsOutputProtocol
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&HandleCount,
		&GraphicsOutputControllerHandles
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			GraphicsOutputControllerHandles[HandleIndex],
			&gEfiGraphicsOutputProtocolGuid,
			(VOID**)&GraphicsOutput);
		if (EFI_ERROR(Status))	continue;

		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}

//================Debug function================
//Name: DisplayModeInformation
//Input:
//Output:
EFI_STATUS DisplayModeInformation(void)
{
	Print(L"Print Current Mode Message:\n");
	Print(L"::MaxMode=%02x\n",GraphicsOutput->Mode->MaxMode);
	Print(L"::Current Mode=%02x\n",GraphicsOutput->Mode->Mode);
	Print(L"::SizeOfInfo=%x\n",GraphicsOutput->Mode->SizeOfInfo);
	Print(L"::Info->Version=%x\n",GraphicsOutput->Mode->Info->Version);
	Print(L"::Info->HorizontalResolution=%x\n",GraphicsOutput->Mode->Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%x\n",GraphicsOutput->Mode->Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%x\n",GraphicsOutput->Mode->Info->PixelsPerScanLine);
	
	return EFI_SUCCESS;
}
//Name: DisplaySpecifyModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber)
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *Info;
	UINTN                                  SizeOfInfo;
	EFI_STATUS                         Status;
	
	Status=GraphicsOutput->QueryMode(GraphicsOutput,ModeNumber,&SizeOfInfo,&Info);
	Print(L"Print Mode %d Message:\n",ModeNumber);
	Print(L"::Info->Version=%d\n",Info->Version);
	Print(L"::Info->HorizontalResolution=%d\n",Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%d\n",Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%d\n",Info->PixelsPerScanLine);
	
	return Status;
}
//Name: DisplayTextModeInformation(void)
//Input:
//Output:
EFI_STATUS DisplayTextModeInformation(void)
{
	Print(L"Print SimPleTextMode Message:\n");
	Print(L"::MaxMode=%d\n",gST->ConOut->Mode->MaxMode);
	Print(L"::Current Mode=%d\n",gST->ConOut->Mode->Mode);
	Print(L"::Attribute=%d\n",gST->ConOut->Mode->Attribute);
	Print(L"::CursorColumn=%d\n",gST->ConOut->Mode->CursorColumn);
	Print(L"::CursorRow=%d\n",gST->ConOut->Mode->CursorRow);
	Print(L"::CursorVisible=%d\n",gST->ConOut->Mode->CursorVisible);
	
	return EFI_SUCCESS;	
}

//Name: DisplaySpecifyTextModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber)
{
	EFI_STATUS                         Status;
	UINTN Columns;
	UINTN Rows;
	
	Status=gST->ConOut->QueryMode(gST->ConOut,ModeNumber,&Columns,&Rows);
	Print(L"Print Text Mode %d Message:\n",ModeNumber);
	Print(L"::Column=%d\n",Columns);
	Print(L"::Row=%d\n",Rows);
	
	return Status;
}