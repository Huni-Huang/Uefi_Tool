/** @file
  This sample application bases on HelloWorld PCD setting
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/Dwaxidmac.h>
#include <Library/HandleParsingLib.h>
#include <Protocol/IoMmu.h>
#include "UefiShellDebug1CommandsLib.h"

typedef enum {
  ShellIommuvalMemCopy,
  ShellIommuMap,
  ShellIommuUnMap,
  ShellIommuvalHelp
} SHELL_IOMMUVAL_ACCESS_TYPE;

typedef struct {
  UINT8 Handles;
  UINTN Regbase;
} DMAC_HANDLER;

static DMAC_HANDLER mDmacHandlerList[] = {
  {0x01, 0xF7001000},
  {0x02, 0xF7002000},
  {0, 0}
};

STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-dma", TypeFlag},
  {L"-c", TypeFlag},
  {L"-m", TypeFlag},
//  {L"-unmap", TypeFlag},  
  {L"-h", TypeFlag},
  {NULL, TypeMax}
};

CONST UINT64 mShellMmMaxNumber[] = {
  0, MAX_UINT8, MAX_UINT16, 0, MAX_UINT32, 0, 0, 0, MAX_UINT64
};

// #define MAP_HANDLE_INFO_SIGNATURE  SIGNATURE_32 ('H', 'M', 'A', 'P')
// typedef struct {
//   UINT32                                    Signature;
//   LIST_ENTRY                                Link;
//   EFI_HANDLE                                DeviceHandle;
//   UINT64                                    IoMmuAccess;
// } MAP_HANDLE_INFO;
// #define MAP_HANDLE_INFO_FROM_LINK(a) CR (a, MAP_HANDLE_INFO, Link, MAP_HANDLE_INFO_SIGNATURE)

// #define MAP_INFO_SIGNATURE  SIGNATURE_32 ('D', 'M', 'A', 'P')
// typedef struct {
//   UINT32                                    Signature;
//   LIST_ENTRY                                Link;
//   EDKII_IOMMU_OPERATION                     Operation;
//   UINTN                                     NumberOfBytes;
//   UINTN                                     NumberOfPages;
//   EFI_PHYSICAL_ADDRESS                      HostAddress;
//   EFI_PHYSICAL_ADDRESS                      DeviceAddress;
//   LIST_ENTRY                                HandleList;
// } MAP_INFO;
// #define MAP_INFO_FROM_LINK(a) CR (a, MAP_INFO, Link, MAP_INFO_SIGNATURE)

// LIST_ENTRY                        gMaps = INITIALIZE_LIST_HEAD_VARIABLE(gMaps);
// #define VTD_TPL_LEVEL TPL_NOTIFY

///*
//   This function fills DeviceHandle/IoMmuAccess to the MAP_HANDLE_INFO,
//   based upon the DeviceAddress.

//   @param[in]  DeviceHandle      The device who initiates the DMA access request.
//   @param[in]  DeviceAddress     The base of device memory address to be used as the DMA memory.
//   @param[in]  Length            The length of device memory address to be used as the DMA memory.
//   @param[in]  IoMmuAccess       The IOMMU access.

// */
// VOID
// SyncMapInfo (
//   IN UINTN MapAddress,
//   IN UINTN MapLength,
//   IN  VOID                                     **Mapping
//   )
// {
//   MAP_INFO                 *MapInfo;
//   LIST_ENTRY               *Link;
//   EFI_TPL                  OriginalTpl;
// DEBUG ((DEBUG_ERROR, "SyncMapInfo: MapAddress(0x%lx) - not found\n", MapAddress));
//   //
//   // Find MapInfo according to DeviceAddress
//   //
//   OriginalTpl = gBS->RaiseTPL (VTD_TPL_LEVEL);
//   MapInfo = NULL;
//   for (Link = GetFirstNode (&gMaps)
//        ; !IsNull (&gMaps, Link)
//        ; Link = GetNextNode (&gMaps, Link)
//        ) {
//     MapInfo = MAP_INFO_FROM_LINK (Link);
//     if (MapInfo->DeviceAddress == MapAddress) {
//       break;
//     }
//   }
//   if ((MapInfo == NULL) || (MapInfo->DeviceAddress != MapAddress)) {
//     DEBUG ((DEBUG_ERROR, "SyncMapInfo: MapAddress(0x%lx) - not found\n", MapAddress));
//     gBS->RestoreTPL (OriginalTpl);
//     return ;
//   }

