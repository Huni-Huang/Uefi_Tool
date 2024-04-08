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
#include "Mouse.h"
#include "Font.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"
#include "MyGUI.h"
#include "Pictures.h"
#include "GUIPIC.h"
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>


extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
extern EFI_SIMPLE_POINTER_PROTOCOL *gMouse;   //Mouse
extern EFI_BLOCK_IO_PROTOCOL* gBlockIoArray[256];
extern UINTN nBlockIO;

INT32 robin_rand(); //robin's random function



VOID NotifyWaitFunc(IN EFI_EVENT Event, IN VOID *Context);
VOID NotifySignalFunc(IN EFI_EVENT Event, IN VOID *Context);
VOID TestNotifyEvent(VOID);
VOID RandomBlock(UINT32 Width,UINT32 Height,EFI_GRAPHICS_OUTPUT_BLT_PIXEL* BltArray,UINT32 BltArraySize);

EFI_STATUS HotKeySample(IN EFI_KEY_DATA *hotkey);
EFI_STATUS HotKeyNotifyFunc(IN EFI_KEY_DATA *hotkey);

VOID dumpBlockIOMedia(EFI_BLOCK_IO_PROTOCOL* gBlockIo);

int
main (
  IN int Argc,
  IN char **Argv
  )
{
  UINT64 flag;
  UINT16 i,j;
  // char buff[200];
  UINT16 number;
  EFI_LBA rAddress;
  UINT8 Buffer[512];  //读取一个扇区数
  // UINT32 rAddress;
  // UINT8 *s_text = "1234567890abcdABCD";  

  // flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO | SIMPLE_POINTER);
  flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | FILE_IO | BLOCK_IO);
	Print(L"flag=%x\n",flag);
	
  if(Argc == 1)  //列出所有BlockIO设备
  {
    Print(L"BlockIO counts: %d\n",nBlockIO);
    for(i=0; i<nBlockIO; i++)
    {
      Print(L" Number %02d:\n",i);
      dumpBlockIOMedia(gBlockIoArray[i]);
      WaitKey();
    }
  }
  else if(Argc == 2)
  {
  
    if((strcmp("-h",Argv[1])==0) ||(strcmp("-H",Argv[1])==0) ||  (strcmp("-?",Argv[1])==0))
    {
      Print(L"Syntax: Diskdump x y\n");
      Print(L"  x: number of BlockIO\n");
      Print(L"  y: LBA Address\n");
    }
  }
  else if(Argc == 3)
  {
    EFI_STATUS Status;
    sscanf(Argv[1],"%d",&number);
    sscanf(Argv[2],"%lld",&rAddress);
    Print(L"Get data from: BlockIo[%x],LBA-%ld\n",number,rAddress);
    if(number > (UINT16)nBlockIO)
    {
      Print(L"Error: Out of range!\n");
    }
    else
    {
      Status = gBlockIoArray[number]->ReadBlocks(
                        gBlockIoArray[number],
                        gBlockIoArray[number]->Media->MediaId,
                        rAddress,512,Buffer);
      if(EFI_ERROR(Status))
        Print(L"%r\n",Status);
      else
      {
        Print(L"--------0--1--2--3--4--5--6--7--8--9--A--B--C--D--E--F-\n");
        for(i=0; i<32; i++)
        {
          if(i==16)
          {
            WaitKey();
            Print(L"--------0--1--2--3--4--5--6--7--8--9--A--B--C--D--E--F-\n");
          }
          Print(L"0x%03x:  ",i*16);
          gST->ConOut->SetAttribute(gST->ConOut,EFI_BACKGROUND_RED|EFI_WHITE);
          for(j=0; j<16; j++)
          {      
            Print(L"%02x",Buffer[i*16+j]);
            if(j<15)Print(L" ");
          }
          gST->ConOut->SetAttribute(gST->ConOut,EFI_BACKGROUND_BLACK|EFI_LIGHTGRAY);
          Print(L"\n");
        }
      }
    }
    
   

  }
  
  // SwitchGraphicsMode(TRUE);
  // InSYGraphicsMode();
	// SetBKG(&(gColorTable[3]));
  // // ShowBMP24True(L"mygui.bmp",400,100);
  // ShowMyGuiPic(400,100);
  // InitGUI();
  // HanlderGUI();

  // if (EFI_ERROR (DisplayImage("bounce.bmp"))) {
  //   return 1;
  // }
  
  // bounce();

  // SetMyMode(0x0);
  // OutSYGraphicsMode();
	// SwitchGraphicsMode(FALSE);
  return(0);
}

