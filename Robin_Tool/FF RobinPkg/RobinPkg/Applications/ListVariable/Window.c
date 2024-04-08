//code by luobing 2013-5-15 10:19:35

#include "Window.h"
#include "Graphic.h"

UINT32 SYGraphicsMode;
UINT32 OldGraphicsMode;
UINT32 SYPixPerScanLine;

//============================== Global color table ================================
EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[]={
  0,    0,    0,    0,        /*  BLACK */\
  255,  255,  255,  0,        /*  WHITE */\
  200,  208,  212,  0,        /*  LIGHT GRAY  */\
  167,  95, 18, 0,        /*  DEEP BLUE */\
  64, 64, 64, 0,        /*  DARK BLACK  */\
  128,  128,  128,  0,        /*  LIGHT BLACK */\
  0,    255,  255,  0,        /*  YELLOW  */\
  0,    153,  0,    0,        /*  GREEN */\
  0,    0,    153,  0,        /*  RED   */\
  192,  0,    0,    0,        /* BLUE   */  
};

//Function Name: SetBKG
//Input: color
//Output: None
//Description: set background
VOID SetBKG(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *color)
{
  rectblock(0,0,SY_SCREEN_WIDTH-1,SY_SCREEN_HEIGHT-1,color);
}
//Function Name: InSYGraphicsMode
//Input
//Output:
//Description: set mode
VOID InSYGraphicsMode(void)
{
	GetGraphicModeNumber(SY_SCREEN_WIDTH,SY_SCREEN_HEIGHT,&SYGraphicsMode,&OldGraphicsMode,&SYPixPerScanLine);
	SetMyMode(SYGraphicsMode);
}
//Function Name: OutSYGraphicsMode
//Input
//Output:
//Description: set old mode
VOID OutSYGraphicsMode(void)
{
	//GetGraphicModeNumber(SY_SCREEN_WIDTH,SY_SCREEN_HEIGHT,&SYGraphicsMode,&OldGraphicsMode,&SYPixPerScanLine);
	SetMyMode(OldGraphicsMode);
}


//=======ʵ����Ч��һЩ����������Graphic���ṩ�ĵײ㺯��ʵ��==========
//Function Name: SimpleTransparent
//Input: x1,y1,Width,Height,*ImagePointer
//Output:
//Description: ImagePointer,ָ��Width*Height��С��ͼ�񻺳��������С�ض�ΪWidth*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
//ʵ����򵥵�͸��Ч������ʹ��Alpha��ʽ��AlphaֵΪ1/2
VOID SimpleTransparent(IN UINTN x,IN UINTN y,IN UINTN Width,IN UINTN Height,
									IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ImagePointer)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 i;
	
	//1 ����ͬ����С���ڴ棬׼��ʹ��
	BltBufferSize =(UINT32) (Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
  //2 Ҫ���������ͼ����ȡ
  getRectImage(x,y,Width,Height,BltBuffer); 
  //3 �������ڴ���д���,ѡȡAlpha=0.5   
  //Alpha��Ϲ�ʽ���£�(A������B��)
	//                  R(C)=(1-alpha)*R(B) + alpha*R(A)
	//                  G(C)=(1-alpha)*G(B) + alpha*G(A)
	//                  B(C)=(1-alpha)*B(B) + alpha*B(A)
	for(i=0;i<((UINT32)Width * (UINT32)Height);i++)
	{
		BltBuffer[i].Blue=((BltBuffer[i].Blue)>>1) + ((ImagePointer[i].Blue)>>1);
		BltBuffer[i].Green=((BltBuffer[i].Green)>>1) + ((ImagePointer[i].Green)>>1);
		BltBuffer[i].Red=((BltBuffer[i].Red)>>1) + ((ImagePointer[i]).Red>>1);
		BltBuffer[i].Reserved=((BltBuffer[i].Reserved)>>1) + ((ImagePointer[i].Reserved)>>1);
	}
	//3 ��ʾ
	putRectImage(x,y,Width,Height,BltBuffer); 
	
  gBS->FreePool(BltBuffer);
}

//Function Name: AlphaTransparent
//Input: x1,y1,Width,Height,*ImagePointer,Alpha
//Output:
//Description: ImagePointer,ָ��Width*Height��С��ͼ�񻺳��������С�ض�ΪWidth*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
//ʵ��͸��Ч����ʹ��Alpha��ʽ��Alphaֵ�ɵ�,��0-16�ɵ�
VOID AlphaTransparent(IN UINTN x,IN UINTN y,IN UINTN Width,IN UINTN Height,
									IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ImagePointer,
									IN UINT16 Alpha)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 i;
	
	//1 ����ͬ����С���ڴ棬׼��ʹ��
	BltBufferSize = (UINT32)(Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
  //2 Ҫ���������ͼ����ȡ
  getRectImage(x,y,Width,Height,BltBuffer); 
  //3 �������ڴ���д���,ѡȡAlpha����ĸ16,���ڼ���   
  //Alpha��Ϲ�ʽ���£�(A������B��)
	//                  R(C)=(1-alpha)*R(B) + alpha*R(A)
	//                  G(C)=(1-alpha)*G(B) + alpha*G(A)
	//                  B(C)=(1-alpha)*B(B) + alpha*B(A)
	for(i=0;i<((UINT32)Width * (UINT32)Height);i++)
	{
		BltBuffer[i].Blue=(UINT8)(((16-Alpha)* BltBuffer[i].Blue + Alpha * ImagePointer[i].Blue)>>4);
		BltBuffer[i].Green=(UINT8)(((16-Alpha)* BltBuffer[i].Green + Alpha * ImagePointer[i].Green)>>4);
		BltBuffer[i].Red=(UINT8)(((16-Alpha)* BltBuffer[i].Red + Alpha * ImagePointer[i].Red)>>4);
		BltBuffer[i].Reserved=(UINT8)(((16-Alpha)* BltBuffer[i].Reserved + Alpha * ImagePointer[i].Reserved)>>4);
	}
	//3 ��ʾ
	putRectImage(x,y,Width,Height,BltBuffer); 
	
  gBS->FreePool(BltBuffer);
}

