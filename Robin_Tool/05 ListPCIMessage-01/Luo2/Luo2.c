/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>
//所有包含的目录到mdepkg下去找
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>
//#include <Library/PciLib.h>
//#include <Library/PciExpressLib.h>
//#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>						//获取PciIO protocol所需
#include <Protocol/PciRootBridgeIo.h>	//获取PciRootBridgeIO protocol所需
#include <IndustryStandard/Pci.h>  //pci访问所需的头文件，包含pci22.h,pci23.h...

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

//Macro defined
//#define EFI_PCI_ADDRESS(bus, dev, func, reg) \
//    ((UINT64) ((((UINTN) bus) << 24) + (((UINTN) dev) << 16) + (((UINTN) func) << 8) + ((UINTN) reg)))
//Global variables
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;
EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
EFI_PCI_IO_PROTOCOL *gPCIIO;
//Functions declare
EFI_STATUS LocateSimpleTextInputEx(void);
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
EFI_STATUS GetKey(EFI_INPUT_KEY *key);
VOID FlushKeyBuffer(VOID);
VOID WaitKey(VOID);

EFI_STATUS LocateGraphicsOutput (void);
EFI_STATUS SwitchGraphicsMode(BOOLEAN flag);
VOID SetMyMode(UINT32 ModeNumber);
VOID putpixel(UINTN x,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);

VOID HLine(UINTN x1,UINTN x2,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID VLine(UINTN x,UINTN y1,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  rectblock(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  rectangle(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);
VOID  circle(UINTN centerx,UINTN centery,UINTN radius,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color);

EFI_STATUS LocatePCIRootBridgeIO(void);
EFI_STATUS LocatePCIIO(void);
EFI_STATUS PciDevicePresent (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridgeIo,
  OUT PCI_TYPE00                          *Pci,
  IN  UINT8                               Bus,
  IN  UINT8                               Device,
  IN  UINT8                               Func
  );
EFI_STATUS GetSpecifyPciDevice(
	IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridgeIo,
  OUT PCI_TYPE00                          *Pci,
  IN	UINT16 VendorID,
  IN	UINT16 DeviceID
  );
EFI_STATUS ListPCIMessage(void);

EFI_STATUS DisplayModeInformation(void);
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber);
EFI_STATUS DisplayTextModeInformation(void);
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber);

EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[]={
  0,    0,    0,    0,        /*  BLACK */\
  255,  255,  255,  0,        /*  WHITE */\
  200,  208,  212,  0,        /*  LIGHT GRAY  */\
  167,  95, 18, 0,        /*  DEEP BLUE */\
  64, 64, 64, 0,        /*  DARK BLACK  */\
  128,  128,  128,  0,        /*  LIGHT BLACK */\
  0,    255,  255,  0,        /*  YELLOW  */\
  0,    153,  0,    0,        /*  GREEN */\
  0,    0,    153,  0,        /*  RED   */\
  192,  0,    0,    0,        /* BLUE   */  
};
/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
int
main (
  IN int Argc,
  IN char **Argv
  )
{
	EFI_STATUS      Status;
	PCI_TYPE00 Pci;
//	UINT16 scanCode=0;
//	UINT16 uniChar=0;
//	UINT32 shiftState;
//	EFI_KEY_TOGGLE_STATE toggleState;
//	UINT32 count=0;
//	EFI_INPUT_KEY key={0,0};
	
	gST->ConOut->OutputString(gST->ConOut,L"========= EDKII Test Samples ========\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Author: luobing  \n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Data: 2013-2-1 11:57:51\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"Context: Control Keyboard input--\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"======================================\n\r");
	//text out test
//	iStatus = gBS->AllocatePool(EfiBootServicesData,0x100,(void**)&buffer);
//	if (EFI_ERROR (iStatus)) {
//    return EFI_UNSUPPORTED;
//  }
//  SetMem(buffer,0x100,0x00);
//	j=13;
//	//swprintf (WName, (strlen (StringIdentifierName) + 1) * sizeof (WCHAR), L"%S", StringIdentifierName);
//	swprintf(buffer,99,L"Now is 20%d, the world is exist!\n",j);
//	gST->ConOut->OutputString(gST->ConOut,buffer);
//	gBS->FreePool(buffer);
	//测试键盘输入的例子，打印所有我需要的键盘信息
	gST->ConOut->OutputString(gST->ConOut,L"begin...\n\r");
	gST->ConOut->OutputString(gST->ConOut,L"please input key(ESC to exit):\n\r");
//调试pci
	Status=LocatePCIRootBridgeIO();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocatePCIRootBridgeIO, Can't find protocol!\n");
	}
	else
	{
		Print(L"Call LocatePCIRootBridgeIO, Find protocol!\n");
	}
	Status=LocatePCIIO();
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call LocatePCIIO, Can't find protocol!\n");
	}
	else
	{
		Print(L"Call LocatePCIIO, Find protocol!\n");
	}
	ListPCIMessage();
	Status=GetSpecifyPciDevice(gPCIRootBridgeIO,&Pci,0x9999,0x8000);
	if (EFI_ERROR (Status)) 
	{
		Print(L"Call GetSpecifyPciDevice, Can't find device!\n");
	}
	else
	{
		Print(L"Call GetSpecifyPciDevice, device!\n");
		Print(L"BaseAddress0 = %x\n",Pci.Device.Bar[0]);
		Print(L"BaseAddress2 = %x\n",Pci.Device.Bar[1]);
		Print(L"BaseAddress2 = %x\n",Pci.Device.Bar[2]);
		Print(L"BaseAddress3 = %x\n",Pci.Device.Bar[3]);
		Print(L"BaseAddress4 = %x\n",Pci.Device.Bar[4]);
		Print(L"BaseAddress5 = %x\n",Pci.Device.Bar[5]);
		
	}
	
	
	//lbdebug begin