VOID RandomBlock(UINT32 Width,UINT32 Height,EFI_GRAPHICS_OUTPUT_BLT_PIXEL* BltArray,UINT32 BltArraySize)
{
  //随机显示图形块测试
  EFI_EVENT myEvent;
  EFI_STATUS Status;
  UINTN       repeats=0;
  UINTN       index=0;
  UINT32 rand_x1, rand_y1,randColor;
  
  //create timer event and settimer
  Status = gBS->CreateEvent(EVT_TIMER  , TPL_CALLBACK, (EFI_EVENT_NOTIFY)NULL, (VOID*)NULL, &myEvent);
  Status = gBS->SetTimer(myEvent,TimerPeriodic , 2 * 1000 * 1000); //triggered per 200ms
  while(1)
  {
    Status = gBS->WaitForEvent(1, &myEvent, &index);
    //do something
    rand_x1 = (robin_rand() % Width);
    rand_y1 = (robin_rand() % Height);
    randColor = (robin_rand()) % BltArraySize;     
    rectblock(rand_x1,rand_y1,rand_x1+20,rand_y1+20,&(BltArray[randColor]));
    if(repeats ++ == 100){
          break;
      }
  }
  Status = gBS->CloseEvent(myEvent);
}

VOID TestNotifyEvent(VOID)
{
	EFI_EVENT myWaitEvent;
  EFI_EVENT mySignalEvent;
	UINTN       index=0;
  EFI_STATUS Status;

	Status = gBS->CreateEvent(EVT_NOTIFY_WAIT, TPL_NOTIFY, (EFI_EVENT_NOTIFY)NotifyWaitFunc, (VOID*)L"Hi,UEFI!", &myWaitEvent);
	Print(L"CreateEvent myWaitEvent=%r\n",Status);
  Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, (EFI_EVENT_NOTIFY)NotifySignalFunc, (VOID*)L"Hi,me!", &mySignalEvent);
  Print(L"CreateEvent mySignalEvent=%r\n",Status);
	Status = gBS->WaitForEvent(1, &myWaitEvent, &index);
  Print(L"myWaitEvent is signaled! index=%d %r\n",index,Status);
  gBS->SignalEvent(mySignalEvent);
  Status = gBS->WaitForEvent(1, &mySignalEvent, &index);
  Print(L"mySignalEvent is signaled! index=%d %r\n",index, Status);
  gBS->CloseEvent(myWaitEvent);
  gBS->CloseEvent(mySignalEvent);
}

VOID NotifyWaitFunc(IN EFI_EVENT Event, IN VOID *Context)
{
  static UINTN count = 0;
  Print(L"NotifyWaitFunc: count=%d, Context=%s\n",count,Context);
  count++;
  if((count%5)==0)
    gBS->SignalEvent(Event);
    
}

VOID NotifySignalFunc(IN EFI_EVENT Event, IN VOID *Context)
{
  Print(L"NotifySignalFunc: Context=%s\n",Context);
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

INT32 robin_rand(VOID)
{
  INT32 hi, lo, x;
  static UINT32 next = 1;
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

//--------------hot key sample---------------------------
extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gConInEx;

EFI_STATUS HotKeySample(IN EFI_KEY_DATA *hotkey)
{
  EFI_STATUS Status;
  EFI_KEY_DATA key;
  EFI_HANDLE hotkeyNotifyHandle;
  
  hotkey->KeyState.KeyShiftState|=EFI_SHIFT_STATE_VALID;
  hotkey->KeyState.KeyToggleState|=EFI_TOGGLE_STATE_VALID|EFI_KEY_STATE_EXPOSED;

  Status = gConInEx->RegisterKeyNotify(gConInEx,
                                                hotkey,
                                                HotKeyNotifyFunc,
                                                (VOID **)&hotkeyNotifyHandle);
  Print(L"RegisterKeyNotify=%r\n",Status);

  while(key.Key.ScanCode!=0x17)
  {
    UINTN index;
    gBS->WaitForEvent(1,&(gConInEx->WaitForKeyEx),&index);
    Status = gConInEx->ReadKeyStrokeEx(gConInEx,&key);
    Print(L"key.Key.UnicodeChar=%x\n",key.Key.UnicodeChar);
  }

  Status = gConInEx->UnregisterKeyNotify(gConInEx,hotkeyNotifyHandle);

  return EFI_SUCCESS;
}

EFI_STATUS HotKeyNotifyFunc(IN EFI_KEY_DATA *hotkey)
{
  Print(L"Hot key pressed!\n");
  return EFI_SUCCESS;
}

//====================== Block IO functions begin ==========================

VOID dumpBlockIOMedia(EFI_BLOCK_IO_PROTOCOL* gBlockIo)
{
  Print(L"  MediaID: %x\n",gBlockIo->Media->MediaId);
  if(gBlockIo->Media->RemovableMedia)
    Print(L"  RemovableMedia: True\n");
  else
    Print(L"  RemovableMedia: False\n");

  if(gBlockIo->Media->MediaPresent)
    Print(L"  MediaPresent: True\n");
  else
    Print(L"  MediaPresent: False\n");

  if(gBlockIo->Media->LogicalPartition)
    Print(L"  LogicalPartition: True\n");
  else
    Print(L"  LogicalPartition: False\n");
  
  if(gBlockIo->Media->ReadOnly)
    Print(L"  ReadOnly: True\n");
  else
    Print(L"  ReadOnly: False\n");

  Print(L"  BlockSize: %x\n",gBlockIo->Media->BlockSize);
  Print(L"  LastBloce(LBA): %lx\n",gBlockIo->Media->LastBlock);
  

}
//====================== Block IO functions end ==========================