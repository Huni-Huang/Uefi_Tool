//TalkCard.c: talk to sy card 
//Code by luobing 2011/9/8 14:45:31

#include "Common.h"
#include "Smbus.h"
#include "TalkCard.h"
#include "PciRW.h"

//extern EFI_BOOT_SERVICES   *gBS;           //Pointer to boot services
//UINT16 PMBASE_ADDR=PM_BASE_ADDRESS;  //define token.h(ami vbe);
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
UINT16 PMBASE_ADDR=0x1800;		//for h81
//========================= ... function ========================================
//函数名: GetNWState
//输入参数: None
//输出参数: 1=外网 0=内网
//描述: 获取当前隔离卡状态
UINT8 GetNWState(VOID)
{
//	return 1;   //外网
	return 0;	//内网
}

//Function Name: SetNWState
//Input: 1:Go to Outter  0: Go to Inner
//Output: None
//Description: 
VOID SetNWState(UINT8 flag)
{
  if(flag)
    SendMainCommand(0x07);
  else
    SendMainCommand(0x06);
  SendMainCommand(0x00);
  SendMainCommand(0x0A);
  SendMainCommand(0x00);
}

//Function Name: TestNWExist
//Input: None
//Output: TRUE=exist,Flase=not exist
//Description: wether the other system is exist or not
BOOLEAN TestNWExist(VOID)
{
  UINT8 value;
  
  SendMainCommand(0x05);    //get nw state,inner or outter
  value=GetMainState();
  if(value)     //Outter
  {
    SendMainCommand(0x11);
    SendMainCommand(0x03);
    value=GetMainState();
    SendMainCommand(0x00);
    if(value)
      return TRUE;
    else
      return FALSE;
  }
  else          //Inner
  {
    SendMainCommand(0x10);
    SendMainCommand(0x09);
    value=GetMainState();
    SendMainCommand(0x00);
    if(value)
      return TRUE;
    else
      return FALSE;
  }
}
  
//Function Name: GetSwitchFlag
//Input: None
//Output: TRUE=flag has set,Flase=flag not set
//Description: 
BOOLEAN GetSwitchFlag(VOID)
{
  UINT8 value;
  
  //read State of D7
  SendMainCommand(0x0B);
  value=GetMainState();
  SendMainCommand(0x00);
  if(value)
    return TRUE;
  else
    return FALSE;
}
//Function Name: SetSwitchFlag
//Input: None
//Output: None
//Description: set switch flag 
VOID SetSwitchFlag(VOID)
{
  //Set State of D7
  SendMainCommand(0x0D);
  SendMainCommand(0x00);
}
//Function Name: ClrSwitchFlag
//Input: None
//Output: None
//Description: clear switch flag 
VOID ClrSwitchFlag(VOID)
{
  //Clear State of D7
  SendMainCommand(0x0C);
  SendMainCommand(0x00);
}
//Function Name: GetPMBASE
//Input:none
//Output: PMBASE_ADDR
EFI_STATUS GetPMBASE(VOID)
{
	EFI_STATUS status;
	UINT16 pcibuff[128];	//256字节的配置空间
	
	status=GetPciConfigurationSpace(gPCIRootBridgeIO,(UINT8 *)pcibuff,0,31,0);
	if (EFI_ERROR (status)) 
	{
		return status;
	}
	else
	{
		PMBASE_ADDR=(pcibuff[32] &0x0fffe);	//clear bit0
		return EFI_SUCCESS;
	}
	
}
//Function Name: ShutDown
//Input: Pmbase,Pm1_cnt
//Output: Value
//Description: if there is no other OS exist,shutdown,otherwise,suspend
void ShutDown(UINT16 Pmbase,UINT16 Pm1_cnt)
{
  UINT16 value;
  //if other os is exist..
  if(TestNWExist()) //exist
  {
    //
    SendMainCommand(0x02);
    GetMainState();
    value=IoRead16(Pmbase+Pm1_cnt);
    value=((value|0x3400)&0xf7ff);
    IoWrite16(Pmbase+Pm1_cnt,value);
  }
  else //not exist
  {
    //otherwise
    value=IoRead16(Pmbase+Pm1_cnt);
    IoWrite16(Pmbase+Pm1_cnt,(value|0x3c00));
  }
}
//move to common.c
////Function Name: Delayms
////Input: ms-milliseconds
////Output: none
////Description: delay by microsecond
//VOID Delayms(IN UINTN ms)
//{
//  gBS->Stall(ms*1000);
//}
//================================== Talk to all register of sy device =======================
//Function Name: SendMainCommand
//Input: cmd(00~ff) refer to sy programmer manual
//Output: none
//Description: send cmd to sy device,register index=SY_MAIN_INDEX
VOID SendMainCommand(UINT8 cmd)
{
  WriteSmbusByte(SY_SMB_ADDRESS,SY_MAIN_INDEX,cmd);
  Delayms(10);  //delay,until mcu finish his work
}
//Function Name: GetMainState
//Input: 1 or 0
//Output: none
//Description: Get main register index state
UINT8 GetMainState(VOID)
{
  if(ReadSmbusByte(SY_SMB_ADDRESS,SY_MAIN_INDEX))
    return 1;
  else
    return 0;
}

//Function Name: SendFlagCommand
//Input: cmd(00~ff) refer to sy programmer manual
//Output: none
//Description: send cmd to sy device,register index=SY_FLAG_INDEX
VOID SendFlagCommand(UINT8 cmd)
{
  WriteSmbusByte(SY_SMB_ADDRESS,SY_FLAG_INDEX ,cmd);
  Delayms(10);  //delay,until mcu finish his work
}
//Function Name: GetFlagState
//Input: 1 or 0
//Output: none
//Description: Get flag register index state
UINT8 GetFlagState(VOID)
{
  if(ReadSmbusByte(SY_SMB_ADDRESS,SY_FLAG_INDEX))
    return 1;
  else
    return 0;
}