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
#include "Common.h"
#include "FileRW.h"
#include "Keyboard.h"
#include "Graphic.h"
#include "Window.h"
#include "Font.h"

#include  <Uefi.h>
#include  <Library/UefiLib.h>
// #include  <Library/ShellCEntryLib.h>


extern EFI_GRAPHICS_OUTPUT_BLT_PIXEL gColorTable[];
extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *gPCIRootBridgeIO;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL       *gGraphicsOutput;
INT32 robin_rand(); //robin's random function
VOID NotifyWaitFunc(IN EFI_EVENT Event, IN VOID *Context);
VOID NotifySignalFunc(IN EFI_EVENT Event, IN VOID *Context);
VOID TestNotifyEvent(VOID);
VOID RandomBlock(UINT32 Width,UINT32 Height,EFI_GRAPHICS_OUTPUT_BLT_PIXEL* BltArray,UINT32 BltArraySize);

EFI_STATUS HotKeySample(IN EFI_KEY_DATA *hotkey);
EFI_STATUS HotKeyNotifyFunc(IN EFI_KEY_DATA *hotkey);


VOID CreateMap(VOID);
VOID SnakeElement(UINT32 x,UINT32 y,UINT8 ColorIndex);
VOID InitSnake(VOID);
UINT8 BiteSelf();
VOID RandomFood(VOID);
UINT8 NotCrossWall(VOID);
UINT8 SnakeMove(VOID);
VOID GameRun(VOID);
VOID EndGame(VOID);
/***
  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT64 flag;
  // EFI_STATUS Status;
  // // EFI_INPUT_KEY key={0,0};
  // UINT16 scanCode=0;
	// UINT16 uniChar=0;
	// UINT32 shiftState;
	// EFI_KEY_TOGGLE_STATE toggleState;
  // UINT32 count=0;
  // UINTN i;
  // Print(L"Hello there fellow Programmer.\n");
  // Print(L"Welcome to the world of EDK II.\n");
  // Print(L"Argc=%d\n", Argc);
  // for (i = 0; i < Argc;i++)
  // {
  //   Print(L"Argv[%d]= %s\n", i,Argv[i]);
  // }
    
  flag = InintGloabalProtocols(S_TEXT_INPUT_EX | GRAPHICS_OUTPUT );
	Print(L"flag=%x\n",flag);
	WaitKey();

  SwitchGraphicsMode(TRUE);
	SetBKG(&(gColorTable[DEEPBLUE]));
  CreateMap();
  InitSnake();
  RandomFood();
  GameRun();
  WaitKey();
  // RandomBlock(SY_SCREEN_WIDTH,SY_SCREEN_HEIGHT,gColorTable,10);
// {
// 		//随机显示图形块测试
//     EFI_EVENT myEvent;
//     EFI_STATUS Status;
//     UINTN       repeats=0;
//     UINTN       index=0;
    
		
// 		// WaitKey();
//     //create timer event and settimer
//     Status = gBS->CreateEvent(EVT_TIMER  , TPL_CALLBACK, (EFI_EVENT_NOTIFY)NotifyWaitFunc, (VOID*)NULL, &myEvent);
//     Status = gBS->SetTimer(myEvent,TimerPeriodic , 2 * 1000 * 1000); //triggered per 200ms
//     while(1)
//     {
//       UINT32 rand_x1, rand_y1,randColor;
//       Status = gBS->WaitForEvent(1, &myEvent, &index);
//       //do something
//       rand_x1 = (robin_rand() % SY_SCREEN_WIDTH);
//       rand_y1 = (robin_rand() % SY_SCREEN_HEIGHT);
//       randColor = (robin_rand()) % 10;  //gColorTable has 10 elements
//       // rand_x2 = rand_x1 + 30;
//       // rand_y2 = rand_y1 + 30;
//       // if(rand_x2>(SY_SCREEN_WIDTH-1))
//       //   rand_x2 = SY_SCREEN_WIDTH - 1;
//       // if(rand_y2>(SY_SCREEN_HEIGHT-1))
//       //   rand_y2 = SY_SCREEN_HEIGHT - 1;
      
//       rectblock(rand_x1,rand_y1,rand_x1+20,rand_y1+20,&(gColorTable[randColor]));
//       if(repeats ++ == 100){
//             break;
//         }
//     }
//     Status = gBS->CloseEvent(myEvent);
	
// 		// rectangle(100,100,150,150,&(gColorTable[8]));//red
// 		// circle(300,300,40,&(gColorTable[1]));//blue
// 		// rectblock(450,450,500,500,&(gColorTable[6]));
// 		// WaitKey();
		
// 		// WaitKey();
		
// 	}
  SetMyMode(0x0);
	SwitchGraphicsMode(FALSE);
  return(0);
}

VOID RandomBlock(UINT32 Width,UINT32 Height,EFI_GRAPHICS_OUTPUT_BLT_PIXEL* BltArray,UINT32 BltArraySize)
{
  //随机显示图形块测试
  EFI_EVENT myEvent;
  EFI_STATUS Status;
  UINTN       repeats=0;
  UINTN       index=0;
  UINT32 rand_x1, rand_y1,randColor;
  
  //create timer event and settimer
  Status = gBS->CreateEvent(EVT_TIMER  , TPL_CALLBACK, (EFI_EVENT_NOTIFY)NULL, (VOID*)NULL, &myEvent);
  Status = gBS->SetTimer(myEvent,TimerPeriodic , 2 * 1000 * 1000); //triggered per 200ms
  while(1)
  {
    Status = gBS->WaitForEvent(1, &myEvent, &index);
    //do something
    rand_x1 = (robin_rand() % Width);
    rand_y1 = (robin_rand() % Height);
    randColor = (robin_rand()) % BltArraySize;     
    rectblock(rand_x1,rand_y1,rand_x1+20,rand_y1+20,&(BltArray[randColor]));
    if(repeats ++ == 100){
          break;
      }
  }
  Status = gBS->CloseEvent(myEvent);
}

VOID TestNotifyEvent(VOID)
{
	EFI_EVENT myWaitEvent;
  EFI_EVENT mySignalEvent;
	UINTN       index=0;
  EFI_STATUS Status;

	Status = gBS->CreateEvent(EVT_NOTIFY_WAIT, TPL_NOTIFY, (EFI_EVENT_NOTIFY)NotifyWaitFunc, (VOID*)L"Hi,UEFI!", &myWaitEvent);
	Print(L"CreateEvent myWaitEvent=%r\n",Status);
  Status = gBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_NOTIFY, (EFI_EVENT_NOTIFY)NotifySignalFunc, (VOID*)L"Hi,me!", &mySignalEvent);
  Print(L"CreateEvent mySignalEvent=%r\n",Status);
	Status = gBS->WaitForEvent(1, &myWaitEvent, &index);
  Print(L"myWaitEvent is signaled! index=%d %r\n",index,Status);
  gBS->SignalEvent(mySignalEvent);
  Status = gBS->WaitForEvent(1, &mySignalEvent, &index);
  Print(L"mySignalEvent is signaled! index=%d %r\n",index, Status);
  gBS->CloseEvent(myWaitEvent);
  gBS->CloseEvent(mySignalEvent);
}

VOID NotifyWaitFunc(IN EFI_EVENT Event, IN VOID *Context)
{
  static UINTN count = 0;
  Print(L"NotifyWaitFunc: count=%d, Context=%s\n",count,Context);
  count++;
  if((count%5)==0)
    gBS->SignalEvent(Event);
    
}

VOID NotifySignalFunc(IN EFI_EVENT Event, IN VOID *Context)
{
  Print(L"NotifySignalFunc: Context=%s\n",Context);
}

/** Compute a pseudo-random number.
  *
  * Compute x = (7^5 * x) mod (2^31 - 1)
  * without overflowing 31 bits:
  *      (2^31 - 1) = 127773 * (7^5) + 2836
  * From "Random number generators: good ones are hard to find",
  * Park and Miller, Communications of the ACM, vol. 31, no. 10,
  * October 1988, p. 1195.
**/
// #define RAND_MAX  0x7fffffff

