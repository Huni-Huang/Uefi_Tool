//code by luobing 2013-5-16 16:29:34
#ifndef _SERIALPORT_H
#define _SERIALPORT_H	
#include "Common.h"

EFI_STATUS DisplayAllSerialMode(void);
EFI_STATUS DisplaySerialMode(void);
EFI_STATUS SendDataToSerial(UINTN length,VOID *buffer);
EFI_STATUS GetDataFromSerial(UINTN *length,VOID *buffer);
EFI_STATUS SetSerialPortAttrib(UINT64 BaudRate, EFI_PARITY_TYPE Parity,UINT8 DataBits, EFI_STOP_BITS_TYPE StopBits);
EFI_STATUS GetSerialControlBits(UINT32 *control);
EFI_STATUS ResetSerialPort(void);
#endif
