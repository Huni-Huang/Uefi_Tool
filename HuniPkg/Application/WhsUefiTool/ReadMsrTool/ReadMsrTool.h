#ifndef _READ_MSR_TOOL_H_
#define _READ_MSR_TOOL_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/ShellLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Register/Intel/Cpuid.h>
#include <Register/Intel/Msr.h>
#include <Protocol/MpService.h>


VOID
EFIAPI
ShowHelpInfo(
  VOID
);

VOID
EFIAPI
StrUpr (
  IN OUT CHAR16  *Str
);

INTN
EFIAPI
MsrRead(
  CONST CHAR16             *Address
);

INTN
EFIAPI
MsrWrite(
  CONST CHAR16            *Address,
  CONST CHAR16            *Value
);
#endif