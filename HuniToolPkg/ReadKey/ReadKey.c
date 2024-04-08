#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Protocol/SimpleTextInEx.h>

#define EFI_INPUT_ENTER 0X0D

#define EFI_BUFFER_SIZE 1024

EFI_STATUS
EFIAPI
UefiMain(
    EFI_HANDLE           ImageHandle,
    EFI_SYSTEM_TABLE    *SystemTable
){
    EFI_STATUS          Status;
    UINTN               Index;
    EFI_KEY_DATA        KeyDate;
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *Input;
    CHAR16              *Buffer;
    UINTN               BufferSize;
    UINT8               Strlen = 0;
    OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY *EntryBuffer = NULL;
    OUT UINTN                               EntryCount;
    
    

    BufferSize = EFI_BUFFER_SIZE;

    Buffer = AllocateZeroPool(BufferSize);
    if (Buffer == NULL)
    {
        Print(L"allocatepool fail\r\n");
        return 0;
    }

    gST -> ConOut ->OutputString(gST->ConOut,L"----Start Input Char----\r\n");

    //Status = gBS ->LocateProtocol(&gEfiSimpleTextInputExProtocolGuid, NULL,(VOID **) &Input);
    //Print(L"status = %r \n",Status);

    Status = gBS -> OpenProtocol(gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid, &Input, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if ((EFI_ERROR(Status)))
    {
        Print(L"status = %r \n",Status);
        return Status;
    }
    
    while (1)
    {
        Status = gBS-> WaitForEvent(1, &Input->WaitForKeyEx, &Index);
        if ((EFI_ERROR(Status)))
    {
        Print(L"status = %r \n",Status);
        return Status;
    }

        Status = Input->ReadKeyStrokeEx (Input, &KeyDate);
        if ((EFI_ERROR(Status)))
    {
        Print(L"status = %r \n",Status);
        return Status;
    }

        if (KeyDate.Key.UnicodeChar == EFI_INPUT_ENTER )
        {
            break;
        }
        Buffer[Strlen++] = KeyDate.Key.UnicodeChar;
    }
    Print(L"input string: %s\r\n", Buffer);

    Print(L"scancode = %04x\r\n",KeyDate.Key.ScanCode);

    Print(L"UnicodeChar = %04x\r\n",KeyDate.Key.UnicodeChar);

    Print(L"KeyShiftState = %08x\r\n",KeyDate.KeyState.KeyShiftState);

    Print(L"KeyToggleState = %02x\r\n",KeyDate.KeyState.KeyToggleState);

    Status = gBS ->OpenProtocolInformation (gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid, &EntryBuffer, &EntryCount);
    Print(L"Hanle = %p GUID = %g Entrybuffer = %g EntryCount = %d\n", gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid, &EntryBuffer, &EntryCount);
    Print(L"status = %r \n",Status);

    Status = gBS ->CloseProtocol (gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid,gImageHandle ,NULL);
    Print(L"status = %r \n",Status);
    if (Buffer != NULL)
    {
        FreePool(Buffer);
        Print(L"Free buffer successfully\r\n");
    }
    return Status;
}