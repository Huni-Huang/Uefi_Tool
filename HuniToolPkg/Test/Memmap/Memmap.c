#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>



EFI_STATUS
EFIAPI
DumpMem(
    IN EFI_HANDLE                      ImageHandle,
    IN EFI_SYSTEM_TABLE               *SystemTable
){
    EFI_STATUS                         Status;
    UINTN                              MemoryMapSize;
    EFI_MEMORY_DESCRIPTOR             *MemoryMap = NULL;
    EFI_MEMORY_DESCRIPTOR             *MemoryMapStart;
    EFI_MEMORY_DESCRIPTOR             *MemoryMapEnd;
    UINTN                              MapKey;
    UINTN                              DescriptorSize;
    UINT32                             DescriptorVersio;

    Status = gBS-> GetMemoryMap(
                     &MemoryMapSize,
                     MemoryMap,
                     &MapKey,
                     &DescriptorSize,
                     &DescriptorVersio);
    ASSERT(Status == EFI_BUFFER_TOO_SMALL);
    do
    {
        MemoryMap = AllocatePool(MemoryMapSize);
        if (MemoryMap == NULL)
        {
            Print(L"AllocatePool MemoryMap size is NULL ");
            ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
        }
        Status = gBS-> GetMemoryMap(
                         &MemoryMapSize,
                         MemoryMap,
                         &MapKey,
                         &DescriptorSize,
                         &DescriptorVersio);
        if (EFI_ERROR (Status))
        {
            FreePool(MemoryMap);
        }
    } while (Status == EFI_BUFFER_TOO_SMALL);
    Print(L"Memsize = %d   DescSize =  %d   MemMap = %d\n",MemoryMapSize, DescriptorSize, &MemoryMap);

    MemoryMapStart = MemoryMap;
    MemoryMapEnd = (EFI_MEMORY_DESCRIPTOR *)((UINT8*)MemoryMap + MemoryMapSize);
    Print(L" Type   PhysicalStart         VirtualStart:      NumberOfPages:   \n\r");
    while (MemoryMapStart < MemoryMapEnd)
    {
        if (MemoryMapStart->Type == EfiLoaderCode)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiReservedMemoryType)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart-> Type == EfiLoaderData)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiBootServicesCode)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiBootServicesData)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiRuntimeServicesCode)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiRuntimeServicesData)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiConventionalMemory)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiUnusableMemory)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiACPIReclaimMemory)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiACPIMemoryNVS)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiMemoryMappedIO)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiMemoryMappedIOPortSpace)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }else if (MemoryMapStart->Type == EfiPalCode)
        {
            Print(L" %3d    0x%x             0x%d                 %d\n",
            MemoryMapStart->Type, MemoryMapStart->PhysicalStart, MemoryMapStart->Attribute, MemoryMapStart->NumberOfPages);
        }
        MemoryMapStart = NEXT_MEMORY_DESCRIPTOR(MemoryMapStart, DescriptorSize);
        
    }

    return Status;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE                    ImageHandle,
    IN EFI_SYSTEM_TABLE             *SystemTable
){
    EFI_STATUS                      Status;
    EFI_PHYSICAL_ADDRESS            Address;
    CHAR16                          *Test;
    CHAR16                          *New;

    DumpMem(ImageHandle, SystemTable);

    Test = L"Keep on going and never give up";
    Address = SIZE_4GB - 1;

    Status = gBS->AllocatePages(AllocateMaxAddress, EfiLoaderCode, 1, &Address);
    if (!EFI_ERROR(Status))
    {
        Print(L"AlloctePage success!\n\r");
    }

    New = (CHAR16*)Address;

    gBS-> CopyMem(New, Test, sizeof(CHAR16) * 32);

    Print(L"Address: 0x%x\n",Address);
    Print(L"New: %s\n", New);

    Status = gBS-> FreePages(Address, 1);
    if (!EFI_ERROR(Status))
    {
        Print(L"Free Page Pass !\n\r");
    }
    
    return Status;
}