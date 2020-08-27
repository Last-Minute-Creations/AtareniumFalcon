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

static tPtplayerMod *s_pMod;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

extern tState g_sStateMenu;
extern tState g_sStateGameOver;
extern tState g_sStateScore;
extern tState g_sStateRobbo;
extern tStateManager *g_pStateMachineGame;

#define MAP_TILE_HEIGHT 7
#define MAP_TILE_WIDTH 10
#define FALCON_HEIGHT 32

char szMsg[50];  // do wyswietlania wegla na HUD
char szMsg2[50]; // do wyswietlania kondkow na HUD
char levelFilePath[20];

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

BYTE stoneHit = 0;
BYTE frameHit = 0;

CONST BYTE startingCoal = 1;

BYTE coal = startingCoal;
BYTE capacitors = 0;
BYTE level = 1;
BYTE robboMsgNr = 0;

void cleanUp();

void coalDecrementAndPrintOnHUD(void)
{
  coal = coal - 1;
  blitCopy(s_pHUD, 32, 0, s_pVpManager->pBack, 32, 224, 32, 32, MINTERM_COOKIE, 0xFF);
  sprintf(szMsg, "%d", coal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 42, 231, 5, FONT_COOKIE);
}

void waitFrames(tVPort *pVPort, UBYTE ubHowMany, UWORD uwPosY)
{
  for (UBYTE i = 0; i < ubHowMany; ++i)
  {
    vPortWaitForPos(pVPort, uwPosY);
  }
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
      ubStoneImg = ubRandMinMax(0, 2);
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
    else if (ubZmienna == 0x31)
    {
      kamyki[x][y] = 1;
      falkonx = x;
      falkony = y;
      krawedzx = x;
      krawedzy = y;
      if (falkonFace == 0)
      {
        blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      }
      else if (falkonFace == 32)
      {
        blitCopyMask(s_pTiles, 160, 32, s_pVpManager->pBack, x * 32, y * 32, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
      }
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

void printOnHUD(void)
{
  sprintf(szMsg, "%d", coal);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 42, 231, 5, FONT_COOKIE);

  sprintf(szMsg2, "%d", capacitors);
  fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
  fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 106, 231, 5, FONT_COOKIE);
}

void nextLevel(void)
{
  switch (level)
  {
  case 2:
    robboMsgNr = 1;
    break;
  case 3:
    robboMsgNr = 2;
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo2.bm", 0);
    break;
  case 4:
    robboMsgNr = 3;
    break;
  case 5:
    robboMsgNr = 4;
    break;
  case 6:
    robboMsgNr = 5;
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo3.bm", 0);
    break;
  case 7:
    robboMsgNr = 6;
    break;
  case 9:
    robboMsgNr = 7;
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo4.bm", 0);
    break;
  case 10:
    robboMsgNr = 8;

    break;
  case 11:
    robboMsgNr = 9;
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo5.bm", 0);
    break;
  case 13:
    bitmapDestroy(s_pBg);
    s_pBg = bitmapCreateFromFile("data/tlo6.bm", 0);
    break;
  case 15: // ta ma byc ostatnia

    robboMsgNr = 10;
    break;
  }
  clearTiles();
  blitCopy(s_pBg, 0, 0, s_pVpManager->pBack, 0, 0, 320, 128, MINTERM_COPY, 0xFF);
  blitCopy(s_pBg, 0, 128, s_pVpManager->pBack, 0, 128, 320, 128, MINTERM_COPY, 0xFF);
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COPY, 0xFF);
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
        waitFrames(s_pVp, 2, uwPosY + FALCON_HEIGHT);
      }
      blitCopy(s_pBg, uwPosX, uwPosY, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE, 0xFF);
      blitCopyMask(s_pTiles, i * 32, 128, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
  }
  else if (falkonFace == 32)
  {
    for (BYTE i = 0; i < 4; ++i)
    {
      for (BYTE k = 0; k < 4; ++k)
      {
        waitFrames(s_pVp, 2, uwPosY + FALCON_HEIGHT);
      }
      blitCopy(s_pBg, uwPosX, uwPosY, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE, 0xFF);
      blitCopyMask(s_pTiles, i * 32, 160, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
  }
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
    blitCopy(s_pHUD, 96, 0, s_pVpManager->pBack, 96, 224, 32, 32, MINTERM_COOKIE, 0xFF);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 106, 231, 5, FONT_COOKIE);
    break;

  case 9:
    capacitors = capacitors + 4;
    blitCopy(s_pHUD, 96, 0, s_pVpManager->pBack, 96, 224, 32, 32, MINTERM_COOKIE, 0xFF);
    sprintf(szMsg2, "%d", capacitors);
    fontFillTextBitMap(s_pFont, s_pBmText, szMsg2);
    fontDrawTextBitMap(s_pVpManager->pBack, s_pBmText, 106, 231, 5, FONT_COOKIE);
    break;

  case 10:
    portalAnim();
    ++level;
    if (level == 16)
    {
      ptplayerStop();
      stateChange(g_pStateMachineGame, &g_sStateScore);
      return;
    }
    nextLevel();
    break;

  case 11:
    coalDecrementAndPrintOnHUD();
    mt_mastervol(0);
    statePush(g_pStateMachineGame, &g_sStateRobbo);
    return;
    break;
  }
  coalDecrementAndPrintOnHUD();
}

