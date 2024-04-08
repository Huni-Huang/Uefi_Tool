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

typedef struct {
  EFI_HOB_GUID_TYPE    EfiHobGuidType;
  UINTN        Age;    
  UINTN        height;   
} HUNI_INFO_HOB;