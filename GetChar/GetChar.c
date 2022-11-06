#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#define UNICODE_CHAR_ENTER 0x0D

EFI_STATUS
EFIAPI
UefiMain(
	IN EFI_HANDLE             ImageHandle,
	IN EFI_SYSTEM_TABLE      *SystemTable
){
	EFI_STATUS       Status;
	UINTN            Index;
	EFI_KEY_DATA     Key;
	UINTN            Strlen = 0;
	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL   *InputEx;
	CHAR16           StrBuffer[100];
	Status = gBS->LocateProtocol ( &gEfiSimpleTextInputExProtocolGuid,
	                               NULL, 
								   (VOID **) &InputEx);
	while (1)
	{
		//wait keyboard input event
		Status = gBS -> WaitForEvent(1, &InputEx->WaitForKeyEx, &Index);
        //read keyboard input
		Status = InputEx->ReadKeyStrokeEx(InputEx,&Key);
		//End loop when read a 'enter'
		if (Key.Key.UnicodeChar == UNICODE_CHAR_ENTER)
		{
			break;
		}
	    //save key to strbuffer
	    StrBuffer[Strlen] = Key.Key.UnicodeChar;
		Strlen++;
		Print(L"%c\n", Key.Key.UnicodeChar);
	}
	//output key 
	Print(L"input string: %s\n",StrBuffer);

	return EFI_SUCCESS;
}