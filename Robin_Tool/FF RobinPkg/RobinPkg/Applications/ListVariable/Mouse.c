//member functions of mouse
//code by luobing 2013-5-17 13:10:35
#include "Mouse.h"
extern EFI_SIMPLE_POINTER_PROTOCOL *gMouse;



//  ====== debug function ==============

//Name: DisplaySerialMode
//Input: gMouse
//Output: None
//Description:
EFI_STATUS DisplayMouseMode(void)
{
	Print(L"Print Current Mode of Mouse:\n");
	Print(L"::ResolutionX=0x%x\n",gMouse->Mode->ResolutionX);
	Print(L"::ResolutionY=%d\n",gMouse->Mode->ResolutionY);
	Print(L"::ResolutionZ=%d\n",gMouse->Mode->ResolutionZ);
	Print(L"::LeftButton=%d\n",gMouse->Mode->LeftButton);
	Print(L"::RightButton=%d\n",gMouse->Mode->RightButton);
	
	return EFI_SUCCESS;
}



