/** @file
  function definitions for internal to common functions.
	code by luobing 2013-3-19 9:47:01
**/

#ifndef _COMMON_H
#define _COMMON_H
//���а�����Ŀ¼��mdepkg��ȥ��
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/DebugLib.h>

#include <Library/BaseMemoryLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
//#include <Protocol/SimpleTextInEx.h>
//#include <Library/PciLib.h>
//#include <Library/PciExpressLib.h>
//#include <IndustryStandard/Pci.h>
#include <Protocol/SimplePointer.h>		//��ȡ���?
#include <Protocol/SerialIo.h>	//���ڣ�
#include <Protocol/PciIo.h>						//��ȡPciIO protocol����
#include <Protocol/PciRootBridgeIo.h>	//��ȡPciRootBridgeIO protocol����
#include <IndustryStandard/Pci.h>  //pci���������ͷ�ļ�������pci22.h,pci23.h...

#define S_TEXT_INPUT_EX  		0x0000000000000001
#define GRAPHICS_OUTPUT  		0x0000000000000002
#define PCI_ROOTBRIDGE_IO		0x0000000000000004
#define PCI_IO							0x0000000000000008

UINT64 InintGloabalProtocols(UINT64 flag);
EFI_STATUS LocateSimpleTextInputEx(void);
EFI_STATUS LocateGraphicsOutput (void);
EFI_STATUS LocatePCIRootBridgeIO(void);
EFI_STATUS LocatePCIIO(void);

EFI_STATUS LocateMouse(void);

//����ʹ��
EFI_STATUS LocateSerialIO(void);
//
//extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;
//extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
//extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
//extern EFI_PCI_IO_PROTOCOL *gPCIIO;
//

#endif