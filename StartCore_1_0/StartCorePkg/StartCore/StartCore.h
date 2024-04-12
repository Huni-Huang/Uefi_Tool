/** @file
  * Primary include file for the StartCore application.
  *
  * Declarations and macros for "public" objects within the StartCore application.
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
  * @par  Include File Dependencies
  *       - <Base.h>
  *       - <ProcessorBind.h>
  *
**/

#ifndef _STARTCORE_H_
#define _STARTCORE_H_

#define NUMLOOPS  10   ///< Number of iterations to run the application

/** GUID which uniquely identifies the StartCore Application.
  *
  * {F7073B95-57DB-4aa1-8750-9EC33C2C8A8F}
  *
**/
#define STARTCORE_GUID \
  { \
    0xf7073b95, 0x57db, 0x4aa1, 0x87, 0x50, 0x9e, 0xc3, 0x3c, 0x2c, 0x8a, 0x8f \
  }

/** Task Control Block.
  *
  * A pointer to an instance of this structure is passed to ClientTask() as its parameter.
  *
  * ProcNum through Thread are only written by the Application code running on the BSP.
  * Result and Ready are used to communicate results using the following synchronization rules.
  *   - The producer may only write into Result and Ready if Ready is zero.
  *   - A consumer will only read from Result if Ready is non-zero.
  *   - The consumer will write zero to a non-zero Ready to indicate that Result has been read.
  *
  * Ready is used to control the data exchange in order to allow Result
  * to contain any possible value as data.  If Result were a pointer, there
  * would be no need for Ready since ZERO (NULL) is never a valid value for
  * a pointer.
**/
typedef struct {
            UINTN             ProcNum;    ///< Zero-based logical number of this processor.
            UINTN             MaxCount;   ///< How high the client task should count.
            UINT32            StatusFlag; ///< Flags indicating if processor is BSP or Enabled
            UINT32            Package;    ///< Zero-based number of physical package containing this CPU
            UINT32            Core;       ///< Zero-based number of physical core within this package
            UINT32            Thread;     ///< Zero-based logical thread number within this core.
  volatile  UINT32            Result;     ///< Communicate result to Root through here.
  volatile  UINT32            Ready;      ///< Flag to indicate Result is ready
} TCB;

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
  *   -# Go back to step 3.
  * -# Done, so Send an "END" value to the Root task then exit.
  *
  * @param[in]  parameter Pointer to the TCB for this task.
  *
  * @pre  Tcb->Ready is initialized to zero to indicate that there is no
  *       data in Result.
**/
VOID
EFIAPI
ClientTask( IN VOID* parameter);

/** Procedure that interacts with the Client task.
  *
  * @param[in]  Tcb   Pointer to the TCB for the Client task.
  *
**/
VOID
EFIAPI
RootTask( IN TCB *Tcb);

#endif  // _STARTCORE_H_
