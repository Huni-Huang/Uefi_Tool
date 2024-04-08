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

#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/BaseLib.h>  //�ַ���ת��
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
    //1 ��jpeg�ļ�����ת������Ӧ��bmp�ļ���
    jfif = jfif_load(Argv[1]);
    ret=jfif_decode(jfif, &bmp);
    printf("ret of jfif_decode=%d\n", ret);
    WaitKey();
    jfif_free  (jfif);

    //2 ����Ӧ��ͼת����bltBuffer��
    // bmp_save(&bmp, "decode.bmp");  //�����õ�
    //24λbmpÿ�е��ֽ���������4����ɫ��3���ֽ�Bleue  Green  Red���δ洢
    if((bmp.width * 3) % 4 ==0)
      bytesPerLine = bmp.width * 3;
    else
      bytesPerLine = bmp.width * 3 + 4 - ((bmp.width * 3) % 4);
    //��Ȼ��Ҳ����ֱ��ʹ���Ѿ�����������ݣ�Ҳ����
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
    
    //3 ��ʾ
    SwitchGraphicsMode(TRUE);
    SetMyMode(0x3);
		SetBKG(&(gColorTable[3]));
    putRectImage(100, 100, jpgWidth, jpgHeight, bltBuffer);
    
    WaitKey();
    FreePool(bltBuffer);
		SetMyMode(0x0);
		SwitchGraphicsMode(FALSE);     
  }
  // {
  //   //1 ��ͼ�����ݴ�
  //   CHAR16 *buffStr;
  //   EFI_STATUS Status;
  //   EFI_FILE_PROTOCOL *file;
  //   // EFI_FILE_INFO *fileInfo;
  //   EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bltBuffer;
  //   UINT32 Width, Height;

  //   PCX_HEADER PCXheader; //��ǰ��pcx�ļ�ͷ
  //   UINTN bufLength = sizeof(PCX_HEADER);

  //   //1-1 ��pcx�ļ�
  //   Print(L"--open %s --\n",Argv[1]);  //�����в�������bmp�ļ�����
  //   buffStr=AllocateZeroPool(200);
  //   AsciiStrToUnicodeStr(Argv[1],buffStr);
   
  //   Status = OpenFile(&file, buffStr, EFI_FILE_MODE_READ);
  //   FreePool(buffStr);

  //   if(Status == EFI_SUCCESS)
  //   {
  //     Print(L"Open file: %s success!\n", Argv[1]);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   {
  //     // EFI_FILE_INFO *fileInfo;
  //     // fileInfo = FileHandleGetInfo(file);  //EFI_FILE_HANDLEΪEFI_FILE_PROTOCOLָ��
  //     // Print(L"File Info: Size=0x%x,FileSize=0x%lx,FileName=0x%x \n",fileInfo->Size,(UINT32)fileInfo->FileSize,fileInfo->FileName[0]);
      
  //   }
    
    
  //   //2 �����ڴ棬��ת������
  //   Status=ReadFile(file,&bufLength, &PCXheader);
  //   if(Status ==EFI_SUCCESS)
  //   {
  //     Print(L"Read file: %s success!\n", Argv[1]);
  //     Width = (PCXheader.xMax - PCXheader.xMin) + 1;
  //     Height = (PCXheader.yMax - PCXheader.yMin) + 1;
  //     Print(L"PCX size=0x%x\n",Width*Height);
  //     WaitKey();
  //     // file->Close(file);
  //   }
  //   //2-1 �����ڴ棬����pcx�ļ������ڴ���
  //   bltBuffer = AllocateZeroPool(sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)*Width*Height);
  //   if(bltBuffer  !=  NULL)
  //   {
  //     Print(L"Allocate memory to bltBuffer success!\n");
  //   }
  //   else
  //   {
  //     Print(L"Allocate memory to bltBuffer fail!\n");
  //   }
  //   decompressPCX24bits_fast(file, bltBuffer);
    
  //   //decompressPCX24bits(file, bltBuffer);
  //   file->Close(file);
  //   //3 ����ͼ��ģʽ����ʾͼ��;
    
	// 	SwitchGraphicsMode(TRUE);
  //   SetMyMode(0x3);
	// 	SetBKG(&(gColorTable[3]));
  //   putRectImage(100, 100, Width, Height, bltBuffer);
    
  //   WaitKey();
  //   FreePool(bltBuffer);
	// 	SetMyMode(0x0);
	// 	SwitchGraphicsMode(FALSE);
    
  // }

  {
		// //ͼ����ʾ����
		
		// //DisplayModeInformation();
		// UINT32 ModeIndex;
		// for(ModeIndex=0;ModeIndex<gGraphicsOutput->Mode->MaxMode;ModeIndex++)
		// {
		// 	DisplaySpecifyModeMessage(ModeIndex);
		// 	WaitKey();
		// }
	
		// SwitchGraphicsMode(TRUE);
		// SetBKG(&(gColorTable[3]));
		// WaitKey();
		// //rectblock(0,0,799,599-1,&(gColorTable[1])); //white
		// rectangle(100,100,150,150,&(gColorTable[8]));//red
		// circle(300,300,40,&(gColorTable[1]));//blue
		// rectblock(450,450,500,500,&(gColorTable[6]));
		// WaitKey();
		// // ShowBmp();
		// WaitKey();
		// SetMyMode(0x0);
		// SwitchGraphicsMode(FALSE);
	}
  return(0);
}




