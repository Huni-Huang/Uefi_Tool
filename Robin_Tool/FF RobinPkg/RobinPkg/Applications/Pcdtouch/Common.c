/** @file
  Member functions of global protocol and functions for creation,
  code by luobing 2013-3-19 9:51:53.
**/

#include "Common.h"

EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gConInEx;
EFI_SIMPLE_POINTER_PROTOCOL *gMouse;   //Mouse
EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
EFI_PCI_IO_PROTOCOL *gPCIIO;

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *gSimpleFileSystem;  //操作FAT文件系统的Protocol实例
EFI_FILE_PROTOCOL *gFileRoot;		//操作文件的Protocol实例

EFI_HII_FONT_PROTOCOL *gHiiFont;
EFI_HII_IMAGE_PROTOCOL *gHiiImage;
EFI_HII_IMAGE_EX_PROTOCOL *gHiiImageEx;
// EFI_HII_IMAGE_EX_PROTOCOL *gHiiImageExArray[256];
// UINTN nImageExInstance = 0;
EFI_HII_IMAGE_DECODER_PROTOCOL *gHiiImageDecoder;

EFI_RNG_PROTOCOL *gRNGOut;			//Handle of Generating random number 2019-08-31 11:36:43 robin

EFI_BLOCK_IO_PROTOCOL* gBlockIoArray[256];
UINTN nBlockIO = 0;
//================  Member function =======================================
//Name:InintGloabalProtocols
//Input:
//Output: 0: 所有protocol都被获取
//		非零: 所对应位标志的protocol没有被正确获取
//        
//Descriptor:
UINT64 InintGloabalProtocols(UINT64 flag)
{
	EFI_STATUS                        Status;
	UINT64	retVal=0;
	
	//Locate SimpleTextInputEx protocol
	if((flag&S_TEXT_INPUT_EX) == S_TEXT_INPUT_EX)
	{
		Status = LocateSimpleTextInputEx();
		if (EFI_ERROR (Status)) 
			retVal|=S_TEXT_INPUT_EX;
	}
	
	//Locate GraphicsOutput protocol
	if((flag&GRAPHICS_OUTPUT) == GRAPHICS_OUTPUT)
	{
		Status = LocateGraphicsOutput();
		if (EFI_ERROR (Status)) 
			retVal|=GRAPHICS_OUTPUT;
	}
	
	//Locate PCIRootBridgeIO protocol
	if((flag&PCI_ROOTBRIDGE_IO) == PCI_ROOTBRIDGE_IO)
	{
		Status = LocatePCIRootBridgeIO();
		if (EFI_ERROR (Status)) 
			retVal|=PCI_ROOTBRIDGE_IO;
	}
	
	//Locate PCIIO protocol
	if((flag&PCI_IO) == PCI_IO)
	{
		Status = LocatePCIIO();
		if (EFI_ERROR (Status)) 
			retVal|=PCI_IO;
	}

	//Locate FileIO protocol
	if((flag&FILE_IO) == FILE_IO)
	{
		Status = LocateFileRoot();
		if (EFI_ERROR (Status)) 
			retVal|=FILE_IO;
	}
	
	//Locate HII FONT protocol
	if((flag&HII_FONT) == HII_FONT)
	{
		Status = LocateHiiFont();
		if (EFI_ERROR (Status)) 
			retVal|=HII_FONT;
	}
	if((flag&HII_IMAGE) == HII_IMAGE)
	{
		Status = LocateHiiImage();
		if (EFI_ERROR (Status)) 
			retVal|=HII_IMAGE;
	}
	if((flag&HII_IMAGE_EX) == HII_IMAGE_EX)
	{
		Status = LocateHiiImageEx();
		if (EFI_ERROR (Status)) 
			retVal|=HII_IMAGE_EX;
	}
	
	if((flag&HII_IMAGE_DECODER) == HII_IMAGE_DECODER)
	{
		Status = LocateHiiImageDecoder();
		if (EFI_ERROR (Status)) 
			retVal|=HII_IMAGE_DECODER;
	}

	//Locate RNGOut protocol
	if((flag&RNG_OUT) == RNG_OUT)
	{
		Status = LocateRNGOut();
		if (EFI_ERROR (Status)) 
			retVal|=RNG_OUT;
	}

	if((flag&SIMPLE_POINTER) == SIMPLE_POINTER)
	{
		Status = LocateMouse();
		if (EFI_ERROR (Status)) 
			retVal|=SIMPLE_POINTER;
	}

	if((flag&BLOCK_IO) == BLOCK_IO)
	{
		Status = LocateBlockIO();
		if (EFI_ERROR (Status)) 
			retVal|=BLOCK_IO;
	}
	return retVal;
}
//Name:LocateSimpleTextInputEx
//Input:
//Output: EFI_STATUS=EFI_SUCCESS, *SimpleTextInputEx
//				EFI_STATUS=
//Descriptor: Get the handle of SimpleTextInputEx
EFI_STATUS LocateSimpleTextInputEx(void)
{
	EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;

  
  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &gConInEx);
    
    if (EFI_ERROR(Status))	
    	continue;
		else
		{
			return EFI_SUCCESS;
  	}
  }	 
	return Status;
}
//Name: LocateGraphicsOutput
//Input: 
//Output: *GraphicsOutput
//Descriptor:
EFI_STATUS LocateGraphicsOutput (void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *GraphicsOutputControllerHandles = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports GraphicsOutputProtocol
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		&HandleCount,
		&GraphicsOutputControllerHandles
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			GraphicsOutputControllerHandles[HandleIndex],
			&gEfiGraphicsOutputProtocolGuid,
			(VOID**)&gGraphicsOutput);
		if (EFI_ERROR(Status))	continue;

		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: LocatePCIRootBridgeIO
