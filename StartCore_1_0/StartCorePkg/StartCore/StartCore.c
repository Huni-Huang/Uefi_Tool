/** @file
  * Implementation of the main function and some support functions for the StartCore application.
  *
  * ShellAppMain() is the entry point for the StartCore application and is the
  * only public function in this file.
  *
  * Copyright (c) 2009 - 2010, Intel Corporation. All rights reserved.<BR>
  * This program and the accompanying materials are licensed and made available
  * under the terms and conditions of the BSD License which accompanies this
  * distribution.  The full text of the license may be found at
  *   http://opensource.org/licenses/bsd-license.php.
  *
  * THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  * WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
  *
**/

#include <Pi/PiDxeCis.h>
#include <Library/UefiLib.h>  // Needed for Print()
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellCEntryLib.h>

#include "StartCore.h"
#include "Utility.h"

/* ############################################################################
###                                                                         ###
###                Private Function and Data Definitions                    ###
###                                                                         ###
############################################################################ */

STATIC CHAR16   UseStr[]      = L"\n\
Usage Syntax:\n\
  %s: CpuNum\n\n\
  CpuNum is a zero-based decimal number specifying which Core (CPU) to run\n\
  the application on.\n\n\
";

/** Pointer to the Task Status and Control structure for the target processor.
  * For this demo application it only allows a single task on a single processor core,
  * there is no need for anything more complex.
  *
  * For applications using multiple APs, "Tcb" could be an array of TCB structures,
  * one for each logical processor in the platform.
**/
STATIC
TCB *mTcb = NULL;

/** Display a help message describing the usage of the StartCore application.
  *
  * Displays a helpful message showing the proper invocation syntax
  * for this application. This function is local to this file and is only
  * called by GetArgument.
  *
  * @param[in]  MyName  The name of this application; "StartCore".
  *
**/
STATIC
VOID
Usage(
  IN CHAR16 *MyName
  )
{
  Print(UseStr, MyName);
}

/** Parse the application's command line.
 *
 *  The application is passed a single integer argument which
 *  specifies which CPU is to be started.
 *
 *  The CPU number is zero-based.  The BSP is usually CPU 0.
 *  The BSP may not be specified as the target processor.
 *
 *  @param[in]  Argc    The number of tokens on the command line.
 *  @param[in]  Argv    Pointer to a list of pointers to the command line tokens.
 *  @param[out] CpuNum  Pointer to where the extracted argument is to be stored.
 *
 *  @retval EFI_SUCCESS             The function completed successfully.
 *  @retval EFI_INVALID_PARAMETER   No command line argument provided.
 *
**/
STATIC
EFI_STATUS
GetArgument(
  IN      UINTN     Argc,
  IN      CHAR16  **Argv,
     OUT  UINTN    *CpuNum
)
{
  EFI_STATUS    Status  = EFI_INVALID_PARAMETER;
  UINTN         Index;

  if( Argc < 2) {
    Print(L"The target CPU number must be specified.  Try again!\n");
    Usage(Argv[0]);
  }
  else {
    if ( Argc > 2 ) {
      Print(L"%s: invoked with %d parameters\n", Argv[0], Argc);
      Print(L"%d extra arguments ignored.\n", Argc - 2);
      for (Index = 0; Index < Argc; Index++) {
        Print(L"    Argv[%d]: %s\n", Index, Argv[Index]);
      }
    }
    *CpuNum = StrDecimalToUintn(Argv[1]);
    Status = EFI_SUCCESS;
  }
  return Status;
}

/* ############################################################################
###                                                                         ###
###                     Public Function Definitions                         ###
###                                                                         ###
############################################################################ */

/** Main entry point to StartCore.

  Load and run an application on a specified core.

  @param[in]  Argc   Number of arguments in Argv.
  @param[in]  Argv   Array of pointers to the tokenized command line arguments.

  @retval  0         The application exited normally.
  @retval  NonZero   An error occurred.

**/
INTN
EFIAPI
ShellAppMain (
  IN UINTN    Argc,
  IN CHAR16 **Argv
  )
{
  MP_SERVICE_UTILITIES       *MpService;  // Mp Service Utilities pointer
  UINTN                       ProcNum;    // The CPU selected for the demo task
  UINTN                       NumProc;    // Number of logical CPUs in the system
  UINTN                       NumEnabled; // Number of enabled logical CPUs in the system
  EFI_STATUS                  Status;

  /* The do-while loop is used as a poor-man's exception handling capability.
     The 'do-while' block can be viewed as a 'try' block.
     This allows the 'break' statement within the compound block to function as a
     primitive 'throw' -- The enclosing 'try' block is exited.
  */
  do
  {
    Status = UtilityConstructor();   // Create an instance of the Data for the MP Utilities
    if(EFI_ERROR (Status)) {
      Print(L"Failure allocating space for Utility Data: %r\n", Status);
      break;
    }

    // Create an instance of the Task Control Block and initialize it to ZERO
    mTcb = (TCB*)AllocateZeroPool( sizeof(TCB));
    if( mTcb == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      Print(L"Failure allocating space for the Task Control Block: %r\n", Status);
      break;
    }

    // Get command line argument specifying the processor (AP) number.
    Status = GetArgument( Argc, Argv, &ProcNum);
    if(EFI_ERROR (Status)) break;

    // Get MpService pointer and data
    Status = GetMpInfo( mTcb, &MpService, &NumProc, &NumEnabled, ProcNum);
    if(EFI_ERROR (Status)) break;

    // Initialize Task Control Block
    mTcb->ProcNum   = ProcNum;      // Cpu the Client is to run on
    mTcb->MaxCount  = NUMLOOPS;     // Number of loops Client is to perform before returning.
    mTcb->Ready     = 0;            // No results are ready yet

    // Check the validity of the requested target core
    Status = Validate( ProcNum, mTcb);
    if(EFI_ERROR (Status)) break;

    // Start the Application Processors
    Status = StartAPs(mTcb, MpService);
    if(EFI_ERROR (Status)) break;

    RootTask(mTcb);   // Start the part of the application that runs on the Boot Core

    FreePool( mTcb);        // Free the Task Control Block
    UtilityDestructor();    // Free any dynamic resources the Utility Functions consumed

  } while( FALSE );   // Guarantee we only execute the code once.

  return Status;
}
