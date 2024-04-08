//Version.c: get sy version and test
//Code by luobing 2013-11-27 14:07:53
#include "Smbus.h"
#include "Keyboard.h"
#include "Graphic.h"

#include "Font.h"
#include "Logo.h"
#include "Window.h"

#include "TalkCard.h"
#include "Version.h"

extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];

//========================= Global String ========================================
UINT8 STR_VER1[]  = {0xb5,0xb1,0xc7,0xb0,0xb0,0xe6,0xb1,0xbe,0xba,0xc5,0x3a,0x20,0x20,0x20,0x53,0x59,0x42,0x49,0x4f,0x53,0x00};         
UINT8 STR_iVer[]  = {0x20,0x2e,0x20,0x20,0x28,0x20,0x29,0x00};
UINT8 STR_VER11[] = {0xbf,0xd8,0xd6,0xc6,0xbf,0xa8,0xb0,0xe6,0xb1,0xbe,0x3a,0x20,0x20,0x20,0x56,0x65,0x72,0x20,0x00};
UINT8 SY_CARD_V[] = {0x20,0x2e,0x20,0x20,0x28,0x20,0x29,0x00};
UINT8 STR_VER2[]  = {0xca,0xb9,0xd3,0xc3,0xd0,0xad,0xd2,0xe9,0x3a,0x20,0x20,0x20,0x20,0x20,0x48,0x38,0x31,0xb1,0xea,0xd7,0xbc,0xd0,0xad,0xd2,0xe9,0x31,0x2e,0x30,0x30,0x00};
UINT8 STR_VER21[] = {0xc5,0xe4,0xcc,0xd7,0xbc,0xec,0xb2,0xe2,0x3a,0x20,0x20,0x20,0x20,0x20,0x00};
UINT8 STR_VER22[] = {0xb7,0xfb,0xba,0xcf,0x2c,0xc8,0xed,0xbc,0xfe,0xb9,0xa4,0xd7,0xf7,0xd5,0xfd,0xb3,0xa3,0x2e,0x00};     
UINT8 STR_VER23[] = {0xb2,0xbb,0xc6,0xa5,0xc5,0xe4,0x21,0x00};
UINT8 STR_VER3[]  = {0xc7,0xeb,0xb0,0xb4,0xc8,0xce,0xd2,0xe2,0xbc,0xfc,0xbb,0xd8,0xb5,0xbd,0xd6,0xf7,0xbd,0xe7,0xc3,0xe6,0x2e,0x2e,0x2e,0x00};
UINT8 STR_VER33[] = {0xd7,0xa2,0xd2,0xe2,0x3a,0xc4,0xfa,0xca,0xb9,0xd3,0xc3,0xb5,0xc4,0xbf,0xd8,0xd6,0xc6,0xbf,0xa8,0xd3,0xeb,0x42,0x49,0x4f,0x53,0xb2,0xbb,0xc5,0xe4,0xcc,0xd7,0x2c,0xc7,0xeb,0xb8,0xfc,0xbb,0xbb,0x21,0x00};

//========================= menu and control function ========================================

//Function Name: SYVersionMenu
//Input: None
//Output: TRUE=version match FALSE=Not match
//Description: display version, if not match,jmp $
BOOLEAN SYVersionMenu(VOID)
{
	UINT8 MainBoard,MainVer,SubVer,OEM_index;
	 //Get Current Card Version
  MainBoard=ReadSmbusByte(SY_SMB_ADDRESS,SY_MB_INDEX);
  MainVer=ReadSmbusByte(SY_SMB_ADDRESS,SY_MAIN_VERSION_INDEX);
  SubVer=ReadSmbusByte(SY_SMB_ADDRESS,SY_SUB_VERSION_INDEX);
  OEM_index=ReadSmbusByte(SY_SMB_ADDRESS,SY_OEM);
  //Compare to sybios version
  if((MainBoard==SY_MainBoard) && (MainVer==SY_MainVer) && (SubVer==SY_SubVer) && (OEM_index==SY_OEMID))
	{
    return TRUE;
  }
  else
  {
  	DisplayCardVersion();
  	while(1)
  	{
    	putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*7,STR_VER33,&(gColorTable[RED]),1);  
    	Delayms(1500);
    	putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*7,STR_VER33,&(gColorTable[WND_BG_Color]),1); 
    	Delayms(1500);
    }
    return FALSE;
   } 
  
}
//Function Name: DisplayVersion
//Input: None
//Output: None
//Description: display version, Any key to continue
VOID DisplayVersion(VOID)
{
	DisplayCardVersion();
	putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*7,STR_VER3,&(gColorTable[GREEN]),1); 
	WaitKey();
	//WaitKey();	//must call twice,why?maybe press a key will trigger two event?
	//clear version menu
	rectblock(WND_FRAME_LEFT,WND_FRAME_TOP,WND_FRAME_RIGHT,WND_FRAME_BOTTOM,&(gColorTable[BKG_COLOR]));
}


