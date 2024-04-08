//MainMenu.c: main routine
//Code by luobing 2013-11-27 14:12:05

#include "Smbus.h"
#include "Keyboard.h"
#include "Graphic.h"

#include "Font.h"
#include "Logo.h"
#include "Window.h"

#include "TalkCard.h"
//#include "Version.h"
//#include "SafeModule.h"
#include "MainMenu.h"

#define STR_CNT_X CHARWIDTH*4
#define  STR_CNT_Y SY_SCREEN_HEIGHT-CHARHEIGHT*2

extern UINT16 PMBASE_ADDR;
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern UINT32 DualNetModeNum;
UINT8 str_sywai[]={0x31,0x2e,0x20,0xbd,0xf8,0xc8,0xeb,0xcd,0xe2,0xcd,0xf8,0x00};
UINT8 str_synei[]={0x32,0x2e,0x20,0xbd,0xf8,0xc8,0xeb,0xc4,0xda,0xcd,0xf8,0x00};
UINT8 STR_CNT_WAI[] =  {0x31,0x35,0x20,0xc3,0xeb,0xba,0xf3,0xbd,0xf8,0xc8,0xeb,0xb5,0xb1,0xc7,0xb0,0xcd,0xf8,0x20,0x28,0xcd,0xe2,0xcd,0xf8,0x29,0x2c,0xb0,0xb4,0xc8,0xce,0xd2,0xe2,0xbc,0xfc,0xd6,0xd5,0xd6,0xb9,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x00};
UINT8 STR_CNT_NEI[] =  {0x31,0x35,0x20,0xc3,0xeb,0xba,0xf3,0xbd,0xf8,0xc8,0xeb,0xb5,0xb1,0xc7,0xb0,0xcd,0xf8,0x20,0x28,0xc4,0xda,0xcd,0xf8,0x29,0x2c,0xb0,0xb4,0xc8,0xce,0xd2,0xe2,0xbc,0xfc,0xd6,0xd5,0xd6,0xb9,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x00};



//Function Name: main
//Input: None
//Output: None
//Description: Main function
VOID main(VOID)
{
  EFI_STATUS    STATUS = EFI_SUCCESS;
  UINT8 count,Sel=0;

  UINT32 Key;
  UINT8 flag=0;
  UINT16 clickNumber;		//
  EFI_INPUT_KEY ikey={0,0};
  
  SwitchGraphicsMode(TRUE);
//  SetMyMode(SYGraphicsMode);  //init graphic mode
  SetMyMode(DualNetModeNum);  //Rongzq debug  //aaron+
  SetBKG(&(gColorTable[BKG_COLOR]));

  Sel=DisplayMainMenu();    //Current Select 1 or 2 :outter or inner
//  FlushAMIKeyExBuffer();
	FlushKeyBuffer();
  STATUS=EFI_NOT_READY;
  count=DEFAULTTIME;
  //Display counter down
  
  for(clickNumber=0;clickNumber<(DEFAULTTIME*10);clickNumber++)
  {
//  	STATUS=AnyAMIKeyExExist();
		STATUS=CheckKeyEvent();
  	if(STATUS==EFI_SUCCESS)
  		break;								//Any key pressed
  	Delayms(100);
  	if((clickNumber%10) == 0)
  	{
  		rectblock(STR_CNT_X,STR_CNT_Y,STR_CNT_X+CHARWIDTH*2,STR_CNT_Y+CHARHEIGHT,&(gColorTable[BKG_COLOR]));
  		if(Sel==1)
      	DisplayCounterTime(STR_CNT_X,STR_CNT_Y,STR_CNT_WAI,count,&(gColorTable[STR_CNT_Color]));
    	else
      	DisplayCounterTime(STR_CNT_X,STR_CNT_Y,STR_CNT_NEI,count,&(gColorTable[STR_CNT_Color]));
  		--count;			//15,14,13,12,11......
  	}
  }
  //clear count down display
  rectblock(STR_CNT_X,STR_CNT_Y,STR_CNT_X+CHARWIDTH*(sizeof(STR_CNT_WAI)),STR_CNT_Y+CHARHEIGHT,&(gColorTable[BKG_COLOR]));
  if(count!=0)  //count=0,then go to current os
  {
    while(!flag)
    {
			GetKey(&ikey);
      //Key=|0000 | 0000|  scancode|unicodechar
			Key=((UINT32)(ikey.ScanCode)<<16)  +  (UINT32)(ikey.UnicodeChar);
      switch(Key)
      {
        case MAIN_KEY_1:   //'1'
          flag=1;
          Sel=1;
          DisplayUserSel(Sel); 
          break;
        case MAIN_KEY_2:   //'2'
          flag=1;
          Sel=2;
          DisplayUserSel(Sel);
          break;
        case MAIN_KEY_ENTER: //enter
          flag=1;
          DisplayUserSel(Sel);
          break;
        case MAIN_KEY_TAB: //tab
          if(Sel==1)Sel=2;
          else Sel=1;
          DisplayUserSel(Sel);
          break;
        case MAIN_KEY_UP: //up
          Sel=1;
          DisplayUserSel(Sel);
          break;
        case MAIN_KEY_DOWN: //down
          Sel=2;
          DisplayUserSel(Sel);
          break;
//        case MAIN_KEY_V: //v
//          DisplayVersion();
//          Sel=DisplayMainMenu();	//reinit main menu
          break;
        default:
          break;
          
      }
    }
    //here,which os you want to go?
    if(Sel==1)    //user select outter	//选择外网时候的动作
    {
//      Sendmaincommand(0X05);    //Get Nw State,Inner Or Outter
//      If(Getmainstate())        //Outter
//         Return ;
//      Else											//Current Os Is Inner
//      {
//        Setswitchflag();
//        Setnwstate(1); //Go To Outter
//        Shutdown(Pmbase_Addr,Intel_Pm1_Cnt);
//      }
    }
    else  //user select inner	//选择内网时候的动作
    {
//      SendMainCommand(0x05);    //get nw state,inner or outter
//      if(GetMainState())        //Outter
//      {
//        SetSwitchFlag();
//        SetNWState(0); //go to inner
//        ShutDown(PMBASE_ADDR,INTEL_PM1_CNT);
//      }
//      else
//        return;       
    }
    
  }
}
//========================= menu function =========================================