INT32 robin_rand(VOID)
{
  INT32 hi, lo, x;
  static UINT32 next = 1;
  /* Can't be initialized with 0, so use another value. */
  if (next == 0)
    next = 123459876;
  hi = next / 127773;
  lo = next % 127773;
  x = 16807 * lo - 2836 * hi;
  if (x < 0)
    x += 0x7fffffff;
  return ((next = x) % ((UINT32)0x7fffffff + 1));
}

//--------------hot key sample---------------------------
extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *gSimpleTextInputEx;

EFI_STATUS HotKeySample(IN EFI_KEY_DATA *hotkey)
{
  EFI_STATUS Status;
  EFI_KEY_DATA key;
  EFI_HANDLE hotkeyNotifyHandle;
  
  hotkey->KeyState.KeyShiftState|=EFI_SHIFT_STATE_VALID;
  hotkey->KeyState.KeyToggleState|=EFI_TOGGLE_STATE_VALID|EFI_KEY_STATE_EXPOSED;

  Status = gSimpleTextInputEx->RegisterKeyNotify(gSimpleTextInputEx,
                                                hotkey,
                                                HotKeyNotifyFunc,
                                                (VOID **)&hotkeyNotifyHandle);
  Print(L"RegisterKeyNotify=%r\n",Status);

  while(key.Key.ScanCode!=0x17)
  {
    UINTN index;
    gBS->WaitForEvent(1,&(gSimpleTextInputEx->WaitForKeyEx),&index);
    Status = gSimpleTextInputEx->ReadKeyStrokeEx(gSimpleTextInputEx,&key);
    Print(L"key.Key.UnicodeChar=%x\n",key.Key.UnicodeChar);
  }

  Status = gSimpleTextInputEx->UnregisterKeyNotify(gSimpleTextInputEx,hotkeyNotifyHandle);

  return EFI_SUCCESS;
}