//     *Mapping       = MapInfo;
//       DEBUG ((DEBUG_INFO, "MapInfo->Operation2: 0x%08x \n", MapInfo->Operation));
//     DEBUG ((DEBUG_INFO, "MapInfo->NumberOfBytes: 0x%08x\n", MapInfo->NumberOfBytes));
//     DEBUG ((DEBUG_INFO, "MapInfo->NumberOfPages: 0x%08x\n", MapInfo->NumberOfPages));
//         DEBUG ((DEBUG_INFO, "MapInfo->HostAddress: 0x%08x\n", MapInfo->HostAddress));
//             DEBUG ((DEBUG_INFO, "MapInfo->DeviceAddress: 0x%08x\n", MapInfo->DeviceAddress));
// //  DEBUG ((DEBUG_INFO, "IoMmuMap: 0x%08x - 0x%08x <==\n", *MapAddress, *Mapping));
//   gBS->RestoreTPL (OriginalTpl);

//   return ;
// }

// BOOLEAN
// CompareValue (
//   IN      UINTN                Source,
//   IN      UINTN                Destination,
//   IN      UINTN                Length
//   )
// {
//     UINTN                                 Index;
//   UINT32               SourceVal;
//   UINT32               DestinationVal;
//   UINT8                EquFlag =1;

//  for (Index = 0; Index < Length; Index = Index +4){
//     SourceVal = MmioRead32(Source + Length);
//     DestinationVal = MmioRead32(Destination + Length);
//       ShellPrintEx(-1,-1,L"\n66=========================================================\n");
//        ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_READ_INDEX_VALUE_METHOD), gShellDebug1HiiHandle, Length, SourceVal);
//           ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_READ_INDEX_VALUE_METHOD), gShellDebug1HiiHandle, Length, DestinationVal);
//     if (SourceVal != DestinationVal){
//       EquFlag = 0;
//       break;
//     }    
//   }
//   if(EquFlag){
//     return TRUE;
//   } else{
//   return FALSE;
//   } 
// }

