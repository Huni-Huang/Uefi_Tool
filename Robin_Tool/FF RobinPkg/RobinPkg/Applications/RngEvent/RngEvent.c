/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include "Common.h"
#include "FileRW.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>


extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
INT32 robin_rand(); //robin's random function
/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  UINT64 flag;
  UINTN i;
  // Print(L"Hello there fellow Programmer.\n");
  // Print(L"Welcome to the world of EDK II.\n");
  Print(L"Argc=%d\n", Argc);
  for (i = 0; i < Argc;i++)
  {
    Print(L"Argv[%d]= %s\n", i,Argv[i]);
  }
    
        flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO | RNG_OUT);
	Print(L"flag=%x\n",flag);
	WaitKey();
  

{
		//图形显示测试
    EFI_EVENT myEvent;
    EFI_STATUS Status;
    UINTN       repeats=0;
    UINTN       index=0;
    
    //DisplayModeInformation();
		// UINT32 ModeIndex;
		// for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		// {
		// 	DisplaySpecifyModeMessage(ModeIndex);
		// 	WaitKey();
		// }

		SwitchGraphicsMode(TRUE);
		SetBKG(&(gColorTable[3]));
		// WaitKey();
    //create timer event and settimer
    Status = gBS->CreateEvent(EVT_TIMER  , TPL_CALLBACK, (EFI_EVENT_NOTIFY)NULL, (VOID*)NULL, &myEvent);
    Status = gBS->SetTimer(myEvent,TimerPeriodic , 2 * 1000 * 1000); //triggered per 200ms
    while(1)
    {
      UINT32 rand_x1, rand_y1;
      UINT32 rand_x2, rand_y2;
      UINT32 randColor;
      Status = gBS->WaitForEvent(1, &myEvent, &index);
      //do something
      rand_x1 = (robin_rand() % SY_SCREEN_WIDTH);
      rand_y1 = (robin_rand() % SY_SCREEN_HEIGHT);
      randColor = (robin_rand()) % 10;  //gColorTable has 10 elements
      rand_x2 = rand_x1 + 30;
      rand_y2 = rand_y1 + 30;
      if(rand_x2>(SY_SCREEN_WIDTH-1))
        rand_x2 = SY_SCREEN_WIDTH - 1;
      if(rand_y2>(SY_SCREEN_HEIGHT-1))
        rand_y2 = SY_SCREEN_HEIGHT - 1;
      // putpixel(rand_x, rand_y, &(gColorTable[8]));
      rectblock(rand_x1,rand_y1,rand_x2,rand_y2,&(gColorTable[randColor]));
      if(repeats ++ == 10000){
            break;
        }
    }
    Status = gBS->CloseEvent(myEvent);
		//rectblock(0,0,799,599-1,&(gColorTable[1])); //white
		rectangle(100,100,150,150,&(gColorTable[8]));//red
		circle(300,300,40,&(gColorTable[1]));//blue
		rectblock(450,450,500,500,&(gColorTable[6]));
		WaitKey();
		// ShowBmp();
		WaitKey();
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);
	}
  return(0);
}




/** Compute a pseudo-random number.
  *
  * Compute x = (7^5 * x) mod (2^31 - 1)
  * without overflowing 31 bits:
  *      (2^31 - 1) = 127773 * (7^5) + 2836
  * From "Random number generators: good ones are hard to find",
  * Park and Miller, Communications of the ACM, vol. 31, no. 10,
  * October 1988, p. 1195.
**/
// #define RAND_MAX  0x7fffffff
static UINT32 next = 1;
INT32 robin_rand()
{
  INT32 hi, lo, x;

  /* Can't be initialized with 0, so use another value. */
  if (next == 0)
    next = 123459876;
  hi = next / 127773;
  lo = next % 127773;
  x = 16807 * lo - 2836 * hi;
  if (x < 0)
    x += 0x7fffffff;
  return ((next = x) % ((UINT32)0x7fffffff + 1));
}