//	Status=LocateSimpleTextInputEx();
//	if (EFI_ERROR (Status)) 
//	{
//		Print(L"Call LocateSimpleTextInputEx, Can't find protocol!\n");
//	}
//	else
//	{
//		Print(L"Call LocateSimpleTextInputEx, Find protocol!\n");
//	}
//	Status=LocateGraphicsOutput();
//	if (EFI_ERROR (Status)) 
//	{
//		Print(L"Call LocateGraphicsOutput, Can't find graphics protocol!\n");
//	}
//	else
//	{
//		Print(L"Call LocateGraphicsOutput, Find graphics protocol!\n");
//	}
//	{
//		//DisplayTextMode...
//		UINTN ModeIndex;
//		DisplayTextModeInformation();
//		for(ModeIndex=0;ModeIndex<(UINTN)(gST->ConOut->Mode->MaxMode);ModeIndex++)
//		{
//			DisplaySpecifyTextModeMessage(ModeIndex);
//			WaitKey();
//		}
//	}
//	{
//		
//		UINT32 ModeIndex;
//		DisplayModeInformation();
//		for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
//		{
//			DisplaySpecifyModeMessage(ModeIndex);
//			WaitKey();
//		}
//	}
//	{
//		//图形显示测试
//		SwitchGraphicsMode(TRUE);
//		SetMyMode(0x0);
//		WaitKey();
//		rectblock(0,0,799,599-1,&(gColorTable[1])); //white
//		rectangle(100,100,150,150,&(gColorTable[8]));//red
//		circle(300,300,40,&(gColorTable[3]));//blue
//		rectblock(450,450,500,500,&(gColorTable[6]));
//		WaitKey();
//		SetMyMode(0x3);
//		SwitchGraphicsMode(FALSE);
//		
//	}
//	//lbdebug end
//	
//	while(key.ScanCode!=0x17)	//ESC
//	{
//		//Status=GetKeyEx(&scanCode,&uniChar,&shiftState,&toggleState);
//		Status=GetKey(&key);
//		if (EFI_ERROR (Status)) 
//		{
//			Print(L"Call GetKeyEx() Error!\n");
//			break;
//		}
//		else
//		{
//			Print(L"NO.%08d\n",count);
//			++count;
//			Print(L"  ScanCode=%04x",key.ScanCode);
//			Print(L"  UnicodeChar=%04x",key.UnicodeChar);
////			Print(L"  ShiftState=%08x",shiftState);
////			Print(L"  ToggleState=%02x",toggleState);
//			Print(L"\n");
//		}
//	}
	return 0;
}