//Input: 
//Output: *gPCIRootBridgeIO
//Descriptor:
EFI_STATUS LocatePCIRootBridgeIO(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PciHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciRootBridgeIoProtocolGuid,
		NULL,
		&HandleCount,
		&PciHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PciHandleBuffer[HandleIndex],
			&gEfiPciRootBridgeIoProtocolGuid,
			(VOID**)&gPCIRootBridgeIO);
		if (EFI_ERROR(Status))	continue;
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: LocatePCIIO
//Input: 
//Output: *gPCIIO
//Descriptor:
EFI_STATUS LocatePCIIO(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PciHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&HandleCount,
		&PciHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PciHandleBuffer[HandleIndex],
			&gEfiPciIoProtocolGuid,
			(VOID**)&gPCIIO);
		if (EFI_ERROR(Status))	continue;
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}

//Name: LocateFileIO
//Input: 
//Output: *gFileRoot
//Descriptor:
EFI_STATUS LocateFileRoot(void)
{
	EFI_STATUS  Status = 0;
	
	 Status = gBS->LocateProtocol(
            &gEfiSimpleFileSystemProtocolGuid,
            NULL,
            (VOID**)&gSimpleFileSystem //实际程序用不到，还是预留了接口 2019-06-10 19:51:02 luobing
    );
    if (EFI_ERROR(Status)) {
     //找到EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
        return Status;
    }
    Status = gSimpleFileSystem->OpenVolume(gSimpleFileSystem, &gFileRoot);
}

//Name: LocateHiiFont
//Input: 
//Output: *gHiiFont
//Descriptor:
EFI_STATUS LocateHiiFont(void)  ////2019-6-8 7:38:56 luobing
{
	EFI_STATUS                         Status;
	Status = gBS->LocateProtocol (&gEfiHiiFontProtocolGuid, NULL, (VOID **) &gHiiFont);
	
	return Status;
}


//Name: LocateHiiImage
//Input: 
//Output: *gHiiImage
//Descriptor:
EFI_STATUS LocateHiiImage(void)   
{
	EFI_STATUS                         Status;
	Status = gBS->LocateProtocol (&gEfiHiiImageProtocolGuid, NULL, (VOID **) &gHiiImage);
	
	return Status;
}

