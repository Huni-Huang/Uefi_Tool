//Member functions of keyboard
//code by luobing 2013-3-19 10:52:38

#include "Keyboard.h"

extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;

//===========================================Keyboard function begin====================================
//Name:  GetKeyEx
//Input:
//Output:
//Descriptor: 
EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)
{
	EFI_STATUS                        Status;
  EFI_KEY_DATA                      KeyData;
 	UINTN															Index;
	  
	gBS->WaitForEvent(1,&(gSimpleTextInputEx->WaitForKeyEx),&Index);
	Status = gSimpleTextInputEx->ReadKeyStrokeEx(gSimpleTextInputEx,&KeyData);
	if(!EFI_ERROR(Status))
	{
 		*ScanCode=KeyData.Key.ScanCode;
		*UniChar=KeyData.Key.UnicodeChar;
		*ShiftState=KeyData.KeyState.KeyShiftState;
		*ToggleState=KeyData.KeyState.KeyToggleState;
		return EFI_SUCCESS;
	} 
	return Status;
}
//Function Name: FlushKeyBuffer
//Input: none
//Output: none
//Description: Flush all keyboard buffer
VOID FlushKeyBuffer(VOID)
{
  EFI_INPUT_KEY     Key;
 
  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
}
//Function Name: WaitKey
//Input: none
//Output: none
//Description: Wait any key press
VOID WaitKey(VOID)
{
  EFI_INPUT_KEY     Key;
  UINTN Index;

  // flush keyboard buffer
  while ( gST->ConIn->ReadKeyStroke(gST->ConIn,&Key) == EFI_SUCCESS );
  gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return;
}
//Function Name: GetKey
//Input: none
//Output: none
//Description: Get key code,return unicode code and scan code(such as F1...)
EFI_STATUS GetKey(EFI_INPUT_KEY *key)
{
	UINTN Index;
	gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &Index );
  return gST->ConIn->ReadKeyStroke(gST->ConIn,key);  
}
//Function Name: GetUserKey
//Input: none
//Output: Unicode Code 
//Description: Get key code,just return unicode code
CHAR16 GetUserKey(VOID)
{
   EFI_INPUT_KEY      Key;
   gST->ConIn->ReadKeyStroke(gST->ConIn,&Key);
   return Key.UnicodeChar;
}
//Function Name: CheckKeyEvent
//Input: 
//Output: EFI_SUCCESS: 发生
//				EFI_NOT_READY: 没有发生
//Description: 检测是否有键盘事件发生
EFI_STATUS CheckKeyEvent(VOID)
{
	return gBS->CheckEvent( gST->ConIn->WaitForKey);
}
//Function Name: GetKeyState
//Input: none
//Output: key
//Description: Get key code,return unicode code and scan code(such as F1...)
//注意，配合CheckKeyboardEvent之类的函数使用
EFI_STATUS GetKeyState(EFI_INPUT_KEY *key)
{
  return gST->ConIn->ReadKeyStroke(gST->ConIn,key);  
}

//Function Name: iSimpleGetKeyIn
//Input:         None
//Output:        0~255
//Description:   Get digital from 0~255
UINT8  iSimpleGetKeyIn(VOID)
{
	UINT8 userDigit,input=0;	//
	CHAR16	ch=0;
	UINT8 count=0;	//Only receive 2 char
//	CHAR16 buffer[0x20];
	
	while(ch!=0x0d)	//Enter
	{
		WaitKey();
		ch=GetUserKey();
		
		if(count<2)
		{
			if(HexToDigit(&userDigit,ch))
			{
				input=((input<<(count*4)) &0x0f0);
				input|=userDigit;
//				MemSet(buffer,0x20, 0x00);
//				Swprintf(buffer,L"%x",userDigit);
				Print(L"%x",userDigit);
//				gST->ConOut->OutputString(gST->ConOut,buffer);
				++count;
			}
			
		}
	}
	return input;
}

//Function Name: HexToDigit
//Inout:  Digit - Pointer to byte that receives the value of the hex character.
//        Char  - Unicode character to test.
//Output:     TRUE  - If the character is a hexadecimal digit.
//            FALSE - Otherwise.
//Description: Determines if a Unicode character is a hexadecimal digit.
//             The test is case insensitive.
BOOLEAN HexToDigit (
  OUT UINT8      *Digit,
  IN  CHAR16      Ch )
{
  if ((Ch >= L'0') && (Ch <= L'9')) {
    *Digit = (UINT8) (Ch - L'0');
    return TRUE;
  }

  if ((Ch >= L'A') && (Ch <= L'F')) {
    *Digit = (UINT8) (Ch - L'A' + 0x0A);
    return TRUE;
  }

  if ((Ch >= L'a') && (Ch <= L'f')) {
    *Digit = (UINT8) (Ch - L'a' + 0x0A);
    return TRUE;
  }

  return FALSE;
}
