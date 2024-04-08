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

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleTextInEx.h>
#include "MyProtocol.h"


/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  	EFI_STATUS      Status;
	EFI_MYSAMPLE_PROTOCOL *myprotocol;
	
	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2019-10-15 12:47:33\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Test ServiceDriverSample--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
	//text out test

	//1 Locate MyProtocol
	Print(L"1. Locate Myprocotol..\n");
	Status=gBS->LocateProtocol(&gEfiMYSampleProtocolGUID,NULL, (VOID **)&myprotocol);
	if(EFI_ERROR(Status))
	{
		Print(L"LocatProtocol error %r\n", Status);
		return Status;
	}
	//2 Test function 
	Status=myprotocol->MySample_In(myprotocol,L"Hello,My protocol!\n");
	Status=myprotocol->MySample_DoSth(myprotocol,L"Enjoy UEFI!\n");
	Status=myprotocol->MySample_Out(myprotocol);
	
  return EFI_SUCCESS;
}
