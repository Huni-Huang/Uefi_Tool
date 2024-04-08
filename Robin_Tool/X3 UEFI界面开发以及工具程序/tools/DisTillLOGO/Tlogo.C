//提取神郁LOGO的例程
//必须自己将logo图放在640x480 256色图的左下角，logo图的周围用logo中没有的颜色填充，（比如深蓝色）
//提取了两个文件，logopal.bin和logopic.bin为颜色和图象信息
//由于SYBIOS本身用了十种颜色，所以当前提取图象信息时注意每个象素的值要加10
//加到SYBIOS中时，注意修改以下值：
//															LOGO_WIDTH,LOGO_HEIGHT,为显示logo，SYBIOS中的几个函数也做了相应的修改
//														PUTWINDOW,而且缺省的标题栏的高度也修改了
//2005-7-12 luobing 20:41

//.为了显示的方便，除了真彩色外，其他的每中颜色模式的行字节数要用数据“00”补齐为4的整数倍。
//如果显示模式为16色，当图像宽为19时，存储时每行则要补充4-(19/2+1)%4=2个字节（加1是因为里面
//有一个像素点要独占了一字节）。如果显示模式为256色，当图像宽为19时，每行也要补充4-19%4=1个
//字节。

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
#include <time.h>

//在此处修改提取的logo图的大小
//#define	LOGO_WIDTH	24
//#define	LOGO_HEIGHT	24
#define LOGO_FILE		"sylogo.bmp"
#define	LOGO_WIDTH	24
#define	LOGO_HEIGHT	24
#define BKG_COLOR		0			//一共有256种颜色存在，第几个是背景色，即在显示的时候不去输出颜色
//#define BKG_MASK_VALUE	0x55
#define	USED_COLOR_COUNT	(10+116+17)				//神郁和ESC的图标分别用掉了116和17种颜色
//#define	USED_COLOR_COUNT	10
/*宏及全局变量*/
#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640

#define PALETTE_MASK 0x3c6
#define PALETTE_REGISTER_RD 0x3c7
#define PALETTE_REGISTER_WR 0x3c8
#define PALETTE_DATA 0x3c9

#define VGA256 0x13
#define SVGA256 0x0101
#define TEXT_MODE 0x03

unsigned char far *video_buffer=(char far *)0xA0000000L;
/*---END---*/

/*BMP的数据结构 320*200 256色*/
typedef struct BMP_file						/*文件信息区*/
{
	unsigned int bfType;						/*文件类型 "BM"*/
	unsigned long bfSize;						/*文件长度*/
	unsigned int Reserved1;
	unsigned int Reserved2;
	unsigned long bfOffset;					/*文件描述区长度，16色118，256色1024*/
}bitmapfile;	

typedef struct BMP_info						/*图象信息区*/
{
	unsigned long biSize;						/*图形尺寸*/
	unsigned long biWidth;					/*图形宽度*/
	unsigned long biHeight;					/*图形高度*/
	unsigned int biPlanes;
	unsigned int biBitCount;				/*每个象素占二进制位数*/
	unsigned long biCompression;		/*是否压缩格式*/
	unsigned long biSizeImage;
	unsigned long biXpolsPerMeter;
	unsigned long biYpelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
}bitmapinfo;

typedef struct RGB_BMP_typ				/*调色板区*//*256色=256x4byte*/				
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
}RGB_BMP,*RGB_BMP_ptr;

typedef struct bmp_picture_typ
{
	bitmapfile file;
	bitmapinfo info;
	RGB_BMP palette[256];
	unsigned	char far *buffer;
} bmp_picture, *bmp_picture_ptr;

