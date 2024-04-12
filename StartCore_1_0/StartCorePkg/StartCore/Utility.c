/** @file
  * Public MP Utility functions used by the StartCore application.
  *
  * All MP specific functions are abstracted here so that differences between
  * firmware implementations can be easily accommodated -- such as PI vs. Framework,
  * or differing processor architectures.
  *
  * This file accommodates both the PI and Framework implementations of the
  * MP Services Protocol.
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

#include <Pi/PiDxeCis.h>      // Needed for the definition of EFI_AP_PROCEDURE
#include <Library/UefiLib.h>  // Needed for Print()
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/MpService.h>
#include <Protocol/FrameworkMpService.h>

#include "StartCore.h"
#include "Utility.h"

/* ############################################################################
###                                                                         ###
###                Private Function and Data Definitions                    ###
###                                                                         ###
############################################################################ */

/// A private structure used by the MP Utilities to encapsulate their common data.
typedef struct {
  EFI_MP_SERVICES_PROTOCOL               *PiMpService;        ///< PI version of MpService, or NULL.
  FRAMEWORK_EFI_MP_SERVICES_PROTOCOL     *FrameworkMpService; ///< Framework version of MpService, or NULL.
  EFI_EVENT                               Event;    ///< Event signaled by StartupThisAp() when AP exits.
  UINTN                                   BspId;    ///< Logical number of the BootStrap Processor (core)

  /** Buffer used for both variants of GetProcessorInfo().
    *
    * Both the PI function GetProcessorInfo() and the Framework function GetProcessorContext()
    * return their information in a buffer specified by the caller.  Several Framework
    * implementations return a buffer that is larger than sizeof(EFI_MP_PROC_CONTEXT), which is
    * the size specified for the buffer in the FrameworkMpService.h header file.  By using a large,
    * fixed size, buffer, we always have an acceptable destination for these functions,
    * regardless of implementation differences.
  **/
  UINT8       BigBuffer[100];

} UTILITY_DATA;

STATIC
UTILITY_DATA   *mUtilityData = NULL;   ///< Pointer to private data for use in this file only.

/* ############################################################################
                  PI Private Utility Function Definitions
############################################################################ */

/** Determine the number of processors present in the system using the
  * PI version of the MpService protocol.
  *
  * This function returns the total number of processors present in the platform
  * and the number of processors enabled.  These are logical processors which include individual cores
  * and threads within each physical processor package.
  *
  * Because the MP Service Protocol provides services to enable and disable processors
  * dynamically, the number of enabled logical processors may vary during the
  * course of a boot session.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]    This          Pointer to the active instance of the Utility Functions.
  * @param[out]   NumProc       Pointer to the variable to receive the total number of processors.
  * @param[out]   NumEnabled    Pointer to the variable to receive the number of enabled processors.
  *
  * @retval EFI_SUCCESS             The function completed successfully.
  * @retval EFI_DEVICE_ERROR        This function was not called by the BSP.
  * @retval EFI_INVALID_PARAMETER   NumProc is NULL.
  * @retval EFI_INVALID_PARAMETER   NumEnabled is NULL.
  *
**/
STATIC
EFI_STATUS
PiCountProcessors (
  IN      MP_SERVICE_UTILITIES  *This,
      OUT UINTN                 *NumProc,
      OUT UINTN                 *NumEnabled
  )
{
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL *MpService;

  MpService = mUtilityData->PiMpService;

  // Get the total number and number of enabled processors
  Status = MpService->GetNumberOfProcessors( MpService, NumProc, NumEnabled);

  return Status;
}