//
// String token ID of help message text.
// Shell supports to find help message in the resource section of an application image if
// .MAN file is not found. This global variable is added to make build tool recognizes
// that the help string is consumed by user and then build tool will add the string into
// the resource section. Thus the application can use '-?' option to show help message in
// Shell.
//
//GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mStringHelpTokenId = STRING_TOKEN (STR_HELLO_WORLD_HELP_INFORMATION);

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
IommuValMain (
  IN UINTN AddressSrc,
  IN UINTN AddressDes,
  IN UINTN Length,
  IN UINT64 ControllerCounter
  )
{
  UINTN                                 Index;
//  UINT32                                Value32;
//  UINT32                                ValueSRC = 0;
  EFI_DMAC_IO_PROTOCOL                  *Dmac_Io;
  UINTN                                 HandleCounter;
  EFI_HANDLE                            *Handles;
  EFI_STATUS                            Status;
  SHELL_STATUS                          ShellStatus;
  EFI_DMAC_TRANSFER                     DmacTransfer;
  UINTN                                 Regbase;
  dma_config                            *Dmaconfig = NULL;

  Handles   = NULL;

  ShellStatus = SHELL_SUCCESS;
  Index = 0;
  ShellPrintEx(-1,-1,L"ControllerCounter1 = %x \n", ControllerCounter);
  while (mDmacHandlerList[Index].Handles != 0){
    if (mDmacHandlerList[Index].Handles == ControllerCounter){
      Regbase = mDmacHandlerList[Index].Regbase;
      break;
    }
    Index++;
  }
ShellPrintEx(-1,-1,L"ControllerCounter = %x \n", ControllerCounter);
ShellPrintEx(-1,-1,L"Index = %x \n", Index);
ShellPrintEx(-1,-1,L"Regbase = %x \n", Regbase);
ShellPrintEx(-1,-1,L"listsize = %x \n", sizeof(mDmacHandlerList)/sizeof(DMAC_HANDLER));
if (Index > (sizeof(mDmacHandlerList)/sizeof(DMAC_HANDLER)-1)) {
  ShellPrintEx(-1,-1,L"No DMA controller match user input\n");
 return EFI_UNSUPPORTED;
}
  ShellPrintEx(-1,-1,L"\n=========================================================\n");
  // for (Index = 0; Index < Length; Index = Index +4){
  //   Value32 = MmioRead32 (AddressSrc + Index);
  // }
  //   ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, Value32);
  //   for (Index = 0; Index < Length; Index = Index +4){
  //   MmioWrite32 (AddressSrc + Index, 0x55AA55AA);
  //   MmioWrite32 (AddressDes + Index, 0xAA55AA55);
  // }
//     for (Index = 0; Index < 0x10; Index = Index +4){
//     Value32 = MmioRead32 (AddressSrc + Index);
// // ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
//   }
//     for (Index = 0; Index < 0x10; Index = Index +4){
//     Value32 = MmioRead32 (AddressDes + Index);
// //  ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_DESTINATION_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
//   }
  Status    = gBS->LocateHandleBuffer (ByProtocol, &gDmatransferProtocolGuid, NULL, &HandleCounter, &Handles);
  if (EFI_ERROR (Status)) {
    ShellPrintEx(-1,-1,L"No DMA controller\n");
    return SHELL_INVALID_PARAMETER;
  }
  for (Index = 0; Index < HandleCounter; Index++) {
  //     if (ConvertHandleIndexToHandle(Index) != Handles[Index]) {
  //       continue;
  //    }

    Status = gBS->HandleProtocol (Handles[Index], &gDmatransferProtocolGuid, (VOID**)&Dmac_Io);
    if (!EFI_ERROR (Status)) {
            if (Dmac_Io->Regbase == Regbase){
                 DmacTransfer = Dmac_Io->Transfer;
                 for (Index = 0; Index < Length; Index = Index +4){
                    Dmaconfig->src = AddressSrc + Index;
                    Dmaconfig->dst = AddressDes + Index;
                    Dmaconfig->length = 3;
                    Dmaconfig->dma_mode = MEM_TO_MEM_DMAC;
                    Dmac_Io->Transfer(Dmac_Io,Dmaconfig);
                 }
            }
    }
}
  // for (Index = 0; Index < 0x10; Index = Index +4){
  //   Value32 = MmioRead32 (AddressSrc + Index);
  //   ValueSRC = Value32;
  // ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
  // }
  //   for (Index = 0; Index < 0x10; Index = Index +4){
  //   Value32 = MmioRead32 (AddressDes + Index);
  // ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_DESTINATION_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
  // }

  // for (Index = Length; Index > (Length-0x10); Index = Index -4){
  //   Value32 = MmioRead32 (AddressSrc + Index);
  //   ValueSRC = Value32;
  // ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
  // }
  //   for (Index = Length; Index > (Length-0x10); Index = Index -4){
  //   Value32 = MmioRead32 (AddressDes + Index);
  // ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_DESTINATION_VALUE_METHOD), gShellDebug1HiiHandle, Index, Value32);
  // }

//  for (Index = 0; Index < Length; Index = Index +4){
//   Value32 = MmioRead32 (AddressSrc + Index);
//   ValueSRC = MmioRead32 (AddressDes + Index);
//   if (Value32&ValueSRC){
//     continue;
//   } else{
//     goto FAIL;
//   }
//  }
//    ShellPrintEx(-1,-1,L"\nSuccess\n");
//    ShellPrintEx(-1,-1,L"\n=========================================================\n");
//      return EFI_SUCCESS;
// FAIL:
//    ShellPrintEx(-1,-1,L"\nFailed\n");
//    ShellPrintEx(-1,-1,L"\n=========================================================\n");
   return EFI_SUCCESS;
}

// EFI_STATUS
// EFIAPI
// IommuValUnMap (
//     IN UINTN MapAddress,
//     IN UINTN MapLenth
//   )
// {
//   UINTN                                 Index;
//   EDKII_IOMMU_PROTOCOL                  *mIoMmu;
//   EFI_DMAC_IO_PROTOCOL                  *Dmac_Io;
// //  VOID                  *Map0;
//   VOID                  *Map;
//   UINTN                                 HandleCounter;
//   EFI_HANDLE                            *Handles = NULL;
// //  EFI_HANDLE                            *Dmac_controller = NULL;
//   EFI_STATUS                            Status;
//   UINTN                                 Regbase0;
//   UINTN                                 Regbase1;  
// //  UINTN                                 MapLength = 0x200000;
//     // EFI_PHYSICAL_ADDRESS  DeviceAddress;
//     // UINTN                 Length = 0;
//   Handles   = NULL;
//   Map = NULL;