//Function Name: DisplayMainMenu
//Input: None
//Output: 1=Outter 2=Inner
//Description: Display main menu and return current system state
UINT8 DisplayMainMenu(VOID)
{
  PutLenovoFrame(); 
  PutButton(BUTTON_LEFT_WAI,BUTTON_TOP_WAI,str_sywai,&(gColorTable[WND_WaiStr_Color]));
  PutButton(BUTTON_LEFT_NEI,BUTTON_TOP_NEI,str_synei,&(gColorTable[WND_NeiStr_Color]));  
  
//  SendMainCommand(0x05);    //get nw state,inner or outter
//  if(GetMainState())        //Outter
	if(GetNWState())	//外网
  {
     DisplayNWSelect(BUTTON_LEFT_WAI,BUTTON_TOP_WAI,str_sywai,&(gColorTable[BLACK]));
     return 1;
  }
  else
  {
    DisplayNWSelect(BUTTON_LEFT_NEI,BUTTON_TOP_NEI,str_synei,&(gColorTable[BLACK]));
     return 2;
  }
}
//Function Name: DisplayNWSelect
//Input: x,y,str=str_nei or str_wai sel_color
//Output: None
//Description:
VOID DisplayNWSelect(UINTN x,UINTN y,UINT8 *str,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *selColor)
{
  UINTN length,x1,y1;
  
  for(length=0;length<255;length++)   //max str =255
  {
    if(str[length]==0x00)
      break;
  }
  //length<<=3;         //width of ascii is 8
  x1=x+((length+4)<<3)-1;
  y1=y+DEFAULTBUTTONHEIGHT-1;
  rectangle(x-1,y-1,x1+1,y1+1,selColor);
}

//Function Name: DisplayCounterTime
//Input: x,y,str,seconds(0~99),color
//Output: None
//Description:
VOID DisplayCounterTime(UINTN x,UINTN y,UINT8 *str,UINT8 seconds,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
//  CHAR8 buff[3]={0,0,0};  
	CHAR8 buff[4]={0,0,0,0};  
  //convert seconds to ascii code and fill in str
//  Sprintf(buff,"%02d",seconds);
	AsciiSPrint(buff,3,"%02d",seconds);
  str[0]=buff[0];
  str[1]=buff[1];
  
  //Display str
  putStr(x,y,str,color,1);
}

//Function Name: DisplayUserSel
//Input: sel 1/2
//Output: None
//Description:
VOID DisplayUserSel(UINT8 sel)
{
//	PutButton(BUTTON_LEFT_WAI,BUTTON_TOP_WAI,str_sywai,&(gColorTable[WND_WaiStr_Color]));
//	PutButton(BUTTON_LEFT_NEI,BUTTON_TOP_NEI,str_synei,&(gColorTable[WND_NeiStr_Color])); 
  if(sel==2)
  {
    DisplayNWSelect(BUTTON_LEFT_NEI,BUTTON_TOP_NEI,str_synei,&(gColorTable[BLACK]));
    DisplayNWSelect(BUTTON_LEFT_WAI,BUTTON_TOP_WAI,str_sywai,&(gColorTable[WND_BG_Color]));
  }
  else
  {
    DisplayNWSelect(BUTTON_LEFT_WAI,BUTTON_TOP_WAI,str_sywai,&(gColorTable[BLACK]));
    DisplayNWSelect(BUTTON_LEFT_NEI,BUTTON_TOP_NEI,str_synei,&(gColorTable[WND_BG_Color]));
  }
  
}