//Function Name: DisplayCardVersion
//Input: None
//Output: TRUE=version match FALSE=Not match
//Description: display current card version, compare to sybios version
BOOLEAN DisplayCardVersion(VOID)
{
  UINT8 MainBoard,MainVer,SubVer,OEM,OEM_index,buff;
  UINT8 OEM_Table[]={'E','Q','F','G','L','?','?','?','?','?','?'};
//  UINT8 buff1[0x100],*pbuff1;
  UINTN length;
  
  PutLenovoFrame(); 
  //Get Current Card Version
  MainBoard=ReadSmbusByte(SY_SMB_ADDRESS,SY_MB_INDEX);
  MainVer=ReadSmbusByte(SY_SMB_ADDRESS,SY_MAIN_VERSION_INDEX);
  SubVer=ReadSmbusByte(SY_SMB_ADDRESS,SY_SUB_VERSION_INDEX);
  OEM_index=ReadSmbusByte(SY_SMB_ADDRESS,SY_OEM);
  OEM=OEM_Table[OEM_index];
  //Display sybios version
  HexToChar(&buff,SY_MainBoard);
  STR_iVer[0]=buff;
  HexToChar(&buff,SY_MainVer);
  STR_iVer[2]=buff;
  HexToChar(&buff,SY_SubVer);
  STR_iVer[3]=buff;
  STR_iVer[5]=OEM_Table[SY_OEMID];
//  MemSet(buff1,0x100,00);
//  MemCpy(buff1,STR_VER1,(sizeof(UINT8))*(sizeof(STR_VER1)-1));
//  pbuff1=buff1+sizeof(STR_VER1)-1;
//  MemCpy(pbuff1,STR_iVer,sizeof(STR_iVer)*sizeof(UINT8));
	for(length=0;length<255;length++)   //max str =255
  {
    if(STR_VER1[length]==0x00)
      break;
  }
  putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT/2,STR_VER1,&(gColorTable[RED]),1);
  putStr(TOOLTIP_LEFT+CHARWIDTH*length,TOOLTIP_TOP+CHARHEIGHT/2,STR_iVer,&(gColorTable[RED]),1);
  
  //Display card version
  HexToChar(&buff,MainBoard);
  SY_CARD_V[0]=buff;
  HexToChar(&buff,MainVer);
  SY_CARD_V[2]=buff;
  HexToChar(&buff,SubVer);
  SY_CARD_V[3]=buff;
  SY_CARD_V[5]=OEM;
//  MemSet(buff1,0x100,00);
//  MemCpy(buff1,STR_VER11,(sizeof(UINT8))*(sizeof(STR_VER11)-1));
//  pbuff1=buff1+sizeof(STR_VER11)-1;
//  MemCpy(pbuff1,STR_iVer,sizeof(SY_CARD_V)*sizeof(UINT8));
	for(length=0;length<255;length++)   //max str =255
  {
    if(STR_VER11[length]==0x00)
      break;
  }
  putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*2,STR_VER11,&(gColorTable[RED]),1);
  putStr(TOOLTIP_LEFT+CHARWIDTH*length,TOOLTIP_TOP+CHARHEIGHT*2,SY_CARD_V,&(gColorTable[RED]),1);
  //protocol
  putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*3+CHARHEIGHT/2,STR_VER2,&(gColorTable[RED]),1);
  //match test
  putStr(TOOLTIP_LEFT,TOOLTIP_TOP+CHARHEIGHT*5,STR_VER21,&(gColorTable[RED]),1);
  for(length=0;length<255;length++)   //max str =255
  {
    if(STR_VER21[length]==0x00)
      break;
  }
  if((MainBoard==SY_MainBoard) && (MainVer==SY_MainVer) && (SubVer==SY_SubVer) && (OEM_index==SY_OEMID))
	{
    putStr(TOOLTIP_LEFT+CHARWIDTH*length,TOOLTIP_TOP+CHARHEIGHT*5,STR_VER22,&(gColorTable[GREEN]),1);
    return TRUE;
  }
  else
  {
    putStr(TOOLTIP_LEFT+CHARWIDTH*length,TOOLTIP_TOP+CHARHEIGHT*5,STR_VER23,&(gColorTable[GREEN]),1);  
    return FALSE;
  } 
}