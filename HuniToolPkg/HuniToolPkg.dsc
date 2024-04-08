[Defines]
  PLATFORM_NAME                  = HuniToolPkg
  PLATFORM_GUID                  = 0348cade-8a6e-4ee5-b773-1b2ffbda3e06
  PLATFORM_VERSION               = 0.01
  DSC_SPECIFICATION              = 0x00010006
  OUTPUT_DIRECTORY               = Build/HuniToolPkg
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT


[LibraryClasses]
  #
  # Entry point
  #
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
	


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

[LibraryClasses.common.UEFI_APPLICATION]
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
[Components]

#### Sample Applications.
  #Huni add 2022-11-05
  HuniToolPkg/GetChar/GetChar.inf
  HuniToolPkg/ShellMain/ShellApp.inf
  HuniToolPkg/GetVariable/GetVariable.inf
  HuniToolPkg/ReadKey/ReadKey.inf
  HuniToolPkg/TouchPad/TouchPad.inf

