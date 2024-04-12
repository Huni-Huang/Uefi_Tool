/** @file
  * Demo application to be run on a specified processor core (AP).
  *
  * This application may NOT call into or use
  * UEFI, PI, or other firmware functions or data; other than:
  *   - data specifically passed to the application by way of its parameter.
  *   - Library functions validated as being MP safe
  *   - Protocol member functions validated and specified as being MP safe
  *
  * An "Application" may be significantly larger than this sample, and may
  * consist of many supporting functions.  Think of the "Application" function
  * as if it were "main" in a normal application, except that it is for another processor.
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

#include <Library/BaseLib.h>      // BaseLib is MP safe
#include "StartCore.h"

/** Entry point function for the AP portion of the StartCore Application.
  *
  * This is the portion of the StartCore application that runs on one of the
  * Application Processors (APs).  It is extremely simple so that it is easier
  * to see what is happening.
  *
  * The values 0 through MaxCount are generated followed by a special "END" value.
  * Thus, there are MaxCount + 2 values generated.
  *
  * -# The ClientTask reports the initial value of Counter, 0.
  * -# ClientTask signals that data is ready by writing ONE to Ready
  * -# As long as ClientTask has work to do:
  *   -# Do our work: increment Counter
  *   -# Wait until the Root task is ready to receive our result: Ready == 0
  *   -# Send the result to the Root task: Result=Counter, Ready=1
  * -# Done, so Send an "END" value to the Root task then exit.
  *
  * @param[in]  parameter Pointer to the TCB for this task.
  *
  * @pre  Tcb->Ready is initialized to zero to indicate that there is no
  *       data in Result.
**/
VOID
EFIAPI
ClientTask( IN VOID* parameter)
{
  TCB   *Tcb      = (TCB*)parameter;  // Convert "parameter" to the correct type
  UINTN  LoopCount;                   // Number of loops to execute
  UINT32 Counter  = 0;                // This is what I am working on

  Tcb->Result = Counter;      // Report the initial value of Counter.
  Tcb->Ready = 1;            // Indicate that there is something in Result.

  // Do this the "long" way, instead of using a for loop,
  // because we don't want to be efficient.
  LoopCount = Tcb->MaxCount;  // How high to count.

  // Count from 1 to LoopCount
  do {
    ++Counter;                  // Do my work
    while(Tcb->Ready != 0) {   // Wait until the Root task is ready for more data
      CpuPause();                 // Hint to CPU that this is a spin loop
    }
    Tcb->Result = Counter;      // Report my results
    Tcb->Ready = 1;            // Indicate that there is something in Result.
  } while(--LoopCount);       // Do our work LoopCount times
  // We have now done all of our work and could exit right now.

  // For debugging and paranoia's sake, Send one last "special" value.
  while(Tcb->Ready != 0) { // Wait until the Root task reads the last result.
    CpuPause();                 // Hint to CPU that this is a spin loop
  }
  // Send one more result so we can catch off-by-one errors in the Root Task
  Tcb->Result = 0xFEEDFACE;     // Indicate that ClientTask is exiting
  Tcb->Ready = 1;              // This should be ignored

  // When the ClientTask exits, the firmware's MP infrastructure will mark
  // this processor as IDLE and place it in a quiescent state so that it can
  // be used again at a later time.
}