typedef struct {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;
/*---END---*/
/*===============global variable==================*/
EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[256];		//存储256色bmp的颜色值
/*函数声明*/
//void	distill_pal(void);
//void	distill_pic(void);
unsigned int getColorTable(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *gColorTable,char *srcfile);
unsigned int OutEfiColorFile(char *srcfile,char *dstfile);
/*---END---*/
char	szHelp[]=\
"Syntax is : distill.exe\n"
" You must make sure sylogo.bmp exist.--luobing";
int	main(int	argc,char	*argv[])
{
	int	i=0;
	if(argc==1)
	{
//		distill_pal();
//		distill_pic();
		OutEfiColorFile(LOGO_FILE,"sylogo.c");
//		printf("gColorTable:Blue Green Red Reserved\n");
//		for(i=0;i<10;i++)
//		{
//			printf("%02d:          %02x   %02x    %02x  %02x \n",i,gColorTable[i].Blue,gColorTable[i].Green,gColorTable[i].Red,gColorTable[i].Reserved);
//		}
	}
	if(argc>1)
	{
		for(i=1;i<argc;i++)
		{
			if((argv[i][0] == '/')||(argv[i][0] == '-'))
			{
				switch(argv[i][1])
				{
					case	'?':
					case	'H':
					case	'h':
						printf(szHelp);
						break;
					default:
						printf("Command error.");
						break;
					}
				}
			}
		}
	//
	getch();
	return 0;
}
//函数名: getColorTable
//入口参数: gColorTable=存储颜色的表  srcfile=文件名
//出口参数: 0:成功  1:失败 
unsigned int getColorTable(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *gColorTable,char *srcfile)
{
	FILE	*in;
	
	//提取256色bmp图中的颜色值
	if((in=fopen(srcfile,"rb"))==NULL)
	{
		printf("Cannot open file:%s.\n",srcfile);
		return 1;
	}
	if(fseek(in,54,0))		//移动到调色板区
 	{
 		printf("fseek file %s fail.\n",srcfile);
 		return 1;
 	}
 	fread(gColorTable,sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),256,in);
// 	gColorTable[BKG_COLOR].Reserved=BKG_MASK_VALUE;
 	fclose(in);
 	return 0;
}
//函数名: OutEfiColorFile
//入口参数: srcfile=*.bmp(256色的logo) dstfile=*.c提取颜色信息
//出口参数: 0表示成功，1则失败
unsigned int OutEfiColorFile(char *srcfile,char *dstfile)
{
	//将bmp信息区的颜色输出到指定的文件中
	FILE *src,*dst;
	int	i=0,j=0;
	struct time t;
	struct date d;
	long	int	logopos;
	unsigned	char	buf;
	unsigned	int width;
	unsigned 	int res;
	unsigned char *buff;
	unsigned int count=0;
	
	getColorTable(gColorTable,srcfile);		//将颜色值存入到表中
	if((src=fopen(srcfile,"rb"))==NULL)
	{
		printf("Cannot open file:%s.\n",srcfile);
		return 1;
	}
	if((dst=fopen(dstfile,"w"))==NULL)
	{
		printf("Cannot creat file:%s \n",dstfile);
		return 1;
	}
	fprintf(dst,"/* %s: bmp 图形点阵表 by luobing*/\n",dstfile);
	getdate(&d);
	gettime(&t);
	fprintf(dst,"/* 提取时间: %04d-%02d-%02d %02d:%02d */\n",d.da_year,d.da_mon,d.da_day,t.ti_hour,t.ti_min);
	fprintf(dst,"EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYLogoColorTable[]={\n");
	for(i=0;i<255;i++)
		fprintf(dst,"  0x%02x,  0x%02x,  0x%02x,  0x%02x,      //%03d\n",gColorTable[i].Blue,gColorTable[i].Green,gColorTable[i].Red,gColorTable[i].Reserved,i);
	fprintf(dst,"  0x%02x,  0x%02x,  0x%02x,  0x%02x      //%03d\n",gColorTable[255].Blue,gColorTable[255].Green,gColorTable[255].Red,gColorTable[255].Reserved,i);
	fprintf(dst,"};\n\n");
	fprintf(dst,"UINT8 gSYLogoPicture[]={\n");
	
//	#if (LOGO_WIDTH<25)
//	for(i=LOGO_HEIGHT-1;i>=0;i--)
// 	{
// 		logopos=i*LOGO_WIDTH+1078;
// 		if(fseek(src,logopos,0))		//移动到图象区
// 		{
// 			printf("fseek fail.");
// 			getch();
// 		}
//// 		fprintf(out,"\n						db");
//		if(i==00)
//		{
//			for(j=0;j<LOGO_WIDTH-1;j++)
// 			{	
// 				fread(&buf,sizeof(buf),1,src);
// 				fprintf(dst," 0x%02x,",buf);
// 			}
// 			fread(&buf,sizeof(buf),1,src);
// 			fprintf(dst," 0%02x",buf);
//		}
//		else
//		{
// 			for(j=0;j<LOGO_WIDTH;j++)
// 			{	
// 				fread(&buf,sizeof(buf),1,src);
// 				fprintf(dst," 0x%02x,",buf);
// 			}
// 		}
// 		fprintf(dst,"\n");	
// 	}
// 	fprintf(dst,"};");
//	#else
		width=(((LOGO_WIDTH*1L)*LOGO_HEIGHT)/24);
		res=(((LOGO_WIDTH*1L)*LOGO_HEIGHT)%24);
		printf("width=%d\n",width);
		printf("res=%d\n",res);
		if((buff=malloc(LOGO_WIDTH*LOGO_HEIGHT+1))==NULL)
		{
			printf("Not enough to allocate for the point\n");
			getch();
			exit(1);
		}
		count=0;
		//读取图像数据到buff
		for(i=LOGO_HEIGHT-1;i>=0;i--)
 		{
 			logopos=i*LOGO_WIDTH+1078;
 			if(fseek(src,logopos,0))		//移动到图象区
 			{
 				printf("fseek fail.");
 				getch();
 			}
 			for(j=0;j<LOGO_WIDTH;j++)
 			{	
 				fread(&buf,sizeof(buf),1,src);
 				buff[count]=buf;
 				++count;
 			}
 		}
		//
		count=0;
		for(i=0;i<width;i++)
 		{
// 			fprintf(out,"\n						db");
			if(i==(width-1))
			{
				for(j=0;j<24-1;j++)
 				{	
 					buf=buff[count];
 					++count;
 					fprintf(dst," 0x%02x,",buf);
 				}
 				buf=buff[count];
 				++count;
 				if(res==0)
 					fprintf(dst," 0x%02x",buf);
 				else
 					fprintf(dst," 0x%02x,",buf);
			}
			else
			{
 				for(j=0;j<24;j++)
 				{	
 					buf=buff[count];
 					++count;
 					fprintf(dst," 0x%02x,",buf);
 				}
 			}
 			fprintf(dst,"\n");
 		}
// 		fprintf(out,"\n						db");
 		if(res)
 		{
 			for(j=0;j<res-1;j++)
 			{	
				buf=buff[count];
 				++count;
				fprintf(dst," 0x%02x,",buf);
 			}
 			buf=buff[count];
 			++count;
 			fprintf(dst," 0x%02x",buf);
 			fprintf(dst,"\n");
 		}
	
	fprintf(dst,"};\n");
	free(buff);
//	#endifs
	printf("count=%d\n",count);
 	fclose(src);
 	fclose(dst);
	return 0;
}


