#include "ReadMsrTool.h"

EFI_MP_SERVICES_PROTOCOL          *mMpService;
EFI_PROCESSOR_INFORMATION         *mProcessorLocBuf;
UINTN                             mProcessorNum;
UINTN                             mBspIndex;

CHAR16
ToUpper (
  CHAR16  a
  )
{
  if ('a' <= a && a <= 'z') {
    return (CHAR16) (a - 0x20);
  } else {
    return a;
  }
}

VOID
EFIAPI
StrUpr (
  IN OUT CHAR16  *Str
  )
{
  while (*Str) {
    *Str = ToUpper (*Str);
    Str += 1;
  }
}

//
// Bits definition of Command flag list
//
#define OPCODE_RDMSR_BIT                      BIT0    // Read MSR register
#define OPCODE_WRMSR_BIT                      BIT1    // Write MSR register
#define OPCODE_CPUID_BIT                      BIT2    // Read MSR
#define OPCODE_ALLPROCESSOR_BIT               BIT3    // Support run routine on All processors
#define OPCODE_PROCESSOR_INDEX_BIT            BIT4    // Support run routine on a specific AP
#define OPCODE_SGDT_BIT                       BIT5    // Read GDTR
#define OPCODE_CR_BIT                         BIT6    // Read CR registers
#define OPCODE_RMM_BIT                        BIT7    // Read MMIO/IO
#define OPCODE_WMM_BIT                        BIT8    // Write MMIO/IO
#define OPCODE_DUMPMEM_BIT                    BIT9    // Dump memory region to a file
#define OPCODE_UCODE_BIT                      BIT10   // Get CPU Microcode signature/version

typedef struct {
  // MSR
  UINT32                    MsrIndex;
  UINT64                    MsrValue;

  // CPUID
  UINT32                    CpuIdIndex;
  UINT32                    CpuIdSubIndex;

  UINTN                     ProcessorIndex;

  // MEM / MMIO / IO
  UINTN                     MmAddress;
  UINTN                     MmValue;
  UINTN                     MmWidth;
  UINT8                     MmAccessType;  // 0: Mem / MMIO; 1: IO

  // Dump memory to file
  UINTN                     DumpMemAddress;
  UINTN                     DumpMemSize;
  CHAR16                    DumpMemFile[256];
} UEFI_TOOL_CONTEXT;

UEFI_TOOL_CONTEXT    gUtContext;

VOID
ApUtReadMsr (
  OUT UINT64             *MsrData
  )
{
  *MsrData = AsmReadMsr64 (gUtContext.MsrIndex);
}

EFI_STATUS
GetProcessorsCpuLocation (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             EnabledProcessorNum;
  UINTN                             Index;

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **)&mMpService
                  );
  if (EFI_ERROR (Status)) {
    //
    // MP protocol is not installed
    //
    return EFI_UNSUPPORTED;
  }

  Status = mMpService->GetNumberOfProcessors (
                         mMpService,
                         &mProcessorNum,
                         &EnabledProcessorNum
                         );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof(EFI_PROCESSOR_INFORMATION) * mProcessorNum,
                  (VOID **) &mProcessorLocBuf
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < mProcessorNum; Index++) {
    Status = mMpService->GetProcessorInfo (
                           mMpService,
                           Index,
                           &mProcessorLocBuf[Index]
                           );
    if (EFI_ERROR (Status)) {
      FreePool(mProcessorLocBuf);
      return Status;
    }
  }

  Status = mMpService->WhoAmI (
                         mMpService,
                         &mBspIndex
                         );
  return Status;
}

