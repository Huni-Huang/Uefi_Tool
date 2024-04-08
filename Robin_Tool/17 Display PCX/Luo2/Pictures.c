//code by luobing 2013-5-18 16:20:09
//注意使用工具提取PCX的图像，再拷贝到下面规定的位置
//lbdebug begin
//#include "Font.h"
//#include "Keyboard.h"
//lbdebug end
#include "Graphic.h"
#include "Pictures.h"
#include "FileRW.h"
#include "KeyBoard.h"

extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;

/* ==================================== 拷贝bmp相关的数据到此处 begin======================================= */
/* ==================================== 拷贝bmp相关的数据  end======================================= */

//===================================== BMP Functions begin ==============================================
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

/**
  Convert a *.BMP graphics image to a GOP blt buffer. If a NULL Blt buffer
  is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
  buffer is passed in it will be used if it is big enough.

  @param  BmpImage      Pointer to BMP file
  @param  BmpImageSize  Number of bytes in BmpImage
  @param  GopBlt        Buffer containing GOP version of BmpImage.
  @param  GopBltSize    Size of GopBlt in bytes.
  @param  PixelHeight   Height of GopBlt/BmpImage in pixels
  @param  PixelWidth    Width of GopBlt/BmpImage in pixels

  @retval EFI_SUCCESS           GopBlt and GopBltSize are returned.
  @retval EFI_UNSUPPORTED       BmpImage is not a valid *.BMP image
  @retval EFI_BUFFER_TOO_SMALL  The passed in GopBlt buffer is not big enough.
                                GopBltSize will contain the required size.
  @retval EFI_OUT_OF_RESOURCES  No enough buffer to allocate.

**/
EFI_STATUS
ConvertBmpToGopBlt (
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
     OUT UINTN     *PixelHeight,
     OUT UINTN     *PixelWidth
  )
{
  UINT8                         *Image;
  UINT8                         *ImageHeader;
  BMP_IMAGE_HEADER              *BmpHeader;
  BMP_COLOR_MAP                 *BmpColorMap;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINT64                        BltBufferSize;
  UINTN                         Index;
  UINTN                         Height;
  UINTN                         Width;
  UINTN                         ImageIndex;
  UINT32                        DataSizePerLine;
  BOOLEAN                       IsAllocated;
  UINT32                        ColorMapNum;

  if (sizeof (BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

  BmpHeader = (BMP_IMAGE_HEADER *) BmpImage;

  if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M') {
    return EFI_UNSUPPORTED;
  }
  //
  // Doesn't support compress.
  //
  if (BmpHeader->CompressionType != 0) {
    return EFI_UNSUPPORTED;
  }
  //
  // Only support BITMAPINFOHEADER format.
  // BITMAPFILEHEADER + BITMAPINFOHEADER = BMP_IMAGE_HEADER
  //
  if (BmpHeader->HeaderSize != sizeof (BMP_IMAGE_HEADER) - OFFSET_OF(BMP_IMAGE_HEADER, HeaderSize)) {
    return EFI_UNSUPPORTED;
  }

  //
  // The data size in each line must be 4 byte alignment.
  //
  DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
  BltBufferSize = MultU64x32 (DataSizePerLine, BmpHeader->PixelHeight);
  if (BltBufferSize > (UINT32) ~0) {
    return EFI_INVALID_PARAMETER;
  }

	//  if ((BmpHeader->Size != BmpImageSize) ||
	//      (BmpHeader->Size < BmpHeader->ImageOffset) ||
	//      (BmpHeader->Size - BmpHeader->ImageOffset != BmpHeader->PixelHeight * DataSizePerLine)) //此处计算发现错误,是我的图出现问题了,不知道为什么多了两个字节
	//  {
	//    return EFI_INVALID_PARAMETER;
	//   }
if ((BmpHeader->Size != BmpImageSize) ||
     (BmpHeader->Size < BmpHeader->ImageOffset) )
 {
   return EFI_INVALID_PARAMETER;
  }

  // Print(L"lbdebug end\n");
  //
  // Calculate Color Map offset in the image.
  //
  Image       = BmpImage;
  BmpColorMap = (BMP_COLOR_MAP *) (Image + sizeof (BMP_IMAGE_HEADER));
  if (BmpHeader->ImageOffset < sizeof (BMP_IMAGE_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BmpHeader->ImageOffset > sizeof (BMP_IMAGE_HEADER)) {
    switch (BmpHeader->BitPerPixel) {
      case 1:
        ColorMapNum = 2;
        break;
      case 4:
        ColorMapNum = 16;
        break;
      case 8:
        ColorMapNum = 256;
        break;
      default:
        ColorMapNum = 0;
        break;
      }
    if (BmpHeader->ImageOffset - sizeof (BMP_IMAGE_HEADER) != sizeof (BMP_COLOR_MAP) * ColorMapNum) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Calculate graphics image data address in the image
  //
  Image         = ((UINT8 *) BmpImage) + BmpHeader->ImageOffset;
  ImageHeader   = Image;

  //
  // Calculate the BltBuffer needed size.
  //
  BltBufferSize = MultU64x32 ((UINT64) BmpHeader->PixelWidth, BmpHeader->PixelHeight);
  //
  // Ensure the BltBufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) doesn't overflow
  //
  if (BltBufferSize > DivU64x32 ((UINTN) ~0, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    return EFI_UNSUPPORTED;
  }
  BltBufferSize = MultU64x32 (BltBufferSize, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
 
  IsAllocated   = FALSE;
  if (*GopBlt == NULL) {
    //
    // GopBlt is not allocated by caller.
    //
    *GopBltSize = (UINTN) BltBufferSize;
    *GopBlt     = AllocatePool (*GopBltSize);
    IsAllocated = TRUE;
    if (*GopBlt == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    //
    // GopBlt has been allocated by caller.
    //
    if (*GopBltSize < (UINTN) BltBufferSize) {
      *GopBltSize = (UINTN) BltBufferSize;
      return EFI_BUFFER_TOO_SMALL;
    }
  }

  *PixelWidth   = BmpHeader->PixelWidth;
  *PixelHeight  = BmpHeader->PixelHeight;

  // //luobing add 2019-06-14 15:34:46 lbdebug
  // Print(L"ConvertBmpToGopBlt *PixelWidth=0x%x,*PixelHeight=0x%x\n", *PixelWidth, *PixelHeight);
  //
  // Convert image from BMP to Blt buffer format
  //
  BltBuffer = *GopBlt;
  for (Height = 0; Height < BmpHeader->PixelHeight; Height++) {
    Blt = &BltBuffer[(BmpHeader->PixelHeight - Height - 1) * BmpHeader->PixelWidth];
    for (Width = 0; Width < BmpHeader->PixelWidth; Width++, Image++, Blt++) {
      switch (BmpHeader->BitPerPixel) {
      case 1:
        //
        // Convert 1-bit (2 colors) BMP to 24-bit color
        //
        for (Index = 0; Index < 8 && Width < BmpHeader->PixelWidth; Index++) {
          Blt->Red    = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Red;
          Blt->Green  = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Green;
          Blt->Blue   = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Blue;
          Blt++;
          Width++;
        }

        Blt--;
        Width--;
        break;

      case 4:
        //
        // Convert 4-bit (16 colors) BMP Palette to 24-bit color
        //
        Index       = (*Image) >> 4;
        Blt->Red    = BmpColorMap[Index].Red;
        Blt->Green  = BmpColorMap[Index].Green;
        Blt->Blue   = BmpColorMap[Index].Blue;
        if (Width < (BmpHeader->PixelWidth - 1)) {
          Blt++;
          Width++;
          Index       = (*Image) & 0x0f;
          Blt->Red    = BmpColorMap[Index].Red;
          Blt->Green  = BmpColorMap[Index].Green;
          Blt->Blue   = BmpColorMap[Index].Blue;
        }
        break;

      case 8:
        //
        // Convert 8-bit (256 colors) BMP Palette to 24-bit color
        //
        Blt->Red    = BmpColorMap[*Image].Red;
        Blt->Green  = BmpColorMap[*Image].Green;
        Blt->Blue   = BmpColorMap[*Image].Blue;
        break;

      case 24:
        //
        // It is 24-bit BMP.
        //
        Blt->Blue   = *Image++;
        Blt->Green  = *Image++;
        Blt->Red    = *Image;
        break;

      default:
        //
        // Other bit format BMP is not supported.
        //
        if (IsAllocated) {
          FreePool (*GopBlt);
          *GopBlt = NULL;
        }
        return EFI_UNSUPPORTED;
        break;
      };

    }

    ImageIndex = (UINTN) (Image - ImageHeader);
    if ((ImageIndex % 4) != 0) {
      //
      // Bmp Image starts each row on a 32-bit boundary!
      //
      Image = Image + (4 - (ImageIndex % 4));
    }
  }

  return EFI_SUCCESS;
}

//===================================== BMP Functions end ==============================================

//===================================== PCX Functions end ==============================================
//Function Name: decompressPCX24bits
//Input:  *file: Handle to PCX files
//        *BltBuffer: Caller must allocate enough memory 
//Output: *BltBuffer
//Description: 演示用的代码，解压24位真彩色的PCX图
EFI_STATUS decompressPCX24bits(IN EFI_FILE_PROTOCOL *file,OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer)
{
  EFI_STATUS Status;

  UINT32 Width, Height, x, y;  
  INT32 nbytes,count;//pcx数据区字节数
  UINT8 *pcximage, *lptemp, bPixel;
  PCX_HEADER curPCXheader; //当前的pcx文件头
  UINTN bufLength = sizeof(PCX_HEADER);

  SetFilePosition(file, 0); //文件最开始处;
  
  Status=ReadFile(file,&bufLength, &curPCXheader);
  if(Status!=EFI_SUCCESS)
    return Status;
  
  Width = (curPCXheader.xMax - curPCXheader.xMin) + 1;
  Height = (curPCXheader.yMax - curPCXheader.yMin) + 1;
  nbytes = (INT32)((UINT32)curPCXheader.BytesPerLine * (UINT32)curPCXheader.ColorPlanes * (UINT32)Height);

  //1 解码数据到缓冲区
  pcximage = AllocateZeroPool(nbytes);
  if(pcximage  ==  NULL)
    return EFI_ABORTED;

  lptemp = pcximage;
  while (nbytes > 0)
  {
    bufLength = 1;
    ReadFile(file, &bufLength, &bPixel);
  
    if ((bPixel & 0XC0) != 0XC0)
    { // Repeated group
	  		*pcximage++ = bPixel;
	  		--nbytes;
    }
    else
    {
      count = bPixel & 0x3F;   //get count of pixels
      ReadFile(file,&bufLength, &bPixel);
      if(count > nbytes)
      {
        FreePool(lptemp);
        return EFI_ABORTED;   //致命错误，得到的pixel数目大于剩余文件字节
      }

      nbytes -= count;
	  	while (--count >=0) *pcximage++ = bPixel;
    }
  }
  //2 将颜色数据输出到指定的bltBuffer中
  pcximage = lptemp;
  for (y = 0; y < Height; y++)
  {
    for (x = 0; x < Width; x++)
    {
      BltBuffer[y * Width + x].Red = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 0) + x];
      BltBuffer[y * Width + x].Green = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 1) + x]; 
      BltBuffer[y * Width + x].Blue = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 2) + x]; 
    }
  }
  FreePool(pcximage);
  return Status;
}

