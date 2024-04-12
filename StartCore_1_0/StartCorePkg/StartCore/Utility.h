/** @file
  * Declarations of public MP utility functions used by the StartCore application.
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
#ifndef _STARTCORE_UTILITY_H_
#define _STARTCORE_UTILITY_H_

#define PI_START_AP_TIMEOUT         10000000  ///< 10 seconds
#define FRAMEWORK_START_AP_TIMEOUT         1  ///<  1 microsecond

/// Type specification for the structure containing pointers to the MP Utility functions.
typedef struct _MP_SERVICE_UTILITIES_  MP_SERVICE_UTILITIES;

/// Instantiate dynamic data for use by the MP Utility functions.
EFI_STATUS
EFIAPI
UtilityConstructor ( VOID );

/// Free dynamically created resources
VOID
EFIAPI
UtilityDestructor ( VOID );

/** Type declaration for the GetNumberOfProcessors utility function.
  *
  * The GetNumberOfProcessors function returns the total number of processors present in the platform
  * and the number of processors enabled.  These are logical processors which include individual cores
  * and threads within each physical processor package.
  *
  * Differences between the PI and Framework implementations of the MpServices protocol are
  * hidden by this function so that a single function call can be used without regard to the
  * underlying implementation.
  *
  * @param[in]    This                        The instance of the MP Utilities being used.
  * @param[out]   NumberOfProcessors          Pointer to the variable in which to store the total
  *                                           number of processors.
  * @param[out]   NumberOfEnabledProcessors   Pointer to the variable in which to store the number of
  *                                           enabled processors.
  *
  * @retval EFI_SUCCESS             The number of logical processors and enabled
                                    logical processors was retrieved.
  * @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  * @retval EFI_INVALID_PARAMETER   NumberOfProcessors is NULL.
  * @retval EFI_INVALID_PARAMETER   NumberOfEnabledProcessors is NULL.
  *
**/
typedef
EFI_STATUS
(EFIAPI *MP_COUNT_PROCESSORS)(
  IN  MP_SERVICE_UTILITIES  *This,
  OUT UINTN                 *NumberOfProcessors,
  OUT UINTN                 *NumberOfEnabledProcessors
);

/** Type declaration for the GetProcessorInfo utility function.
  *
  * Populates the Tcb with status and location information for the specified processor.
  *
  * Differences between the PI and Framework implementations of the MpServices protocol are
  * hidden by this function so that a single function call can be used without regard to the
  * underlying implementation.
  *
  * Due to differences in Framework implementations of the GetProcessorContext() function,
  * this function may, or may not, return valid data.  Know what the target platform returns
  * before relying upon information from this function.
  *
  * @param[in]    This                Pointer to the instance of the MP Utilities being used.
  * @param[in]    ProcessorNumber     Zero-based number of the processor to get information for.
  * @param[out]   Tcb                 Pointer to the TCB in which to deposit the retrieved information.
  *
  @retval EFI_SUCCESS             Processor information was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL    The underlying Framework implementation requires a buffer larger
                                  than 100 bytes.  The size of BigBuffer should be increased.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_INVALID_PARAMETER   Framework: The logical Processor specified by
                                  ProcessorNumber does not exist.
  @retval EFI_NOT_FOUND           The logical Processor specified by
                                  ProcessorNumber does not exist.
  *
**/
typedef
EFI_STATUS
(EFIAPI *MP_GET_PROCESSOR_INFO)(
  IN  MP_SERVICE_UTILITIES   *This,
  IN  UINTN                   ProcessorNumber,
  OUT TCB                    *Tcb
);

/** Type declaration for the StartThisAP() utility function.
  *
  * The StartThisAP() function executes a caller-provided function on one specified logical processor.
  * The specified processor will execute the caller-provided Procedure in non-blocking mode.
  * This means that this function will return immediately after starting the AP, allowing
  * the BSP to operate concurrently with the AP.
  *
  * @param[in]    This                  Pointer to the instance of the MP Utilities being used.
  * @param[in]    Procedure             A pointer to the function to be run on the specified processor.
  * @param[in]    ProcessorNumber       The zero-based number of the processor to be started.
  * @param[in]    ProcedureArgument     The parameter passed in to Procedure.
  *
  * @retval EFI_SUCCESS             The function has been dispatched to the specified AP.
  * @retval EFI_UNSUPPORTED         The request was made after the UEFI event
  *                                 EFI_EVENT_GROUP_READY_TO_BOOT was signaled.
  * @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  * @retval EFI_NOT_READY           PI, IPF: The specified AP is busy or was not previously shut down cleanly.
  * @retval EFI_NOT_FOUND           The logical processor specified by ProcessorNumber does not exist.
  * @retval EFI_INVALID_PARAMETER   Procedure is NULL.
  * @retval EFI_INVALID_PARAMETER   Framework: Specified AP is busy.
  * @retval EFI_INVALID_PARAMETER   IA32, X64: ProcessorNumber specifies the BSP or a disabled AP.
  * @retval EFI_NOT_AVAILABLE_YET   IPF: ProcessorNumber specifies the BSP
  * @retval EFI_NOT_AVAILABLE_YET   IPF: Specified AP is disabled, unhealthy, or untested.
  *
**/
typedef
EFI_STATUS
(EFIAPI *MP_START_THIS_AP)(
  IN  MP_SERVICE_UTILITIES     *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  VOID                      *ProcedureArgument
);

/** Containing structure for pointers to the MP Utility Functions.
  *
**/
struct _MP_SERVICE_UTILITIES_ {
  MP_COUNT_PROCESSORS     GetNumberOfProcessors;
  MP_GET_PROCESSOR_INFO   GetProcessorInfo;
  MP_START_THIS_AP        StartThisAP;
};

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
);

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
 *  @param[out]   Tcb           Pointer to the TCB for the Client task.
 *  @param[out]   MpService     Pointer to the location to store the pointer to the MP Services Protocol.
 *  @param[out]   NumProc       Pointer to the location to store the Number of Processors into.
 *  @param[out]   NumEnabled    Pointer to variable to receive the Number of Enabled processors.
 *  @param[in]    ProcNum       Identifying number of the CPU to retrieve information about.
 *
 *  @retval EFI_SUCCESS         The function completed successfully.
 *  @retval EFI_DEVICE_ERROR    This function was not called by the BSP.
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
);

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
);

#endif  // _STARTCORE_UTILITY_H_
