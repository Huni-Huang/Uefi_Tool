//code by luobing 2013-5-16 16:30:22
#include "SerialPort.h"

extern EFI_SERIAL_IO_PROTOCOL *gSerialIO;
//Name: DisplaySerialMode
//Input: gSerialIO
//Output: None
//Description:
EFI_STATUS DisplaySerialMode(void)
{
	Print(L"Print Current Mode of SerialPort:\n");
	Print(L"::ControlMask=0x%x\n",gSerialIO->Mode->ControlMask);
	Print(L"::Timeout=%d\n",gSerialIO->Mode->Timeout);
	Print(L"::BaudRate=%d\n",gSerialIO->Mode->BaudRate);
	Print(L"::ReceiveFifoDepth=%d\n",gSerialIO->Mode->ReceiveFifoDepth);
	Print(L"::DataBits=%d\n",gSerialIO->Mode->DataBits);
	Print(L"::Parity=%d\n",gSerialIO->Mode->Parity);
	Print(L"::StopBits=%d\n",gSerialIO->Mode->StopBits);
	
	return EFI_SUCCESS;
}

//Name: SendDataBySerial
//Input: 
//Output: @retval EFI_SUCCESS      The new control bits were set on the serial device.
//  @retval EFI_UNSUPPORTED  The serial device does not support this operation.
//  @retval EFI_DEVICE_ERROR The serial device is not functioning correctly.
//Description:
EFI_STATUS SendDataBySerial(UINTN length,VOID *buffer)
{
	EFI_STATUS Status;
	UINTN BufferSize;
	
	BufferSize=length;
	Status = gSerialIO->Read(gSerialIO,&BufferSize,buffer);
	return Status;
}

//Name: SetSerialPortAttrib
//Input: gSerialIO
//      Parity:DefaultParity,NoParity,EvenParity,OddParity,MarkParity,SpaceParity
//			StopBits:DefaultStopBits,OneStopBit,OneFiveStopBits,TwoStopBits
//Output: EFI_SUCCESS: The new attributes were set on the serial device
//				EFI_INVALID_PARAMETRE: One or more of the attributes has an unsupported value
//				EFI_DEVICE_ERROR: The serial device is not functioning correctly
//Description:
EFI_STATUS SetSerialPortAttrib(UINT64 BaudRate, EFI_PARITY_TYPE Parity,UINT8 DataBits, EFI_STOP_BITS_TYPE StopBits)
{
	return gSerialIO->SetAttributes(gSerialIO,BaudRate,0,0,Parity,DataBits,StopBits);
}