EFI_STATUS
EFIAPI
UefiToolRoutine (
  UINT64             Opcode
  )
{
  UINT64             MsrData;
  UINTN              Index;

  //
  // Read a MSR register
  //
  if (Opcode == OPCODE_RDMSR_BIT) {
    MsrData = AsmReadMsr64 (gUtContext.MsrIndex);
    Print (L"RDMSR[0x%X]: 0x%016lX\n", gUtContext.MsrIndex, MsrData);
  }

  //
  // Write a MSR register
  //
  if (Opcode == OPCODE_WRMSR_BIT) {
    MsrData = AsmWriteMsr64 (gUtContext.MsrIndex, gUtContext.MsrValue);
    Print (L"WR Data 0x%016lX to MSR[0x%X]\n", gUtContext.MsrValue, gUtContext.MsrIndex);
  }

  //
  // Read a MSR register for a specific AP
  //
  if (Opcode == (OPCODE_RDMSR_BIT | OPCODE_PROCESSOR_INDEX_BIT)) {
    Index = gUtContext.ProcessorIndex;
    MsrData = 0;
    if (Index == mBspIndex) {
        MsrData = AsmReadMsr64 (gUtContext.MsrIndex);
    } else {
      mMpService->StartupThisAP (
                    mMpService,
                    (EFI_AP_PROCEDURE) ApUtReadMsr,
                    Index,
                    NULL,
                    0,
                    (UINT64 *) &MsrData,
                    NULL
                    );
    }

    Print (L"RDMSR[0x%X][ProcNum: %d S%d_C%d_T%d]: [64b] 0x%016lX\n", gUtContext.MsrIndex, Index,
      mProcessorLocBuf[Index].Location.Package,
      mProcessorLocBuf[Index].Location.Core,
      mProcessorLocBuf[Index].Location.Thread,
      MsrData);
  }

  //
  // Read a MSR register for BSP and all APs
  //
  if (Opcode == (OPCODE_RDMSR_BIT | OPCODE_ALLPROCESSOR_BIT)) {
    for (Index = 0; Index < mProcessorNum; Index++) {
      MsrData = 0;
      if (Index == mBspIndex) {
        MsrData = AsmReadMsr64 (gUtContext.MsrIndex);
      } else {
        mMpService->StartupThisAP (
                      mMpService,
                      (EFI_AP_PROCEDURE) ApUtReadMsr,
                      Index,
                      NULL,
                      0,
                      (UINT64 *) &MsrData,
                      NULL
                      );
      }

      Print (L"RDMSR[0x%X][ProcNum: %d S%d_C%d_T%d]: [64b] 0x%16lX\n", gUtContext.MsrIndex, Index,
        mProcessorLocBuf[Index].Location.Package,
        mProcessorLocBuf[Index].Location.Core,
        mProcessorLocBuf[Index].Location.Thread,
        MsrData);
    }
  }

  return EFI_SUCCESS;
}


// MSR Read Functon
INTN
EFIAPI
MsrRead(
  CONST CHAR16             *Address
  )
{
  EFI_STATUS                Status;
  UINT64                    Opcode;
  
  Opcode = 0x0;
  SetMem (&gUtContext, sizeof (UEFI_TOOL_CONTEXT), 0x0);

  Status = GetProcessorsCpuLocation ();
  gUtContext.MsrIndex  = (UINT32) StrHexToUintn (Address);
  Opcode = (OPCODE_RDMSR_BIT | OPCODE_ALLPROCESSOR_BIT);

  UefiToolRoutine (Opcode);
  return EFI_SUCCESS;
}

//MSR Write Function
INTN
EFIAPI
MsrWrite(
  CONST CHAR16            *Address,
  CONST CHAR16            *Value
){
  EFI_STATUS                Status;
  UINT64                    Opcode;
  
  Opcode = 0x0;
  SetMem (&gUtContext, sizeof (UEFI_TOOL_CONTEXT), 0x0);
  Status = GetProcessorsCpuLocation ();

  gUtContext.MsrIndex  = (UINT32) StrHexToUintn (Address);
  gUtContext.MsrValue  = StrHexToUintn (Value);
  Opcode |= OPCODE_WRMSR_BIT;

  UefiToolRoutine (Opcode);
  return EFI_SUCCESS;
}