//  SyncMapInfo(MapAddress, MapLenth, &Map);
//   ShellPrintEx(-1,-1,L"\n=========================================================\n");

//   Regbase0 = 0xF7001000;
//   Regbase1 = 0xF7002000;
//   Status    = gBS->LocateHandleBuffer (ByProtocol, &gDmatransferProtocolGuid, NULL, &HandleCounter, &Handles);
//   if (EFI_ERROR (Status)) {
//     ShellPrintEx(-1,-1,L"No DMA controller\n");
//     return SHELL_INVALID_PARAMETER;
//   }
//   for (Index = 0; Index < HandleCounter; Index++) {

//     Status = gBS->HandleProtocol (Handles[Index], &gDmatransferProtocolGuid, (VOID**)&Dmac_Io);
//     if (!EFI_ERROR (Status)) {
//       if (Dmac_Io->Regbase == Regbase1){
//             ShellPrintEx(-1,-1,L"Regbase1\n");
// //        Dmac_controller = Handles[Index];
// //        Map = Map0;


//   // gBS->AllocatePages (
//   //      AllocateAddress,
//   //      EfiBootServicesData,
//   //      EFI_SIZE_TO_PAGES (MapLength),
//   //      &MapAddress
//   //      );
// // ShellPrintEx(-1,-1,L"MapAddress = %x \n", MapAddress);
// //   DeviceAddress = (EFI_PHYSICAL_ADDRESS) MapAddress;
// //   ShellPrintEx(-1,-1,L"DeviceAddress = %x \n", DeviceAddress);
// //   Length = MapLength;
// //   Status = gBS->AllocatePages (
// //        AllocateAddress,
// //        EfiRuntimeServicesData,
// //        EFI_SIZE_TO_PAGES (Length),
// //        &DeviceAddress
// //        );
// //     if (!EFI_ERROR (Status)) {
// //             ShellPrintEx(-1,-1,L"Allocate Done\n");
// //           }
  
//   Status = gBS->LocateProtocol (
//           &gEdkiiIoMmuProtocolGuid,
//           NULL,
//           (VOID **) &mIoMmu
//           );
//     if (!EFI_ERROR (Status)) {
//             ShellPrintEx(-1,-1,L"gEdkiiIoMmuProtocolGuid Done\n");
//           }

//   if (mIoMmu != NULL){
//                 ShellPrintEx(-1,-1,L"mIoMmu Done\n");
//   mIoMmu->SetAttribute (
//           mIoMmu,
//           Handles[Index],
//           Map,
//           0
//           );
//   mIoMmu->Unmap(
//           mIoMmu,
//           Map
//           );
//     }
//    }
//   }
//  }

//    return EFI_SUCCESS;


// }


