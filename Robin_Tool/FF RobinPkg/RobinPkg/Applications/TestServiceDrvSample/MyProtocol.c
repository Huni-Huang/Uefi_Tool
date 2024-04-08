

#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>

#include "MyProtocol.h"

EFI_MYSAMPLE_PROTOCOL RobinSampleProtocol;
//--------------------------------------------------  GUID data  ---------------------------
#define EFI_MYSAMPLE_PROTOCOL_GUID \
{ \
    0xce345181, 0xabad, 0x11e2, {0x8e, 0x5f, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
}

EFI_GUID gEfiMYSampleProtocolGUID = EFI_MYSAMPLE_PROTOCOL_GUID ;

//------------------------------------------  Defintion of protocols -------------------------
EFI_STATUS
EFIAPI
MySample_In(
        IN EFI_MYSAMPLE_PROTOCOL* This,
        IN CHAR16* UserString
        )
{
    Print(L"Enter the MySample Protocol.\n");  
    Print(L"User's string:%s\n",UserString);
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MySample_Out(
        IN EFI_MYSAMPLE_PROTOCOL* This 
        )
{
    Print(L"Exit the MySample Protocol.\n");
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MySample_DoSth(
        IN EFI_MYSAMPLE_PROTOCOL* This,
        IN CHAR16* UserString
        )
{
    Print(L"User's string:%s",UserString);
    Print(L"Do something... MySample Protocol.\n");
    return EFI_SUCCESS;
}


EFI_STATUS MySampleProtocolInit(VOID)
{
    RobinSampleProtocol.Revision=0x101;
    RobinSampleProtocol.MySample_In=MySample_In;
    RobinSampleProtocol.MySample_Out=MySample_Out;
    RobinSampleProtocol.MySample_DoSth=MySample_DoSth;

    return EFI_SUCCESS;
}
