#include <Uefi.h>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/SimpleTextInEx.h>
#include <Library/ShellCEntryLib.h>


VOID
EFIAPI
PrintTime(
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
){
    EFI_TIME        Time;

    // UINTN Count = *(UINTN *)Context;

    gRT->GetTime(&Time, NULL);
    gST->ConOut->ClearScreen (gST->ConOut);
    Print(L"%04d-%02d-%02d %02d:%02d:%02d",
          Time.Year, Time.Month, Time.Day,
          Time.Hour, Time.Minute, Time.Second);
    // Count++;
    // *(UINTN *)Context = Count;

    // if (Count > 100)
    // {
    //     gBS->CloseEvent(Event);
    // }
}

VOID
EFIAPI
PrintTest1(
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
){

    Print(L"Huni: You can you up, No can no BB\n\r");

}

VOID
EFIAPI
PrintTest2(
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
){
    UINTN Count = *(UINTN *)Context;

    Print(L"Hi Huni, some people like to say Love is fleeting.\n\r");
    Count++;
    *(UINTN *)Context = Count;

    if (Count > 20)
    {
        gBS->CloseEvent(Event);
    }
}

VOID
PrintTimePeriodicEvent(void){
    EFI_STATUS                        Status;
    EFI_EVENT                         HuniPrint;
    UINT64                            TriggerTime = 10 * 1000 * 1000; //1s
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *Input;
    EFI_KEY_DATA                       Key;  
    //UINTN       Count = 0;

    Status = gBS->LocateProtocol(&gEfiSimpleTextInputExProtocolGuid, NULL, (VOID**)&Input);

    Status = Input->Reset(Input, TRUE);

    Status = gBS-> CreateEvent(
                   EVT_TIMER | EVT_NOTIFY_SIGNAL, 
                   TPL_CALLBACK, 
                   PrintTime,
                   NULL,
                   &HuniPrint);

    if (EFI_ERROR(Status))
    {
        Print (L"Create Event Error! \n\r");
        return ;
    }
    Status = gBS->SetTimer(HuniPrint, TimerPeriodic, TriggerTime);

    if (EFI_ERROR(Status))
    {
        Print(L"Set Timer Error! \r\n");
        return ;
    }

    while (TRUE)
    {
        Status = Input->ReadKeyStrokeEx(Input, &Key);
        if (!EFI_ERROR (Status) && Key.Key.ScanCode == SCAN_ESC)
        {
            gBS->SetTimer(HuniPrint, TimerCancel, 0);
            gBS->CloseEvent(HuniPrint);
            break;
        }
            gBS->WaitForEvent(1, &HuniPrint, NULL);
    }
    return;
}

VOID
PrintTimeRelativeEvent(void){
    EFI_STATUS                        Status;
    EFI_EVENT                         RelativeEvent;
    EFI_EVENT                         PeriodicEvent;
    UINT64                            TriggerTime1 = 10 * 1000 * 1000; //1s
    UINT64                            TriggerTime = 100 * 1000 * 1000; //10s
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *Input;
    EFI_KEY_DATA                       Key;  
    UINTN       Count = 0;

    Status = gBS->LocateProtocol(&gEfiSimpleTextInputExProtocolGuid, NULL, (VOID**)&Input);

    Status = Input->Reset(Input, TRUE);

    Status = gBS-> CreateEvent(
                   EVT_TIMER | EVT_NOTIFY_SIGNAL, 
                   TPL_CALLBACK, 
                   PrintTest1,
                   NULL,
                   &RelativeEvent);

    if (EFI_ERROR(Status))
    {
        Print (L"Create RelativeEvent Error! \n\r");
        return ;
    }
    Status = gBS->SetTimer(RelativeEvent, TimerRelative, TriggerTime);

    if (EFI_ERROR(Status))
    {
        Print(L"Set Timer Error! \r\n");
        return;
    }

    Status = gBS->CreateEvent(
        EVT_TIMER | EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        PrintTest2,
        &Count,
        &PeriodicEvent);

    if (EFI_ERROR(Status))
    {
        Print (L"Create RelativeEvent Error! \n\r");
        return ;
    }
    Status = gBS->SetTimer(PeriodicEvent, TimerPeriodic, TriggerTime1);

    if (EFI_ERROR(Status))
    {
        Print(L"Set Timer Error! \r\n");
        return ;
    }

    while (TRUE)
    {
        Status = Input->ReadKeyStrokeEx(Input, &Key);
        if (!EFI_ERROR (Status) && Key.Key.ScanCode == SCAN_ESC)
        {
            gBS->SetTimer(PeriodicEvent, TimerCancel, 0);
            gBS->CloseEvent(PeriodicEvent);
            gBS->SetTimer(RelativeEvent, TimerCancel, 0);
            gBS->CloseEvent(RelativeEvent);
            break;
        }
            gBS->WaitForEvent(1, &PeriodicEvent, NULL);
    }
    return;
}

INTN
EFIAPI
ShellAppMain(
  IN UINTN   Argc,
  IN CHAR16  **Argv
)
{
    //EFI_STATUS                        Status;
	if ((Argc!=2) && (Argc!=1) )
	{
		Print(L"Syntax: Event_Timer.efi [Direct]\n");
		Print(L"Sample: Event_Timer.efi 2\n");
		return EFI_SUCCESS;
	}

    if (Argc == 1)
    {
        Print(L"This is Fake love!!!\n");
        return EFI_SUCCESS;
    }
    if (Argc == 2)
    {
        switch (Argv[1][0])
        {
        case '1':
            PrintTimeRelativeEvent();
            break;
        case '2':
            PrintTimePeriodicEvent();
            break;
        default:
            Print(L"Yes, I'm fine\n");
            break;
        }
    }
    return EFI_SUCCESS;
}