/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/




#include "common.h"
#include "FileRW.h"
#include "KeyBoard.h"
#include "Graphic.h"
#include "Window.h"
#include "PciRW.h"
#include "Pictures.h"
#include "PCX256.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/BaseLib.h>  //字符串转换
#include  <Library/ShellCEntryLib.h>

#include <Pi/PiFirmwareFile.h>


#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>

#define MyPic                      L"luobing.bmp"		//where the my bmp located 
// EFI_STATUS ShowBmp (void);


extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
// INTN
// EFIAPI
// ShellAppMain (
//   IN UINTN Argc,
//   IN CHAR16 **Argv
//   )
int
main (
  IN int Argc,
  IN char **Argv
  )
{
  UINT64 flag;
  int i;
  

  // Print(L"Hello there fellow Programmer.\n");
  // Print(L"Welcome to the world of EDK II.\n");
  // Print(L"Argc=%d\n", Argc);
  // for (i = 0; i < Argc;i++)
  // {
  //   Print(L"Argv[%d]= %s\n", i,Argv[i]);
  // }
    
        flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT | PCI_ROOTBRIDGE_IO | PCI_IO | FILE_IO);
	Print(L"flag=%x\n",flag);
	WaitKey();
  {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bltBuffer;
    void *jfif = NULL;
    BMP_FORJPG   bmp  = {0};
    int ret;
    int x, y, bytesPerLine;
    UINT32 position, index, jpgWidth,jpgHeight;
    UINT8 *bmpdata;

    printf("Argc=%d\n", Argc);
    for (i = 0; i < Argc;i++)
    {
      printf("internal:Argv[%d]= %s\n", i,Argv[i]);
    }
    WaitKey();
    //1 打开jpeg文件，并转换到对应的bmp文件中
    jfif = jfif_load(Argv[1]);
    ret=jfif_decode(jfif, &bmp);
    printf("ret of jfif_decode=%d\n", ret);
    WaitKey();
    jfif_free  (jfif);

    //2 将对应的图转换到bltBuffer中
    // bmp_save(&bmp, "decode.bmp");  //测试用的
    //24位bmp每行的字节数能整除4，颜色以3个字节Bleue  Green  Red依次存储
    if((bmp.width * 3) % 4 ==0)
      bytesPerLine = bmp.width * 3;
    else
      bytesPerLine = bmp.width * 3 + 4 - ((bmp.width * 3) % 4);
    //当然，也可以直接使用已经算出来的数据，也即：
    // bytesPerLine = bmp.stride;
    
    bltBuffer = AllocateZeroPool(sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)* (bmp.height) * (bmp.width));
    index = 0;
    bmpdata = (UINT8 *)(bmp.pdata);
    for (y = 0; y < bmp.height; y++)
    {
      position = y * bytesPerLine;

      for (x = 0; x < bmp.width; x++)
      {
        bltBuffer[index].Blue = bmpdata[position];
        ++position;
        bltBuffer[index].Green = bmpdata[position];
        ++position;
        bltBuffer[index].Red = bmpdata[position];
        ++position;
        bltBuffer[index].Reserved = 0;
        ++index;
      }
    }
    jpgHeight = bmp.height;
    jpgWidth = bmp.width;
    bmp_free(&bmp);
    
    //3 显示
    SwitchGraphicsMode(TRUE);
    SetMyMode(0x3);
		SetBKG(&(gColorTable[3]));
    //  putRectImage(00, 400, jpgWidth, jpgHeight, bltBuffer);
    // putImageLine(0, 0, jpgWidth, jpgHeight, 3, 10, 400, 300, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, bltBuffer);
    // putImageLine(0, 0, jpgWidth, jpgHeight, 100, 4, 100, 400, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, bltBuffer);
    // {
    //   UINTN  y;
    //   for (y = 0; y < jpgHeight;y++)
    //     putImageLine(100, 100, jpgWidth, jpgHeight, 0, y, jpgWidth-1, y, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, bltBuffer);
    // }
    // Line(0, 0, 200, 200, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, &(gColorTable[RED]));
    // Line(3, 3, 1200, 100, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, &(gColorTable[RED]));
    // WaitKey();

        
    //3-1 灰度转换
    // TransferToGray(100, 100, jpgWidth, jpgHeight);
    // WaitKey();
    //3-2 上下翻转、左右翻转、同时上下左右翻转
    // ReviseImage(100, 100, jpgWidth, jpgHeight,1);
    // WaitKey();
    // putRectImage(100, 100, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // ReviseImage(100, 100, jpgWidth, jpgHeight,2);
    // WaitKey();
    // putRectImage(100, 100, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // ReviseImage(100, 100, jpgWidth, jpgHeight,3);
    // WaitKey();
    //3-3 透明演示
    // AlphaTransparent(100,100,jpgWidth,jpgHeight,bltBuffer,10);
    // WaitKey();
    // AlphaTransparent(200,200,jpgWidth,jpgHeight,bltBuffer,5);
    // WaitKey();
    
    //3-4 拉幕效果
    // hOpenCurtain(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // for (index = 0; index < jpgWidth; index++)
    // {
    //   VLine(index, 0, jpgHeight - 1, &(gColorTable[3]));
    //   Delayms(2);
    // }
    // WaitKey();
    // vOpenCurtain(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // for (index = 0; index < jpgHeight; index++)
    // {
    //   HLine(0, jpgWidth-1, index, &(gColorTable[3]));
    //   Delayms(2);
    // }
    // WaitKey();
    //3-5 雨滴效果
    // RainFallShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // SetBKG(&(gColorTable[3]));
    // RainAscendShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    //3-6 百叶窗显现和消隐
    // SetBKG(&(gColorTable[3]));
    // hWindowShadesShow(100, 100, jpgWidth, jpgHeight, bltBuffer);//横向百叶窗
    // WaitKey();
    // {
    //   //消隐
    //   INTN i, j;
    //   for(j=(INTN)(jpgWidth/10-1);j>=0;j--)
    //     for (i = 0; i < 10;i++)
    //     {
    //         rectblock(i * jpgWidth / 10 + j, 0, i * jpgWidth / 10 + j, jpgHeight - 1,&(gColorTable[3]));
    //         Delayms(2);
    //     }
         
    // }
    // WaitKey();
    // vWindowShadesShow(100, 100, jpgWidth, jpgHeight, bltBuffer); //纵向百叶窗
    // WaitKey();
    // {
    //   //消隐
    //   INTN i, j;
    //   for(j=(INTN)(jpgHeight/10);j>=0;j--)
    //     for (i = 0; i < 10;i++)
    //     {
    //         rectblock(0,i * jpgHeight / 10 + j, jpgWidth-1, i * jpgHeight / 10 + j,&(gColorTable[3]));
    //         Delayms(2);
    //     }
         
    // }
    // WaitKey();
    //3-7 从四角向中间显示
    // CornerToCenter(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // SetBKG(&(gColorTable[3]));
    // CenterToCorner(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    //3-8 交错显示
    // hInterlaceShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // SetBKG(&(gColorTable[3]));
    // vInterlaceShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    //3-8 相互交错显示
    // hInteractiveShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    // SetBKG(&(gColorTable[3]));
    // vInteractiveShow(0, 0, jpgWidth, jpgHeight, bltBuffer);
    // WaitKey();
    //3-9 螺旋显示
    // SpiralShow(0, 0, jpgWidth, jpgHeight, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, bltBuffer);
    // WaitKey();
    // SpiralShowClear(0, 0, jpgWidth, jpgHeight, SY_SCREEN_WIDTH - 1, SY_SCREEN_HEIGHT - 1, &(gColorTable[3]));
    // WaitKey();
    //3-10 渐隐
    SetBKG(&(gColorTable[BLACK]));
    putRectImage(0, 0, jpgWidth, jpgHeight, bltBuffer);
    WaitKey();
    // DarkSpecifyZone(0,0,jpgWidth, jpgHeight,TRUE);
		// WaitKey();
		// DarkSpecifyZone(0,0,jpgWidth, jpgHeight,FALSE);
		// WaitKey();
    SmoothInZone(0,0,jpgWidth, jpgHeight);
		WaitKey();
    SmoothOutZone(0,0,jpgWidth, jpgHeight);
		WaitKey();
    FreePool(bltBuffer);
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);     
  }
  {
extern UINT16 gSYBMP01_Width; //bmp图像的宽度 
extern UINT16 gSYBMP01_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable01[];
extern UINT8 gSYBMPPicture01[];
extern UINT16 gSYBMP02_Width; //bmp图像的宽度 
extern UINT16 gSYBMP02_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable02[];
extern UINT8 gSYBMPPicture02[];
extern UINT16 gSYBMP03_Width; //bmp图像的宽度 
extern UINT16 gSYBMP03_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable03[];
extern UINT8 gSYBMPPicture03[];
extern UINT16 gSYBMP04_Width; //bmp图像的宽度 
extern UINT16 gSYBMP04_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable04[];
extern UINT8 gSYBMPPicture04[];
extern UINT16 gSYBMP05_Width; //bmp图像的宽度 
extern UINT16 gSYBMP05_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable05[];
extern UINT8 gSYBMPPicture05[];
extern UINT16 gSYBMP06_Width; //bmp图像的宽度 
extern UINT16 gSYBMP06_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable06[];
extern UINT8 gSYBMPPicture06[];
extern UINT16 gSYBMP07_Width; //bmp图像的宽度 
extern UINT16 gSYBMP07_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable07[];
extern UINT8 gSYBMPPicture07[];
extern UINT16 gSYBMP08_Width; //bmp图像的宽度 
extern UINT16 gSYBMP08_Height; //bmp图像的高度
extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gSYBMPColorTable08[];
extern UINT8 gSYBMPPicture08[];
#define DelayTImes 50
		//休息一下，哈哈
		EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
		UINT32                        BltBufferSize;
		UINT32 i,j;
		SwitchGraphicsMode(TRUE);
		InSYGraphicsMode();
		SetBKG(&(gColorTable[3]));
		putPCX256_fast(100,100);
		//
		BltBufferSize = 200*200*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
		gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  BltBufferSize,
                  (VOID **) &BltBuffer
                  );
    for(i=0;i<100;i++)
    	for(j=0;j<100;j++)
    	{
    		getRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP01_Width,gSYBMP01_Height,gSYBMPColorTable01,gSYBMPPicture01,0x62);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP02_Width,gSYBMP02_Height,gSYBMPColorTable02,gSYBMPPicture02,0x4f);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP03_Width,gSYBMP03_Height,gSYBMPColorTable03,gSYBMPPicture03,0xec);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP04_Width,gSYBMP04_Height,gSYBMPColorTable04,gSYBMPPicture04,0xd5);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP05_Width,gSYBMP05_Height,gSYBMPColorTable05,gSYBMPPicture05,0xb7);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP06_Width,gSYBMP06_Height,gSYBMPColorTable06,gSYBMPPicture06,0x6e);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP07_Width,gSYBMP07_Height,gSYBMPColorTable07,gSYBMPPicture07,0xd6);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    		putBMP256_fun(j*8,i*5,gSYBMP08_Width,gSYBMP08_Height,gSYBMPColorTable08,gSYBMPPicture08,0xf6);
    		Delayms(DelayTImes);
    		putRectImage(j*8,i*5,200,200,BltBuffer);
    	}             
    gBS->FreePool(BltBuffer);
		//
		OutSYGraphicsMode();
		SwitchGraphicsMode(FALSE);
	}
  
  return(0);
}
