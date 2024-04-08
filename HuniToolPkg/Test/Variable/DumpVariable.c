#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
DumpVariable(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
){
    EFI_STATUS              Status;
    UINTN                   NameSize;
    UINTN                   VarNameSize;
    CHAR16                 *VarName;
    EFI_GUID                Guid;
    
    VarNameSize = 50 * sizeof(CHAR16);
    VarName = AllocateZeroPool(VarNameSize);
    if (VarName == NULL){
        FreePool(VarName);
        return EFI_OUT_OF_RESOURCES;
    }

    while (TRUE)
    {
        NameSize = VarNameSize;
        Status = gRT -> GetNextVariableName(&NameSize, VarName, &Guid);
        if (Status == EFI_BUFFER_TOO_SMALL){
            VarName = ReallocatePool(VarNameSize, NameSize, VarName);
            if (VarName == NULL)
            {
                return EFI_OUT_OF_RESOURCES;
            }
            VarNameSize = NameSize;
            Status = gRT-> GetNextVariableName(&NameSize, VarName, &Guid);
            }
        if (Status == EFI_NOT_FOUND){
            break;
        }
        Print(L"VarName: %s\nVarGuid: %g\n", VarName, Guid);
    }
    Print(L"Status : %r\n", Status);
    return Status;
}

EFI_STATUS
EFIAPI
AddVariable(
    IN EFI_HANDLE                  ImageHandle,
    IN EFI_SYSTEM_TABLE           *SystemTable
){
    EFI_STATUS                 Status;
    CHAR16                    *Varname;
    UINTN                      Data;
    Data = 100;
    Varname = L"HuniEnable";
    Status = gRT-> SetVariable (
                              Varname,
                              &gEfiHuniEnableGuid,
                              EFI_VARIABLE_NON_VOLATILE |
                              EFI_VARIABLE_BOOTSERVICE_ACCESS |
                              EFI_VARIABLE_RUNTIME_ACCESS,
                              sizeof(Data),
                              &Data);
    if (!EFI_ERROR(Status)){
        Print(L"SetVarName: %s\n Data: %d\n", Varname, Data);
    }
    Print(L"Status = %r\n", Status);
    return Status;
}

EFI_STATUS
EFIAPI
FindVariable(
    IN EFI_HANDLE                   ImageHandle,
    IN EFI_SYSTEM_TABLE            *SystemTable
){
    EFI_STATUS                      Status;
    CHAR16                         *VarName;
    UINTN                           Data;
    UINTN                           NewData;
    UINTN                           DataSize;
    DataSize = sizeof (Data);
    VarName = L"HuniEnable";
    Status = gRT -> GetVariable (VarName, 
                                 &gEfiHuniEnableGuid,
                                 NULL,
                                 &DataSize,
                                 &Data);
    Print(L"Varname = %s   Data =  %d   Status = %r\n",VarName, Data, Status);
    if (EFI_ERROR(Status)){
        return Status;
    }

    //Modify variable data
    Data = 200;
    Status = gRT-> SetVariable( VarName,
                                &gEfiHuniEnableGuid,
                                EFI_VARIABLE_NON_VOLATILE|
                                EFI_VARIABLE_BOOTSERVICE_ACCESS|
                                EFI_VARIABLE_RUNTIME_ACCESS,
                                sizeof(Data),
                                &Data);
    if (EFI_ERROR(Status)){
        return Status;
        }
    Status = gRT-> GetVariable( VarName,
                                &gEfiHuniEnableGuid,
                                NULL,
                                &DataSize,
                                &NewData
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }else{
        Print(L"Varname = %s   ModifyData =  %d   Status = %r\n",VarName, NewData, Status);
    }
    
    //delete Variable
    Status = gRT-> SetVariable( VarName,
                                &gEfiHuniEnableGuid,
                                0,
                                0,
                                NULL);
    if (EFI_ERROR(Status)){
        return Status;
    }

    Status = gRT-> GetVariable(
                               VarName,
                               &gEfiHuniEnableGuid,
                               NULL,
                               &DataSize,
                               &Data
    );
    if (Status == EFI_NOT_FOUND)
    {
        Print(L"Delete Variable success\n");
    }
    return Status;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE             ImageHandle,
    IN EFI_SYSTEM_TABLE      *SystemTable
){
    EFI_STATUS                Status;
    Status = EFI_SUCCESS;
    Print(L"start dump Variable\n");
    DumpVariable(ImageHandle, SystemTable);
    Print(L"start Add Variable\n");
    AddVariable(ImageHandle, SystemTable);
    Print(L"start modify and delete Variable\n");
    FindVariable(ImageHandle, SystemTable);

    return Status;

}