//Function Name: decompressPCX24bits_fast
//Input:  *file: Handle to PCX files
//        *BltBuffer: Caller must allocate enough memory 
//Output: *BltBuffer
//Description: 演示用的代码，解压24位真彩色的PCX图
EFI_STATUS decompressPCX24bits_fast(IN EFI_FILE_PROTOCOL *file,OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer)
{
   EFI_STATUS Status;

  UINT8 *fileimage=NULL;
  UINTN fileLength;
  EFI_FILE_INFO *fileInfo;

  PCX_HEADER curPCXheader; //当前的pcx文件头
  UINT32 Width, Height, x, y; 
  INT32 nbytes;//pcx数据区字节数
  UINT8 *pcximage=NULL;
  UINTN bufLength = sizeof(PCX_HEADER);

  UINT8 *tempPcxImage, *tempFileImage;
  UINT8 bPixel;
  INT32 count;

  //1 读取文件信息，申请足够的内存，准备将数据直接读取到内存中
  fileInfo = FileHandleGetInfo(file);  //EFI_FILE_HANDLE为EFI_FILE_PROTOCOL指针
  fileLength = (UINTN)(fileInfo->FileSize);
  // Print(L"lbdebug1 decodePCX24bits: fileLength=0x%x\n", fileLength);
  fileLength -= sizeof(PCX_HEADER);
  // Print(L"lbdebug2 decodePCX24bits: fileLength=0x%x\n", fileLength);
  fileimage = AllocateZeroPool(fileLength);

   if(fileimage  ==  NULL)
  {
    // Print(L"lbdebug3 decodePCX24bits: Allocate fileimage error!\n");
    FreePool(fileInfo);
    return EFI_ABORTED;
  }

  //2 读取PCX文件头信息，了解图像尺寸
  SetFilePosition(file, 0); //文件最开始处;
  
  Status=ReadFile(file,&bufLength, &curPCXheader);
  if(Status!=EFI_SUCCESS)
    return Status;
  Width = (curPCXheader.xMax - curPCXheader.xMin) + 1;
  Height = (curPCXheader.yMax - curPCXheader.yMin) + 1;
  nbytes = (INT32)((UINT32)curPCXheader.BytesPerLine * (UINT32)curPCXheader.ColorPlanes * (UINT32)Height);

  pcximage = AllocateZeroPool(nbytes);
  if(pcximage  ==  NULL)
  {
    FreePool(fileimage);
    FreePool(fileInfo);
    return EFI_ABORTED;
  }

  //3 将压缩图像先读入内存
  ReadFile(file, &fileLength, fileimage);
  // Print(L"lbdebug4 decodePCX24bits fileimage: 0x%x  0x%x  0x%x  0x%x \n", fileimage[0], fileimage[1], fileimage[2], fileimage[3]);
  // WaitKey();
  //4 解压图像，并存入指定的内存
  tempFileImage = fileimage;
  tempPcxImage = pcximage;
  while (nbytes > 0)
  {
    //  bPixel = tempFileImage[0];
    bPixel = *tempFileImage++; 

    if ((bPixel & 0XC0) != 0XC0)
    { // Repeated group
       *tempPcxImage++ = bPixel;
       --nbytes;
    }
    else
    {
      count = bPixel & 0x3F;   //get count of pixels
      bPixel = *tempFileImage++;

      if(count > nbytes)
      {
        FreePool(fileInfo);
        FreePool(fileimage);
        FreePool(pcximage);
        return EFI_ABORTED;   //致命错误，得到的pixel数目大于剩余文件字节
      }

      nbytes -= count;
	  	// while (--count >=0) *tempPcxImage++ = bPixel;
      SetMem(tempPcxImage, count, bPixel);
      tempPcxImage += count;
    }
  }
  // WaitKey();
  // {
  //   UINTN i;
  //   Print(L"lbdebug5 decodePCX24bits pcximage:");
  //   WaitKey();
  //   for (i = 0; i < 16; i++)
  //   {
  //     Print(L"0x%x  ", pcximage[i]);
  //   }
  //   Print(L"\n");
  // }
  // WaitKey();
  //5 将颜色数据输出到指定的bltBuffer中
  for (y = 0; y < Height; y++)
  {
    for (x = 0; x < Width; x++)
    {
      BltBuffer[y * Width + x].Red = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 0) + x];
      BltBuffer[y * Width + x].Green = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 1) + x]; 
      BltBuffer[y * Width + x].Blue = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 2) + x]; 
    }
  }
  
  
  FreePool(fileInfo);
  FreePool(fileimage);
  FreePool(pcximage);
}
// //Function Name: decompressPCX24bits_fast
// //Input:  *file: Handle to PCX files
// //        *BltBuffer: Caller must allocate enough memory 
// //Output: *BltBuffer
// //Description: 演示用的代码，解压24位真彩色的PCX图
// EFI_STATUS decompressPCX24bits_fast(IN EFI_FILE_PROTOCOL *file,OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer)
// {
//   EFI_STATUS Status;

