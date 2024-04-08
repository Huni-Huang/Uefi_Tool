/*exetrace hz form files*/
/*luobing 2008-10-20 12:29*/
/*程序功能: 可提取最大32个文件中由" "包含的汉字，并提取出其字模*/
/*注意:     必须保证在提取的文件中字符串"**"中没有//和/* 字符,否则可能会提取错误*/
/*luobing 2011-09-06 21:28:11
  在UEFI的环境下显示汉字，目前没有找到方法以汉字形式来定义字符串，比如"我"在AMI的
  编译环境下是显示不出来的。因此增加一个缓冲机制，每个需要显示的字符串都定义为形
  如 UINT8 Str[]={0x12,0x34,...0x00};的样子，然后再通过相应的字符串显示函数显示*/
/*为了处理上的简单，源文件中的字符串都定义为UINT8 Str[]="吾爱吾妻forever";之类的形式*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include "ver_l.h"

unsigned char getHZ(char *dstfile,char *srcfile);
unsigned char ConvertStr(char *srcfile);

char  szHelpContext[] = \
"Syntax is:  efihz.exe dst src1 src2 ...\n" 
" /V        Version control	    \n"

" /? or /H  Display Help file\n";

int	main(int argc,char *argv[])
{
	char *dstfile;
	FILE *dst,*hzlib,*src;
	struct time t;
	struct date d;
	unsigned int hzcount=0;
	int i,j;
	unsigned int k,qu,wei,zi;
	unsigned char zi0,zi1;
	int hz[2];
	long s;
	unsigned long flag=0;			/* 是否提取汉字成功的标志 bitx=1为对应的文件提取成功 */
														/* */

	if(argc==1)
		printf(szHelpContext);
	else if(argc==2)
	{
		if((argv[1][0] == '/')||(argv[1][0] == '-'))
		{
    	switch(argv[1][1])
    	{
    	  case '?':
    	  case 'h':
    	  case 'H':
    	        printf(szHelpContext);
    	        break;
    	  case 'V':
    	  case 'v':
    	  	  printf("Current Version is: V%d.%d%d\n",ReleaseVerH,MainVer,ReleaseVerL);      
    	  	  break;
    	  default:
    	  	printf ("Syntax error! Please use /? get help.");
    	      break;        
    	}
		}
	}
	else if(argc>2)
	{
		for(i=2;i<argc;i++)
		{
			//提取所有汉字到指定的文件
			printf(" %d Extract chinese words from %s:",i-1,argv[i]);
			if(!getHZ("__TEMP__.h",argv[i]))
			{
				flag|=(1l<<(i-2));			/* 设置提取成功的标志 */
				printf(" Success!\n");
			}
			else
				printf(" Failure!\n");
				
			//转换文件
			printf(" %d Convert file (uefi string) %s:",i-1,argv[i]);
			if(!ConvertStr(argv[i]))
			{
				printf(" Success!\n");
			}
			else
				printf(" Failure!\n");
		}
	}
	else printf ("Syntax error! Please use /? get help.");
	
	if(argc>2)
	{
		dstfile=argv[1];					/* 需要存放汉字库的目标文件 */
		if((hzlib=fopen("hzk16","rb"))==NULL)
		{
			printf("Can not open hzlib: hzk16,abnormal quit!");
			remove("__TEMP__.h");
			exit(1);
		}
		if((src=fopen("__TEMP__.h","r"))==NULL)
		{
			fclose(hzlib);
			printf("Can not fine temporary file,maybe your filename input error!");
			exit(1);
		}
		if((dst=fopen(dstfile,"w"))==NULL)
		{
			printf("Can not create file:%s!",dstfile);
			fclose(hzlib);
			remove("__TEMP__.h");
			exit(1);
		}
		fprintf(dst,"/* %s: Chinese words library for C  coded by luobing*/\n",dstfile);
		fprintf(dst,"/* Tool: %s v%d.%d%d*/\n",argv[0],ReleaseVerH,MainVer,ReleaseVerL);
		fprintf(dst,"/* Source files: ");
		for(i=2;i<argc;i++)
		{
			if((flag>>(i-2))&0x01)
				fprintf(dst," %s",argv[i]);
		}
		fprintf(dst,"*/\n");
		getdate(&d);
		gettime(&t);
		fprintf(dst,"/* Process Time: %04d-%02d-%02d %02d:%02d */\n",d.da_year,d.da_mon,d.da_day,t.ti_hour,t.ti_min);
		
		fprintf(dst,"#ifndef _HZTABLE__H\n");
		fprintf(dst,"#define _HZTABLE__H\n");
		fprintf(dst,"\n");
		fseek(src,0L,SEEK_END);
		fprintf(dst,"#define HZCOUNT %ld   /* Max number of Chines wors */\n",(ftell(src)/2));
		fseek(src,0L,SEEK_SET);
		fprintf(dst,"#ifndef HZK16TYPE_DEFINE\n");
		fprintf(dst,"#define HZK16TYPE_DEFINE\n");
		fprintf(dst,"struct	hzk16_typ{         /*  Chinese mode struct  */\n");
		fprintf(dst,"  UINT16 code;\n");
		fprintf(dst,"  UINT16 array[16];\n");
		fprintf(dst,"};\n");
		fprintf(dst,"#endif\n");
		
		fprintf(dst,"/*================================================= Chinese words =========================================================*/\n");
		/*	第一行的处理	*/
		fprintf(dst,"struct	hzk16_typ	hzk16_table[]={\n");
		hz[0]=fgetc(src);
		hz[1]=fgetc(src);
		k=(unsigned int)(hz[0])+((unsigned int)(hz[1])<<8);
		fprintf(dst,"{0x%04x,{",k);
		qu=hz[0]-0x0a0;
		wei=hz[1]-0x0a0;
		s=(qu-1)*94+wei-1;
		s*=32;
		fseek(hzlib,(long)s,0);
		for(i=0;i<16;i++)
		{
			zi0=fgetc(hzlib);
			zi1=fgetc(hzlib);
			zi=(unsigned int)(zi0)+((unsigned int)(zi1)<<8);
			if(i==15)
				fprintf(dst,"0x%04x}},",zi);
			else
				fprintf(dst,"0x%04x,",zi);
		}
		hzcount=1;
		
		/*	其余行的处理	*/
		
		while((hz[0]=fgetc(src))!=EOF)
		{
			fprintf(dst,"\n{");
			//hz[0]=fgetc(src);
			hz[1]=fgetc(src);
			hzcount++;
			k=(unsigned int)(hz[0])+((unsigned int)(hz[1])<<8);
			fprintf(dst,"0x%04x,{",k);
			qu=hz[0]-0x0a0;
			wei=hz[1]-0x0a0;
			s=(qu-1)*94+wei-1;
			s*=32;
			fseek(hzlib,(long)s,0);
			for(j=0;j<16;j++)
			{
				zi0=fgetc(hzlib);
				zi1=fgetc(hzlib);
				zi=(unsigned int)(zi0)+((unsigned int)(zi1)<<8);
				if(j==15)
					fprintf(dst,"0x%04x}},",zi);
				else
					fprintf(dst,"0x%04x,",zi);
			}	
		}
		fprintf(dst,"\n};");
		fprintf(dst,"\n#endif");
		fclose(src);
		fclose(dst);
		fclose(hzlib);
		remove("__TEMP__.h");
		printf("luobing: Till HZ OK,count: %d\n",hzcount);
	}
	return 0;
}