/** Gets status and location information for the requested processor at the
  * instant this call is made.
  *
  * Populate the Tcb with a specified processor's status and location information.
  * This function uses the PI version of the MpService protocol to retrieve
  * status and location information about a specific processor.
  * Note the following:
  *   - The processor information may change during the course of a boot session.
  *   - The information presented here is specific to the specified processor.
  *   - This function may only be called from the BSP.
  *
  * The raw information retrieved from the MpService protocol is displayed for
  * informational purposes.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]  This                Pointer to the active instance of the Utility Functions.
  * @param[in]  ProcessorNumber     The handle number of processor.
  * @param[out] Tcb                 A pointer to the Task Control Block in which to
  *                                 place the retrieved information
  *
  * @retval EFI_SUCCESS             Processor information was returned.
  * @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  * @retval EFI_NOT_FOUND           The processor specified by ProcessorNumber does not exist.
  *
**/
STATIC
EFI_STATUS
PiGetProcessorInfo (
  IN      MP_SERVICE_UTILITIES     *This,
  IN      UINTN                     ProcessorNumber,
      OUT TCB                      *Tcb
  )
{
  EFI_MP_SERVICES_PROTOCOL     *MpService;
  EFI_PROCESSOR_INFORMATION    *ProcessorInfoBuffer;
  EFI_STATUS                    Status;

  MpService = mUtilityData->PiMpService;
  ProcessorInfoBuffer = (EFI_PROCESSOR_INFORMATION*)mUtilityData->BigBuffer;

  // Retrieve detailed health, status, and location info about the target processor
  Status = MpService->GetProcessorInfo( MpService, ProcessorNumber, ProcessorInfoBuffer);
  if( Status == EFI_SUCCESS)
  {
    Tcb->Package    = ProcessorInfoBuffer->Location.Package;
    Tcb->Core       = ProcessorInfoBuffer->Location.Core;
    Tcb->Thread     = ProcessorInfoBuffer->Location.Thread;
    Tcb->StatusFlag = ProcessorInfoBuffer->StatusFlag;
  }

  // Display Processor Info Buffer
  Print(L" ProcID   Enabled   Type   Status   Pkg   Core  Thread\n");
  Print(L"--------  --------  ----  -------- -----  ----  ------\n");
  Print(L"%08Lx      %s      %3s  %08x  %4d    %2d      %d\n\n",
      ProcessorInfoBuffer->ProcessorId,
      (ProcessorInfoBuffer->StatusFlag & PROCESSOR_ENABLED_BIT) ? L"Y"   : L"N",
      (ProcessorInfoBuffer->StatusFlag & PROCESSOR_AS_BSP_BIT)  ? L"BSP" : L" AP",
      ProcessorInfoBuffer->StatusFlag,
      ProcessorInfoBuffer->Location.Package,
      ProcessorInfoBuffer->Location.Core,
      ProcessorInfoBuffer->Location.Thread);

  return Status;
}

/** Execute a caller-provided function on a specified processor in non-blocking mode using the
  * PI version of the MpService protocol.
  *
  * This function is used to dispatch one enabled AP to the function specified by
  * Procedure passing in the argument specified by ProcedureArgument.  It may only
  * be called by the BSP.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]  This                    Pointer to the active instance of the Utility Functions.
  * @param[in]  Procedure               A pointer to the function to be run on the
  *                                     specified AP. The function is of type EFI_AP_PROCEDURE.
  * @param[in]  ProcessorNumber         The number of the AP to be started. The range is
  *                                     from 0 to the total number of logical
  *                                     processors minus 1.
  * @param[in]  ProcedureArgument       The parameter passed into Procedure.
  *
  * @retval EFI_SUCCESS             The function has been dispatched to the specified AP.
  * @retval EFI_UNSUPPORTED         This function was called after the
  *                                 UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was
  *                                 signaled.  { Errata approved to change this to after ExitBootServices. }
  * @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  * @retval EFI_NOT_READY           The specified AP is busy or was not terminated properly.
  * @retval EFI_NOT_FOUND           The processor specified by ProcessorNumber does not exist.
  * @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP or a disabled AP.
  * @retval EFI_INVALID_PARAMETER   Procedure is NULL.
  *
**/
STATIC
EFI_STATUS
PiStartThisAP (
  IN  MP_SERVICE_UTILITIES     *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  VOID                      *ProcedureArgument
  )
{
  EFI_STATUS                  Status;
  EFI_MP_SERVICES_PROTOCOL   *MpService;

  MpService = mUtilityData->PiMpService;

  // Start our Application on the specified CPU.
  // Successful AP startup should return EFI_SUCCESS.
  Status = MpService->StartupThisAP(  MpService, Procedure, ProcessorNumber, mUtilityData->Event,
                                      PI_START_AP_TIMEOUT, ProcedureArgument, NULL);
  Print(L"Starting Application on CPU %d: %r\n", ProcessorNumber, Status);

  return Status;
}

/** The PI instance of the MP Utilities function container.
  *
  * Functions within this instance of the MP_SERVICE_UTILITIES structure implement
  * the PI versions of the:
  *   - GetNumberOfProcessors
  *   - GetProcessorInfo
  *   - StartThisAP
  * utility functions.
**/
STATIC
MP_SERVICE_UTILITIES  PiMpUtilities = {
  PiCountProcessors,
  PiGetProcessorInfo,
  PiStartThisAP
};

