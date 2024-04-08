//Smbus.h:
//Code by luobing 2013-11-27 12:45:50
#ifndef SMBUS_H
#define SMBUS_H

#include "Common.h"

//*************************** Function Declaration ***************************************
UINT8 ReadSmbusByte(IN UINT8 SlaveAddress,IN UINT8 RegisterIndex);
VOID WriteSmbusByte(IN UINT8 SlaveAddress,IN UINT8 RegisterIndex,IN UINT8 Value);



#endif