EFI_STATUS HotKeyNotifyFunc(IN EFI_KEY_DATA *hotkey)
{
  Print(L"Hot key pressed!\n");
  return EFI_SUCCESS;
}
//------------------------- snake ------------------------------
#define X_MAP 50      //容纳多少个SnakeBlock
#define Y_MAP 50
#define SNAKEBLOCK 8
#define SNAKEBLANK 2

#define CROSSWALL 1
#define BITESELF  2
#define USEREXIT  3

#define SnakeUP     1
#define SnakeDOWN   2
#define SnakeLEFT   3
#define SnakeRIGHT  4
typedef struct GREEDSNAKE //贪吃蛇的数据结构
{
	INT32 x;
	INT32 y;
  INT32 BlockNumber; //总共X_MAP*Y_MAP个SnakeBlock
	struct GREEDSNAKE *next;
}greedsnake;

greedsnake *head,*food;  //蛇头指针，食物指针
greedsnake *pSnake; //遍历所用指针
UINT8 foodColor = BLACK;
UINT8 snakeColor = BLACK;
INT32 FoodBlocks[X_MAP*Y_MAP];  //保存可用的SnakeBlock
INT32 FoodBlockCounts;
INT32 SnakeStatus,SleepTime = 200; //运行的延时时间，ms为单位
INT32 Score=0;       //成绩，吃到一个食物则加10分


UINT8 EndGameFlag = 0;  //1 撞墙了  2 咬到自己  3 主动退出游戏
/**
  绘制Snake的元素，以8x8矩形块表示，占据10x10空间.
  
  @param  x            起始X坐标
  @param  y            起始Y坐标
  @param  ColorIndex   颜色索引
  @retval VOID              
**/
VOID SnakeElement(UINT32 x,UINT32 y,UINT8 ColorIndex)
{
  rectblock(x+ SNAKEBLANK,y+SNAKEBLANK,x+ SNAKEBLANK +SNAKEBLOCK,y+ SNAKEBLANK +SNAKEBLOCK,&(gColorTable[ColorIndex]));
}
/**
  绘制地图
  
  @param  VOID            
  @retval VOID              
**/
VOID CreateMap(VOID)
{
  INT32 xnum;
  INT32 ynum;
  INT32 counter=0;
  UINT8 *s_gamename = "沁沁宝贝的贪吃蛇游戏";
  draw_string(s_gamename, 600, 50, &MyFontArray, &(gColorTable[WHITE]));
  for(xnum=0;xnum<X_MAP;xnum++)
  {
    SnakeElement(xnum*(SNAKEBLOCK+SNAKEBLANK),0,BLACK);
    SnakeElement(xnum*(SNAKEBLOCK+SNAKEBLANK),(Y_MAP-1)*(SNAKEBLOCK+SNAKEBLANK),BLACK);
  }

  for (ynum = 0; ynum < Y_MAP; ynum++)
  {
	SnakeElement(0, ynum*(SNAKEBLOCK + SNAKEBLANK), BLACK);
	SnakeElement((X_MAP - 1)*(SNAKEBLOCK + SNAKEBLANK), ynum*(SNAKEBLOCK + SNAKEBLANK), BLACK);
  }
  for(ynum=1;ynum<Y_MAP-1;ynum++)
    for(xnum=1;xnum<X_MAP-1;xnum++)
    {
      FoodBlocks[counter]=ynum*X_MAP + xnum;
      counter++;
    }
  FoodBlockCounts=counter;
}

