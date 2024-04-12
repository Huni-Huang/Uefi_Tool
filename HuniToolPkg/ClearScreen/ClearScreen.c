#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS ClearConsole(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut) {
    // 清除整个屏幕并将光标移动到屏幕的左上角
    ConOut->ClearScreen(ConOut);
    return EFI_SUCCESS;
}


EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_STATUS Status;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
    UINTN         i = 0;

    Status = gBS->HandleProtocol(gST->ConsoleOutHandle, &gEfiSimpleTextOutProtocolGuid, (VOID*)&ConOut);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // 清除屏幕
    ClearConsole(ConOut);
    while (1) {

        // 打印
        Print(L"Huni: i = %d \r\n", i);

        // 延时5秒
        gBS->Stall(5000000);

        // 清除屏幕
        ClearConsole(ConOut);
        i++;
        if (i == 5)
        {
           break;
        }
        
    }

    return EFI_SUCCESS;
}