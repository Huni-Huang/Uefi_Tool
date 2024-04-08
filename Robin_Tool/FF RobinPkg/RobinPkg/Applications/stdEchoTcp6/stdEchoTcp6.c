#pragma warning( disable : 4706) 
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
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>

#include <sys/EfiSysCall.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h> //inet_pton
#include <stdio.h>

#include "Common.h"
#include "KeyBoard.h"
#include "Network.h"

#define MAXBUF 1024
EFI_STATUS tcp6Client(IN int Argc,
                      IN char **Argv);
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
  tcp6Client(Argc, Argv);
  return 0;
}


EFI_STATUS tcp6Client(IN int Argc,
  IN char **Argv)
{
  EFI_STATUS Status = EFI_SUCCESS;
  // UINT32 ServerIPAddr[4];
  // UINT16 ServerPort;
  struct sockaddr_in6 ServerIp;
  int sockfd,recvLen;
  // int RetVal;
  // char msgStr[1024],recvStr[1024];
  char buffer[MAXBUF + 1];

  //1 get the server ip and port
  if(Argc != 3)
  {
    printf("TCP6 Client.Usage: %s <IP> <port>", Argv[1], atoi(Argv[2]));
    Status = EFI_ABORTED;
    return Status;
  }
  else
  {
    printf("Ready to start TCP6 Client. IP=%s,port=%d\n", Argv[1], atoi(Argv[2]));
  }
  // Print(L"IP: %d.%d.%d.%d, port: %d \n", ServerIPAddr[0], ServerIPAddr[1], ServerIPAddr[2], ServerIPAddr[3], ServerPort);
  // WaitKey();
  //2 create a socket
  if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {      // IPv6
      printf("Socket()");
      Status = EFI_ABORTED;
      return Status;
  }
  //3 connet the server
  memset ( &ServerIp, 0, sizeof ( ServerIp ));
  ServerIp.sin6_family = AF_INET6;
  ServerIp.sin6_port = htons(atoi(Argv[2]));

  if ( inet_pton(AF_INET6, Argv[1], &ServerIp.sin6_addr) < 0 ) {                 // IPv6
      printf(Argv[1]);
      Status = EFI_ABORTED;
      return Status;
    }
  //3 connet the server
   if (connect(sockfd, (struct sockaddr *) &ServerIp, sizeof(ServerIp)) != 0) {
      printf("Connect() ");
      Status = EFI_ABORTED;
      return Status;
  }
  
  //4 send message to server and get message from server
  while(1)
  {
    printf("Input message(Q to quit):");
	  gets(buffer); //abandon the Enter key
    if (!strcmp(buffer, "q") || !strcmp(buffer, "Q"))
		  break;

    recvLen = send(sockfd, buffer, strlen(buffer), 0);
    if (recvLen < 0)
    {
      printf("Send message to server fail! error code=%d: %s\n",errno,strerror(errno));
      break;
    }
    recvLen = recv(sockfd, buffer, MAXBUF, 0);
    if (recvLen < 0)
    {
      printf("Recv message from server fail! error code=%d: %s\n",errno,strerror(errno));
      break;
    }
    buffer[recvLen]='\0';
    printf("Message from server:%s\n", buffer);
  }
  
  close(sockfd);  
  return Status;
}
