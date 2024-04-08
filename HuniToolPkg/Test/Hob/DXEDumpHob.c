#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <Base.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "CreateHob.h"


EFI_GUID    gEfiHuniHobGuid                = {0x27D61E35, 0xFF1A, 0x465B, {0x9e, 0xfa, 0x71, 0x42, 0xf1, 0xd2, 0x8e, 0x22}};


EFI_STATUS
EFIAPI
UefiMain (
    IN EFI_HANDLE              ImageHandle,
    IN EFI_SYSTEM_TABLE       *SystemTable
)
{
   EFI_STATUS                       Status;
   HUNI_INFO_HOB                    *HuniHob;               

   Status = EfiGetSystemConfigurationTable(&gEfiHobListGuid, &HuniHob);
   if (EFI_ERROR(Status)){
    return Status;
   }

   HuniHob = GetNextGuidHob(&gEfiHuniHobGuid, HuniHob);
   if (HuniHob == NULL){
    return EFI_NOT_FOUND;
   }else{
      Print (L"GUID: %g\n",&HuniHob->EfiHobGuidType.Name);
      Print (L"HobType: %d\n",HuniHob->EfiHobGuidType.Header.HobType);
      Print (L"HobLength: %d\n",HuniHob->EfiHobGuidType.Header.HobLength);
      Print (L"Reserved: %d\n",HuniHob->EfiHobGuidType.Header.Reserved);
      Print (L"Age: %d\n",HuniHob->Age);
      Print (L"Height: %d\n",HuniHob->height);
   }
   return Status;
}