void falkonFlying(void)
{
  UWORD uwPosX = falkonx * 32;
  UWORD uwPosY = falkony * 32;
  blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF);
  waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE, 0xFF);

  for (BYTE i = 0; i < 16; ++i)
  {
    blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE, 0xFF); // rysuje tlo ze zmeinnej
    switch (kierunek)
    {
    case 1:
      ++uwPosX;
      ++uwPosX;                                                                                                             // 2px w prawo
      blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF);                       // fragment tla wrzuca do zmiennej
      blitCopyMask(s_pTiles, pAnimR[i], 64, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
      waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
      break;
    case 2:
      --uwPosX;
      --uwPosX;                                                                                                             // 2px w prawo
      blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF);                       // fragment tla wrzuca do zmiennej
      blitCopyMask(s_pTiles, pAnimL[i], 96, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
      waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
      break;
    case 3:
      --uwPosY;
      --uwPosY;                                                                                       // 2px w prawo
      blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF); // fragment tla wrzuca do zmiennej
      switch (falkonFace)
      {
      case 0:
        blitCopyMask(s_pTiles, pAnimR[i], 64, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
        break;
      case 32:
        blitCopyMask(s_pTiles, pAnimL[i], 96, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
        break;
      }
      waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);

      break;
    case 4:
      ++uwPosY;
      ++uwPosY;                                                                                       // 2px w prawo
      blitCopy(s_pVpManager->pBack, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF); // fragment tla wrzuca do zmiennej
      switch (falkonFace)
      {
      case 0:
        blitCopyMask(s_pTiles, pAnimR[i], 64, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]); // rysuje falkona
        break;
      case 32:
        blitCopyMask(s_pTiles, pAnimL[i], 96, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
        break;
      }
      waitFrames(s_pVp, 3, uwPosY + FALCON_HEIGHT);
      break;

      blitCopy(s_pBg, uwPosX, uwPosY, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF);
      blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, uwPosX, uwPosY, 33, 32, MINTERM_COOKIE, 0xFF);
      blitCopyMask(s_pTiles, 0, 64, s_pVpManager->pBack, uwPosX, uwPosY, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);
    }
  }
}

void falconMove(void)
{

  // jesli byl kamien to brak ruchu
  if (stoneHit == 1)
  {
    stoneHit = 0;
    return;
  }

  if (frameHit == 1)
  {
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

void noCoalLeft(void)
{
  // sprawdzenie warunku na game over

  if (coal == 0)
  {
    coal = 1;
    ptplayerStop();
    stateChange(g_pStateMachineGame, &g_sStateGameOver);
    return;
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
  s_pFalconBg = bitmapCreate(48, 32, 5, BMF_INTERLEAVED);

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(200, s_pFont->uwHeight); // bitmapa robocza długa na 200px, wysoka na jedną linię tekstu

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
  blitCopy(s_pHUD, 0, 0, s_pVpManager->pBack, 0, 224, 320, 32, MINTERM_COOKIE, 0xFF);
  joyOpen(); // b�dziemy u�ywa� d�oja w grze
  keyCreate();
  // na koniec create:

  randInit(1337);

  // Draw grid
  //for(UBYTE i = 0; i < 10; ++i) {
  //  blitLine(s_pBg, i * 32, 0, i*32, 256, 5, 0xFFFF, 0);
  //}
  //for(UBYTE i = 0; i < 8; ++i) {
  //  blitLine(s_pBg, 0, i * 32, 320, i * 32, 5, 0xFFFF, 0);
  //}
  //blitCopyAligned(s_pBg, 0, 0, s_pVpManager->pBack, 0, 0, 320, 256 / 2);
  //blitCopyAligned(s_pBg, 0, 256 / 2, s_pVpManager->pBack, 0, 256 / 2, 320, 256 / 2 - 32);

  printOnHUD();

  viewProcessManagers(s_pView);
  viewLoad(s_pView);

  blitCopy(s_pBg, 0, 0, s_pFalconBg, 0, 0, 48, 32, MINTERM_COOKIE, 0xFF);
  blitCopy(s_pFalconBg, 0, 0, s_pVpManager->pBack, 0, 0, 33, 32, MINTERM_COOKIE, 0xFF);

  // narysujmy falkona
  // blitCopyMask(s_pTiles, 128, 32, s_pVpManager->pBack, falkonx, falkony, 32, 32, (UWORD *)s_pTilesMask->Planes[0]);

  drawTiles();
  ptplayerEnableMusic(1);
  systemUnuse(); // system w trakcie loop nie jest nam potrzebny
}

void stateGameLoop(void)
{
  // Here goes code done each game frame

  //--------------------------------------------------------------- gdzie� w loop:
  joyProcess();
  keyProcess();

  kierunek = 0;
  if (joyUse(JOY1_RIGHT) || keyUse(KEY_D) || keyUse(KEY_RIGHT))
  {
    kierunek = 1;
  }
  else if (joyUse(JOY1_LEFT) || keyUse(KEY_A) || keyUse(KEY_LEFT))
  {
    kierunek = 2;
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
      mt_mastervol(0);
    }
    else if (musicPlay == 0)
    {
      musicPlay = 1;
      mt_mastervol(64);
    }
  }

  if (kierunek != 0)
  {
    isThisStone();
    czyRamka();
    falconMove();
    coalAndCollect();
  }

  noCoalLeft();

  viewProcessManagers(s_pView); // obliczenia niezb�dne do poprawnego dzia�ania viewport�w
  copProcessBlocks();           // obliczenia niezb�dne do poprawnego dzia�ania coppera
  vPortWaitForEnd(s_pVp);       // r�wnowa�ne amosowemu wait vbl
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
