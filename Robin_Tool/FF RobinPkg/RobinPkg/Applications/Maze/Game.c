/*
  Copyright (C) 2017 Tim Lewis. 
  This program and the accompanying materials are licensed and made available 
  under the terms and conditions of the BSD License which accompanies this 
  distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Uefi.h>
#include <IndustryStandard/Bmp.h>

#include "Library/BaseLib.h"
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

//
// External References
//
BOOLEAN
InitGop(VOID);

BOOLEAN
LoadBitmapFile(
  IN CONST CHAR8    *BmpFilePath, 
  OUT VOID          **Blt, 
  OUT UINTN         *Height, 
  OUT UINTN         *Width
  );

EFI_STATUS
CopyBitmapToBitmap(
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Dest,
  IN UINTN          DestWidth,
  IN UINTN          DestHeight,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Src,
  IN UINTN          SrcWidth,
  IN UINTN          SrcHeight,
  IN UINTN          OffsetX,
  IN UINTN          OffsetY
  );

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *mGraphicsOutput;

//
// Forward References.
//
EFI_STATUS
DisplayImageStack(VOID);

// Global Bitmaps
UINTN               mBgWidth;           // width of background image.
UINTN               mBgHeight;          // height of background image.
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mBgBlt;  // background image.
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mRockBlt;// rock image.
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mCharBlt;// character image.

EFI_GRAPHICS_OUTPUT_BLT_PIXEL *mDisplay;// copy of grid display.
UINT32              mDisplayWidth;      // width (in pixels) of grid display.
UINT32              mDisplayHeight;     // height (in pixels) of grid display.

EFI_GRAPHICS_OUTPUT_BLT_PIXEL **mGrid = NULL; // grid of boxes for the game
INT32               mGridHeight;        // number of columns in the grid.
INT32               mGridWidth;         // number of rows in the grid.

UINTN               mGenImageWidth;     // width of images that take up one box
UINTN               mGenImageHeight;    // height of images that take up one box

INT32               mCharacterX;        // character's X cell location.
INT32               mCharacterY;        // character's Y cell location.

INT32               mEntranceX;
INT32               mEntranceY;

INT32               mExitX;
INT32               mExitY;


  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %
// %                          PLAYER SUPPORT
// %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

typedef enum {
  TEMP,
  MOVE
} MODE;                                 //  Defines the possible game modes

typedef enum {
  NORTH,
  SOUTH,
  EAST,
  WEST
} DIRECTION;                            // Defines the possible directions a player may move to



/**
  Sets all the values associated with images, including all Blt Buffers, height 
  and width.

  @retval EFI_SUCCESS

**/
EFI_STATUS 
SetUpImages (VOID)
{
  // Initialize the graphics support.
  if (!InitGop()) {
    return EFI_UNSUPPORTED;
  }

  // Load the background bitmap.
  if (!LoadBitmapFile("bg.bmp", &mBgBlt, &mBgHeight, &mBgWidth)) {
    return EFI_UNSUPPORTED;
  }

  // Load the rock bitmap.
  if (!LoadBitmapFile("object.bmp", &mRockBlt, &mGenImageHeight, &mGenImageWidth)) {
    return EFI_UNSUPPORTED;
  }

  // Load the primary character bitmap.
  if (!LoadBitmapFile("char.bmp", &mCharBlt, &mGenImageHeight, &mGenImageWidth)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %
// %                          GRID SUPPORT
// %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

VOID
GridSet(
  IN                INT32 X,
  IN                INT32 Y,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Image
)
{
  if (X >= mGridWidth || Y >= mGridHeight || Image == NULL) {
    return;
  }

  mGrid[Y * mGridWidth + X] = Image;

  CopyBitmapToBitmap(
    mDisplay,
    mDisplayWidth,
    mDisplayHeight,
    Image,
    mGenImageWidth,
    mGenImageHeight,
    X * mGenImageWidth,
    Y * mGenImageHeight
  );

}

/*
  Set the specified grid coordinate to background.
*/
VOID
GridSetBackground(IN INT32 X, IN INT32 Y)
{
  GridSet(X, Y, mBgBlt);
}


VOID
GridInit(IN INT32 X, IN INT32 Y)
{
  GridSetBackground(X, Y);
}

/*
 Is the specified grid coordinate empty?
*/
BOOLEAN
GridIsBackground(IN INT32 X, IN INT32 Y)
{
  return mGrid[Y * mGridWidth + X] == mBgBlt;
}



// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %
// %                          MAZE GENERATION
// %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define WALL 1
#define PATH 0

STATIC char *mMaze;
STATIC char *mBacktrackX;
STATIC char *mBacktrackY;

char MazeGet(int X, int Y)
{
  assert(Y < mGridHeight);
  assert(X < mGridWidth);
  return mMaze[Y * mGridWidth + X];
}

void MazeSet(int X, int Y, char C)
{
  assert(Y < mGridHeight);
  assert(X < mGridWidth);
  mMaze[Y * mGridWidth + X] = C;
}

BOOLEAN 
MazeIsClosed(int X, int Y)
{
  if (MazeGet(X - 1, Y) == WALL &&
      MazeGet(X, Y - 1) == WALL &&
      MazeGet(X, Y + 1) == WALL &&
      MazeGet(X + 1, Y) == WALL) {
    return TRUE;
  }
  return FALSE;
}

BOOLEAN
MazeInit(VOID)
{
  int x;
  int y;

  // Allocate a grid to hold the generated mMaze. Each position in the grid can
  // hold a path or a wall. It is important to note that there will only be 
  // (width - 1)/2 actual locations where a character can move, since walls 
  // take up as much space as paths in the grid.
  mMaze = malloc(mGridWidth * mGridHeight);
  if (mMaze == NULL) {
    return FALSE;
  }
  memset(mMaze, PATH, mGridWidth * mGridHeight);

  // Allocate a backtrack array X, one for each location on the grid.
  mBacktrackX = malloc(mGridWidth * mGridHeight);
  if (mBacktrackX == NULL) {
    free(mMaze);
    return FALSE;
  }
  mBacktrackX[0] = 1;

  // Allocate a backtrack array Y, one for each location on the grid.
  mBacktrackY = malloc(mGridWidth * mGridHeight);
  if (mBacktrackY == NULL) {
    free(mBacktrackX);
    free(mMaze);
    return FALSE;
  }
  mBacktrackY[0] = 1;

  // Now go through and set each even location on the grid to be a wall.
  for (x = 0; x < mGridWidth; x++) { 
    for (y = 0; y < mGridHeight; y++) { 
      if (x % 2 == 0 || y % 2 == 0) { // even always have a wall
        MazeSet(x, y, WALL);
      } 
    }
  }
  return TRUE;
}

VOID 
MazeGenerate(
  int               Index,              // backtrack index.
  int               X,                  // grid horizontal position to investigate.
  int               Y,                  // grid vertical position to investigate.
  int               W,                  // number of unique horizontal maze positions.
  int               H,                  // number of unique vertical maze positions.
  int               Visited             // number of maze positions visited.
  )
{
  int               neighbor_valid;     // valid neighbors: 0 = none.
  int               neighbor_x[4];      // array of grid positions of possible neighbor (x).
  int               neighbor_y[4];      // array of grid positions of possible neighbor (y).
  int               step[4];            // array of valid possible neighbors.
  int               x_next;             // next selected grid position (x).
  int               y_next;             // next selected grid position (y).
  int               random;             // randomly selected neighbor to try.

  if (Visited < H * W) {
    neighbor_valid = 0; 

    // Add the left neighbor if it is completely blocked off.
    if (X - 2 > 0 && MazeIsClosed(X - 2, Y)) {
      neighbor_x[neighbor_valid] = X - 2;;
      neighbor_y[neighbor_valid] = Y;
      step[neighbor_valid] = 1;
      neighbor_valid++;
    }

    // Add the up neighbor if it is completely blocked off.
    if (Y - 2 > 0 && MazeIsClosed(X, Y - 2)) {
      neighbor_x[neighbor_valid] = X;
      neighbor_y[neighbor_valid] = Y - 2;
      step[neighbor_valid] = 2;
      neighbor_valid++;
    }

    // Add the down neighbor if it is completely blocked off.
    if (Y + 2 < H * 2 + 1 && MazeIsClosed(X, Y + 2)) {
      neighbor_x[neighbor_valid] = X;
      neighbor_y[neighbor_valid] = Y + 2;
      step[neighbor_valid] = 3; 
      neighbor_valid++;
    }
    
    // Add the right neighbor if it is completely blocked off.
    if (X + 2 < W * 2 + 1 && MazeIsClosed(X + 2, Y)) {
      neighbor_x[neighbor_valid] = X + 2; 
      neighbor_y[neighbor_valid] = Y;
      step[neighbor_valid] = 4; 
      neighbor_valid++;
    }
    
    // Count the number of neighbors that are completely surrounded by walls. 
    // If there are none, then backtrack to the previous location and try 
    // again.
    if (neighbor_valid == 0) { // backtrack 
      x_next = mBacktrackX[Index]; 
      y_next = mBacktrackY[Index]; 
      Index--;

    // There is at least one neighbor that was completely blocked off, so 
    // randomly select among them.
    } else {
      random = rand() % neighbor_valid;

      // Find the next grid location to try from among the valid neighbors.
      x_next = neighbor_x[random]; 
      y_next = neighbor_y[random]; 

      // Record the next location in the backtrack location list.
      Index++; 
      mBacktrackX[Index] = (char)x_next; 
      mBacktrackY[Index] = (char)y_next; 

      // Make sure there is an opening between the next location and the 
      // location by putting a path there.
      switch (step[random]) {
        case 1: MazeSet(x_next + 1, y_next, PATH); break;
        case 2: MazeSet(x_next, y_next + 1, PATH); break;
        case 3: MazeSet(x_next, y_next - 1, PATH); break;
        case 4: MazeSet(x_next - 1, y_next, PATH); break;
        default: assert(FALSE); break;
      }

      Visited++; 
    } 

    // Recursively generate the maze from the next location selected.
    MazeGenerate(Index, x_next, y_next, W, H, Visited); 
  } 
} 

void MazePrint(VOID) 
{ 
  for(int a = 0; a < mGridWidth; a++) 
  { 
    for (int b = 0; b < mGridHeight; b++) {
      if (MazeGet(a, b) == WALL) {
        GridSet(a, b, mRockBlt);
      } else {
        GridSetBackground(a, b);
      }
    } 
  } 
} 


VOID
MazeCreate(VOID)
{
  assert(mGridWidth % 2 == 1);
  assert(mGridHeight % 2 == 1);
  MazeGenerate(0, 1, 1, (mGridWidth - 1)/2, (mGridHeight - 1)/2, 1);
}

VOID
MazeShutdown(VOID)
{
  free(mMaze);
  free(mBacktrackX);
  free(mBacktrackY);
}


/**

  Sets the coordinates and positions of all the boxes on the game grid

  @retval 

**/
EFI_STATUS 
GridSetup (VOID)
{ 
  INT32 grid_x;
  INT32 grid_y;

  // Calculate the size of the grid, based on the screen resolution and the 
  // size of the thermometer images.
  mGridWidth = (mGraphicsOutput->Mode->Info->HorizontalResolution) / mGenImageWidth;
  mGridHeight = mGraphicsOutput->Mode->Info->VerticalResolution / mGenImageHeight;
  if (mGridWidth < 6 || mGridHeight < 6) {
    Print(L"screen resolution is too small.\n");
    return EFI_DEVICE_ERROR;
  }
  if (mGridWidth % 2 == 0) {            // make sure the width is an odd number 
    mGridWidth--;
  }
  if (mGridHeight % 2 == 0) {           // make sure the height is an odd number
    mGridHeight--;
  }

  // Create the foundation background image.
  mDisplayHeight = mGridHeight * mGenImageHeight;
  mDisplayWidth = mGridWidth * mGenImageWidth;
  mDisplay = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)malloc(mDisplayHeight * mDisplayWidth * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if (mDisplay == NULL) {
    Print(L"out of memory.\n");

    free(mGrid);
    return EFI_OUT_OF_RESOURCES;
  }

  // Create the array that is used to record the current board state.
  mGrid = malloc(mGridHeight * mGridWidth * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if (mGrid == NULL) {
    Print(L"out of memory.\n");
    return EFI_OUT_OF_RESOURCES;
  }

  // Mark all grid locations as empty.
  for (grid_x = 0; grid_x < mGridWidth; grid_x++) {
    for (grid_y = 0; grid_y < mGridHeight; grid_y++) {
      GridInit(grid_x, grid_y);
    }
  }

  // Initialize the background image.
  DisplayImageStack();

  // Generate the Maze.
  if (!MazeInit()) {
    Print(L"out of memory.\n");
    return EFI_OUT_OF_RESOURCES;
  }
  MazeCreate();
  MazePrint();
  MazeShutdown();

  // Make the entrance a random location on the first row.
  mEntranceX = (rand() % ((mGridWidth - 1) / 2) * 2) + 1;
  mEntranceY = 0;

  // Make the exit a random location on the bottom row.
  mExitX = (rand() % ((mGridWidth - 1) / 2) * 2) + 1;
  mExitY = mGridHeight - 1;
  GridSetBackground(mExitX, mExitY);

  // Move the player character to the entrance.
  mCharacterX = mEntranceX;
  mCharacterY = mEntranceY;
  GridSetBackground(mCharacterX, mCharacterY);
  GridSet(mCharacterX, mCharacterY, mCharBlt);
}

/**
  Flattens all the images for the game into one buffer and draws it on the screen
**/
EFI_STATUS 
DisplayImageStack (VOID)
{  

  mGraphicsOutput->Blt (
                   mGraphicsOutput,
                   mDisplay,
                   EfiBltBufferToVideo,
                   0,
                   0,
                   0,
                   0,
                   mDisplayWidth,
                   mDisplayHeight,
                   mDisplayWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                   );

  return EFI_SUCCESS;
}

/**
  This function moves the character sprite in a given direction

  @retval EFI_SUCCESS
**/
EFI_STATUS
PlayerMove(IN DIRECTION dir)
{
  int xd;
  int yd;
  int newx;
  int newy;

  switch (dir) {
    case NORTH: xd = 0; yd = -1; break;
    case SOUTH: xd = 0; yd = 1; break;
    case EAST: xd = 1; yd = 0; break;
    case WEST: xd = -1; yd = 0; break;
  }

  newx = mCharacterX + xd;
  newy = mCharacterY + yd;
  if (newx < 0 || newx >= mGridWidth || newy < 0 || newy >= mGridHeight || !GridIsBackground(newx, newy)) {
    return EFI_UNSUPPORTED;
  }

  GridSet(newx, newy, mCharBlt);
  GridSetBackground(mCharacterX, mCharacterY);
  mCharacterX = newx;
  mCharacterY = newy;
  return EFI_SUCCESS;
}

/**
  Contains the loop that runs the game until a win condition is encountered or
  the player exits the application

  @retval 

**/
EFI_STATUS 
RunGame (VOID)
{ 
  EFI_INPUT_KEY     Key;
  UINTN             index;
  
  do {    
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &index);

    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
    
    switch (Key.ScanCode) {
      case SCAN_NULL:
        break;

      case SCAN_UP:  
        PlayerMove (NORTH);
        break;

      case SCAN_DOWN:   
        PlayerMove (SOUTH);
        break;

      case SCAN_LEFT: 
        PlayerMove (WEST);
        break;

      case SCAN_RIGHT:  
        PlayerMove (EAST);
        break;

      default:
        break;
    }

    DisplayImageStack();

   } while ((mCharacterX != mExitX || mCharacterY != mExitY));     //end condition

  //clear memory
  free (mBgBlt);

  return EFI_SUCCESS;
}

//
// Main Game Functions.
//
int
main(
  IN int Argc,
  IN char **Argv
)
{
  EFI_STATUS Status;

  srand((unsigned)time(NULL));

  gST->ConOut->Reset(gST->ConOut, FALSE);

  Status = SetUpImages();
  if (EFI_ERROR(Status)) {
    printf("Error loading images\n");
    return 1;
  }

  GridSetup();

  Status = DisplayImageStack();
  if (EFI_ERROR(Status)) {
    printf("Error Displaying images\n");
    return 1;
  }

  RunGame();

  // Clear the screen.
  gST->ConOut->Reset(gST->ConOut, FALSE);

  return 0;
}

