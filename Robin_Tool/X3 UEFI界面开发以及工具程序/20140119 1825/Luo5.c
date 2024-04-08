/** @file
  This sample application bases on HelloWorld PCD setting 
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#include "Common.h"
//#include "Smbus.h"
#include "Keyboard.h"
#include "Graphic.h"

#include "Font.h"
#include "Logo.h"
#include "Window.h"

#include "TalkCard.h"

//#include "Version.h"
//#include "SafeModule.h"
#include "MainMenu.h"

UINT32 HorRes = SY_SCREEN_WIDTH;
UINT32 VerRes = SY_SCREEN_HEIGHT;

//*************************** Extern Variable *******************************************
extern EFI_SYSTEM_TABLE    *gST;           //Pointer to system table
extern EFI_BOOT_SERVICES   *gBS;           //Pointer to boot services                              //bit 3
extern UINT16 PMBASE_ADDR;
VOID GLK_BIOS(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
);

//Aaron{
extern EFI_GRAPHICS_OUTPUT_PROTOCOL  *gGraphicsOutput;   //Protocol of Graphics output
UINT32 DualNetModeNum = 0;
EFI_STATUS GetGraphicsModeNumber (
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, 
    IN  UINT32                       HorRes,
    IN  UINT32                       VerRes,
	OUT UINT32                       *ModeNum,
    IN  BOOLEAN                      ExactMatch,
    OUT UINT32                       *ActualHorRes OPTIONAL,
    OUT UINT32                       *ActualVerRes OPTIONAL
)
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION	Info;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION	*pInfo = &Info;
	EFI_STATUS				                Status;
	UINTN							        SizeOfInfo;
	UINT32							        i;

	for(i = 0; i < GraphicsOutput->Mode->MaxMode; i++) {
		Status = GraphicsOutput->QueryMode(GraphicsOutput, i, &SizeOfInfo, &pInfo);

		if (!EFI_ERROR(Status)) { 
            if (ExactMatch && pInfo->HorizontalResolution == HorRes && pInfo->VerticalResolution == VerRes ) {
			    *ModeNum = i;
                return Status;
            }
            if(!ExactMatch && pInfo->HorizontalResolution >= HorRes && pInfo->VerticalResolution >= VerRes ) {
                *ModeNum = i;
                *ActualHorRes = pInfo->HorizontalResolution;
                *ActualVerRes = pInfo->VerticalResolution;
                return Status;
            }
		}
	}
    return EFI_NOT_FOUND;
}
//Aaron}
           
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{  
  GLK_BIOS(ImageHandle,SystemTable);
  
  return EFI_SUCCESS;
}
//=========================== Function ============================================ 
//Function Name: SYBIOS
//Input:
//Output:
//Description:
VOID GLK_BIOS(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
   EFI_STATUS Status;


	UINT64 flag;
	
	flag=InintGloabalProtocols(S_TEXT_INPUT_EX|GRAPHICS_OUTPUT|PCI_ROOTBRIDGE_IO|PCI_IO|SMBUSHC);
//	GetPMBASE();
	
  Status = GetGraphicsModeNumber(gGraphicsOutput, HorRes, VerRes, &DualNetModeNum, TRUE, NULL, NULL);
    if(EFI_ERROR(Status))
    	{
    		Status = GetGraphicsModeNumber(gGraphicsOutput, HorRes, VerRes, &DualNetModeNum, FALSE, &HorRes, &VerRes);
        if(EFI_ERROR(Status))
        	DualNetModeNum = 2; //default to Mode 2
      }

	main();
	SwitchGraphicsMode(FALSE);
}

