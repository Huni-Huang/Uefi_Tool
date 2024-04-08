#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#define INIT_NAME_BUFFER_SIZE 128
#define INIT_DATA_BUFFER_SIZE 1024
EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
){
    EFI_STATUS              Status;
    UINT32                  Att;
    UINTN                   NameSize;
    UINTN                   OldNameBufferSize;
    UINTN                   NameBufferSize;
    UINTN                   DateSize;
    UINTN                   DateBufferSize;
    UINT8                  *DateBuffer;
    CHAR16                 *VarName;
    CHAR16                 *OldName;
    EFI_GUID                VarGuid;

    NameBufferSize = INIT_NAME_BUFFER_SIZE;
    DateBufferSize = INIT_DATA_BUFFER_SIZE;

    VarName = AllocateZeroPool(NameBufferSize);
    if (VarName == NULL)
    {
        return EFI_OUT_OF_RESOURCES;
    }
    DateBuffer = AllocatePool(DateBufferSize);
    if (DateBuffer == NULL)
    {
        FreePool(VarName);
        return EFI_OUT_OF_RESOURCES;
    }

    for ( ;;)
    {
        Status = gRT -> GetNextVariableName(&NameSize, VarName, &VarGuid);
        if (Status == EFI_BUFFER_TOO_SMALL)
        {
            OldName = VarName;
            OldNameBufferSize = NameBufferSize;
            NameBufferSize = NameSize > NameBufferSize *2 ? NameSize:NameBufferSize *2;
            VarName = AllocateZeroPool(NameBufferSize);
            if (VarName == NULL)
            {
                Status = EFI_OUT_OF_RESOURCES;
                FreePool(OldName);
                break;
            }
            CopyMem(VarName, OldName, OldNameBufferSize);
            FreePool(OldName);
            NameSize = NameBufferSize;
            Status = gRT-> GetNextVariableName(&NameSize, VarName, &VarGuid);
        }
        if (Status == EFI_NOT_FOUND)
        {
            break;
        }
        Print(L"VarName: %s\nVarGuid: %g\n",VarName,VarGuid);
        DateSize = DateBufferSize;
        Status = gRT -> GetVariable(VarName, &VarGuid, &Att, &DateSize, DateBuffer);
        
        /*if (Status == EFI_BUFFER_TOO_SMALL)
        {
            Print(L"Buffer too small!!!\n");
        }*/
    }
    FreePool(VarName);
    FreePool(DateBuffer);
    return Status;
}