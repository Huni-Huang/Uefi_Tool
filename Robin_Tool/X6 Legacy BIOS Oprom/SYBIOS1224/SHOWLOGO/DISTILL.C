//提取神郁LOGO的例程
//必须自己将logo图放在640x480 256色图的左下角，logo图的周围用logo中没有的颜色填充，（比如深蓝色）
//提取了两个文件，logopal.bin和logopic.bin为颜色和图象信息
//由于SYBIOS本身用了十种颜色，所以当前提取图象信息时注意每个象素的值要加10
//加到SYBIOS中时，注意修改以下值：
//															LOGO_WIDTH,LOGO_HEIGHT,为显示logo，SYBIOS中的几个函数也做了相应的修改
//														PUTWINDOW,而且缺省的标题栏的高度也修改了
//2005-7-12 luobing 20:41
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

//在此处修改提取的logo图的大小
#define	LOGO_WIDTH	24
#define	LOGO_HEIGHT	24
#define	USED_COLOR_COUNT	10
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
/*---END---*/
/*函数声明*/
void	distill_pal(void);
void	distill_pic(void);
/*---END---*/
char	szHelp[]=\
"Syntax is : distill.exe\n"
" You must make sure sylogo.bmp exist.--luobing";
int	main(int	argc,char	*argv[])
{
	int	i=0;
	if(argc==1)
	{
		distill_pal();
		distill_pic();
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

void	distill_pal(void)
{
	int	i=0;
	FILE	*in,*out;
	RGB_BMP	logopal;
	//Set_SVGA_Mode(SVGA256); 
	//BMP_Load_Screen("sylogo.bmp");
	//
	if((in=fopen("sylogo.bmp","rb"))==NULL)
	{
		printf("Cannot open file:sylogo.bmp.\n");
		exit(1);
	}
	if((out=fopen("logopal.bin","w"))==NULL)
	{
		printf("Cannot creat file:logopal.bin\n");
		exit(1);
	}
	//
	fprintf(out,"logopalatte");
	if(fseek(in,54,0))		//移动到调色板区
 	{
 		printf("fseek fail");
 		getch();
 	}
 	for(i=0;i<256;i++)
 	{
 		fread(&logopal,sizeof(logopal),1,in);
 		fprintf(out,"\n						db 0%02xH,",logopal.red);
 		fprintf(out," 0%02xH,",logopal.green);
 		fprintf(out," 0%02xH",logopal.blue);
 	}
 	fclose(in);
 	fclose(out);
}

void	distill_pic(void)
{
	int	i=0,j=0;
	long	int	logopos;
	unsigned	char	buf;
	FILE	*in,*out;
	RGB_BMP	logopal;
	//
	if((in=fopen("sylogo.bmp","rb"))==NULL)
	{
		printf("Cannot open file:sylogo.bmp.\n");
		exit(1);
	}
	if((out=fopen("logopic.bin","w"))==NULL)
	{
		printf("Cannot creat file:logopic.\n");
		exit(1);
	}
	//
	fprintf(out,"logopic");
	for(i=LOGO_HEIGHT-1;i>=0;i--)
 	{
 		logopos=i*SCREEN_WIDTH+1078;
 		if(fseek(in,logopos,0))		//移动到图象区
 		{
 			printf("fseek fail.");
 			getch();
 		}
 		fprintf(out,"\n						db");
 		for(j=0;j<LOGO_WIDTH-1;j++)
 		{	
 			fread(&buf,sizeof(buf),1,in);
 			fprintf(out," 0%02xH,",buf+USED_COLOR_COUNT);
 		}
 		fread(&buf,sizeof(buf),1,in);
 		fprintf(out," 0%02xH",buf+USED_COLOR_COUNT);
 	}
 	fclose(in);
 	fclose(out);
}