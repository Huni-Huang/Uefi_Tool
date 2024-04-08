#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
//lb ver1.0
//以2k为计量单位，不足2k的补0
//此.bin文件以ISA ROM格式注入BIOS
char  szHelpContext[] = \
"Syntax is:  com2bin [options]\n" 
" /V        Version control	    \n"

" /? or /H  Display Help file\n"
"Example: com2bin.exe xxx.com xxx.bin";
void	main(int argc,char *argv[])
{
	unsigned char	ch;
	long	int	total=0l;							//.bin文件长度
	unsigned	char	length=0x40;					//.com文件块长度 512字节为一块
	unsigned	char	checksum;				//校验和
	unsigned char	NEG;
	unsigned	int	fill2k;				//2k为一块准备填充0
	FILE	*fp1,*fp2;
	int	i;
	//帮助信息
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
                printf(szHelpContext);
                break;
          case 'V':
          case 'v':
          	  printf("lb ver1.0  2005-6-17");      
          	  break;
          default:
              break;        
        }
      }
      else
      {
      	//打开文件
	
		if((fp1=fopen(argv[1],"rb"))==NULL)	/*打开一个由argv[1]所指的文件*/
		{
			printf("Canot not open	%s",argv[1]);
			exit(0);
		}
		if((fp2=fopen(argv[2],"wb"))==NULL)	/*创建一个由argv[2]所指的文件*/
		{
			printf("\nCanot not creat file	%s",argv[2]);
			exit(0);
		}
	
	//处理文件
	ch=getc(fp1);						//只能这么写 
	while(!feof(fp1))
	{
		putc((int)ch,fp2);
		total++;
		checksum=checksum+ch;
		ch=getc(fp1);	/* 从文件读一字符*/
	}
	//处理文件的长度，程序中在<32K时，已经设定为40h
	//length=(unsigned	char)(total/512);
	//length=(length>0x40)?0x80:0x40;
	/*if(length==0x40)
	{
		NEG=256-checksum;
		putc((int)NEG,fp2);
	}
	else
	{
		NEG=256-(checksum+0x40);
		putc((int)NEG,fp2);
		fseek(fp2,0x03l,SEEK_SET);
		putc(0x80,fp2);
	}
	*/
	NEG=256-checksum;
	putc((int)NEG,fp2);
	total++;
	
	fill2k=2048-(total%2048);
	fseek(fp2,0x0l,SEEK_END);
	for(i=0;i<fill2k;i++)
	putc(0x00,fp2);
	
	printf("\t\t%s\t\t%s",argv[1],argv[2]);
	printf("\nLength(byte):\t%ld,\t\t%ld",(total-1),(total));
	printf("\nNEG:         \t    \t\t%d",NEG);
	printf("\nFileLength:  \t    \t\t%d",length);
	fclose(fp1);
	fclose(fp2);
	getch();
      }
    }
  }
   
	
}