//===========================================Keyboard function begin====================================
//Name:LocateSimpleTextInputEx
//Input:
//Output: EFI_STATUS=EFI_SUCCESS, *SimpleTextInputEx
//				EFI_STATUS=
//Descriptor: Get the handle of SimpleTextInputEx
EFI_STATUS LocateSimpleTextInputEx(void)
{
	EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;

  
  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &gSimpleTextInputEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			return EFI_SUCCESS;
  	}
  }	 
	return Status;
}
//Name:  GetKeyEx
//Input:
//Output:
//Descriptor: 
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)
{
	EFI_STATUS                        Status;
  EFI_KEY_DATA                      KeyData;
 	UINTN															Index;
	  
	gBS->WaitForEvent(1,&(gSimpleTextInputEx->WaitForKeyEx),&Index);
	Status = gSimpleTextInputEx->ReadKeyStrokeEx(gSimpleTextInputEx,&KeyData);
	if(!EFI_ERROR(Status))
	{
 		*ScanCode=KeyData.Key.ScanCode;
		*UniChar=KeyData.Key.UnicodeChar;
		*ShiftState=KeyData.KeyState.KeyShiftState;
		*ToggleState=KeyData.KeyState.KeyToggleState;
		return EFI_SUCCESS;
	} 
	return Status;
}
//Function Name: FlushKeyBuffer
//Input: none
//Output: none
//Description: Flush all keyboard buffer
VOID FlushKeyBuffer(VOID)
{
  EFI_INPUT_KEY     Key;
 
  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
}
//Function Name: WaitKey
//Input: none
//Output: none
//Description: Wait any key press
VOID WaitKey(VOID)
{
  EFI_INPUT_KEY     Key;
  UINTN Index;

  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
  gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return;
}
//Function Name: GetKey
//Input: none
//Output: none
//Description: Get key code,return unicode code and scan code(such as F1...)
EFI_STATUS GetKey(EFI_INPUT_KEY *key)
{
	UINTN Index;
	gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return gST->ConIn->ReadKeyStroke(gST->ConIn,key);  
}
//===========================================Keyboard function end====================================

//===========================================Graphic function begin====================================
//Name: LocateGraphicsOutput
//Input: 
//Output: *GraphicsOutput
//Descriptor:
EFI_STATUS LocateGraphicsOutput (void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *GraphicsOutputControllerHandles = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports GraphicsOutputProtocol
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&HandleCount,
		&GraphicsOutputControllerHandles
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			GraphicsOutputControllerHandles[HandleIndex],
			&gEfiGraphicsOutputProtocolGuid,
			(VOID**)&gGraphicsOutput);
		if (EFI_ERROR(Status))	continue;

		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: SwitchGraphicsMode(BOOLEAN flag)
