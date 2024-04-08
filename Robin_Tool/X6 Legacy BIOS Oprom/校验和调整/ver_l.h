/*南京易思克研发部 版本控制文件 ver_m.h：为底层C软件所做*/
/*2008-8-13 12:59  luobing*/

#ifndef __Ver_L_H__
#define __Ver_L_H__
//如果需要修改版本信息，请直接修改宏定义，不要去改动下面的变量
#define MainVer     0x01                          //主版本号(0~3)
#define SubVer      0x01                          //次版本号(0~63)
#define CodeType    0x0F                          //产品代码的类型(0x0:生产代码 0x1:OEM特单 0xF:ESC内部工具)
#define ReleaseVerH 0x01                          //发布的版本号的高位(0~15)
#define ReleaseVerL 0x02                          //发布的版本号的低位(0~255)
#define OEM_ID      0x00                          //(0~65535)OEM厂商ID，如果是OEM特单的代码，此ID有效；否则无效
#define PString     "Chks.exe"       //产品描述字符串


/*----------------------------------程序版本控制 begin-------------------------*/
/*为保证编译器的无关性，定义为如下的数组形式*/
/*第一个数组为标志，不得更改位置及内容*/
/*第二个数组的内容依次为: 标志、内部版本号、ExtVer1的高字节、ExtVer1的低字节、ExtVer2*/
/*
  MyVersion[12]='2': 底层软件固定标志，不要更改
    MyVersion[13]=InsideVer:  研发部内部版本号，高两位为主版本号，低六位为次版本号
  MyVersion[14]=ExtVer1的高字节: 其高四位为代码类型，低四位是程序Release版本的高四位
    代码类型：  0x00:生产代码，即MyVersion[14]=0x0? (?代表任意数)
                0x01:OEM特单， 即MyVersion[14]=0x1? (?代表任意数)
                0x0F:ESC内部工具集 即MyVersion[14]=0xF? (?代表任意数)
  MyVersion[15]=ExtVer1的低字节: 程序Release版本的低八位
  MyVersion[16]=ExtVer2的高字节--|
                                 |--厂商编号:只有当OEM特单时此编号才有意义
  MyVersion[17]=ExtVer2的低字节--|
*/    
unsigned char MyVersion[]={'N','a','n','j','i','n','g',0x20,'E','S','C',0x00,  
                           '2',((MainVer<<6)+SubVer),((CodeType<<4)+(ReleaseVerH)),
                               ReleaseVerL,(OEM_ID>>8),(OEM_ID&0xff)};
unsigned char Prog_ver[]=PString;
/*----------------------------------程序版本控制 end-------------------------*/
#endif