/* 从指定.c .h 文件获取汉字 */
/*入口参数: dstfile:目标文件名 srcfile源文件名*/
/*出口参数: 返回0,提取成功  否则失败          */
/*注意,本程序缺省认为字符串中不包含行如'//' '/*' 的字符*/
/* 如果包含了，请将其去除再使用，否则结果会不正确*/
unsigned char getHZ(char *dstfile,char *srcfile)
{
	FILE *src,*dst,*temp1,*temp2;
	int ch,i;
	char strbuf[1024],*ptr,*ptr1;
	unsigned char flag;
	unsigned int hz1,hz2,*ptrhz;
	
	if((src=fopen(srcfile,"r"))==NULL)
		return 1;
	if((temp1=fopen("__temp1.c","w"))==NULL)
	{
		fclose(src);
		return 1;
	}
	/* 第一遍扫描,将原始文件读入tmep中,并去除空格和制表符*/
	while((ch=fgetc(src))!=EOF)
	{
		if((ch!='\t')&&(ch!=' '))
			fputc(ch,temp1);
	}
	fclose(src);
	fclose(temp1);
	/* 第二遍扫描,去除 注释/**/
	if((temp1=fopen("__temp1.c","r"))==NULL)
	{
		return 1;
	}
	if((temp2=fopen("__temp2.c","w"))==NULL)
	{
		fclose(temp1);
		return 1;
	}
	flag=0;
	while(!feof(temp1))
	{
		memset(strbuf,0,512);
		fscanf(temp1,"%s",strbuf);
		if(flag==0)
		{
			if((ptr=strstr(strbuf,"/*"))==NULL)		/*没有发现*/
			{
				ptr=(strbuf+strlen(strbuf));															
				fprintf(temp2,"%s\n",strbuf);
			}
			else
			{
				
				if((ptr1=strstr(strbuf,"*/"))==NULL)		/*没有发现*/
				{
					/* ptr指向 /* 开始的位置并且没有与其相对应的配对*/
					flag=1;
					ptr1[0]='\0';
					fprintf(temp2,"%s\n",strbuf);
				}
				else
				{
					/* ptr指向 /* 开始的位置 ptr1指向与其相对应的配对*/
					for(i=0;i<(strlen(ptr)-strlen(ptr1)+2);i++)
						ptr[i]='1';		/*以字符 '1' 替代注释*/
					fprintf(temp2,"%s\n",strbuf);
				}
				
			}
			
		}
		else
		{
			if((ptr1=strstr(strbuf,"*/"))==NULL)		/*没有发现*/
			{}
			else	flag=0;
		}
	}
	fclose(temp1);
	fclose(temp2);
	/* 第三遍扫描,去除//注释之后的所有字符 */
	/*须保证" "的字符串中没有//字样 */
	
	if((temp1=fopen("__temp2.c","r"))==NULL)
	{
		return 1;
	}
	if((temp2=fopen("__temp1.c","w"))==NULL)		/*现在向__temp1.c写*/
	{
		fclose(temp1);
		return 1;
	}
	while(!feof(temp1))
	{
		memset(strbuf,0,512);
		fscanf(temp1,"%s",strbuf);
		if((ptr=strstr(strbuf,"//"))==NULL)		/*没有发现*/
		{
			ptr=(strbuf+strlen(strbuf));															
			fprintf(temp2,"%s\n",strbuf);
		}
		else
		{
			ptr[0]='\0';												/*字符串去除*/
			fprintf(temp2,"%s\n",strbuf);
		}
	}
	fclose(temp1);
	fclose(temp2);
	
	/* 第四遍扫描,所有非汉字的字符全部去除 */
	if((temp2=fopen("__temp2.c","w"))==NULL)
	{
		return 1;
	}
	if((temp1=fopen("__temp1.c","r"))==NULL)
	{
		fclose(temp2);
		return 1;
	}
	while((ch=fgetc(temp1))!=EOF)
	{
		if((ch>0xa0))
			fputc(ch,temp2);
	}
	fclose(temp1);
	fclose(temp2);
	/* 第五遍扫描,将temp2中的无重复汉字拷贝到temp1中 */
	if((temp2=fopen("__temp2.c","r"))==NULL)
	{
		return 1;
	}
	if((temp1=fopen("__temp1.c","w"))==NULL)
	{
		fclose(temp2);
		return 1;
	}
	memset(strbuf,'\0',1024);
	while((ch=fgetc(temp2))!=EOF)
	{
		hz1=(unsigned int)(ch);
		ch=fgetc(temp2);
		hz1+=((unsigned int)(ch)<<8);
		ptrhz=(unsigned int *)(strbuf);
		while((*ptrhz!=hz1)&&(*ptrhz!='\0'))
		{
			ptrhz+=1;
		}
		if(*ptrhz=='\0')													/* 没有发现相同的字符 */
			ptrhz[0]=hz1;															/* 插入 */			
	}
	fprintf(temp1,"%s",strbuf);
	fclose(temp1);
	fclose(temp2);
	remove("__temp2.c");
	/* 第六遍扫描,将文件中的汉字插入目标文件中 */
	/* 确保目标文件已经存在                    */
	if((dst=fopen(dstfile,"a+t"))==NULL)			/* 追加模式 */
		return 1;
	if((temp1=fopen("__temp1.c","r"))==NULL)
	{
		fclose(dst);
		return 1;
	}
	while((ch=fgetc(temp1))!=EOF)
	{
		hz1=(unsigned int)(ch);
		ch=fgetc(temp1);
		hz1+=((unsigned int)(ch)<<8);
		flag=0;
		hz2=0;
		rewind(dst);		/* 从文件头开始比较 */
		while((ch=fgetc(dst))!=EOF)
		{
			hz2=(unsigned int)(ch);
			ch=fgetc(dst);
			hz2+=((unsigned int)(ch)<<8);
			if(hz2==hz1)
				flag=1;
		}/*读完后指针将指向文件起始*/
		rewind(dst);
		if(flag==0)
			fprintf(dst,"%c%c",(unsigned char)(hz1),(unsigned char)(hz1>>8));	
			
	}
	fclose(dst);
	fclose(temp1);
	remove("__temp1.c");
	return 0;
}

