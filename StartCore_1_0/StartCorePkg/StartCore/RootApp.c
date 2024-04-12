/*++ @file
  * Routine for interacting with the Demo application.
  *
  * This is the routine for interacting with the Demo application and
  * verifying that its behavior is as expected.
  *
  * The accompaning file, ClientApp1.c, contains the application, ClientTask(), which is to
  * be run on the target processor.
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
--*/

#include <Library/UefiLib.h>
#include "StartCore.h"

/** Procedure that interacts with the Client task.
  *
  * @param[in]  Tcb   Pointer to the TCB for the Client task.
  *
**/
VOID
EFIAPI
RootTask( IN TCB *Tcb)
{
  INTN  count;
  INTN  waitLimit = 1000;

  Print(L" #   Cpu# Max# Data Gate\n");
  Print(L"---: ---- ---- ---- ----\n");
  // The first line of data printed should be:  "-1: 1 100 0 1" if ProcNum=1 and NUMLOOPS=100

  // Retrieve and Display NUMLOOPS values from the Client Task
  for(count = NUMLOOPS; count > 0; --count) {
    // Wait until the ClientTask signals data is ready, or I time out
    waitLimit = 1000;   // This is how long I am willing to wait
    while( (Tcb->Ready == 0) && (waitLimit > 0)) {
      --waitLimit;
      CpuPause();   // From BaseLib, provides hint to CPU that we are in a spin-loop
    }
    if( waitLimit <= 0 ) {
      // Timeout waiting for Client to provide the next value
      Print(L"%3d: Client task did not respond in time!\n", count);
    }
    else {
      // Display what we received from the Client
      Print(L"%3d: %4d %4d %4d %4d\n", NUMLOOPS-count, Tcb->ProcNum, Tcb->MaxCount, Tcb->Result, Tcb->Ready);
      Tcb->Ready = 0;           // Tell the Client Task she can run
    }
  }
  // Give the ClientTask a chance to signal she is done.
  waitLimit = 1000;   // This is how long I am willing to wait
  while( (Tcb->Ready == 0) && (waitLimit > 0)) {
    --waitLimit;
    CpuPause();   // From BaseLib, provides hint to CPU that we are in a spin-loop
  }
  // Print the final state of the Tcb.
  Print(L"END: %4d %4d %4d %4d\n", Tcb->ProcNum, Tcb->MaxCount, Tcb->Result, Tcb->Ready);
}
