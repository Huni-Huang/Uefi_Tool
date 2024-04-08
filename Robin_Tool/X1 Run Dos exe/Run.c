//2009-12-16 luobing 制作可以运行dos下exe文件的代码
//要求：  1 不能依赖于DOS下的假设环境，如PSP
//				2 本身代码不能使用任何DOS环境下的中断
//        3 所运行的代码不使用任何DOS中断
//				4 运行的代码可以不返回DOS环境

//程序框架：1 本身代码所在位置,如果移植的话，放在0x10000处
//					2 程序运行的位置,将程序放到0x4000:0处
//					3 分析代码，并重定位，跳转执行
#include<io.h>
#include<stdio.h>
#include<dos.h>
#include<string.h>
#include<math.h>
#include<bios.h>
#include<mem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<conio.h>

typedef struct exeStruc {
  unsigned int signature; 												//'MZ' DOS程序头
  unsigned int bytes_in_last_block;								//文件长度 mod512的余数
  unsigned int blocks_in_file;										//文件扇区数,/512的商
  unsigned int num_relocs;												//重定位项
  unsigned int header_paragraphs;									//文件首部长度，以节为单位，每节16字节
  unsigned int min_extra_paragraphs;							//加载文件所需最小节数
  unsigned int max_extra_paragraphs;							//加载文件所需最大节数
  unsigned int ss;																//SS 堆栈段，以节为单位，从程序主体起始单元算起
  unsigned int sp;																//SP 堆栈偏移
  unsigned int checksum;													//文件中所有字的校验补码
  unsigned int ip;																//IP
  unsigned int cs;																//CS
  unsigned int reloc_table_offset;								//重定项表位置，从文件起始单元算起
  unsigned int overlay_number;										//覆盖号，可变保留区
}EXE;

struct exeReloc {
  unsigned int offset;														//重定向表 地址偏移
  unsigned int segment;														//重定向表 段偏移
}EXERELOC;