//Input:
//Output:
EFI_STATUS SwitchGraphicsMode(BOOLEAN flag)
{
	EFI_STATUS                         Status;
//	if(flag)
//	{
//		Status=gST->ConOut->SetMode(gST->ConOut,EfiConsoleControlScreenGraphics);
//	}
//	else
//	{
//		Status=gST->ConOut->SetMode(gST->ConOut,EfiConsoleControlScreenText);
//	}
	if(flag)
	{
		Status=gST->ConOut->EnableCursor (gST->ConOut, FALSE);
	}
	else
	{
		Status=gST->ConOut->EnableCursor (gST->ConOut, TRUE);
	}
	return Status;
}
//Function Name:  SetMyMode
//Input: ModeNumber
//Output: Void
//Description: Set Specify mode
VOID SetMyMode(UINT32 ModeNumber)
{
   gGraphicsOutput->SetMode(gGraphicsOutput,ModeNumber);
   return;
}
VOID putpixel(UINTN x,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
//  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,x,y,1,1,0);
}
//Function Name:  HLine
//Input: x1,x2,y,color (x1<x2)
//Output: Void
//Description: Draw horizontal line
VOID HLine(UINTN x1,UINTN x2,UINTN y,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN minx,maxx;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  minx=(x1<x2)?x1:x2;
  maxx=(x1>x2)?x1:x2;
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,minx,y,(maxx-minx+1),1,0);  
}
//Function Name:  VLine
//Input: x,y1,y2,color(y1<y2)
//Output: Void
//Description: Draw vertical line
VOID VLine(UINTN x,UINTN y1,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN miny,maxy;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  miny=(y1<y2)?y1:y2;
  maxy=(y1>y2)?y1:y2;
  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,x,miny,1,(maxy-miny+1),0);  
}
//Function Name:  rectblock
//Input: x1,y1,x2,y2,color(y1<y2)
//Output: Void
//Description: Draw block rect
VOID  rectblock(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN minx,miny,maxx,maxy;
  
  minx=(x1<x2)?x1:x2;
  miny=(y1<y2)?y1:y2;
  maxx=(x1>x2)?x1:x2;
  maxy=(y1>y2)?y1:y2;
  
  gGraphicsOutput->Blt (gGraphicsOutput,color,EfiBltVideoFill,0,0,minx,miny,(maxx-minx+1),(maxy-miny+1),0);   
}

//another function
//Function Name:  rectangle
//Input: x1,y1,x2,y2,color_number: Pointer to ColorTable(y1<y2)
//Output: Void
//Description: Draw Rectangle
VOID  rectangle(UINTN x1,UINTN y1,UINTN x2,UINTN y2,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  UINTN left,top,right,bottom,i;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;

  /* Location */
  left=(x1<x2)?x1:x2;
  right=(x1>x2)?x1:x2;
  top=(y1<y2)?y1:y2;
  bottom=(y1>y2)?y1:y2;
//  right=(right<maxX)?right:maxX;
//  bottom=(bottom<maxY)?bottom:maxY;       /* Cut off the graphic which out of screen */
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    
  for(i=left;i<right;i++)putpixel(i,top,color);
  for(i=top;i<bottom;i++)putpixel(right,i,color);
  for(i=right;i>left;i--)putpixel(i,bottom,color);
  for(i=bottom;i>top;i--)putpixel(left,i,color);
}
//Function Name:  circle
//Input: centerx,centery,radius,color
//Output: Void
//Description: Draw Rectangle
VOID  circle(UINTN centerx,UINTN centery,UINTN radius,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  signed int x,y,d;
//  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
  
//  MemCpy(&color,&(gColorTable[color_number]),sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if(!radius)return;
  y=(signed int)(radius);
  d=3-((signed int)(radius)<<1);
  
  
  /*see foxdisk3.01 by luobing*/
  putpixel(centerx,centery+y,color);        /*0,-1*/
  putpixel(centerx,centery-y,color);        /*0,1*/
  putpixel(centerx+y,centery,color);
  putpixel(centerx-y,centery,color);
  
  for(x=1;x<y;x++)
  {
    putpixel(centerx+x,centery+y,color);
    putpixel(centerx+x,centery-y,color);
    putpixel(centerx-x,centery-y,color);
    putpixel(centerx-x,centery+y,color);
    putpixel(centerx+y,centery+x,color);
    putpixel(centerx+y,centery-x,color);
    putpixel(centerx-y,centery-x,color);
    putpixel(centerx-y,centery+x,color);
    if(d<0)d+=((x<<2)+6);           
    else d+=(((x-(y--))<<2)+10);        
  }
  /* x=y */
  putpixel(centerx+y,centery+y,color);
  putpixel(centerx+y,centery-y,color);
  putpixel(centerx-y,centery-y,color);
  putpixel(centerx-y,centery+y,color);
}   
//Function Name: SetBKG
//Input: color
//Output: None
//Description: set background
//VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
//{
//  rectblock(0,0,SY_SCREEN_WIDTH-1,SY_SCREEN_HEIGHT-1,color);
//}
//===========================================Graphic function end======================================
//===========================================PCI function begin======================================
//Name: LocatePCIRootBridgeIO
//Input: 
//Output: *gPCIRootBridgeIO
//Descriptor:
EFI_STATUS LocatePCIRootBridgeIO(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PciHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciRootBridgeIoProtocolGuid,
		NULL,
		&HandleCount,
		&PciHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
//	if (EFI_ERROR(Status))	
//	{
//		Print(L"LocatePCIRootBridgeIO: err 1\n");//在此处就找不到？？
//		return Status;		//unsupport
//	}
	
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PciHandleBuffer[HandleIndex],
			&gEfiPciRootBridgeIoProtocolGuid,
			(VOID**)&gPCIRootBridgeIO);
		if (EFI_ERROR(Status))	continue;
