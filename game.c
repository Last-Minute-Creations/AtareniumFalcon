#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>
#include <ace/managers/rand.h>
#include <ace/managers/state.h>
#include <ace/utils/custom.h>
#include <ace/utils/ptplayer.h>
#include <ace/utils/file.h>

//------------------------------------------------------- gdzie� przed funkcjami
// zmienne trzymaj�ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;
static tBitMap *s_pTiles;
static tBitMap *s_pTilesMask;
static tBitMap *s_pBg;
static tBitMap *s_pHUD;
static tBitMap *s_pFalconBg;
static tBitMap *s_pRobbo;

static tPtplayerMod *s_pMod;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

extern tState g_sStateMenu;
extern tState g_sStateGameOver;
extern tState g_sStateScore;
extern tState g_sStateGuruMastah;
extern tStateManager *g_pStateMachineGame;
extern tState g_sStateScoreAmi;

#define MAP_TILE_HEIGHT 7
#define MAP_TILE_WIDTH 10
#define FALCON_HEIGHT 32
#define ANIM_FRAME_COUNT 16

#define LAST_LEVEL_NUMBER 28

char szMsg[50];  // do wyswietlania wegla na HUD
char szMsg2[50]; // do wyswietlania kondkow na HUD
char szMsg3[50];
char szMsg4[50];
char levelFilePath[20];
char szLvl[50];

char szRobboMsg[80];
char *szRobbo1stLine = "ROBBO says:";

BYTE youWin = 0;

BYTE musicPlay = 1;

BYTE ubStoneImg = 0;

BYTE kamyki[10][7];

// coordsy do rysowania falkona i kontrolowania zeby sie nie wypierdolil za ekran
BYTE falkonx = 0;
BYTE falkony = 0;
BYTE krawedzx = 0;
BYTE krawedzy = 0;
BYTE kierunek = 0;
BYTE falkonFace = 0; // kierunek dziobem

UWORD pAnimR[] = {0, 32, 64, 96, 128, 96, 64, 32, 0, 32, 64, 96, 128, 96, 64, 32};
UWORD pAnimL[] = {128, 96, 64, 32, 0, 32, 64, 96, 128, 96, 64, 32, 0, 32, 64, 96};

UWORD uwPosX = 0;
UWORD uwPosY = 0;

BYTE stoneHit = 0;
BYTE frameHit = 0;

CONST BYTE startingCoal = 10;

BYTE falkonIdle = 0;

BYTE coal = startingCoal;
BYTE capacitors = 0;
BYTE excesscoal = 0;
BYTE level = 16;

BYTE robboMsgNr = 0;
BYTE robboMsgCtrl = 0;
BYTE robboMsgCount = 0;
BYTE HUDfontColor = 23;

int amiganium = 0;
int amiganiumIdle = 192;
BYTE amigaMode = 0;

void waitFrames(tVPort *pVPort, UBYTE ubHowMany, UWORD uwPosY)
{
  for (UBYTE i = 0; i < ubHowMany; ++i)
  {
    vPortWaitForPos(pVPort, uwPosY, 0);
  }
}

void cleanUp();

void printOnHUD(void)
{

  blitCopy(s_pHUD, 32, 0, s_pVpManager->pBack, 32, 224, 32, 32, MINTERM_COOKIE);
  sprintf(szMsg, "%d", coal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 42, 232, HUDfontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 188, 0, s_pVpManager->pBack, 188, 224, 32, 32, MINTERM_COOKIE);
  sprintf(szMsg2, "%d", capacitors);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 190, 236, HUDfontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 128, 0, s_pVpManager->pBack, 128, 224, 32, 32, MINTERM_COOKIE);
  sprintf(szMsg3, "%d", excesscoal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg3);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 130, 232, HUDfontColor, FONT_COOKIE);
  blitCopy(s_pHUD, 248, 0, s_pVpManager->pBack, 248, 224, 32, 32, MINTERM_COOKIE);
  sprintf(szMsg4, "%d", robboMsgCount);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg4);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 250, 236, HUDfontColor, FONT_COOKIE);

  blitCopy(s_pHUD, 288, 0, s_pVpManager->pBack, 288, 224, 32, 32, MINTERM_COOKIE);
  sprintf(szLvl, "%d", level);
  fontFillTextBitMap(s_pFont, s_pBmText, szLvl);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 290, 236, HUDfontColor, FONT_COOKIE);
}