/* 将指定的源文件中的字符串以规定形式输出到另外一个文件中 */
/*入口参数:  srcfile源文件名*/
/*出口参数: 返回0,提取成功  否则失败,成功的话将输出到文件dst_源文件名的文件中    */
/*注意,本程序直接转换""中的字符，所以在注释中要注意不要包含*/

unsigned char ConvertStr(char *srcfile)
{
	FILE *src,*dst;
	int ch;
	char dst_name[100];
	char *pdst_name=NULL;
	char flag=0;
	memset(dst_name,'\0',100);
	pdst_name=(char*)(dst_name+4);	
	memcpy(pdst_name,srcfile,(strlen(srcfile)+1)*sizeof(char));
	dst_name[0]='d';
	dst_name[1]='s';
	dst_name[2]='t';
	dst_name[3]='_';
	pdst_name=(char*)dst_name;
//	printf("lbdebug: dstname=%s\n",pdst_name);
	if((src=fopen(srcfile,"r"))==NULL)
		return 1;
	if((dst=fopen(pdst_name,"w"))==NULL)
	{
		fclose(src);
		return 1;
	}
	//读取文件,如果没有发现\"，则原文拷贝
	//如果发现\"，替换为'{',并寻找下一个\"，替换为'}'
	while((ch=fgetc(src))!=EOF)
	{
		  if(flag==1)		//flag为\"的标志
		  {
		  	if(ch=='\"')
		  	{
		  		flag=0;		//结束的\"，清除标志
		  		fprintf(dst,"0x00}",(unsigned char)ch);
		  	}
		  	else
		  		fprintf(dst,"0x%02x,",(unsigned char)ch);
		  }
		  else
		  {
		  	if(ch=='\"')
		  	{
		  		flag=1;		//开始的\"，设置标志
		  		fprintf(dst,"{",(unsigned char)ch);
		  	}
		  	else
					fputc(ch,dst);
			}
	}
	
	fclose(src);
	fclose(dst);
	return 0;
}