//		if (EFI_ERROR(Status))	
//		{
//			Print(L"LocatePCIRootBridgeIO: err 2,HandleIndex=%d\n",HandleIndex);
//			continue;
//		}
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: LocatePCIIO
//Input: 
//Output: *gPCIIO
//Descriptor:
EFI_STATUS LocatePCIIO(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PciHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&HandleCount,
		&PciHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PciHandleBuffer[HandleIndex],
			&gEfiPciIoProtocolGuid,
			(VOID**)&gPCIIO);
		if (EFI_ERROR(Status))	continue;
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
/**
  This routine is used to check whether the pci device is present.

  @param PciRootBridgeIo   Pointer to instance of EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Pci               Output buffer for PCI device configuration space.
  @param Bus               PCI bus NO.
  @param Device            PCI device NO.
  @param Func              PCI Func NO.

  @retval EFI_NOT_FOUND    PCI device not present.
  @retval EFI_SUCCESS      PCI device is found.

**/
//从\MdeModulePkg\Bus\Pci\PciBusDxe\PciEnumerator.c中拿来的
EFI_STATUS PciDevicePresent (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridgeIo,
  OUT PCI_TYPE00                          *Pci,
  IN  UINT8                               Bus,
  IN  UINT8                               Device,
  IN  UINT8                               Func
  )
{
  UINT64      Address;
  EFI_STATUS  Status;
  
  // Create PCI address map in terms of Bus, Device and Func
  Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);
  // Read the Vendor ID register
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  Address,
                                  1,
                                  Pci
                                  );

  if (!EFI_ERROR (Status) && (Pci->Hdr).VendorId != 0xffff) {
    // Read the entire config header for the device  
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    sizeof (PCI_TYPE00) / sizeof (UINT32),
                                    Pci
                                    );

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