void gameOnResume(void)
{
  viewLoad(s_pView);
}

void drawTiles(void)
{

  sprintf(levelFilePath, "data/%d.txt", level);
  systemUse();
  tFile *levelFile = fileOpen(levelFilePath, "r");
  BYTE ubZmienna = 0;
  BYTE x = 0;
  BYTE y = 0;

  falkonx = 0;
  falkony = 0;
  krawedzx = 0;
  krawedzy = 0;
  kierunek = 0;

  for (BYTE i = 0; i < 82; ++i)
  {
    fileRead(levelFile, &ubZmienna, 1);

    if (ubZmienna == 0x30)
    {
      kamyki[x][y] = 0;
    }

    else if (ubZmienna == 0x33)
    {
      kamyki[x][y] = 3;
      ubStoneImg = ulRandMinMax(0, 2);
      blitCopyMask(s_pTiles, ubStoneImg * 32, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x34)
    {
      kamyki[x][y] = 4;
      blitCopyMask(s_pTiles, 96, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }

    else if (ubZmienna == 0x35)
    {
      kamyki[x][y] = 5;
      blitCopyMask(s_pTiles, 128, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }

    else if (ubZmienna == 0x36)
    {
      kamyki[x][y] = 6;
      blitCopyMask(s_pTiles, 160, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x37)
    {
      kamyki[x][y] = 7;
      blitCopyMask(s_pTiles, 192, 0, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x38)
    {
      kamyki[x][y] = 8;
      blitCopyMask(s_pTiles, 0, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x39)
    {
      kamyki[x][y] = 9;
      blitCopyMask(s_pTiles, 32, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x45)
    {
      kamyki[x][y] = 10;
      blitCopyMask(s_pTiles, 64, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x52)
    {
      kamyki[x][y] = 11;
      blitCopyMask(s_pTiles, 96, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x42)
    {
      kamyki[x][y] = 12;
      blitCopyMask(s_pTiles, 160, 256, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
    else if (ubZmienna == 0x31)
    {
      kamyki[x][y] = 1;
      falkonx = x;
      falkony = y;
      krawedzx = x;
      krawedzy = y;
      uwPosX = falkonx * 32;
      uwPosY = falkony * 32;
      //if (falkonFace == 0)
      //{
      //  blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      //}
      //else if (falkonFace == 32)
      //{
      //  blitCopyMask(s_pTiles, 160, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      //}
    }

    ++x;
    if (ubZmienna == 0x0a)
    {
      x = 0;
      ++y;
    }
  }
  fileClose(levelFile);
  systemUnuse();
}

void clearTiles(void)
{
  for (UBYTE y = 0; y < MAP_TILE_HEIGHT; ++y)
  {
    for (UBYTE x = 0; x < MAP_TILE_WIDTH; ++x)
    {
      kamyki[x][y] = 0;
    }
  }
}

void levelScore(void)
{
  BYTE thisLevelExcessCoal = coal - 1;
  for (UBYTE i = 0; i < thisLevelExcessCoal; ++i)
  {
    --coal;
    ++excesscoal;
    blitCopy(s_pHUD, 32, 0, s_pVpManager->pBack, 32, 224, 32, 32, MINTERM_COOKIE);
    sprintf(szMsg, "%d", coal);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 42, 231, HUDfontColor, FONT_COOKIE);
    blitCopy(s_pHUD, 128, 0, s_pVpManager->pBack, 128, 224, 32, 32, MINTERM_COOKIE);
    sprintf(szMsg3, "%d", excesscoal);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg3);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 130, 232, HUDfontColor, FONT_COOKIE);
    waitFrames(s_pVp, 10, uwPosY + FALCON_HEIGHT);
  }
}

void nextLevel(void)
{
  coal = 1;

  switch (level)
  {

  case 4:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo2.bm", 0);
    break;

  case 8:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo3.bm", 0);
    break;

  case 12:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo4.bm", 0);
    break;

  case 17:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo5.bm", 0);
    break;
  case 22:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo6.bm", 0);
    break;

  case LAST_LEVEL_NUMBER - 1: // ta ma byc przedostatnia

    robboMsgNr = LAST_LEVEL_NUMBER - 1;
    break;

  case LAST_LEVEL_NUMBER: // ta ma byc ostatnia

    robboMsgNr = LAST_LEVEL_NUMBER;
    break;
  }
  clearTiles();
  blitCopy(s_pBg, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128, MINTERM_COPY);
  blitCopy(s_pBg, 0, 128, s_pVpManager->pBack, 0, 128, 320, 128, MINTERM_COPY);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COPY);
  printOnHUD();
  drawTiles();
}

void czyRamka(void)
{
  // tu jest funkcja sprawdzajaca czy sie chcemy wypierdolic za ekran i nie pozwalajaca na to
  switch (kierunek)
  {
  case 1:
    krawedzx = krawedzx + 1;
    if (krawedzx == 10)
    {
      krawedzx = 9;
      falkonx = 9;
      frameHit = 1;
    }
    break;
  case 2:
    krawedzx = krawedzx - 1;
    if (krawedzx == -1)
    {
      krawedzx = 0;
      falkonx = 0;
      frameHit = 1;
    }
    break;
  case 3:
    krawedzy = krawedzy - 1;
    if (krawedzy == -1)
    {
      krawedzy = 0;
      falkony = 0;
      frameHit = 1;
    }
    break;
  case 4:
    krawedzy = krawedzy + 1;
    if (krawedzy == 7)
    {
      krawedzy = 6;
      falkony = 6;
      frameHit = 1;
    }
    break;
  }
}

void isThisStone(void)
{
  // funkcja sprawdzajaca przed wykonaniem ruchu czy chcemy wleciec w kamien

  BYTE stoneX = 0;
  BYTE stoneY = 0;

  switch (kierunek)
  {
  case 1:
    stoneX = falkonx + 1;
    if (kamyki[stoneX][falkony] == 3)
    {
      stoneHit = 1;
    }
    break;
  case 2:
    stoneX = falkonx - 1;
    if (kamyki[stoneX][falkony] == 3)
    {
      stoneHit = 1;
    }
    break;
  case 3:
    stoneY = falkony - 1;
    if (kamyki[falkonx][stoneY] == 3)
    {
      stoneHit = 1;
    }
    break;
  case 4:
    stoneY = falkony + 1;
    if (kamyki[falkonx][stoneY] == 3)
    {
      stoneHit = 1;
    }
    break;
  }
}

void portalAnim(void)
{

  UWORD uwPosX = falkonx * 32;
  UWORD uwPosY = falkony * 32;
  if (falkonFace == 0)
  {
    for (BYTE i = 0; i < 4; ++i)
    {
      for (BYTE k = 0; k < 4; ++k)
      {
        waitFrames(s_pVp, 1, uwPosY + FALCON_HEIGHT);
      }
      blitCopy(s_pBg, uwPosX, uwPosY, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
      blitCopyMask(s_pTiles, i * 32, 128, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
  }
  else if (falkonFace == 32)
  {
    for (BYTE i = 0; i < 4; ++i)
    {
      for (BYTE k = 0; k < 4; ++k)
      {
        waitFrames(s_pVp, 1, uwPosY + FALCON_HEIGHT);
      }
      blitCopy(s_pBg, uwPosX, uwPosY, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
      blitCopyMask(s_pTiles, i * 32, 160, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
  }
}

void robboScrollUp(void)
{
  {
    blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 248, 320, 8, MINTERM_COOKIE);
    waitFrames(s_pVp, 3, 0);
    blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 240, 320, 16, MINTERM_COOKIE);
    waitFrames(s_pVp, 3, 0);
    blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 232, 320, 24, MINTERM_COOKIE);
    waitFrames(s_pVp, 3, 0);
    blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);

    // waitFrames(s_pVp, 3, 0);
  }
}

void robboScrollDown(void)
{

  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 8, MINTERM_COOKIE);
  blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 232, 320, 24, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, 0);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 16, MINTERM_COOKIE);
  blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 240, 320, 16, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, 0);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
  blitCopy(s_pRobbo, 0, 0, s_pVpManager->pBack, 0, 248, 320, 8, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, 0);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, 0);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
}

void robboSays(void)
{
  switch (robboMsgNr)
  {
  case 0:
    sprintf(szRobboMsg, "Follow the Atari portal.");
    break;
  case 1:
    sprintf(szRobboMsg, "Keep an eye on your coal supplies.");
    break;
  case 2:
    sprintf(szRobboMsg, "Don't waste our coal hitting the meteorites.");
    break;
  case 3:
    sprintf(szRobboMsg, "Try to steal some red and blue capacitors.");
    break;
  case 4:
    sprintf(szRobboMsg, "Infiltrate the Amigans territory.");
    break;
  case 5:
    sprintf(szRobboMsg, "Minister Renton is counting on you, Sir.");
    break;
  case 6:
    sprintf(szRobboMsg, "Please clean up here, I found some GermZ.");
    break;
  case 7:
    sprintf(szRobboMsg, "Take me home, this place sucks!");
    break;
  case 8:
    sprintf(szRobboMsg, "Find the coal warehouse and reclaim it.");
    break;

  case 9:
    sprintf(szRobboMsg, "Have you played Aminer yet?");
    break;

  case 10:
    sprintf(szRobboMsg, "Drop me out at LK Avalon please.");
    break;

  case 11:
    sprintf(szRobboMsg, "You like Paula? I love Laura!");
    break;

  case 12:
    sprintf(szRobboMsg, "Atari has no glitches...");
    break;

  case 13:
    sprintf(szRobboMsg, "..even if played in 2077.");
    break;
  case 14:
    sprintf(szRobboMsg, "Make River Raid not Bridge Strike!");
    break;
  case 15:
    sprintf(szRobboMsg, "Cytadela is better than Doom.");
    break;
  case 16:
    sprintf(szRobboMsg, "How to double the value of your Atari...?");
    break;
  case 17:
    sprintf(szRobboMsg, "...Just insert the cassette into tape drive.");
    break;

  case LAST_LEVEL_NUMBER - 1:
    sprintf(szRobboMsg, "We're close, I feel it in my DSP.");
    break;
  case LAST_LEVEL_NUMBER:
    sprintf(szRobboMsg, "Well done! Now collect the coal and GTFO !!!");
    break;
  }
  fontFillTextBitMap(s_pFont, s_pBmText, szRobbo1stLine);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 8, 230, 23, FONT_COOKIE);
  fontFillTextBitMap(s_pFont, s_pBmText, szRobboMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 8, 240, 23, FONT_COOKIE);
}

void coalAndCollect(void)
{
  //funkcja do zbierania zasobu jesli jest na danym tajlu

  BYTE pickSthX = 0;
  BYTE pickSthY = 0;

  pickSthX = falkonx;
  pickSthY = falkony;

  BYTE what = kamyki[pickSthX][pickSthY];
  kamyki[pickSthX][pickSthY] = 0;

  --coal;

  switch (what)
  {
  case 4:
    coal = coal + 2;
    break;

  case 5:
    coal = coal + 3;
    break;

  case 6:
    coal = coal + 4;
    break;

  case 7:
    coal = coal + 5;
    break;

  case 8:
    capacitors = capacitors + 2;
    blitCopy(s_pHUD, 188, 0, s_pVpManager->pBack, 188, 224, 32, 32, MINTERM_COOKIE);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 190, 236, HUDfontColor, FONT_COOKIE);
    break;

  case 9:
    capacitors = capacitors + 4;
    blitCopy(s_pHUD, 188, 0, s_pVpManager->pBack, 188, 224, 32, 32, MINTERM_COOKIE);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 190, 236, HUDfontColor, FONT_COOKIE);
    break;

  case 10:
    levelScore();
    portalAnim();

    ++level;
    if (level == LAST_LEVEL_NUMBER + 1)
    {
      ptplayerStop();
      youWin = 1;
    }
    else
    {
      nextLevel();
    }
    break;

  case 11:

    ++robboMsgCount;
    robboMsgCtrl = 1;
    robboScrollUp();
    robboSays();
    ++robboMsgNr;
    return;
    break;
  
  case 12:
  amiganium = 192;
  amiganiumIdle = 320;
  amigaMode = 1;
  bitmapDestroy(s_pHUD);
  s_pHUD = bitmapCreateFromFile("data/amiHUD.bm", 0);
  portalAnim();
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
  printOnHUD();
  return;
  break;
  }
  printOnHUD();
}

void falkonHittingStone(void)
{
  UWORD HitControl = 0;
  UWORD YAnimRow = 0;
  uwPosX = falkonx * 32;
  uwPosY = falkony * 32;

  blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE);

  UWORD *pAnim;

  for (BYTE i = 0; i < ANIM_FRAME_COUNT; ++i)
  {
    if (falkonFace == 0)
    {
      pAnim = pAnimR;
      YAnimRow = 64 + amiganium;
    }
    else if (falkonFace == 32)
    {
      pAnim = pAnimL;
      YAnimRow = 96 + amiganium;
    }

    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE); // rysuje tlo ze zmeinnej
    switch (kierunek)
    {
    case 1:

      if (HitControl == 0)
      {
        ++uwPosX;
        HitControl = 2;
      }
      else if (HitControl == 1)
      {
        --uwPosX;
        HitControl = 0;
      }
      else if (HitControl == 2)
      {
        HitControl = 1;
      }
      break;

    case 2:

      if (HitControl == 0)
      {
        ++uwPosX;
        HitControl = 2;
      }
      else if (HitControl == 1)
      {
        --uwPosX;
        HitControl = 0;
      }
      else if (HitControl == 2)
      {
        HitControl = 1;
      }
      break;

    case 3:

      if (HitControl == 0)
      {
        ++uwPosY;
        HitControl = 2;
      }
      else if (HitControl == 1)
      {
        --uwPosY;
        HitControl = 0;
      }
      else if (HitControl == 2)
      {
        HitControl = 1;
      }
      break;

    case 4:
      if (HitControl == 0)
      {
        ++uwPosY;
        HitControl = 2;
      }
      else if (HitControl == 1)
      {
        --uwPosY;
        HitControl = 0;
      }
      else if (HitControl == 2)
      {
        HitControl = 1;
      }
      break;
    }

    blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE);                                  // fragment tla wrzuca do zmiennej
    blitCopyMask(s_pTiles, pAnim[i], YAnimRow, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
    waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
  }
}