//Function Name: DarkSpecifyZone
//Input: x1,y1,Width,Height,flag
//Output:
//Description:  Make the specify zone dark if flag=TRUE, otherwise,Light the zone
VOID DarkSpecifyZone(IN UINTN x,IN UINTN y,IN UINTN Width,IN UINTN Height,BOOLEAN flag)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 i;
	UINT8 value;
	
	if(flag)
		value=0;
	else
		value=255;
	//1 ������ϴ�С���ڴ棬׼��ʹ��
	BltBufferSize =(UINT32) (Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
  //2 Ҫ���������ͼ����ȡ
  getRectImage(x,y,Width,Height,BltBuffer); 
  //3 ���ڴ���а�������  
  
	for(i=0;i<((UINT32)Width * (UINT32)Height);i+=2)
	{
		BltBuffer[i].Blue=value;
		BltBuffer[i].Green=value;
		BltBuffer[i].Red=value;
		BltBuffer[i].Reserved=value;
	}
	//4 ��ʾ
	putRectImage(x,y,Width,Height,BltBuffer); 
	
  gBS->FreePool(BltBuffer);
}

//Function Name: SmoothOutZone
//Input: x1,y1,Width,Height
//Output:
//Description: ��ָ�������𽥱�ڣ�ʵ�ֽ���Ч��
VOID SmoothOutZone(IN UINTN x,IN UINTN y,IN UINTN Width,IN UINTN Height)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	UINT32                        BltBufferSize;
	UINT32 i;
	UINT16 j;
	//1 ������ϴ�С���ڴ棬׼��ʹ��
	BltBufferSize = (UINT32)(Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
  //2 Ҫ���������ͼ����ȡ
  getRectImage(x,y,Width,Height,BltBuffer); 
  //3 ���ڴ����ؽ��а�������,ͬʱ��ʾ  
  for(j=0;j<256;j++)
  {
		for(i=0;i<((UINT32)Width * (UINT32)Height);i++)
		{
			if(BltBuffer[i].Blue>0)--BltBuffer[i].Blue;
			if(BltBuffer[i].Green>0)--BltBuffer[i].Green;
			if(BltBuffer[i].Red>0)--BltBuffer[i].Red;
			if(BltBuffer[i].Reserved>0)--BltBuffer[i].Reserved;
		}
		putRectImage(x,y,Width,Height,BltBuffer); 
		Delayms(9);
	}	
  gBS->FreePool(BltBuffer);
}

//Function Name: SmoothInZone
//Input: x1,y1,Width,Height
//Output:
//Description: ��ָ�������𽥴Ӻڱ�Ϊʵ��ͼ�Σ�ʵ�ֽ���Ч��
//ע�⣬��ǰ������ԭ����ȡ��Ļ->����Ļ���->����Ϊԭ��ͼ�����������Ǹ������ú�����ʵ��
//  ��Ҫ��ʱ��Ҫ��������
VOID SmoothInZone(IN UINTN x,IN UINTN y,IN UINTN Width,IN UINTN Height)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer1;
	UINT32                        BltBufferSize;
	UINT32 i;
	UINT16 j;
	//1 ������ϴ�С���ڴ棬׼��ʹ��
	BltBufferSize = (UINT32)(Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
  gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer1
                  );
  gBS->SetMem(BltBuffer1,BltBufferSize,0);
  //2 Ҫ���������ͼ����ȡ,����������
  getRectImage(x,y,Width,Height,BltBuffer);
  putRectImage(x,y,Width,Height,BltBuffer1); 
  //3 ���ڴ����ؽ��а�������,ͬʱ��ʾ  
  for(j=0;j<256;j++)
  {
		for(i=0;i<((UINT32)Width * (UINT32)Height);i++)
		{
			if(BltBuffer1[i].Blue			<BltBuffer[i].Blue)			++BltBuffer1[i].Blue;
			if(BltBuffer1[i].Green		<BltBuffer[i].Green)		++BltBuffer1[i].Green;
			if(BltBuffer1[i].Red			<BltBuffer[i].Red)			++BltBuffer1[i].Red;
			if(BltBuffer1[i].Reserved	<BltBuffer[i].Reserved)	++BltBuffer1[i].Reserved;
		}
		putRectImage(x,y,Width,Height,BltBuffer1); 
		Delayms(12);
	}	
  gBS->FreePool(BltBuffer);
  gBS->FreePool(BltBuffer1);
}
