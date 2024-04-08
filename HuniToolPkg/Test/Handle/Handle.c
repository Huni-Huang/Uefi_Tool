#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/TimerLib.h>

#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h> 
#include <Library/HandleParsingLib.h>
#include <Protocol/UsbIo.h>



EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE                       ImageHandle,
    IN EFI_SYSTEM_TABLE                *SystemTable
){
    EFI_STATUS                         Status;

    UINTN                              AllHandleCount;
    EFI_HANDLE                         *AllHandleBuffer;
    CONST CHAR16                       *AllHandleName;

    UINTN                              HandleCount;
    EFI_HANDLE                         *HandleBuffer;
    CONST CHAR16                       *HandleName;

    EFI_GUID                           **GuidList;
    UINTN                              ProtocolCount;
    CHAR16                             *ProtocolName;

    Status = gBS-> LocateHandleBuffer( AllHandles,
                                       NULL,
                                       NULL, 
                                       &AllHandleCount, 
                                       &AllHandleBuffer );
    
    if (EFI_ERROR(Status)){
        Print(L"Status: %r\n", Status);
        return Status;
    }

    Print(L"All handle Name:\n");
    for (UINTN j = 0; j < AllHandleCount; j++)
    {
        AllHandleName = GetStringNameFromHandle(AllHandleBuffer[j], NULL);
        Print(L"%s\n",AllHandleName);
    }
    Print(L"Total Handle: %d\n", AllHandleCount);

    Print(L"\n\n");
    MicroSecondDelay(5000000);
    
    Status = gBS-> LocateHandleBuffer( ByProtocol, 
                                       //&gEfiUsbIoProtocolGuid,
                                       &gEfiLoadedImageProtocolGuid,
                                       NULL, 
                                       &HandleCount, 
                                       &HandleBuffer );
    
    if (EFI_ERROR(Status)){
        Print(L"Status: %r\n", Status);
        return Status;
    }

    Print(L"LI Protocol handle Name:\n");
    for (UINTN j = 0; j < HandleCount; j++)
    {
        HandleName = GetStringNameFromHandle(HandleBuffer[j], NULL);
        Print(L"%s\n",HandleName);
    }
    Print(L"LI Protocol Total Handle: %d\n", HandleCount);


    Print(L"\n\n");
    Status = gBS-> ProtocolsPerHandle( ImageHandle, 
                                       &GuidList, 
                                       &ProtocolCount);

    if (EFI_ERROR(Status))
    {
        Print(L"Status: %r\n", Status);
        return Status;
    }
    
    Print(L"Protocol count: %d\n", ProtocolCount);
    Print(L"ProtocolName             GUID\n");
    for (UINTN i = 0; i < ProtocolCount; i++){
        ProtocolName = GetStringNameFromGuid(GuidList[i], NULL);
        Print(L"%s          %g\n",ProtocolName, GuidList[i]);
    }
    
    FreePool(AllHandleBuffer);
    FreePool(HandleBuffer);
    FreePool(GuidList);
    return Status;
}