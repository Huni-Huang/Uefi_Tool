#include <uefi.h> 
#include <Library/UefiLib.h> 
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Pi/PiHob.h>
#include <Library/DebugLib.h>
#include "CreateHob.h"

EFI_GUID    gEfiHuniHobGuid                = {0x27D61E35, 0xFF1A, 0x465B, {0x9e, 0xfa, 0x71, 0x42, 0xf1, 0xd2, 0x8e, 0x22}};

EFI_STATUS
EFIAPI
CreateHobEntry(
  IN       EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
)
{ 
  EFI_STATUS                              Status;
  HUNI_INFO_HOB                          *HuniHob = NULL;
  
  DEBUG ((DEBUG_ERROR, "Create Huni Hob Start..\n"));

  Status = (*PeiServices)->CreateHob (
                             PeiServices,
                             EFI_HOB_TYPE_GUID_EXTENSION,
                             sizeof (HUNI_INFO_HOB),
                             &HuniHob
                             );

  if (Status == EFI_SUCCESS)
  {
    CopyMem(&HuniHob->EfiHobGuidType.Name, &gEfiHuniHobGuid, sizeof(EFI_GUID));
    HuniHob->Age = 88;
    HuniHob->height = 789;
  }else{
    DEBUG ((DEBUG_ERROR, "Create Huni Hob Failed\n"));
    return Status;
  }
  DEBUG ((DEBUG_ERROR, "GUID: %g\n",&HuniHob->EfiHobGuidType.Name));
  DEBUG ((DEBUG_ERROR, "Age: %d\n",HuniHob->Age));
  DEBUG ((DEBUG_ERROR, "Height: %d\n",HuniHob->height));
  DEBUG ((DEBUG_ERROR, "Create Huni Hob Start End..\n"));
  return Status;
}