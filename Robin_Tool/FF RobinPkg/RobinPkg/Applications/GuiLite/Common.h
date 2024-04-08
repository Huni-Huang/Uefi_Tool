/** @file
  function definitions for internal to common functions.
	code by luobing 2013-3-19 9:47:01
**/

#ifndef _COMMON_H
#define _COMMON_H
//所有包含的目录到mdepkg下去找
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Protocol/PciIo.h>						//获取PciIO protocol所需
#include <Protocol/PciRootBridgeIo.h>	//获取PciRootBridgeIO protocol所需
#include <IndustryStandard/Pci.h>  //pci访问所需的头文件，包含pci22.h,pci23.h...
#include <Protocol/Rng.h>  //Random Number Generator Protocol 2019-08-31 11:32:03 robin

#include <Protocol/SimpleFileSystem.h> //文件系统访问
#include <IndustryStandard/Bmp.h> //for bmp


#define S_TEXT_INPUT_EX  		0x0000000000000001
#define GRAPHICS_OUTPUT  		0x0000000000000002
#define PCI_ROOTBRIDGE_IO		0x0000000000000004
#define PCI_IO							0x0000000000000008
#define HII_FONT						0x0000000000000010  //2019-6-8 7:39:47 luobing
#define FILE_IO             0x0000000000000020  //2019-06-10 19:53:27 luobing
#define RNG_OUT             0x0000000000000040  //2019-08-31 11:33:18 robin

UINT64 InintGloabalProtocols(UINT64 flag);
EFI_STATUS LocateSimpleTextInputEx(void);
EFI_STATUS LocateGraphicsOutput (void);
EFI_STATUS LocatePCIRootBridgeIO(void);
EFI_STATUS LocatePCIIO(void);
EFI_STATUS LocateFileRoot(void);
EFI_STATUS LocateRNGOut(void);
void delay_ms(unsigned short nms);

//
//extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;
//extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
//extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
//extern EFI_PCI_IO_PROTOCOL *gPCIIO;
//

#endif