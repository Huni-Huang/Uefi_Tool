/*exetrace hz form files*/
/*luobing 2008-10-20 12:29*/
/*������: ����ȡ���32���ļ�����" "�����ĺ��֣�����ȡ������ģ*/
/*ע��:     ���뱣֤����ȡ���ļ����ַ���"**"��û��//��/* �ַ�,������ܻ���ȡ����*/
/*luobing 2011-09-06 21:28:11
  ��UEFI�Ļ�������ʾ���֣�Ŀǰû���ҵ������Ժ�����ʽ�������ַ���������"��"��AMI��
  ���뻷��������ʾ�������ġ��������һ��������ƣ�ÿ����Ҫ��ʾ���ַ���������Ϊ��
  �� UINT8 Str[]={0x12,0x34,...0x00};�����ӣ�Ȼ����ͨ����Ӧ���ַ�����ʾ������ʾ*/
/*Ϊ�˴����ϵļ򵥣�Դ�ļ��е��ַ���������ΪUINT8 Str[]="�ᰮ����forever";֮�����ʽ*/
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
	unsigned long flag=0;			/* �Ƿ���ȡ���ֳɹ��ı�־ bitx=1Ϊ��Ӧ���ļ���ȡ�ɹ� */
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
			//��ȡ���к��ֵ�ָ�����ļ�
			printf(" %d Extract chinese words from %s:",i-1,argv[i]);
			if(!getHZ("__TEMP__.h",argv[i]))
			{
				flag|=(1l<<(i-2));			/* ������ȡ�ɹ��ı�־ */
				printf(" Success!\n");
			}
			else
				printf(" Failure!\n");
				
			//ת���ļ�
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
		dstfile=argv[1];					/* ��Ҫ��ź��ֿ��Ŀ���ļ� */
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
		/*	��һ�еĴ���	*/
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
		
		/*	�����еĴ���	*/
		
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

/* ��ָ��.c .h �ļ���ȡ���� */
/*��ڲ���: dstfile:Ŀ���ļ��� srcfileԴ�ļ���*/
/*���ڲ���: ����0,��ȡ�ɹ�  ����ʧ��          */
/*ע��,������ȱʡ��Ϊ�ַ����в���������'//' '/*' ���ַ�*/
/* ��������ˣ��뽫��ȥ����ʹ�ã��������᲻��ȷ*/
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
	/* ��һ��ɨ��,��ԭʼ�ļ�����tmep��,��ȥ���ո���Ʊ��*/
	while((ch=fgetc(src))!=EOF)
	{
		if((ch!='\t')&&(ch!=' '))
			fputc(ch,temp1);
	}
	fclose(src);
	fclose(temp1);
	/* �ڶ���ɨ��,ȥ�� ע��/**/
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
			if((ptr=strstr(strbuf,"/*"))==NULL)		/*û�з���*/
			{
				ptr=(strbuf+strlen(strbuf));															
				fprintf(temp2,"%s\n",strbuf);
			}
			else
			{
				
				if((ptr1=strstr(strbuf,"*/"))==NULL)		/*û�з���*/
				{
					/* ptrָ�� /* ��ʼ��λ�ò���û���������Ӧ�����*/
					flag=1;
					ptr1[0]='\0';
					fprintf(temp2,"%s\n",strbuf);
				}
				else
				{
					/* ptrָ�� /* ��ʼ��λ�� ptr1ָ���������Ӧ�����*/
					for(i=0;i<(strlen(ptr)-strlen(ptr1)+2);i++)
						ptr[i]='1';		/*���ַ� '1' ���ע��*/
					fprintf(temp2,"%s\n",strbuf);
				}
				
			}
			
		}
		else
		{
			if((ptr1=strstr(strbuf,"*/"))==NULL)		/*û�з���*/
			{}
			else	flag=0;
		}
	}
	fclose(temp1);
	fclose(temp2);
	/* ������ɨ��,ȥ��//ע��֮��������ַ� */
	/*�뱣֤" "���ַ�����û��//���� */
	
	if((temp1=fopen("__temp2.c","r"))==NULL)
	{
		return 1;
	}
	if((temp2=fopen("__temp1.c","w"))==NULL)		/*������__temp1.cд*/
	{
		fclose(temp1);
		return 1;
	}
	while(!feof(temp1))
	{
		memset(strbuf,0,512);
		fscanf(temp1,"%s",strbuf);
		if((ptr=strstr(strbuf,"//"))==NULL)		/*û�з���*/
		{
			ptr=(strbuf+strlen(strbuf));															
			fprintf(temp2,"%s\n",strbuf);
		}
		else
		{
			ptr[0]='\0';												/*�ַ���ȥ��*/
			fprintf(temp2,"%s\n",strbuf);
		}
	}
	fclose(temp1);
	fclose(temp2);
	
	/* ���ı�ɨ��,���зǺ��ֵ��ַ�ȫ��ȥ�� */
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
	/* �����ɨ��,��temp2�е����ظ����ֿ�����temp1�� */
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
		if(*ptrhz=='\0')													/* û�з�����ͬ���ַ� */
			ptrhz[0]=hz1;															/* ���� */			
	}
	fprintf(temp1,"%s",strbuf);
	fclose(temp1);
	fclose(temp2);
	remove("__temp2.c");
	/* ������ɨ��,���ļ��еĺ��ֲ���Ŀ���ļ��� */
	/* ȷ��Ŀ���ļ��Ѿ�����                    */
	if((dst=fopen(dstfile,"a+t"))==NULL)			/* ׷��ģʽ */
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
		rewind(dst);		/* ���ļ�ͷ��ʼ�Ƚ� */
		while((ch=fgetc(dst))!=EOF)
		{
			hz2=(unsigned int)(ch);
			ch=fgetc(dst);
			hz2+=((unsigned int)(ch)<<8);
			if(hz2==hz1)
				flag=1;
		}/*�����ָ�뽫ָ���ļ���ʼ*/
		rewind(dst);
		if(flag==0)
			fprintf(dst,"%c%c",(unsigned char)(hz1),(unsigned char)(hz1>>8));	
			
	}
	fclose(dst);
	fclose(temp1);
	remove("__temp1.c");
	return 0;
}

/* ��ָ����Դ�ļ��е��ַ����Թ涨��ʽ���������һ���ļ��� */
/*��ڲ���:  srcfileԴ�ļ���*/
/*���ڲ���: ����0,��ȡ�ɹ�  ����ʧ��,�ɹ��Ļ���������ļ�dst_Դ�ļ������ļ���    */
/*ע��,������ֱ��ת��""�е��ַ���������ע����Ҫע�ⲻҪ����*/

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
	//��ȡ�ļ�,���û�з���\"����ԭ�Ŀ���
	//�������\"���滻Ϊ'{',��Ѱ����һ��\"���滻Ϊ'}'
	while((ch=fgetc(src))!=EOF)
	{
		  if(flag==1)		//flagΪ\"�ı�־
		  {
		  	if(ch=='\"')
		  	{
		  		flag=0;		//������\"�������־
		  		fprintf(dst,"0x00}",(unsigned char)ch);
		  	}
		  	else
		  		fprintf(dst,"0x%02x,",(unsigned char)ch);
		  }
		  else
		  {
		  	if(ch=='\"')
		  	{
		  		flag=1;		//��ʼ��\"�����ñ�־
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
