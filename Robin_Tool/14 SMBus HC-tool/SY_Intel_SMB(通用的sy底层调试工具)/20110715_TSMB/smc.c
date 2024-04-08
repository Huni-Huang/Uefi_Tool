//luobing 2009-11-10 9:09 Q45 TOOL

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
char  HelpContext[] = \
"  Syntax is:  smc [options]\n"
"   /V        Version control	    \n"
"   /? or /H  Display Help file\n"
"   /M or /m  Display MCU spec."
"   Example: smc.exe /?\n"
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

extern unsigned char I2C_Read(unsigned char DeviceID,unsigned char RegIndex);
extern void I2C_Write(unsigned char DeviceID,unsigned char RegIndex,unsigned char value);
extern void Delay10Ms(void);
extern unsigned char ASC2HEX(unsigned char ch);
void SendCmd(unsigned char cmd);
void	main(int argc,char *argv[])
{
	unsigned char ch;
	unsigned char alx;
	int i;
	unsigned char McuSpec;
	
	if(argc>1)
  {
    for(i=1;i<argc;i++)
    {
      if((argv[i][0] == '/')||(argv[i][0] == '-'))
      {
        switch(argv[i][1])
        {
          case '?':
          case 'h':
          case 'H':
                printf(HelpContext);
                break;
          case 'V':
          case 'v':
          	  printf("smc ver2.0 for Q45  2009-11-10 9:28"); 
          	       
          	  break;
          case 'M':
          case 'm':
          //获取单片机所用的协议号
          McuSpec=I2C_Read(0x0f0,0x01);
          //McuSpec=0xFF;
          printf("Mcu spec byte: 0x%x\n",McuSpec);
          printf("Mcu Version:   %d.%02d",(unsigned char)(McuSpec>>6),(unsigned char)(McuSpec&0x03f));
          	break;
          default:
              break;        
        }
      }
    }
  }
	else
	{
		printf("Debug tool for Q45 SY --luobing\n");
		printf("Please input command number,'FF' key to exit.\n\n");
		ch=0;
		while(ch!=0xff)
		{
			printf("Cmd(0x00~0xff)= 0x");
			scanf("%x",&ch);
			SendCmd(ch);
			Delay10Ms();
			alx=I2C_Read(0xf0,0x00);
			printf("   SmbusCmd= 0x%02x",ch);
			printf("   Return= 0x%02x\n",alx);     
		}
	}
}
void SendCmd(unsigned char cmd)
{
	//发送命令：当前只发送slave address=0xf0 ,register index=0
	I2C_Write(0x0f0,00,cmd);
	Delay10Ms();
}