#define DESTMEM		0x4000				//移动到的内存位置
//#define DESTSEG		0x4000					//对应DESTMEM
#define MAKE_FP(seg,ofs)    ((void far *)(((unsigned long)(seg) << 16) | (unsigned int)(ofs)))
void listEXE(char *name);
void listReloc(char *name);
unsigned int LoadEXE(char *name);
unsigned int LoadEXE1(char *name);
void bootEXE(unsigned int mycs,unsigned int myip,unsigned int myds,unsigned int myss,unsigned int mysp);
//Syntax: Run xxx.exe -l
void 	main(int argc,char * argv[])
{
	int i;
	unsigned int flag=0;				//1:列举文件头  2:列举重定位表 4：运行
	unsigned int never=0;
	char name[14]={'L','U','O','.','e','x','e',0} ;
	
	//1 词法分析，获取用户需要的操作
	if(argc>1)
  {
  	
  	for(i=1;i<argc;i++)
  	{
//  		printf("argc1=%d\n",argc);
  	  if((argv[i][0] == '/')||(argv[i][0] == '-'))			//记录操作选择
  	  {
  	    switch(argv[i][1])
  	    {
  	    	case 'l':
  	    	case 'L':
  	    		flag|=0x01;
  	    		break;
  	    	case 't':
  	    	case 'T':
  	    		flag|=0x02;
  	    		break;
  	    	case 'r':
  	    	case 'R':
  	    		flag|=0x04;
  	    		break;
  	    	default:
  	    		break;
  	    }
  	  }
  	  else
  	  {	
  	  	if(never==0)
  	  	{
  	  		never=1;
  	  		memset(name,0,14);
//					printf("ss: %s\n",argv[i]);
					if(strlen(argv[i])<14)
						strcpy(name,argv[i]);
					else
  	  			memcpy(name,argv[i],13);
//  	  		printf("ss: %s\n",argv[i]);  		
  	  		if(strstr(name,".exe")==NULL)		//补全.exe
  	  		{
  	  			if(strlen(name)>=8)
  	  			{
  	  				name[8]='.';
  	  				name[9]='e';
  	  				name[10]='x';
  	  				name[11]='e';
  	  				name[12]='\0';
  	  			}
  	  			else strcat(name,".exe");
  	  		}
  	  	}
  	  }
  	}
  }
//  printf("flag=%.4x,name=%s",flag,name);		 
	//2 按照要求做
	printf("Run.exe: Analyse DOS EXE file.\n");
	if(never==1)
		printf("  Input EXE File: %s\n\n",name);
	else
	{
		printf("Syntax Error--by run.exe!");
		exit(1);
	}
	if(flag&0x01)
		listEXE(name);
	
	if(flag&0x02)
		listReloc(name);	   
	if(flag&0x04)
		LoadEXE(name); 
}
//列举当前EXE文件的信息
//入口参数：exe文件名
//出口参数：无
void listEXE(char *name)
{
	FILE *fp;
	
	if((fp=fopen(name,"rb"))==NULL)
	{
		printf("Open exe file %s error!",name);
		exit(1);
	}
	//1 列举当前exe 的结构
	printf("DOS EXE Header list:\n");
	printf("  DOS Header signature(0-1):        0x%.4x\n",getw(fp));
	printf("  Exe File mod 512(2-3):            0x%.4x\n",getw(fp));
	printf("  Exe File blocks(4-5):             0x%.4x\n",getw(fp));
	printf("  Relocation Numbers(6-7):          0x%.4x\n",getw(fp));
	printf("  Header paragraphs(8-9):           0x%.4x\n",getw(fp));
	printf("  Min memory(/paragraphs)(A-B)      0x%.4x\n",getw(fp));
	printf("  Max memory(/paragraphs)(C-D)      0x%.4x\n",getw(fp));
	printf("  SS(E-F):                          0x%.4x\n",getw(fp));
	printf("  SP(10-11):                        0x%.4x\n",getw(fp));
	printf("  Neg checksum(12-13):              0x%.4x\n",getw(fp));
	printf("  IP(14-15):                        0x%.4x\n",getw(fp));
	printf("  CS(16-17):                        0x%.4x\n",getw(fp));
	printf("  Relocation Table offset(18-19):   0x%.4x\n",getw(fp));
	printf("  Overlay number(1A-1B):            0x%.4x\n",getw(fp));
	fclose(fp);
}
//列举重定位表
//入口参数：exe文件名
//出口参数：无
void listReloc(char *name)
{
	FILE *fp;
	unsigned int i,j,k,t,num,segment,offset;
	unsigned long pos;
	if((fp=fopen(name,"rb"))==NULL)
	{
		printf("Open exe file %s error!",name);
		exit(1);
	}
	fseek(fp,0x6l,0);
	num=(unsigned int)getw(fp);
	fseek(fp,0x18l,0);
	pos=(unsigned long)(getw(fp));
	printf("Relocation Table list:\n");
	printf(" Number of relocation = 0x%.4x\n",num);
	fseek(fp,pos,0);
	if(num<=12)
	{
		i=0;
		while(num)
		{
			offset=getw(fp);
			segment=getw(fp);
			printf("REL%.4x:0x%.4x:%.4x ",i,segment,offset);
			--num;
			++i;
//			if((num>>2)==0)	printf("\n");
		}
	}
	else
	{
		j=12;
		i=0;
		while(j)
		{
			offset=getw(fp);
			segment=getw(fp);
			printf("REL%.4x:0x%.4x:%.4x ",i,segment,offset);
			--j;
			++i;
//			if((j%4)==0)	printf("\n");
		}
		printf("Press any key to continue...\n");
		getch();
		k=((num-12)/(4*20));			//剩下的以20行一屏显示
		if(k)
		{
			for(t=0;t<k;t++)
			{
				j=80;
				while(j)
				{
					offset=getw(fp);
					segment=getw(fp);
					printf("REL%.4x:0x%.4x:%.4x ",80-j+12+t*80,segment,offset);
					--j;
//					if((j%4)==0)	printf("\n");
				}
				printf("Press any key to continue...\n");
				getch();
			}
		}
		j=((num-12) % (4*20));			//mod 一屏后的剩余
		i=0;
		while(j)
		{
			offset=getw(fp);
			segment=getw(fp);
			printf("REL%.4x:0x%.4x:%.4x ",i+12+k*80,segment,offset);
			--j;
			++i;
//			if((j%4)==0)	printf("\n");
		}
		
	}
//	printf("REL0000:0x0000:0000 REL0000:0x0000:0000 REL0000:0x0000:0000 REL0000:0x0000:00000\n");
//	printf("REL0000:0x0000:0000    REL1:0x0000:0000    REL2:0x0000:0000    REL3:0x0000:0000\n");
//	printf("REL0000:0x0000:0000    REL1:0x0000:0000    REL2:0x0000:0000    REL3:0x0000:0000\n");
	
	fclose(fp);
}
//读取exe文件到内存
//入口参数： 文件名
//出口参数： 是否成功的标志，0：表示成功，其他：失败
//说明：作为以后加载foxdisk 的原型程序，现在首先定义一个大概的框架
//      ，将来的开发中，考虑1 对文件进行镜像校验，2 可调整的内存位置和存放位置
//			此函数将代码加载到DESTMEM+0x100处运行。
unsigned int LoadEXE(char *name)
{
	FILE *fp;
	unsigned char far * destmem=(char far *)( ((unsigned long)DESTMEM)<<16 | 0);
	unsigned long pText,lenText,di;
	EXE iexe;
	unsigned char *ptr,buf[512];
	unsigned int i,j,k,numSeg,resSeg,reloc_seg,reloc_ofs,myss,mysp,myds,mycs,myip;
	unsigned int far *ptr1;
	
	if((fp=fopen(name,"rb"))==NULL)
	{
		printf("Open exe file %s error!",name);
		exit(1);
	}
	ptr=(unsigned char *)(&iexe);
	for(i=0;i<sizeof(EXE);i++)
		ptr[i]=(unsigned char)(getc(fp));

//	printf("iCode Seg: %.4x\n",_CS);
	pText=(unsigned long)(iexe.header_paragraphs * 16);														//程序正文在文件中的位置
	lenText=(512l*(iexe.blocks_in_file-1))+(1l* iexe.bytes_in_last_block)-pText;	//程序占用的内存大小
	resSeg=(unsigned int)((lenText+0x100l)%65536l);
	numSeg=(unsigned int)((lenText+0x100l)/65536l);
	if(resSeg)
		numSeg+=1;
//	printf("pText=0x%.8lx\n",pText);
//	printf("Len of Text=0x%.8lx\n",lenText);
//	printf("num Reloc: 0x%.4x\n",iexe.num_relocs);
//	printf("numSeg: 0x%.4x, resSeg: 0x%.4x\n",numSeg,resSeg);

	//1 将程序加载到destmem
	fseek(fp,pText,0);			//程序正文在文件中的位置
	if(numSeg>1)						// exceed 64k
	{
		di=0x100;
		for(j=0;j<numSeg-1;j++)
		{	
			while(di<65536)
			{
				destmem[di]=(unsigned char)(getc(fp));
				++di;
			}
			if(di>=65536)
			{		
				destmem=(char far *)( ((unsigned long)(DESTMEM+(0x1000*(j+1))))<<16 | 0);
				di=0;
			}
		}
		di=lenText+0x100-(((numSeg-1)*1l)<<16);
		j=0;
		while(di)
		{
			destmem[j]=(unsigned char)(getc(fp));
			--di;
			++j;
		}
	}
	else
	{
		i=0x100;			
		resSeg-=0x100;				//因为将程序向后移动了0x100
		while(resSeg)
		{
			destmem[i]=(unsigned char)(getc(fp));
			--resSeg;
			++i;
		}
	}
	//2 修改重定位表
	//现在，程序放在了DESTMEM:100处了，开始修改重定位表
	fseek(fp,(unsigned long)(iexe.reloc_table_offset),0);		//定位
	for(i=0;i<iexe.num_relocs;i++)
	{
		reloc_ofs=getw(fp);
		reloc_seg=getw(fp);
		destmem=MAKE_FP(DESTMEM+reloc_seg,0x100+reloc_ofs);
		ptr1=(unsigned int far *)destmem;
		j=ptr1[0];
		ptr1[0]=(j+DESTMEM+0x10);
	}
	fclose(fp);
	//3 规定DS SS SP并远跳转
	myds=DESTMEM;
	myss=DESTMEM+0x10+iexe.ss;
	mysp=iexe.sp;
	mycs=DESTMEM+0x10+iexe.cs;
	myip=iexe.ip;
	asm int 3
	bootEXE(mycs,myip,myds,myss,mysp);
	asm int 3
	return 0;
}
//读取exe文件到内存
//入口参数： 文件名
//出口参数： 是否成功的标志，0：表示成功，其他：失败
//说明：作为以后加载foxdisk 的原型程序，现在首先定义一个大概的框架
//      ，将来的开发中，考虑1 对文件进行镜像校验，2 可调整的内存位置和存放位置
//			此函数将代码加载到DESTMEM处运行。
unsigned int LoadEXE1(char *name)
{
	FILE *fp;
	unsigned char far * destmem=(char far *)( ((unsigned long)DESTMEM)<<16 | 0);
	unsigned long pText,lenText,di;
	EXE iexe;
	unsigned char *ptr,buf[512];
	unsigned int i,j,k,numSeg,resSeg,reloc_seg,reloc_ofs,myss,mysp,myds,mycs,myip;
	unsigned int far *ptr1;
	
	if((fp=fopen(name,"rb"))==NULL)
	{
		printf("Open exe file %s error!",name);
		exit(1);
	}
	ptr=(unsigned char *)(&iexe);
	for(i=0;i<sizeof(EXE);i++)
		ptr[i]=(unsigned char)(getc(fp));

	pText=(unsigned long)(iexe.header_paragraphs * 16);														//程序正文在文件中的位置
	lenText=(512l*(iexe.blocks_in_file-1))+(1l* iexe.bytes_in_last_block)-pText;	//程序占用的内存大小
	resSeg=(unsigned int)((lenText)%65536l);
	numSeg=(unsigned int)((lenText)/65536l);
	if(resSeg)
		numSeg+=1;

	//1 将程序加载到destmem
	fseek(fp,pText,0);			//程序正文在文件中的位置
	if(numSeg>1)						// exceed 64k
	{
		di=0;
		for(j=0;j<numSeg-1;j++)
		{	
			while(di<65536)
			{
				destmem[di]=(unsigned char)(getc(fp));
				++di;
			}
			if(di>=65536)
			{		
				destmem=(char far *)( ((unsigned long)(DESTMEM+(0x1000*(j+1))))<<16 | 0);
				di=0;
			}
		}
		di=lenText-(((numSeg-1)*1l)<<16);
		j=0;
		while(di)
		{
			destmem[j]=(unsigned char)(getc(fp));
			--di;
			++j;
		}
	}
	else
	{
		i=0;			
//		resSeg-=0x100;				//因为将程序向后移动了0x100
		while(resSeg)
		{
			destmem[i]=(unsigned char)(getc(fp));
			--resSeg;
			++i;
		}
	}
	//2 修改重定位表
	//现在，程序放在了DESTMEM处了，开始修改重定位表
	fseek(fp,(unsigned long)(iexe.reloc_table_offset),0);		//定位
	for(i=0;i<iexe.num_relocs;i++)
	{
		reloc_ofs=getw(fp);
		reloc_seg=getw(fp);
		destmem=MAKE_FP(DESTMEM+reloc_seg,reloc_ofs);
		ptr1=(unsigned int far *)destmem;
		j=ptr1[0];
		ptr1[0]=(j+DESTMEM);
	}
	fclose(fp);
	//3 规定DS SS SP并远跳转
	myds=DESTMEM;
	myss=DESTMEM+iexe.ss;
	mysp=iexe.sp;
	mycs=DESTMEM+iexe.cs;
	myip=iexe.ip;
	bootEXE(mycs,myip,myds,myss,mysp);
	return 0;
}
//跳转运行exe文件
//入口参数：新的CS DS SS IP SP
//出口参数：无
void bootEXE(unsigned int mycs,unsigned int myip,unsigned int myds,unsigned int myss,unsigned int mysp)
{
	asm{
		cli
		mov	ax,myds
		mov	ds,ax
		mov	es,ax
		
		mov	ax,myip
		mov	bx,myss
		mov	cx,mysp
		mov	dx,mycs

		mov	ss,bx
		mov	sp,cx
		sti
		push	dx
		push	ax
		retf
	}
}