/* ############################################################################
              Framework Private Utility Function Definitions
############################################################################ */

/** Determine the number of processors present in the system using the
  * Framework version of the MpService protocol.
  *
  * This function returns the total number of processors present in the platform
  * and the number of processors enabled.  These are logical processors which include individual cores
  * and threads within each physical processor package.
  *
  * Because the MP Service Protocol provides services to enable and disable processors
  * dynamically, the number of enabled logical processors may vary during the
  * course of a boot session.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]    This          Pointer to the active instance of the Utility Functions.
  * @param[out]   NumProc       Pointer to the variable to receive the total number of processors.
  * @param[out]   NumEnabled    Pointer to the variable to receive the number of enabled processors.
  *
  * @retval EFI_SUCCESS         The function completed successfully.
  * @retval other               An error occurred.
  *
**/
STATIC
EFI_STATUS
FrameworkCountProcessors (
  IN      MP_SERVICE_UTILITIES  *This,
      OUT UINTN                 *NumProc,
      OUT UINTN                 *NumEnabled
  )
{
  EFI_STATUS                          Status;
  FRAMEWORK_EFI_MP_SERVICES_PROTOCOL *MpService;

  MpService = mUtilityData->FrameworkMpService;

  Status = MpService->GetGeneralMPInfo( MpService, NumProc, NULL, NumEnabled, NULL, NULL);

  return Status;
}

/** Gets status and location information for the requested processor at the
  * instant this call is made.
  *
  * This function uses the Framework version of the MpService protocol to retrieve
  * status and location information about a specific processor.
  * Note the following:
  *   - The processor information may change during the course of a boot session.
  *   - The information presented here is specific to the specified processor.
  *   - This function may only be called from the BSP.
  *
  * Due to differences in Framework implementations of the GetProcessorContext() function,
  * this function may, or may not, return valid data.  Know what the target platform returns
  * before relying upon information from this function.
  *
  * The raw information retrieved from the MpService protocol is displayed for
  * informational purposes.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]  This                  Pointer to the active instance of the Utility Functions.
  * @param[in]  ProcessorNumber       The handle number of processor.
  * @param[out] Tcb                   A pointer to the Task Control Block in which to
  *                                   place the retrieved information
  *
  * @retval EFI_SUCCESS             Processor information was returned.
  * @retval EFI_INVALID_PARAMETER   The processor specified by ProcessorNumber does not exist.
  *
**/
STATIC
EFI_STATUS
FrameworkGetProcessorInfo (
  IN      MP_SERVICE_UTILITIES     *This,
  IN      UINTN                     ProcessorNumber,
      OUT TCB                      *Tcb
  )
{
  FRAMEWORK_EFI_MP_SERVICES_PROTOCOL *MpService;
  EFI_MP_PROC_CONTEXT                *ProcessorContextBuffer;
  UINTN                               BufferSize;
  EFI_STATUS                          Status;

  MpService = mUtilityData->FrameworkMpService;

  ProcessorContextBuffer = (EFI_MP_PROC_CONTEXT*)mUtilityData->BigBuffer;
  BufferSize = sizeof(mUtilityData->BigBuffer);

  Status = MpService->GetProcessorContext (
                        MpService,
                        ProcessorNumber,
                        &BufferSize,
                        ProcessorContextBuffer
                        );
  if( Status == EFI_SUCCESS)
  {
    // Fill in processor information in the TCB
    Tcb->Package = (UINT32) ProcessorContextBuffer->PackageNumber;
    Tcb->Core    = (UINT32) ProcessorContextBuffer->NumberOfCores;
    Tcb->Thread  = (UINT32) ProcessorContextBuffer->NumberOfThreads;

    Tcb->StatusFlag = 0;
    if (ProcessorContextBuffer->Enabled) {
      Tcb->StatusFlag |= PROCESSOR_ENABLED_BIT;
    }
    if (ProcessorContextBuffer->Designation == EfiCpuBSP) {
      Tcb->StatusFlag |= PROCESSOR_AS_BSP_BIT;
    }
    if (ProcessorContextBuffer->Health.Flags.Uint32 == 0) {
      Tcb->StatusFlag |= PROCESSOR_HEALTH_STATUS_BIT;
    }
  }

  // Display Context Buffer
  Print(L" ApicID   Enabled   Type   Health   Pkg   Core  Thread\n");
  Print(L"--------  --------  ----  -------- -----  ----  ------\n");
  Print(L"%08x      %s      %3s  %08x  %4d    %2d      %d\n\n",
      ProcessorContextBuffer->ApicID,
      (ProcessorContextBuffer->Enabled) ? L"Y" : L"N",
      (ProcessorContextBuffer->Designation == EfiCpuBSP) ? L"BSP" : L" AP",
      ProcessorContextBuffer->Health.Flags.Uint32,
      ProcessorContextBuffer->PackageNumber,
      ProcessorContextBuffer->NumberOfCores,
      ProcessorContextBuffer->NumberOfThreads);

  return Status;
}

