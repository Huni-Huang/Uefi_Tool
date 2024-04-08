//为在OS层访问smbus，尝试在底层按照同样的方式先实现看看
//为了在DOS层访问SMBUS设备，特别的写了此工具，主要是为了访问神郁卡。2011-07-15 9:55:57 luobing
//代码脱胎自Q45的sybios，Intel的主板还是比较正规，应该可以通用.
//命令行:/f 输入指定地址访问
//			 /m 打印sy device 版本
//       /x 对卡进行读写的测试，读写D7
//       /h 打印帮助
//可以参照相关的神郁文档，特别是H61开始，增加了若干命令
//luobing 2011-07-15 11:23:29 : 开始作为通用的神郁底层调试工具使用，相比于前一版，
//                              SendCmd修改了，增加了register index作为参数输入
#include<io.h>
#include<stdio.h>
#include<dos.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include<bios.h>
#include<mem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<conio.h>

typedef unsigned long ULONG;
typedef unsigned int USHORT;
typedef unsigned char UCHAR;
//Macro define
#define ICH_SMB_IOREG00		0x00		// SMbus Host Status
#define ICH_SMB_IOREG02		0x02		// SMbus Host Control
#define ICH_SMB_IOREG03		0x03		// SMbus Host Command
#define ICH_SMB_IOREG04		0x04		// SMbus Xmit Slave Address
#define ICH_SMB_IOREG05		0x05		// SMbus Host Data 0

#define SMB_INT_MASK		 	0x002	    // SMbus int mask            
#define SMB_ERROR_MASK		 0x01c	  // SMbus error mask            
#define SMB_START		 	0x040	        // SMbus control - start         
#define SMB_READ		 	0x008	        // SMbus control - read             
#define SMB_ERROR_INT_MASK	 	(SMB_ERROR_MASK + SMB_INT_MASK)                 
#define SMB_CLEAR_MASK		 	0x0be	  // SMbus clear status flags

#define  IO_SC 0x0cf8   //config  space  control  address
#define  IO_DA 0x0cfc   //config  space  data  address

#define SYDEVICE_SLAVEADDR	0x0f0		//ShenYu device slave address (F320/342)
UCHAR smbRead(USHORT baseAddr,UCHAR DeviceID,UCHAR RegIndex);
void smbWrite(USHORT baseAddr,UCHAR DeviceID,UCHAR RegIndex,UCHAR value);

UCHAR smbSetSlaveAddr(USHORT baseAddr,UCHAR smbSlaveAddr);
UCHAR smbSetCommand(USHORT baseAddr,UCHAR regIndex);
void smbSendCommand(USHORT baseAddr,UCHAR cmdBits);
void smbClearStatus(USHORT baseAddr);
USHORT smbReadStatus(USHORT baseAddr);
UCHAR smbIoRegRead(USHORT baseAddr,UCHAR regOffset);
void smbIoRegWrite(USHORT baseAddr,UCHAR regOffset,UCHAR value);
ULONG GetSmbusBaseAddress(UCHAR busNum,UCHAR devNum,UCHAR funNum,UCHAR offset);

extern UCHAR READ_PORT_UCHAR(USHORT port);
extern ULONG READ_PORT_ULONG(USHORT port);
extern void WRITE_PORT_UCHAR(USHORT port,UCHAR value);
extern void WRITE_PORT_ULONG(USHORT port,ULONG value);
extern void Delay5ms(void);
void SendCmd(USHORT baseAddr,UCHAR registerIndex,UCHAR cmd);

char  HelpContext[] = \
"  Syntax is:  tsmb [options]\n"
"   /V        Version control	    \n"
"   /? or /H  Display Help file\n"
"   /M or /m  Display MCU spec.\n"
"   /F or /f  Specify Slave address and read data from this device.\n"
"   Example: tsmb.exe /?\n"
"List Command:(R:read command  W:write command)\n"
"______________________________________________________________________________\n"
"|CMD    |Meaning          |CMD    |Meaning         |CMD    |Meaning          |\n"
"|-------|-----------------|-------|----------------|-------|-----------------|\n"
"|0x00   | Stop            |0x01(R)| S3->Reset(D3)  |0x02(R)| S5->S3(X0)      |\n"
"|0x04(W)| Clear D3        |0x08(R)| CmosProtect(X2)|0x05(R)| Net Flag(D0)    |\n"
"|0x06(W)| To outer(D1)    |0x07(W)| To inner(D1)   |0x03(R)| Inner in S3?(D5)|\n"
"|0x09(R)| Outer in S3?(D6)|0x0A(W)| Wake Up Now(D2)|0x0B(R)| Read D7         |\n"
"|0x0C(W)| Clear D7        |0x0D(W)| Set D7         |0x0E(R)| S4?(Hold memory)|\n"
"|0x0F   | Reserve         |0x10(W)| Clear D5       |0x11(W)| Clear D6        |\n"
"------------------------------------------------------------------------------\n"
"                              Others are reserved";