void falkonFlying(void)
{
  UWORD YAnimRow = 0;
  uwPosX = falkonx * 32;
  uwPosY = falkony * 32;
  blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
  waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);

  UWORD *pAnim;
  if (falkonFace == 0)
  {
    pAnim = pAnimR;
  }
  else if (falkonFace == 32)
  {
    pAnim = pAnimL;
  }

  for (BYTE i = 0; i < ANIM_FRAME_COUNT; ++i)
  {
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE); // rysuje tlo ze zmeinnej
    switch (kierunek)
    {
    case 1:
      YAnimRow = 64 + amiganium;
      ++uwPosX;
      ++uwPosX;
      break;
    case 2:
      YAnimRow = 96 + amiganium;
      --uwPosX;
      --uwPosX;
      break;
    case 3:
      --uwPosY;
      --uwPosY;
      switch (falkonFace)
      {
      case 0:
        YAnimRow = 64 + amiganium;
        break;
      case 32:
        YAnimRow = 96 + amiganium;
        break;
      }
      break;
    case 4:
      ++uwPosY;
      ++uwPosY;
      switch (falkonFace)
      {
      case 0:
        YAnimRow = 64 + amiganium;
        break;
      case 32:
        YAnimRow = 96 + amiganium;
        break;
      }
      break;
    }

    blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);                                  // fragment tla wrzuca do zmiennej
    blitCopyMask(s_pTiles, pAnim[i], YAnimRow, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
    waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
  }
  if (robboMsgCtrl == 1)
  {
    robboMsgCtrl = 0;
    robboScrollDown();
  }
}