/** Execute a caller-provided function on a specified processor in non-blocking mode using the
  * Framework version of the MpService protocol.
  *
  * This function is used to dispatch one enabled AP to the function specified by
  * Procedure passing in the argument specified by ProcedureArgument.  It may only
  * be called by the BSP.
  *
  * Both the PI and Framework versions of this function have identical parameters and return type.
  * Returned data is presented identically by both versions.  This enables the functions to be called
  * without the caller having to know whether the underlying implementation is PI or Framework.
  *
  * @param[in]  This                    Pointer to the active instance of the Utility Functions.
  * @param[in]  Procedure               A pointer to the function to be run on the
  *                                     specified AP. The function is of type EFI_AP_PROCEDURE.
  * @param[in]  ProcessorNumber         The number of the AP to be started. The range is
  *                                     from 0 to the total number of logical
  *                                     processors minus 1.
  * @param[in]  ProcedureArgument       The parameter passed into Procedure.
  *
  * @retval EFI_SUCCESS             The function has been dispatched to the specified AP.
  * @retval EFI_UNSUPPORTED         This function was called after the
  *                                 UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was
  *                                 signaled.  { Errata approved to change this to after ExitBootServices. }
  * @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  * @retval EFI_NOT_READY           The specified AP is busy or was not terminated properly.
  * @retval EFI_NOT_FOUND           The processor specified by ProcessorNumber does not exist.
  * @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP or a disabled AP.
  * @retval EFI_INVALID_PARAMETER   Procedure is NULL.
  *
**/
STATIC
EFI_STATUS
FrameworkStartThisAP (
  IN  MP_SERVICE_UTILITIES     *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  VOID                      *ProcedureArgument
  )
{
  EFI_STATUS                          Status;
  FRAMEWORK_EFI_MP_SERVICES_PROTOCOL *MpService;

  MpService = mUtilityData->FrameworkMpService;

  // Start our Application on the specified CPU.
  // We have specified a very short timeout and don't care if the Event is signaled
  // so successful AP startup should return EFI_TIMEOUT.
  Status = MpService->StartupThisAP(  MpService, Procedure, ProcessorNumber, mUtilityData->Event,
                                      FRAMEWORK_START_AP_TIMEOUT, ProcedureArgument);
  Print(L"Start Application on CPU %d: %r\n", ProcessorNumber, Status);
  if(Status == EFI_TIMEOUT) {
    Status = EFI_SUCCESS;   // Timeout indicates successful AP startup.
  }

  return Status;
}

/** The Framework instance of the MP Utilities function container.
  *
  * Functions within this instance of the MP_SERVICE_UTILITIES structure implement
  * the Framework versions of the:
  *   - GetNumberOfProcessors
  *   - GetProcessorInfo
  *   - StartThisAP
  * utility functions.
**/
STATIC
MP_SERVICE_UTILITIES  FrameworkMpUtilities = {
  FrameworkCountProcessors,
  FrameworkGetProcessorInfo,
  FrameworkStartThisAP
};

/* ############################################################################
###                                                                         ###
###                     Public Function Definitions                         ###
###                                                                         ###
############################################################################ */

/* ############################################################################
            Constructor and Destructor functions for this file.
############################################################################ */