void	main(int argc,char *argv[])
{

	USHORT baseSmbAddr;
	//调试通过的代码
//	asm int 3
//	bRet1=smbRead(0x500,0xf0,0x01);
//	bRet2=smbRead(0x500,0xf0,0x02);
//	printf("bRet1=%02x,bRet2=%02x\n",bRet1,bRet2);
//	asm int 3
//	baseSmbAddr= (USHORT)GetSmbusBaseAddress(0x0,0x1f,0x03,0x20); //g31 return 501
//	printf("baseSmbAddr=%04x\n",baseSmbAddr);
	UCHAR ch;
	UCHAR alx;
	int i,j;
	UCHAR McuSpec,iRegisterIndex;
	UCHAR userSlvAddr,userData[256];
	
	printf("Attention! This program just for Intel chipest.\n");
	baseSmbAddr= (USHORT)GetSmbusBaseAddress(0x0,0x1f,0x03,0x20);
	baseSmbAddr=(baseSmbAddr&(~0x0001));
	printf("Current chipest(0,0x1f,03,0x20): SmbusBaseAddr=%04x\n",baseSmbAddr);
	if(argc>1)
  {
    for(i=1;i<argc;i++)
    {
      if((argv[i][0] == '/')||(argv[i][0] == '-'))
      {
        switch(argv[i][1])
        {
        	case 'F':
        	case 'f':
        	//对其他地址的设备进行访问并打印，特别是针对SPD
        	printf("Please input Slave address:");
        	flushall();//clear buff
        	userSlvAddr=0;
					scanf("%x",&userSlvAddr);
					printf("User specify Address=%02x\n",userSlvAddr);
					printf("--------------------------list data------------------------\n");
					printf("   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
					for(i=0;i<256;i++)
						userData[i]=smbRead(baseSmbAddr,userSlvAddr,(UCHAR)i);
					for(i=0;i<256;i++)
					{
						if((i%16)==0)
							printf("\n%02x",i);
						printf(" %02x",userData[i]);
					}
					break;
          case '?':
          case 'h':
          case 'H':
                printf(HelpContext);
                break;
          case 'V':
          case 'v':
          	  printf("tsmb ver1.0 for ShenYu  2011-07-15 9:28"); 
          	       
          	  break;
          case 'M':
          case 'm':
          //获取单片机所用的协议号
          //按照协议，应该针对不同的神郁系列编写此处的代码，但是......实在没有时间了，就这么着吧 2011-5-19 10:18:37luobing
          McuSpec=smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x01);
          //McuSpec=0xFF;
          printf("Mcu spec byte: 0x%x\n",McuSpec);
          printf("other byte: regIndex02=0x%x\n",smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x02));
          printf("other byte: regIndex03=0x%x\n",smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x03));
          printf("other byte: regIndex04=0x%x\n",smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x04));
          printf("Mcu Version(fit for G31 or 945):   %d.%02d",(UCHAR)(McuSpec>>6),(UCHAR)(McuSpec&0x03f));
          	break;
          case 'X':
          case 'x':
          //疯狂测试
          printf("SMB base address is:0x%04x\n",baseSmbAddr);
          printf("   Press any key to continue...\n");
          getch();
          for(i=0;i<1000;i++)
          {
          	printf("NO.%04d:  ",i);
          	printf("Set D7..");
          	SendCmd(baseSmbAddr,0x00,0xd);
          	Delay5ms();Delay5ms();
          	smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x00);
          	Delay5ms();Delay5ms();
          	SendCmd(baseSmbAddr,0x00,0xb);
          	Delay5ms();Delay5ms();
          	printf("Now D7= %02x    ",smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x00));
          	printf("Clear D7...");
          	Delay5ms();Delay5ms();
          	SendCmd(baseSmbAddr,0x00,0xc);
          	Delay5ms();Delay5ms();
          	SendCmd(baseSmbAddr,0x00,0xb);
          	Delay5ms();Delay5ms();
          	printf("Now D7= %02x",smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,0x00));
          	printf("\n");
          	
          }
          	break;
          default:
              break;        
        }
      }
    }
  }
	else
	{
		printf("Debug tool for SY --luobing\n");
		printf("Please input command number,press 'FF' to exit.\n\n");
		iRegisterIndex=0;
		printf("Please input Register Index:");
		flushall();//clear buff
		scanf("%x",&iRegisterIndex);
		printf("SY device Slave address=0xf0,Specify Register Index=%02x\n",iRegisterIndex);
		printf("Begin to access...............\n");
		ch=0;
		while(ch!=0xff)
		{
			printf("Cmd(0x00~0xff)= 0x");
			ch=0;			//重新初始化
			flushall();//clear buff
			scanf("%x",&ch);
			SendCmd(baseSmbAddr,iRegisterIndex,ch);
			Delay5ms();
			Delay5ms();
			alx=smbRead(baseSmbAddr,SYDEVICE_SLAVEADDR,iRegisterIndex);
			printf("   SmbusCmd= 0x%02x",ch);
			printf("   Return= 0x%02x\n",alx);     
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////*********   Level 2  ************///////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//smbcommand: ShenYu routine
//Entry: cmd-ShenYu command (see ShenYu datasheet)
//Exit:  void
void SendCmd(USHORT baseAddr,UCHAR registerIndex,UCHAR cmd)
{
	//发送命令：当前只发送slave address=0xf0 ,register index=?
	smbWrite(baseAddr,SYDEVICE_SLAVEADDR,registerIndex,cmd);
	Delay5ms();
	Delay5ms();
}

//Read byte from smbus device
//Entry: baseAddr-smbus base address  DeviceID-smbus device ID  RegIndex- smbus register index
//Exit: byte form smbus device
UCHAR smbRead(USHORT baseAddr,UCHAR DeviceID,UCHAR RegIndex)
{
	UCHAR bRet;
	
	smbClearStatus(baseAddr);
	smbSetCommand(baseAddr,RegIndex);
	smbSetSlaveAddr(baseAddr,(DeviceID|0x01));
	smbSendCommand(baseAddr,SMB_READ);
	//罗冰的怀疑：为什么此处检测状态是没有错误则循环？
	while(!smbReadStatus(baseAddr))	//check the SMBus status
	{
		//loop if nothing there
	}
	bRet=smbIoRegRead(baseAddr,ICH_SMB_IOREG05);
	return bRet;
}
//Write byte to smbus device
//Entry:  baseAddr-smbus base address  DeviceID-smbus device ID  RegIndex- smbus register index
//        value-byte to be written
//Exit:   void
void smbWrite(USHORT baseAddr,UCHAR DeviceID,UCHAR RegIndex,UCHAR value)
{
	
	smbClearStatus(baseAddr);
	smbSetCommand(baseAddr,RegIndex);
	smbSetSlaveAddr(baseAddr,DeviceID);
	smbIoRegWrite(baseAddr,ICH_SMB_IOREG05,value);
	smbSendCommand(baseAddr,SMB_READ);
	while(!smbReadStatus(baseAddr))	//check the SMBus status
	{
		//loop if nothing there
	}
} 

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////*********   Level 1  ************///////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

//Sets the SMBus slave address
//Entry: baseAddr-smbus base address
//Exit:
UCHAR smbSetSlaveAddr(USHORT baseAddr,UCHAR smbSlaveAddr)
{
	smbIoRegWrite(baseAddr,ICH_SMB_IOREG04,smbSlaveAddr);
	return smbSlaveAddr;
}
//Sets the SMBus command register
//Entry: baseAddr-smbus base address  regIndex-byte to write to the command register(usually is registerIndex)
//Exit:  offset
UCHAR smbSetCommand(USHORT baseAddr,UCHAR regIndex)
{
	smbIoRegWrite(baseAddr,ICH_SMB_IOREG03,regIndex);
	return regIndex;
}
//Tells the SMBus to send the packet
//Entry: baseAddr-smbus base address  cmbits-Command bits
//Exit:  void
void smbSendCommand(USHORT baseAddr,UCHAR cmdBits)
{
	UCHAR bWrite,bRead,bBuf;
	bWrite=(cmdBits&(0x07<<2));			//only command bits
	bRead=smbIoRegRead(baseAddr,ICH_SMB_IOREG02);
	bBuf=(bRead&(~(0x07<<2)));			//clear command bits
	bBuf=(bBuf|SMB_START);
	bWrite=(bBuf|bWrite);
	smbIoRegWrite(baseAddr,ICH_SMB_IOREG02,bWrite);
}

//Clears the SMBus status register
//Entry:    baseAddr-smbus base address
//Exit:     void
void smbClearStatus(USHORT baseAddr)
{
	smbIoRegWrite(baseAddr,ICH_SMB_IOREG00,SMB_CLEAR_MASK);
}
//Reads the SMBus status register
//Entry:    baseAddr-smbus base address
//Exit:     1: an error or interrupt occured
//          0: nothing happened
USHORT smbReadStatus(USHORT baseAddr)
{
	UCHAR bRead;
	USHORT bRet;
	bRead=smbIoRegRead(baseAddr,ICH_SMB_IOREG00);
	if(bRead&SMB_ERROR_INT_MASK)
		bRet=1;
	else
		bRet=0;
	return bRet;
}

//Read a register in I/O space
//Entry:    baseAddr-smbus base address    regOffset-register offset
//Exit:     register value
UCHAR smbIoRegRead(USHORT baseAddr,UCHAR regOffset)
{
	USHORT port;
	UCHAR bRead;
	port=baseAddr+((USHORT)regOffset);
	bRead=READ_PORT_UCHAR(port);
	return bRead;
}
//Write to a register in I/O space
//Entry:    baseAddr-smbus base address    regOffset-register offset  value-value to be written
//Exit:     void
void smbIoRegWrite(USHORT baseAddr,UCHAR regOffset,UCHAR value)
{
	USHORT port;
	port=baseAddr+((USHORT)regOffset);
	WRITE_PORT_UCHAR(port,value);
}

//GET Smbus base Address
//Entry: busnumber,device number, function number, offset
//Exit:  smbaseAddr
ULONG GetSmbusBaseAddress(UCHAR busNum,UCHAR devNum,UCHAR funNum,UCHAR offset)
{
	ULONG dataAddress,baseAddress;
	
	dataAddress=0x80000000l+0x10000l*(ULONG)busNum+0x800l*(ULONG)devNum+0x100l*(ULONG)funNum+1l*(ULONG)offset;
//	printf("GetSmbusBaseAddress:dataAddress=%08lx\n",dataAddress);
	WRITE_PORT_ULONG(IO_SC,dataAddress);
	baseAddress=READ_PORT_ULONG(IO_DA);
	
	return baseAddress;
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////*********   Level 0  ************///////////////////////////
///////////////////////////*********            ************///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////从指定寄存器中读出值-字节
//UCHAR READ_PORT_UCHAR(USHORT port)
//{
//	UCHAR bRet;
//	asm{
//    mov dx,port
//	  in  al,dx
//	  mov bRet,al
//	  out	0ebh,al
//		out	0ebh,al
//	}
//	
//	return bRet;
//}
////从指定寄存器中读出值-双字(未完成)
//ULONG READ_PORT_ULONG(USHORT port)
//{
//	ULONG bRet;
//	asm{
//		mov dx,port
//		in	eax,dx
//		mov	bRet,eax
//		out 0ebh,al
//		out 0ebh,al
//	}
//}
////写入字节到指定寄存器
//void WRITE_PORT_UCHAR(USHORT port,UCHAR value)
//{
//	asm{
//		mov	dx,port
//		mov	al,value
//		out dx,al
//		out	0ebh,al
//		out	0ebh,al
//	}	
//	return ;
//}
////写入双字到指定寄存器（未完成）
//void WRITE_PORT_ULONG(USHORT port,ULONG value)
//{
//	asm{
//		mov	dx,port
//		mov	eax,value
//		out dx,eax
//		out	0ebh,al
//		out	0ebh,al
//	}	
//	return ;
//}
///////////////////////////////////////////////////////////////////////