//Name: ListPCIMessage(void)
//Input:
//Output:
EFI_STATUS ListPCIMessage(void)
{
	EFI_STATUS                         Status=EFI_SUCCESS;
	PCI_TYPE00 Pci;
	UINT16 i,j,k,count=0;
	
	Print(L"NO.  Bus. Dev. Func. | Vendor. Device. ClassCode\n");
	for(k=0;k<128;k++)//应该没有这么多总线吧
		for(i=0;i<=PCI_MAX_DEVICE;i++)
			for(j=0;j<=PCI_MAX_FUNC;j++)
			{
				Status = PciDevicePresent(gPCIRootBridgeIO,&Pci,(UINT8)k,(UINT8)i,(UINT8)j);
				if (Status == EFI_SUCCESS)		//Find device
				{
					++count;
					if((count%16) ==0)	WaitKey();		//lbdebug
					Print(L" %d    %x    %x    %x",count,(UINT8)k,(UINT8)i,(UINT8)j);
					Print(L"       %x       %x        %x",Pci.Hdr.VendorId,Pci.Hdr.DeviceId,Pci.Hdr.ClassCode[0]);
					Print(L"\n");
				}
			}
	return EFI_SUCCESS;
}
//Name GetSpecifyPciDevice
//Input:
//Output:
EFI_STATUS GetSpecifyPciDevice(
	IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridgeIo,
  OUT PCI_TYPE00                          *Pci,
  IN	UINT16 VendorID,
  IN	UINT16 DeviceID
  )
{
	UINT8 Bus,Dev,Func;
	EFI_STATUS                         Status=EFI_NOT_FOUND;
	BOOLEAN myFlag=TRUE;
	//Find specify device
	for(Bus=0;Bus<=10;Bus++)		//应该没有这么多总线吧
		for(Dev=0;Dev<=PCI_MAX_DEVICE;Dev++)
			for(Func=0;Func<=PCI_MAX_FUNC;Func++)
			{
				if(myFlag)
				{
					Status = PciDevicePresent(gPCIRootBridgeIO,Pci,Bus,Dev,Func);
					if (Status == EFI_SUCCESS)		//Find device
					{
						Print(L"mes:   %x    %x    %x",Bus,Dev,Func);
						Print(L"       %x       %x        %x\n",(Pci->Hdr).VendorId,(Pci->Hdr).DeviceId,(Pci->Hdr).ClassCode[0]);
						if(((Pci->Hdr).VendorId == VendorID) && ((Pci->Hdr).DeviceId == DeviceID))
						{
							Status=EFI_SUCCESS;
							myFlag=FALSE;
							Print(L"GetSpecifyPciDevice OK,bus=%x,dev=%x,func=%d\n",Bus,Dev,Func);
							break;
						}
					}
				}
			}
	
	return Status;
}
//===========================================PCI function end======================================
//================Debug function================
//Name: DisplayModeInformation
//Input:
//Output:
EFI_STATUS DisplayModeInformation(void)
{
	Print(L"Print Current Mode Message:\n");
	Print(L"::MaxMode=%02x\n",gGraphicsOutput->Mode->MaxMode);
	Print(L"::Current Mode=%02x\n",gGraphicsOutput->Mode->Mode);
	Print(L"::SizeOfInfo=%x\n",gGraphicsOutput->Mode->SizeOfInfo);
	Print(L"::Info->Version=%x\n",gGraphicsOutput->Mode->Info->Version);
	Print(L"::Info->HorizontalResolution=%x\n",gGraphicsOutput->Mode->Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%x\n",gGraphicsOutput->Mode->Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%x\n",gGraphicsOutput->Mode->Info->PixelsPerScanLine);
	
	return EFI_SUCCESS;
}
//Name: DisplaySpecifyModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyModeMessage(UINT32 ModeNumber)
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *Info;
	UINTN                                  SizeOfInfo;
	EFI_STATUS                         Status;
	
	Status=gGraphicsOutput->QueryMode(gGraphicsOutput,ModeNumber,&SizeOfInfo,&Info);
	Print(L"Print Mode %d Message:\n",ModeNumber);
	Print(L"::Info->Version=%d\n",Info->Version);
	Print(L"::Info->HorizontalResolution=%d\n",Info->HorizontalResolution);
	Print(L"::Info->VerticalResolution=%d\n",Info->VerticalResolution);
	Print(L"::Info->PixelsPerScanLine=%d\n",Info->PixelsPerScanLine);
	
	return Status;
}
//Name: DisplayTextModeInformation(void)
//Input:
//Output:
EFI_STATUS DisplayTextModeInformation(void)
{
	Print(L"Print SimPleTextMode Message:\n");
	Print(L"::MaxMode=%d\n",gST->ConOut->Mode->MaxMode);
	Print(L"::Current Mode=%d\n",gST->ConOut->Mode->Mode);
	Print(L"::Attribute=%d\n",gST->ConOut->Mode->Attribute);
	Print(L"::CursorColumn=%d\n",gST->ConOut->Mode->CursorColumn);
	Print(L"::CursorRow=%d\n",gST->ConOut->Mode->CursorRow);
	Print(L"::CursorVisible=%d\n",gST->ConOut->Mode->CursorVisible);
	
	return EFI_SUCCESS;	
}

//Name: DisplaySpecifyTextModeMessage(void)
//Input:
//Output:
EFI_STATUS DisplaySpecifyTextModeMessage(UINTN ModeNumber)
{
	EFI_STATUS                         Status;
	UINTN Columns;
	UINTN Rows;
	
	Status=gST->ConOut->QueryMode(gST->ConOut,ModeNumber,&Columns,&Rows);
	Print(L"Print Text Mode %d Message:\n",ModeNumber);
	Print(L"::Column=%d\n",Columns);
	Print(L"::Row=%d\n",Rows);
	
	return Status;
}