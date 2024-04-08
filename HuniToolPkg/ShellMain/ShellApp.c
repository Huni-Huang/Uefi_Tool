#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

INTN ShellAppMain(IN UINTN Argc, IN CHAR16 **Argv){
    EFI_STATUS         Status;
    gST-> ConOut ->OutputString (gST->ConOut,L"Huni\n");
    return 0;
}
