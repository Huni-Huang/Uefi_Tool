//2009-12-16 luobing ������������dos��exe�ļ��Ĵ���
//Ҫ��  1 ����������DOS�µļ��軷������PSP
//				2 ������벻��ʹ���κ�DOS�����µ��ж�
//        3 �����еĴ��벻ʹ���κ�DOS�ж�
//				4 ���еĴ�����Բ�����DOS����

//�����ܣ�1 �����������λ��,�����ֲ�Ļ�������0x10000��
//					2 �������е�λ��,������ŵ�0x4000:0��
//					3 �������룬���ض�λ����תִ��
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
  unsigned int signature; 												//'MZ' DOS����ͷ
  unsigned int bytes_in_last_block;								//�ļ����� mod512������
  unsigned int blocks_in_file;										//�ļ�������,/512����
  unsigned int num_relocs;												//�ض�λ��
  unsigned int header_paragraphs;									//�ļ��ײ����ȣ��Խ�Ϊ��λ��ÿ��16�ֽ�
  unsigned int min_extra_paragraphs;							//�����ļ�������С����
  unsigned int max_extra_paragraphs;							//�����ļ�����������
  unsigned int ss;																//SS ��ջ�Σ��Խ�Ϊ��λ���ӳ���������ʼ��Ԫ����
  unsigned int sp;																//SP ��ջƫ��
  unsigned int checksum;													//�ļ��������ֵ�У�鲹��
  unsigned int ip;																//IP
  unsigned int cs;																//CS
  unsigned int reloc_table_offset;								//�ض����λ�ã����ļ���ʼ��Ԫ����
  unsigned int overlay_number;										//���Ǻţ��ɱ䱣����
}EXE;

struct exeReloc {
  unsigned int offset;														//�ض���� ��ַƫ��
  unsigned int segment;														//�ض���� ��ƫ��
}EXERELOC;

#define DESTMEM		0x4000				//�ƶ������ڴ�λ��
//#define DESTSEG		0x4000					//��ӦDESTMEM
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
	unsigned int flag=0;				//1:�о��ļ�ͷ  2:�о��ض�λ�� 4������
	unsigned int never=0;
	char name[14]={'L','U','O','.','e','x','e',0} ;
	
	//1 �ʷ���������ȡ�û���Ҫ�Ĳ���
	if(argc>1)
  {
  	
  	for(i=1;i<argc;i++)
  	{
//  		printf("argc1=%d\n",argc);
  	  if((argv[i][0] == '/')||(argv[i][0] == '-'))			//��¼����ѡ��
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
  	  		if(strstr(name,".exe")==NULL)		//��ȫ.exe
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
	//2 ����Ҫ����
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
//�оٵ�ǰEXE�ļ�����Ϣ
//��ڲ�����exe�ļ���
//���ڲ�������
void listEXE(char *name)
{
	FILE *fp;
	
	if((fp=fopen(name,"rb"))==NULL)
	{
		printf("Open exe file %s error!",name);
		exit(1);
	}
	//1 �оٵ�ǰexe �Ľṹ
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
//�о��ض�λ��
//��ڲ�����exe�ļ���
//���ڲ�������
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
		k=((num-12)/(4*20));			//ʣ�µ���20��һ����ʾ
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
		j=((num-12) % (4*20));			//mod һ�����ʣ��
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
//��ȡexe�ļ����ڴ�
//��ڲ����� �ļ���
//���ڲ����� �Ƿ�ɹ��ı�־��0����ʾ�ɹ���������ʧ��
//˵������Ϊ�Ժ����foxdisk ��ԭ�ͳ����������ȶ���һ����ŵĿ��
//      �������Ŀ����У�����1 ���ļ����о���У�飬2 �ɵ������ڴ�λ�úʹ��λ��
//			�˺�����������ص�DESTMEM+0x100�����С�
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
	pText=(unsigned long)(iexe.header_paragraphs * 16);														//�����������ļ��е�λ��
	lenText=(512l*(iexe.blocks_in_file-1))+(1l* iexe.bytes_in_last_block)-pText;	//����ռ�õ��ڴ��С
	resSeg=(unsigned int)((lenText+0x100l)%65536l);
	numSeg=(unsigned int)((lenText+0x100l)/65536l);
	if(resSeg)
		numSeg+=1;
//	printf("pText=0x%.8lx\n",pText);
//	printf("Len of Text=0x%.8lx\n",lenText);
//	printf("num Reloc: 0x%.4x\n",iexe.num_relocs);
//	printf("numSeg: 0x%.4x, resSeg: 0x%.4x\n",numSeg,resSeg);

	//1 ��������ص�destmem
	fseek(fp,pText,0);			//�����������ļ��е�λ��
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
		resSeg-=0x100;				//��Ϊ����������ƶ���0x100
		while(resSeg)
		{
			destmem[i]=(unsigned char)(getc(fp));
			--resSeg;
			++i;
		}
	}
	//2 �޸��ض�λ��
	//���ڣ����������DESTMEM:100���ˣ���ʼ�޸��ض�λ��
	fseek(fp,(unsigned long)(iexe.reloc_table_offset),0);		//��λ
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
	//3 �涨DS SS SP��Զ��ת
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
//��ȡexe�ļ����ڴ�
//��ڲ����� �ļ���
//���ڲ����� �Ƿ�ɹ��ı�־��0����ʾ�ɹ���������ʧ��
//˵������Ϊ�Ժ����foxdisk ��ԭ�ͳ����������ȶ���һ����ŵĿ��
//      �������Ŀ����У�����1 ���ļ����о���У�飬2 �ɵ������ڴ�λ�úʹ��λ��
//			�˺�����������ص�DESTMEM�����С�
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

	pText=(unsigned long)(iexe.header_paragraphs * 16);														//�����������ļ��е�λ��
	lenText=(512l*(iexe.blocks_in_file-1))+(1l* iexe.bytes_in_last_block)-pText;	//����ռ�õ��ڴ��С
	resSeg=(unsigned int)((lenText)%65536l);
	numSeg=(unsigned int)((lenText)/65536l);
	if(resSeg)
		numSeg+=1;

	//1 ��������ص�destmem
	fseek(fp,pText,0);			//�����������ļ��е�λ��
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
//		resSeg-=0x100;				//��Ϊ����������ƶ���0x100
		while(resSeg)
		{
			destmem[i]=(unsigned char)(getc(fp));
			--resSeg;
			++i;
		}
	}
	//2 �޸��ض�λ��
	//���ڣ����������DESTMEM���ˣ���ʼ�޸��ض�λ��
	fseek(fp,(unsigned long)(iexe.reloc_table_offset),0);		//��λ
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
	//3 �涨DS SS SP��Զ��ת
	myds=DESTMEM;
	myss=DESTMEM+iexe.ss;
	mysp=iexe.sp;
	mycs=DESTMEM+iexe.cs;
	myip=iexe.ip;
	bootEXE(mycs,myip,myds,myss,mysp);
	return 0;
}
//��ת����exe�ļ�
//��ڲ������µ�CS DS SS IP SP
//���ڲ�������
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
