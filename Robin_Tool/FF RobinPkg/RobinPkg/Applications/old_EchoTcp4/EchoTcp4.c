/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

    Robin's uefi framework application, 2020-3-20
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
//#include <Protocol/SimpleTextInEx.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <wchar.h>
#include "Common.h"
#include "KeyBoard.h"
#include "Network.h"

EFI_STATUS TCP4Test(UINT32 Ip32, UINT16 Port);
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
	UINT64 flag;
	
	flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO | RNG_OUT);
	Print(L"flag=%x\n",flag);
	// WaitKey();
	//text out test

	//Testnetwork
  TCP4Test(MYIPV4(192, 168, 0, 111), 8888);
  return 0;
}

EFI_STATUS TCP4Test(UINT32 Ip32,UINT16 Port)
{
  EFI_STATUS Status = 0;
  UINTN myfd;
  CHAR8 SendStr[] = "Hello,I'm a client of UEFI.";
  CHAR8 *RecvBuffer = (CHAR8*) malloc(1024);
  UINTN recvLen = 0;

  myfd = CreateTCP4Socket();
  Status = ConnectTCP4Socket(myfd, Ip32, Port);
  
  Status = SendTCP4Socket(myfd, SendStr, AsciiStrLen(SendStr));
  Print(L"TCP4Test: SendTCP4Socket, %r\n", Status);
  gBS->Stall(1000);
  Print(L"TCP4Test: Length of SendStr is %d \n", AsciiStrLen(SendStr));
  Status = RecvTCP4Socket(myfd, RecvBuffer, 1024, &recvLen);
  Print(L"TCP4Test: RecvTCP4Socket, %r\n", Status);
 
  Print(L"TCP4Test Recv: %d bytes\n", recvLen);
  // AsciiPrint("Recv raw data:%c %c %c %c \n", RecvBuffer[0],RecvBuffer[1],RecvBuffer[2],RecvBuffer[3]);
  RecvBuffer[recvLen] = '\0';
  Print(L"TCP4Test: Recv data is: %a\n", RecvBuffer);
  Status = CloseTCP4Socket(myfd);
  if(EFI_ERROR(Status))
    gST->ConOut->OutputString(gST->ConOut,L"Close tcp4 error!\n\r");

  free(RecvBuffer);

  return Status;
}