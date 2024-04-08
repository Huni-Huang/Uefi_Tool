#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>


VOID
EFIAPI
PrintTxt(
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
){
    Print (L"Keep on going and never give up...\n\r");
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE                     ImageHandle,
    IN EFI_SYSTEM_TABLE              *SystemTable
){
    EFI_STATUS                        Status;

    EFI_EVENT                         HuniPrint;
    UINT64                            TriggerTime = 10 * 1000 * 1000;
    UINTN                             Index = 0;

    EFI_EVENT                         WaitEvt = NULL;
    UINTN                             Count = 0;


    Status = gBS->CreateEvent (
                  EVT_TIMER,
                  TPL_CALLBACK,
                  NULL,
                  NULL,
                  &WaitEvt
                  );
    if (!EFI_ERROR (Status)) {
        Status = gBS->SetTimer (
                      WaitEvt,
                      TimerRelative,
                      EFI_TIMER_PERIOD_SECONDS (5));
                      }
    if (EFI_ERROR (Status)) {
        return Status;
        }

    while (EFI_ERROR (gBS->CheckEvent (WaitEvt))) {
        Print (L"Waiting %d sencond ...\n", ++Count);
        gBS->Stall (1000 * 1000 * 1);
        }

    gBS->SetTimer (WaitEvt, TimerCancel, 0);
    gBS->CloseEvent (WaitEvt);


    Status = gBS-> CreateEvent(
                   EVT_NOTIFY_SIGNAL | EVT_TIMER, 
                   TPL_CALLBACK, 
                   PrintTxt,
                   NULL,
                   &HuniPrint);

    if (EFI_ERROR(Status))
    {
        Print (L"Create Event Error! \n\r");
        return Status;
    }

    Status = gBS-> SetTimer(HuniPrint, TimerPeriodic, TriggerTime);

    if (EFI_ERROR (Status)) {
        Print(L"Set Timer Error! \r\n");
        return Status;
    }

    while (1)
    {
        Status = gBS-> WaitForEvent(1, &HuniPrint, &Index);
    }
    
    gBS->SetTimer(HuniPrint, TimerCancel, 0);
    gBS-> CloseEvent(HuniPrint);

    return Status;
}