/**
  初始化蛇身
  
  @param  VOID            
  @retval VOID              
**/
VOID InitSnake(VOID) 
{
	greedsnake *tail;
	INT32 i;
	tail = (greedsnake*)AllocateZeroPool(sizeof(greedsnake));//从蛇尾开始，头插法，以x,y设定开始的位置//
	tail->x = (X_MAP/2)*(SNAKEBLOCK + SNAKEBLANK);
	tail->y = (Y_MAP/2)*(SNAKEBLOCK + SNAKEBLANK);
  tail->BlockNumber = (Y_MAP/2) * X_MAP + (X_MAP/2);
	tail->next = NULL;
  
	for (i = 1; i <= 4; i++)  //4个SnakeBlock
	{
		head = (greedsnake*)AllocateZeroPool(sizeof(greedsnake));
		head->next = tail;
		head->x = (X_MAP/2)*(SNAKEBLOCK + SNAKEBLANK) + i*(SNAKEBLOCK + SNAKEBLANK);
		head->y = (Y_MAP/2)*(SNAKEBLOCK + SNAKEBLANK);
    head->BlockNumber = tail->BlockNumber + 1;
		tail = head;
	}
	while (tail != NULL)//从头到尾，输出蛇身
	{
		// Pos(tail->x, tail->y);
		// printf("■");
    SnakeElement(tail->x, tail->y,snakeColor);
		tail = tail->next;
	}
}
/**
  判断是否咬到自己
  
  @param  VOID            
  @retval 1     咬到自己
          0     没有咬到                           
**/
UINT8 BiteSelf()
{
	greedsnake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;
		}
		self = self->next;
	}
	return 0;
}
/**
  随机出现食物
  
  @param  VOID            
  @retval VOID              
**/
VOID RandomFood(VOID)
{
  greedsnake *tempfood;
  INT32 randNum;

  randNum = robin_rand() % FoodBlockCounts;  //不能超过食物可出现位置的总数
  tempfood = (greedsnake*)AllocateZeroPool(sizeof(greedsnake));
  tempfood->BlockNumber = FoodBlocks[randNum];
  //递归判断蛇身与食物是否重合
  pSnake=head;
  while(pSnake == NULL)
  {
    if(pSnake->BlockNumber == FoodBlocks[randNum])  //重合了
    {
      FreePool(tempfood);  //释放内存
      RandomFood();
    }
    pSnake = pSnake->next;
  }

  tempfood->x = ((tempfood->BlockNumber) % X_MAP)  *(SNAKEBLOCK + SNAKEBLANK);
  tempfood->y = ((tempfood->BlockNumber) / X_MAP)  *(SNAKEBLOCK + SNAKEBLANK);
  tempfood->next = NULL;

  food = tempfood;
  foodColor = (UINT8)(robin_rand() % 10);  //共10个颜色可选
  if(foodColor == DEEPBLUE)
    foodColor = BLACK;
  SnakeElement(food->x,food->y,foodColor);
}
/**
  不能穿墙
  
  @param  VOID            
  @retval 1   穿墙了
          0   没有穿墙              
**/
UINT8 NotCrossWall(VOID)
{
  UINT32 BlockX,BlockY;
  BlockX = ((head->BlockNumber) % X_MAP);
  BlockY = ((head->BlockNumber) / X_MAP);
  if((BlockX==0) || (BlockX==(X_MAP-1)) || (BlockY==0) || (BlockY==(Y_MAP-1)))  
  {
    EndGameFlag =  CROSSWALL;
    return 1;
  }
  return 0;
}
/**
  蛇的移动
  
  @param  VOID            
  @retval 1     撞到自己或墙了
          0     啥事没有             
**/
UINT8 SnakeMove(VOID)
{
  greedsnake *nexthead;

  if(NotCrossWall())
    return 1;

  nexthead = (greedsnake*)AllocateZeroPool(sizeof(greedsnake));

  switch(SnakeStatus)
  {
    case SnakeUP:
      nexthead->BlockNumber = head->BlockNumber - X_MAP; 
      break;
    case SnakeDOWN:
      nexthead->BlockNumber = head->BlockNumber + X_MAP; 
      break;
    case SnakeLEFT:
      nexthead->BlockNumber = head->BlockNumber -1; 
      break;
    case SnakeRIGHT:
      nexthead->BlockNumber = head->BlockNumber +1; 
      break;
    default:
      break;
  }
  if(nexthead->BlockNumber == food->BlockNumber)  //找到食物!
  {
    nexthead->x = food->x;
    nexthead->y = food->y;
    nexthead->next=head;
    head=nexthead;
    pSnake = head; //准备遍历
    snakeColor = foodColor;
    while(pSnake != NULL)
    {
      SnakeElement(pSnake->x,pSnake->y,snakeColor);  //变成食物的颜色
      Delayms(50);
      pSnake=pSnake->next;
    }
    Score+=10;
    RandomFood();
  }
  else
  {
    nexthead->x = ((nexthead->BlockNumber) % X_MAP)  *(SNAKEBLOCK + SNAKEBLANK);
    nexthead->y = ((nexthead->BlockNumber) / X_MAP)  *(SNAKEBLOCK + SNAKEBLANK);
    nexthead->next = head;
	  head = nexthead;

    pSnake = head; //准备遍历
    while(pSnake->next->next !=NULL)
    {
      SnakeElement(pSnake->x,pSnake->y,snakeColor);  
      pSnake=pSnake->next;  
    }
    SnakeElement(pSnake->next->x,pSnake->next->y,DEEPBLUE);  //消除，即变成背景色
    FreePool(pSnake->next);  //释放内存
    pSnake->next=NULL;
  }
  
  if(BiteSelf() == 1)
  {
    EndGameFlag = BITESELF;
    return 1;
  }
  return 0;
}
/**
  运行游戏
  
  @param  VOID            
  @retval VOID              
**/
VOID GameRun(VOID)
{
  EFI_INPUT_KEY key={0,0};

  SnakeStatus = SnakeRIGHT;
  while(1)
  {
    CheckKey(&key);
    if((key.ScanCode==0x01) && (SnakeStatus!=SnakeDOWN)) //UP key
    {
      SnakeStatus=SnakeUP;
    }
    else if((key.ScanCode==0x02) && (SnakeStatus!=SnakeUP))   //DOWN key
    {
      SnakeStatus=SnakeDOWN;
    }
    else if((key.ScanCode==0x03) && (SnakeStatus!=SnakeLEFT))   //RIGHT key
    {
      SnakeStatus=SnakeRIGHT;
    }
    else if((key.ScanCode==0x04) && (SnakeStatus!=SnakeRIGHT))   //LEFT key
    {
      SnakeStatus=SnakeLEFT;
    }
    else if(key.ScanCode==0x17)   //ESC
    {
      EndGameFlag = USEREXIT;
      break;
    }
    Delayms(SleepTime);
    if(SnakeMove())
      break;
  }
  EndGame();
}
/**
  结束游戏
  
  @param  VOID            
  @retval VOID              
**/
VOID EndGame(VOID)
{
  UINT8 *s_crosswall = "对不起，撞到墙了，游戏结束！";
  UINT8 *s_biteself = "对不起，咬到自己了，游戏结束！";
  UINT8 *s_userexit = "您退出了游戏！";
  UINT8 *s_gamescore = "您的得分是： ";
  CHAR8 buffsStr[12]={0};
  UINTN strX=20;
  UINTN strY=200;
  greedsnake *temp;

  pSnake = head; //准备遍历
  while(pSnake!=NULL)
  {
    temp = pSnake->next;
    FreePool(pSnake);  //释放内存
    pSnake = temp;
  }

  if(EndGameFlag == CROSSWALL)
    draw_string(s_crosswall, strX, strY, &MyFontArray, &(gColorTable[WHITE]));
  else if(EndGameFlag == BITESELF)
    draw_string(s_biteself, strX, strY, &MyFontArray, &(gColorTable[WHITE]));
  else if(EndGameFlag == USEREXIT)
    draw_string(s_userexit, strX, strY, &MyFontArray, &(gColorTable[WHITE]));
  
  draw_string(s_gamescore, strX, strY+20, &MyFontArray, &(gColorTable[WHITE]));
  AsciiSPrint(buffsStr,5,"%d",Score);
  draw_string(buffsStr, strX+108+2, strY+20, &MyFontArray, &(gColorTable[WHITE])); //108+2是计算s_gamescore长度的结果
  // AsciiSPrint();
}