/** Initialize and allocate Utility Data prior to calling any other Utility Function.
  *
  * @retval EFI_SUCCESS           Data was successfully allocated and initialized.
  * @retval EFI_OUT_OF_RESOURCES  Unable to allocate data space.
**/
EFI_STATUS
EFIAPI
UtilityConstructor ( VOID )
{
  EFI_STATUS      Status=EFI_SUCCESS;

  // Create an instance of the private data structure and initialize it to ZERO
  mUtilityData = (UTILITY_DATA*)AllocateZeroPool( sizeof(UTILITY_DATA));
  if( mUtilityData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  return Status;
}

/** Free data space and resources dynamically allocated by the Utility functions.
**/
VOID
EFIAPI
UtilityDestructor ( VOID )
{
  if( mUtilityData != NULL) {   // Only execute if dereferencing mUtilityData is safe!
    // If an event has been created, free it
    if( mUtilityData->Event != NULL) {
      (VOID) gBS->CloseEvent( mUtilityData->Event);
    }
    FreePool( mUtilityData);  // Free the local data structure
    mUtilityData = NULL;      // Don't point to freed data
  }
}

/* ############################################################################
            Public Utility Functions
############################################################################ */

/** Verify that ProcNum specifies a valid, enabled, processor.
 *
 *  - ProcNum must NOT equal BspId
 *  - ProcNum must be enabled
 *
 *  @param[in]  ProcNum   Target CPU number.
 *  @param[in]  Tcb     Pointer to the TCB for the target CPU.
 *
 *  @retval EFI_SUCCESS             The function completed successfully.
 *  @retval EFI_INVALID_PARAMETER   The target CPU does not meet the required criteria.
 *
**/
EFI_STATUS
EFIAPI
Validate(
  IN UINTN   ProcNum,
  IN TCB    *Tcb
)
{
  EFI_STATUS      Status=EFI_SUCCESS;
  UINT32          TempVal;

  Print(L"Processor %Ld is the target processor, %d is the BSP.\n", Tcb->ProcNum, mUtilityData->BspId);

  TempVal = (Tcb->StatusFlag & (  PROCESSOR_AS_BSP_BIT
                                | PROCESSOR_ENABLED_BIT ));
  Print(L"Processor's reported status is 0x%x, indicating:\n", TempVal);
  Print(L"  processor is%sthe BSP.\n", ( TempVal & PROCESSOR_AS_BSP_BIT ) ? L" " : L" not ");
  Print(L"  processor is%senabled\n", ( TempVal & PROCESSOR_ENABLED_BIT ) ? L" " : L" not ");

  if( ProcNum == mUtilityData->BspId) {
    Status = EFI_INVALID_PARAMETER;
    Print(L"Specified target processor %d is the BSP and may NOT be used.\n", ProcNum);
  }
  else {
    Print(L"Processor core %d meets the criteria for running the Client task.\n\n", ProcNum);
  }
  return Status;
}

/** Find the MpService protocol and get the Number of CPUs in the system.
 *
 *  Find the MpService protocol and get the Number of CPUs in the system.
 *  The TCB is populated with information about the target processor.
 *  Also, assume the CPU we are running on right now is the BSP.
 *
 *  Does some basic validation of the target ProcNum:
 *  - ProcNum must be < NumProc
 *  - NumEnabled must be >= 2
 *
 *  @param[in,out]  Tcb           Pointer to the TCB for the Client task.
 *  @param[out]     MpService     Pointer to the location to store the pointer to the MP Services Protocol.
 *  @param[out]     NumProc       Pointer to the location to store the Number of Processors into.
 *  @param[out]     NumEnabled    Pointer to variable to receive the Number of Enabled processors.
 *  @param[in]      ProcNum       Identifying number of the CPU to retrieve information about.
 *
 *  @retval EFI_SUCCESS         The function completed successfully.
 *  @retval EFI_DEVICE_ERROR    This function was not called by the BSP.
 *  @retval EFI_DEVICE_ERROR    Less than two processors are enabled.
 *  @retval EFI_NOT_FOUND       The MP Services Protocol was not found.
 *  @retval EFI_NOT_FOUND       There is no processor number ProcNum.
 *
**/
EFI_STATUS
EFIAPI
GetMpInfo(
      OUT  TCB                       *Tcb,
      OUT MP_SERVICE_UTILITIES      **MpService,
      OUT UINTN                      *NumProc,
      OUT UINTN                      *NumEnabled,
  IN      UINTN                       ProcNum
)
{
  MP_SERVICE_UTILITIES     *MpUtilities;
  CHAR16                   *MpId;
  EFI_STATUS                Status;

  do
  {
    // Find the PI MpService protocol
    MpId = L"PI";
    Status = gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL, &mUtilityData->PiMpService);
    if (EFI_ERROR(Status)) {
      Print(L"Unable to locate the PI MpService procotol: %r\n", Status);

      // Find the Framework MpService protocol
      MpId = L"Framework";
      Status = gBS->LocateProtocol(&gFrameworkEfiMpServiceProtocolGuid, NULL, &mUtilityData->FrameworkMpService);
      if (EFI_ERROR(Status)) {
        Print(L"Unable to locate the %s MpServices procotol: %r\n", MpId, Status);
        break;
      }
      MpUtilities = &FrameworkMpUtilities;  // Save pointer to the Framework MP Utilities
      // Framework: I am running on the BSP.  Find out my logical processor number.
      Status = mUtilityData->FrameworkMpService->WhoAmI( mUtilityData->FrameworkMpService, &mUtilityData->BspId);
      if (EFI_ERROR(Status)) {
        Print(L"%s WhoAmI failed: %r\n", MpId, Status);
        break;
      }
    }
    else {                                // OR
      MpUtilities = &PiMpUtilities;         // Save pointer to the PI MP Utilities
      // PI: I am running on the BSP.  Find out my logical processor number.
      Status = mUtilityData->PiMpService->WhoAmI( mUtilityData->PiMpService, &mUtilityData->BspId);
      if (EFI_ERROR(Status)) {
        Print(L"%s WhoAmI failed: %r\n", MpId, Status);
        break;
      }
    }
    *MpService = MpUtilities;               // Point the caller to the MP Utilities
    Print(L"Successfully located the %s MpService protocol.\n", MpId);

    // Get the total number and number of enabled processors
    Status = MpUtilities->GetNumberOfProcessors( MpUtilities, NumProc, NumEnabled);
    if (EFI_ERROR(Status)) {
      Print(L"Unable to get the number of processors: %r\n", Status);
      break;
    }

    Print(L"This platform has %d logical processors of which %d are enabled.\n", *NumProc, *NumEnabled);
    if( *NumEnabled < 2 ) {
      Print(L"Two or more enabled processors are required to run this application.\n\n");
      Status = EFI_DEVICE_ERROR;
      break;
    }
    if ( *NumProc <= ProcNum ) {
      Print(L"Requested target processor %d is out of range.\n", ProcNum);
      Status = EFI_NOT_FOUND;
      break;
    }

    // Retrieve detailed health, status, and location info about the target processor
    Status = MpUtilities->GetProcessorInfo( MpUtilities, ProcNum, Tcb);
    if (EFI_ERROR(Status)) {
      Print(L"Unable to get information for processor %d: %r\n", ProcNum, Status);
      break;
    }
  } while( FALSE );

  return Status;
}