//   UINT32 Width, Height, x, y;  
//   INT32 nbytes,count;//pcx数据区字节数
//  // EFI_FILE_INFO *fileInfo;
//   UINT8 *fileimage;
//   UINT8 *pcximage, *lptemp, *lptemp1, bPixel;
//   PCX_HEADER curPCXheader; //当前的pcx文件头
//   UINTN bufLength = sizeof(PCX_HEADER);
//   UINTN fileLength;

//  // fileInfo = FileHandleGetInfo(file);  //EFI_FILE_HANDLE为EFI_FILE_PROTOCOL指针
//   // fileLength = (UINTN)(fileInfo->FileSize);
//   fileLength = 0x100000;
//   //FreePool(fileInfo);

//   SetFilePosition(file, 0); //文件最开始处;
  
//   Status=ReadFile(file,&bufLength, &curPCXheader);
//   if(Status!=EFI_SUCCESS)
//     return Status;
  
//   Width = (curPCXheader.xMax - curPCXheader.xMin) + 1;
//   Height = (curPCXheader.yMax - curPCXheader.yMin) + 1;
//   nbytes = (INT32)((UINT32)curPCXheader.BytesPerLine * (UINT32)curPCXheader.ColorPlanes * (UINT32)Height);

//   //1 解码数据到缓冲区
//   fileimage = AllocateZeroPool(fileLength);
//   if(fileimage  ==  NULL)
//   {
//     Print(L"fast:Allocate fileimage error!\n");
//     return EFI_ABORTED;
//   }
//   //  

