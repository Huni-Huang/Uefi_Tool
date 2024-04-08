//code by luobing 2013-3-19 10:54:39

#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include "Common.h"

//unicode code
#define KEY_1																			0x00000031
#define KEY_2																			0x00000032
#define KEY_UP
#define KEY_DOWN
#define KEY_TAB
//shitf and alt state
#define EFI_SHIFT_STATE_VALID                     0x80000000 
#define EFI_RIGHT_SHIFT_PRESSED                   0x00000001 
#define EFI_LEFT_SHIFT_PRESSED                    0x00000002 
#define EFI_RIGHT_CONTROL_PRESSED                 0x00000004 
#define EFI_LEFT_CONTROL_PRESSED                  0x00000008 
#define EFI_RIGHT_ALT_PRESSED                     0x00000010 
#define EFI_EFI_LEFT_ALT_PRESSED                  0x00000020 
#define EFI_RIGHT_LOGO_PRESSED                    0x00000040 
#define EFI_LEFT_LOGO_PRESSED                     0x00000080 
#define EFI_MENU_KEY_PRESSED                      0x00000100 
#define EFI_SYS_REQ_PRESSED                       0x00000200 

EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState);
EFI_STATUS GetKey(EFI_INPUT_KEY *key);
VOID FlushKeyBuffer(VOID);
VOID WaitKey(VOID);

EFI_STATUS GetKeyState(EFI_INPUT_KEY *key);
EFI_STATUS CheckKeyEvent(VOID);


CHAR16 GetUserKey(VOID);
UINT8  iSimpleGetKeyIn(VOID);
//BOOLEAN HexToDigit (
//  OUT UINT8      *Digit,
//  IN  CHAR16      Ch );

#endif