/** Start the ClientTask on the target processor.
 *
 *  Creates an Event which signals when the target processor completes
 *  its task.  This is required in order to start the target in non-blocking
 *  mode.  The ClientTask is then started on the target processor as specified by ProcNum.
 *  The pointer to the task's TCB is passed to the ClientTask as its only parameter.
 *
 *  @param[in]  Tcb
 *  @param[in]  MpService
 *
 *  @retval EFI_SUCCESS           The Client Task was successfully started.
 *  @retval EFI_OUT_OF_RESOURCES  The Event could not be created.
 *  @retval EFI_DEVICE_ERROR      This function was not called by the BSP.
 *  @retval EFI_NOT_READY         The target processor is busy.
 *  @retval EFI_NOT_FOUND         The target processor specified in Tcb does not exist.
 *  @retval EFI_INVALID_PARAMETER The specified target processor is disabled or is the BSP.
 *
**/
EFI_STATUS
EFIAPI
StartAPs(
  IN TCB                  *Tcb,
  IN MP_SERVICE_UTILITIES *MpService
  )
{
  EFI_STATUS    Status;
  UINTN         ProcNum;

  // Package, Core, Thread, StatusFlag, and ProcNum; in the TCB, have already been filled in.
  ProcNum = Tcb->ProcNum;           // Which AP to run on

  // Create an Event, which is required to call StartupThisAP in non-blocking mode
  Status = gBS->CreateEvent( 0, TPL_NOTIFY, NULL, NULL, &mUtilityData->Event);
  if(Status == EFI_SUCCESS)
  {
    Print(L"Successfull Event creation.\n");
    // Start our Application on the specified CPU.
    Status = MpService->StartThisAP(  MpService, ClientTask, ProcNum, Tcb);
    if(Status == EFI_SUCCESS) {
      Print(L"Application successfully started.\n");
    }
    else Print(L"Failed to start Application on CPU %d: %r\n", ProcNum, Status);
  }
  else {
    Print(L"Event creation failed: %r\n", Status);
  }
  return Status;
}