//Name: LocateHiiImageEx
//Input: 
//Output: *gHiiImageEx
//Descriptor:
EFI_STATUS LocateHiiImageEx(void)   
{
	EFI_STATUS                         Status;
	// EFI_STATUS                         Status;
	// EFI_HANDLE                         *ImgExHandleBuffer = NULL;
	// UINTN                              HandleIndex = 0;
	// UINTN                              HandleCount = 0;
	// //get the handles which supports 
	// Status = gBS->LocateHandleBuffer(
	// 	ByProtocol,
	// 	&gEfiHiiImageExProtocolGuid,
	// 	NULL,
	// 	&HandleCount,
	// 	&ImgExHandleBuffer
	// 	);
	// if (EFI_ERROR(Status))	return Status;		//unsupport
	// nImageExInstance = HandleCount;		//保存串口数目
	// for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	// {
	// 	Status = gBS->HandleProtocol(
	// 		ImgExHandleBuffer[HandleIndex],
	// 		&gEfiHiiImageExProtocolGuid,
	// 		(VOID**)&(gHiiImageExArray[HandleIndex]));
	// 	// if (EFI_ERROR(Status))	continue;
	// 	// else
	// 	// {
	// 	// 	return EFI_SUCCESS;
	// 	// }
	// }
	// gHiiImageEx = gHiiImageExArray[0];
	
	// return Status;

	Status = gBS->LocateProtocol (&gEfiHiiImageExProtocolGuid, NULL, (VOID **) &gHiiImageEx);
	
	return Status;
}

//Name: LocateHiiImageDecoder
//Input: 
//Output: *gHiiImageDecoder
//Descriptor:
EFI_STATUS LocateHiiImageDecoder(void)   
{
	EFI_STATUS                         Status;
	Status = gBS->LocateProtocol (&gEfiHiiImageDecoderProtocolGuid, NULL, (VOID **) &gHiiImageDecoder);
	
	return Status;
}

//Name: LocateRNGOut
//Input: 
//Output: *gRNGOut
//Descriptor:
EFI_STATUS LocateRNGOut(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *RngHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiRngProtocolGuid,
		NULL,
		&HandleCount,
		&RngHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			RngHandleBuffer[HandleIndex],
			&gEfiRngProtocolGuid,
			(VOID**)&gRNGOut);
		if (EFI_ERROR(Status))	continue;
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}

//Name: LocateMouse
//Input: 
//Output: *gMouse
//Descriptor:
EFI_STATUS LocateMouse(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PointerHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiSimplePointerProtocolGuid,
		NULL,
		&HandleCount,
		&PointerHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PointerHandleBuffer[HandleIndex],
			&gEfiSimplePointerProtocolGuid,
			(VOID**)&gMouse);
		if (EFI_ERROR(Status))	continue;
		else
		{
			return EFI_SUCCESS;
		}
	}
	return Status;
}
//Name: LocateBlockIO
//Input: 
//Output: *gBlockIoArray[]
//Descriptor:
EFI_STATUS LocateBlockIO(void)
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *BlockIOHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports 
	Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiBlockIoProtocolGuid,
		NULL,
		&HandleCount,
		&BlockIOHandleBuffer
		);
	if (EFI_ERROR(Status))	return Status;		//unsupport
	nBlockIO = HandleCount;		//保存BlockIO数目
	if(HandleCount>250)
		HandleCount = 250; //只支持250个存储设备，应该不大可能有这么多
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			BlockIOHandleBuffer[HandleIndex],
			&gEfiBlockIoProtocolGuid,
			(VOID**)&(gBlockIoArray[HandleIndex]));
		if (EFI_ERROR(Status))	break;
		else
		{
			Status = EFI_SUCCESS;
		}
	}
	// gSerialIO = gSerialIOArray[1];
	if(BlockIOHandleBuffer!=NULL)
		FreePool(BlockIOHandleBuffer);
	return Status;
}

//Function Name: Delayms
//Input: ms-milliseconds
//Output: none
//Description: delay by microsecond
VOID Delayms(IN UINTN ms)
{
  gBS->Stall(ms*1000);
}