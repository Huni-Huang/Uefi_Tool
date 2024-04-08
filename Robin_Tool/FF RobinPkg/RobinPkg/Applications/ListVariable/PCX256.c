//code by luobing 2013-5-17 22:54:02
//ע��ʹ�ù�����ȡPCX��ͼ���ٿ���������涨��λ��
//lbdebug begin
//#include "Font.h"
//#include "Keyboard.h"
//lbdebug end
#include "Graphic.h"
#include "PCX256.h"
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;

/* ==================================== ����pcx��ص����ݵ��˴� begjin======================================= */

/* ==================================== ����pcx��ص����� end======================================= */

//Function Name: putPCX256
//Input:  x,y,Width,Height,ColorTable=Pointer to logo color table,
//       Picture=picture of logo,MaskColor=not display color in Picture
//Output: None
//Description: display the 256 color pcx
//ע�⣬������ʾ�����ٶ��е���������fast�����Ϻ�
VOID putPCX256(UINTN x,UINTN y,UINTN Width,UINTN Height,
              EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorTable,
              UINT8 *Picture,
              UINT8 MaskColor)
{
	UINT32 index,count;
	UINT16 num_bytes;
	UINT8 pcxdata;
	UINTN putX,putY;
	
	index=0;	//ͼ�����ݵ�ָ��
	count=0;	//������λ����λ�õļ�����
	// UINTN tempWidth;  //pcx�����У�ÿ���ֽ���������ż���������ֵ������
	// if((Width % 2) == 0)
	// 	tempWidth = Width;
	// else
	// 	tempWidth += 1;

	//ѹ�������ݴ���
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
				if(pcxdata!=MaskColor)	//Mask��ɫ����ʾ
					putpixel(putX,putY,&(ColorTable[pcxdata]));
				++count;
			} // end while
		} // end if rle
		else
		{
			// actual data, just copy it into buffer at next location
			putX=x+(count%Width);
			putY=y+(count/Width);
			if(pcxdata!=MaskColor)	//Mask��ɫ����ʾ
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
//ע�⣬MaskColor�������������û�����õģ���ȻҲ����ʵ�֣�����
//   ��ǰ�ĺ���Ӧ�ÿ������㲻ͬ�����µ�Ҫ�󣬱������ڴ濽����ʵ����Ļ��ʾ
//   ֮��ġ���Ҫʵ��ʱ�ο�BMP256.c�еĺ���
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
	
	index=0;	//ͼ�����ݵ�ָ��
	count=0;	//������λ����λ�õļ�����
	BltBufferSize = ((UINT32)Width *  (UINT32)Height * (sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
	Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );//����������2^30��256ɫpcx���ݣ���32678x32678��ͼ���������

	//1 �����ݽ�ѹ����������
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

	//2 ͼ��ֱ��д��
	gGraphicsOutput->Blt (gGraphicsOutput,BltBuffer,EfiBltBufferToVideo,0,0,x,y,Width,Height,0);
//	FreePool(BltBuffer);
	gBS->FreePool(BltBuffer);
}