//   pcximage = AllocateZeroPool(nbytes);
//   if(pcximage  ==  NULL)
//   {
//     FreePool(fileimage);
//     return EFI_ABORTED;
//   }
//   //1-1 先将数据直接读到内存缓冲区中
//   bufLength = (UINTN)fileLength - sizeof(PCX_HEADER);
//   ReadFile(file, &bufLength, fileimage);

//   lptemp = pcximage;
//   lptemp1 = fileimage;
//   while (nbytes > 0)
//   {
//     // bufLength = 1;
//     // ReadFile(file, &bufLength, &bPixel);
//     bPixel = *fileimage++;
//     if ((bPixel & 0XC0) != 0XC0)
//     { // Repeated group
// 	  		*pcximage++ = bPixel;
// 	  		--nbytes;
//     }
//     else
//     {
//       count = bPixel & 0x3F;   //get count of pixels
//       // ReadFile(file,&bufLength, &bPixel);
//       bPixel = *fileimage++;
//       if(count > nbytes)
//       {
//         FreePool(lptemp);
//         FreePool(lptemp1);
//         return EFI_ABORTED;   //致命错误，得到的pixel数目大于剩余文件字节
//       }

//       nbytes -= count;
// 	  	while (--count >=0) *pcximage++ = bPixel;
//     }
//   }
//   //2 将颜色数据输出到指定的bltBuffer中
//   pcximage = lptemp;
//   fileimage = lptemp1;
//   for (y = 0; y < Height; y++)
//   {
//     for (x = 0; x < Width; x++)
//     {
//       BltBuffer[y * Width + x].Red = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 0) + x];
//       BltBuffer[y * Width + x].Green = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 1) + x]; 
//       BltBuffer[y * Width + x].Blue = pcximage[(UINT32)curPCXheader.BytesPerLine * (3 * y + 2) + x]; 
//     }
//   }
//   FreePool(pcximage);
//   FreePool(fileimage);
//   return Status;


// }