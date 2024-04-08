#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/FileInfo.h>


EFI_STATUS
EFIAPI
AddFile(
    IN EFI_HANDLE                 ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
){
    EFI_STATUS                          Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL    *FilePro;
    EFI_FILE_PROTOCOL                  *Root;
    EFI_FILE_PROTOCOL                  *FileRoot;

    CHAR16 *Buffer =(CHAR16*) L"Kobe is NO.1";
    UINTN   BufferSize = StrLen(Buffer) * 2;

    Status = gBS-> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, &FilePro);
    if(EFI_ERROR(Status)){
        return Status;
    }
    Status = FilePro->OpenVolume(FilePro, &Root);
    
    //Add Huni.txt file
    Status = Root-> Open(
                        Root,
                        &FileRoot,
                        (CHAR16*)L"Huni.txt",
                        EFI_FILE_MODE_CREATE| EFI_FILE_MODE_READ| EFI_FILE_MODE_WRITE,
                        0                    
    );

    // Write Huni.txt 
    Status = FileRoot->Write(FileRoot, &BufferSize, Buffer);
    
    FileRoot->Close(FileRoot);
    Root->Close(Root);

    return Status;
}

EFI_STATUS
EFIAPI
ReadFile(
    IN EFI_HANDLE                 ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
){
    EFI_STATUS                          Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL    *FilePro;
    EFI_FILE_PROTOCOL                  *Root;
    EFI_FILE_PROTOCOL                  *FileRoot;
    UINTN                              InfoSize = 0;
    EFI_FILE_INFO                      *Info =NULL;

    CHAR16                            *FileName;
    UINT64                             FileSize;
    CHAR16                             *Buffer ;
    UINT16                             Year;
    UINT16                             Month;
    UINT16                             Day;

    Status = gBS-> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, &FilePro);
    if(EFI_ERROR(Status)){
        return Status;
    }
    Status = FilePro->OpenVolume(FilePro, &Root);
    
    //open Huni.txt file
    Status = Root-> Open(
                        Root,
                        &FileRoot,
                        (CHAR16*)L"Huni.txt",
                        EFI_FILE_MODE_CREATE| EFI_FILE_MODE_READ| EFI_FILE_MODE_WRITE,
                        0                    
    );
    
    //get huni.txt info
    Status = FileRoot-> GetInfo(FileRoot, 
                               &gEfiFileInfoGuid, 
                               &InfoSize, 
                               Info);
    
    if (Status == EFI_BUFFER_TOO_SMALL){
        Info = AllocatePool(InfoSize);
        if (Info == NULL){
            return EFI_OUT_OF_RESOURCES;
        }
        Status = FileRoot-> GetInfo(FileRoot, 
                               &gEfiFileInfoGuid, 
                               &InfoSize, 
                               Info);
    }
    
    Year  = Info->CreateTime.Year;
    Month = Info->CreateTime.Month;
    Day   = Info->CreateTime.Day;
    FileSize = Info->FileSize;
    FileName = Info->FileName;
    Print(L"  Year = %d Month = %d Day = %d FileName = %s FileSize = %d \n\r",Year, Month, Day, FileName, FileSize);

    //read Huni.txt
    Buffer = AllocatePool(FileSize);
    Status = FileRoot->Read(FileRoot, &FileSize, Buffer);
    Print(L" File = %s \n\r", Buffer);
    FileRoot->Close(FileRoot);
    Root->Close(Root);

    return Status;
}