EFI_STATUS
EFIAPI
IommuValMap (
   IN UINT64 MapAddress,
   IN UINT64 MapLength,
   IN UINT64 ControllerCounter
  )
{
  UINTN                                 Index;
  EDKII_IOMMU_PROTOCOL                  *mIoMmu;
  EFI_DMAC_IO_PROTOCOL                  *Dmac_Io;
//  VOID                  *Map0;
  VOID                  *Map;
  UINTN                                 HandleCounter;
  EFI_HANDLE                            *Handles = NULL;
//  EFI_HANDLE                            *Dmac_controller = NULL;
  EFI_STATUS                            Status;
  UINTN                                 Regbase;
//  UINTN                                 MapLength = 0x200000;
    EFI_PHYSICAL_ADDRESS  DeviceAddress;
    UINT64                 Length = 0;
  Handles   = NULL;


  ShellPrintEx(-1,-1,L"\n=========================================================\n");
  Index = 0;
  while (mDmacHandlerList[Index].Handles != 0){
    if (mDmacHandlerList[Index].Handles == ControllerCounter){
      Regbase = mDmacHandlerList[Index].Regbase;
      break;
    }
    Index++;
  }
ShellPrintEx(-1,-1,L"ControllerCounter = %x \n", ControllerCounter);
ShellPrintEx(-1,-1,L"Index = %x \n", Index);
ShellPrintEx(-1,-1,L"Regbase = %x \n", Regbase);
ShellPrintEx(-1,-1,L"listsize = %x \n", sizeof(mDmacHandlerList)/sizeof(DMAC_HANDLER));
if (Index > (sizeof(mDmacHandlerList)/sizeof(DMAC_HANDLER)-1)){
  ShellPrintEx(-1,-1,L"No DMA controller match user input\n");
 return EFI_UNSUPPORTED;
}

  Status    = gBS->LocateHandleBuffer (ByProtocol, &gDmatransferProtocolGuid, NULL, &HandleCounter, &Handles);
  if (EFI_ERROR (Status)) {
    ShellPrintEx(-1,-1,L"No DMA controller\n");
    return SHELL_INVALID_PARAMETER;
  }
  for (Index = 0; Index < HandleCounter; Index++) {

    Status = gBS->HandleProtocol (Handles[Index], &gDmatransferProtocolGuid, (VOID**)&Dmac_Io);
    if (!EFI_ERROR (Status)) {
      if (Dmac_Io->Regbase == Regbase){
            ShellPrintEx(-1,-1,L"Regbase0\n");
//        Dmac_controller = Handles[Index];
//        Map = Map0;


  // gBS->AllocatePages (
  //      AllocateAddress,
  //      EfiBootServicesData,
  //      EFI_SIZE_TO_PAGES (MapLength),
  //      &MapAddress
  //      );
  ShellPrintEx(-1,-1,L"MapAddress = %x \n", MapAddress);
  DeviceAddress = (EFI_PHYSICAL_ADDRESS) MapAddress;
  ShellPrintEx(-1,-1,L"DeviceAddress = %x \n", DeviceAddress);
  Length = MapLength;
  ShellPrintEx(-1,-1,L"Length = %x \n", Length);
  Status = gBS->AllocatePages (
       AllocateAddress,
       EfiBootServicesData,
       EFI_SIZE_TO_PAGES (Length),
       &DeviceAddress
       );
    if (!EFI_ERROR (Status)) {
            ShellPrintEx(-1,-1,L"Allocate Done\n");
          }
  
  Status = gBS->LocateProtocol (
          &gEdkiiIoMmuProtocolGuid,
          NULL,
          (VOID **) &mIoMmu
          );
    if (!EFI_ERROR (Status)) {
            ShellPrintEx(-1,-1,L"gEdkiiIoMmuProtocolGuid Done\n");
          }
  // mIoMmu->Map(
  //         mIoMmu,
  //         EdkiiIoMmuOperationBusMasterCommonBuffer,
  //         &MapAddress,
  //         &MapLength,
  //         &MapAddress,
  //         &Map
  //         );

  // mIoMmu->SetAttribute (
  //         mIoMmu,
  //         Dmac_controller,
  //         Map,
  //         EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
  //         );
  if (mIoMmu != NULL){
                ShellPrintEx(-1,-1,L"mIoMmu Done\n");
  // mIoMmu->AllocateBuffer (
  //            mIoMmu,
  //            AllocateMaxAddress,
  //            EfiBootServicesData,
  //            EFI_SIZE_TO_PAGES (Length),
  //            (void **)(&DeviceAddress),
  //            (UINT64)EDKII_IOMMU_ATTRIBUTE_VALID_FOR_ALLOCATE_BUFFER
  //            );

  mIoMmu->Map(
          mIoMmu,
          EdkiiIoMmuOperationBusMasterCommonBuffer64,
          (void *)DeviceAddress,
          &Length,
          &DeviceAddress,
          &Map
          );

  mIoMmu->SetAttribute (
          mIoMmu,
          Handles[Index],
          Map,
          EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
          );
    }
   }
  }
 }

   return EFI_SUCCESS;
}