void falconMove(void)
{

  // jesli byl kamien to brak ruchu
  if (stoneHit == 1)
  {
    switch (kierunek)
    {
    case 1:
      krawedzx = krawedzx - 1;
      break;
    case 2:
      krawedzx = krawedzx + 1;
      break;
    case 3:
      krawedzy = krawedzy + 1;
      break;
    case 4:
      krawedzy = krawedzy - 1;
      break;
    }

    falkonHittingStone();
    stoneHit = 0;
    return;
  }

  if (frameHit == 1)
  {
    falkonHittingStone();
    frameHit = 0;
    return;
  }

  switch (kierunek)
  {

  case 1:
    falkonFace = 0;
    falkonFlying();
    falkonx = falkonx + 1;
    break;

  case 2:
    falkonFace = 32;
    falkonFlying();
    falkonx = falkonx - 1;
    break;

  case 3:
    falkonFlying();
    falkony = falkony - 1;
    break;

  case 4:
    falkonFlying();
    falkony = falkony + 1;
    break;
  }
}


void stateGameCreate(void)
{
  // Here goes your startup code
  //-------------------------------------------------------------- gdzie� w create
  s_pView = viewCreate(0,
                       TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK, // spos�b u�ywania coppera - ten jest najprostszy bo nic z nim nie musisz wi�cej robi�
                       TAG_VIEW_GLOBAL_CLUT, 1,                  // globalna paleta dla wszystkich viewport�w
                       TAG_END                                   // wi�cej argument�w nie ma
  );

  s_pVp = vPortCreate(0,
                      TAG_VPORT_VIEW, s_pView, // parent view
                      TAG_VPORT_BPP, 5,        // bits per pixel: 4bpp = 16col, 5pp = 32col, etc.
                      TAG_END);

  ptplayerCreate(1);
  s_pMod = ptplayerModCreate("data/mod.falkon");
  ptplayerLoadMod(s_pMod, 0, 0);

  // Paleta z falkona
  paletteLoad("data/falkon.plt", s_pVp->pPalette, 32);

  g_pCustom->color[0] = 0x0FFF; // zmie� kolor zero aktualnie u�ywanej palety na 15,15,15

  s_pTiles = bitmapCreateFromFile("data/tileset.bm", 0);          // z pliku tileset.bm, nie lokuj bitmapy w pami�ci FAST
  s_pTilesMask = bitmapCreateFromFile("data/tileset_mask.bm", 0); // z pliku tileset_mask.bm, nie lokuj bitmapy w pami�ci FAST
  s_pBg = bitmapCreateFromFile("data/tlo1.bm", 0);                // fragmenty tla do podstawiania po ruchu
  s_pHUD = bitmapCreateFromFile("data/HUD.bm", 0);
  s_pRobbo = bitmapCreateFromFile("data/falkon_robbo.bm", 0);

  s_pFalconBg = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight); // bitmapa robocza długa na 200px, wysoka na jedną linię tekstu

  // proste wy�wietlanie bitmapy na viewporcie
  s_pVpManager = simpleBufferCreate(0,
                                    TAG_SIMPLEBUFFER_VPORT, s_pVp,                              // parent viewport
                                    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst�pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
                                    TAG_SIMPLEBUFFER_IS_DBLBUF, 0,                              // nie potrzebujemy double buffering
                                    TAG_SIMPLEBUFFER_BOUND_WIDTH, 320 + 16,
                                    TAG_END);

  cameraSetCoord(s_pVpManager->pCamera, 0, 0);

  // po zrobieniu simpleBufferCreate()
  bitmapLoadFromFile(s_pVpManager->pBack, "data/tlo1.bm", 0, 0); // wczytaj zawarto�� bg1.bm bezpo�rednio do bitmapy bufora ekranu, zaczynaj�c od pozycji 0,0
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE);
  joyOpen(); // b�dziemy u�ywa� d�oja w grze
  keyCreate();
  // na koniec create:

  randInit(1337);

  // Draw grid
  //for(UBYTE i = 0; i < 10; ++i) {
  //  blitLine(s_pBg, i * 32, 0, i*32, 256, 5, FF, 0);
  //}
  //for(UBYTE i = 0; i < 8; ++i) {
  //  blitLine(s_pBg, 0, i * 32, 320, i * 32, 5, FF, 0);
  //}
  //blitCopyAligned(s_pBg, 0, 0, s_pVpManager->pBack, 0, 0, 320, 256 / 2);
  //blitCopyAligned(s_pBg, 0, 256 / 2, s_pVpManager->pBack, 0, 256 / 2, 320, 256 / 2 - 32);

  printOnHUD();

  viewProcessManagers(s_pView);
  viewLoad(s_pView);

  blitCopy(s_pBg, 0, 0, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, 0, 0, 33, 32, MINTERM_COOKIE);

  // narysujmy falkona
  // blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx, falkony, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

  drawTiles();
  ptplayerEnableMusic(1);
  systemUnuse(); // system w trakcie loop nie jest nam potrzebny
}

