//chks.c: 校验任意给出的文件使得字校验为0，字节校验为0
//2009-1-22 14:23 luobing
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include "ver_l.h"

unsigned char bSum(char *file);
unsigned int wSum(char *file);
void bw_chk(char *file);

char  szHelpContext[] = \
"Syntax is:  chks.exe [options]\n" 
" /V        Version control	    \n"

" /? or /H  Display Help file\n"
" /b or /B xxx.bin Get file\'s BYTE checksum\n"
" /w or /W xxx.bin Get file\'s WORD checksum\n"
"Example: chks.exe xxx.bin";

void	main(int argc,char *argv[])
{
	if(argc==2)
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
        	  printf("%s %d.%d%d",PString,ReleaseVerH,MainVer,ReleaseVerL);      
        	  break;
        default:
        		printf("Syntax error! Please use /? to get help.");
            break;        
      }
    }
    else
    	bw_chk(argv[1]);			/* 直接对文件进行校验调整 */  	
	}
	else if(argc==3)
	{
		if((argv[1][0] == '/')||(argv[1][0] == '-'))
		{
			switch(argv[1][1])
      {
        case 'b':
        case 'B':
           		printf("Filename: %s, checksum(BYTE): 0x%02x",argv[2],bSum(argv[2]));
              break;
        case 'w':
        case 'W':
        	  printf("Filename: %s, checksum(WORD): 0x%04x",argv[2],wSum(argv[2]));    
        	  break;
        default:
        		printf("Syntax error! Please use /? to get help.");
            break;        
      }
    }
    else	printf("Syntax error! Please use /? to get help.");
	}
	else printf("Syntax error! Please use /? to get help.");

	
}

/* 获得文件的字节校验和 */
/*入口: file:文件名 */
/*出口: 字节校验和 */
unsigned char bSum(char *file)
{
	FILE *fp;
	unsigned char ch,checksum=0;
	
	if((fp=fopen(file,"rb")) ==NULL)
	{
		printf("Cannot open file: %s,Byte check sum invalid!\n",file);
		exit(0);
	}
	
	ch=getc(fp);		/* 注意读到结尾的时候会读入0xff，所以必须这样写*/
	while(!feof(fp))
	{
		checksum+=ch;
		ch=getc(fp);		
	}
	fclose(fp);	
	return checksum;
}

/* 获得文件的字校验和 */
/*入口: file:文件名 */
/*出口: 字节校验和 */
unsigned int wSum(char *file)
{
	FILE *fp;
	unsigned int wch,checksum=0;
	
	if((fp=fopen(file,"rb")) ==NULL)
	{
		printf("Cannot open file: %s,Word check sum invalid!\n",file);
		exit(0);
	}
	wch=getw(fp);		/* 注意读到结尾的时候会读入0xff，所以必须这样写*/
	while(!feof(fp))
	{
		checksum+=wch;
		wch=getw(fp);		
	}
	fclose(fp);	
	return checksum;
}


void bw_chk(char *file)
{
	FILE *fp;
	unsigned int old_wsum,neg1,neg2,j,k;
	unsigned long i;
	unsigned char old_bsum,ch,checksum=0;
	
	old_wsum=wSum(file);
	
	if((fp=fopen(file,"r+b")) ==NULL)
	{
		printf("Cannot open file: %s! \n",file);
		exit(0);
	}
	/*1 判断其文件到结尾处是否空余了 1+255+1个字	全部为0 */
	if(fseek(fp,-514l,SEEK_END))
	{
		printf("The length file:%s less 514 bytes!",file);
		fclose(fp);
		exit(0);
	}
	for(i=0;i<514;i++)
		if(getc(fp))
		{
			printf("The last 514 byte must 0,please check it!");
			fclose(fp);
			exit(0);
		}
	/*2 计算加了字校验补后的字节校验和 */
	fseek(fp,0L,SEEK_SET);
	neg1=0-old_wsum;			/* 要补的第一个字 */
	
	ch=getc(fp);		/* 注意读到结尾的时候会读入0xff，所以必须这样写*/
	while(!feof(fp))
	{
		checksum+=ch;
		ch=getc(fp);		
	}
	printf("old checksum(BYTE): %02x, old checksum(WORD):%04x\n",checksum,old_wsum);
	
	checksum+=(unsigned char)(neg1);
	checksum+=(unsigned char)(neg1>>8);
	neg2=(unsigned int)(checksum);
	printf("add word1:%04x, word2:%04x\n",neg1,neg2);
	printf("add number of 0xffff: %04x\n",neg2);
	printf("Operated OK,congratulation!");
	if(fseek(fp,-514l,SEEK_END))
		printf("fseek error!");
	putw(neg1,fp);
	putw(neg2,fp);
	for(j=0;j<neg2;j++)
		putw(0xffff,fp);
	
	fclose(fp);
	return;
}