//void	distill_pal(void)
//{
//	int	i=0;
//	FILE	*in,*out;
//	RGB_BMP	logopal;
//	//Set_SVGA_Mode(SVGA256); 
//	//BMP_Load_Screen("sylogo.bmp");
//	//
//	if((in=fopen("sylogo.bmp","rb"))==NULL)
//	{
//		printf("Cannot open file:sylogo.bmp.\n");
//		exit(1);
//	}
//	if((out=fopen("logopal.bin","w"))==NULL)
//	{
//		printf("Cannot creat file:logopal.bin\n");
//		exit(1);
//	}
//	//
//	fprintf(out,"logopalatte");
//	if(fseek(in,54,0))		//移动到调色板区
// 	{
// 		printf("fseek fail");
// 		getch();
// 	}
// 	for(i=0;i<256;i++)
// 	{
// 		fread(&logopal,sizeof(logopal),1,in);
// 		fprintf(out,"\n						db 0%02xH,",logopal.red);
// 		fprintf(out," 0%02xH,",logopal.green);
// 		fprintf(out," 0%02xH",logopal.blue);
// 	}
// 	fclose(in);
// 	fclose(out);
// 	return;
//}
//
//void	distill_pic(void)
//{
//	int	i=0,j=0;
//	long	int	logopos;
//	unsigned	char	buf;
//	unsigned	int width;
//	unsigned 	int res;
//	unsigned char *buff;
//	unsigned int count=0;
//	FILE	*in,*out;
//	RGB_BMP	logopal;
//	//
//	if((in=fopen("sylogo.bmp","rb"))==NULL)
//	{
//		printf("Cannot open file:sylogo.bmp.\n");
//		exit(1);
//	}
//	if((out=fopen("logopic.bin","w"))==NULL)
//	{
//		printf("Cannot creat file:logopic.\n");
//		exit(1);
//	}
//	//
//	fprintf(out,"logopic");
//	if(LOGO_WIDTH<25)
//	{
//		for(i=LOGO_HEIGHT-1;i>=0;i--)
// 		{
// 			logopos=i*LOGO_WIDTH+1078;
// 			if(fseek(in,logopos,0))		//移动到图象区
// 			{
// 				printf("fseek fail.");
// 				getch();
// 			}
// 			fprintf(out,"\n						db");
// 			for(j=0;j<LOGO_WIDTH-1;j++)
// 			{	
// 				fread(&buf,sizeof(buf),1,in);
// 				fprintf(out," 0%02xH,",buf+USED_COLOR_COUNT);
// 			}
// 			fread(&buf,sizeof(buf),1,in);
// 			fprintf(out," 0%02xH",buf+USED_COLOR_COUNT);
// 		}
//	}
//	else
//	{
//		width=(((LOGO_WIDTH*1L)*LOGO_HEIGHT)/24);
//		res=(((LOGO_WIDTH*1L)*LOGO_HEIGHT)%24);
//		printf("width=%d\n",width);
//		printf("res=%d\n",res);
//		if((buff=malloc(LOGO_WIDTH*LOGO_HEIGHT+1))==NULL)
//		{
//			printf("Not enough to allocate for the point\n");
//			getch();
//			exit(1);
//		}
//		count=0;
//		for(i=LOGO_HEIGHT-1;i>=0;i--)
// 		{
// 			logopos=i*LOGO_WIDTH+1078;
// 			if(fseek(in,logopos,0))		//移动到图象区
// 			{
// 				printf("fseek fail.");
// 				getch();
// 			}
// 			for(j=0;j<LOGO_WIDTH;j++)
// 			{	
// 				fread(&buf,sizeof(buf),1,in);
// 				buff[count]=buf+USED_COLOR_COUNT;
// 				++count;
// 			}
// 		}
//		//
//		count=0;
//		for(i=0;i<width;i++)
// 		{
// 			fprintf(out,"\n						db");
// 			for(j=0;j<24-1;j++)
// 			{	
// 				buf=buff[count];
// 				++count;
// 				fprintf(out," 0%02xH,",buf);
// 			}
// 			buf=buff[count];
// 			++count;
// 			fprintf(out," 0%02xH",buf);
// 		}
// 		fprintf(out,"\n						db");
// 		if(res)
// 		{
// 			for(j=0;j<res-1;j++)
// 			{	
//				buf=buff[count];
// 				++count;
//				fprintf(out," 0%02xH,",buf);
// 			}
// 			buf=buff[count];
// 			++count;
// 			fprintf(out," 0%02xH",buf);
// 		}
//	}
//	free(buff);
//	printf("count=%d\n",count);
// 	fclose(in);
// 	fclose(out);
//}