/**
  Function for 'IommuVal' command.

  @param[in] ImageHandle  Handle to the Image (NULL if Internal).
  @param[in] SystemTable  Pointer to the System Table (NULL if Internal).
**/
SHELL_STATUS
EFIAPI
ShellCommandRunIommuVal (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS          Status;
  LIST_ENTRY          *Package;
  CHAR16              *ProblemParam;
  SHELL_STATUS        ShellStatus;
  CONST CHAR16                          *Temp = NULL;
  UINT8                                 AccessType = 0;
  UINT64 AddressSrc = 0;
  UINT64 AddressDes = 0;
  UINT64 MapAddress = 0;
  UINT64 MapLength = 0; 
  UINT64 Length = 0;
  UINT64 Controller = 0;
  Package        = NULL;
  ShellStatus    = SHELL_SUCCESS;



  //
  // Parse arguments
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gShellDebug1HiiHandle, L"iommuval", ProblemParam);
      FreePool (ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto Done;
    } else {
      ASSERT (FALSE);
    }
  } else {
    // if (ShellCommandLineGetCount (Package) < 3) {
    //   ShellPrintEx(-1,-1,L"\na=========================================================\n");
    //   ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_FEW), gShellDebug1HiiHandle, L"iommuval");
    //   ShellStatus = SHELL_INVALID_PARAMETER;
    //   goto Done;
    // } else if (ShellCommandLineGetCount (Package) > 4) {
    //   ShellPrintEx(-1,-1,L"\nb=========================================================\n");
    //   ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gShellDebug1HiiHandle, L"iommuval");
    //   ShellStatus = SHELL_INVALID_PARAMETER;
    //   goto Done;
    // } else if (ShellCommandLineGetFlag(Package, L"-c") && ShellCommandLineGetValue(Package, L"-c") == NULL) {
    //   ShellPrintEx(-1,-1,L"\nc=========================================================\n");
    //   ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gShellDebug1HiiHandle, L"iommuval", L"-c");
    //   ShellStatus = SHELL_INVALID_PARAMETER;
    // } else {
    //   if (ShellCommandLineGetFlag (Package, L"-h")) {
    //     AccessType = ShellIommuvalHelp;
    //     if (ShellCommandLineGetFlag (Package, L"-c")
    //         || ShellCommandLineGetFlag (Package, L"-c")
    //         ) {
    //       ShellPrintEx(-1,-1,L"\nd=========================================================\n");
    //       ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gShellDebug1HiiHandle, L"iommuval");
    //       ShellStatus = SHELL_INVALID_PARAMETER;
    //       goto Done;
    //     }
    //   } else 
      if (ShellCommandLineGetFlag (Package, L"-dma") && ShellCommandLineGetFlag (Package, L"-c")) {
        ShellPrintEx(-1,-1,L"\n========================Copy=============================\n");
        AccessType = ShellIommuvalMemCopy;
      }
      if (ShellCommandLineGetFlag (Package, L"-dma") && ShellCommandLineGetFlag (Package, L"-m")) {
        ShellPrintEx(-1,-1,L"\n===================SetupPageTable========================\n");
        AccessType = ShellIommuMap;
      }
      if (ShellCommandLineGetFlag (Package, L"-unmap")) {
        ShellPrintEx(-1,-1,L"\n===================Unmap========================\n");
        AccessType = ShellIommuUnMap;
      }
      if (ShellCommandLineGetFlag(Package, L"-dma") && ShellCommandLineGetFlag(Package, L"-h")) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GET_HELP_IOMMUVAL), gShellDebug1HiiHandle,  L"iommuval", L"-h");
      }
    }
//  }

    //
    // Non interactive for a script file or for the specific parameter
    //
