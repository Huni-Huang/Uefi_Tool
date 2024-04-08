//code by luobing 2013-5-17 22:54:02
//注意使用工具提取PCX的图像，再拷贝到下面规定的位置
//lbdebug begin
//#include "Font.h"
//#include "Keyboard.h"
//lbdebug end
#include "Graphic.h"
#include "PCX256.h"
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;

/* ==================================== 拷贝pcx相关的数据到此处 begjin======================================= */

/* ==================================== 拷贝pcx相关的数据 end======================================= */

//Function Name: putPCX256
//Input:  x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//Description: display the 256 color pcx
//注意，这种显示方法速度有点慢，采用fast函数较好
VOID putPCX256(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
	UINT32 index,count;
	UINT16 num_bytes;
	UINT8 pcxdata;
	UINTN putX,putY;
	
	index=0;	//图像数据的指针
	count=0;	//用来定位像素位置的计数器
	// UINTN tempWidth;  //pcx编码中，每行字节数必须是偶数，以这个值来计算
	// if((Width % 2) == 0)
	// 	tempWidth = Width;
	// else
	// 	tempWidth += 1;

	//压缩区数据处理
	while(count<=(((UINT32)Width) * ((UINT32)Height)))
	{
		// get the first piece of data
		pcxdata = Picture[index];
		++index;
		// is this a rle?
		if(pcxdata>=192)
		{
			// how many bytes in run?
			num_bytes = pcxdata-192;
			// get the actual data for the run
			pcxdata  = Picture[index];;
			++index;
			// replicate data in buffer num_bytes times
			while(num_bytes-->0)
			{
				putX=x+(count%Width);
				putY=y+(count/Width);
				if(pcxdata!=MaskColor)	//Mask颜色不显示
					putpixel(putX,putY,&(ColorTable[pcxdata]));
				++count;
			} // end while
		} // end if rle
		else
		{
			// actual data, just copy it into buffer at next location
			putX=x+(count%Width);
			putY=y+(count/Width);
			if(pcxdata!=MaskColor)	//Mask颜色不显示
				putpixel(putX,putY,&(ColorTable[pcxdata]));
			++count;
		} // end else not rle
	} // end while 
}

//Function Name: putPCX256_fast
//Input:  x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//Description: display the 256 color pcx
//注意，MaskColor在这个函数中是没有作用的，当然也可以实现，但是
//   当前的函数应该可以满足不同环境下的要求，比如用内存拷贝来实现屏幕显示
//   之类的。需要实现时参考BMP256.c中的函数
VOID putPCX256_fast(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 index,count;
	UINT16 num_bytes;
	UINT8 pcxdata;
	EFI_STATUS Status;
	
	index=0;	//图像数据的指针
	count=0;	//用来定位像素位置的计数器
	BltBufferSize = ((UINT32)Width *  (UINT32)Height * (sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
	Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );//最大可以容纳2^30的256色pcx数据，即32678x32678的图，不会溢出

	//1 将数据解压到缓冲区中
	while(count<(((UINT32)Width) * ((UINT32)Height)))
	{
		// get the first piece of data
		pcxdata = Picture[index];
		++index;
		// is this a rle?
		if(pcxdata>=192)
		{
			// how many bytes in run?
			num_bytes = pcxdata-192;
			// get the actual data for the run
			pcxdata  = Picture[index];;
			++index;
			// replicate data in buffer num_bytes times
			while(num_bytes-->0)
			{
				BltBuffer[count].Blue =ColorTable[pcxdata].Blue;
				BltBuffer[count].Green =ColorTable[pcxdata].Green;
				BltBuffer[count].Red =ColorTable[pcxdata].Red;
				BltBuffer[count].Reserved =ColorTable[pcxdata].Reserved;
				count+=1;
			} // end while
		} // end if rle
		else
		{
			// actual data, just copy it into buffer at next location
			BltBuffer[count].Blue =ColorTable[pcxdata].Blue;
			BltBuffer[count].Green =ColorTable[pcxdata].Green;
			BltBuffer[count].Red =ColorTable[pcxdata].Red;
			BltBuffer[count].Reserved =ColorTable[pcxdata].Reserved;
			count+=1;
		} // end else not rle
	} // end while 

	//2 图像直接写屏
	gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,x,y,Width,Height,0);
//	FreePool(BltBuffer);
	gBS->FreePool(BltBuffer);
}

