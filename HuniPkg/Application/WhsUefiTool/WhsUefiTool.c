#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Library/ShellCommandLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HiiLib.h>
#include "WhsUefiTool.h"
#include "IOMMUVal.h"



EFI_HII_HANDLE  gWhsDebugHiiHandle = NULL;

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
    {L"-RDMSR",TypeFlag},
    {L"-MSR",TypeFlag},
    {L"-WRMSR",TypeFlag},
    {L"-CPUID",TypeFlag},
    {L"-DMA",TypeFlag},
    {L"-SMBIOS",TypeFlag},
    {L"-h",TypeFlag},
    {L"-a",TypeFlag},
    {L"-c",TypeFlag},
    {L"-m",TypeFlag},
    {L"-l",TypeFlag},
    {NULL,TypeMax}
};

typedef enum WHS_DEBUG_Option{
    WHSHELP = 1,
    RDMSR,
    WRMSR,
    MSRHELP,
    CPUID,
    DMACPOY,
    DMAMAP,
    DMAHELP,
    SMBIOSHELP,
    SMBIOSALL,
    SMBIOSLIST,
} WHS_DEBUG_Option;

/*
* WHS UEFI Test tool for WHS Validation Team
* ----------------------------
* v1.0 @11/02/2022 by Guangyuan Huang
* ----------------------------
* Email: guangyuanx.huang@intel.com
*/

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_STATUS               Status;
    LIST_ENTRY               *Package;
    CHAR16                   *ProblemParam = NULL;
    CONST CHAR16             *Address = NULL;
    CONST CHAR16             *Value = NULL;
    
    WHS_DEBUG_Option Option;  

    // install the HII stuff.
    gWhsDebugHiiHandle = HiiAddPackages (&gWhsDebugHiiGuid, gImageHandle, WhsUefiToolStrings, NULL);
    if (gWhsDebugHiiHandle == NULL){
        return EFI_DEVICE_ERROR;
    }

    Status = ShellInitialize();
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_INFO, "ShellInitialize Fail!!!,Status :%r.\n", Status));
    }
    Status = CommandInit();
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_INFO, "CommandInit Fail!!!,Status :%r.\n", Status));
    }

    Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN(STR_WHS_TOOL_INFOR), gWhsDebugHiiHandle);

    Status = ShellCommandLineParse(ParamList, &Package, &ProblemParam, TRUE);

    if (EFI_ERROR (Status) && ParamList == NULL)
    {
        Print(L"Error: Error processing input parameter(s), ParamList: NULL\n");
    }else if (ShellCommandLineGetCount (Package) > 10){
            Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN(STR_TOO_MANY_PARAMETERS), gWhsDebugHiiHandle);
        }else{
            if (ShellCommandLineGetFlag(Package, L"-h")){
            Option = WHSHELP;
        }
            if ((ShellCommandLineGetFlag (Package,L"-RDMSR")) && (ShellCommandLineGetFlag (Package,L"-a"))){
            ShellPrintEx(-1,-1,L"\n-------------Read MSR register-----------\n");
            Option = RDMSR;
        }   if ((ShellCommandLineGetFlag (Package,L"-MSR")) && (ShellCommandLineGetFlag (Package,L"-h"))){
            ShellPrintEx(-1,-1,L"\n---------Get MSR Help information--------\n");
            Option = MSRHELP;
        }
            if (ShellCommandLineGetFlag(Package, L"-WRMSR")){
            ShellPrintEx(-1,-1,L"\n----------Write MSR register-------------\n");
            Option = WRMSR;
        }
            if (ShellCommandLineGetFlag(Package, L"-CPUID")){
            ShellPrintEx(-1,-1,L"\n----------Get CPUID information----------\n");
            Option = CPUID;
        }   if ((ShellCommandLineGetFlag (Package, L"-DMA") ) && (ShellCommandLineGetFlag (Package,L"-c"))){
            ShellPrintEx(-1,-1,L"\n----------Start DMA COPY Test------------\n");
            Option = DMACPOY;
        }   if ((ShellCommandLineGetFlag (Package, L"-DMA") ) && (ShellCommandLineGetFlag (Package,L"-m"))){
            ShellPrintEx(-1,-1,L"\n----------Start DMA MAP Test-------------\n");
            Option = DMAMAP;
        }   if ((ShellCommandLineGetFlag (Package, L"-DMA") ) && (ShellCommandLineGetFlag (Package,L"-h"))){
            ShellPrintEx(-1,-1,L"\n-----Get IOMMU Tool Help information-----\n");
            Option = DMAHELP;
        }   if ((ShellCommandLineGetFlag (Package, L"-SMBIOS") ) && (ShellCommandLineGetFlag (Package,L"-h"))){
            ShellPrintEx(-1,-1,L"\n-----Get SMBIOS Tool Help information-----\n");
            Option = SMBIOSHELP;
        }   if ((ShellCommandLineGetFlag (Package, L"-SMBIOS") ) && (ShellCommandLineGetFlag (Package,L"-a"))){
            ShellPrintEx(-1,-1,L"\n-----Get All SMBIOS Table information-----\n");
            Option = SMBIOSALL;
        }   if ((ShellCommandLineGetFlag (Package, L"-SMBIOS") ) && (ShellCommandLineGetFlag (Package,L"-l"))){
            ShellPrintEx(-1,-1,L"\n----------Get SMBIOS Table list-----------\n");
            Option = SMBIOSLIST;
        }
            
    }

    switch (Option){
        case RDMSR:
          Address = ShellCommandLineGetRawValue(Package,1);
          if (Address != NULL) {
            MsrRead(Address);
          }else {
            Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN(STR_INPUT_ADDRESS_ERROR), gWhsDebugHiiHandle);
          }
          break;
        case WRMSR:
          Address = ShellCommandLineGetRawValue(Package,1);
          Value = ShellCommandLineGetRawValue(Package,2);
          if (Address != NULL && Value != NULL) {
            MsrWrite(Address, Value);
          }else{
            Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_INPUT_VALUE_ERROR), gWhsDebugHiiHandle);
          }
          break;
        case MSRHELP:
          Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_MSR_TOOL_HELP), gWhsDebugHiiHandle, L"MSRToolHelpInfo");
          break;
        case CPUID:
          RunCpuid(ImageHandle, SystemTable);
          break;
        case WHSHELP:
          Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_WHS_TOOL_HELP), gWhsDebugHiiHandle, L"WhsToolHelpInfo");
          break;
        case DMACPOY:
          ShellCommandRunIommuVal(gImageHandle, SystemTable);
          break;
        case DMAMAP:
          ShellCommandRunIommuVal(gImageHandle, SystemTable);
          break;
        case DMAHELP:
          ShellCommandRunIommuVal(gImageHandle, SystemTable);
          break;
        case SMBIOSHELP:
          ShellCommandRunSmbiosView(gImageHandle, SystemTable);
          break;
        case SMBIOSALL:
          ShellCommandRunSmbiosView(gImageHandle, SystemTable);
          break;
        case SMBIOSLIST:
          ShellCommandRunSmbiosView(gImageHandle, SystemTable);
          break;
        default:
          Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_INVALID_PARAMETERS), gWhsDebugHiiHandle); 
          Status = ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_WHS_TOOL_HELP), gWhsDebugHiiHandle, L"WhsToolHelpInfo");
          return EFI_INVALID_PARAMETER;
    }
    ShellCommandLineFreeVarList (Package);
    return EFI_SUCCESS;
}