EFI_STATUS
EFIAPI
ModifyFile(
    IN EFI_HANDLE                 ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
){
    EFI_STATUS                          Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL    *FilePro;
    EFI_FILE_PROTOCOL                  *Root;
    EFI_FILE_PROTOCOL                  *FileRoot;


    
    CHAR16* Mfile = (CHAR16*) L"eat eat drink drink";
    UINTN                              BufferSize = StrLen(Mfile) *2;

    Status = gBS-> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, &FilePro);
    if(EFI_ERROR(Status)){
        return Status;
    }
    Status = FilePro->OpenVolume(FilePro, &Root);
    
    //open Huni.txt file
    Status = Root-> Open(
                        Root,
                        &FileRoot,
                        (CHAR16*)L"Huni.txt",
                        EFI_FILE_MODE_CREATE| EFI_FILE_MODE_READ| EFI_FILE_MODE_WRITE,
                        0                    
    );
    
    Status = FileRoot->Write(FileRoot, &BufferSize, Mfile);

    FileRoot->Close(FileRoot);
    Root->Close(Root);

    return Status;
}

EFI_STATUS
EFIAPI
ModifyFile22(
    IN EFI_HANDLE                 ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
){
    EFI_STATUS                          Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL    *FilePro;
    EFI_FILE_PROTOCOL                  *Root;
    EFI_FILE_PROTOCOL                  *FileRoot;
    EFI_FILE_INFO                      *Info = NULL;
    UINTN                               FileInfoSize;


    
    CHAR16*                            Mfile = L"Kobe";
    UINTN                              BufferSize = StrLen(Mfile) *2;

    Status = gBS-> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, &FilePro);
    if(EFI_ERROR(Status)){
        return Status;
    }
    Status = FilePro->OpenVolume(FilePro, &Root);
    
    //open Huni.txt file
    Status = Root-> Open(
                        Root,
                        &FileRoot,
                        (CHAR16*)L"Huni.txt",
                        EFI_FILE_MODE_CREATE| EFI_FILE_MODE_READ| EFI_FILE_MODE_WRITE,
                        0                    
    );

    Status = FileRoot->GetInfo(FileRoot, &gEfiFileInfoGuid, &FileInfoSize, Info);
    if (Status == EFI_BUFFER_TOO_SMALL)
    {
        Status = gBS->AllocatePool(EfiBootServicesData, FileInfoSize, &Info);
        if (Info == NULL)
        {
            return EFI_OUT_OF_RESOURCES;
        }
        Status = FileRoot->GetInfo(FileRoot, &gEfiFileInfoGuid, &FileInfoSize, Info);
    }

    Status = FileRoot->SetPosition(FileRoot, 2);
    
    Status = FileRoot->Write(FileRoot, &BufferSize, Mfile);

    FileRoot->Close(FileRoot);
    Root->Close(Root);

    return Status;
}

EFI_STATUS
EFIAPI
DeleteFile(
    IN EFI_HANDLE                ImageHandle,
    IN EFI_SYSTEM_TABLE         *SystemTable
){
    EFI_STATUS                         Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL    *FilePro;
    EFI_FILE_PROTOCOL                  *Root;
    EFI_FILE_PROTOCOL                  *FileRoot;

    Status = gBS-> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, &FilePro);
    if(EFI_ERROR(Status)){
        return Status;
    }
    Status = FilePro->OpenVolume(FilePro, &Root);
    
    //open Huni.txt file
    Status = Root-> Open(
                        Root,
                        &FileRoot,
                        (CHAR16*)L"Huni.txt",
                        EFI_FILE_MODE_CREATE| EFI_FILE_MODE_READ| EFI_FILE_MODE_WRITE,
                        0                    
    );

    Status = FileRoot -> Delete(FileRoot);
    if (!EFI_ERROR (Status)){
        Print(L"Detele Huni.txt success\n");
    }

    return Status;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE                 ImageHandle,
    IN EFI_SYSTEM_TABLE          *SystemTable
){

    AddFile(ImageHandle, SystemTable);
    ReadFile(ImageHandle, SystemTable);
    ModifyFile(ImageHandle, SystemTable);
    ReadFile(ImageHandle, SystemTable);
    ModifyFile22(ImageHandle, SystemTable); 
    ReadFile(ImageHandle, SystemTable);
    DeleteFile(ImageHandle, SystemTable);


    return TRUE;
}