//    Interactive = TRUE;
    switch(AccessType){
      case ShellIommuvalMemCopy:
        Temp = ShellCommandLineGetRawValue (Package, 1);
        Status = ShellConvertStringToUint64 (Temp, &AddressSrc, TRUE, TRUE);
  //              ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, L"AddressSrc", AddressSrc);
        if (EFI_ERROR (Status)) {
  //       ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
         ShellStatus = SHELL_INVALID_PARAMETER;
         goto Done;
        }

        Temp = ShellCommandLineGetRawValue (Package, 2);
                ShellPrintEx (-1, -1, L"AddressDesTemp = 0x%x", *Temp);
        Status = ShellConvertStringToUint64 (Temp, &AddressDes, TRUE, TRUE);
                ShellPrintEx(-1,-1,L"AddressDes = 0x%x", AddressDes);
        if (EFI_ERROR (Status)) {
  //        ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
          ShellStatus = SHELL_INVALID_PARAMETER;
          goto Done;
        }

        Temp = ShellCommandLineGetRawValue (Package, 3);
        ShellPrintEx (-1, -1, L"LengthTemp = 0x%x", *Temp);
        Status = ShellConvertStringToUint64 (Temp, &Length, TRUE, TRUE);
        ShellPrintEx (-1, -1, L"Length = 0x%x", Length);
        if (EFI_ERROR (Status)) {
          ShellStatus = SHELL_INVALID_PARAMETER;
          goto Done;
        }

        Temp = ShellCommandLineGetRawValue (Package, 4);
        ShellPrintEx (-1, -1, L"ControllerTemp = 0x%x", *Temp);
        Status = ShellConvertStringToUint64 (Temp, &Controller, TRUE, TRUE);
        ShellPrintEx (-1, -1, L"Controller = 0x%x", Controller);
        if (EFI_ERROR (Status)) {
          ShellStatus = SHELL_INVALID_PARAMETER;
          goto Done;
        }
        break;

      case ShellIommuMap:
        Temp = ShellCommandLineGetRawValue (Package, 1);
        Status = ShellConvertStringToUint64 (Temp, &MapAddress, TRUE, TRUE);
 //               ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, MapAddress, MapAddress);
        if (EFI_ERROR (Status)) {
//         ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
         ShellStatus = SHELL_INVALID_PARAMETER;
         goto Done;
        }

        Temp = ShellCommandLineGetRawValue (Package, 2);
        Status = ShellConvertStringToUint64 (Temp, &MapLength, TRUE, TRUE);
        if (EFI_ERROR (Status)) {
 //         ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
          ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, MapAddress, MapAddress);
          ShellStatus = SHELL_INVALID_PARAMETER;
          goto Done;
        }
        Temp = ShellCommandLineGetRawValue (Package, 3);
        Status = ShellConvertStringToUint64 (Temp, &Controller, TRUE, TRUE);
        if (EFI_ERROR (Status)) {
  //        ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
          ShellStatus = SHELL_INVALID_PARAMETER;
          goto Done;
        }
        break;
//       case ShellIommuUnMap:
//         Temp = ShellCommandLineGetRawValue (Package, 1);
//         Status = ShellConvertStringToUint64 (Temp, &MapAddress, TRUE, TRUE);
//  //\               ShellPrintHiiEx(-1,-1,NULL,STRING_TOKEN (STR_IOMMUVAL_SOURCE_VALUE_METHOD), gShellDebug1HiiHandle, MapAddress, MapAddress);
//         if (EFI_ERROR (Status)) {
// //\         ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
//          ShellStatus = SHELL_INVALID_PARAMETER;
//          goto Done;
//         }

//         Temp = ShellCommandLineGetRawValue (Package, 2);
//         Status = ShellConvertStringToUint64 (Temp, &MapLength, TRUE, TRUE);
//         if (EFI_ERROR (Status)) {
//  //\         ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_PARAM_INV), gShellDebug1HiiHandle, L"iommuval", Temp);
//           ShellStatus = SHELL_INVALID_PARAMETER;
//           goto Done;
//         }              
      default:
       break;
     }
    // if ((Address & (Size - 1)) != 0) {
    //   ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_MM_NOT_ALIGNED), gShellDebug1HiiHandle, L"mm", Address);
    //   ShellStatus = SHELL_INVALID_PARAMETER;
    //   goto Done;
    // }
    switch(AccessType){
      case ShellIommuvalMemCopy:
      ShellPrintEx (-1, -1, L"Controller3 = 0x%x", Controller);
       IommuValMain((UINTN)AddressSrc,(UINTN) AddressDes, (UINTN)Length, Controller);
       break;
      case ShellIommuMap:
        IommuValMap(MapAddress,MapLength, Controller);
       //      IommuValMap();
       break;
      // case ShellIommuUnMap:
      //   IommuValUnMap((UINTN)MapAddress,(UINTN)MapLength);
      //  //      IommuValUnMap();
      //  break;
      default:
       goto Done;
    }

Done:
  // if (InputStr != NULL) {
  //   FreePool (InputStr);
  // }
  if (Package != NULL) {
    ShellCommandLineFreeVarList (Package);
  }
  return ShellStatus;
}