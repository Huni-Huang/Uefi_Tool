#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>


#define  PCI_Bus  0x100
#define  PCI_DEV  0x20
#define  PCI_Fun  0x8

EFI_STATUS
EFIAPI
DumpPCI(
    IN EFI_HANDLE                   ImageHandle,
    IN EFI_SYSTEM_TABLE            *SystemTable
){
    UINT32                          Bus;
    UINT32                          Dev;
    UINT32                          Fun;

    for ( Bus = 0; Bus < PCI_Bus; Bus++)
    {
        for ( Dev = 0; Dev < PCI_DEV; Dev++)
        {
            for ( Fun = 0; Fun < PCI_Fun; Fun++)
            {
                
            }
            
        }
        
    }
    
}