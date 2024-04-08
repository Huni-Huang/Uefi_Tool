#  This is the UEFI Test application
#
#  Copyright (c) 2022-11-02, Huang Guangyuan. All rights reserved.<BR>

[Defines]
  PLATFORM_NAME                  = Huni
  PLATFORM_GUID                  = 4C834D5F-F525-4670-B14E-7ABE369D6C7E
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/Huni
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT


[LibraryClasses]
	UefiLib|MdePkg\Library\UefiLib\UefiLib.inf
	UefiApplicationEntryPoint|MdePkg\Library\UefiApplicationEntryPoint\UefiApplicationEntryPoint.inf
	PrintLib|MdePkg\Library\BasePrintLib\BasePrintLib.inf
	PcdLib|MdePkg\Library\BasePcdLibNull\BasePcdLibNull.inf
	MemoryAllocationLib|MdePkg\Library\UefiMemoryAllocationLib\UefiMemoryAllocationLib.inf
	DebugLib|MdePkg\Library\UefiDebugLibConOut\UefiDebugLibConOut.inf
	BaseMemoryLib|MdePkg\Library\BaseMemoryLib\BaseMemoryLib.inf
	BaseLib|MdePkg\Library\BaseLib\BaseLib.inf
	UefiBootServicesTableLib|MdePkg\Library\UefiBootServicesTableLib\UefiBootServicesTableLib.inf
	DevicePathLib|MdePkg\Library\UefiDevicePathLib\UefiDevicePathLib.inf
	UefiRuntimeServicesTableLib|MdePkg\Library\UefiRuntimeServicesTableLib\UefiRuntimeServicesTableLib.inf
	RegisterFilterLib|MdePkg\Library\RegisterFilterLibNull\RegisterFilterLibNull.inf
	DebugPrintErrorLevelLib|MdePkg\Library\BaseDebugPrintErrorLevelLib\BaseDebugPrintErrorLevelLib.inf
	ShellLib|ShellPkg\Library\UefiShellLib\UefiShellLib.inf
	FileHandleLib|MdePkg\Library\UefiFileHandleLib\UefiFileHandleLib.inf
	HiiLib|MdeModulePkg\Library\UefiHiiLib\UefiHiiLib.inf
	SortLib|MdeModulePkg\Library\UefiSortLib\UefiSortLib.inf
	UefiHiiServicesLib|MdeModulePkg\Library\UefiHiiServicesLib\UefiHiiServicesLib.inf
	ShellCommandLib|ShellPkg\Library\UefiShellCommandLib\UefiShellCommandLib.inf
	HandleParsingLib|ShellPkg\Library\UefiHandleParsingLib\UefiHandleParsingLib.inf
	OrderedCollectionLib|MdePkg\Library\BaseOrderedCollectionRedBlackTreeLib\BaseOrderedCollectionRedBlackTreeLib.inf
	PeCoffGetEntryPointLib|MdePkg\Library\BasePeCoffGetEntryPointLib\BasePeCoffGetEntryPointLib.inf
	IpmiBaseLib|MdeModulePkg\Library\BaseIpmiLibNull\BaseIpmiLibNull.inf
	UefiBootManagerLib|MdeModulePkg\Library\UefiBootManagerLib\UefiBootManagerLib.inf
	HobLib|MdeModulePkg\Library\BaseHobLibNull\BaseHobLibNull.inf
	PerformanceLib|MdePkg\Library\BasePerformanceLibNull\BasePerformanceLibNull.inf
	DxeServicesTableLib|MdePkg\Library\DxeServicesTableLib\DxeServicesTableLib.inf
	DxeServicesLib|MdePkg\Library\DxeServicesLib\DxeServicesLib.inf
	ReportStatusCodeLib|MdePkg\Library\BaseReportStatusCodeLibNull\BaseReportStatusCodeLibNull.inf
	VariablePolicyHelperLib|MdeModulePkg\Library\VariablePolicyHelperLib\VariablePolicyHelperLib.inf
	UefiShellDebug1CommandsLib|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
	IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
	BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf



[Components]
  HuniPkg/Application/WhsUefiTool/WhsUefiTool.inf
  HuniPkg/Application/GetChar/GetChar.inf
  HuniPkg/Application/GetVariable/GetVariable.inf