void stateGameLoop(void)
{
  // Here goes code done each game frame
  ++falkonIdle;

  if (falkonIdle == 10)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 0, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 20)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 32, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 30)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 64, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 40)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 96, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 50)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 128, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 60)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 96, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 70)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 64, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
  }
  else if (falkonIdle == 80)
  {
    blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 32, 32, MINTERM_COOKIE);
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, MINTERM_COOKIE);
    blitCopyMask(s_pTiles, 32, amiganiumIdle + falkonFace, s_pVpManager->pBack, falkonx * 32, falkony * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    falkonIdle = 0;
  }

  joyProcess();
  keyProcess();

  kierunek = 0;
  if (joyUse(JOY1_RIGHT) || keyUse(KEY_D) || keyUse(KEY_RIGHT))
  {
    kierunek = 1;
    falkonFace = 0;
  }
  else if (joyUse(JOY1_LEFT) || keyUse(KEY_A) || keyUse(KEY_LEFT))
  {
    kierunek = 2;
    falkonFace = 32;
  }
  else if (joyUse(JOY1_UP) || keyUse(KEY_W) || keyUse(KEY_UP))
  {
    kierunek = 3;
  }
  else if (joyUse(JOY1_DOWN) || keyUse(KEY_S) || keyUse(KEY_DOWN))
  {
    kierunek = 4;
  }
  else if (keyUse(KEY_ESCAPE))
  {
    ptplayerStop();
    clearTiles();
    cleanUp();
    stateChange(g_pStateMachineGame, &g_sStateMenu);
    return;
  }
  else if (keyUse(KEY_N))
  {
    ++level;
    nextLevel();
    return;
  }
  else if (keyUse(KEY_M))
  {
    if (musicPlay == 1)
    {
      musicPlay = 0;
      ptplayerSetMasterVolume(0);
    }
    else if (musicPlay == 0)
    {
      musicPlay = 1;
      ptplayerSetMasterVolume(64);
    }
  }

  if (kierunek != 0)
  {
    isThisStone();
    czyRamka();
    falconMove();
    coalAndCollect();
  }

  if (amigaMode == 1){     // jesli zebralem ami-kondka to wyswietlam ekran z plot twistem
    amigaMode = 2;   // ustawiam dla sprawdzenia na koniec czy bedzie alternatywne zakonczenie
   // ptplayerStop();
   statePush(g_pStateMachineGame, &g_sStateGuruMastah);
   return;
   }

  if (youWin == 1)   // sprawdzenie ktore zakonczenie uruchomic
  {
    youWin = 0;
    if (amigaMode == 0){    
    stateChange(g_pStateMachineGame, &g_sStateScore);  // atari ending
    return;
    }
    else if (amigaMode == 2){
    stateChange(g_pStateMachineGame, &g_sStateScoreAmi);  // amiga ending
    return; 
    }
  }

  if (coal == 0)
  {
    coal = 1;
    ptplayerStop();
    stateChange(g_pStateMachineGame, &g_sStateGameOver);
    return;
  }



  viewProcessManagers(s_pView);      // obliczenia niezb�dne do poprawnego dzia�ania viewport�w
  copProcessBlocks();                // obliczenia niezb�dne do poprawnego dzia�ania coppera
  vPortWaitForPos(s_pVp, uwPosY, 0); // r�wnowa�ne amosowemu wait vbl
}

void stateGameDestroy(void)
{
  // Here goes your cleanup code
  systemUse(); // w��cz grzecznie system
  ptplayerModDestroy(s_pMod);
  ptplayerDestroy();
  bitmapDestroy(s_pTiles);
  bitmapDestroy(s_pTilesMask);
  bitmapDestroy(s_pBg);
  bitmapDestroy(s_pHUD);
  bitmapDestroy(s_pFalconBg);

  fontDestroy(s_pFont);
  fontDestroyTextBitMap(s_pBmText);

  viewDestroy(s_pView); // zwolnij z pami�ci view, wszystkie do��czone do niego viewporty i wszystkie do��czone do nich mened�ery
  joyClose();
  keyDestroy();
}

// pod funkcjami bo kompilator czyta od g�ry do do�u i musi najpierw napotka� funkcje by wiedzie� �e istniej�
tState g_sStateGame = {
    .cbCreate = stateGameCreate,
    .cbLoop = stateGameLoop,
    .cbDestroy = stateGameDestroy,
    .cbSuspend = 0,
    .cbResume = gameOnResume,
    .pPrev = 0};
