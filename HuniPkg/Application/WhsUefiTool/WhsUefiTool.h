#ifndef _WHS_UEFI_TOOL_H_
#define _WHS_UEFI_TOOL_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ShellLib.h>

#include "ReadMsrTool/ReadMsrTool.h"
#include "CpuidTool/Cpuid.h"

extern        EFI_HII_HANDLE  gWhsDebugHiiHandle;

//RDMSR Test Tool
INTN
EFIAPI
MsrRead(
  CONST CHAR16             *Address
);

//WRMSR Test Tool
INTN
EFIAPI
MsrWrite(
  CONST CHAR16            *Address,
  CONST CHAR16            *Value
);

EFI_STATUS
EFIAPI
RunCpuid(
  IN EFI_HANDLE          gImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
);

SHELL_STATUS
EFIAPI
ShellCommandRunSmbiosView (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );
#endif