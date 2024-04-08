//code by luobing 2013-5-18 16:20:09
//注意使用工具提取PCX的图像，再拷贝到下面规定的位置
//lbdebug begin
//#include "Font.h"
//#include "Keyboard.h"
//lbdebug end
#include "Graphic.h"
#include "BMP256.h"

extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;

/* ==================================== 拷贝bmp相关的数据到此处 begjin======================================= */
/* ==================================== 拷贝bmp相关的数据  end======================================= */

//========Functions==========
//Function Name: putBMP256
//Input: x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//可以根据图像的情况，选择一个不显示的颜色，以适应显示logo的情况
VOID putBMP256(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
  UINTN i,j;
  UINT32 index=0;
  UINT8 color_number;
  
  for(j=y;j<Height+y;j++)
    for(i=x;i<Width+x;i++)
    {
      color_number=Picture[index];
      if(color_number!=MaskColor)
        putpixel(i,j,&(ColorTable[color_number]));
      ++index;
    }
}

//Function Name: putBMP256_fast
//Input: x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//注意1，实际上，MaskColor在当前的函数中是无效的。因为直接缓冲区写屏，导致有些不想写
//     的像素无法滤过。当然，如果底色是纯色的，可以添加代码去替代这部分。或者，添加
//     读屏的函数，将屏幕上的像素填充需要掩盖的位置，出来的效果也很好。这部分代码
//      暂时没有添加。 ---luobing 2013-5-18 21:54:40
//注意2，程序员必须保证图像的位置不能超过屏幕本身，否则...blt不会显示任何东西出来
VOID putBMP256_fast(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 index,count;
//	UINT16 num_bytes;
	UINT32 pcxdata;
	EFI_STATUS Status;
//	CHAR8 StrBuffer[30];
	
	index=0;	//图像数据的指针
	count=0;	//用来定位像素位置的计数器
	BltBufferSize = ((UINT32)Width *  (UINT32)Height * (sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
	Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );//最大可以容纳2^30的256色pcx数据，即32678x32678的图，不会溢出
	//1 将数据拷贝到缓冲区中
	while(count<((UINT32)Width *  (UINT32)Height))
	{
		pcxdata =(UINT32)(Picture[index]);
		++index;
		BltBuffer[count].Blue =ColorTable[pcxdata].Blue;
		BltBuffer[count].Green =ColorTable[pcxdata].Green;
		BltBuffer[count].Red =ColorTable[pcxdata].Red;
		BltBuffer[count].Reserved =ColorTable[pcxdata].Reserved;
		++count;
	}
	//2 图像直接写屏
	gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,x,y,Width,Height,0);
	gBS->FreePool(BltBuffer);
}

//Function Name: putBMP256_fun
//Input: x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//Just for fun 解决了不能使用markcolor的问题
VOID putBMP256_fun(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 index,count;
//	UINT16 num_bytes;
	UINT32 pcxdata;
	EFI_STATUS Status;
//	CHAR8 StrBuffer[30];
	
	index=0;	//图像数据的指针
	count=0;	//用来定位像素位置的计数器
	BltBufferSize = ((UINT32)Width *  (UINT32)Height * (sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
	Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );//最大可以容纳2^30的256色pcx数据，即32678x32678的图，不会溢出

	getRectImage(x,y,Width,Height,BltBuffer);  
	//1 将数据拷贝到缓冲区中
	while(count<((UINT32)Width *  (UINT32)Height))
	{
		pcxdata =(UINT32)(Picture[index]);
		if(MaskColor!=(UINT8)pcxdata)
		{
			BltBuffer[count].Blue =ColorTable[pcxdata].Blue;
			BltBuffer[count].Green =ColorTable[pcxdata].Green;
			BltBuffer[count].Red =ColorTable[pcxdata].Red;
			BltBuffer[count].Reserved =ColorTable[pcxdata].Reserved;	
		}
		++index;
		++count;
	}
	//2 图像直接写屏
	gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,x,y,Width,Height,0);
	gBS->FreePool(BltBuffer);
}