//TalkCard.h:
//Code by luobing 2011/9/8 14:47:23
#ifndef TALKCARD_H
#define TALKCARD_H
#include "Common.h"


#define INTEL_PM1_CNT	 		0x04									//PM1_CNT register

#define SY_SMB_ADDRESS		0xf0							//SY card smbus address
#define SY_MAIN_INDEX     0x00							//main register index
#define SY_FLAG_INDEX			0x10			        //flag register index

#define SY_MB_INDEX						0x02					//Mainboard
#define SY_MAIN_VERSION_INDEX 0x01					//Main version
#define SY_SUB_VERSION_INDEX	0x03					//Sub version
#define SY_OEM								0x04					//OEM,such as lenovo¡¢founder...

//*************************** Function Declaration ***************************************
VOID ShutDown(UINT16 Pmbase,UINT16 Pm1_cnt);
VOID Delayms(IN UINTN microseconds);

VOID SendMainCommand(UINT8 cmd);
UINT8 GetMainState(VOID);
VOID SendFlagCommand(UINT8 cmd);
UINT8 GetFlagState(VOID);

BOOLEAN GetSwitchFlag(VOID);
VOID SetSwitchFlag(VOID);
VOID ClrSwitchFlag(VOID);

UINT8 GetNWState(VOID);
VOID SetNWState(UINT8 flag);
BOOLEAN TestNWExist(VOID);

EFI_STATUS GetPMBASE(VOID);
/****** DO NOT WRITE BELOW THIS LINE *******/

#endif